#include "Demo5.h"
#include "Macro.h"
#include "Logger.h"
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <stdlib.h>

namespace reflectionDemo6 {
	namespace reflection {
		static void setInspector(const std::string& inspectorName);
		template <class T> static void inspect(T& t, const std::string& name, size_t depth);
	}

	class BaseProperty100 {
		std::string _name;
	public:
		BaseProperty100(const std::string name) : _name(name)
		{ }
		const std::string& getName() { return _name; }
		virtual void inspect(size_t depth) = 0;
	};

	template <class T>
	class Property100 : public BaseProperty100 {
		T& _reference;
	public:
		Property100(T& reference, const std::string& name)
			: BaseProperty100(name), _reference(reference)
		{ }
		virtual void inspect(size_t depth) {
			reflection::inspect(_reference, getName(), depth);
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
		static std::vector<std::string> PropertyRegistrationNames;
		static bool RegistrationsInitialized;
		std::vector<BaseProperty100*> _properties;
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
	public:
		Reflectable100() : _propertiesInitialized(false)
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
		virtual std::vector<BaseProperty100*>& getProperties() {
			if (!_propertiesInitialized) {
				initProperties();
				_propertiesInitialized = true;
			}
			return _properties;
		}
		template <class U>
		void addProperty(U& value, const std::string& name) { _properties.push_back(new Property100<U>(value, name)); }
	};

	template <class T>
	std::vector<void(T::*)()> Reflectable100<T>::PropertyRegistrations;

	template <class T>
	std::vector<std::string> Reflectable100<T>::PropertyRegistrationNames;

	template <class T> bool Reflectable100<T>::RegistrationsInitialized = false;

	template <void(*Action)()>
	class Invocation100 {
	public:
		Invocation100() {
			Action();
		}
	};

	// https://stackoverflow.com/questions/37031844/logic-of-stdis-base-of-in-c-11
	template<typename D, typename B>
	class IsDerivedFrom100
	{
		class No { };
		class Yes { No no[2]; };

		static Yes Test(B*) {};
		static No Test(...) {};
	public:
		static bool value() {
			return sizeof(Test(static_cast<D*>(0))) == sizeof(Yes);
		}
	};

	class MyInnerReflectable100 : public Reflectable100<MyInnerReflectable100> {
		double _myDouble;
		void addProperty_myDouble() {
			addProperty(_myDouble, SB_NAMEOF(_myDouble));
		}
		static void register_myDouble() {
			addRegistration(&MyInnerReflectable100::addProperty_myDouble, SB_NAMEOF(_myDouble));
		}
		Invocation100<register_myDouble> invoke_register_myInt;
	public:
		void setMyDouble(double myDouble) { _myDouble = myDouble; }
		double getMyDouble() const { return _myDouble; }
	};


	class MyReflectable100 : public Reflectable100<MyReflectable100> {
		int _myInt;
		void addProperty_myInt() {
			addProperty(_myInt, SB_NAMEOF(_myInt));
		}
		static void register_myInt() {
			addRegistration(&MyReflectable100::addProperty_myInt, SB_NAMEOF(_myInt));
		}
		Invocation100<register_myInt> invoke_register_myInt;
		float _myFloat;
		void addPropert_myFloat() {
			addProperty(_myFloat, SB_NAMEOF(_myFloat));
		}
		static void register_myFloat() {
			addRegistration(&MyReflectable100::addPropert_myFloat, SB_NAMEOF(_myFloat));
		}
		Invocation100<register_myFloat> invoke_register_myFloat;
		MyInnerReflectable100 _myInnerReflectable;
		void addPropert_myInnerReflectable() {
			addProperty(_myInnerReflectable, SB_NAMEOF(_myInnerReflectable));
		}
		static void register_myInnerReflectable() {
			addRegistration(&MyReflectable100::addPropert_myInnerReflectable, SB_NAMEOF(_myInnerReflectable));
		}
		Invocation100<register_myInnerReflectable> invoke_register_myInnerReflectable;
	public:
		int getMyInt() const { return _myInt; }
		float getMyFloat() const { return _myFloat; }
		const MyInnerReflectable100& getMyInnerRefletable() const { return _myInnerReflectable; }
		void setMyInt(int myInt) { _myInt = myInt; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
		MyInnerReflectable100& getMyInnerReflectable() { return _myInnerReflectable; }
	};

