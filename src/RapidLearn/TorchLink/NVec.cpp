#pragma once
#include "NVec.h"
#include "../Bindings.h"

using namespace RPLC;

RPLC_ADD_BIND(NVec) {
	//https://pyb.readthedocs.io/en/stable/advanced/pycpp/numpy.html
	pyb::class_<NVecF>(m, "NVecF", pyb::buffer_protocol())
		.def_buffer(
			[](NVecF& self) -> pyb::buffer_info {
				return pyb::buffer_info(
					self.GetData().data(), /* Pointer to buffer */
					sizeof(float), /* Size of one scalar */
					pyb::format_descriptor<float>::format(), /* Python struct-style format descriptor */
					self.GetDims().size(), /* Number of dimensions */
					self.GetDims(), /* Buffer dimensions */
					self.CalcByteStrides() /* Strides (in bytes) for each index */
				);
			}
	);

	//https://pyb.readthedocs.io/en/stable/advanced/pycpp/numpy.html
	pyb::class_<NVecI>(m, "NVecI", pyb::buffer_protocol())
		.def_buffer(
			[](NVecI& self) -> pyb::buffer_info {
				return pyb::buffer_info(
					self.GetData().data(), /* Pointer to buffer */
					sizeof(int), /* Size of one scalar */
					pyb::format_descriptor<int>::format(), /* Python struct-style format descriptor */
					self.GetDims().size(), /* Number of dimensions */
					self.GetDims(), /* Buffer dimensions */
					self.CalcByteStrides() /* Strides (in bytes) for each index */
				);
			}
	);
}