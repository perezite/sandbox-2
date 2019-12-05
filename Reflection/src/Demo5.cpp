#include "Demo5.h"
#include "Macro.h"
#include "Logger.h"
#include <string>
#include <vector>

namespace reflectionDemo5 {

	void demo1000() {
		/*
		MyReflectable myReflectable;
		myReflectable.setMyInt(42);
		myReflectable.setMyFloat(3.1415f);
		std::string result;
		TextWriter::write(myReflectable, result);
		MyReflectable myReflectable2;
		TextReader::read(myReflectable2, result);
		ConsoleEditor::edit(myReflectable);
		*/
	}
	
	namespace reflection {
		static void setInspector(const std::string& inspectorName);
		//static std::string& getInspector();
		template <class T> static void inspect(T& t, const std::string& name, size_t depth, std::string& result);
	}

	class BaseProperty100 { 
	public:
		virtual void inspect(std::string& result, size_t depth) = 0;
	};

	template <class T>
	class Property100 : public BaseProperty100 {
		T& _reference;
		std::string _name;
	public:
		Property100(T& reference, const std::string& name)
			: _reference(reference), _name(name)
		{ }
		virtual void inspect(std::string& result, size_t depth) {
			reflection::inspect(_reference, _name, depth, result);
		}
	};

	class BaseReflectable100 { 
	public:
		virtual std::vector<BaseProperty100*>& getProperties() = 0;
	};

	template <class T>
	class Reflectable100 : public BaseReflectable100 {
		typedef void(T::*Registration)();
		static std::vector<Registration> PropertyRegistrations;
		std::vector<BaseProperty100*> _properties;
		bool _propertiesRegistered;
	protected:
		virtual void initProperties() {
			T* instance = (T*)this;
			for (size_t i = 0; i < PropertyRegistrations.size(); i++)
				(instance->*PropertyRegistrations[i])();
			_propertiesRegistered = true;
		}
	public:
		Reflectable100() : _propertiesRegistered(false)
		{ }
		static void addRegistration(Registration registration) {
			PropertyRegistrations.push_back(registration);
		}
		virtual std::vector<BaseProperty100*>& getProperties() {
			if (!_propertiesRegistered) {
				initProperties();
				_propertiesRegistered = true;
			}
			return _properties;
		}
		template <class U> 
		void addProperty(U& value, const std::string& name) { _properties.push_back(new Property100<U>(value, name)); }
	};

	template <class T>
	std::vector<void(T::*)()> Reflectable100<T>::PropertyRegistrations;

	template <void(*Action)()>
	class Invocation100 {
	public:
		Invocation100() {
			Action();
		}
	};

	class MyReflectable100 : public Reflectable100<MyReflectable100> {
		int _myInt;
		void addProperty_myInt() {
			addProperty(_myInt, SB_NAMEOF(_myInt));
		}
		static void register_myInt() {	
			addRegistration(&MyReflectable100::addProperty_myInt);
		}
		Invocation100<register_myInt> invoke_register_myInt;
		float _myFloat;
	public:
		void setMyInt(int myInt) { _myInt = myInt; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
	};

	template <class T>
	std::string stringify(T& t) {
		static std::ostringstream os; os << t;
		return os.str();
	}

	class TextWriter100 {
		static void print(const std::string& name, const std::string& value, size_t depth, std::string& result) {
			result += std::string(depth, ' ') + name + ' ' + value + '\n';
		}
	public:
		static void writeProperty(BaseReflectable100& reflectable, const std::string& name, size_t depth, std::string& result) {
			result += std::string(depth, ' ') + name + '\n';
			auto properties = reflectable.getProperties();
			for (size_t i = 0; i < properties.size(); i++)
				properties[i]->inspect(result, depth + 1);
		}
		template <class T> static void writeProperty(const T& t, const std::string& name, size_t depth, std::string& result) {
		}
		static void write(BaseReflectable100& reflectable, std::string& result) {
			reflection::setInspector(SB_NAMEOF(TextWriter100));
			writeProperty(reflectable, "root", 0, result);
		}
	};

	template <> void TextWriter100::writeProperty<int>(const int& t, const std::string& name, size_t depth, std::string& result) {
		print(name, stringify(t), depth, result);
	}

	namespace reflection {
		static std::string CurrentInspectorName;
		static void setInspector(const std::string& inspectorName) { CurrentInspectorName = inspectorName; }
		template <class T> static void inspect(T& t, const std::string& name, size_t depth, std::string& result) {
			if (CurrentInspectorName == SB_NAMEOF(TextWriter100))
				TextWriter100::writeProperty(t, name, depth, result);
			else
				SB_ERROR("Inspector " << CurrentInspectorName << " not found");
		}
	}

	void demo100() {
		// Write reflectable
		// output:
		// _
		// __myInt 42
		// __myFloat 3.1415f
		MyReflectable100 myReflectable;
		myReflectable.setMyInt(42);
		myReflectable.setMyFloat(3.1415f);
		std::string result;
		TextWriter100::write(myReflectable, result);
		std::cout << result << std::endl;
	}

	void run() {
		demo100();
		// demo1000();
	}

	// demo5: reflectable (write, read, edit)
}
