#include "Demo8.h"
#include "Logger.h"
#include "Macro.h"
#include <vector>
#include <map>
#include <algorithm>

namespace reflectionDemo8 {
	using namespace std;

	/*
	class MyClass {
	public:
		SB_PROPERTY(int, myInt)
		SB_PROPERTY(int* myIntPointer)
	public:
		MyClass() : myIntPointer(&myInt) {
		
		}
	}

	int myExternalInt;

	class MyInvalidClass {
	public:
		SB_PROPERTY(int*, myExternalIntPointer);
	public:
		MyInvalidClass() : myExternalIntPointer(&myExternalInt) {
		}
	}

	void demo1000() {
		// write
		MyClass myClass;
		myClass.myInt = 42;
		std::ostringstream os;
		TextWriter::write(myClass, os);
		cout << os.str() << endl;

		// read
		MyClass myClass2;
		std::istringstream is(os.str());
		TextReader::read(myClass2, is);
		cout << myClass2.myInt << endl;
		cout << *myClass2.myIntPointer << endl;

		// edit
		string index; string value;
		while (value != "exit") {
			TextWriter::write(myClass2, cout, true);
			cout << "Enter the index of the property to edit: ";
			getline(cin, index);
			cout << "Enter the new value for the property (or exit to leave): ";
			getline(cin, value);
			ConsoleEditor::edit(parse<int>(index), value);
		}

		// prevent external pointers

	}
	*/

	// https://stackoverflow.com/questions/2631585/c-how-to-require-that-one-template-type-is-derived-from-the-other
	template <typename B, typename D>
	struct is_base_of // check if B is a base of D
	{
		typedef char yes[1];
		typedef char no[2];

		static yes& test(B*) {};
		static no& test(...) {};

		static D* get(void) {};

		static const bool value = (sizeof(test(get())) == sizeof(yes));
	};


	namespace reflection {
		static void setInspector(const std::string& inspectorName);
		template <class T> static void inspect(T& t, const std::string& name, size_t depth);
	}

	class BaseReflectable0;
	class BaseProperty0 {
		std::string _name;
	public:
		BaseProperty0(const std::string name) : _name(name)
		{ }
		const std::string& getName() { return _name; }
		virtual const bool isReflectable() const = 0;
		virtual void inspect(size_t depth) = 0;
		virtual vector<BaseProperty0*> getChildProperties() = 0;
		virtual BaseReflectable0& getValueAsReflectable() = 0;
	};

	class BaseReflectable0 {
	public:
		virtual std::vector<BaseProperty0*>& getProperties() = 0;
	};

	class BaseProperty0;
	template <class T> class Property0;

	template <bool, class TBase, class TCurrent>
	struct returnBaseIf0;

	template < class TBase, class TCurrent>
	struct returnBaseIf0<false, TBase, TCurrent> {
		static TCurrent& value(TCurrent& t) {
			return t;
		}
	};

	template <class TBase, class TCurrent>
	struct returnBaseIf0<true, TBase, TCurrent> {
		static TBase& value(TCurrent& t) {
			return (TBase&)t;
		}
	};

	// converts the object to the base type and returns it, if possible. Otherwise, just returns the input object unchanged
	#define TRY_CONVERT_TO_BASE800(potentialBaseType, currentType, object) \
		returnBaseIf0<is_base_of<potentialBaseType, currentType>::value, potentialBaseType, currentType>::value(object)

	template <class T>
	class Reflectable0 : public BaseReflectable0 {
		typedef void(T::*Registration)();
		static std::vector<Registration> PropertyRegistrations;
		static std::vector<std::string> PropertyRegistrationNames;
		static bool RegistrationsInitialized;
		std::vector<BaseProperty0*> _properties;
		bool _propertiesInitialized;
	protected:
		virtual void initProperties() {
			T* instance = (T*)this;
			for (size_t i = 0; i < PropertyRegistrations.size(); i++)
				(instance->*PropertyRegistrations[i])();
			_propertiesInitialized = true;
		}
		static bool findRegistrationName(std::string name) {
			return std::find(PropertyRegistrationNames.begin(), PropertyRegistrationNames.end(), name)
				!= PropertyRegistrationNames.end();
		}
		template <class TConverted> void addConvertedProperty(TConverted& value, const std::string& name) {
			_properties.push_back(new Property0<TConverted>(value, name));
		}
	public:
		Reflectable0() : _propertiesInitialized(false)
		{ }
		static void addRegistration(Registration registration, const std::string name) {
			if (findRegistrationName(name))
				RegistrationsInitialized = true;
			else
				PropertyRegistrationNames.push_back(name);
			if (!RegistrationsInitialized) {
				PropertyRegistrations.push_back(registration);
			}
		}
		virtual std::vector<BaseProperty0*>& getProperties() {
			if (!_propertiesInitialized) {
				initProperties();
				_propertiesInitialized = true;
			}
			return _properties;
		}
		template <class U> void addProperty(U& value, const std::string& name) {
			addConvertedProperty(TRY_CONVERT_TO_BASE800(BaseReflectable0, U, value), name);
		}
	};

