#include "OPUA.Solver.h"
#include <ilcplex/ilocplex.h>
using namespace OPUA;

/* OPUA::Solver::OpCPXCfgCvt */

// CPX���÷�����
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
	// CPX���ģ��

	IloEnv cenv_; // CPX��������
	IloModel cmdl_; // CPXģ�Ͷ���
	IloCplex csol_; // CPX���������
protected:
	// OPUA-CPXӳ����Ϣ

	std::unordered_map<OpLInt, IloNumVar> vardict_; // CPX������
	std::unordered_map<OpLInt, IloRange> lcdict_; // CPX����Լ����
	std::unordered_map<OpLInt, IloRange> qcdict_; // CPX����Լ����
	std::unordered_map<OpLInt, IloConstraint> scdict_; // CPXSOSԼ����
	std::unordered_map<OpLInt, IloConstraint> nlcdict_; // CPX������Լ����
	std::unordered_map<OpLInt, IloObjective> objdict_; // CPX�Ż�Ŀ�꺯����
	OpCPXCfgCvt cfgcvt_; // ���÷�����

	friend class OpCPXSol;
private:
	IloNumVar::Type typeConvert(Variable::OpVarType type); // ��OPUA��������ת��ΪCPX��������
	IloNumVar addCPXVar(Variable::OpVar var); // ��OPUA��������һ��CPX����
	IloNumExpr addCPXLE(const Expression::OpLinExpr& expr); // ��OPUA���Ա��ʽ����һ��CPX���Ա��ʽ
	IloNumExpr addCPXQE(const Expression::OpQuadExpr& expr); // ��OPUA���α��ʽ����һ��CPX���α��ʽ
	IloRange addCPXLC(Constraint::OpLinCon con); // ��OPUA����Լ������һ��CPXԼ��
	IloRange addCPXQC(Constraint::OpQuadCon con); // ��OPUA����Լ������һ��CPXԼ��
	IloConstraint addCPXSOS(Constraint::OpSOSCon con); // ��OPUASOSԼ������һ��CPXԼ��
	IloConstraint addCPXNLC(Constraint::OpNLCon con); // ��OPUA������Լ������һ��CPXԼ��
	IloConstraint addCPXSum(Constraint::OpNLCon con); // ��OPUA������Լ������һ��CPXԼ��
	IloConstraint addCPXAbs(Constraint::OpNLCon con); // ��OPUA������Լ������һ��CPXԼ��
	IloConstraint addCPXMax(Constraint::OpNLCon con); // ��OPUA������Լ������һ��CPXԼ��
	IloConstraint addCPXMin(Constraint::OpNLCon con); // ��OPUA������Լ������һ��CPXԼ��
	IloConstraint addCPXSquare(Constraint::OpNLCon con); // ��OPUA������Լ������һ��CPXԼ��
	IloConstraint addCPXSqrt(Constraint::OpNLCon con); // ��OPUA������Լ������һ��CPXԼ��
	IloConstraint addCPXPower(Constraint::OpNLCon con); // ��OPUA������Լ������һ��CPXԼ��
	IloConstraint addCPXExp(Constraint::OpNLCon con); // ��OPUA������Լ������һ��CPXԼ��
	IloConstraint addCPXLog(Constraint::OpNLCon con); // ��OPUA������Լ������һ��CPXԼ��
	IloConstraint addCPXIfThen(Constraint::OpCdtCon con); // ��OPUA����Լ���д���һ��CPXԼ��
	IloObjective addCPXObj(Objective::OpObj obj); // ��OPUAĿ�꺯������һ��CPXĿ�꺯��
protected:
	void init(); // ��ʼ��
	void clear(); // ������������ӳ����Ϣ
	void extract(Model::OpModel mdl); // ��ȡOPUAģ��
	void solve(); // ���ģ��
	void setParam(const OpConfig& cfg); // ��������
	OpLInt getStatus() const; // ��ȡ���״̬
	OpFloat getObjValue() const; // ��ȡĿ�꺯����
	OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
	OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
	OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
	OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
	OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
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
	case Expression::OpNLFunc::Unknow: // �޷�����
		break;
	case Expression::OpNLFunc::Sum: // �޷�����
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
	case Expression::OpNLFunc::Exp2: // �޷�����
		break;
	case Expression::OpNLFunc::Log1: // �޷�����
		break;
	case Expression::OpNLFunc::Log2:
		tmp = addCPXLog(con);
		break;
	case Expression::OpNLFunc::Log3: // �޷�����
		break;
	case Expression::OpNLFunc::Sin: // �޷�����
		break;
	case Expression::OpNLFunc::Cos: // �޷�����
		break;
	case Expression::OpNLFunc::Tan: // �޷�����
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
	// �������ԭģ��
	if (cenv_.getImpl())
	{
		clear();
		init();
	}
	// ��������ģ��
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
