#include "Demo9.h"
#include "Logger.h"
#include "Macro.h"
#include <vector>
#include <algorithm>
using namespace std;

namespace reflectionDemo9 {

	/*
	void demo() {
		// write
		MyDerived myDerived;
		myDerived.setMyInt(42);
		myDerived.setMyFloat(1.2345f);
		ostringstream os;
		TextWriter::write(myDerived, os);

		// read
		MyDerived myDerived2;
		istringstream is(os);
		TextReader::read(myDerived2, is);
		cout << myDerived2.getMyInt() << endl;
		cout << myDerived2.getMyFloat() << endl;

		// edit
		string value; size_t index;
		while(value != "exit") {
		TextWriter::write(myDerived2, cout, true);
		cout << "Enter the index of the property to edit: ";
		getline(cin, index);
		cout << "Enter the new value for the property (or exit to leave): ";
		getline(cin, value);
		ConsoleEditor::edit(parse<int>(index), value);
		}
	}
	*/

	template <class T>
	std::string stringify(const T& t) {
		static std::ostringstream os; os.str(""); os << t;
		return os.str();
	}

	template <class T> void setAddressFromVoidPointer(T& target, void* address) {
		SB_ERROR("Target is not a pointer type");
	}

	template <class T> void setAddressFromVoidPointer(T*& target, void* address) {
		SB_ERROR_IF(target != NULL, "The target was not NULL before assignment. To prevent memory leaks, set the target to NULL before assigning an address");
		target = (T*)address;
	}

	namespace reflection {
		static void setInspector(const std::string& inspectorName);
		template <class T> static void inspect(T& t, const std::string& name, size_t depth);
	}

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

	template <bool, class TBase, class TCurrent>
	struct returnBaseIf0;

	template < class TBase, class TCurrent>
	struct returnBaseIf0<false, TBase, TCurrent> {
		static TCurrent& value(TCurrent& t) {
			return t;
		}
	};

	// converts the object to the base type and returns it, if possible. Otherwise, just returns the input object unchanged
	#define TRY_CONVERT_TO_BASE0(potentialBaseType, currentType, object) \
		returnBaseIf0<is_base_of<potentialBaseType, currentType>::value, potentialBaseType, currentType>::value(object)

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
		virtual void* getVoidPointerToValue() = 0;
		virtual void setAddress(void* address) = 0;
	};

	class BaseReflectable0 {
	public:
		virtual vector<BaseProperty0*>& getProperties() = 0;
	};

	template <class T> class Property0;

	template <class T>
	class Reflectable0 : public BaseReflectable0 {
		typedef void(*Registration)(void*);
		static std::vector<Registration> PropertyRegistrations;
		static std::vector<std::string> PropertyRegistrationNames;
		static bool RegistrationsInitialized;
		std::vector<BaseProperty0*> _properties;
		bool _propertiesInitialized;
	protected:
		virtual void initProperties() {
			auto instance = (void*)this;
			for (size_t i = 0; i < PropertyRegistrations.size(); i++)
				(*PropertyRegistrations[i])(instance);
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
			if (!RegistrationsInitialized) 
				PropertyRegistrations.push_back(registration);
		}
		virtual std::vector<BaseProperty0*>& getProperties() {
			if (!_propertiesInitialized) {
				initProperties();
				_propertiesInitialized = true;
			}
			return _properties;
		}
		template <class U> void addProperty(U& value, const std::string& name) {
			addConvertedProperty(TRY_CONVERT_TO_BASE0(BaseReflectable0, U, value), name);
		}
	};

	template <class T> std::vector<void(*)(void*)> Reflectable0<T>::PropertyRegistrations;
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
		void* getVoidPointerToValue() { return (void*)(&_reference); }
		void setAddress(void* address) {
			setAddressFromVoidPointer(_reference, address);
		}
	};

	class Writer0 {
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
		static void initInspector() {
			Properties.clear();
			Pointers.clear();
		}
	};

	vector<void*> Writer0::Properties;
	vector<Writer0::Pointer> Writer0::Pointers;

	class TextWriter0 : public Writer0 {
		static std::ostream* Stream;
		static bool PrettyPrint;
		static size_t Counter;
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
				getStream() << " " << pointers[i].sourceId << " " << findPropertyId(pointers[i].target) << endl;
			}
		}
		static string getCounterDescription(size_t value) {
			return PrettyPrint ? " (" + stringify(value) + ")" : "";
		}
	public:
		template <class T> static void write(T& t, const string& name, size_t depth) {
			getStream() << std::string(depth, ' ') << name << " " << t << " " <<
				storePropertyAndGetId(t) << getCounterDescription(Counter++) << endl;
		}
		template <class T> static void write(T* t, const string& name, size_t depth) {
			getStream() << std::string(depth, ' ') << name << " pointer " <<
				storePointerAndGetId(t) << getCounterDescription(Counter++) << endl;
		}
		static void writeReflectableProperty(BaseProperty0& theProperty, const string& name, size_t depth) {
			getStream() << std::string(depth, ' ') << name << " " << storePropertyAndGetId(theProperty.getValueAsReflectable()) << " " << endl;
			write(theProperty.getChildProperties(), name, depth);
		}
		static void write(BaseReflectable0& reflectable, std::ostream& os, bool prettyPrint = false) {
			reflection::setInspector(SB_NAMEOF(TextWriter0));
			Stream = &os;
			Counter = 0;
			PrettyPrint = prettyPrint;
			initInspector();
			write(reflectable.getProperties(), "root", -1);
			if (!PrettyPrint)
				writePointers();
		}
	};

	std::ostream* TextWriter0::Stream;
	bool TextWriter0::PrettyPrint = false;
	size_t TextWriter0::Counter = 0;

	template <void(*Action)()>
	class Invocation0 {
	public:
		Invocation0() {
			Action();
		}
	};

	class MyBase0 : public Reflectable0<MyBase0> {
		int _myInt;
		static void addProperty_myInt(void* instance) {
			auto typedInstance = (MyBase0*)instance;
			typedInstance->addProperty(typedInstance->_myInt, SB_NAMEOF(_myInt));
		}
		static void register_myInt() {
			addRegistration(&addProperty_myInt, SB_NAMEOF(_myInt));
		}
		Invocation0<register_myInt> invoke_register_myInt;
	public:
		int getMyInt() const { return _myInt; }
		void setMyInt(int myInt) { _myInt = myInt; }
	};

	class MyDerived0 : public MyBase0 {
		float _myFloat;
		void addProperty_myFloat() {
			addProperty(_myFloat, SB_NAMEOF(_myFloat));
		}
		static void register_myFloat() {
			// NEXT: Invoke this properly
		}
		Invocation0<register_myFloat> invoke_register_myFloat;
	public:
		float getMyFloat() const { return _myFloat; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
	};

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
		// write
		MyDerived0 myDerived;
		myDerived.setMyInt(42);
		myDerived.setMyFloat(1.2345f);
		TextWriter0::write(myDerived, cout);
	}

	void run() {
		// demo9: inheritance (write, read, edit)
		demo0();
	}
}

