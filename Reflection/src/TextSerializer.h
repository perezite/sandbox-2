#pragma once
#include "BaseReflectable.h"
#include "InheritanceCheck.h"
#include "Logger.h"
#include "Macro.h"
#include <string>
#include <sstream>

namespace sb {
	namespace reflection {
		static void setInspector(const std::string& inspectorTypeName_);
	}
}

namespace sb {
	class TextSerializer {
		static std::string _result;

	protected:
		static std::string serializeReflectable(BaseReflectable& baseReflectable, size_t depth = 0) {
			std::ostringstream os; os << std::string(depth, ' ') << baseReflectable.getTypeName() << std::endl;
			auto& properties = baseReflectable.getProperties();
			for (size_t i = 0; i < properties.size(); i++)
				os << properties[i]->serialize(depth + 1);
			return os.str();
		}
		template <class T>
		static std::string serializePrimitive(T& t, const std::string typeName, size_t depth) {
			std::ostringstream os; os << std::string(depth, ' ') << typeName << std::endl
				<< std::string(depth + 1, ' ') << t << std::endl; return os.str();
		}
		static std::string stripTemplateTypename(const std::string& typeName) {
			auto left = typeName.find('<');
			auto right = typeName.rfind('>');
			return typeName.substr(left + 1, right - left - 1);
		}
		static void inspectReflectable(BaseReflectable& reflectable, size_t depth) {
			auto properties = reflectable.getProperties();
			for (size_t i = 0; i < properties.size(); i++)
				properties[i]->inspect(depth + 1);
		}
		template <class T>
		static void serializePrimitive2(T& t, size_t depth) {
			std::ostringstream os; os << std::string(depth, ' ') << t << std::endl;
			_result += os.str();
		}
	public:
		template <class T>
		static std::string serialize(T& t, const std::string& typeName, size_t depth = 0) {
			if (InheritanceCheck<T, BaseReflectable>::value()) {
				return serializeReflectable((BaseReflectable&)t, depth);
			}

			SB_ERROR("serialization not specified for type '" << typeName << "'");
			return "";
		}
		template <class T>
		static std::string serialize(std::vector<T>& v, const std::string& typeName, size_t depth = 0) {
			std::ostringstream os; os << std::string(depth, ' ') << typeName << std::endl;
			for (auto it = v.begin(); it != v.end(); it++)
				os << serialize(*it, stripTemplateTypename(typeName), depth + 1);
			return os.str();
		}
		template <class T>
		static void inspect(T& t, size_t depth = 0) {
			if (sb::InheritanceCheck<T, sb::BaseReflectable>::value()) {
				inspectReflectable((BaseReflectable&)t, depth);
				return;
			}

			SB_ERROR("Type not serializable");
		}
		template <class T>
		static std::string toString(T& t, size_t depth = 0) {
			sb::reflection::setInspector(SB_NAMEOF(sb::TextSerializer));
			_result = std::string();
			inspect<T>(t, depth);
			return _result;
		}
	};

	template <>
	std::string TextSerializer::serialize<int>(int& t, const std::string& typeName, size_t depth) {
		return serializePrimitive(t, typeName, depth);
	}

	template <>
	std::string TextSerializer::serialize<float>(float& t, const std::string& typeName, size_t depth) {
		return serializePrimitive(t, typeName, depth);
	}

	template <>
	std::string TextSerializer::serialize<std::string>(std::string& t, const std::string& typeName, size_t depth) {
		return serializePrimitive(t, typeName, depth);
	}

	template <>
	void TextSerializer::inspect<int>(int& t, size_t depth) { serializePrimitive2(t, depth); }

	template <>
	void TextSerializer::inspect<float>(float& t, size_t depth) { serializePrimitive2(t, depth); }

	std::string TextSerializer::_result;
}