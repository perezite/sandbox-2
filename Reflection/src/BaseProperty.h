#pragma once
#include <string>

namespace sb {
	class BaseProperty {
	public:
		virtual ~BaseProperty() { }
		virtual std::string serialize(size_t depth = 0) = 0;
	};
}