#pragma once


namespace sb {
	class Reflection {
	public:
		template <class T>
		static std::string serialize(T& t, const std::string& typeName, size_t depth) {
			//return SB_SERIALIZER::serialize(t, typeName, depth);
			return "TODO";
		}
	};
}