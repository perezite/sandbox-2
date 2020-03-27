class BaseProperty {
	std::string _name;
public:
	const& std::string getName() { return _name; }
	virtual std::string getValue();		
}

template <class T>
class Property: public BaseProperty {
	T& _property;
public:
	Property(T& field, const& std::string name)
		: BaseProperty(name), _property(field)
	{ }
	std::string getValue() {
		// http://www.cplusplus.com/doc/tutorial/basic_io/
		std::ostringstream oss;
		oss << field;
		return oss.str();
	}
}

class Reflectable {
	std::vector<BaseProperty*> _properties;
public:	
	virtual ~Reflectable() {
		for (size_t i = 0; i < _properties.size(); i++)
			delete _properties[i];
	}
	template <class T>
	void addProperty(T& property, const std::string& name) {
		_properties.push_back(new Property<T>(property, name));
	}
	std::vector<BaseProperty*> getProperties() { return _properties; }
}

template <void (*Func)()>
class Caller {
	Caller() {
		Func();
	}
}

class Diego : public Reflectable {
	float x;	
	inline register_x() {
		addProperty(x, "x", this);
	}
	Caller<register_x> caller_x;
	
};
// this becomes:
class Diego : public Reflectable {
	SB_PROPERTY(float, x)
};

void {
	Diego diego;
	auto properties = diego.getProperties();
	
	for (size_t i = 0; i < properties.size(); i++) 
		std::cout << properties[i].getName() << properties[i].getValue() << std::endl;
}
