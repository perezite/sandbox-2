class MyClass {	
public:
	int myInt;
}

template <class T> class Type;

template <class TParent> class Member : public Type {
public:
  virtual Object getMemberObject(TParent& parent) = 0;
}

template <class TParent, class TMember> class ConcreteMember : public Member {
	TMember TParent::*_memberPointer;
	
public:
	Member(TMember TParent::*memberPointer, const string& name) : Type(name), memberPointer(_memberPointer) { }
	Object getMemberObject(TParent& parent) {
		TMember& memberRef = parent.*memberPointer; 
		Object memberObject(memberRef);
		return memberObject;
	}
}

template <class T> class Type {
	static string _name;
	static vector<*Member<T>> _members;
public:
	Type(const string& name) : _name(name) { }
	virtual ~Type() { deleteAll(_memebers); }
	const string& getName() { return _name; }
	Object getMemberObject(T& parent, size_t index) { return _members[index]->getObject(parent); } 
	static void setName(const string& name) { _name = name; }
	template <class TMember> void addMember(TMemeber T::*memberPointer, const string& name) { _members.push_back(new ConcreteMember(memberPointer, name)); }
}

template <class T> string toString(T& object) {
	ostringstream os; os << object;
	return os.str();
}

template <class T> class Object {
	T& _object;
	string _name;
public:
	Object(T& object, const string& name) : _object(object), _name(name) { }
	const string& getTypeName() const { return Type<T>::getName();}
	const string& getName() const { return _name; }
	Object getMember(size_t index) { Type<T>::getMemberObject(*this, index); }
	string getValue() { toString<T>(_object); }		// TODO: Use Sfinae here
}

template <class T> writeText(Object& object) {
	cout << object.getTypeName() << " " << object.getName() << endl; 
	Object member = object.getMember(0);
	cout << "  " << object.getTypeName() << " " << object.getName() << " " << object.getValue();
}

void main() {
	MyClass myClass;
	myClass.setMyInt(42);
	
	Type<MyClass>::setName("myClass");
	Type<MyClass>::addMember<MyClass, int>(&MyClass::myInt, "myInt");

	ConcreteObject object<MyClass>(myClass, "myClass");
	writeText(object);
	
	// Expected output:
	// MyClass myClass
	//   int myInt = 42
}
