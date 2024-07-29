#pragma once
#include "Bindings.h"
#include "Threading/GameInst.h"

namespace RPLC {

	namespace Collector {
		void Run(EnvCreateFn envCreateFunc);
	};
}