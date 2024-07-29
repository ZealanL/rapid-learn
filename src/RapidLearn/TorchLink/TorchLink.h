#pragma once
#include "../Bindings.h"
#include "TorchLink.h"

namespace RPLC {
	namespace TorchLink {

		pybind11::module* GetTorchMod();

		struct Tensor {
			pybind11::object _obj;

			Tensor();

			RG_NO_COPY(Tensor);
		};
	}
}