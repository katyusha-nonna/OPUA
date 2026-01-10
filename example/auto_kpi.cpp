#include <iostream>
#include <ctime>
#include <random>
#include <map>
#include "../src/OPUA/OPUA.h"

/*
	这是一个自动计算绩效打分的程序
*/
int main()
{
	using namespace OPUA;
	using namespace Container;
	using namespace OPUA::Variable;
	using namespace OPUA::Expression;
	using namespace OPUA::Constraint;
	using namespace OPUA::Objective;
	using namespace OPUA::Algorithm;
	using namespace OPUA::Model;
	using namespace OPUA::Solver;

	std::ifstream fin;
	fin.open("config.txt", std::ios::in);
	OpCfgParser cfg(fin);
	OpEnv env(true, "OPUA_ENV_TEST");
	//加载配置文件
	OpLInt N(cfg.getLInt("AutoKPI.StaffNum", OpLInt(10))); //参数-员工人数（默认10）
	OpLInt M(cfg.getLInt("AutoKPI.MonthNum", OpLInt(12))); //参数-月度数（默认12）
	OpLInt K0(cfg.getLInt("AutoKPI.LastQuarterEnd", OpLInt(0))); //参数-上一季度结束月份（默认0， 请填写0-2）
	if (K0 < 0 || K0 > 2 || K0 >= M) K0 = 0;
	OpLInt K(((M - K0) % 3 > 0 ? (M - K0) / 3 + 1 : (M - K0) / 3) + (K0 > 0 ? 1 : 0)); // 参数-季度数
	OpLIntArr qk1(env, K, OpLInt(0));
	OpLIntArr qk2(env, K, OpLInt(0));
	{
		OpULInt k(0);
		for (OpULInt j = 0; j < K0; j++)
			qk2[k] += 1;
		if (K0 > 0)
			qk1[++k] = K0;
		for (OpULInt j0 = K0, j = j0; j < M; j++)
		{
			if (j < j0 + 3)
				qk2[k] += 1;
			else
				qk2[++k] += 1, j0 += 3, qk1[k] = j0;
		}
		//for (OpULInt k = 0; k < K; k++)
			//std::cout << "(" << qk1[k] << ", " << qk2[k] << ")\t";
	}
	OpFloat monLB(cfg.getFloat("AutoKPI.MonthLowerBound", OpFloat(0.08))); //参数-月度激励指标下限（默认0.08）
	OpFloat monUB(cfg.getFloat("AutoKPI.MonthUpperBound", OpFloat(0.1))); //参数-月度激励指标上限（默认0.1）
	OpFloat quarLB(cfg.getFloat("AutoKPI.QuarterLowerBound", OpFloat(0.046))); //参数-季度激励指标下限（默认0.046）
	OpFloat quarUB(cfg.getFloat("AutoKPI.QuarterUpperBound", OpFloat(0.08))); //参数-季度激励指标上限（默认0.08）
	OpFloat monArg(cfg.getLInt("AutoKPI.MonthAverage", OpFloat(100))); //参数-月度分数平均值（默认100）
	OpLInt staffUB(cfg.getLInt("AutoKPI.StaffUpperBound", OpLInt(120))); //参数-个人分数上限（默认120）
	OpLInt staffLB(cfg.getLInt("AutoKPI.StaffLowerBound", OpLInt(80))); //参数-个人分数下限（默认80）
	OpLInt monDiffUB(cfg.getLInt("AutoKPI.MonthDiffUpperBound", OpLInt(25))); //参数-月度分差上限（默认25）
	OpLInt quarDiffUB(cfg.getLInt("AutoKPI.QuarterDiffUpperBound", OpLInt(25))); //参数-季度分差上限（默认25）
	OpFloat randUB1(cfg.getFloat("AutoKPI.RandomUpperBound1", OpFloat(0.005))); //参数-随机值上限1（控制A岗与B岗分差）（默认0.005）
	OpFloat randUB2(cfg.getFloat("AutoKPI.RandomUpperBound2", OpFloat(0.001))); //参数-随机值上限2（控制A岗内部、B岗内部分差）（默认0.001）
	OpBool isRand(cfg.getBool("AutoKPI.IsRandom", OpBool(true))); //参数-是否开启随机化（默认开启）
	OpLInt priorNum(cfg.getLInt("AutoKPI.PriorNum", OpLInt(2))); //参数-A岗人数（默认2，不能高于总人数-1）
	if (priorNum < 0) priorNum = 0;
	if (priorNum > N - 1) priorNum = N - 1;
	OpBool isSemi(cfg.getBool("AutoKPI.IsSemiannual", OpBool(true))); //参数-是否开启半年约束（默认开启）
	if (M < 6)isSemi = false;
	std::default_random_engine rande;
	std::uniform_real_distribution<OpFloat> randd1(randUB1 / 2, randUB1);
	std::uniform_real_distribution<OpFloat> randd2(randUB2 / 2, randUB2);
	std::uniform_int_distribution<OpLInt> randd3(0, 1000 * N);
	OpLIntArr semiOrd(env, N - priorNum, OpLInt(-1));
	rande.seed(time(0));
	{
		std::map<OpLInt, OpLInt> semiOrd0;
		OpLInt tmpc(0);
		for (OpULInt i = priorNum; i < N; i++)
		{
			OpLInt tmp(randd3(rande));
			if (semiOrd0.find(tmp) == semiOrd0.end())
				semiOrd0[tmp] = i;
		}
		for (auto& e : semiOrd0)
			semiOrd[tmpc++] = e.second;
	}

	// 决策变量组
	OpModel mdl(env);
	OpVarMat x(env); // 变量矩阵-整数-打分
	for (OpULInt i = 0; i < N; i++)
	{
		OpVarArr tmp(env);
		for (OpULInt j = 0; j < M; j++)
			tmp.add(OpVar(env, OpVarType::Int, staffLB, staffUB, "x_" + std::to_string(i) + "_" + std::to_string(j)));
		x.add(tmp);
	}
	OpVarArr z01(env); // 变量数组-浮点-个人年度总分（辅助变量）
	for (OpULInt i = 0; i < N; i++)
		z01.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "z01_" + std::to_string(i)));
	OpVarArr z02(env); // 变量数组-浮点-月度总分（辅助变量）
	for (OpULInt j = 0; j < M; j++)
		z02.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "z02_" + std::to_string(j)));
	OpVarArr z03(env); // 变量数组-浮点-季度总分（辅助变量）
	for (OpULInt k = 0; k < K; k++)
		z03.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "z03_" + std::to_string(k)));
	OpVarArr z04(env); // 变量数组-浮点-个人半年总分（辅助变量）
	if (isSemi)
		for (OpULInt i = 0; i < N; i++)
			z04.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "z04_" + std::to_string(i)));
	OpVarArr z1(env); // 变量数组-浮点-个人年度均值绝对值（辅助变量）
	for (OpULInt i = 0; i < N; i++)
		z1.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "z1_" + std::to_string(i)));
	OpVarArr z2(env); // 变量数组-浮点-个人年度均值差值（辅助变量）
	for (OpULInt i = 0; i < N; i++)
		z2.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "z2_" + std::to_string(i)));
	OpVarMat z3(env); // 变量数组-浮点-月度均值绝对值（辅助变量）
	for (OpULInt i = 0; i < N; i++)
	{
		OpVarArr tmp(env);
		for (OpULInt j = 0; j < M; j++)
			tmp.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "z3_" + std::to_string(i) + "_" + std::to_string(j)));
		z3.add(tmp);
	}
	OpVarMat z4(env); // 变量数组-浮点-月度均值差值（辅助变量）
	for (OpULInt i = 0; i < N; i++)
	{
		OpVarArr tmp(env);
		for (OpULInt j = 0; j < M; j++)
			tmp.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "z4_" + std::to_string(i) + "_" + std::to_string(j)));
		z4.add(tmp);
	}
	OpVarMat z5(env); // 变量数组-浮点-季度均值绝对值（辅助变量）
	for (OpULInt i = 0; i < N; i++)
	{
		OpVarArr tmp(env);
		for (OpULInt k = 0; k < K; k++)
			tmp.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "z5_" + std::to_string(i) + "_" + std::to_string(k)));
		z5.add(tmp);
	}
	OpVarMat z6(env); // 变量数组-浮点-季度均值差值（辅助变量）
	for (OpULInt i = 0; i < N; i++)
	{
		OpVarArr tmp(env);
		for (OpULInt k = 0; k < K; k++)
			tmp.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "z6_" + std::to_string(i) + "_" + std::to_string(k)));
		z6.add(tmp);
	}
	OpVarArr rd1(env), rd2(env); // 变量数组-浮点-随机化辅助数组（辅助变量）
	if (isRand)
	{
		for (OpULInt j = 0; j < M - 1; j++)
		{
			rd1.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "rd1_" + std::to_string(j)));
			rd2.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "rd2_" + std::to_string(j)));
		}
	}
	// 中间变量
	OpLinExpr obj; // 中间变量-目标函数
	OpLEArr s1(env, N, OpFloat(0)); // 中间变量数组-个人年度总分
	OpLEArr s2(env, M, OpFloat(0)); // 中间变量数组-员工月度总分
	OpLEArr s3(env, K, OpFloat(0)); // 中间变量数组-员工季度总分
	OpLEArr s4(env, N, OpFloat(0)); // 中间变量数组-个人半年总分
	OpFloat v1(monArg * M); // 中间变量-个人年度总分均值（月度均值*月份数）
	OpFloat v2(monArg); // 中间变量-月度均值
	OpFloat v3(monArg); // 中间变量-季度均值
	//OpQEArr d1(env, N, OpFloat(0)); // 中间变量-年度总分离散值（ (个人年度总分-年度均分)*2 ）
	OpLEArr d1(env, N, OpFloat(0)); // 中间变量-年度总分离散值（ (个人年度总分-年度均分) ）
	//OpArray<OpQEArr> d2(env, N, env, M, OpFloat(0)); // 中间变量-月度总分离散值（ (月度得分-月度均分)*2 ）
	OpArray<OpLEArr> d2(env, N, env, M, OpFloat(0)); // 中间变量-月度总分离散值（ (月度得分-月度均分) ）
	OpArray<OpLEArr> d3(env, N, env, K, OpFloat(0)); // 中间变量-季度总分离散值（ (季度得分-季度均分) ）
	OpLEArr a2(env, M, OpFloat(0)); // 中间变量-月度激励指标
	OpLEArr a3(env, K, OpFloat(0)); // 中间变量-季度激励指标
	for (OpULInt k = 0; k < K; k++)
	{
		OpLInt j0(k * 3 < K0 ? 0 : k * 3);
		OpLInt j1(j0 + 3);
	}
	for (OpLInt i = 0; i < N; i++)
		for (OpLInt j = 0; j < M; j++)
			s1[i] += x[i][j];
	for (OpLInt j = 0; j < M; j++)
		for (OpLInt i = 0; i < N; i++)
			s2[j] += x[i][j];
	for (OpLInt i = 0; i < N; i++)
		for (OpULInt k = 0; k < K; k++)
			for (OpLInt j0(qk1[k]), j1(j0 + qk2[k]), j(j0); j < j1; j++)
				s3[k] += x[i][j];
	if (isSemi)
		for (OpLInt i = 0; i < N; i++)
			for (OpLInt j = 0; j < 6; j++)
				s4[i] += x[i][j];
	for (OpLInt i = 0; i < N; i++)
		obj += z1[i];
	//obj /= v1 * N;
	for (OpLInt i = 0; i < N; i++)
		//d1[i] = (z01[i] - v1) * (z01[i] - v1);
		d1[i] = z01[i] - v1;
	for (OpLInt i = 0; i < N; i++)
		for (OpLInt j = 0; j < M; j++)
			//d2[i][j] = (x[i][j] - v2) * (x[i][j] - v2);
			d2[i][j] = x[i][j] - v2;
	for (OpLInt i = 0; i < N; i++)
		for (OpULInt k = 0; k < K; k++)
			for (OpLInt j0(qk1[k]), j1(j0 + qk2[k]), j(j0); j < j1; j++)
				d3[i][k] += x[i][j] - v2;
	for (OpLInt j = 0; j < M; j++)
		for (OpLInt i = 0; i < N; i++)
			a2[j] += z3[i][j];
	for (OpLInt k = 0; k < K; k++)
		for (OpLInt i = 0; i < N; i++)
			a3[k] += z5[i][k];
	// 创建约束条件
	// 年度
	for (OpLInt i = 0; i < N; i++)
		mdl.add(OpLinCon(env, OpFloat(0), s1[i] - z01[i], OpFloat(0)));
	for (OpLInt i = 0; i < N; i++)
		//mdl.add(OpQuadCon(env, OpFloat(0), d1[i] - z2[i], OpFloat(0)));
		mdl.add(OpLinCon(env, OpFloat(0), d1[i] - z2[i], OpFloat(0)));
	for (OpLInt i = 0; i < N; i++)
		//mdl.add(OpSqrt(env, z1[i], z2[i]));
		mdl.add(OpAbs(env, z1[i], z2[i]));
	// 月度
	for (OpLInt j = 0; j < M; j++)
		mdl.add(OpLinCon(env, OpFloat(0), s2[j] - z02[j], OpFloat(0)));
	for (OpLInt i = 0; i < N; i++)
		for (OpLInt j = 0; j < M; j++)
			//mdl.add(OpQuadCon(env, OpFloat(0), d2[i][j] - z4[i][j], OpFloat(0)));
			mdl.add(OpLinCon(env, OpFloat(0), d2[i][j] - z4[i][j], OpFloat(0)));
	for (OpLInt i = 0; i < N; i++)
		for (OpLInt j = 0; j < M; j++)
			//mdl.add(OpSqrt(env, z3[i][j], z4[i][j]));
			mdl.add(OpAbs(env, z3[i][j], z4[i][j]));
	for (OpLInt j = 0; j < M; j++)
		mdl.add(OpLinCon(env, monLB * v2 * N, a2[j], monUB * v2 * N));
	for (OpLInt j = 0; j < M; j++)
		mdl.add(OpLinCon(env, v2 * N, s2[j], v2 * N));
	// 季度
	for (OpLInt k = 0; k < K; k++)
		mdl.add(OpLinCon(env, OpFloat(0), s3[k] - z03[k], OpFloat(0)));
	for (OpLInt i = 0; i < N; i++)
		for (OpLInt k = 0; k < K; k++)
			mdl.add(OpLinCon(env, OpFloat(0), d3[i][k] - z6[i][k], OpFloat(0)));
	for (OpLInt i = 0; i < N; i++)
		for (OpLInt k = 0; k < K; k++)
			mdl.add(OpAbs(env, z5[i][k], z6[i][k]));
	for (OpLInt k = 0; k < K; k++)
		mdl.add(OpLinCon(env, quarLB * v2 * qk2[k] * N, a3[k], quarUB * v2 * qk2[k] * N));
	// 半年度
	if (isSemi)
		for (OpLInt i = 0; i < N; i++)
			mdl.add(OpLinCon(env, OpFloat(0), s4[i] - z04[i], OpFloat(0)));
	// 随机化
	if (isRand)
	{
		// 年度总分差约束（A岗之间、A岗与非A岗之间，非A岗之间）
		if (priorNum > 0)
		{
			// A岗岗内分差
			for (OpLInt i = 0; i < priorNum - 1; i++)
				mdl.add(OpLinCon(env, v1 * randd2(rande), z01[i] - z01[i + 1], Constant::Infinity));
			// A-B岗分差
			mdl.add(OpLinCon(env, v1 * randd1(rande), z01[priorNum - 1] - z01[priorNum], Constant::Infinity));
		}
		// B岗岗内分差
		for (OpLInt i = priorNum; i < N - 1; i++)
			mdl.add(OpLinCon(env, v1 * randd2(rande), z01[i] - z01[i + 1], Constant::Infinity));
		// 半年总分差约束
		if (isSemi)
		{
			if (priorNum > 0)
			{
				// A岗岗内分差
				for (OpLInt i = 0; i < priorNum - 1; i++)
					mdl.add(OpLinCon(env, v2 * 6 * randd2(rande), z04[i] - z04[i + 1], Constant::Infinity));
				// A-B岗分差
				mdl.add(OpLinCon(env, v2 * 6 * randd1(rande), z04[priorNum - 1] - z04[semiOrd[0]], Constant::Infinity));
			}
			// B岗之间随机排序分差
			for (OpLInt ii = 0; ii < N - priorNum - 1; ii++)
				mdl.add(OpLinCon(env, v2 * 6 * randd2(rande), z04[semiOrd[ii]] - z04[semiOrd[ii + 1]], Constant::Infinity));
		}
	}
	// 目标函数
	mdl.setObj(OpMinimize(env, obj));
	//mdl.write("");
	// 创建求解器对象并抽取模型
	OpSCIPSol solver(env, mdl);
	solver.write("test.cip");
	// 创建配置器并配置求解参数
	OpConfig config;
	config.regCfg("OPUA.SCIP.Limits.Time", cfg.getFloat("OPUA.SCIP.Limits.Time", OpFloat(3600)));
	config.regCfg("OPUA.SCIP.Limits.Gap", cfg.getFloat("OPUA.SCIP.Limits.Gap", OpFloat(1e-2)));
	config.regCfg("OPUA.SCIP.Parallel.MaxNThreads", cfg.getInt("OPUA.SCIP.Parallel.MaxNThreads", OpInt(1)));
	solver.setParam(config);
	// 求解模型
	solver.solve();
	// 输出结果
	OpDisplay dp("result.txt");
	dp << "Status:\t" << solver.getStatus(), dp.newLine();
	dp << "Obj:\t" << solver.getObjValue(), dp.newLine();
	dp << "x: ", dp.newLine();
	for (OpULInt i = 0; i < N; i++)
	{
		for (OpULInt j = 0; j < M; j++)
			dp << solver.getValue(x[i][j]) << '\t';
		dp.newLine();
	}
	// 释放内存
	mdl.release();
	env.release();
	system("pause");
	return 0;
}