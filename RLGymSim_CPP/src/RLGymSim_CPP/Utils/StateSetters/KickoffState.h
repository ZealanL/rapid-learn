#pragma once
#include "StateSetter.h"

namespace RLGSC {
	class KickoffState : public StateSetter {
	public:
		virtual void ResetArena(Arena* arena) {
			arena->ResetToRandomKickoff();
		}
	};
}