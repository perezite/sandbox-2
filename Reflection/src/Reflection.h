#pragma once
#include "TextSerializer.h"

namespace sb {
	namespace reflection {
		template <class T>
		inline static std::string serialize(T& t, const std::string& typeName, size_t depth) {
			return TextSerializer::serialize(t, typeName, depth);
		}
	}
}
