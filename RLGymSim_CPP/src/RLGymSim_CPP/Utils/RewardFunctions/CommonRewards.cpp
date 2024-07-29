#pragma once
#include "CommonRewards.h"

float RLGSC::EventReward::GetReward(const PlayerData& player, const GameState& state) {
	float reward = 0;

	bool teamGoal = player.team == Team::BLUE ? state.blueScored : state.orangeScored;
	bool teamConcede = player.team == Team::BLUE ? state.orangeScored : state.blueScored;

	reward += teamGoal * weights.teamGoal;
	reward += teamConcede * weights.concede;

	reward += player.flags.assist * weights.assist;
	reward += player.flags.bump * weights.bump;
	reward += player.flags.bumped * weights.bumped;
	reward += player.flags.demo * weights.demo;
	reward += player.flags.demoed * weights.demoed;
	reward += player.flags.playerGoal * weights.playerGoal;
	reward += player.flags.save * weights.save;
	reward += player.flags.shot * weights.shot;
	reward += player.flags.shotPass * weights.shotPass;

	return reward;
}