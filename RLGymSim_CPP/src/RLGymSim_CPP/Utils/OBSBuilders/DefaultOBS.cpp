#include "DefaultObs.h"

void RLGSC::DefaultObs::AddPlayerToObs(FList& obs, const PlayerData& player, bool inv) {
	PhysObj phys = player.GetInvert(inv);

	obs += phys.pos * POS_COEF;
	obs += phys.rotMat.forward;
	obs += phys.rotMat.up;
	obs += phys.vel * VEL_COEF;
	obs += phys.angVel * ANG_VEL_COEF,
		
	obs += {
		player.boostFraction,
		(float)player.carState.isOnGround,
		(float)player.hasFlipOrJump,
		(float)player.carState.isDemoed,
	};
}

RLGSC::FList RLGSC::DefaultObs::BuildObs(const PlayerData& player, const GameState& state) {
	FList result = {};

	bool inv = player.team == Team::ORANGE;

	auto& ball = state.GetBallPhys(inv);
	auto& pads = state.GetBoostPads(inv);

	result += ball.pos * POS_COEF;
	result += ball.vel * VEL_COEF;
	result += ball.angVel * ANG_VEL_COEF;

	for (int i = 0; i < player.prevAction.ELEM_AMOUNT; i++)
		result += player.prevAction[i];

	for (int i = 0; i < CommonValues::BOOST_LOCATIONS_AMOUNT; i++)
		result += (float)pads[i];

	AddPlayerToObs(result, player, inv);

	FList teammates = {}, opponents = {};

	for (auto& otherPlayer : state.players) {
		if (otherPlayer.carId == player.carId)
			continue;

		AddPlayerToObs(
			(otherPlayer.team == player.team) ? teammates : opponents,
			otherPlayer,
			inv
		);
	}

	result += teammates;
	result += opponents;
	return result;
}