#pragma once
#include "Framework.h"

namespace RPLC {
	// Interface for the policy that controls the players in multiple envs
	class PolicyInterface {
	public:
		// allObs: obs array for each player
		// returns: action parser inputs for each player
		virtual RLGSC::ActionParser::Input GetActions(const RLGSC::FList2& allObs) {
			RG_ERR_CLOSE("PolicyInterface::GetActions() is unimplemented");
			return {};
		}
	};
}