#include "PolicyInterface.h"

#include "Bindings.h"

using namespace RPLC;
using namespace RLGSC;

class PyPolicyInterface : public PolicyInterface {
public:
	PYB_V_WRAP(ActionParser::Input, PolicyInterface, GetActions, get_actions, (const FList2& allObs), (allObs));
};
class TPyPolicyInterface : public PyPolicyInterface {
public:
	PYB_V_OVERRIDE_PURE(ActionParser::Input, PyPolicyInterface, get_actions, (const FList2& allObs), allObs)
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