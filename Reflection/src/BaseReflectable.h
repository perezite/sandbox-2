#pragma once
#include "BaseProperty.h"
#include <string>
#include <vector>

namespace sb {
	class BaseReflectable {
	public:
		virtual const std::string getTypeName() = 0;
		virtual	const std::vector<BaseProperty*>& getProperties() = 0;
	};
}