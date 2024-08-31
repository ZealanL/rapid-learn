#include "RocketSim.h"

RPLC_ADD_BIND(GameMode) {
#define PYB_CUR_CLASS GameMode
	PYB_ENUM(GameMode)
		PYB_ENUM_VAL(GameMode::SOCCAR)
		PYB_ENUM_VAL(GameMode::HOOPS)
		PYB_ENUM_VAL(GameMode::HEATSEEKER)
		PYB_ENUM_VAL(GameMode::SNOWDAY)
		PYB_ENUM_END()
		;
}

RPLC_ADD_BIND(Team) {
#define PYB_CUR_CLASS Team
	PYB_ENUM(Team)
		PYB_ENUM_VAL(Team::BLUE)
		PYB_ENUM_VAL(Team::ORANGE)
		PYB_ENUM_END()
		;
}

RPLC_ADD_BIND(MathTypes) {
#define PYB_CUR_CLASS Vec
	PYB_CLASS_BUF(Vec)
		PYB_DEFAULT_INITS()
		.def(pyb::init<float, float, float>(), PYBA("x"), PYBA("y"), PYBA("z"))
		PYBP(x)
		PYBP(y)
		PYBP(z)

		.def("__getitem__", [](const Vec& v, size_t i) { return v[i]; })
		.def("__setitem__", [](Vec& v, size_t i, float f) { v[i] = f; })

		.def(pyb::self + pyb::self)
		.def(pyb::self - pyb::self)
		.def(pyb::self * pyb::self)
		.def(pyb::self / pyb::self)
		.def(pyb::self * float())
		.def(pyb::self / float())

		.def(pyb::self += pyb::self)
		.def(pyb::self -= pyb::self)
		.def(pyb::self *= pyb::self)
		.def(pyb::self /= pyb::self)
		.def(pyb::self *= float())
		.def(pyb::self /= float())

		.def(PYB_M(Cross))
		.def(PYB_M(Dist))
		.def(PYB_M(Dist2D))
		.def(PYB_M(DistSq))
		.def(PYB_M(DistSq2D))
		.def(PYB_M(Dot))
		.def(PYB_M(IsZero))
		.def(PYB_M(Length))
		.def(PYB_M(LengthSq))
		.def(PYB_M(Normalized))
		.def("__str__", [](const Vec& v) { return RS_STR(v); })

		.def_buffer(
			[](Vec& self) -> pyb::buffer_info {
				return pyb::buffer_info(
					&self.x, /* Pointer to buffer */
					sizeof(float), /* Size of one scalar */
					pyb::format_descriptor<float>::format(), /* Python struct-style format descriptor */
					1, /* Number of dimensions */
					{ 3 }, /* Buffer dimensions */
					{ 1 } /* Strides (in bytes) for each index */
				);
			})
		;

#define PYB_CUR_CLASS RotMat
	PYB_CLASS(RotMat)
		PYB_DEFAULT_INITS()
		.def(pyb::init<Vec, Vec, Vec>(), PYBA("forward"), PYBA("right"), PYBA("up"))
		.def("__getitem__", [](const RotMat& mat, std::pair <size_t, size_t> i) { return mat[i.first][i.second]; })
		.def("__setitem__", [](const RotMat& mat, std::pair <size_t, size_t> i, float val) { mat[i.first][i.second] = val; })

		PYBP(forward)
		PYBP(right)
		PYBP(up)

		.def_static(PYB_M(RotMat::GetIdentity))
		.def_static(PYB_M(RotMat::LookAt), PYBA("forward"), PYBA("up"))
		.def(PYB_M(Transpose))
		.def("dot", [](const RotMat& mat, const Vec& vec) { return mat.Dot(vec); })
		.def("dot", [](const RotMat& mat, const RotMat& other) { return mat.Dot(other); })

		.def("__str__", [](const RotMat& mat) { return RS_STR(mat);  })
		;

#define PYB_CUR_CLASS Angle
	PYB_CLASS(Angle)
		PYB_DEFAULT_INITS()
		.def(pyb::init<float, float, float>(), PYBA("yaw"), PYBA("pitch"), PYBA("roll"))

		PYBP(yaw)
		PYBP(pitch)
		PYBP(roll)

		.def_static(PYB_M(FromRotMat))
		.def(PYB_M(ToRotMat))
		.def(PYB_M(GetDeltaTo))
		.def(PYB_M(GetForwardVec))
		.def(PYB_M(NormalizeFix))

		.def("__str__", [](const Angle& ang) { return RS_STR(ang);  })
		;

#define PYB_CUR_CLASS CarConfig
		PYB_CLASS(CarConfig)
			PYB_DEFAULT_INITS()
			PYBP(backWheels)
			PYBP(dodgeDeadzone)
			PYBP(frontWheels)
			PYBP(hitboxPosOffset)

			.def_readonly_static("BREAKOUT", &CAR_CONFIG_BREAKOUT)
			.def_readonly_static("DOMINUS", &CAR_CONFIG_DOMINUS)
			.def_readonly_static("HYBRID", &CAR_CONFIG_HYBRID)
			.def_readonly_static("MERC", &CAR_CONFIG_MERC)
			.def_readonly_static("OCTANE", &CAR_CONFIG_OCTANE)
			.def_readonly_static("PLANK", &CAR_CONFIG_PLANK)
			;
}