#ifdef OPUA_COMPILE_SCIP
#include "OPUA.Solver.h"
#include "OPUA.Exception.h"
#include <objscip/objscip.h>
#include <objscip/objscipdefplugins.h>
#include <iostream>

using namespace OPUA;
#endif

#ifdef OPUA_COMPILE_SCIP
/* OPUA::Solver::OpSCIPCfgCvt */

// SCIP���÷�����
class Solver::OpSCIPCfgCvt
{
protected:

public:
	void init();

public:
	OpSCIPCfgCvt();
};

void Solver::OpSCIPCfgCvt::init()
{

}

Solver::OpSCIPCfgCvt::OpSCIPCfgCvt()
{
	init();
}

/* OPUA::Solver::OpSCIPSolI */

class Solver::OpSCIPSolI
	: public OpImplBase
{
protected:
	// SCIP���

	bool modified_;
	SCIP_OBJSENSE sense_; // SCIPĿ�꺯������
	SCIP* scip_; // SCIPģ��/����
	SCIP_Sol* sol_; // SCIP��
	std::vector<SCIP_VAR*> vars_; // SCIP������
	std::vector<SCIP_CONS*> cons_; // SCIPԼ����
protected:
	// OPUA-SCIPӳ�����

	std::unordered_map<Variable::OpVarI*, size_t> varidxs_; // ������ű�
	std::unordered_map<Constraint::OpLinConI*, size_t> lcidxs_; // ����Լ����ű�
	std::unordered_map<Constraint::OpQuadConI*, size_t> qcidxs_; // ����Լ����ű�
	std::unordered_map<Constraint::OpNLConI*, size_t> nlcidxs_; // ������Լ����ű�
	Expression::OpLinExpr obj0_; // ��ǰĿ�꺯��

	friend class OpSCIPSol;
private:
	void initSCIP(); // ��ʼ��SCIP����
	void clearSCIP(); // ����SCIP����
	void clearMaps(); // ����ӳ���
	void update(); // ����SCIPģ��
	SCIP_VARTYPE senseConvert1(Variable::OpVarType sense); // OPUA->SCIP��������ת��
	SCIP_OBJSENSE senseConvert2(Objective::OpObjSense sense); // OPUA->SCIP�Ż�Ŀ������ת��
	SCIP_EXPR* createNewExprByOp(OpFloat val); // ���һ��SCIP�������ʽ
	SCIP_EXPR* createNewExprByOp(Variable::OpVar var); // ���һ��SCIP���������ʽ
	SCIP_EXPR* createNewExprByOp(const Expression::OpNLExpr& expr, SCIP_EXPROP op, bool param); // ���һ��SCIP��Ŀ���ʽ
	SCIP_EXPR* createNewExprByOp2(const Expression::OpNLExpr& expr, SCIP_EXPROP op, bool param); // ���һ��SCIP˫Ŀ���ʽ
	SCIP_EXPR* createNewExprByOpN(const Expression::OpNLExpr& expr, SCIP_EXPROP op, int nvars, bool param); // ���һ��SCIP��Ŀ���ʽ
	SCIP_EXPR* createNewExprOfNLE(const Expression::OpNLExpr& expr); // ����һ��OPUA�����Ա��ʽ���һ��SCIP���ʽ
protected:
	void init(); // ��ʼ��(�����ڹ��캯��)
	void clear(); // ������������ӳ����Ϣ(��������������)
	void extract(Model::OpModel mdl); // ��ȡOPUAģ��
	void solve(); // ���ģ��
	OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
	OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���Ա��ʽ�Ľ�
	OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���α��ʽ�Ľ�
	OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
	OpFloat getObjValue(); // ��ȡĿ�꺯���Ľ�
	SCIP_STATUS getStatus(); // ��ȡ���״̬
	void write(OpStr path); // 
	virtual OpULInt getMemoryUsage() const; // ��ȡ�ڴ�ռ��
protected:
	OpSCIPSolI(OpEnvI* env);
	OpSCIPSolI(OpEnvI* env, Model::OpModel mdl);
public:
	virtual ~OpSCIPSolI();
};

