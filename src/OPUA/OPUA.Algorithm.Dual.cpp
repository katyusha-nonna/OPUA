#include "OPUA.Algorithm.h"
#include "OPUA.Exception.h"
#include <set>

using namespace OPUA;

void Algorithm::OpAutoKKTSet::init(OpEnv env)
{
	primalVar = Variable::OpVarArr(env);
	dualVar = Variable::OpVarArr(env);
	slackVar = Variable::OpVarArr(env);
	gradCon = Constraint::OpLCArr(env);
	primalCon = Constraint::OpLCArr(env);
	dualCon = Constraint::OpLCArr(env);
	csCon1 = Constraint::OpLCArr(env);
	csCon2 = Constraint::OpQCArr(env);
}

void Algorithm::OpAutoKKTSet::clear(OpBool deep)
{
	if (deep)
	{
		csCon2.releaseElements();
		csCon1.releaseElements();
		dualCon.releaseElements();
		primalCon.releaseElements();
		gradCon.releaseElements();
		slackVar.releaseElements();
		dualVar.releaseElements();
		primalVar.releaseElements();
	}
	csCon2.release();
	csCon1.release();
	dualCon.release();
	primalCon.release();
	gradCon.release();
	slackVar.release();
	dualVar.release();
	primalVar.release();	
}

Algorithm::OpAutoKKTSet::OpAutoKKTSet()
{

}

Algorithm::OpAutoKKTSet::OpAutoKKTSet(OpEnv env)
	: primalVar(env),
	dualVar(env),
	slackVar(env),
	gradCon(env),
	primalCon(env),
	dualCon(env),
	csCon1(env),
	csCon2(env)
{

}

void Algorithm::OpAlgoAutoKKT(OpEnv env, OpAutoKKTSet& result, Variable::OpVarArr X0, Variable::OpVarArr Y0, Constraint::OpLCArr NEC0, Constraint::OpLCArr EC0, const Expression::OpLinExpr& O0, Container::OpFloatArr M, OpBool relax)
{
	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;

	const auto NX1 = X0.getSize();
	const auto NX2 = Y0.getSize();
	const auto NC1= NEC0.getSize();
	const auto NC2 = EC0.getSize();
	std::unordered_map<OpVarI*, std::set<OpULInt> > XY0AtNEC0; // 决策变量出现的不等式约束范围查询表
	std::unordered_map<OpVarI*, std::set<OpULInt> > XY0AtEC0; // 决策变量出现的等式约束范围查询表
	// 创建对偶变量
	OpVarArr lbd(env, NC1, env, OpVarType::Con, 0, Constant::Infinity);
	OpVarArr mu(env, NC2, env, OpVarType::Con, -Constant::Infinity, Constant::Infinity);
	// 首先统计变量出现的约束的范围
	for (OpULInt i = 0; i < NC1; i++)
		for (auto it = NEC0[i].getExpr().getLBegin(); it != NEC0[i].getExpr().getLEnd(); ++it)
			XY0AtNEC0[it.getVar().getImpl()].emplace(i);
	for (OpULInt i = 0; i < NC2; i++)
		for (auto it = EC0[i].getExpr().getLBegin(); it != EC0[i].getExpr().getLEnd(); ++it)
			XY0AtEC0[it.getVar().getImpl()].emplace(i);
	// 添加梯度约束
	for (OpULInt i = 0; i < NX1; i++)
	{
		auto var(X0[i]);
		OpLinExpr expr(0.0);
		OpFloat objCoeff(O0.getCoeff(var));
		OpBool flag(false);
		if (XY0AtNEC0.find(var.getImpl()) != XY0AtNEC0.end())
		{
			for (auto& j : XY0AtNEC0.at(var.getImpl()))
				expr.addLinTerm(lbd[j], NEC0[j].getExpr().getCoeff(var));
			flag = true;
		}
		if (XY0AtEC0.find(var.getImpl()) != XY0AtEC0.end())
		{
			for (auto& j : XY0AtEC0.at(var.getImpl()))
				expr.addLinTerm(mu[j], EC0[j].getExpr().getCoeff(var));
			flag = true;
		}
		if (flag)
		{
			result.gradCon.add(OpLinCon(env, -Constant::Infinity, expr, objCoeff));
		}
		else
		{
			throw OpExcBase("[OPUA::Algorithm::OpAlgoAutoKKT]: Exception-> variable(X0) "
				+ var.getName()
				+ " does not belong to any constraint");
		}
	}
	for (OpULInt i = 0; i < NX2; i++)
	{
		auto var(Y0[i]);
		OpLinExpr expr(0.0);
		OpFloat objCoeff(O0.getCoeff(var));
		OpBool flag(false);
		if (XY0AtNEC0.find(var.getImpl()) != XY0AtNEC0.end())
		{
			for (auto& j : XY0AtNEC0.at(var.getImpl()))
				expr.addLinTerm(lbd[j], NEC0[j].getExpr().getCoeff(var));
			flag = true;
		}
		if (XY0AtEC0.find(var.getImpl()) != XY0AtEC0.end())
		{
			for (auto& j : XY0AtEC0.at(var.getImpl()))
				expr.addLinTerm(mu[j], EC0[j].getExpr().getCoeff(var));
			flag = true;
		}
		if (flag)
		{
			result.gradCon.add(OpLinCon(env, objCoeff, expr, objCoeff));
		}
		else
		{
			throw OpExcBase("[OPUA::Algorithm::OpAlgoAutoKKT]: Exception-> variable(Y0) "
				+ var.getName()
				+ " does not belong to any constraint");
		}
	}
	// 添加原问题变量&原问题可行约束
	result.primalCon.add(NEC0);
	result.primalCon.add(EC0);
	result.primalVar.add(X0);
	result.primalVar.add(Y0);
	// 添加对偶问题变量&对偶问题可行约束
	result.dualVar.add(lbd);
	result.dualVar.add(mu);
	// 添加辅助变量&互补松弛约束
	if (relax)
	{
		for (OpULInt i = 0; i < NC1; i++)
		{
			OpVar slack(env, OpVarType::Bool, 0, 1);
			result.csCon1.add(OpLinCon(env, -Constant::Infinity, lbd[i] - M[i] * slack, 0.0));
			result.csCon1.add(OpLinCon(env, -Constant::Infinity, NEC0[i].getExpr() + M[i] * slack, M[i] + NEC0[i].getLb()));
			result.slackVar.add(slack);
		}
	}
	else
	{
		for (OpULInt i = 0; i < NC1; i++)
			result.csCon2.add(OpQuadCon(env, 0.0, lbd[i] * (NEC0[i].getExpr() - NEC0[i].getLb()), 0.0));
	}
	// 释放临时对象
	lbd.release();
	mu.release();
}

