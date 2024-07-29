#include "Collector.h"

using namespace RLGSC;
void RPLC::Collector::Run(EnvCreateFn envCreateFunc) {
	
	RocketSim::Init("collision_meshes", true);

	auto envConfig = envCreateFunc();
	Env* env = new Env(envConfig);
	RG_LOG("Env created!");
}

RPLC_ADD_BIND(Collector) {
	m.def_submodule("collector")
		.def("run", RPLC::Collector::Run, PYBA("env_create_func"))
		;
}