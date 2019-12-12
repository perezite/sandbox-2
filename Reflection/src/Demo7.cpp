#include "Demo5.h"
#include "Macro.h"
#include "Logger.h"
#include <vector>

using namespace std;

namespace reflectionDemo7 {
	// https://stackoverflow.com/questions/37031844/logic-of-stdis-base-of-in-c-11
	template<typename D, typename B>
	class IsDerivedFrom10
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

	class BaseReflectable10 {

	};

	class DerivedReflectable10 : public BaseReflectable10 {

	};

	template <class TDerived>
	class MyBase10 {
	public:
		template <class T> static void baseInspect() {
			if (IsDerivedFrom10<T, BaseReflectable10>::value())
				TDerived::inspectReflectable();
			else
				TDerived::template inspect<T>();
		}
	};

	class MyDerived10 : public MyBase10<MyDerived10> {
	public:
		static void inspectReflectable() {
			cout << "BaseReflectable100" << endl;
		}

		template <class T> static void inspect() {
			cout << "type not supported" << endl;
		}
	};

	template <> void MyDerived10::inspect<BaseReflectable10>() { cout << "must not be called!!" << endl; }

	template <> void MyDerived10::inspect<int>() { cout << "int" << endl; }

	template <> void MyDerived10::inspect<float>() { cout << "float" << endl; }

	class RandomType10 { };

	void demo10() {
		MyDerived10::baseInspect<int>();
		MyDerived10::baseInspect<float>();
		MyDerived10::baseInspect<RandomType10>();
		MyDerived10::baseInspect<DerivedReflectable10>();
	}

	class MySprite100 {
	};

	template <class T>
	class MyNode100 {
		T* _reference;
	};

	template <class T>
	class MyNestedClass100 {

	};

	template <class T, template <class Inner> class V>
	void myMethod100(V<T> &con)
	{
	}

	void demo100() {
		MyNestedClass100<int> myNestedClass;
		myMethod100(myNestedClass);
		MyNode100<MySprite100> mySpriteNode;
		myMethod100(mySpriteNode);
	}

	void run() {
		demo100();
		//demo10();
	}

}
