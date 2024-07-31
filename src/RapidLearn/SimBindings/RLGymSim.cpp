#include "RLGymSim.h"

#include <RLGymSim_CPP/Utils/RewardFunctions/CombinedReward.h>
#include <RLGymSim_CPP/Utils/RewardFunctions/CommonRewards.h>
#include <RLGymSim_CPP/Utils/RewardFunctions/ZeroSumReward.h>
#include <RLGymSim_CPP/Utils/TerminalConditions/GoalScoreCondition.h>
#include <RLGymSim_CPP/Utils/TerminalConditions/NoTouchCondition.h>
#include <RLGymSim_CPP/Utils/OBSBuilders/DefaultOBS.h>
#include <RLGymSim_CPP/Utils/OBSBuilders/DefaultOBSPadded.h>
#include <RLGymSim_CPP/Utils/StateSetters/KickoffState.h>
#include <RLGymSim_CPP/Utils/StateSetters/RandomState.h>
#include <RLGymSim_CPP/Utils/ActionParsers/DefaultAction.h>

using namespace RLGSC;

RPLC_ADD_BIND(Action) {
#define PYB_CUR_CLASS Action
	pyb::class_<Action>(m, "Action", pyb::buffer_protocol())
		PYB_DEFAULT_INITS()
		PYBP(throttle)
		PYBP(steer)

		PYBP(pitch)
		PYBP(yaw)
		PYBP(roll)

		PYBP(jump)
		PYBP(boost)
		PYBP(handbrake)

		.def_buffer(
			[](Action& self) -> pyb::buffer_info {
				return pyb::buffer_info(
					self.begin(), /* Pointer to buffer */
					sizeof(float), /* Size of one scalar */
					pyb::format_descriptor<float>::format(), /* Python struct-style format descriptor */
					1, /* Number of dimensions */
					{ Action::ELEM_AMOUNT }, /* Buffer dimensions */
					{ 1 } /* Strides (in bytes) for each index */
				);
			}
		)
		;
}

RPLC_ADD_BIND(EnvConfig) {
#define PYB_CUR_CLASS RLGSC::EnvConfig
	PYB_CLASS(EnvConfig)
		PYB_DEFAULT_INITS()
		PYBP(rewardFn)
		PYBP(terminalConds)
		PYBP(obsBuilder)
		PYBP(actionParser)
		PYBP(stateSetter)
		;
}

////////////////////////////////////////////////////////////////////////////////////////////////

class PyRewardFunction : public RewardFunction {
public:
	PYB_V_WRAP(void, RewardFunction, Reset, reset, (const GameState& state), (state));
	PYB_V_WRAP(void, RewardFunction, PreStep, pre_step, (const GameState& state), (state));
	PYB_V_WRAP(float, RewardFunction, GetReward, get_reward, (const PlayerData& player, const GameState& state), (player, state));
	PYB_V_WRAP(FList, RewardFunction, GetAllRewards, get_all_rewards, (const GameState& state), (state));
};
class TPyRewardFunction : public PyRewardFunction {
public:
	virtual void reset(const GameState& state) {
		PYBIND11_OVERRIDE(void, PyRewardFunction, reset, state);
	}

	virtual void pre_step(const GameState& state) {
		PYBIND11_OVERRIDE(void, PyRewardFunction, pre_step, state);
	}

	virtual float get_reward(const PlayerData& player, const GameState& state) {
		PYBIND11_OVERRIDE_PURE(float, PyRewardFunction, get_reward, player, state);
	}

