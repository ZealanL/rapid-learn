#include "RLGymSim_State.h"

using namespace RLGSC;

RPLC_ADD_BIND(PhysObj) {
#define PYB_CUR_CLASS RLGSC::PhysObj
	PYB_CLASS(PhysObj)
		PYB_DEFAULT_INITS()
		PYBP(pos)
		PYBP(rotMat)
		PYBP(vel)
		PYBP(angVel)
		.def(PYB_M(Invert))
		.def(PYB_M(MirrorX))
		.def(PYB_M(GetInvert))
		;
}

// Car state property (read-only)
#define CSP(name) .def_property_readonly(PYBS(#name), [](const PlayerData& playerData) { return playerData.carState.name; })

// Car state property (func)
#define CSPF(func) .def_property_readonly(PYBS(#func), [](const PlayerData& playerData) { return playerData.carState.func(); })

// Flag bool property (read-only)
#define FBP(name) .def_property_readonly(PYBS("flag_" #name), [](const PlayerData& playerData) { return playerData.flags.name; })

RPLC_ADD_BIND(PlayerData) {
#define PYB_CUR_CLASS RLGSC::PlayerData
	PYB_CLASS(PlayerData)
		PYB_DEFAULT_INITS()

		PYBPR(carId)
		PYBPR(team)
		.def_property_readonly("team_num", [](const PlayerData& playerData) { return (int)playerData.team; })

		PYBPR(hasFlipOrJump)
		PYBPR(boostFraction)
		PYBPR(prevAction)

		PYBPR(ballTouchedStep)
		PYBPR(ballTouchedTick)

		FBP(playerGoal) FBP(save) FBP(assist)
		FBP(shot) FBP(shotPass)
		FBP(bump) FBP(bumped) FBP(demo) FBP(demoed)

		CSP(isOnGround)
		CSP(hasDoubleJumped)
		CSP(airTime)
		CSP(airTimeSinceJump)
		
		CSP(hasJumped)
		CSP(isJumping)
		CSP(jumpTime)

		CSP(hasFlipped)
		CSP(isFlipping)
		CSP(flipTime)
		CSP(flipRelTorque)
		;
}

#undef CSP
#undef CSPF
#undef FBP

RPLC_ADD_BIND(GameState) {
#define PYB_CUR_CLASS RLGSC::GameState
	PYB_CLASS(GameState)
		PYB_DEFAULT_INITS()
		PYBPR(prev)
		PYBPR(deltaTime)
		PYBPR(isTerminal)
		PYBPR(blueScored) PYBPR(orangeScored)
		PYBPR(lastTouchCarID)

		PYBP(players)
		PYBP(ball) PYBP(ballInv)

		PYBPR(boostPads) PYBPR(boostPadsInv)
		PYBPR(boostPadTimers) PYBPR(boostPadTimersInv)
		PYBPR(lastTickCount)
		;
}