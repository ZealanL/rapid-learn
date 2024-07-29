#pragma once
#include "../Gamestates/GameState.h"
#include "../BasicTypes/Action.h"
#include "../BasicTypes/Lists.h"

// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/obs_builders/obs_builder.py
namespace RLGSC {
	// TODO: Only designed for discrete actions currently 
	class ActionParser {
	public:
		typedef IList Input;

		virtual ActionSet ParseActions(const Input& actionsData, const GameState& gameState) {
			RG_ERR_CLOSE(typeid(*this).name() << ": ParseActions() is unimplemented");
			return {};
		}

		virtual int GetActionAmount() {
			RG_ERR_CLOSE(typeid(*this).name() << ": GetActionAmount() is unimplemented");
			return {};
		}
	};
}