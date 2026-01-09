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
	OpHierarchicalModel model(env);
	OpLInt masteridx(-1), sub1idx(-1), sub2idx(-1);
	masteridx = model.initRoot();
	sub1idx = model.addLower(masteridx);
	sub2idx = model.addLower(masteridx);
	// 构建主问题
	auto x1m = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x1m");
	auto x2m = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x2m");
	auto Rm = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "Rm");
	model.add(masteridx, x1m);
	model.add(masteridx, x2m);
	model.add(masteridx, Rm);
	model.add(masteridx, Rm == x1m + x2m);
	// 构建子问题1
	auto Rs1 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "Rs1");
	auto x3s1 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x3s1");
	model.add(sub1idx, Rs1);
	model.add(sub1idx, x3s1);
	model.add(sub1idx, 0 >= Rs1 + (x3s1 - 5) * (x3s1 - 5) - 3);
	model.setObj(sub1idx, -Rs1 + x3s1);
	// 构建子问题2
	auto x1s2 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x1s2");
	auto x2s2 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x2s2");
	model.add(sub2idx, x1s2);
	model.add(sub2idx, x2s2);
	model.add(sub2idx, 0 >= x1s2 + x2s2 - 4);
	model.setObj(sub2idx, (x1s2 + x2s2) * (x1s2 + x2s2));
	// 关联变量
	model.link(masteridx, sub1idx, Rm, Rs1);
	model.link(masteridx, sub2idx, x1m, x1s2);
	model.link(masteridx, sub2idx, x2m, x2s2);
	// 创建ATC求解器&配置器
	OpAlgoATC solver(model);
	auto config = DefaultCfg4ATC();
	config.regCfg("OPUA.Algorithm.ATC.LambdaInitVal", OpFloat(0.5));
	config.regCfg("OPUA.Algorithm.ATC.RhoInitVal", OpFloat(1));
	config.regCfg("OPUA.Algorithm.ATC.RhoUpdateFactor", OpFloat(1.05));
	config.regCfg("OPUA.Algorithm.ATC.ATCShakeCount", OpLInt(3));
	config.regCfg("OPUA.Algorithm.ATC.ATCIterMax", OpLInt(100));
	config.regCfg("OPUA.Algorithm.ATC.MIPSolverMode", OpChar('C'));
	config.regCfg("OPUA.GRB.MIP.NonConvex", OpInt(2));
	auto flag = solver.solve(config);
	// 输出解
	if (flag)
	{
		auto printSol = [](auto& var, auto& model, auto idx) {
			std::cout << var.getName() << ": " << model.getValue(idx, var) << std::endl;
		};
		printSol(x1m, model, masteridx);
		printSol(x2m, model, masteridx);
		printSol(Rm, model, masteridx);
		printSol(Rs1, model, sub1idx);
		printSol(x3s1, model, sub1idx);
		printSol(x1s2, model, sub2idx);
		printSol(x2s2, model, sub2idx);
		std::cout << "Objm: " << model.getObjValue(masteridx) << std::endl;
		std::cout << "Objs1: " << model.getObjValue(sub1idx) << std::endl;
		std::cout << "Objs2: " << model.getObjValue(sub2idx) << std::endl;
		std::cout << "Obj(original): " << model.getObjValue(-1) << std::endl;
	}
	env.release();
	system("pause");
	return 0;
}

/*
	运行结果：

	x1m: 0.190931
	x2m: 0.190948
	Rm: 0.381879
	Rs1: 0.387181
	x3s1: 3.38358
	x1s2: 0.193509
	x2s2: 0.193537
	Objm: 0
	Objs1: 2.9964
	Objs2: 0.149805
	Obj(original): 3.1462
	请按任意键继续. . .
*/