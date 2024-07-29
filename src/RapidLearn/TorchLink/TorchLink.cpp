#include "TorchLink.h"

using namespace RPLC;
namespace py = pybind11;

pybind11::module* RPLC::TorchLink::GetTorchMod() {
	// TODO: Temporary
	static pybind11::module torchMod = py::module::import("torch");
	return &torchMod;
}

RPLC::TorchLink::Tensor::Tensor() {
	_obj = GetTorchMod()->attr("tensor")();
}