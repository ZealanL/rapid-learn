#include "Match.h"

namespace RLGSC {
	void Match::EpisodeReset(const GameState& initialState) {
		for (auto cond : terminalConditions)
			cond->Reset(initialState);
		rewardFn->Reset(initialState);
		obsBuilder->Reset(initialState);
	}

	FList2 Match::BuildObservations(const GameState& state) {
		auto result = FList2(state.players.size());

		obsBuilder->PreStep(state);

		for (int i = 0; i < state.players.size(); i++) {
			result[i] =
				obsBuilder->BuildObs(state.players[i], state);
		}

		return result;
	}

	FList Match::GetRewards(const GameState& state) {
		auto result = FList(state.players.size());

		rewardFn->PreStep(state);
		return rewardFn->GetAllRewards(state);
	}

	bool Match::IsDone(const GameState& state) {
		for (auto& cond : terminalConditions)
			if (cond->IsTerminal(state))
				return true;

		return false;
	}

	ActionSet Match::ParseActions(const ActionParser::Input& actionsData, const GameState& gameState) {
		ActionSet actions = actionParser->ParseActions(actionsData, gameState);

		for (int i = 0; i < gameState.players.size(); i++)
			if (gameState.players[i].carState.isDemoed)
				actions[i] = {};

		return actions;
	}

	GameState Match::ResetState(Arena* arena) {
		stateSetter->ResetArena(arena);
		GameState newState = GameState(arena);

		if (newState.players.size() != playerAmount) {
			RG_ERR_CLOSE(
				"Match::ResetState(): New state has a different amount of players, "
				"expected " << playerAmount << " but got " << newState.players.size() << ".\n"
				"Changing number of players at state reset is currently not supported.\n" <<
				"If you want differing player amounts, set a differing player amount per env."
			);
		}

		return newState;
	}
}