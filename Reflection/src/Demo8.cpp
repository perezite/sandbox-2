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
		virtual void* getVoidPointerToValue() = 0;
		virtual void setAddress(void* address) = 0;
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

	template <class T> void setAddressFromVoidPointer(T& target, void* address) {
		SB_ERROR("Target is not a pointer type");
	}

	template <class T> void setAddressFromVoidPointer(T*& target, void* address) {
		SB_ERROR_IF(target != NULL, "The target was not NULL before assignment. To prevent memory leaks, set the target to NULL before assigning an address");
		target = (T*)address;
	}

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

	template <class T>
	T parse(const std::string& input) {
		std::istringstream is(input);
		T result; is >> result;
		return result;
	}

	int countStart(const std::string& str, char token) {
		size_t counter = 0;
		for (size_t i = 0; i < str.length(); i++) {
			if (str[i] == token)
				counter++;
			else
				break;
		}

		return counter;
	}

	void split(const std::string& s, const std::string& delimiter, std::vector<std::string>& result) {
		size_t start = 0;
		size_t pos = 0;
		size_t delimiterLen = delimiter.length();
		while (true) {
			pos = s.find(delimiter, start);
			if (pos != std::string::npos) {
				size_t len = pos - start;
				if (len > 0)
					result.emplace_back(s.substr(start, len));
				start = pos + delimiterLen;
			}
			else {
				size_t len = s.length() - start;
				if (len > 0)
					result.emplace_back(s.substr(start, len));
				break;
			}
		}
	}

	BaseProperty0* findProperty(const vector<BaseProperty0*>& properties, const std::string& propertyName) {
		BaseProperty0* result = NULL;
		for (size_t i = 0; i < properties.size(); i++) {
			if (properties[i]->getName() == propertyName)
				result = properties[i];
		}
		return result;
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
		MyReflectable0() : _myIntPointer(NULL) { }
		int getMyInt() const { return _myInt; }
		int& getMyInt() { return _myInt; }
		int* getMyPointer() { return _myIntPointer; }
		float getMyFloat() const { return _myFloat; }
		MyInnerReflectable0& getMyInnerRefletable() { return _myInnerReflectable; }
		void setMyInt(int myInt) { _myInt = myInt; }
		void setMyIntPointer(int* myIntPointer) { _myIntPointer = myIntPointer; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
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
			initInspector();
			write(reflectable.getProperties(), "root", -1);
			writePointers();
		}
	};

	std::ostream* TextWriter0::Stream;

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

	template <class T> class Reference500 {
		T& _reference;
	protected:
		template<typename U>
		void setAddressFromVoidPointer(U& t, void* address) {
			SB_ERROR("cannot set address because the type is not a pointer");
		}
		template<typename U>
		void setAddressFromVoidPointer(U*& t, void* address) {
			t = (U*)address;
		};
	public:
		Reference500(T& t) : _reference(t) { }
		void setAddress(void* address) {
			setAddressFromVoidPointer(_reference, address);
		}
		T& getValue() { return _reference; }
	};

	void demo500() {
		int myInt = 42;
		Reference500<int> reference1(myInt);
		int* myIntPointer;
		Reference500<int*> reference2(myIntPointer);
		int myInt2 = 43;
		void* myVoidPointerToInt2 = &myInt2;
		reference2.setAddress(myVoidPointerToInt2);
		cout << *reference2.getValue() << endl;

		cout << "Press enter to provoke an error";
		cin.get();
		reference1.setAddress(myVoidPointerToInt2);
	}

	class ReaderStream {
		istream* _stream;
		string _buffer;
	public:
		void setSource(istream& is) { _stream = &is; }
		bool getLine(string& line) {
			if (_buffer.size() > 0) {
				line = _buffer;
				_buffer = "";
				return true;
			}
			else
				return bool(getline(*_stream, line));
		}
		void putBack(const string& line) {
			SB_ERROR_IF(_buffer.size() > 0, "There was already a line put back into the buffer");
			_buffer = line;
		}
	};
	
	bool extractLine(std::string& name, std::string& value, size_t& index, int depth, ReaderStream& stream) {
		std::string line;
		if (stream.getLine(line)) {
			size_t currentDepth = countStart(line, ' ');
			if (depth == currentDepth) {
				std::vector<std::string> result;
				split(line, " ", result);
				if (result.size() > 1) {
					name = result[0];
					value = result.size() == 3 ? result[1] : "";
					index = parse<size_t>(result[result.size() - 1]);
					return true;
				}
			}
		}

		stream.putBack(line);
		return false;
	}

	bool extractLine(size_t& pointerSource, size_t& pointerTarget, ReaderStream& stream) {
		string line;
		if (stream.getLine(line)) {
			std::vector<std::string> result;
			split(line, " ", result);
			pointerSource = parse<size_t>(result[0]);
			pointerTarget = parse<size_t>(result[1]);
			return true;
		}

		return false;
	}

	bool skipLine(ReaderStream& stream) {
		string dummy;
		return stream.getLine(dummy);
	}

	class Reader1000 {	
		static map<size_t, BaseProperty0*> Properties;
	public:
		static void storeProperty(size_t index, BaseProperty0& theProperty) { Properties[index] = &theProperty; }
		static BaseProperty0* getProperty(size_t index) { return Properties[index]; }
		static void initReader() {
			Properties.clear();
		}
	};

	map<size_t, BaseProperty0*> Reader1000::Properties;

	class TextReader1000 : public Reader1000 {
		static ReaderStream Stream;
		static std::string CurrentValue;
	protected:
		static void read(const vector<BaseProperty0*>& properties, size_t depth) {
			std::string name; size_t index;
			while (extractLine(name, CurrentValue, index, depth, Stream)) {
				BaseProperty0* property = findProperty(properties, name);
				storeProperty(index, *property);
				if (property->isReflectable())
					read(property->getChildProperties(), depth + 1);
				else
					property->inspect(depth);
			}
		}
		static void readPointers() {
			size_t sourceIndex; size_t targetIndex;
			skipLine(Stream);
			while (extractLine(sourceIndex, targetIndex, Stream)) {
				BaseProperty0* source = getProperty(sourceIndex);
				BaseProperty0* target = getProperty(targetIndex);
				source->setAddress(target->getVoidPointerToValue());
			}
		}
	public:
		template <class T> static void read(T& t, const std::string& name, size_t depth) {
			t = parse<T>(CurrentValue);
		}
		template <class T> static void read(T* t, const std::string& name, size_t depth) { }
		static void read(BaseReflectable0& reflectable, std::istream& is) {
			reflection::setInspector(SB_NAMEOF(TextReader1000));
			Stream.setSource(is);
			initReader();
			read(reflectable.getProperties(), 0);
			readPointers();
		}
	};

	ReaderStream TextReader1000::Stream;
	std::string TextReader1000::CurrentValue;

	namespace reflection {
		static std::string CurrentInspectorName;
		static void setInspector(const std::string& inspectorName) { CurrentInspectorName = inspectorName; }
		template <class T> static void inspect(T& t, const std::string& name, size_t depth) {
			if (CurrentInspectorName == SB_NAMEOF(TextWriter0))
				TextWriter0::write(t, name, depth);
			else if (CurrentInspectorName == SB_NAMEOF(TextReader1000))
				TextReader1000::read(t, name, depth);
			else
				SB_ERROR("Inspector " << CurrentInspectorName << " not found");
		}
	}

	void demo1000() {
		ostringstream os;
		os << "_myInt 42 0" << endl;
		os << "_myIntPointer pointer 1" << endl;
		os << "_myFloat 1.234 2" << endl;
		os << "_myInnerReflectable 3" << endl;
		os << " _myDouble 9.876 4" << endl;
		os << "pointers" << endl;
		os << " 1 0" << endl;

		MyReflectable0 myReflectable;
		istringstream is(os.str());
		TextReader1000::read(myReflectable, is);
		cout << myReflectable.getMyInt() << " " << &myReflectable.getMyInt() << endl;
		cout << *myReflectable.getMyPointer() << " " << myReflectable.getMyPointer() << endl;
		cout << myReflectable.getMyFloat() << endl;
		cout << myReflectable.getMyInnerRefletable().getMyDouble() << endl;
	}
	
	void run() {
		// demo8: link pointers (write, raed, edit)
		demo1000();
		//demo500();
		// demo0();
		//demo1000();
	}
}
