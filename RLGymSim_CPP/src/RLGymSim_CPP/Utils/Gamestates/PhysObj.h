#pragma once
#include "../../Framework.h"

namespace RLGSC {
	// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/gamestates/physics_object.py
	struct PhysObj {
		Vec pos;
		RotMat rotMat;
		Vec vel, angVel;

		PhysObj() = default;
		PhysObj(const BallState& ballState);
		PhysObj(const CarState& carState);

		// Rotate 180 degrees around Z axis, scales everything by (-1, -1, 1)
		PhysObj Invert() const;

		PhysObj GetInvert(bool inverted) const {
			return inverted ? Invert() : *this;
		}

		// Mirror along X axis
		PhysObj MirrorX() const;

		void WriteToPhysState(PhysState& physState) const;
	};
}