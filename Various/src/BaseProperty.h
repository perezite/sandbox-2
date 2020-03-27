#pragma once
#include <string>

namespace sb {
	class BaseProperty {
	public:
		virtual ~BaseProperty() { }
		virtual void inspect(size_t depth = 0) = 0;
	};
}