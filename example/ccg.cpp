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
	using namespace OPUA::Algorithm;

	// 创建环境变量
	OpEnv env(true, "OPUA_ENV_TEST");
	// 创建两阶段鲁棒模型
	OpRobustModel model(env);
	// 创建变量
	OpVarArr y(env);
	for (OpULInt i = 0; i < 3; i++)
		y.add(OpVar(env, OpVarType::Bool, 0, 1, "y_" + std::to_string(i)));
	OpVarArr z(env);
	for (OpULInt i = 0; i < 3; i++)
		z.add(OpVar(env, OpVarType::Con, 0, 800, "z_" + std::to_string(i)));
	OpVarMat x(env);
	for (OpULInt i = 0; i < 3; i++)
	{
		OpVarArr tmp(env);
		for (OpULInt j = 0; j < 3; j++)
			tmp.add(OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x_" + std::to_string(i) + "_" + std::to_string(j)));
		x.add(tmp);
	}
	OpVarArr s(env);
	for (OpULInt i = 0; i < 6; i++)
		s.add(OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s_" + std::to_string(i)));
	OpFloat alpha = 1e3;
	OpVarArr d(env);
	for (OpULInt i = 0; i < 3; i++)
		d.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "d_" + std::to_string(i)));
	d[0].setLb(206), d[0].setUb(246);
	d[1].setLb(274), d[1].setUb(314);
	d[2].setLb(220), d[2].setUb(260);
	OpVarArr g(env);
	for (OpULInt i = 0; i < 3; i++)
		g.add(OpVar(env, OpVarType::Con, 0, 1, "g_" + std::to_string(i)));
	// 形成两阶段鲁棒模型
	// [1] 第一阶段
	// [1-1] 目标函数
	model.setObj(400 * y[0] + 414 * y[1] + 326 * y[2] + 18 * z[0] + 25 * z[1] + 20 * z[2], RobustStageType::FirstStage);
	// [1-2] 约束条件
	for (int i = 0; i < 3; i++)
		model.add(0 <= 800 * y[i] - z[i], RobustStageType::FirstStage, true);
	// [1-3] 决策变量
	model.add(y, RobustStageType::FirstStage, true);
	model.add(z, RobustStageType::FirstStage, true);
	// [2] 第二阶段原问题
	// [2-1] 目标函数
	model.setObj(22 * x[0][0] + 33 * x[0][1] + 24 * x[0][2]
		+ 33 * x[1][0] + 23 * x[1][1] + 30 * x[1][2]
		+ 20 * x[2][0] + 25 * x[2][1] + 27 * x[2][2]
		+ alpha * (s[0] + s[1] + s[2] + s[3] + s[4] + s[5]), RobustStageType::SecondStagePrimal);
	// [2-2] 约束条件
	for (int i = 0; i < 3; i++)
		model.add(s[i] + z[i] - (x[i][0] + x[i][1] + x[i][2]) + 5 >= 5, RobustStageType::SecondStagePrimal, true);
	for (int i = 0; i < 3; i++)
		model.add(s[i + 3] + x[0][i] + x[1][i] + x[2][i] - d[i] >= 0, RobustStageType::SecondStagePrimal, true);
	// [2-3] 决策变量
	for (int i = 0; i < 3; i++)
		model.add(x[i], RobustStageType::SecondStagePrimal, true);
	model.add(s, RobustStageType::SecondStagePrimal, true);
	// [4] 不确定集
	// [4-1] 约束条件
	model.add(d[0] - 40 * g[0] - 206 == 0, RobustStageType::Uncertainty, false);
	model.add(d[1] - 40 * g[1] - 274 == 0, RobustStageType::Uncertainty, false);
	model.add(d[2] - 40 * g[2] - 220 == 0, RobustStageType::Uncertainty, false);
	model.add(1.8 >= g[0] + g[1] + g[2], RobustStageType::Uncertainty, true);
	model.add(1.2 >= g[0] + g[1], RobustStageType::Uncertainty, true);
	// [4-2] 决策变量
	model.add(d, RobustStageType::Uncertainty, true);
	model.add(g, RobustStageType::Uncertainty, true);
	// 自动推导对偶
	model.autoStd(RobustStageType::SecondStagePrimal);
	model.autoDual();
	model.update();
	// 设置初始解
	model.setValue(d[0].getImpl(), RobustStageType::Uncertainty, 206);
	model.setValue(d[1].getImpl(), RobustStageType::Uncertainty, 274);
	model.setValue(d[2].getImpl(), RobustStageType::Uncertainty, 220);
	model.setValue(g[0].getImpl(), RobustStageType::Uncertainty, 0);
	model.setValue(g[1].getImpl(), RobustStageType::Uncertainty, 0);
	model.setValue(g[2].getImpl(), RobustStageType::Uncertainty, 0);
	// 导出模型
	model.write("./Model/Test");
	// 创建CCG求解器&配置器
	OpAlgoCCG solver(model);
	auto config = DefaultCfg4CCG();
	config.regCfg("OPUA.Algorithm.CCG.SubProbSolveMode", OpLInt(0));
	config.regCfg("OPUA.Algorithm.CCG.MIPSolverMode", OpChar('C'));
	auto flag = solver.solve(config);
	// 输出解
	if (flag)
	{
		auto printSol = [](auto& varArr, auto& model, auto stage) {
			for (OpULInt i = 0; i < varArr.getSize(); i++)
				std::cout << model.getValue(varArr[i], stage) << '\t';
		};
		std::cout << "y: ";
		printSol(y, model, RobustStageType::FirstStage);
		std::cout << std::endl;
		std::cout << "z: ";
		printSol(z, model, RobustStageType::FirstStage);
		std::cout << std::endl;
		std::cout << "x: ";
		for (OpULInt i = 0; i < x.getSize(); i++)
			printSol(x[i], model, RobustStageType::SecondStagePrimal);
		std::cout << std::endl;
		std::cout << "d: ";
		printSol(d, model, RobustStageType::Uncertainty);
		std::cout << std::endl;
		std::cout << "g: ";
		printSol(g, model, RobustStageType::Uncertainty);
		std::cout << std::endl;
		std::cout << "obj(auto): " << model.getObjValue(RobustStageType::Unknown) << std::endl;
	}
	model.release();
	env.release();
	system("pause");
	return 0;
}

/*
	运行结果：

	y: 1	0	1
	z: 252	0	520
	x: 0	0	252	0	0	0	206	314	0
	d: 206	314	252
	g: 0	1	0.8
	obj(auto): 33680
	请按任意键继续. . .
*/