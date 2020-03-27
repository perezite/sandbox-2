#pragma once

namespace sb {
	// https://stackoverflow.com/questions/37031844/logic-of-stdis-base-of-in-c-11
	template<typename D, typename B>
	class InheritanceCheck
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
}
