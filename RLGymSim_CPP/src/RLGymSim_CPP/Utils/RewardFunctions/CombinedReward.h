#pragma once
#include "RewardFunction.h"

// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/reward_functions/combined_reward.py
namespace RLGSC {
	class CombinedReward : public RewardFunction {
	public:
		std::vector<RG_SHR(RewardFunction)> rewardFuncs;
		std::vector<float> rewardWeights;
		bool ownsFuncs;

		CombinedReward(std::vector<RG_SHR(RewardFunction)> rewardFuncs, std::vector<float> rewardWeights) :
			rewardFuncs(rewardFuncs), rewardWeights(rewardWeights), ownsFuncs(ownsFuncs) {
			assert(rewardFuncs.size() == rewardWeights.size());
		}

		CombinedReward(std::vector<std::pair<RG_SHR(RewardFunction), float>> funcsWithWeights) :
			ownsFuncs(ownsFuncs) {
			for (auto& pair : funcsWithWeights) {
				rewardFuncs.push_back(pair.first);
				rewardWeights.push_back(pair.second);
			}
		}

	protected:
		virtual void Reset(const GameState& initialState) {
			for (auto func : rewardFuncs)
				func->Reset(initialState);
		}

		virtual void PreStep(const GameState& state) {
			for (auto func : rewardFuncs)
				func->PreStep(state);
		}

		virtual std::vector<float> GetAllRewards(const GameState& state) {
			std::vector<float> allRewards(state.players.size());

			for (int i = 0; i < rewardFuncs.size(); i++) {
				auto rewards = rewardFuncs[i]->GetAllRewards(state);
				for (int j = 0; j < rewards.size(); j++)
					allRewards[j] += rewards[j] * rewardWeights[i];
			}

			return allRewards;
		}
	};
}