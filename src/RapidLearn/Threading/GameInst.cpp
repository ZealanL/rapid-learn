#include "GameInst.h"

void RPLC::GameInst::Start() {
	curObs = env->Reset();
}

RLGSC::Env::StepResult RPLC::GameInst::Step(const IList& actions) {

	// Step with agent actions
	auto stepResult = env->Step(actions);

	auto& nextObs = stepResult.obs;

	{ // Update avg rewards
		float totalRew = 0;
		for (int i = 0; i < env->match.playerAmount; i++)
			totalRew += stepResult.reward[i];

		avgStepRew.Add(totalRew, env->match.playerAmount);
		curEpRew += totalRew / env->match.playerAmount;
	}

	if (stepCallback)
		stepCallback(this, stepResult, _metrics);

	// Environment ending
	if (stepResult.done) {
		nextObs = env->Reset();
		
		avgEpRew += curEpRew;
		curEpRew = 0;
	}

	curObs = nextObs;
	totalSteps++;

	return stepResult;
}