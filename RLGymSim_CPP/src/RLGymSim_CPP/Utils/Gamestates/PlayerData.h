#pragma once
#include "../../Framework.h"
#include "PhysObj.h"
#include "../BasicTypes/Action.h"

namespace RLGSC {

	// Each flag is true for 1 step as the event occurs
	struct PlayerFlags {
		bool
			playerGoal, save, assist, shot, shotPass,
			bump, bumped, demo, demoed;

		PlayerFlags() = default;
	};

	// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/gamestates/player_data.py
	struct PlayerData : PhysObj {

		int index; // Index in gamestate array
		uint32_t carId;
		Team team;

		CarState carState = {};

		Action prevAction;

		PlayerFlags flags = {};

		//////////////////
		bool hasFlipOrJump;

		float boostFraction; // From 0 to 1

		bool ballTouchedStep; // True if the player touched the ball during any of tick of the step
		bool ballTouchedTick; // True if the player is touching the ball on the final tick of the step
		//////////////////

		PlayerData() {}

		void UpdateFromCar(Car* car, uint64_t tickCount, int tickSkip);

		void SyncCarState();
	};
}