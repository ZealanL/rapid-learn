#pragma once
#include "Framework.h"

namespace RPLC {

	template<typename T>
	const std::unordered_map<std::string, T>& GetEnumStrMap();

	template<typename T>
	struct StringEnum {
		T val;

		StringEnum() : val({}) {}
		StringEnum(T val) : val(val) {}
		StringEnum(std::string name) {
			auto& map = GetEnumStrMap<T>();
			for (char& c : name)
				c = tolower(c);

			if (map.empty())
				RG_ERR_CLOSE("No valid string values exist for type\"" << typeid(T).name() << "\".");

			auto itr = map.find(name);
			if (itr == map.end()) {
				std::stringstream errorMsg;
				errorMsg << "\"" << name << "\" is not a valid value of \"" << typeid(T).name() << "\". Valid types are: ";

				for (auto citr = map.begin(); citr != map.end(); citr++) {
					if (citr != map.begin())
						errorMsg << ", ";
					errorMsg << "\"" << citr->first << "\"";
				}

				RG_ERR_CLOSE(errorMsg.str());
			}
		}

		operator T() const {
			return (T)val;
		}
	};
}