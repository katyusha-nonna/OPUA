#include "OPUA.Solver.h"
#ifdef OPUA_COMPILE_CPX
#include <ilcplex/ilocplex.h>
#endif

using namespace OPUA;

#ifdef OPUA_COMPILE_CPX
/* OPUA::Solver::OpCPXCfgCvt */

// CPX���÷�����
class Solver::OpCPXCfgCvt
{
protected:
	std::unordered_map<OpStr, IloCplex::NumParam> npdict_;
	std::unordered_map<OpStr, IloCplex::BoolParam> bpdict_;
	std::unordered_map<OpStr, IloCplex::IntParam> ipdict_;
	std::unordered_map<OpStr, IloCplex::LongParam> lpdict_;
	std::unordered_map<OpStr, IloCplex::StringParam> spdict_;

public:
	void init();
	IloCplex::NumParam getNumParam(OpStr key);
	IloCplex::BoolParam getBoolParam(OpStr key);
	IloCplex::IntParam getIntParam(OpStr key);
	IloCplex::LongParam getLongParam(OpStr key);
	IloCplex::StringParam getStringParam(OpStr key);
	
public:
	OpCPXCfgCvt();
};

void Solver::OpCPXCfgCvt::init()
{
#ifdef OPUA_CPX_VERSION_128
	// CPX�汾��12.8
	// ����Barrier����
	ipdict_.emplace("OPUA.CPX.Advance", IloCplex::Param::Advance);
	ipdict_.emplace("OPUA.CPX.Barrier.Algorithm", IloCplex::Param::Barrier::Algorithm);
	ipdict_.emplace("OPUA.CPX.Barrier.ColNonzeros", IloCplex::Param::Barrier::ColNonzeros);
	npdict_.emplace("OPUA.CPX.Barrier.ConvergeTol", IloCplex::Param::Barrier::ConvergeTol);
	ipdict_.emplace("OPUA.CPX.Barrier.Crossover", IloCplex::Param::Barrier::Crossover);
	ipdict_.emplace("OPUA.CPX.Barrier.Display", IloCplex::Param::Barrier::Display);
	lpdict_.emplace("OPUA.CPX.Barrier.Limits.Corrections", IloCplex::Param::Barrier::Limits::Corrections);
	npdict_.emplace("OPUA.CPX.Barrier.Limits.Growth", IloCplex::Param::Barrier::Limits::Growth);
	lpdict_.emplace("OPUA.CPX.Barrier.Limits.Iteration", IloCplex::Param::Barrier::Limits::Iteration);
	npdict_.emplace("OPUA.CPX.Barrier.Limits.ObjRange", IloCplex::Param::Barrier::Limits::ObjRange);
	ipdict_.emplace("OPUA.CPX.Barrier.Ordering", IloCplex::Param::Barrier::Ordering);
	npdict_.emplace("OPUA.CPX.Barrier.QCPConvergeTol", IloCplex::Param::Barrier::QCPConvergeTol);
	ipdict_.emplace("OPUA.CPX.Barrier.StartAlg", IloCplex::Param::Barrier::StartAlg);
	ipdict_.emplace("OPUA.CPX.Simplex.Crash", IloCplex::Param::Simplex::Crash);
	ipdict_.emplace("OPUA.CPX.Simplex.Dgradient", IloCplex::Param::Simplex::DGradient);
	ipdict_.emplace("OPUA.CPX.Simplex.Display", IloCplex::Param::Simplex::Display);
	lpdict_.emplace("OPUA.CPX.Simplex.Limits.Iterations", IloCplex::Param::Simplex::Limits::Iterations);

	bpdict_.emplace("OPUA.CPX.Preprocessing.Presolve", IloCplex::Param::Preprocessing::Presolve);
#endif
}

IloCplex::NumParam Solver::OpCPXCfgCvt::getNumParam(OpStr key)
{
	return npdict_.at(key);
}

IloCplex::BoolParam Solver::OpCPXCfgCvt::getBoolParam(OpStr key)
{
	return bpdict_.at(key);
}

