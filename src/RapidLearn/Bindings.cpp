#include "Bindings.h"

// TODO: Lame global variable
static std::vector<RPLC::Bindings::InitFn> g_AllInitFuncs;

RPLC::Bindings::AddInitFunc::AddInitFunc(InitFn initFunc) {
	g_AllInitFuncs.push_back(initFunc);
}

PYBIND11_MODULE(rapid_learn, m) {

	try {
		for (auto fn : g_AllInitFuncs)
			fn(m);

	} catch (std::exception e) {
		RG_ERR_CLOSE("Failed to initialize pybind11 module: Got exception: " << e.what());
	}
}