#pragma once
#include "../Lists.h"
#include "../Util/AvgTracker.h"
#include "../Util/Report.h"
#include <RLGymSim_CPP/Match.h>
#include <RLGymSim_CPP/Env.h>

namespace RPLC {
	typedef std::function<void(class GameInst*, const RLGSC::Env::StepResult&, Report&)> StepCallback;


	typedef std::function<RLGSC::EnvConfig()> EnvCreateFn;

	class RG_IMEXPORT GameInst {
	public:

		// This game instance is being used for evaluation instead of training
		bool isEval = false;

		RLGSC::Env* env;

		FList2 curObs;

		uint64_t totalSteps;

		float curEpRew = 0;
		AvgTracker avgStepRew, avgEpRew;

		// Will be reset every iteration, when ResetMetrics() is called
		Report _metrics = {};

		StepCallback stepCallback = NULL;

		// NOTE: Env and match will be deleted when GameInst is deleted
		GameInst(RLGSC::Env* env) : env(env) {
			totalSteps = 0;
		}

		RG_NO_COPY(GameInst);

		void ResetMetrics() {
			avgStepRew.Reset();
			avgEpRew.Reset();
			_metrics.Clear();
		}

		void Start();
		RLGSC::Env::StepResult Step(const IList& actions);

		~GameInst() {
			delete env;
		}
	};
}