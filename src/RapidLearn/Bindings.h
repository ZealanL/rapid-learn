#pragma once
#include "Framework.h"

#include "StringEnum.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
namespace pyb = pybind11;

namespace RPLC {
	namespace Bindings {
		typedef std::function<void(pyb::module&)> InitFn;

		struct AddInitFunc {
			AddInitFunc(InitFn fn);

			RG_NO_COPY(AddInitFunc);
		};
	}
}

#define RPLC_ADD_BIND(name) \
void _BindInitFunc_##name(pyb::module& m); \
auto _g_BindInitFunc_##name = RPLC::Bindings::AddInitFunc(_BindInitFunc_##name); \
void _BindInitFunc_##name(pyb::module& m) 

///////////////////

// Set this to the current class we are working with
#define PYB_CUR_CLASS null

#define PYB_CLASS(className) pyb::class_<className>(m, #className)
#define PYB_CLASS_SH(className) pyb::class_<className, std::shared_ptr<className>>(m, #className)
#define PYB_CLASS_M(mod, className) pyb::class_<className>(mod, #className)
#define PYB_CLASS_MVB(mod, className, baseName) pyb::class_<className, std::shared_ptr<className>, baseName>(mod, #className)
#define PYB_CLASS_V(className, trampolineClassName) pyb::class_<className, std::shared_ptr<className>, trampolineClassName>(m, #className)
#define PYB_INIT_F(name) void PYB_INIT_##name(pyb::module& m)

// Add default constuctor
#define PYB_DEFAULT_INIT() .def(pyb::init<>())

// Add default and copy constructor
#define PYB_DEFAULT_INITS() \
	.def(pyb::init<>()) \
	.def(pyb::init<const PYB_CUR_CLASS&>())

// Add arg annotation
#define PYBA pyb::arg

// Make python string
#define PYBS(s) PYB_MakePythonString(s)

// Turns member into member python name and member qualified name ref
#define PYB_M(memberName) PYBS(#memberName), &PYB_CUR_CLASS::memberName

// Property
#define PYBP(memberName) .def_readwrite(PYB_M(memberName))
#define PYBPR(memberName) .def_readonly(PYB_M(memberName))

#define PYBP_W(memberName) .def_property( \
	PYBS(#memberName), \
	[](const PYB_CUR_CLASS& inst) { return inst.ptr->memberName; }, \
	[](const PYB_CUR_CLASS& inst, const decltype(inst.ptr->memberName)& newVal) { inst.ptr->memberName = newVal; } \
	)

#define PYB_ENUM(enumName) pyb::enum_<enumName>(m, #enumName)
#define PYB_ENUM_VAL(val) .value(PYBS(#val), val)
#define PYB_ENUM_END() .export_values()

// Virtual override
#define PYB_V_WRAP(retType, className, funcName, pyName, defArgs, callArgs) \
virtual retType pyName defArgs { return className::funcName callArgs; } \
retType funcName defArgs override { return this->pyName callArgs; }

// TODO: Literally leaks memory, but should only be called once for each string, so its fine
inline const char* PYB_MakePythonString(const char* name) {
	std::string* result = new std::string();
	char last = NULL;
	bool isInAcronym = false;
	for (const char* pc = name; *pc; pc++) {
		char c = *pc;
		if (isupper(c)) {
			if (last && isupper(last)) {
				isInAcronym = true;
			} else if (last && last != '_') {
				*result += "_";
			}
		} else {
			if (isInAcronym) {
				if (last != '_')
					*result += "_";
				isInAcronym = false;
			}
		}
		if (c != '_')
			*result += tolower(c);

		last = c;
	}

	size_t colonPos = result->rfind(':');
	if (colonPos != std::string::npos)
		result->erase(result->begin(), result->begin() + colonPos + 1);
	return result->c_str();
}