#include "Demo5.h"
#include "Macro.h"
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
	
	class BaseProperty100 { };

	template <class T>
	class Property100 : public BaseProperty100 {
		T& _reference;
		std::string _name;
	public:
		Property100(T& reference, const std::string& name)
			: _reference(reference), _name(name)
		{ }
	};

	class BaseReflectable100 { 
		std::vector<BaseProperty100*> _properties;

	public:
		std::vector<BaseProperty100*>& getProperties() { 
			//if (!_propertiesInit)
			//	initProperties();
			//return _properties;
		}
		template <class T> void addProperty(T& value, const std::string& name) { _properties.push_back(new Property100<T>(value, name)); }
	};

	template <class T>
	class Reflectable100 : public BaseReflectable100 {
		typedef void(T::*Registration)();
		static std::vector<Registration> _registrations;
	public:
		static void addRegistration(Registration registration) {
			_registrations.push_back(registration);
		}
	};

	template <class T>
	std::vector<void(T::*)()> Reflectable100<T>::_registrations;

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

	class TextWriter100 {
	public:
		static void write(BaseReflectable100& reflectable, std::string& result) {
			
		}
	};

	void test() { }
	
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
	}

	void run() {
		demo100();
		// demo1000();
	}

	// demo5: reflectable (write, read, edit)
}
