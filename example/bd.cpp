#include <iostream>
#include "../src/OPUA/OPUA.h"

int main()
{
	using namespace OPUA;
	using namespace OPUA::Variable;
	using namespace OPUA::Expression;
	using namespace OPUA::Constraint;
	using namespace OPUA::Objective;
	using namespace OPUA::Algorithm;

	OpEnv env(true, "OPUA_ENV_TEST");
	OpMSSPModel model(env);
	OpLInt subIdx(model.addSubProb());
	// 构建主问题
	auto y1 = OpVar(env, OpVarType::Bool, 0, 1, "y1");
	auto y2 = OpVar(env, OpVarType::Bool, 0, 1, "y2");
	auto y3 = OpVar(env, OpVarType::Bool, 0, 1, "y3");
	auto y4 = OpVar(env, OpVarType::Bool, 0, 1, "y4");
	auto y5 = OpVar(env, OpVarType::Bool, 0, 1, "y5");
	model.add(y1);
	model.add(y2);
	model.add(y3);
	model.add(y4);
	model.add(y5);
	model.setObj(7 * y1 + 7 * y2 + 7 * y3 + 7 * y4 + 7 * y5);
	// 构建子问题
	auto x1 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x1");
	auto x2 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x2");
	auto x3 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x3");
	auto x4 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x4");
	auto x5 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x5");
	auto s1p = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s1+");
	auto s2p = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s2+");
	auto s3p = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s3+");
	auto s1n = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s1-");
	auto s2n = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s2-");
	auto s3n = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s3-");
	OpFloat pentyFactor(1e5);
	model.add(x1, subIdx);
	model.add(x2, subIdx);
	model.add(x3, subIdx);
	model.add(x4, subIdx);
	model.add(x5, subIdx);
	model.add(s1p, subIdx);
	model.add(s2p, subIdx);
	model.add(s3p, subIdx);
	model.add(s1n, subIdx);
	model.add(s2n, subIdx);
	model.add(s3n, subIdx);
	model.setObj(x1 + x2 + x3 + x4 + x5 + pentyFactor * (s1p + s1n + s2p + s2n + s3p + s3n), subIdx);
	model.add(x1 + x4 + x5 + s1p - s1n == 8, subIdx);
	model.add(x2 + x5 + s2p - s2n == 3, subIdx);
	model.add(x3 + x4 + s3p - s3n == 5, subIdx);
	model.add(x1 <= 8 * y1, subIdx);
	model.add(x2 <= 3 * y2, subIdx);
	model.add(x3 <= 5 * y3, subIdx);
	model.add(x4 <= 5 * y4, subIdx);
	model.add(x5 <= 3 * y5, subIdx);
	// 创建BD求解器&配置器
	OpAlgoBD solver(model);
	auto config = DefaultCfg4BD();
	config.regCfg("OPUA.CPX.Preprocessing.Presolve", OpBool(false));
	auto flag = solver.solve(config);
	// 输出解
	if (flag)
	{
		std::cout << "Objm: " << model.getObjValue() << std::endl;
		std::cout << "Objs: " << model.getObjValue(subIdx) << std::endl;
		std::cout << "y1: " << model.getValue(y1) << std::endl;
		std::cout << "y2: " << model.getValue(y2) << std::endl;
		std::cout << "y3: " << model.getValue(y3) << std::endl;
		std::cout << "y4: " << model.getValue(y4) << std::endl;
		std::cout << "y5: " << model.getValue(y5) << std::endl;
		std::cout << "x1: " << model.getValue(x1, subIdx) << std::endl;
		std::cout << "x2: " << model.getValue(x2, subIdx) << std::endl;
		std::cout << "x3: " << model.getValue(x3, subIdx) << std::endl;
		std::cout << "x4: " << model.getValue(x4, subIdx) << std::endl;
		std::cout << "x5: " << model.getValue(x5, subIdx) << std::endl;
	}
	env.release();
	system("pause");
	return 0;
}

/*
	运行结果：

	Objm: 14
	Objs: 8
	y1: 0
	y2: 0
	y3: 0
	y4: 1
	y5: 1
	x1: 0
	x2: 0
	x3: 0
	x4: 5
	x5: 3
	请按任意键继续. . .
*/