	template <class T>
	std::string stringify(T& t) {
		static std::ostringstream os; os.str(""); os << t;
		return os.str();
	}

	class TextWriter100 {
		static std::ostream* Stream;
	protected:
		static std::ostream& getStream() { return *Stream; }
		static void print(const std::string& name, const std::string& value, size_t depth) {
			getStream() << std::string(depth, ' ') + name + ' ' + value + '\n';
		}
		static void writeProperties(const std::vector<BaseProperty100*>& properties, size_t depth) {
			for (size_t i = 0; i < properties.size(); i++)
				properties[i]->inspect(depth);
		}
	public:
		static void writeProperty(BaseReflectable100& reflectable, const std::string& name, size_t depth) {
			getStream() << std::string(depth, ' ') + name + '\n';
			writeProperties(reflectable.getProperties(), depth + 1);
		}
		template <class T> static void writeProperty(const T& t, const std::string& name, size_t depth) {
			if (IsDerivedFrom100<T, BaseReflectable100>::value() == false)
				SB_ERROR("the type of " << name << " is not supported by TextWriter");
			writeProperty((BaseReflectable100&)t, name, depth);
		}
		static void write(BaseReflectable100& reflectable, std::ostream& os) {
			Stream = &os;
			reflection::setInspector(SB_NAMEOF(TextWriter100));
			writeProperties(reflectable.getProperties(), 0);
		}
	};

	template <> void TextWriter100::writeProperty<int>(const int& t, const std::string& name, size_t depth) {
		print(name, stringify(t), depth);
	}

	template <> void TextWriter100::writeProperty<float>(const float& t, const std::string& name, size_t depth) {
		print(name, stringify(t), depth);
	}

	template <> void TextWriter100::writeProperty<double>(const double& t, const std::string& name, size_t depth) {
		print(name, stringify(t), depth);
	}

	std::ostream* TextWriter100::Stream;

