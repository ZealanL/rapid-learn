#include "Collector.h"

#include "PolicyInterface.h"

using namespace RLGSC;
using namespace RPLC;
RPLC::Collector::Collector(EnvCreateFn envCreateFunc, RG_SHR(PolicyInterface) policyInterface) 
	: envCreateFunc(envCreateFunc), policyInterface(policyInterface) {
	
	RocketSim::Init("collision_meshes", true);

	auto envConfig = envCreateFunc();
	env = new Env(envConfig);
	numPlayers = env->match.playerAmount;
	RG_LOG("Env created!");

	curObs = env->Reset();
}

CollectorExp RPLC::Collector::Collect(int stepsPerEnv) {
	CollectorExp exp = {};
	
	for (int i = 0; i < stepsPerEnv; i++) {
		auto actions = policyInterface->GetActions(curObs);
		auto stepResult = env->Step(actions);

		exp.obs.push_back(curObs);
		exp.actions.push_back(actions);
		exp.rewards.push_back(stepResult.reward);
		exp.dones.push_back(std::vector<float>(actions.size(), (float)stepResult.done));

		if (stepResult.done) {
			curObs = env->Reset();
		} else {
			curObs = stepResult.obs;
		}
	}

	return exp;
}

RPLC_ADD_BIND(CollectorExp) {
#define PYB_CUR_CLASS CollectorExp
	PYB_CLASS_SH(CollectorExp)
		PYBP(obs)
		PYBP(actions)
		PYBP(rewards)
		PYBP(dones)
		;
}

RPLC_ADD_BIND(Collector) {
#define PYB_CUR_CLASS Collector
	PYB_CLASS_SH(Collector)
		.def(pyb::init<EnvCreateFn, RG_SHR(PolicyInterface)>(), PYBA("env_create_func"), PYBA("policy_interface"))
		.def(PYB_M(Collect))
		;
}