void Solver::OpSCIPSolI::initSCIP()
{
	SCIPcreate(&scip_);
	SCIPprintVersion(scip_, nullptr);
	SCIPsetEmphasis(scip_, SCIP_PARAMEMPHASIS_OPTIMALITY, FALSE);
	SCIPincludeDefaultPlugins(scip_);
	SCIPcreateProbBasic(scip_, "MIP");
}

void Solver::OpSCIPSolI::clearSCIP()
{
	for (auto& v : varidxs_)
		if (v.second != SIZE_MAX)
			SCIPreleaseVar(scip_, &(vars_[v.second]));
	vars_ = std::vector<SCIP_VAR*>();
	for (auto& lc : lcidxs_)
		if (lc.second != SIZE_MAX)
			SCIPreleaseCons(scip_, &(cons_[lc.second]));
	for (auto& qc : qcidxs_)
		if (qc.second != SIZE_MAX)
			SCIPreleaseCons(scip_, &(cons_[qc.second]));
	for (auto& nlc : nlcidxs_)
		if (nlc.second != SIZE_MAX)
			SCIPreleaseCons(scip_, &(cons_[nlc.second]));
	cons_ = std::vector<SCIP_CONS*>();
	sol_ = nullptr;
	if (scip_)
	{
		SCIPfree(&scip_);
		scip_ = nullptr;
	}
}

void Solver::OpSCIPSolI::clearMaps()
{
	varidxs_.clear();
	lcidxs_.clear();
	qcidxs_.clear();
	nlcidxs_.clear();
	obj0_.clear();
	modified_ = true;
}

