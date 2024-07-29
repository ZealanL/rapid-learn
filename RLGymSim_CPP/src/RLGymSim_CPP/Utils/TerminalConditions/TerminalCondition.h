#pragma once
#include "../Gamestates/GameState.h"

namespace RLGSC {
	class TerminalCondition {
	public:
		virtual void Reset(const GameState& initialState) {};
		virtual bool IsTerminal(const GameState& currentState) {
			RG_ERR_CLOSE(typeid(*this).name() << ": IsTerminal() is unimplemented");
			return {};
		}
	};
}