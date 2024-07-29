#include "Env.h"

namespace RLGSC {

	template<bool PlayerFlags::* DATA_VAR>
	void TogglePlayerFlag(Car* car, void* userInfo) {
		if (!car)
			return;

		Env* env = (Env*)userInfo;
		for (auto& player : env->prevState.players)
			if (player.carId == car->id)
				(player.flags.*DATA_VAR) = true;
	}

	void _ShotEventCallback(Arena* arena, Car* shooter, Car* passer, void* userInfo) {
		TogglePlayerFlag<&PlayerFlags::shot>(shooter, userInfo);
		TogglePlayerFlag<&PlayerFlags::shotPass>(passer, userInfo);
	}

	void _GoalEventCallback(Arena* arena, Car* scorer, Car* passer, void* userInfo) {
		TogglePlayerFlag<&PlayerFlags::playerGoal>(scorer, userInfo);
		TogglePlayerFlag<&PlayerFlags::assist>(passer, userInfo);
	}

	void _SaveEventCallback(Arena* arena, Car* saver, void* userInfo) {
		TogglePlayerFlag<&PlayerFlags::save>(saver, userInfo);
	}

	void _BumpCallback(Arena* arena, Car* bumper, Car* victim, bool isDemo, void* userInfo) {
		if (bumper->team == victim->team)
			return;

		TogglePlayerFlag<&PlayerFlags::bump>(bumper, userInfo);
		TogglePlayerFlag<&PlayerFlags::bumped>(victim, userInfo);

		if (isDemo) {
			TogglePlayerFlag<&PlayerFlags::demo>(bumper, userInfo);
			TogglePlayerFlag<&PlayerFlags::demoed>(victim, userInfo);
		}
	}

	Env::Env(const EnvConfig& cfg) : tickSkip(cfg.tickSkip) {

		RG_ASSERT(cfg.rewardFn != NULL);
		RG_ASSERT(cfg.obsBuilder != NULL);
		RG_ASSERT(cfg.actionParser != NULL);
		RG_ASSERT(cfg.stateSetter != NULL);

		match = Match(
			cfg.rewardFn,
			cfg.terminalConditions,
			cfg.obsBuilder,
			cfg.actionParser,
			cfg.stateSetter,
			cfg.teamSize,
			cfg.spawnOpponents
		);

		tickSkip = cfg.tickSkip;

		if (cfg.customActionTickDelay == -1) {
			actionDelay = tickSkip - 1;
		} else {
			RG_ASSERT(cfg.customActionTickDelay >= 0 && cfg.customActionTickDelay <= tickSkip);
			actionDelay = cfg.customActionTickDelay;
		}

		arena = Arena::Create(cfg.gameMode);
		arena->SetMutatorConfig(cfg.mutatorConfig);

		for (int i = 0; i < match.teamSize; i++) {
			arena->AddCar(Team::BLUE, cfg.carConfig);
			if (match.spawnOpponents)
				arena->AddCar(Team::ORANGE, cfg.carConfig);
		}

		eventTracker.SetShotCallback(_ShotEventCallback, this);
		eventTracker.SetGoalCallback(_GoalEventCallback, this);
		eventTracker.SetSaveCallback(_SaveEventCallback, this);

		arena->SetCarBumpCallback(_BumpCallback, this);
	}

	FList2 Env::Reset() {
		GameState resetState = match.ResetState(arena);
		match.EpisodeReset(resetState);
		prevState = resetState;
		eventTracker.ResetPersistentInfo();
		isEpisodeStart = true;

		FList2 obs = match.BuildObservations(resetState);
		return obs;
	}

	Env::StepResult Env::Step(const ActionParser::Input& actionsData) {

		prevState.prev = NULL;
		ActionSet actions = match.ParseActions(actionsData, prevState);

		GameState state;

		{ // Step arena with actions
			auto carItr = arena->_cars.begin();
			for (int i = 0; i < actions.size(); i++) {
				(*carItr)->controls = (CarControls)actions[i];
				carItr++;
			}

			// Clear all flags
			for (auto& player : prevState.players)
				player.flags = {};

			arena->Step(tickSkip - actionDelay);
			if (arena->gameMode != GameMode::HEATSEEKER)
				eventTracker.Update(arena);
			state = prevState; // All callbacks have been hit
			state.UpdateFromArena(arena, isEpisodeStart ? NULL : &prevState);
			arena->Step(actionDelay);
			totalTicks += tickSkip;
			totalSteps++;
		}

		bool done = match.IsDone(state);
		state.isTerminal = done;

		FList2 obs = match.BuildObservations(state);
		FList rewards = match.GetRewards(state);
		prevState = state;

		isEpisodeStart = false;

		return StepResult {
			obs,
			rewards,
			done,
			state
		};
	}
}