void Solver::OpSCIPSolI::update()
{
	if (modified_)
	{
		initSCIP();
		size_t count(0);
		vars_.resize(varidxs_.size(), nullptr);
		for (auto& v : varidxs_)
		{
			Variable::OpVar var(v.first);
			double lb(Constant::IsInfinity(var.getLb()) ? -SCIPinfinity(scip_) : var.getLb());
			double ub(Constant::IsInfinity(var.getUb()) ? SCIPinfinity(scip_) : var.getUb());
			SCIPcreateVar(scip_, &(vars_[count]), var.getName().c_str(), lb, ub, 0.0, senseConvert1(var.getType()),
				TRUE, FALSE, NULL, NULL, NULL, NULL, NULL);
			SCIPaddVar(scip_, vars_[count]);
			v.second = count;
			count++;
		}
		count = 0;
		cons_.resize(lcidxs_.size() + qcidxs_.size() + nlcidxs_.size(), nullptr);
		// ��������Լ��
		for (auto& lc : lcidxs_)
		{
			auto& tmp(cons_[count]);
			Constraint::OpLinCon con(lc.first);
			SCIPcreateConsLinear(scip_, &(tmp), con.getName().c_str(),
				0, nullptr, nullptr, -SCIPinfinity(scip_), SCIPinfinity(scip_),
				TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
			for (auto iter = con.getExpr().getLBegin(); iter != con.getExpr().getLEnd(); ++iter)
				SCIPaddCoefLinear(scip_, tmp, vars_[varidxs_.at(iter.getVar().getImpl())], iter.getCoeff());
			if (!Constant::IsInfinity(con.getLb()))
				SCIPchgLhsLinear(scip_, tmp, con.getLb() - con.getExpr().getConstant());
			if (!Constant::IsInfinity(con.getUb()))
				SCIPchgRhsLinear(scip_, tmp, con.getUb() - con.getExpr().getConstant());
			SCIPaddCons(scip_, tmp);
			lc.second = count++;
		}
		// ��������Լ��
		for (auto& qc : qcidxs_)
		{
			auto& tmp(cons_[count]);
			Constraint::OpQuadCon con(qc.first);
			SCIPcreateConsQuadratic(scip_, &(tmp), con.getName().c_str(),
				0, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0.0, 0.0,
				TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE);
			for (auto iter = con.getExpr().getLBegin(); iter != con.getExpr().getLEnd(); ++iter)
				SCIPaddLinearVarQuadratic(scip_, tmp, vars_[varidxs_.at(iter.getVar().getImpl())], iter.getCoeff());
			for (auto iter = con.getExpr().getQBegin(); iter != con.getExpr().getQEnd(); ++iter)
			{
				auto var1(iter.getVar1().getImpl()), var2(iter.getVar2().getImpl());
				if (var1 == var2)
					SCIPaddSquareCoefQuadratic(scip_, tmp, vars_[varidxs_.at(var1)], iter.getCoeff());
				else
					SCIPaddBilinTermQuadratic(scip_, tmp, vars_[varidxs_.at(var1)], vars_[varidxs_.at(var2)], iter.getCoeff());
			}
			if (!Constant::IsInfinity(con.getLb()))
				SCIPchgLhsQuadratic(scip_, tmp, con.getLb() - con.getExpr().getConstant());
			if (!Constant::IsInfinity(con.getUb()))
				SCIPchgRhsQuadratic(scip_, tmp, con.getUb() - con.getExpr().getConstant());
			SCIPaddCons(scip_, tmp);
			qc.second = count++;
		}
		// ������Լ������
		for (auto& nlc : nlcidxs_)
		{
			Constraint::OpNLCon con(nlc.first);
			SCIP_EXPR* rhs(createNewExprOfNLE(con.getExpr()));
			if (rhs)
			{
				auto& tmp(cons_[count]);
				SCIP_EXPR* lhs(createNewExprByOp(con.getVar()));
				SCIP_EXPR* nlexpr(nullptr);
				SCIP_EXPRTREE* nltree(nullptr);
				SCIPexprCreate(SCIPblkmem(scip_), &nlexpr, SCIP_EXPR_MINUS, rhs, lhs);
				SCIPexprtreeCreate(SCIPblkmem(scip_), &nltree, nlexpr, varidxs_.size(), 0, nullptr);
				SCIPexprtreeSetVars(nltree, varidxs_.size(), vars_.data());
				SCIPcreateConsNonlinear(scip_, &tmp, con.getName().c_str(),
					0, nullptr, nullptr, 0, nullptr, nullptr, 0.0, 0.0,
					TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
				SCIPaddExprtreesNonlinear(scip_, tmp, 1, &nltree, nullptr);
				SCIPaddCons(scip_, tmp);
				nlc.second = count;
			}
			else
				OpExcBase("[Solver::OpSCIPSolI::update]: Exception->create NL expression failed");
			count++;
		}
		// ����Ŀ�꺯��
		for (auto iter = obj0_.getLBegin(); iter != obj0_.getLEnd(); ++iter)
			SCIPchgVarObj(scip_, vars_[varidxs_.at(iter.getVar().getImpl())], iter.getCoeff());
		SCIPsetObjsense(scip_, sense_);
		modified_ = false;
	}
}

SCIP_VARTYPE Solver::OpSCIPSolI::senseConvert1(Variable::OpVarType sense)
{
	SCIP_VARTYPE svtype(SCIP_VARTYPE_CONTINUOUS);
	switch (sense)
	{
	case Variable::OpVarType::Bool:
		svtype = SCIP_VARTYPE::SCIP_VARTYPE_BINARY;
		break;
	case Variable::OpVarType::Int:
		svtype = SCIP_VARTYPE::SCIP_VARTYPE_INTEGER;
		break;
	case Variable::OpVarType::Con:
		svtype = SCIP_VARTYPE::SCIP_VARTYPE_CONTINUOUS;
		break;
	case Variable::OpVarType::Sem:
		throw OpExcBase("[Solver::OpSCIPSolI::senseConvert1]: Exception->can not handle Variable::OpVarType::Sem");
		break;
	default:
		throw OpExcBase("[Solver::OpSCIPSolI::senseConvert1]: Exception->can not handle other variable type");
		break;
	}
	return svtype;
}

SCIP_OBJSENSE Solver::OpSCIPSolI::senseConvert2(Objective::OpObjSense sense)
{
	SCIP_OBJSENSE sobjs(SCIP_OBJSENSE_MINIMIZE);
	switch (sense)
	{
	case OPUA::Objective::OpObjSense::Min:
		sobjs = SCIP_OBJSENSE::SCIP_OBJSENSE_MINIMIZE;
		break;
	case OPUA::Objective::OpObjSense::Max:
		sobjs = SCIP_OBJSENSE::SCIP_OBJSENSE_MAXIMIZE;
		break;
	default:
		throw OpExcBase("[Solver::OpSCIPSolI::senseConvert2]: Exception->can not handle other objective function sense");
		break;
	}
	return sobjs;
}

SCIP_EXPR* Solver::OpSCIPSolI::createNewExprByOp(OpFloat val)
{
	SCIP_EXPR* newexpr(nullptr);
	SCIPexprCreate(SCIPblkmem(scip_), &newexpr, SCIP_EXPR_CONST, val);
	return newexpr;
}

SCIP_EXPR* Solver::OpSCIPSolI::createNewExprByOp(Variable::OpVar var)
{
	SCIP_EXPR* newexpr(nullptr);
	SCIPexprCreate(SCIPblkmem(scip_), &newexpr, SCIP_EXPR_VARIDX, varidxs_.at(var.getImpl()));
	return newexpr;
}

SCIP_EXPR* Solver::OpSCIPSolI::createNewExprByOp(const Expression::OpNLExpr& expr, SCIP_EXPROP op, bool param)
{
	SCIP_EXPR* newexpr(nullptr);
	SCIP_EXPR* varexpr(createNewExprByOp(expr.getVar(0)));
	if (param)
	{
		SCIP_EXPR* valexpr(createNewExprByOp(expr.getParam()));
		SCIPexprCreate(SCIPblkmem(scip_), &newexpr, op, varexpr, valexpr);
	}
	else
	{
		SCIPexprCreate(SCIPblkmem(scip_), &newexpr, op, varexpr);
	}
	return newexpr;
}

SCIP_EXPR* Solver::OpSCIPSolI::createNewExprByOp2(const Expression::OpNLExpr& expr, SCIP_EXPROP op, bool param)
{
	SCIP_EXPR* newexpr(nullptr);
	SCIP_EXPR* var1expr(createNewExprByOp(expr.getVar(0)));
	SCIP_EXPR* var2expr(createNewExprByOp(expr.getVar(1)));
	if (param)
	{
		SCIP_EXPR* valexpr(createNewExprByOp(expr.getParam()));
		SCIPexprCreate(SCIPblkmem(scip_), &newexpr, op, var1expr, var2expr, valexpr);
	}
	else
	{
		SCIPexprCreate(SCIPblkmem(scip_), &newexpr, op, var1expr, var2expr);
	}
	return newexpr;
}

SCIP_EXPR* Solver::OpSCIPSolI::createNewExprByOpN(const Expression::OpNLExpr& expr, SCIP_EXPROP op, int nvars, bool param)
{
	SCIP_EXPR* newexpr(nullptr);
	return newexpr;
}

SCIP_EXPR* Solver::OpSCIPSolI::createNewExprOfNLE(const Expression::OpNLExpr& expr)
{
	SCIP_EXPR* rhs(nullptr);
	switch (expr.getFunction())
	{
	case OPUA::Expression::OpNLFunc::Unknow: // �޷�����
		throw OpExcBase("[Solver::OpSCIPSolI::createNewExprOfNLE]: Exception->can not handle Expression::OpNLFunc::Unknow");
		break;
	case OPUA::Expression::OpNLFunc::Sum: // �޷�����
		throw OpExcBase("[Solver::OpSCIPSolI::createNewExprOfNLE]: Exception->can not handle Expression::OpNLFunc::Sum");
		break;
	case OPUA::Expression::OpNLFunc::Abs:
		rhs = createNewExprByOp(expr, SCIP_EXPR_ABS, false);
		break;
	case OPUA::Expression::OpNLFunc::Max:
		rhs = createNewExprByOp2(expr, SCIP_EXPR_MAX, false);
		break;
	case OPUA::Expression::OpNLFunc::Min:
		rhs = createNewExprByOp2(expr, SCIP_EXPR_MIN, false);
		break;
	case OPUA::Expression::OpNLFunc::Square:
		rhs = createNewExprByOp(expr, SCIP_EXPR_SQUARE, false);
		break;
	case OPUA::Expression::OpNLFunc::Sqrt:
		rhs = createNewExprByOp(expr, SCIP_EXPR_SQRT, false);
		break;
	case OPUA::Expression::OpNLFunc::Pow: // �޷�����
		throw OpExcBase("[Solver::OpSCIPSolI::createNewExprOfNLE]: Exception->can not handle Expression::OpNLFunc::Pow");
		break;
	case OPUA::Expression::OpNLFunc::Exp1:
		rhs = createNewExprByOp(expr, SCIP_EXPR_EXP, false);
		break;
	case OPUA::Expression::OpNLFunc::Exp2:// �޷�����
		throw OpExcBase("[Solver::OpSCIPSolI::createNewExprOfNLE]: Exception->can not handle Expression::OpNLFunc::Exp2");
		break;
	case OPUA::Expression::OpNLFunc::Log1:// �޷�����
		throw OpExcBase("[Solver::OpSCIPSolI::createNewExprOfNLE]: Exception->can not handle Expression::OpNLFunc::Log1");
		break;
	case OPUA::Expression::OpNLFunc::Log2:
		rhs = createNewExprByOp(expr, SCIP_EXPR_LOG, false);
		break;
	case OPUA::Expression::OpNLFunc::Log3:// �޷�����
		throw OpExcBase("[Solver::OpSCIPSolI::createNewExprOfNLE]: Exception->can not handle Expression::OpNLFunc::Log3");
		break;
	case OPUA::Expression::OpNLFunc::Sin:
		rhs = createNewExprByOp(expr, SCIP_EXPR_SIN, false);
		break;
	case OPUA::Expression::OpNLFunc::Cos:
		rhs = createNewExprByOp(expr, SCIP_EXPR_COS, false);
		break;
	case OPUA::Expression::OpNLFunc::Tan:
		rhs = createNewExprByOp(expr, SCIP_EXPR_TAN, false);
		break;
	default:
		throw OpExcBase("[Solver::OpSCIPSolI::createNewExprOfNLE]: Exception->can not handle other NL function");
		break;
	}
	return rhs;
}

void Solver::OpSCIPSolI::init()
{

}

void Solver::OpSCIPSolI::clear()
{
	clearSCIP();
	clearMaps();
}

void Solver::OpSCIPSolI::extract(Model::OpModel mdl)
{
	try
	{
		// �������ԭ��ģ��
		clear();
		// ������ģ��
		for (auto iter = mdl.getCBegin<Variable::OpVar>(); iter != mdl.getCEnd<Variable::OpVar>(); ++iter)
			varidxs_.emplace(iter.getVal().getImpl(), SIZE_MAX);
		for (auto iter = mdl.getCBegin<Variable::OpPSDVar>(); iter != mdl.getCEnd<Variable::OpPSDVar>(); ++iter)
			throw OpExcBase("[Solver::OpSCIPSolI::extract]: Exception->can not handle Variable::OpPSDVar");
		for (auto iter = mdl.getCBegin<Constraint::OpLinCon>(); iter != mdl.getCEnd<Constraint::OpLinCon>(); ++iter)
			lcidxs_.emplace(iter.getVal().getImpl(), SIZE_MAX);
		for (auto iter = mdl.getCBegin<Constraint::OpQuadCon>(); iter != mdl.getCEnd<Constraint::OpQuadCon>(); ++iter)
			qcidxs_.emplace(iter.getVal().getImpl(), SIZE_MAX);
		for (auto iter = mdl.getCBegin<Constraint::OpConicCon>(); iter != mdl.getCEnd<Constraint::OpConicCon>(); ++iter)
			throw OpExcBase("[Solver::OpSCIPSolI::extract]: Exception->can not handle Constraint::OpConicCon");
		for (auto iter = mdl.getCBegin<Constraint::OpPSDCon>(); iter != mdl.getCEnd<Constraint::OpPSDCon>(); ++iter)
			throw OpExcBase("[Solver::OpSCIPSolI::extract]: Exception->can not handle Constraint::OpPSDCon");
		for (auto iter = mdl.getCBegin<Constraint::OpSOSCon>(); iter != mdl.getCEnd<Constraint::OpSOSCon>(); ++iter)
			throw OpExcBase("[Solver::OpSCIPSolI::extract]: Exception->can not handle Constraint::OpSOSCon");
		for (auto iter = mdl.getCBegin<Constraint::OpNLCon>(); iter != mdl.getCEnd<Constraint::OpNLCon>(); ++iter)
			nlcidxs_.emplace(iter.getVal().getImpl(), SIZE_MAX);
		for (auto iter = mdl.getCBegin<Constraint::OpCdtCon>(); iter != mdl.getCEnd<Constraint::OpCdtCon>(); ++iter)
			throw OpExcBase("[Solver::OpSCIPSolI::extract]: Exception->can not handle Constraint::OpCdtCon");
		obj0_ = mdl.getObj().getLinExpr();
		sense_ = senseConvert2(mdl.getObj().getSense());
		// ����SCIPģ��
		update();
	}
	catch (OpExcBase& e)
	{
		clear();
		throw e;
	}
	catch (std::exception& e)
	{
		clear();
		throw e;
	}
	catch (...)
	{
		clear();
		throw OpExcBase("[Solver::OpSCIPSolI::extract]: Exception->unknow exception!");
	}
}

void Solver::OpSCIPSolI::solve()
{
	if (scip_)
	{
		SCIPsolve(scip_);
		sol_ = SCIPgetBestSol(scip_);
	}
}

OpFloat Solver::OpSCIPSolI::getValue(Variable::OpVar var) const
{
	return SCIPgetSolVal(scip_, sol_, vars_[varidxs_.at(var.getImpl())]);
}

OpFloat Solver::OpSCIPSolI::getValue(const Expression::OpLinExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	return result;
}

OpFloat Solver::OpSCIPSolI::getValue(const Expression::OpQuadExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar1()) * getValue(iter.getVar2());
	return result;
}

OpFloat Solver::OpSCIPSolI::getValue(Objective::OpObj obj) const
{
	return getValue(obj.getLinExpr()) + getValue(obj.getQuadExpr());
}

OpFloat Solver::OpSCIPSolI::getObjValue()
{
	return SCIPgetSolOrigObj(scip_, sol_) + obj0_.getConstant();
}

SCIP_STATUS Solver::OpSCIPSolI::getStatus()
{
	return SCIPgetStatus(scip_);
}

void Solver::OpSCIPSolI::write(OpStr path)
{
	if (scip_)
	{
		if (path.size())
		{
			FILE* fr(nullptr);
			fr = fopen(path.c_str(), "wt+");
			SCIPprintOrigProblem(scip_, fr, nullptr, FALSE);
			fclose(fr);
		}
		else
		{
			SCIPprintOrigProblem(scip_, nullptr, nullptr, FALSE);
		}
	}
}

OpULInt Solver::OpSCIPSolI::getMemoryUsage() const
{
	return sizeof(*this);
}

Solver::OpSCIPSolI::OpSCIPSolI(OpEnvI* env)
	: OpImplBase('S', env),
	modified_(false), sense_(SCIP_OBJSENSE::SCIP_OBJSENSE_MINIMIZE), scip_(false), sol_(false)
{

}

Solver::OpSCIPSolI::OpSCIPSolI(OpEnvI* env, Model::OpModel mdl)
	: OpImplBase('S', env),
	modified_(false), sense_(SCIP_OBJSENSE::SCIP_OBJSENSE_MINIMIZE), scip_(false), sol_(false)
{
	extract(mdl);
}

Solver::OpSCIPSolI::~OpSCIPSolI()
{
	clear();
}

/* OPUA::Solver::OpSCIPSol */

void Solver::OpSCIPSol::extract(Model::OpModel mdl)
{
	static_cast<OpSCIPSolI*>(impl_)->extract(mdl);
}

void Solver::OpSCIPSol::solve()
{
	static_cast<OpSCIPSolI*>(impl_)->solve();
}

void Solver::OpSCIPSol::solveFixed()
{

}

void Solver::OpSCIPSol::setParam(const OpConfig& cfg)
{
	
}

OpLInt Solver::OpSCIPSol::getStatus() const
{
	return static_cast<OpSCIPSolI*>(impl_)->getStatus();
}

OpFloat Solver::OpSCIPSol::getObjValue() const
{
	return static_cast<OpSCIPSolI*>(impl_)->getObjValue();
}

OpFloat Solver::OpSCIPSol::getValue(Variable::OpVar var) const
{
	return static_cast<OpSCIPSolI*>(impl_)->getValue(var);
}

OpFloat Solver::OpSCIPSol::getValue(const Expression::OpLinExpr& expr) const
{
	return static_cast<OpSCIPSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpSCIPSol::getValue(const Expression::OpQuadExpr& expr) const
{
	return static_cast<OpSCIPSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpSCIPSol::getValue(Objective::OpObj obj) const
{
	return static_cast<OpSCIPSolI*>(impl_)->getValue(obj);
}

OpFloat Solver::OpSCIPSol::getDual(Constraint::OpLinCon con) const
{
	return 0.0;
}

Solver::OpSCIPSolI* Solver::OpSCIPSol::getImpl() const
{
	return static_cast<OpSCIPSolI*>(impl_);
}

void Solver::OpSCIPSol::write(OpStr path) const
{
	static_cast<OpSCIPSolI*>(impl_)->write(path);
}

void Solver::OpSCIPSol::release0()
{
	static_cast<OpSCIPSolI*>(impl_)->release();
}

OpBool Solver::OpSCIPSol::operator==(const OpSCIPSol& sol) const
{
	return impl_ == sol.impl_;
}

OpBool Solver::OpSCIPSol::operator!=(const OpSCIPSol& sol) const
{
	return impl_ != sol.impl_;
}

Solver::OpSCIPSol::OpSCIPSol()
{

}

Solver::OpSCIPSol::OpSCIPSol(OpSCIPSolI* impl)
{
	impl_ = impl;
}

Solver::OpSCIPSol::OpSCIPSol(OpEnv env)
{
	impl_ = new OpSCIPSolI(env.getImpl());
}

OPUA::Solver::OpSCIPSol::OpSCIPSol(OpEnv env, Model::OpModel mdl)
{
	impl_ = new OpSCIPSolI(env.getImpl(), mdl);
}

OPUA::Solver::OpSCIPSol::~OpSCIPSol()
{

}
#endif
