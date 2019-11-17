#pragma once
#include "BaseProperty.h"
#include <string>
#include <vector>

namespace sb {
	namespace reflection {
		template <class T>
		static std::string serialize(T& t, const std::string& typeName, size_t depth);
	}

	class BaseReflectable {
	public:
		inline std::string serialize(size_t depth = 0) {
			return reflection::serialize(*this, getTypeName(), depth);
		}
		virtual const std::string getTypeName() = 0;
		virtual	const std::vector<BaseProperty*>& getProperties() = 0;
	};
}