#pragma once
#include "Bindings.h"
#include "Threading/GameInst.h"
#include "PolicyInterface.h"

namespace RPLC {

	struct CollectorExp {
		std::vector<FList2> obs;
		std::vector<RLGSC::ActionParser::Input> actions;
		std::vector<FList> rewards;
		std::vector<FList> dones;
	};

	struct Collector {
		EnvCreateFn envCreateFunc;
		RG_SHR(PolicyInterface) policyInterface;

		RLGSC::FList2 curObs = {};
		RLGSC::Env* env;
		int numPlayers;

		Collector(EnvCreateFn envCreateFunc, RG_SHR(PolicyInterface) policyInterface);
		RG_NO_COPY(Collector);
		~Collector() {
			delete env;
		}

		CollectorExp Collect(int stepsPerEnv);
	};
}