	void demo100() {
		// stream write
		std::cout << "write:" << std::endl;
		MyReflectable100 myReflectable;
		myReflectable.setMyInt(42);
		myReflectable.setMyFloat(1.2345f);
		myReflectable.getMyInnerReflectable().setMyDouble(6.7891f);
		std::ostringstream os;
		TextWriter100::write(myReflectable, os);
		std::cout << os.str();
		os.str("");
		TextWriter100::write(myReflectable, os);
		std::cout << os.str();
		std::ofstream ofs("C:\\Temp\\Test.txt");
		TextWriter100::write(myReflectable, ofs);
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

	template <class T>
	T convert(const std::string& input) {
		std::istringstream is(input);
		T result; is >> result;
		return result;
	}

	void stripLeft(std::string& input, char token) {
		auto count = countStart(input, token);
		input = input.substr(count, std::string::npos);
	}

	BaseProperty100* findProperty(BaseReflectable100& reflectable, const std::string& propertyName) {
		BaseProperty100* result = NULL;
		auto properties = reflectable.getProperties();
		for (size_t i = 0; i < properties.size(); i++) {
			if (properties[i]->getName() == propertyName)
				result = properties[i];
		}
		return result;
	}

	class TextReader200 {
		static std::istream* Stream;
		static std::string CurrentValue;
	protected:
		static std::istream& getStream() { return *Stream; }
		static bool extractLine(std::string& name, std::string& value, int depth) {
			std::string line;
			if (std::getline(getStream(), line)) {
				size_t currentDepth = countStart(line, ' ');
				if (depth == currentDepth) {
					std::vector<std::string> result;
					split(line, " ", result);
					name = result[0];
					value = result.size() == 2 ? result[1] : "";					
					return true;
				}
			}
			return false;
		}
		static void readProperties(BaseReflectable100& reflectable, size_t depth) {
			std::string name;
			while (extractLine(name, CurrentValue, depth)) {
				BaseProperty100* property = findProperty(reflectable, name);
				property->inspect(depth);
			}
		}
	public:
		static void readProperty(BaseReflectable100& reflectable, const std::string& name, size_t depth) {
			readProperties(reflectable, depth + 1);
		}
		template <class T>
		static void readProperty(T& t, const std::string& name, size_t depth) {
			if (IsDerivedFrom100<T, BaseReflectable100>::value() == false)
				SB_ERROR("the type of " << name << " is not supported by TextWriter");
			readProperty((BaseReflectable100&)t, name, depth);
		}
		static void read(std::istream& is, BaseReflectable100& reflectable) {
			reflection::setInspector(SB_NAMEOF(TextReader200));
			Stream = &is;
			readProperties(reflectable, 0);
		}
	};

	template<> void TextReader200::readProperty<int>(int& t, const std::string& name, size_t depth) {
		t = convert<int>(CurrentValue);
	}

	template<> void TextReader200::readProperty<float>(float& t, const std::string& name, size_t depth) {
		t = convert<float>(CurrentValue);
	}

	template<> void TextReader200::readProperty<double>(double& t, const std::string& name, size_t depth) {
		t = convert<double>(CurrentValue);
	}

	std::istream* TextReader200::Stream;
	std::string TextReader200::CurrentValue;

	void demo200() {
		// stream read
		std::ostringstream os;
		os << "_myInt 42" << std::endl;
		os << "_myFloat 3.1415" << std::endl;
		os << "_myInnerReflectable" << std::endl;
		os << " _myDouble 1.2345" << std::endl;

		std::istringstream is(os.str());
		MyReflectable100 myReflectable;
		TextReader200::read(is, myReflectable);

		std::cout << myReflectable.getMyInt() << std::endl;
		std::cout << myReflectable.getMyFloat() << std::endl;
		std::cout << myReflectable.getMyInnerReflectable().getMyDouble() << std::endl;
	}

	class ConsoleEditor300 {
		enum class Mode { Display, Edit };
		static Mode CurrentMode;
		static size_t Counter;
		static BaseProperty100* CurrentProperty;
		static std::map<size_t, BaseProperty100*> Properties;

	protected:
		static void displayProperties(BaseReflectable100& reflectable, size_t depth) {
			auto properties = reflectable.getProperties();
			for (size_t i = 0; i < properties.size(); i++) {
				std::string dummy = "";
				CurrentProperty = properties[i];
				properties[i]->inspect(depth);
			}
		}
		template <class T> static void displayPrimitive(T& t, const std::string& name, size_t depth) {
			std::cout << std::string(depth, ' ') << name << " " << t << " (" << Counter << ")" << std::endl;
			Properties[Counter++] = CurrentProperty;
		}
		template <class T> static void editPrimitive(T& t, const std::string& name, size_t depth) {
			std::cout << "Please enter value: ";
			std::cin >> t;
			std::cout << std::endl;
		}
		template <class T> static void inspectPrimitive(T& t, const std::string& name, size_t depth) {
			if (CurrentMode == Mode::Display)
				displayPrimitive(t, name, depth);
			else
				editPrimitive(t, name, depth);
		}
		static void displayReflectable(BaseReflectable100& t, const std::string& name, size_t depth) {
			std::cout << std::string(depth, ' ') << name << std::endl;
			displayProperties((BaseReflectable100&)t, depth + 1);
		}
		static void editProperty(size_t index) {
			std::string dummy;
			Properties[index]->inspect(-1);
		}
	public:
		template <class T> static void inspectProperty(T& t, const std::string& name, size_t depth) {
			if (IsDerivedFrom100<T, BaseReflectable100>::value() == false)
				SB_ERROR("Displaying the type of " << name << " is not supported by TextWriter");
			if (CurrentMode == Mode::Edit)
				SB_ERROR("Editing the type of " << name << " is not supported by TextWriter");
			displayReflectable(t, name, depth);
		}
		static bool prompt() {
			std::string input;
			size_t index;

			std::cout << "Which property do you want to edit?" << std::endl;
			std::cin >> input;
			if (input == "exit")
				return false;

			std::istringstream is(input); is >> index;
			if (!is.fail()) {
				CurrentMode = Mode::Edit;
				editProperty(index);
			}
			return true;
		}
		static void edit(BaseReflectable100& reflectable) {
			reflection::setInspector(SB_NAMEOF(ConsoleEditor300));
			std::string input;

			do {
				Counter = 0;
				Properties.clear();
				CurrentMode = Mode::Display;
				displayProperties(reflectable, 0);
			} while (prompt());
		}
	};

	ConsoleEditor300::Mode ConsoleEditor300::CurrentMode = ConsoleEditor300::Mode::Display;
	size_t ConsoleEditor300::Counter = 0;
	BaseProperty100* ConsoleEditor300::CurrentProperty;
	std::map<size_t, BaseProperty100*> ConsoleEditor300::Properties;

	template <> void ConsoleEditor300::inspectProperty<int>(int& t, const std::string& name, size_t depth) {
		inspectPrimitive(t, name, depth);
	}

	template <> void ConsoleEditor300::inspectProperty<float>(float& t, const std::string& name, size_t depth) {
		inspectPrimitive(t, name, depth);
	}

	template <> void ConsoleEditor300::inspectProperty<double>(double& t, const std::string& name, size_t depth) {
		inspectPrimitive(t, name, depth);
	}

	namespace reflection {
		static std::string CurrentInspectorName;
		static void setInspector(const std::string& inspectorName) { CurrentInspectorName = inspectorName; }
		template <class T> static void inspect(T& t, const std::string& name, size_t depth) {
			if (CurrentInspectorName == SB_NAMEOF(TextWriter100))
				TextWriter100::writeProperty(t, name, depth);
			else if (CurrentInspectorName == SB_NAMEOF(TextReader200))
				TextReader200::readProperty(t, name, depth);
			else if (CurrentInspectorName == SB_NAMEOF(ConsoleEditor300))
				ConsoleEditor300::inspectProperty(t, name, depth);
			else
				SB_ERROR("Inspector " << CurrentInspectorName << " not found");
		}
	}

	void demo300() {
		std::cout << "edit: " << std::endl;
		MyReflectable100 myReflectable;
		myReflectable.setMyInt(42);
		myReflectable.setMyFloat(1.2345f);
		myReflectable.getMyInnerReflectable().setMyDouble(6.7891f);
		ConsoleEditor300::edit(myReflectable);
	}

	void demo1000() {
		// stream (write, read)
		std::cout << "write:" << std::endl;
		MyReflectable100 myReflectable;
		myReflectable.setMyInt(42);
		myReflectable.setMyFloat(1.2345f);
		myReflectable.getMyInnerReflectable().setMyDouble(6.7891f);
		std::ostringstream os;
		TextWriter100::write(myReflectable, os);
		std::cout << os.str();

		std::cout << "read:" << std::endl;
		MyReflectable100 myReflectable2;
		std::istringstream is(os.str());
		TextReader200::read(is, myReflectable2);
		std::cout << myReflectable2.getMyInt() << " ";
		std::cout << myReflectable2.getMyFloat() << " ";
		std::cout << myReflectable2.getMyInnerReflectable().getMyDouble() << std::endl;
	}

	void demo1500() {
		// stream (write, read, edit)
		std::cout << "write:" << std::endl;
		MyReflectable100 myReflectable;
		myReflectable.setMyInt(42);
		myReflectable.setMyFloat(1.2345f);
		myReflectable.getMyInnerReflectable().setMyDouble(6.7891f);
		std::ofstream ofs("C:\\Temp\\Demo1500.txt");
		TextWriter100::write(myReflectable, ofs);
		ofs.close();
		std::ostringstream os; os << "notepad " << "C:\\Temp\\Demo1500.txt";
		system(os.str().c_str());

		std::cout << "read:" << std::endl;
		MyReflectable100 myReflectable2;
		std::ifstream ifs("C:\\Temp\\Demo1500.txt");
		TextReader200::read(ifs, myReflectable2);
		std::cout << myReflectable2.getMyInt() << " ";
		std::cout << myReflectable2.getMyFloat() << " ";
		std::cout << myReflectable2.getMyInnerReflectable().getMyDouble() << std::endl;

		std::cout << "edit: " << std::endl;
		ConsoleEditor300::edit(myReflectable2);
		std::cout << myReflectable2.getMyInt() << " ";
		std::cout << myReflectable2.getMyFloat() << " ";
		std::cout << myReflectable2.getMyInnerReflectable().getMyDouble() << std::endl;
	}

	void run() {
		demo1500();
		//demo1000();
		//demo300();
		//demo200();
		//demo100();
	}

}
