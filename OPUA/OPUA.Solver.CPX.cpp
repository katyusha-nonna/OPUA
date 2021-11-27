#include "OPUA.Solver.h"
#include <ilcplex/ilocplex.h>
using namespace OPUA;

/* OPUA::Solver::OpCPXCfgCvt */

// CPX配置翻译器
class Solver::OpCPXCfgCvt
{
protected:
	
public:
	void init();
	
public:
	OpCPXCfgCvt();
};

void Solver::OpCPXCfgCvt::init()
{

}

Solver::OpCPXCfgCvt::OpCPXCfgCvt()
{
	init();
}

/* OPUA::Solver::OpCPXSolI */

class Solver::OpCPXSolI
	: public OpImplBase
{
protected:
	// CPX组件模型

	IloEnv cenv_; // CPX环境变量
	IloModel cmdl_; // CPX模型对象
	IloCplex csol_; // CPX求解器对象
protected:
	// OPUA-CPX映射信息

	std::unordered_map<OpLInt, IloNumVar> vardict_; // CPX变量表
	std::unordered_map<OpLInt, IloRange> lcdict_; // CPX线性约束表
	std::unordered_map<OpLInt, IloRange> qcdict_; // CPX二次约束表
	std::unordered_map<OpLInt, IloConstraint> scdict_; // CPXSOS约束表
	std::unordered_map<OpLInt, IloConstraint> nlcdict_; // CPX非线性约束表
	std::unordered_map<OpLInt, IloObjective> objdict_; // CPX优化目标函数表
	OpCPXCfgCvt cfgcvt_; // 配置翻译器

	friend class OpCPXSol;
private:
	IloNumVar::Type typeConvert(Variable::OpVarType type); // 将OPUA变量类型转换为CPX变量类型
	IloNumVar addCPXVar(Variable::OpVar var); // 从OPUA变量创建一个CPX变量
	IloNumExpr addCPXLE(const Expression::OpLinExpr& expr); // 由OPUA线性表达式创建一个CPX线性表达式
	IloNumExpr addCPXQE(const Expression::OpQuadExpr& expr); // 由OPUA二次表达式创建一个CPX二次表达式
	IloRange addCPXLC(Constraint::OpLinCon con); // 从OPUA线性约束创建一个CPX约束
	IloRange addCPXQC(Constraint::OpQuadCon con); // 从OPUA二次约束创建一个CPX约束
	IloConstraint addCPXSOS(Constraint::OpSOSCon con); // 从OPUASOS约束创建一个CPX约束
	IloConstraint addCPXNLC(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXSum(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXAbs(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXMax(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXMin(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXSquare(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXSqrt(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXPower(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXExp(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXLog(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXIfThen(Constraint::OpCdtCon con); // 从OPUA条件约束中创建一个CPX约束
	IloObjective addCPXObj(Objective::OpObj obj); // 从OPUA目标函数创建一个CPX目标函数
protected:
	void init(); // 初始化
	void clear(); // 清除所有组件与映射信息
	void extract(Model::OpModel mdl); // 抽取OPUA模型
	void solve(); // 求解模型
	void setParam(const OpConfig& cfg); // 设置配置
	OpLInt getStatus() const; // 获取求解状态
	OpFloat getObjValue() const; // 获取目标函数解
	OpFloat getValue(Variable::OpVar var) const; // 获取变量的解
	OpFloat getValue(const Expression::OpLinExpr& expr) const; // 获取表达式的解(速度较慢)
	OpFloat getValue(const Expression::OpQuadExpr& expr) const; // 获取表达式的解(速度较慢)
	OpFloat getValue(Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
	OpFloat getDual(Constraint::OpLinCon con) const; // 获取对偶解
protected:
	OpCPXSolI(OpEnvI* env);
	OpCPXSolI(OpEnvI* env, Model::OpModel mdl);
public:
	virtual ~OpCPXSolI();
};

IloNumVar::Type Solver::OpCPXSolI::typeConvert(Variable::OpVarType type)
{
	auto tmp(IloNumVar::Type::Float);
	switch (type)
	{
	case Variable::OpVarType::Bool:
		tmp = IloNumVar::Type::Bool;
		break;
	case Variable::OpVarType::Int:
		tmp = IloNumVar::Type::Int;
		break;
	case Variable::OpVarType::Con:
		tmp = IloNumVar::Type::Float;
		break;
	default:
		break;
	}
	return tmp;
}

IloNumVar Solver::OpCPXSolI::addCPXVar(Variable::OpVar var)
{
	IloNum lb(Constant::IsInfinity(var.getLb()) ? -IloInfinity : var.getLb());
	IloNum ub(Constant::IsInfinity(var.getUb()) ? IloInfinity : var.getUb());
	return IloNumVar(cenv_, lb, ub, typeConvert(var.getType()), var.getName().c_str());
}

IloNumExpr Solver::OpCPXSolI::addCPXLE(const Expression::OpLinExpr& expr)
{
	IloNumExpr tmp(cenv_, expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		tmp += iter.getCoeff() * vardict_.at(iter.getVar().getIndex());
	return tmp;
}

IloNumExpr Solver::OpCPXSolI::addCPXQE(const Expression::OpQuadExpr& expr)
{
	IloNumExpr tmp(cenv_, expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		tmp += iter.getCoeff() * vardict_.at(iter.getVar().getIndex());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		tmp += iter.getCoeff() * vardict_.at(iter.getVar1().getIndex()) * vardict_.at(iter.getVar2().getIndex());
	return tmp;
}

IloRange Solver::OpCPXSolI::addCPXLC(Constraint::OpLinCon con)
{
	IloRange tmp(nullptr);
	IloNum lb(Constant::IsInfinity(con.getLb()) ? -IloInfinity : con.getLb());
	IloNum ub(Constant::IsInfinity(con.getUb()) ? IloInfinity : con.getUb());
	IloNumExpr expr(addCPXLE(con.getExpr()));
	tmp = IloRange(cenv_, lb, expr, ub, con.getName().c_str());
	expr.end();
	return tmp;
}

IloRange Solver::OpCPXSolI::addCPXQC(Constraint::OpQuadCon con)
{
	IloRange tmp(nullptr);
	IloNum lb(Constant::IsInfinity(con.getLb()) ? -IloInfinity : con.getLb());
	IloNum ub(Constant::IsInfinity(con.getUb()) ? IloInfinity : con.getUb());
	IloNumExpr expr(addCPXQE(con.getExpr()));
	tmp = IloRange(cenv_, lb, expr, ub, con.getName().c_str());
	expr.end();
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXSOS(Constraint::OpSOSCon con)
{
	IloConstraint tmp(nullptr);
	auto& expr(con.getSOSExpr());
	IloNumVarArray vars(cenv_);
	IloNumArray weights(cenv_);
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
	{
		weights.add(iter.getCoeff());
		vars.add(vardict_.at(iter.getVar().getIndex()));
	}
	switch (con.getSense())
	{
	case Constraint::OpConSense::SOS1:
		tmp = IloSOS1(cenv_, vars, weights, con.getName().c_str());
		break;
	case Constraint::OpConSense::SOS2:
		tmp = IloSOS2(cenv_, vars, weights, con.getName().c_str());
		break;
	default:
		break;
	}
	vars.end();
	weights.end();
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXNLC(Constraint::OpNLCon con)
{
	IloConstraint tmp(nullptr);
	switch (con.getFunction())
	{
	case Expression::OpNLFunc::Unknow: // 无法处理
		break;
	case Expression::OpNLFunc::Sum: // 无法处理
		tmp = addCPXSum(con);
		break;
	case Expression::OpNLFunc::Abs:
		tmp = addCPXAbs(con);
		break;
	case Expression::OpNLFunc::Max:
		tmp = addCPXMax(con);
		break;
	case Expression::OpNLFunc::Min:
		tmp = addCPXMin(con);
		break;
	case Expression::OpNLFunc::Square:
		tmp = addCPXSquare(con);
		break;
	case Expression::OpNLFunc::Sqrt:
		tmp = addCPXSqrt(con);
		break;
	case Expression::OpNLFunc::Pow:
		tmp = addCPXPower(con);
		break;
	case Expression::OpNLFunc::Exp1:
		tmp = addCPXExp(con);
		break;
	case Expression::OpNLFunc::Exp2: // 无法处理
		break;
	case Expression::OpNLFunc::Log1: // 无法处理
		break;
	case Expression::OpNLFunc::Log2:
		tmp = addCPXLog(con);
		break;
	case Expression::OpNLFunc::Log3: // 无法处理
		break;
	case Expression::OpNLFunc::Sin: // 无法处理
		break;
	case Expression::OpNLFunc::Cos: // 无法处理
		break;
	case Expression::OpNLFunc::Tan: // 无法处理
		break;
	default:
		break;
	}
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXSum(Constraint::OpNLCon con)
{
	IloConstraint tmp(nullptr);
	IloNumExpr rhs(cenv_);
	auto& expr(con.getExpr());
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
		rhs += vardict_.at(iter.getVar().getIndex());
	rhs += expr.getParam();
	tmp = vardict_.at(con.getVar().getIndex()) == rhs;
	tmp.setName(con.getName().c_str());
	rhs.end();
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXAbs(Constraint::OpNLCon con)
{
	IloConstraint tmp(vardict_.at(con.getVar().getIndex()) == IloAbs(vardict_.at(con.getExpr().getVar(0).getIndex())));
	tmp.setName(con.getName().c_str());
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXMax(Constraint::OpNLCon con)
{
	IloConstraint tmp(nullptr);
	IloNumExprArray vars(cenv_);
	auto& expr(con.getExpr());
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
		vars.add(vardict_.at(iter.getVar().getIndex()));
	vars.add(IloNumExpr(cenv_, expr.getParam()));
	tmp = vardict_.at(con.getVar().getIndex()) == IloMax(vars);
	tmp.setName(con.getName().c_str());
	vars.endElements();
	vars.end();
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXMin(Constraint::OpNLCon con)
{
	IloConstraint tmp(nullptr);
	IloNumExprArray vars(cenv_);
	auto& expr(con.getExpr());
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
		vars.add(vardict_.at(iter.getVar().getIndex()));
	vars.add(IloNumExpr(cenv_, expr.getParam()));
	tmp = vardict_.at(con.getVar().getIndex()) == IloMin(vars);
	tmp.setName(con.getName().c_str());
	vars.endElements();
	vars.end();
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXSquare(Constraint::OpNLCon con)
{
	IloConstraint tmp(vardict_.at(con.getVar().getIndex()) == IloSquare(vardict_.at(con.getExpr().getVar(0).getIndex())));
	tmp.setName(con.getName().c_str());
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXSqrt(Constraint::OpNLCon con)
{
	IloConstraint tmp(vardict_.at(con.getVar().getIndex()) == IloPower(vardict_.at(con.getExpr().getVar(0).getIndex()), 0.5));
	tmp.setName(con.getName().c_str());
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXPower(Constraint::OpNLCon con)
{
	auto& expr(con.getExpr());
	IloConstraint tmp(vardict_.at(con.getVar().getIndex()) == IloPower(vardict_.at(expr.getVar(0).getIndex()), expr.getParam()));
	tmp.setName(con.getName().c_str());
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXExp(Constraint::OpNLCon con)
{
	IloConstraint tmp(vardict_.at(con.getVar().getIndex()) == IloExponent(vardict_.at(con.getExpr().getVar(0).getIndex())));
	tmp.setName(con.getName().c_str());
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXLog(Constraint::OpNLCon con)
{
	IloConstraint tmp(vardict_.at(con.getVar().getIndex()) == IloLog(vardict_.at(con.getExpr().getVar(0).getIndex())));
	tmp.setName(con.getName().c_str());
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXIfThen(Constraint::OpCdtCon con)
{
	IloIfThen tmp(nullptr);
	if (con.isIndicator())
		tmp = IloIfThen(cenv_, vardict_.at(con.getVar().getIndex()) == 1, addCPXLC(con.getCon(false)), con.getName().c_str());
	else
		tmp = IloIfThen(cenv_, addCPXLC(con.getCon(true)), addCPXLC(con.getCon(false)), con.getName().c_str());
	return tmp;
}

IloObjective Solver::OpCPXSolI::addCPXObj(Objective::OpObj obj)
{
	IloObjective tmp(nullptr);
	IloNumExpr lins(addCPXLE(obj.getLinExpr()));
	IloNumExpr quads(addCPXQE(obj.getQuadExpr()));
	switch (obj.getSense())
	{
	case Objective::OpObjSense::Min:
		tmp = IloMinimize(cenv_, lins + quads);
		break;
	case Objective::OpObjSense::Max:
		tmp = IloMaximize(cenv_, lins + quads);
		break;
	default:
		break;
	}
	tmp.setName(obj.getName().c_str());
	lins.end();
	quads.end();
	return tmp;
}

void Solver::OpCPXSolI::init()
{
	cenv_ = IloEnv();
	cmdl_ = IloModel(cenv_);
}

void Solver::OpCPXSolI::clear()
{
	vardict_.clear();
	lcdict_.clear();
	qcdict_.clear();
	scdict_.clear();
	nlcdict_.clear();
	objdict_.clear();
	csol_.end();
	cmdl_.end();
	cenv_.end();
}

void Solver::OpCPXSolI::extract(Model::OpModel mdl)
{
	// 首先清除原模型
	if (cenv_.getImpl())
	{
		clear();
		init();
	}
	// 加载现有模型
	for (auto iter = mdl.getCBegin<Variable::OpVar>(); iter != mdl.getCEnd<Variable::OpVar>(); ++iter)
		vardict_.emplace(iter.getKey(), addCPXVar(iter.getVal()));
	for (auto iter = mdl.getCBegin<Constraint::OpLinCon>(); iter != mdl.getCEnd<Constraint::OpLinCon>(); ++iter)
		lcdict_.emplace(iter.getKey(), addCPXLC(iter.getVal()));
	for (auto iter = mdl.getCBegin<Constraint::OpQuadCon>(); iter != mdl.getCEnd<Constraint::OpQuadCon>(); ++iter)
		qcdict_.emplace(iter.getKey(), addCPXQC(iter.getVal()));
	for (auto iter = mdl.getCBegin<Constraint::OpSOSCon>(); iter != mdl.getCEnd<Constraint::OpSOSCon>(); ++iter)
		scdict_.emplace(iter.getKey(), addCPXSOS(iter.getVal()));
	for (auto iter = mdl.getCBegin<Constraint::OpNLCon>(); iter != mdl.getCEnd<Constraint::OpNLCon>(); ++iter)
		nlcdict_.emplace(iter.getKey(), addCPXNLC(iter.getVal()));
	for (auto iter = mdl.getCBegin<Constraint::OpCdtCon>(); iter != mdl.getCEnd<Constraint::OpCdtCon>(); ++iter)
		nlcdict_.emplace(iter.getKey(), addCPXIfThen(iter.getVal()));
	objdict_.emplace(mdl.getObj().getIndex(), addCPXObj(mdl.getObj()));
	
	for (auto& lc : lcdict_)
		cmdl_.add(lc.second);
	for (auto& qc : qcdict_)
		cmdl_.add(qc.second);
	for (auto& sc : scdict_)
		cmdl_.add(sc.second);
	for (auto& nlc : nlcdict_)
		cmdl_.add(nlc.second);
	for (auto& o : objdict_)
		cmdl_.add(o.second);
}

void Solver::OpCPXSolI::solve()
{
	if (!csol_.getImpl())
		csol_ = IloCplex(cmdl_);
	csol_.solve();
}

void Solver::OpCPXSolI::setParam(const OpConfig& cfg)
{
	if (csol_.getImpl())
	{

	}
}

OpLInt Solver::OpCPXSolI::getStatus() const
{
	return csol_.getStatus();
}

OpFloat Solver::OpCPXSolI::getObjValue() const
{
	return csol_.getObjValue();
}

OpFloat Solver::OpCPXSolI::getValue(Variable::OpVar var) const
{
	return csol_.getValue(vardict_.at(var.getIndex()));
}

OpFloat Solver::OpCPXSolI::getValue(const Expression::OpLinExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	return result;
}

OpFloat Solver::OpCPXSolI::getValue(const Expression::OpQuadExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar1()) * getValue(iter.getVar2());
	return result;
}

OpFloat Solver::OpCPXSolI::getValue(Objective::OpObj obj) const
{
	return getValue(obj.getLinExpr()) + getValue(obj.getQuadExpr());
}

OpFloat Solver::OpCPXSolI::getDual(Constraint::OpLinCon con) const
{
	return csol_.getDual(lcdict_.at(con.getIndex()));
}

Solver::OpCPXSolI::OpCPXSolI(OpEnvI* env)
	: OpImplBase('S', env),
	cenv_(nullptr),
	cmdl_(nullptr), 
	csol_(nullptr)
{
	init();
}

Solver::OpCPXSolI::OpCPXSolI(OpEnvI* env, Model::OpModel mdl)
	: OpImplBase('S', env),
	cenv_(nullptr),
	cmdl_(nullptr),
	csol_(nullptr)
{
	init();
	extract(mdl);
}

Solver::OpCPXSolI::~OpCPXSolI()
{
	clear();
}

/* OPUA::Solver::OpCPXSol */

void Solver::OpCPXSol::extract(Model::OpModel mdl)
{
	static_cast<OpCPXSolI*>(impl_)->extract(mdl);
}

void Solver::OpCPXSol::solve()
{
	static_cast<OpCPXSolI*>(impl_)->solve();
}

void Solver::OpCPXSol::setParam(const OpConfig& cfg)
{
	static_cast<OpCPXSolI*>(impl_)->setParam(cfg);
}

OpLInt Solver::OpCPXSol::getStatus() const
{
	return static_cast<OpCPXSolI*>(impl_)->getStatus();
}

OpFloat Solver::OpCPXSol::getObjValue() const
{
	return static_cast<OpCPXSolI*>(impl_)->getObjValue();
}

OpFloat Solver::OpCPXSol::getValue(Variable::OpVar var) const
{
	return static_cast<OpCPXSolI*>(impl_)->getValue(var);
}

OpFloat Solver::OpCPXSol::getValue(const Expression::OpLinExpr& expr) const
{
	return static_cast<OpCPXSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpCPXSol::getValue(const Expression::OpQuadExpr& expr) const
{
	return static_cast<OpCPXSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpCPXSol::getValue(Objective::OpObj obj) const
{
	return static_cast<OpCPXSolI*>(impl_)->getValue(obj);
}

OpFloat Solver::OpCPXSol::getDual(Constraint::OpLinCon con) const
{
	return static_cast<OpCPXSolI*>(impl_)->getDual(con);
}

Solver::OpCPXSolI* Solver::OpCPXSol::getImpl() const
{
	return static_cast<OpCPXSolI*>(impl_);
}

OpBool Solver::OpCPXSol::operator==(const OpCPXSol& sol) const
{
	return impl_ == sol.impl_;
}

OpBool Solver::OpCPXSol::operator!=(const OpCPXSol& sol) const
{
	return impl_ != sol.impl_;
}

Solver::OpCPXSol::OpCPXSol()
{

}

Solver::OpCPXSol::OpCPXSol(OpCPXSolI* impl)
{
	impl_ = impl;
}

Solver::OpCPXSol::OpCPXSol(OpEnv env)
{
	impl_ = new OpCPXSolI(env.getImpl());
}

Solver::OpCPXSol::OpCPXSol(OpEnv env, Model::OpModel mdl)
{
	impl_ = new OpCPXSolI(env.getImpl(), mdl);
}

Solver::OpCPXSol::~OpCPXSol()
{

}
