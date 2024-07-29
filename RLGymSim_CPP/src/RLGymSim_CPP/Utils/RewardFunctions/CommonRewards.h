#pragma once
#include "RewardFunction.h"

namespace RLGSC {
	// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/reward_functions/common_rewards/misc_rewards.py
	class EventReward : public RewardFunction {
	public:
#define EVENT_REWARD_WEIGHTS_INS(PFM) \
		PFM(teamGoal), PFM(concede), \
		PFM(playerGoal), PFM(save), PFM(assist), PFM(shot), PFM(shotPass), \
		PFM(bump), PFM(bumped), PFM(demo), PFM(demoed)

#define EVENT_REWARD_WEIGHT_ARG_PFM(name) float name
#define EVENT_REWARD_WEIGHT_EMPTY_PFM(name) name
#define EVENT_REWARD_WEIGHT_ARGS EVENT_REWARD_WEIGHTS_INS(EVENT_REWARD_WEIGHT_ARG_PFM)
#define EVENT_REWARD_WEIGHTS EVENT_REWARD_WEIGHTS_INS(EVENT_REWARD_WEIGHT_EMPTY_PFM)

		struct Weights {
			float
				EVENT_REWARD_WEIGHTS;
		};

		Weights weights = {};

		EventReward(Weights weights) : weights(weights) {}
		EventReward(EVENT_REWARD_WEIGHT_ARGS) {
			weights = {
				EVENT_REWARD_WEIGHTS
			};
		}

		static EventReward Make(EVENT_REWARD_WEIGHT_ARGS) {
			return EventReward(EVENT_REWARD_WEIGHTS);
		}

		virtual float GetReward(const PlayerData& player, const GameState& state);
	};

	// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/reward_functions/common_rewards/misc_rewards.py
	class VelocityReward : public RewardFunction {
	public:
		bool isNegative;
		VelocityReward(bool isNegative = false) : isNegative(isNegative) {}
		virtual float GetReward(const PlayerData& player, const GameState& state) {
			return player.vel.Length() / CommonValues::CAR_MAX_SPEED * (1 - 2 * isNegative);
		}
	};

	// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/reward_functions/common_rewards/misc_rewards.py
	class SaveBoostReward : public RewardFunction {
	public:
		float exponent;
		SaveBoostReward(float exponent = 0.5f) : exponent(exponent) {}

		virtual float GetReward(const PlayerData& player, const GameState& state) {
			return RS_CLAMP(powf(player.boostFraction, exponent), 0, 1);
		}
	};

	// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/reward_functions/common_rewards/ball_goal_rewards.py
	class VelocityBallToGoalReward : public RewardFunction {
	public:
		bool ownGoal = false;
		VelocityBallToGoalReward(bool ownGoal = false) : ownGoal(ownGoal) {}

		virtual float GetReward(const PlayerData& player, const GameState& state) {
			bool targetOrangeGoal = player.team == Team::BLUE;
			if (ownGoal)
				targetOrangeGoal = !targetOrangeGoal;

			Vec targetPos = targetOrangeGoal ? CommonValues::ORANGE_GOAL_BACK : CommonValues::BLUE_GOAL_BACK;
			
			Vec ballDirToGoal = (targetPos - state.ball.pos).Normalized();
			return ballDirToGoal.Dot(state.ball.vel / CommonValues::BALL_MAX_SPEED);
		}
	};

	// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/reward_functions/common_rewards/player_ball_rewards.py
	class VelocityPlayerToBallReward : public RewardFunction {
	public:
		virtual float GetReward(const PlayerData& player, const GameState& state) {
			Vec dirToBall = (state.ball.pos - player.pos).Normalized();
			Vec normVel = player.vel / CommonValues::CAR_MAX_SPEED;
			return dirToBall.Dot(normVel);
		}
	};

	// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/reward_functions/common_rewards/player_ball_rewards.py
	class FaceBallReward : public RewardFunction {
	public:
		virtual float GetReward(const PlayerData& player, const GameState& state) {
			Vec dirToBall = (state.ball.pos - player.pos).Normalized();
			return player.carState.rotMat.forward.Dot(dirToBall);
		}
	};

	// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/reward_functions/common_rewards/player_ball_rewards.py
	class TouchBallReward : public RewardFunction {
	public:
		float aerialWeight;
		TouchBallReward(float aerialWeight = 0) : aerialWeight(aerialWeight) {}

		virtual float GetReward(const PlayerData& player, const GameState& state) {
			using namespace CommonValues;

			if (player.ballTouchedStep) {
				return powf((state.ball.pos.z + BALL_RADIUS) / (BALL_RADIUS * 2), aerialWeight);
			} else {
				return 0;
			}
		}
	};
}