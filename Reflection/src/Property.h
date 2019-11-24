#pragma once
#include "BaseProperty.h"
#include <string>
#include <vector>

namespace sb {
	template <class T>
	class Property : public BaseProperty {
		T _reference;
		std::string _typeName;
	public:
		Property(T& reference, const std::string& typeName)
			: _reference(reference), _typeName(typeName)
		{ }
		inline void inspect(size_t depth = 0) {
			return reflection::inspect(_reference, depth);
		}
	};
}