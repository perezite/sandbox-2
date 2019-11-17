#pragma once
#ifndef SB_SERIALIZER
	#include "TextSerializer.h"
	#define SB_SERIALIZER sb::TextSerializer
#endif 

namespace sb {
	namespace reflection {
		template <class T>
		inline static std::string serialize(T& t, const std::string& typeName, size_t depth) {
			return SB_SERIALIZER::serialize(t, typeName, depth);
		}
	}
}
