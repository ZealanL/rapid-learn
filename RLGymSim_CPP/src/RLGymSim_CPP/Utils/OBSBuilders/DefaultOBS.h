#pragma once
#include "ObsBuilder.h"

// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/obs_builders/default_obs.py
namespace RLGSC {
	class DefaultObs : public ObsBuilder {
	public:
		constexpr static float 
			POS_COEF = 5120,
			VEL_COEF = 2300,
			ANG_VEL_COEF = 5.5f;

		virtual void AddPlayerToObs(FList& obs, const PlayerData& player, bool inv);

		virtual FList BuildObs(const PlayerData& player, const GameState& state);
	};
}