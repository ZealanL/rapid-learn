#include "PlayerData.h"

namespace RLGSC {
	void PlayerData::UpdateFromCar(Car* car, uint64_t tickCount, int tickSkip) {
		carId = car->id;
		team = car->team;
		CarState newState = car->GetState();

		if (newState.isDemoed && newState.pos.z == -50000) {
			newState.pos = carState.pos;
			newState.vel = carState.vel;
			newState.angVel = carState.angVel;
		}

		carState = newState;

		PhysObj::PhysObj(carState);

		if (carState.ballHitInfo.isValid) {
			ballTouchedStep = carState.ballHitInfo.tickCountWhenHit >= (tickCount - tickSkip);
			ballTouchedTick = carState.ballHitInfo.tickCountWhenHit == (tickCount - 1);
		} else {
			ballTouchedStep = ballTouchedTick = false;
		}

		hasFlipOrJump = carState.HasFlipOrJump();

		boostFraction = carState.boost / 100;

		prevAction = carState.lastControls;
	}

	void PlayerData::SyncCarState() {
		PhysObj::WriteToPhysState(carState);
		carState.boost = boostFraction * 100;
	}
}