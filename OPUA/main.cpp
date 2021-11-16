#include <iostream>
#include "OPUA.h"

//#define TEST_OPUA_MEMORY_ALLOCATION
//#define TEST_OPUA_EXPRESSION
//#define TEST_OPUA_CONSTRAINT
//#define TEST_OPUA_OBJECTIVE
#define TEST_OPUA_MODEL

int main()
{
	using namespace OPUA;
#ifdef TEST_OPUA_MEMORY_ALLOCATION
	while (true)
	{
		OpEnv env(true, "OPUA_ENV_TEST");
		OpUInt count(1);
		while (count--)
		{
			Variable::OpVarIdxDict x(env);
			for (OpULInt i = 0; i < 10000; i++)
			{
				Variable::OpVar var(env, Variable::OpVarType::Con, 0, 100);
				x.add(var.getIndex(), var);
			}
			for (auto iter=x.getBegin(); iter!=x.getEnd(); ++iter)
			{
				auto& var = iter.getVal();
				var.setUb(99);
				std::cout << var << std::endl;
			}
			x.releaseElements();
			x.release();
		}
		count = 1;
		while (count--)
		{
			Variable::OpVarArr y(env);
			for (OpULInt i = 0; i < 10000; i++)
			{
				Variable::OpVar var(env, Variable::OpVarType::Bool, 0, 1);
				y.add(var);
			}
			for (auto iter = y.getBegin(); iter != y.getEnd(); ++iter)
			{
				auto& var = iter.getVal();
				var.setLb(1);
				std::cout << var << std::endl;
			}
			y.releaseElements();
			y.release();
		}
		env.release();
		system("pause");
	}
#endif

#ifdef TEST_OPUA_EXPRESSION
	{
		OpEnv env(true, "OPUA_ENV_TEST");
		Variable::OpVar var1(env, Variable::OpVarType::Con, 0.0, 100.0, "x1");
		Variable::OpVar var2(env, Variable::OpVarType::Con, 0.0, 100.0, "x2");
		Variable::OpVar var3(env, Variable::OpVarType::Bool, 0, 1, "x3");
		Expression::OpLinExpr linexpr1(0.5);
		linexpr1.addLinTerm(var1, 1.0);
		linexpr1.addLinTerm(var2, 2.0);
		linexpr1.addLinTerm(var3, 3.0);
		std::cout << "linexpr1: " << linexpr1 << std::endl;

		auto linexpr2(linexpr1);
		Expression::OpQuadExpr quadexpr1(0.0);
		quadexpr1 += linexpr1 * linexpr2;
		std::cout << "quadexpr1: " << quadexpr1 << std::endl;
		env.release();
		system("pause");
	}
#endif

#ifdef TEST_OPUA_CONSTRAINT
	{
		using namespace Expression;
		using namespace Constraint;
		OpEnv env(true, "OPUA_ENV_TEST");
		Variable::OpVar var1(env, Variable::OpVarType::Con, 0.0, 100.0, "x1");
		Variable::OpVar var2(env, Variable::OpVarType::Con, 0.0, 100.0, "x2");
		Variable::OpVar var3(env, Variable::OpVarType::Bool, 0, 1, "x3");
		auto linexpr1 = var1 + 2 * var2 + 3 * var3 + 0.5;
		auto linexpr2 = linexpr1 / 2;
		auto quadexpr1 = linexpr1 * linexpr1;
		auto lincon1 = linexpr1 >= 5;
		auto lincon2 = 4 == linexpr2;
		auto quadcon1 = quadexpr1 <= linexpr1;

		std::cout << "original: " << std::endl;
		std::cout << "lincon1: " << lincon1 << std::endl;
		std::cout << "lincon2: " << lincon2 << std::endl;
		std::cout << "quadcon1: " << quadcon1 << std::endl;

		std::cout << "standard: " << std::endl;
		lincon1.standardize();
		lincon2.standardize();
		quadcon1.standardize();
		std::cout << "lincon1: " << lincon1 << std::endl;
		std::cout << "lincon2: " << lincon2 << std::endl;
		std::cout << "quadcon1: " << quadcon1 << std::endl;
		env.release();
		system("pause");
	}
#endif


#ifdef TEST_OPUA_OBJECTIVE
	{
		using namespace Expression;
		using namespace Constraint;
		using namespace Objective;
		OpEnv env(true, "OPUA_ENV_TEST");
		Variable::OpVar var1(env, Variable::OpVarType::Con, 0.0, 100.0, "x1");
		Variable::OpVar var2(env, Variable::OpVarType::Con, 0.0, 100.0, "x2");
		Variable::OpVar var3(env, Variable::OpVarType::Bool, 0, 1, "x3");
		auto linexpr1 = var1 + 2 * var2 + 3 * var3 + 0.5;
		auto linexpr2 = linexpr1 / 2;
		auto quadexpr1 = linexpr1 * linexpr1;
		auto quadobj1 = OpMaximize(env, quadexpr1);
		auto linobj1 = OpMinimize(env, linexpr2);
		std::cout << "obj1: " << std::endl << quadobj1 << std::endl;
		std::cout << "obj2: " << std::endl << linobj1 << std::endl;
		env.release();
		system("pause");
	}
#endif

#ifdef TEST_OPUA_MODEL
	{
		std::unordered_map<int, int> a;
		using namespace Expression;
		using namespace Constraint;
		using namespace Objective;
		using namespace Model;
		OpEnv env(true, "OPUA_ENV_TEST");
		Variable::OpVar var1(env, Variable::OpVarType::Con, 0.0, 100.0);
		Variable::OpVar var2(env, Variable::OpVarType::Con, 0.0, 100.0);
		Variable::OpVar var3(env, Variable::OpVarType::Bool, 0, 1);
		auto linexpr1 = var1 + 2 * var2 + 3 * var3 + 0.5;
		auto linexpr2 = linexpr1 / 2;
		auto quadexpr1 = linexpr1 * linexpr1;
		auto linobj1 = OpMinimize(env, linexpr2);
		auto lincon1 = linexpr1 >= 5;
		auto lincon2 = 4 == linexpr2;
		auto quadcon1 = quadexpr1 <= linexpr1;
		auto sos1con1 = OpSOSCon(env, OpConSense::SOS1, linexpr1);

		OpModel mdl(env, "OPUA_MODEL_TEST");
		mdl.add(lincon1);
		mdl.add(lincon2);
		mdl.add(quadcon1);
		mdl.remove(quadcon1);
		mdl.add(sos1con1);
		mdl.setObj(linobj1);
		mdl.write("MODEL_TEST.lp");
		mdl.release();
		env.release();
		system("pause");
	}
#endif
}
