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
	// ����OPUA����
	OpEnv env(true, "OPUA_ENV_TEST");
	// ����OPUAģ�Ͷ���
	OpModel mdl(env, "N-Queens");
	// ��������
	constexpr OpULInt N = 8;
	std::vector<std::vector<OpVar> > x(N);
	for (OpULInt i = 0; i < N; i++)
		x[i] = std::vector<OpVar>(N);
	for (OpULInt i = 0; i < N; i++)
		for (OpULInt j = 0; j < N; j++)
			x[i][j] = OpVar(env, OpVarType::Bool, 0, 1, "x_" + std::to_string(i) + "_" + std::to_string(j));
	// ���������Լ������
	// Լ��1����֮�Ͳ��ܳ���1
	for (OpULInt j = 0; j < N; j++)
	{
		OpLinExpr colSum(0.0);
		for (OpULInt i = 0; i < N; i++)
			colSum += x[i][j];
		mdl.add(colSum <= 1);
	}
	// Լ��2����֮�Ͳ��ܳ���1
	for (OpULInt i = 0; i < N; i++)
	{
		OpLinExpr rowSum(0.0);
		for (OpULInt j = 0; j < N; j++)
			rowSum += x[i][j];
		mdl.add(rowSum <= 1);
	}
	// Լ��3���Խ���֮�Ͳ��ܳ���1
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
	// ����Ŀ�꺯��	
	{
		OpLinExpr obj(0.0);
		for (size_t i = 0; i < N; i++)
			for (size_t j = 0; j < N; j++)
				obj += x[i][j];
		mdl.setObj(OpMaximize(env, obj));
	}
	// ���ģ��
	mdl.write("");
	// ������������󲢳�ȡģ��
	OpAdapSol solver(OpSolType::GRB, env, mdl);
	// ����������������������
	OpConfig config;
	config.regCfg("OPUA.GRB.Termination.TimeLimit", OpFloat(60));
	config.regCfg("OPUA.GRB.Tolerances.MIPGap", OpFloat(1e-5));
	solver.setParam(config);
	// ���ģ��
	solver.solve();
	// ������
	std::cout << "Status:\t" << solver.getStatus() << std::endl;
	std::cout << "Obj:\t" << solver.getObjValue() << std::endl;
	std::cout << "x: " << std::endl;
	for (OpULInt i = 0; i < N; i++)
	{
		for (OpULInt j = 0; j < N; j++)
			std::cout << solver.getValue(x[i][j]) << '\t';
		std::cout << std::endl;
	}
	// �ͷ��ڴ�
	env.release();
	system("pause");
	return 0;
}

/*
	���н����

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
	�밴���������. . .
*/