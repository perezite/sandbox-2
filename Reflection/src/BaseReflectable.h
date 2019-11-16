#pragma once
#include "BaseProperty.h"
#include "Reflection.h"
#include <string>
#include <vector>

namespace sb {
	class BaseReflectable {
	public:
		std::string serialize(size_t depth = 0) {
			return Reflection::serialize(*this, getTypeName(), depth);
		}
		virtual const std::string getTypeName() = 0;
		virtual	const std::vector<BaseProperty*>& getProperties() = 0;
	};
}