	template <class T> std::vector<void(T::*)()> Reflectable0<T>::PropertyRegistrations;
	template <class T> std::vector<std::string> Reflectable0<T>::PropertyRegistrationNames;
	template <class T> bool Reflectable0<T>::RegistrationsInitialized = false;

	template <bool IsReflectable, class T> struct Inspection0 { };
	template <class T> struct Inspection0<true, T> {
		static void inspect(T& t, const std::string& name, size_t depth) {
			SB_ERROR("Do not call inspect on properties which derived from a reflectable");
		}
	};
	template <class T> struct Inspection0<false, T> {
		static void inspect(T& t, const std::string& name, size_t depth) {
			reflection::inspect(t, name, depth);
		}
	};

	template <class T>
	class Property0 : public BaseProperty0 {
		T& _reference;
	public:
		Property0(T& reference, const std::string& name)
			: BaseProperty0(name), _reference(reference) { }
		const bool isReflectable() const {
			return is_base_of<BaseReflectable0, T>::value;
		}
		void inspect(size_t depth) {
			const bool derivesFromReflectable = is_base_of<BaseReflectable0, T>::value;
			Inspection0<derivesFromReflectable, T>::inspect(_reference, getName(), depth);
		}
		vector<BaseProperty0*> getChildProperties() {
			if (isReflectable()) {
				auto reflectable = (BaseReflectable0*)(&_reference);
				return reflectable->getProperties();
			}
			return vector<BaseProperty0*>();
		}
		BaseReflectable0& getValueAsReflectable() { 
			SB_ERROR_IF(!isReflectable(), "Trying to get property value as reflectable from a primitive (i.e. non-reflectable) property");
			return (BaseReflectable0&)_reference;
		}
	};

	template <void(*Action)()>
	class Invocation0 {
	public:
		Invocation0() {
			Action();
		}
	};

	template <class T>
	std::string stringify(const T& t) {
		static std::ostringstream os; os.str(""); os << t;
		return os.str();
	}

	class MyInnerReflectable0 : public Reflectable0<MyInnerReflectable0> {
		double _myDouble;
		void addProperty_myDouble() {
			addProperty(_myDouble, SB_NAMEOF(_myDouble));
		}
		static void register_myDouble() {
			addRegistration(&MyInnerReflectable0::addProperty_myDouble, SB_NAMEOF(_myDouble));
		}
		Invocation0<register_myDouble> invoke_register_myInt;
	public:
		void setMyDouble(double myDouble) { _myDouble = myDouble; }
		double getMyDouble() const { return _myDouble; }
	};

	class MyReflectable0 : public Reflectable0<MyReflectable0> {
		int _myInt;
		void addProperty_myInt() {
			addProperty(_myInt, SB_NAMEOF(_myInt));
		}
		static void register_myInt() {
			addRegistration(&MyReflectable0::addProperty_myInt, SB_NAMEOF(_myInt));
		}
		Invocation0<register_myInt> invoke_register_myInt;
		int* _myIntPointer;
		void addProperty_myIntPointer() {
			addProperty(_myIntPointer, SB_NAMEOF(_myIntPointer));
		}
		static void register_myIntPointer() {
			addRegistration(&MyReflectable0::addProperty_myIntPointer, SB_NAMEOF(_myIntPointer));
		}
		Invocation0<register_myIntPointer> invoke_register_myIntPointer;
		float _myFloat;
		void addProperty_myFloat() {
			addProperty(_myFloat, SB_NAMEOF(_myFloat));
		}
		static void register_myFloat() {
			addRegistration(&MyReflectable0::addProperty_myFloat, SB_NAMEOF(_myFloat));
		}
		Invocation0<register_myFloat> invoke_register_myFloat;
		MyInnerReflectable0 _myInnerReflectable;
		void addProperty_myInnerReflectable() {
			addProperty((BaseReflectable0&)_myInnerReflectable, SB_NAMEOF(_myInnerReflectable));
		}
		static void register_myInnerReflectable() {
			addRegistration(&MyReflectable0::addProperty_myInnerReflectable, SB_NAMEOF(_myInnerReflectable));
		}
		Invocation0<register_myInnerReflectable> invoke_register_myInnerReflectable;
	public:
		int getMyInt() const { return _myInt; }
		int& getMyInt() { return _myInt; }
		int* getMyPointer() { return _myIntPointer; }
		float getMyFloat() const { return _myFloat; }
		MyInnerReflectable0& getMyInnerRefletable() { return _myInnerReflectable; }
		void setMyInt(int myInt) { _myInt = myInt; }
		void setMyIntPointer(int* myIntPointer) { _myIntPointer = myIntPointer; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
	};

