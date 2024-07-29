#pragma once
#include "../Gamestates/GameState.h"
#include "../BasicTypes/Action.h"
#include "../BasicTypes/Lists.h"

// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/obs_builders/obs_builder.py
namespace RLGSC {
	class ObsBuilder {
	public:
		virtual void Reset(const GameState& initialState) {}

		virtual void PreStep(const GameState& state) {}

		// NOTE: May be called once during environment initialization to determine policy neuron size
		virtual FList BuildObs(const PlayerData& player, const GameState& state) {
			RG_ERR_CLOSE(typeid(*this).name() << ": BuildObs() is unimplemented");
		}
	};
}