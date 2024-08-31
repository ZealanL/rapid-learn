#include "DefaultObsPadded.h"

RLGSC::FList RLGSC::DefaultObsPadded::BuildObs(const PlayerData& player, const GameState& state) {
	FList result = {};

	bool inv = player.team == Team::ORANGE;

	auto ball = state.ball.GetInvert(inv);
	auto& pads = state.GetBoostPads(inv);

	result += ball.pos * POS_COEF;
	result += ball.vel * VEL_COEF;
	result += ball.angVel * ANG_VEL_COEF;

	for (int i = 0; i < player.prevAction.ELEM_AMOUNT; i++)
		result += player.prevAction[i];

	for (int i = 0; i < CommonValues::BOOST_LOCATIONS_AMOUNT; i++)
		result += (float)pads[i];

	FList selfObs = {};
	AddPlayerToObs(selfObs, player, inv);
	result += selfObs;
	int playerObsSize = selfObs.size();

	FList2 teammates = {}, opponents = {};

	for (auto& otherPlayer : state.players) {
		if (otherPlayer.carId == player.carId)
			continue;

		FList playerObs = {};
		AddPlayerToObs(
			playerObs,
			otherPlayer,
			inv
		);
		((otherPlayer.team == player.team) ? teammates : opponents).push_back(playerObs);
	}

	if (teammates.size() > maxPlayersPerTeam - 1)
		RG_ERR_CLOSE("DefaultObsPadded: Too many teammates for obs, maximum is " << (maxPlayersPerTeam - 1));
	
	if (opponents.size() > maxPlayersPerTeam)
		RG_ERR_CLOSE("DefaultObsPadded: Too many opponents for obs, maximum is " << maxPlayersPerTeam);

	for (int i = 0; i < 2; i++) {
		auto& playerList = i ? teammates : opponents;
		int targetCount = i ? maxPlayersPerTeam - 1 : maxPlayersPerTeam;

		while (playerList.size() < targetCount) {
			FList pad = FList(playerObsSize);
			playerList.push_back(pad);
		}
	}

	// Shuffle both lists
	std::shuffle(teammates.begin(), teammates.end(), ::Math::GetRandEngine());
	std::shuffle(opponents.begin(), opponents.end(), ::Math::GetRandEngine());

	for (auto& teammate : teammates)
		result += teammate;
	for (auto& opponent : opponents)
		result += opponent;

	return result;
}