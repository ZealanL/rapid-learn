#pragma once
#include "../Gamestates/GameState.h"
#include "../BasicTypes/Action.h"

// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/reward_functions/reward_function.py
namespace RLGSC {
	class RewardFunction {
	public:
		virtual void Reset(const GameState& state) {}

		virtual void PreStep(const GameState& state) {}

		virtual float GetReward(const PlayerData& player, const GameState& state) {
			RG_ERR_CLOSE(typeid(*this).name() << ": GetReward() is unimplemented. Either implement it or override GetAllRewards().");
			return 0;
		}

		// Get all rewards for all players
		virtual std::vector<float> GetAllRewards(const GameState& state) {

			std::vector<float> rewards = std::vector<float>(state.players.size());
			for (int i = 0; i < state.players.size(); i++) {
				rewards[i] = GetReward(state.players[i], state);
			}

			return rewards;
		}

		virtual ~RewardFunction() {};
	};
}