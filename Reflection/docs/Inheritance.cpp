template <class T>
class Reflectable {
	std::vector<BaseProperty*> _properties;
public:
	std::vector<BaseProperty*> getProperties() {  return _properties; }	
	void addProperty(BaseProperty* property) { _properties.push_back(property); }
}

class Animal : public Reflectable<Animal> {
	// SB_CLASS(Animal)
	...
	// SB_PROPERTY(std::string, _noise)
	std::string _noise;
	static void _noiseDoRegister() {
		addProperty(Property<std::string>(_noise));
	}
	Invocation<_noiseDoRegister> _noiseRegistration;
	...
public:
	const std::string& getNoise() const { return _noise; }
	void setNose(const std::string& noise) { _noise = noise; } 
}

class Dog : public Animal {
	// SB_CLASS(Animal)
	// SB_PROPERTY(std::string, _breed)
	std::string _breed;
public:
	Dog() {
		setNoise("Wauwau");
	}  
}