	virtual FList get_all_rewards(const GameState& state) {
		PYBIND11_OVERRIDE(FList, PyRewardFunction, get_all_rewards, state);
	}
};
RPLC_ADD_BIND(Rewards) {

#define PYB_CUR_CLASS RewardFunction
	pyb::class_<RewardFunction, std::shared_ptr<RewardFunction>>(m, "BaseRewardFunction")
		PYB_DEFAULT_INIT();

#define PYB_CUR_CLASS PyRewardFunction
	pyb::class_<PyRewardFunction, TPyRewardFunction, RewardFunction, std::shared_ptr<PyRewardFunction>>(m, "RewardFunction")
		PYB_DEFAULT_INIT()
		.def(PYB_M(reset))
		.def(PYB_M(pre_step))
		.def(PYB_M(get_reward))
		.def(PYB_M(get_all_rewards))
		;

	auto rewards = m.def_submodule("rewards");

	PYB_CLASS_MVB(rewards, CombinedReward, RewardFunction)
		.def(pyb::init<std::vector<RG_SHR(RewardFunction)>, std::vector<float>>(), PYBA("rewards"), PYBA("weights"))
		.def(pyb::init<std::vector<std::pair<RG_SHR(RewardFunction), float>>>(), PYBA("rewards_with_weights"))
		;

	PYB_CLASS_MVB(rewards, ZeroSumReward, RewardFunction)
		.def(pyb::init<RG_SHR(RewardFunction), float, float>(), PYBA("reward_fn"), PYBA("team_spirit"), PYBA("opp_scale") = 1)
		;

#define EVENT_REWARD_WEIGHT_ANNOTATE_PFM(name) PYBA(PYBS(#name)) = 0.f
#define EVENT_REWARD_WEIGHTS_ANNOTATED EVENT_REWARD_WEIGHTS_INS(EVENT_REWARD_WEIGHT_ANNOTATE_PFM)
	PYB_CLASS_MVB(rewards, EventReward, RewardFunction)
		.def(
			pyb::init<>(&EventReward::Make), 
			EVENT_REWARD_WEIGHTS_ANNOTATED
		)
		;

	PYB_CLASS_MVB(rewards, VelocityReward, RewardFunction) PYB_DEFAULT_INIT();
	PYB_CLASS_MVB(rewards, SaveBoostReward, RewardFunction) PYB_DEFAULT_INIT();
	PYB_CLASS_MVB(rewards, VelocityBallToGoalReward, RewardFunction) PYB_DEFAULT_INIT();
	PYB_CLASS_MVB(rewards, VelocityPlayerToBallReward, RewardFunction) PYB_DEFAULT_INIT();
	PYB_CLASS_MVB(rewards, FaceBallReward, RewardFunction) PYB_DEFAULT_INIT();
	PYB_CLASS_MVB(rewards, TouchBallReward, RewardFunction) PYB_DEFAULT_INIT();
}

////////////////////////////////////////////////////////////////////////////////////////////////

class PyTerminalCondition : public TerminalCondition {
public:
	PYB_V_WRAP(void, TerminalCondition, Reset, reset, (const GameState& state), (state));
	PYB_V_WRAP(bool, TerminalCondition, IsTerminal, is_terminal, (const GameState& state), (state));
};
class TPyTerminalCondition : public PyTerminalCondition {
public:
	virtual void reset(const GameState& state) {
		PYBIND11_OVERRIDE(void, TPyTerminalCondition, reset, state);
	}

	virtual bool is_terminal(const GameState& state) {
		PYBIND11_OVERRIDE_PURE(bool, TPyTerminalCondition, is_terminal, state);
	}
};
RPLC_ADD_BIND(Terminals) {

#define PYB_CUR_CLASS TerminalCondition
	pyb::class_<TerminalCondition, std::shared_ptr<TerminalCondition>>(m, "BaseTerminalCondition")
		PYB_DEFAULT_INIT();

#define PYB_CUR_CLASS PyTerminalCondition
	pyb::class_<PyTerminalCondition, TPyTerminalCondition, TerminalCondition, std::shared_ptr<PyTerminalCondition>>(m, "TerminalCondition")
		PYB_DEFAULT_INIT()
		.def(PYB_M(reset))
		.def(PYB_M(is_terminal))
		;

	auto terminals = m.def_submodule("terminals");
	PYB_CLASS_MVB(terminals, GoalScoreCondition, TerminalCondition) PYB_DEFAULT_INIT();
	PYB_CLASS_MVB(terminals, NoTouchCondition, TerminalCondition)
		.def(pyb::init<float>(), PYBA("touch_timeout"));
}

////////////////////////////////////////////////////////////////////////////////////////////////

class PyObsBuilder : public ObsBuilder {
public:
	PYB_V_WRAP(void, ObsBuilder, Reset, reset, (const GameState& state), (state));
	PYB_V_WRAP(void, ObsBuilder, PreStep, pre_step, (const GameState& state), (state));
	PYB_V_WRAP(FList, ObsBuilder, BuildObs, build_obs, (const PlayerData& player, const GameState& state), (player, state));
};
class TPyObsBuilder : public PyObsBuilder {
public:
	virtual void reset(const GameState& state) {
		PYBIND11_OVERRIDE(void, TPyObsBuilder, reset, state);
	}