	class Inspector0 {
		struct Pointer { size_t sourceId; void* target; };
		static vector<void*> Properties;
		static vector<Pointer> Pointers;
	public:
		static vector<Pointer>& getPointers() { return Pointers; }
		static size_t findPropertyId(void* myProperty) {
			auto it = find(Properties.begin(), Properties.end(), myProperty);
			SB_ERROR_IF(it == Properties.end(), "Property could not be found");
			return distance(Properties.begin(), it);
		}
		template <class T> static size_t storePropertyAndGetId(T& myProperty) {
			auto it = find(Properties.begin(), Properties.end(), &myProperty);
			if (it != Properties.end())
				return distance(Properties.begin(), it);
			Properties.push_back(&myProperty);
			return Properties.size() - 1;
		}	
		template <class T> static size_t storePointerAndGetId(T& t) {
			auto id = storePropertyAndGetId(t);
			Pointer pointer; pointer.sourceId = id; pointer.target = t;
			Pointers.push_back(pointer);
			return id;
		}
		static void clearPropertiesAndPointers() {
			Properties.clear();
			Pointers.clear();
		}
	};

	vector<void*> Inspector0::Properties;
	vector<Inspector0::Pointer> Inspector0::Pointers;

	class TextWriter0 : public Inspector0 {
		static std::ostream* Stream;
	protected:
		static std::ostream& getStream() { return *Stream; }
		static void write(vector<BaseProperty0*> properties, const std::string& name, size_t depth) {
			for (size_t i = 0; i < properties.size(); i++) {
				if (properties[i]->isReflectable())
					writeReflectableProperty(*properties[i], properties[i]->getName(), depth + 1);
				else
					properties[i]->inspect(depth + 1);
			}
		}
		static void writePointers() {
			getStream() << "pointers" << endl;
			auto pointers = getPointers();
			for (size_t i = 0; i < pointers.size(); i++) {
				cout << " " << pointers[i].sourceId << " " << findPropertyId(pointers[i].target) << endl;
			}
		}
	public:
		template <class T> static void write(T& t, const string& name, size_t depth) {
			getStream() << std::string(depth, ' ') << name << " " << t << " " << storePropertyAndGetId(t) << endl;
		}
		template <class T> static void write(T* t, const string& name, size_t depth) {
			getStream() << std::string(depth, ' ') << name << " pointer " << storePointerAndGetId(t) << endl;
		}
		static void writeReflectableProperty(BaseProperty0& theProperty, const string& name, size_t depth) {
			getStream() << std::string(depth, ' ') << name << " " << storePropertyAndGetId(theProperty.getValueAsReflectable()) << " " << endl;
			write(theProperty.getChildProperties(), name, depth);
		}
		static void write(BaseReflectable0& reflectable, std::ostream& os) {
			reflection::setInspector(SB_NAMEOF(TextWriter0));
			Stream = &os;
			clearPropertiesAndPointers();
			write(reflectable.getProperties(), "root", 0);
			writePointers();
		}
	};

	std::ostream* TextWriter0::Stream;

	namespace reflection {
		static std::string CurrentInspectorName;
		static void setInspector(const std::string& inspectorName) { CurrentInspectorName = inspectorName; }
		template <class T> static void inspect(T& t, const std::string& name, size_t depth) {
			if (CurrentInspectorName == SB_NAMEOF(TextWriter0))
				TextWriter0::write(t, name, depth);
			else
				SB_ERROR("Inspector " << CurrentInspectorName << " not found");
		}
	}

	void demo0() {
		// pointer write
		// expected output:
		// _myInt 42 0
		// _myIntPointer pointer 1
		// _myFloat 1.234 2
		// _myInnerReflectable 3
		//  _myDouble 9.876 4
		// pointers
		//  1 0
		MyReflectable0 myReflectable;
		myReflectable.setMyInt(42);
		myReflectable.setMyIntPointer(&myReflectable.getMyInt());
		myReflectable.setMyFloat(1.234f);
		myReflectable.getMyInnerRefletable().setMyDouble(9.876);
		TextWriter0::write(myReflectable, cout);
	}

	void run() {
		// demo8: link pointers (write, raed, edit)
		demo0();
		//demo1000();
	}
}
