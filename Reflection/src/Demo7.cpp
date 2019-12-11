#include "Demo5.h"
#include "Macro.h"
#include "Logger.h"

using namespace std;

namespace reflectionDemo7 {

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

	class BaseReflectable100 {

	};
	
	class DerivedReflectable100 : public BaseReflectable100 {

	};

	template <class TDerived>
	class MyBase {
	public:
		template <class T> static void baseInspect() {
			if (IsDerivedFrom100<T, BaseReflectable100>::value())
				TDerived::inspectReflectable();
			else
				TDerived::template inspect<T>();
		}
	};

	class MyDerived : public MyBase<MyDerived> {
	public:
		static void inspectReflectable() {
			cout << "BaseReflectable100" << endl;
		}

		template <class T> static void inspect() { 
			cout << "type not supported" << endl;
		}
	};

	template <> void MyDerived::inspect<int>() {
		cout << "int" << endl;
	}

	void demo10() {
		MyDerived::baseInspect<int>();
		MyDerived::baseInspect<float>();
		MyDerived::baseInspect<DerivedReflectable100>();

	}

	void run() {
		demo10();
	}

}
