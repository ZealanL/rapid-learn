#pragma once
#include "DefaultObs.h"

namespace RLGSC {
	// Verion of DefaultObs that supports a varying number of players (i.e. 1v1, 2v2, 3v3, etc.)
	// Maximum player count can be however high you want
	// Opponent and teammate slots are randomly shuffled to prevent slot bias
	class DefaultObsPadded : public DefaultObs {
	public:

		int maxPlayers;

		DefaultObsPadded(int maxPlayers) : DefaultObs(), maxPlayers(maxPlayers) {}

		virtual FList BuildObs(const PlayerData& player, const GameState& state);
	};
}