#pragma once
#include "PlayerData.h"
#include "../CommonValues.h"

namespace RLGSC {
	struct ScoreLine {
		int teamGoals[2] = { 0,0 };

		int operator[](size_t index) const {
			return teamGoals[index];
		}

		int& operator[](size_t index) {
			return teamGoals[index];
		}
	};

	// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/gamestates/game_state.py
	struct GameState {
		// NOTE: Will be NULL if this is the first state of an episode
		RG_SHR(const GameState) prev = NULL;

		float deltaTime = 0; // Time that has passed since last step
		bool isTerminal = false; // If this state is the final state in the episode

		bool 
			blueScored = false, 
			orangeScored = false;

		int lastTouchCarID = -1;
		std::vector<PlayerData> players;

		BallState ballState;
		PhysObj ball, ballInv;

		std::array<bool, CommonValues::BOOST_LOCATIONS_AMOUNT> boostPads, boostPadsInv;
		std::array<float, CommonValues::BOOST_LOCATIONS_AMOUNT> boostPadTimers, boostPadTimersInv;

		// Last arena we updated with
		// Can be used to determine current arena from within reward function, for example
		// NOTE: Could be null
		Arena* lastArena = NULL;

		// Last tick count when updated
		uint64_t lastTickCount = 0;

		GameState() = default;
		explicit GameState(Arena* arena) {
			UpdateFromArena(arena, NULL);
		}

		const PhysObj& GetBallPhys(bool inverted) const {
			return inverted ? ballInv : ball;
		}

		const auto& GetBoostPads(bool inverted) const {
			return inverted ? boostPadsInv : boostPads;
		}

		// NOTE: Stored prev state will not have another prev state
		void UpdateFromArena(Arena* arena, const GameState* prev);

		void SyncAndApplyToArena(Arena* arena);
	};
}