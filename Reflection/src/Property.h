#pragma once
#include "BaseProperty.h"
#include "Reflection.h"
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
		inline std::string serialize(size_t depth = 0) {
			return Reflection::serialize(_reference, _typeName, depth);
		}
	};

}