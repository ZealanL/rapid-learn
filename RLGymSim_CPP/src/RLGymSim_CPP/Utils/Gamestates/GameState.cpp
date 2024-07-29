#include "GameState.h"

#include "../../Math.h"

using namespace RLGSC;

static int boostPadIndexMap[CommonValues::BOOST_LOCATIONS_AMOUNT] = {};
static bool boostPadIndexMapBuilt = false;
static std::mutex boostPadIndexMapMutex = {};
void _BuildBoostPadIndexMap(Arena* arena) {
	constexpr const char* ERROR_PREFIX = "_BuildBoostPadIndexMap(): ";
#ifdef RG_VERBOSE
	RG_LOG("Building boost pad index map...");
#endif

	if (arena->_boostPads.size() != CommonValues::BOOST_LOCATIONS_AMOUNT) {
		RG_ERR_CLOSE(
			ERROR_PREFIX << "Arena boost pad count does not match CommonValues::BOOST_LOCATIONS_AMOUNT " <<
			"(" << arena->_boostPads.size() << "/" << CommonValues::BOOST_LOCATIONS_AMOUNT << ")"
		);
	}
	
	bool found[CommonValues::BOOST_LOCATIONS_AMOUNT] = {};
	for (int i = 0; i < CommonValues::BOOST_LOCATIONS_AMOUNT; i++) {
		Vec targetPos = CommonValues::BOOST_LOCATIONS[i];
		for (int j = 0; j < arena->_boostPads.size(); j++) {
			Vec padPos = arena->_boostPads[j]->config.pos;

			if (padPos.DistSq2D(targetPos) < 10) {
				if (!found[i]) {
					found[i] = true;
					boostPadIndexMap[i] = j;
				} else {
					RG_ERR_CLOSE(
						ERROR_PREFIX << "Matched duplicate boost pad at " << targetPos << "=" << padPos
					);
				}
				break;
			}
		}

		if (!found[i])
			RS_ERR_CLOSE(ERROR_PREFIX << "Failed to find matching pad at " << targetPos);
	}

#ifdef RG_VERBOSE
	RG_LOG(" > Done");
#endif
	boostPadIndexMapBuilt = true;
}

void RLGSC::GameState::UpdateFromArena(Arena* arena, const GameState* prev) {
	if (prev) {
		GameState prevDst = *prev;
		prevDst.prev = {};
		this->prev = std::make_shared<const GameState>(prevDst);
	} else {
		this->prev = NULL;
	}

	lastArena = arena;
	int tickSkip = RS_MAX(arena->tickCount - lastTickCount, 0);

	deltaTime = tickSkip * (1 / 120.f);

	ballState = arena->ball->GetState();
	ball = PhysObj(ballState);
	ballInv = ball.Invert();

	players.resize(arena->_cars.size());

	auto carItr = arena->_cars.begin();
	for (int i = 0; i < players.size(); i++) {
		auto& player = players[i];
		player.UpdateFromCar(*carItr, arena->tickCount, tickSkip);
		if (player.ballTouchedStep)
			lastTouchCarID = player.carId;

		carItr++;
	}

	if (!boostPadIndexMapBuilt) {
		boostPadIndexMapMutex.lock();
		// Check again? This seems stupid but also makes sense to me
		//	Without this, we could lock as the index map is building, then go build again
		//	I would like to keep the mutex inside the if statement so it is only checked a few times
		if (!boostPadIndexMapBuilt) 
			_BuildBoostPadIndexMap(arena);
		boostPadIndexMapMutex.unlock();
	}

	for (int i = 0; i < CommonValues::BOOST_LOCATIONS_AMOUNT; i++) {
		int idx = boostPadIndexMap[i];
		int invIdx = boostPadIndexMap[CommonValues::BOOST_LOCATIONS_AMOUNT - i - 1];

		auto state = arena->_boostPads[idx]->GetState();
		auto stateInv = arena->_boostPads[invIdx]->GetState();

		boostPads[i] = state.isActive;
		boostPadsInv[i] = stateInv.isActive;

		boostPadTimers[i] = state.cooldown;
		boostPadTimersInv[i] = stateInv.cooldown;
	}


	blueScored = orangeScored = false;
	// Update goal scoring
	// If you don't have a GoalScoreCondition then that's not my problem lmao
	if (Math::IsBallScored(ball.pos))
		((ball.pos.y > 0) ? blueScored : orangeScored) = true;

	lastTickCount = arena->tickCount;
}

void RLGSC::GameState::SyncAndApplyToArena(Arena* arena) {
	constexpr const char* ERR_PREFIX = "GameState::SyncAndApplyToArena(): ";

	for (auto& player : players)
		player.SyncCarState();
	ball.WriteToPhysState(ballState);

	if (arena->_cars.size() != players.size())
		RG_ERR_CLOSE(ERR_PREFIX << "Arena car amount (" << arena->_cars.size() << ") does not match player amount in GameState (" << players.size() << ")");

	std::unordered_set<uint32_t> foundIds = {};
	for (int i = 0; i < players.size(); i++) {
		auto& player = players[i];
		Car* car = arena->GetCar(player.carId);
		if (!car)
			RG_ERR_CLOSE(ERR_PREFIX << "Car id " << player.carId << " does not exist");

		if (!foundIds.insert(player.carId).second)
			RG_ERR_CLOSE(ERR_PREFIX << "A car with id " << player.carId << " was set twice");

		if (car->team != player.team)
			RG_ERR_CLOSE(ERR_PREFIX << "Cannot change the team of cars in state-setting");

		car->SetState(player.carState);
	}

	arena->ball->SetState(ballState);

	// TODO: Support setting boost pads
}