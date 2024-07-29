#pragma once
#include "../Gamestates/GameState.h"

namespace RLGSC {
	class StateSetter {
	public:
		// NOTE: This should apply the reset state to the arena
		virtual void ResetArena(Arena* arena) {
			RG_ERR_CLOSE(typeid(*this).name() << ": ResetArena() is unimplemented");
		}
	};
}