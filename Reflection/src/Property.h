#pragma once
#include "BaseProperty.h"
#include <string>
#include <vector>

namespace sb {
	namespace reflection {
		template <class T>
		static std::string serialize(T& t, const std::string& typeName, size_t depth);
	}

	template <class T>
	class Property : public BaseProperty {
		T _reference;
		std::string _typeName;
	public:
		Property(T& reference, const std::string& typeName)
			: _reference(reference), _typeName(typeName)
		{ }
		inline std::string serialize(size_t depth = 0) {
			 return reflection::serialize(_reference, _typeName, depth);
		}
		inline void inspect(size_t depth = 0) {
			return reflection::inspect(_reference, depth);
		}
	};
}