	virtual void pre_step(const GameState& state) {
		PYBIND11_OVERRIDE(void, TPyObsBuilder, pre_step, state);
	}

	virtual FList build_obs(const PlayerData& player, const GameState& state) {
		PYBIND11_OVERRIDE_PURE(FList, TPyObsBuilder, build_obs, player, state);
	}
};

RPLC_ADD_BIND(Obs) {
#define PYB_CUR_CLASS ObsBuilder
	pyb::class_<ObsBuilder, std::shared_ptr<ObsBuilder>>(m, "BaseObsBuilder")
		PYB_DEFAULT_INIT();

#define PYB_CUR_CLASS PyObsBuilder
	pyb::class_<PyObsBuilder, TPyObsBuilder, ObsBuilder, std::shared_ptr<PyObsBuilder>>(m, "ObsBuilder")
		PYB_DEFAULT_INIT()
		.def(PYB_M(reset))
		.def(PYB_M(pre_step))
		.def(PYB_M(build_obs))
		;

	auto obs = m.def_submodule("obs");
	PYB_CLASS_MVB(obs, DefaultObs, ObsBuilder)
		PYB_DEFAULT_INIT();
	PYB_CLASS_MVB(obs, DefaultObsPadded, ObsBuilder)
		.def(pyb::init<int>(), PYBA("max_players"));
}

////////////////////////////////////////////////////////////////////////////////////////////////

class PyStateSetter : public StateSetter {
public:
	virtual void reset(GameState& curState) {
		RG_ERR_CLOSE("PyStateSetter(): reset is unimplemented");
	}

	virtual void ResetArena(Arena* arena) {
		arena->ResetToRandomKickoff();
		auto state = GameState(arena);
		reset(state);
		state.SyncAndApplyToArena(arena);
	}
};
class TPyStateSetter : public PyStateSetter {
public:
	virtual void reset(GameState& state) {
		PYBIND11_OVERRIDE_PURE(void, TPyStateSetter, reset, state);
	}
};
RPLC_ADD_BIND(States) {
#define PYB_CUR_CLASS StateSetter
	pyb::class_<StateSetter, std::shared_ptr<StateSetter>>(m, "BaseStateSetter")
		PYB_DEFAULT_INIT();

#define PYB_CUR_CLASS PyStateSetter
	pyb::class_<PyStateSetter, TPyStateSetter, StateSetter, std::shared_ptr<PyStateSetter>>(m, "StateSetter")
		PYB_DEFAULT_INIT()
		.def(PYB_M(reset))
		;

	auto states = m.def_submodule("states");
	PYB_CLASS_MVB(states, KickoffState, StateSetter)
		PYB_DEFAULT_INIT();
	PYB_CLASS_MVB(states, RandomState, StateSetter)
		.def(pyb::init<bool, bool, bool>(), PYBA("rand_ball_speed"), PYBA("rand_car_speed"), PYBA("cars_on_ground"));
}

////////////////////////////////////////////////////////////////////////////////////////////////
 
class PyActionParser : public ActionParser {
public:
	PYB_V_WRAP(ActionSet, ActionParser, ParseActions, parse_actions, (const Input& actionsData, const GameState& state), (actionsData, state));
	PYB_V_WRAP(int, ActionParser, GetActionAmount, get_action_amount, (), ());
};
class TPyActionParser : public PyActionParser {
public:
	virtual ActionSet parse_actions(const Input& actionsData, const GameState& state) {
		PYBIND11_OVERRIDE(ActionSet, TPyActionParser, parse_actions, actionsData, state);
	}

	virtual int get_action_amount() {
		PYBIND11_OVERRIDE(int, TPyActionParser, get_action_amount);
	}
};

RPLC_ADD_BIND(Acts) {
#define PYB_CUR_CLASS ActionParser
	pyb::class_<ActionParser, std::shared_ptr<ActionParser>>(m, "BaseActionParser")
		PYB_DEFAULT_INIT();

#define PYB_CUR_CLASS PyActionParser
	pyb::class_<PyActionParser, TPyActionParser, ActionParser, std::shared_ptr<PyActionParser>>(m, "ActionParser")
		PYB_DEFAULT_INIT()
		.def(PYB_M(parse_actions))
		.def(PYB_M(get_action_amount))
		;

	auto acts = m.def_submodule("acts");
	PYB_CLASS_MVB(acts, DefaultAction, ActionParser)
		PYB_DEFAULT_INIT();
}