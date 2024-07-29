#pragma once
#include "Match.h"

namespace RLGSC {

	struct EnvConfig {
		RG_SHR(RewardFunction) rewardFn = NULL;
		std::vector<RG_SHR(TerminalCondition)> terminalConditions = {};
		RG_SHR(ObsBuilder) obsBuilder = NULL;
		RG_SHR(ActionParser) actionParser = NULL;
		RG_SHR(StateSetter) stateSetter = NULL;

		int teamSize = 1;
		bool spawnOpponents = true;

		int tickSkip = 8;

		// Delay ticks between the chosen action being applied
		// -1 uses (tickSkip - 1), which is the same as RLGym v1 and rlgym-sim
		int customActionTickDelay = -1;

		CarConfig carConfig = CAR_CONFIG_OCTANE;
		GameMode gameMode = GameMode::SOCCAR;
		MutatorConfig mutatorConfig = MutatorConfig(GameMode::SOCCAR);
	};

	class Env {
	public:
		Arena* arena;
		GameEventTracker eventTracker;
		Match match;
		int tickSkip;
		GameState prevState;
		bool isEpisodeStart = true;
		std::vector<uint32_t> carIds;
		int actionDelay;

		int totalTicks = 0;
		int totalSteps = 0;

		Env(const EnvConfig& cfg);

		RG_NO_COPY(Env);

		virtual FList2 Reset();

		struct StepResult {
			FList2 obs;
			FList reward;
			bool done;
			GameState state;
		};
		virtual StepResult Step(const ActionParser::Input& actionsData);

		virtual ~Env() {
			delete arena;
		}
	};
}