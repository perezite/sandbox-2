#include "Demo9.h"
#include "Logger.h"
#include "Macro.h"
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

namespace reflectionDemo10 {

	/*
	void demo() {
		// write
		MyClass myClass;
		myClass.getFloats().push_back(1.2345f);
		myClass.getFloats().push_back(2.3456f);
		ostringstream os;
		TextWriter::write(myDerived, os);

		// read
		MyClass myClass;
		istringstream is(os);
		TextReader::read(myClass, is);
		cout << myDerived2.getFloats()[0] << endl;
		cout << myDerived2.getFloats()[0] << endl;

		// edit
		string value; string index;
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

	namespace reflection {
		//static void setInspector(const std::string& inspectorName);
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

	template <void(*Action)()>
	class Invocation0 {
	public:
		Invocation0() {
			Action();
		}
	};

	class MyClass0 : public Reflectable0<MyClass0> {
		vector<float> _myFloats;
		static void addProperty_myFloats(void* instance) {
			MyClass0* typedInstance = (MyClass0*)instance;
			typedInstance->addProperty(typedInstance->_myFloats, SB_NAMEOF(_myFloats));
		}
		static void register_myFloats() {
			addRegistration(&addProperty_myFloats, SB_NAMEOF(_myFloats));
		}
		Invocation0<register_myFloats> invoke_register_myFloats;
	public:
		vector<float>& getMyFloats() { return _myFloats; }
	};

	namespace reflection {
		static std::string CurrentInspectorName;
		//static void setInspector(const std::string& inspectorName) { CurrentInspectorName = inspectorName; }
		template <class T> static void inspect(T& t, const std::string& name, size_t depth) {
			//if (CurrentInspectorName == SB_NAMEOF(TextWriter0))
			//	TextWriter0::write(t, name, depth);
			//else
				//SB_ERROR("Inspector " << CurrentInspectorName << " not found");
		}
	}

	void demo0() {

	}

	void run() {
		// demo10: vectors (write, read, edit)
		// NEXT: Implement combined demo in demo9 
		demo0();
	}
}