IloCplex::IntParam Solver::OpCPXCfgCvt::getIntParam(OpStr key)
{
	return ipdict_.at(key);
}

IloCplex::LongParam Solver::OpCPXCfgCvt::getLongParam(OpStr key)
{
	return lpdict_.at(key);
}

IloCplex::StringParam Solver::OpCPXCfgCvt::getStringParam(OpStr key)
{
	return spdict_.at(key);
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
	void solveFixed(); // �̶����������Ⲣִ�����
	void setParam(const OpConfig& cfg); // ��������
	OpLInt getStatus() const; // ��ȡ���״̬
	OpFloat getObjValue() const; // ��ȡĿ�꺯����
	OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
	OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
	OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
	OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
	OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
	void write(OpStr path) const; // ��ģ��д���ļ�
	virtual OpULInt getMemoryUsage() const; // ��ȡ�ڴ�ռ��
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
	if (csol_.getImpl())
		csol_.end();
	if (cmdl_.getImpl())
		cmdl_.end();
	if (cenv_.getImpl())
		cenv_.end();
}

void Solver::OpCPXSolI::extract(Model::OpModel mdl)
{
	// �������ԭģ��
	clear();
	init();
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

void Solver::OpCPXSolI::solveFixed()
{
	csol_.solveFixed();
}

void Solver::OpCPXSolI::setParam(const OpConfig& cfg)
{
	if (csol_.getImpl())
	{
		for (auto iter = cfg.getCBegin<OpBool>("OPUA.CPX"); iter != cfg.getCEnd<OpBool>("OPUA.CPX"); ++iter)
			if (iter.ok())
				csol_.setParam(cfgcvt_.getBoolParam(iter.getKey()), iter.getVal());
		for (auto iter = cfg.getCBegin<OpInt>("OPUA.CPX"); iter != cfg.getCEnd<OpInt>("OPUA.CPX"); ++iter)
			if (iter.ok())
				csol_.setParam(cfgcvt_.getIntParam(iter.getKey()), iter.getVal());
		for (auto iter = cfg.getCBegin<OpLInt>("OPUA.CPX"); iter != cfg.getCEnd<OpLInt>("OPUA.CPX"); ++iter)
			if (iter.ok())
				csol_.setParam(cfgcvt_.getLongParam(iter.getKey()), iter.getVal());
		for (auto iter = cfg.getCBegin<OpFloat>("OPUA.CPX"); iter != cfg.getCEnd<OpFloat>("OPUA.CPX"); ++iter)
			if (iter.ok())
				csol_.setParam(cfgcvt_.getNumParam(iter.getKey()), iter.getVal());
		for (auto iter = cfg.getCBegin<OpStr>("OPUA.CPX"); iter != cfg.getCEnd<OpStr>("OPUA.CPX"); ++iter)
			if (iter.ok())
				csol_.setParam(cfgcvt_.getStringParam(iter.getKey()), iter.getVal().c_str());
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

void Solver::OpCPXSolI::write(OpStr path) const
{
	if (path.size())
	{
		csol_.exportModel(path.c_str());
	}
	else
	{	
		// �޷����������̨
	}
}

OpULInt Solver::OpCPXSolI::getMemoryUsage() const
{
	return sizeof(*this);
}

Solver::OpCPXSolI::OpCPXSolI(OpEnvI* env)
	: OpImplBase('S', env),
	cenv_(nullptr),
	cmdl_(nullptr), 
	csol_(nullptr)
{

}

Solver::OpCPXSolI::OpCPXSolI(OpEnvI* env, Model::OpModel mdl)
	: OpImplBase('S', env),
	cenv_(nullptr),
	cmdl_(nullptr),
	csol_(nullptr)
{
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

void Solver::OpCPXSol::solveFixed()
{
	static_cast<OpCPXSolI*>(impl_)->solveFixed();
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

void Solver::OpCPXSol::write(OpStr path) const
{
	static_cast<OpCPXSolI*>(impl_)->write(path);
}

void Solver::OpCPXSol::release0()
{
	static_cast<OpCPXSolI*>(impl_)->release();
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
#endif
