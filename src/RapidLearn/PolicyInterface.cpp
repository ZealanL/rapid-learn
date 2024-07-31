#include "PolicyInterface.h"

#include "Bindings.h"

using namespace RPLC;
using namespace RLGSC;

class PyPolicyInterface : public PolicyInterface {
public:
	virtual pyb::object get_actions(pyb::object allObs) {
		return {};
	}

	ActionParser::Input GetActions(const FList2& allObs) override {
		return get_actions(pyb::cast(allObs)).cast<ActionParser::Input>();
	}
};
class TPyPolicyInterface : public PyPolicyInterface {
public:
	PYB_V_OVERRIDE_PURE(pyb::object, PyPolicyInterface, get_actions, (pyb::object allObs), allObs)
};
RPLC_ADD_BIND(PolicyInterface) {
	pyb::class_<PolicyInterface, RG_SHR(PolicyInterface)>(m, "BasePolicyInterface")
		PYB_DEFAULT_INIT()
		;

#define PYB_CUR_CLASS PyPolicyInterface
	pyb::class_<PyPolicyInterface, RG_SHR(PyPolicyInterface), TPyPolicyInterface, PolicyInterface>(m, "PolicyInterface")
		PYB_DEFAULT_INIT()
		.def(PYB_M(get_actions))
		;
}