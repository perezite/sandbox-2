class MyClass {
	int _myInt;
	
public:
	void setMyInt(int myInt) { myInt = myInt;}
}

string indent(size_t depth) { ... }

class TypeInfo {
	string _typeName;
	vector<TypeInfo> _memberInfos;
	
public:
	const string& getName() { _typeName; }
	const vector<TypeInfo>() { return _memberInfos; }
}

class ObjectInfo {
protected:
	string _typeName;
	TypeInfo* _typeInfo;
	string _name;
	
public:
	ObjectInfo(const string& name) : _name(name) {}	
	virtual ~ObjectInfo() { delete _typeInfo; }
	const string& getTypeName() { return _typeInfo->getName(); }
	const string& getName() { return _name; }
	const ObjectInfo& getMemberInfo(size_t index) { return _typeInfo->getMemberInfos()[index]; }
	virtual string getValue() = 0;
}

void writeText(ObjectInfo& objectInfo) {
	cout << objectInfo.getTypeName() << " " << objectInfo.getName() << endl;
	ObjectInfo memberInfo = objectInfo.getMemberInfo(0); 
	cout << indent(1) << memberInfo.getTypeName() << " " 
	  << memberInfo.getName() << " = " << memberInfo.getValue() << endl; 
}

template <class T> class ConcreteObjectInfo : public ObjectInfo {
	TypeInfo* _typeInfo;
public:
	ConcreteObjectInfo(T& object, const string& name) : ObjectInfo(name) {
		_typeInfo = new ConcreteTypeInfo<T>();
	}	
}

void main() {
	MyClass myClass;
	myClass.setMyInt(42);

	
	ConcreteObjectInfo objectInfo(myClass, "myClass");
	writeText(objectInfo);
}

