#include <iostream>
#include "OPUA.h"

int main()
{
	using namespace OPUA;
	using namespace OPUA::Container;
	using namespace OPUA::Variable;
	using namespace OPUA::Expression;
	using namespace OPUA::Constraint;
	using namespace OPUA::Objective;
	using namespace OPUA::Model;
	using namespace OPUA::Solver;
	// 创建OPUA环境
	OpEnv env(true, "OPUA_ENV_TEST");
	// 创建OPUA模型对象
	OpModel mdl(env, "N-Queens");
	// 创建变量
	constexpr OpULInt N = 8;
	std::vector<std::vector<OpVar> > x(N);
	for (OpULInt i = 0; i < N; i++)
		x[i] = std::vector<OpVar>(N);
	for (OpULInt i = 0; i < N; i++)
		for (OpULInt j = 0; j < N; j++)
			x[i][j] = OpVar(env, OpVarType::Bool, 0, 1, "x_" + std::to_string(i) + "_" + std::to_string(j));
	// 创建并添加约束条件
	// 约束1：列之和不能超过1
	for (OpULInt j = 0; j < N; j++)
	{
		OpLinExpr colSum(0.0);
		for (OpULInt i = 0; i < N; i++)
			colSum += x[i][j];
		mdl.add(colSum <= 1);
	}
	// 约束2：行之和不能超过1
	for (OpULInt i = 0; i < N; i++)
	{
		OpLinExpr rowSum(0.0);
		for (OpULInt j = 0; j < N; j++)
			rowSum += x[i][j];
		mdl.add(rowSum <= 1);
	}
	// 约束3：对角线之和不能超过1
	for (OpULInt j = 0; j < N; j++)
	{
		OpLinExpr diagSum(0.0);
		for (OpULInt i = 0; i < N - j; i++)
			diagSum += x[i][j + i];
		mdl.add(diagSum <= 1);
	}
	for (OpULInt i = 0; i < N; i++)
	{
		OpLinExpr diagSum(0.0);
		for (OpULInt j = 0; j < N - i; j++)
			diagSum += x[i + j][j];
		mdl.add(diagSum <= 1);
	}
	for (OpULInt j = 0; j < N; j++)
	{
		OpLinExpr diagSum(0.0);
		for (OpULInt i = 0; i < N - j; i++)
			diagSum += x[i][N - j - i - 1];
		mdl.add(diagSum <= 1);
	}
	for (OpULInt i = 0; i < N; i++)
	{
		OpLinExpr diagSum(0.0);
		for (OpULInt j = 0; j < N - i; j++)
			diagSum += x[i + j][N - j - 1];
		mdl.add(diagSum <= 1);
	}
	// 创建目标函数	
	{
		OpLinExpr obj(0.0);
		for (size_t i = 0; i < N; i++)
			for (size_t j = 0; j < N; j++)
				obj += x[i][j];
		mdl.setObj(OpMaximize(env, obj));
	}
	// 输出模型
	mdl.write("");
	// 创建求解器对象并抽取模型
	OpAdapSol solver(OpSolType::GRB, env, mdl);
	// 创建配置器并配置求解参数
	OpConfig config;
	config.regCfg("OPUA.GRB.Termination.TimeLimit", OpFloat(60));
	config.regCfg("OPUA.GRB.Tolerances.MIPGap", OpFloat(1e-5));
	solver.setParam(config);
	// 求解模型
	solver.solve();
	// 输出结果
	std::cout << "Status:\t" << solver.getStatus() << std::endl;
	std::cout << "Obj:\t" << solver.getObjValue() << std::endl;
	std::cout << "x: " << std::endl;
	for (OpULInt i = 0; i < N; i++)
	{
		for (OpULInt j = 0; j < N; j++)
			std::cout << solver.getValue(x[i][j]) << '\t';
		std::cout << std::endl;
	}
	// 释放内存
	env.release();
	system("pause");
	return 0;
}

/*
	运行结果：

	Status: 2
	Obj:    8
	x:
	0       0       0       1       0       0       0       0
	0       1       0       0       0       0       0       0
	0       0       0       0       0       0       1       0
	0       0       1       0       0       0       0       0
	0       0       0       0       0       1       0       0
	0       0       0       0       0       0       0       1
	1       0       0       0       0       0       0       0
	0       0       0       0       1       0       0       0
	请按任意键继续. . .
*/