Algorithm::OpAutoKKTSet Algorithm::OpAlgoAutoKKT(OpEnv env, Variable::OpVarArr X0, Variable::OpVarArr Y0, Constraint::OpLCArr NEC0, Constraint::OpLCArr EC0, const Expression::OpLinExpr& O0, Container::OpFloatArr M, OpBool relax)
{
	OpAutoKKTSet result(env);
	OpAlgoAutoKKT(env, result, X0, Y0, NEC0, EC0, O0, M, relax);
	return result;
}

void Algorithm::OpAutoDualSet::init(OpEnv env)
{
	dualVar = Variable::OpVarArr(env);
	dualCon = Constraint::OpLCArr(env);
	dualObj = Expression::OpLinExpr(0.0);
}

void Algorithm::OpAutoDualSet::clear(OpBool deep)
{
	if (deep)
	{
		dualVar.releaseElements();
		dualCon.releaseElements();
	}
	dualObj.clear();
	dualCon.release();	
	dualVar.release();
}

Algorithm::OpAutoDualSet::OpAutoDualSet()
{

}

Algorithm::OpAutoDualSet::OpAutoDualSet(OpEnv env)
	: dualVar(env),
	dualCon(env),
	dualObj(0.0)
{

}

void Algorithm::OpAlgoAutoDual(OpEnv env, OpAutoDualSet& result, Variable::OpVarArr X0, Variable::OpVarArr Y0, Constraint::OpLCArr NEC0, Constraint::OpLCArr EC0, const Expression::OpLinExpr& O0)
{
	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;

	const auto NX1 = X0.getSize();
	const auto NX2 = Y0.getSize();
	const auto NC1 = NEC0.getSize();
	const auto NC2 = EC0.getSize();
	std::unordered_map<OpVarI*, std::set<OpULInt> > XY0AtNEC0; // 决策变量出现的不等式约束范围查询表
	std::unordered_map<OpVarI*, std::set<OpULInt> > XY0AtEC0; // 决策变量出现的等式约束范围查询表
	// 创建对偶变量
	OpVarArr lbd(env, NC1, env, OpVarType::Con, 0, Constant::Infinity);
	OpVarArr mu(env, NC2, env, OpVarType::Con, -Constant::Infinity, Constant::Infinity);
	// 首先统计变量出现的约束的范围
	for (OpULInt i = 0; i < NC1; i++)
		for (auto it = NEC0[i].getExpr().getLBegin(); it != NEC0[i].getExpr().getLEnd(); ++it)
			XY0AtNEC0[it.getVar().getImpl()].emplace(i);
	for (OpULInt i = 0; i < NC2; i++)
		for (auto it = EC0[i].getExpr().getLBegin(); it != EC0[i].getExpr().getLEnd(); ++it)
			XY0AtEC0[it.getVar().getImpl()].emplace(i);
	// 创建对偶约束
	for (OpULInt i = 0; i < NX1; i++)
	{
		auto var(X0[i]);
		OpLinExpr expr(0.0);
		OpFloat objCoeff(O0.getCoeff(var));
		OpBool flag(false);
		if (XY0AtNEC0.find(var.getImpl()) != XY0AtNEC0.end())
		{
			for (auto& j : XY0AtNEC0.at(var.getImpl()))
				expr.addLinTerm(lbd[j], NEC0[j].getExpr().getCoeff(var));
			flag = true;
		}
		if (XY0AtEC0.find(var.getImpl()) != XY0AtEC0.end())
		{
			for (auto& j : XY0AtEC0.at(var.getImpl()))
				expr.addLinTerm(mu[j], EC0[j].getExpr().getCoeff(var));
			flag = true;
		}
		if (flag)
		{
			result.dualCon.add(OpLinCon(env, -Constant::Infinity, expr, objCoeff));
		}
		else
		{
			throw OpExcBase("[OPUA::Algorithm::OpAlgoAutoDual]: Exception-> variable(X0) "
				+ var.getName()
				+ " does not belong to any constraint");
		}
	}
	for (OpULInt i = 0; i < NX2; i++)
	{
		auto var(Y0[i]);
		OpLinExpr expr(0.0);
		OpFloat objCoeff(O0.getCoeff(var));
		OpBool flag(false);
		if (XY0AtNEC0.find(var.getImpl()) != XY0AtNEC0.end())
		{
			for (auto& j : XY0AtNEC0.at(var.getImpl()))
				expr.addLinTerm(lbd[j], NEC0[j].getExpr().getCoeff(var));
			flag = true;
		}
		if (XY0AtEC0.find(var.getImpl()) != XY0AtEC0.end())
		{
			for (auto& j : XY0AtEC0.at(var.getImpl()))
				expr.addLinTerm(mu[j], EC0[j].getExpr().getCoeff(var));
			flag = true;
		}
		if (flag)
		{
			result.dualCon.add(OpLinCon(env, objCoeff, expr, objCoeff));
		}
		else
		{
			throw OpExcBase("[OPUA::Algorithm::OpAlgoAutoDual]: Exception-> variable(Y0) "
				+ var.getName()
				+ " does not belong to any constraint");
		}
	}
	result.dualVar.add(lbd);
	result.dualVar.add(mu);
	// 创建对偶目标函数
	for (OpULInt i = 0; i < NC1; i++)
		result.dualObj.addLinTerm(lbd[i], NEC0[i].getLb());
	for (OpULInt i = 0; i < NC2; i++)
		result.dualObj.addLinTerm(mu[i], EC0[i].getLb());
	// 释放临时对象
	lbd.release();
	mu.release();
}

Algorithm::OpAutoDualSet Algorithm::OpAlgoAutoDual(OpEnv env, Variable::OpVarArr X0, Variable::OpVarArr Y0, Constraint::OpLCArr NEC0, Constraint::OpLCArr EC0, const Expression::OpLinExpr& O0)
{
	OpAutoDualSet result(env);
	OpAlgoAutoDual(env, result, X0, Y0, NEC0, EC0, O0);
	return result;
}


