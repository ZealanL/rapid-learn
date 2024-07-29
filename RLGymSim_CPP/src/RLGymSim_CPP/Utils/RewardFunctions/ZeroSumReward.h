#pragma once
#include "RewardFunction.h"

namespace RLGSC {
	// This is a wrapper class that makes another reward function zero-sum and team-distributed
	// Per-player reward is calculated using: ownReward*(1-teamSpirit) + avgTeamReward*teamSpirit - avgOpponentReward
	class ZeroSumReward : public RewardFunction {
	public:

		RG_SHR(RewardFunction) childFunc;
		bool ownsFunc;

		float teamSpirit, opponentScale;

		// Team spirit is the fraction of reward shared between teammates
		// Opponent scale is the scale of punishment for opponent rewards (normally 1, non-1 is no longer zero-sum)
		ZeroSumReward(RG_SHR(RewardFunction) childFunc, float teamSpirit, float opponentScale = 1)
			: childFunc(childFunc), teamSpirit(teamSpirit), opponentScale(opponentScale) {

		}

		~ZeroSumReward() = default;

	protected:
		virtual void Reset(const GameState& initialState) {
			childFunc->Reset(initialState);
		}

		virtual void PreStep(const GameState& state) {
			childFunc->PreStep(state);
		}

		// Get all rewards for all players
		virtual std::vector<float> GetAllRewards(const GameState& state);
	};
}