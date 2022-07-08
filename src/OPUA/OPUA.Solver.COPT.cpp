#include "OPUA.Solver.h"
#ifdef OPUA_COMPILE_COPT
#include <coptcpp_inc/coptcpp_pch.h>
#endif

#ifdef OPUA_COMPILE_COPT
/* OPUA::Solver::OpCOPTCfgCvt */

// COPT���÷�����
class OPUA::Solver::OpCOPTCfgCvt
{
protected:
	std::unordered_map<OpStr, OpStr> dpdict_;
	std::unordered_map<OpStr, OpStr> ipdict_;
public:
	void init();
	OpStr getDoubleParam(OpStr key);
	OpStr getIntParam(OpStr key);
public:
	OpCOPTCfgCvt();
};

void OPUA::Solver::OpCOPTCfgCvt::init()
{
#ifdef OPUA_COPT_VERSION_501
	// ���ö�Ӧ�汾��5.0.1
	// ������������

	// ���ظ������

#endif
}

OPUA::OpStr OPUA::Solver::OpCOPTCfgCvt::getDoubleParam(OpStr key)
{
	return dpdict_.at(key);
}

OPUA::OpStr OPUA::Solver::OpCOPTCfgCvt::getIntParam(OpStr key)
{
	return ipdict_.at(key);
}

OPUA::Solver::OpCOPTCfgCvt::OpCOPTCfgCvt()
{
	init();
}

/* OPUA::Solver::OpCOPTSolI */

typedef Envr COPTEnv;
typedef Model COPTModel;
typedef Var COPTVar;
typedef VarArray COPTVarArr;
typedef PsdVar COPTPSDVar;
typedef Expr COPTLinExpr;
typedef QuadExpr COPTQuadExpr;
typedef Constraint COPTLinCon;
typedef QConstraint COPTQuadCon;
typedef Sos COPTSOSCon;
typedef GenConstr COPTGenCon;
typedef PsdConstraint COPTPSDCon;

class OPUA::Solver::OpCOPTSolI
	: public OpImplBase
{
protected:
	// COPT���ģ��

	COPTEnv tenv_; // COPT��������
	COPTModel tmdl_; // COPTģ�Ͷ���
protected:
	// OPUA-COPTӳ����Ϣ

	std::unordered_map<OpLInt, COPTVar> vardict_; // COPT������
	std::unordered_map<OpLInt, COPTLinCon> lcdict_; // COPT����Լ����
	std::unordered_map<OpLInt, COPTQuadCon> qcdict_; // COPT����Լ����
	std::unordered_map<OpLInt, COPTSOSCon> scdict_; // COPTSOSԼ����
	std::unordered_map<OpLInt, COPTGenCon> nlcdict_; // COPTָʾ��Լ����
	OpCOPTCfgCvt cfgcvt_; // ���÷�����

	friend class OPUA::Solver::OpCOPTSol;
private:
	COPTVar addCOPTVar(OPUA::Variable::OpVar var); // ��OPUA��������һ��COPT����
	char senseConvert1(OPUA::Constraint::OpConSense sense); // ��OPUAԼ������ת��ΪCOPTԼ������
	int senseConvert2(OPUA::Constraint::OpConSense sense); // ��OPUAԼ������ת��ΪCOPTԼ������
	COPTLinExpr addCOPTLE(const OPUA::Expression::OpLinExpr & expr); // ��OPUA���Ա��ʽ����һ��COPT���Ա��ʽ
	COPTQuadExpr addCOPTQE(const OPUA::Expression::OpQuadExpr& expr); // ��OPUA���α��ʽ����һ��COPT���α��ʽ
	COPTLinCon addCOPTRange(Constraint::OpLinCon con);  // ��OPUA����Լ������һ��COPT����Լ��
	COPTLinCon addCOPTLC(OPUA::Constraint::OpLinCon con); // ��OPUA����Լ������һ��COPT����Լ��
	COPTLinCon addCOPTLC(OPUA::Constraint::OpLinCon con, OpBool lb); // ��OPUA����Լ������һ��COPT����Լ��
	COPTQuadCon addCOPTQC(OPUA::Constraint::OpQuadCon con); // ��OPUA����Լ������һ��COPT����Լ��
	COPTQuadCon addCOPTQC(OPUA::Constraint::OpQuadCon con, OpBool lb); // ��OPUA����Լ������һ��COPT����Լ��
	COPTSOSCon addCOPTSOS(OPUA::Constraint::OpSOSCon con); // ��OPUASOSԼ������һ��COPTSOSԼ��
	COPTGenCon addCOPTGenIndicator(OPUA::Constraint::OpCdtCon con); // ��OPUA����Լ������һ��COPTGenConָʾ��Լ��
	COPTGenCon addCOPTGenIndicator(OPUA::Constraint::OpCdtCon con, OpBool lb); // ��OPUA����Լ������һ��COPTGenConָʾ��Լ��
protected:
	void init(); // ��ʼ��
	void clear(); // ������������ӳ����Ϣ
	void extract(OPUA::Model::OpModel mdl); // ��ȡOPUAģ��
	void solve(); // ���ģ��
	void solveFixed(); // �̶����������Ⲣ���ģ��
	void setParam(const OPUA::Solver::OpConfig& cfg); // ��������
	OPUA::OpLInt getStatus() const; // ��ȡ���״̬
	OPUA::OpFloat getObjValue() const; // ��ȡĿ�꺯����
	OPUA::OpFloat getValue(OPUA::Variable::OpVar var) const; // ��ȡ�����Ľ�
	OPUA::OpFloat getValue(const OPUA::Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
	OPUA::OpFloat getValue(const OPUA::Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
	OPUA::OpFloat getValue(OPUA::Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
	OPUA::OpFloat getDual(OPUA::Constraint::OpLinCon con) const; // ��ȡ��ż��
	void write(OPUA::OpStr path) const; // ��ģ��д���ļ�
	virtual OPUA::OpULInt getMemoryUsage() const; // ��ȡ�ڴ�ռ��
protected:
	OpCOPTSolI(OPUA::OpEnvI* env);
	OpCOPTSolI(OPUA::OpEnvI* env, OPUA::Model::OpModel mdl);
public:
	virtual ~OpCOPTSolI();
};

COPTVar OPUA::Solver::OpCOPTSolI::addCOPTVar(OPUA::Variable::OpVar var)
{
	double lb(OPUA::Constant::IsInfinity(var.getLb()) ? -COPT_INFINITY : var.getLb());
	double ub(OPUA::Constant::IsInfinity(var.getUb()) ? COPT_INFINITY : var.getUb());
	char vtype(COPT_CONTINUOUS);
	switch (var.getType())
	{
	case OPUA::Variable::OpVarType::Bool:
		vtype = COPT_BINARY;
		break;
	case OPUA::Variable::OpVarType::Int:
		vtype = COPT_INTEGER;
		break;
	case OPUA::Variable::OpVarType::Con:
		vtype = COPT_CONTINUOUS;
		break;
	default:
		break;
	}
	return tmdl_.AddVar(lb, ub, 0.0, vtype, var.getName().c_str());
}

char OPUA::Solver::OpCOPTSolI::senseConvert1(OPUA::Constraint::OpConSense sense)
{
	auto tmp(COPT_EQUAL);
	switch (sense)
	{
	case OPUA::Constraint::OpConSense::Equal:
		tmp = COPT_EQUAL;
		break;
	case OPUA::Constraint::OpConSense::LessEqual:
		tmp = COPT_LESS_EQUAL;
		break;
	case OPUA::Constraint::OpConSense::GreatEqual:
		tmp = COPT_GREATER_EQUAL;
		break;
	default:
		break;
	}
	return tmp;
}

int OPUA::Solver::OpCOPTSolI::senseConvert2(OPUA::Constraint::OpConSense sense)
{
	auto tmp(COPT_SOS_TYPE1);
	switch (sense)
	{
	case OPUA::Constraint::OpConSense::SOS1:
		tmp = COPT_SOS_TYPE1;
		break;
	case OPUA::Constraint::OpConSense::SOS2:
		tmp = COPT_SOS_TYPE2;
		break;
	default:
		break;
	}
	return tmp;
}

COPTLinExpr OPUA::Solver::OpCOPTSolI::addCOPTLE(const OPUA::Expression::OpLinExpr& expr)
{
	COPTLinExpr tmp(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		tmp.AddTerm(vardict_.at(iter.getVar().getIndex()), iter.getCoeff());
	return tmp;
}

COPTQuadExpr OPUA::Solver::OpCOPTSolI::addCOPTQE(const OPUA::Expression::OpQuadExpr& expr)
{
	COPTQuadExpr tmp(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		tmp.AddTerm(vardict_.at(iter.getVar().getIndex()), iter.getCoeff());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		tmp.AddTerm(vardict_.at(iter.getVar1().getIndex()), vardict_.at(iter.getVar2().getIndex()), iter.getCoeff());
	return tmp;
}

COPTLinCon OPUA::Solver::OpCOPTSolI::addCOPTRange(Constraint::OpLinCon con)
{
	double lb(OPUA::Constant::IsInfinity(con.getLb()) ? -COPT_INFINITY : con.getLb());
	double ub(OPUA::Constant::IsInfinity(con.getUb()) ? COPT_INFINITY : con.getUb());
	return tmdl_.AddConstr(addCOPTLE(con.getExpr()), lb, ub, con.getName().c_str());
}

COPTLinCon OPUA::Solver::OpCOPTSolI::addCOPTLC(OPUA::Constraint::OpLinCon con)
{
	return tmdl_.AddConstr(addCOPTLE(con.getExpr()), COPT_EQUAL, con.getUb(), con.getName().c_str());
}

COPTLinCon OPUA::Solver::OpCOPTSolI::addCOPTLC(OPUA::Constraint::OpLinCon con, OpBool lb)
{
	if (lb)
		return tmdl_.AddConstr(addCOPTLE(con.getExpr()), COPT_GREATER_EQUAL, con.getLb(), con.getName().c_str());
	else
		return tmdl_.AddConstr(addCOPTLE(con.getExpr()), COPT_LESS_EQUAL, con.getUb(), con.getName().c_str());
}

COPTQuadCon OPUA::Solver::OpCOPTSolI::addCOPTQC(OPUA::Constraint::OpQuadCon con)
{
	return tmdl_.AddQConstr(addCOPTQE(con.getExpr()), COPT_EQUAL, con.getUb(), con.getName().c_str());
}

COPTQuadCon OPUA::Solver::OpCOPTSolI::addCOPTQC(OPUA::Constraint::OpQuadCon con, OpBool lb)
{
	if (lb)
		return tmdl_.AddQConstr(addCOPTQE(con.getExpr()), COPT_GREATER_EQUAL, con.getLb(), con.getName().c_str());
	else
		return tmdl_.AddQConstr(addCOPTQE(con.getExpr()), COPT_LESS_EQUAL, con.getUb(), con.getName().c_str());
}

COPTSOSCon OPUA::Solver::OpCOPTSolI::addCOPTSOS(OPUA::Constraint::OpSOSCon con)
{
	auto& expr(con.getSOSExpr());
	int len(expr.getSize());
	COPTVarArr vars;
	std::vector<double> weights;
	vars.Reserve(len);
	weights.reserve(len);
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
	{
		weights.emplace_back(iter.getCoeff());
		vars.PushBack(vardict_.at(iter.getVar().getIndex()));
	}
	return tmdl_.AddSos(vars, weights.data(), senseConvert2(con.getSense()));
}

COPTGenCon OPUA::Solver::OpCOPTSolI::addCOPTGenIndicator(OPUA::Constraint::OpCdtCon con)
{
	COPTVar ind(vardict_.at(con.getVar().getIndex()));
	auto con2(con.getCon(false));
	return tmdl_.AddGenConstrIndicator(ind, 1, addCOPTLE(con2.getExpr()), COPT_EQUAL, con2.getUb());
}

COPTGenCon OPUA::Solver::OpCOPTSolI::addCOPTGenIndicator(OPUA::Constraint::OpCdtCon con, OpBool lb)
{
	COPTVar ind(vardict_.at(con.getVar().getIndex()));
	auto con2(con.getCon(false));
	if (lb)
		return tmdl_.AddGenConstrIndicator(ind, 1, addCOPTLE(con2.getExpr()), COPT_GREATER_EQUAL, con2.getLb());
	else
		return tmdl_.AddGenConstrIndicator(ind, 1, addCOPTLE(con2.getExpr()), COPT_LESS_EQUAL, con2.getUb());
}

void OPUA::Solver::OpCOPTSolI::init()
{
	tenv_ = COPTEnv();
	tmdl_ = tenv_.CreateModel("");
}

void OPUA::Solver::OpCOPTSolI::clear()
{
	vardict_.clear();
	lcdict_.clear();
	qcdict_.clear();
	scdict_.clear();
	nlcdict_.clear();
	tmdl_ = COPTModel(nullptr);
	tenv_ = COPTEnv(static_cast<Copt::IEnvr*>(nullptr));
}

void OPUA::Solver::OpCOPTSolI::extract(OPUA::Model::OpModel mdl)
{
	// �������ԭģ��
	clear();
	init();
	// ��������ģ��
	for (auto iter = mdl.getCBegin<OPUA::Variable::OpVar>(); iter != mdl.getCEnd<OPUA::Variable::OpVar>(); ++iter)
		vardict_.emplace(iter.getKey(), addCOPTVar(iter.getVal()));
	for (auto iter = mdl.getCBegin<OPUA::Constraint::OpLinCon>(); iter != mdl.getCEnd<OPUA::Constraint::OpLinCon>(); ++iter)
	{
		auto& con(iter.getVal());
		if (OPUA::Constant::IsEqual(con.getLb(), con.getUb()))
			lcdict_.emplace(iter.getKey(), addCOPTLC(con));
		else
			lcdict_.emplace(iter.getKey(), addCOPTRange(con));
	}
	for (auto iter = mdl.getCBegin<OPUA::Constraint::OpQuadCon>(); iter != mdl.getCEnd<OPUA::Constraint::OpQuadCon>(); ++iter)
	{
		auto& con(iter.getVal());
		if (OPUA::Constant::IsEqual(con.getLb(), con.getUb()))
		{
			qcdict_.emplace(iter.getKey(), addCOPTQC(con));
		}
		else
		{
			if (!OPUA::Constant::IsInfinity(con.getLb()))
				qcdict_.emplace(iter.getKey(), addCOPTQC(con, true));
			if (!OPUA::Constant::IsInfinity(con.getUb()))
				qcdict_.emplace(iter.getKey(), addCOPTQC(con, false));
		}
	}
	for (auto iter = mdl.getCBegin<OPUA::Constraint::OpSOSCon>(); iter != mdl.getCEnd<OPUA::Constraint::OpSOSCon>(); ++iter)
		scdict_.emplace(iter.getKey(), addCOPTSOS(iter.getVal()));
	for (auto iter = mdl.getCBegin<OPUA::Constraint::OpCdtCon>(); iter != mdl.getCEnd<OPUA::Constraint::OpCdtCon>(); ++iter)
	{
		auto& con(iter.getVal());
		if (con.isIndicator())
		{
			auto& con2(con.getCon(false));
			if (OPUA::Constant::IsEqual(con2.getLb(), con2.getUb()))
			{
				nlcdict_.emplace(iter.getKey(), addCOPTGenIndicator(con));
			}
			else
			{
				if (!OPUA::Constant::IsInfinity(con2.getLb()))
					nlcdict_.emplace(iter.getKey(), addCOPTGenIndicator(con, true));
				if (!OPUA::Constant::IsInfinity(con2.getUb()))
					nlcdict_.emplace(iter.getKey(), addCOPTGenIndicator(con, false));
			}
		}
	}
	switch (mdl.getObj().getSense())
	{
	case OPUA::Objective::OpObjSense::Min:
		tmdl_.SetQuadObjective(addCOPTQE(mdl.getObj().getQuadExpr() + mdl.getObj().getLinExpr()), COPT_MINIMIZE);
		break;
	case OPUA::Objective::OpObjSense::Max:
		tmdl_.SetQuadObjective(addCOPTQE(mdl.getObj().getQuadExpr() + mdl.getObj().getLinExpr()), COPT_MAXIMIZE);
		break;
	default:
		break;
	}
}

void OPUA::Solver::OpCOPTSolI::solve()
{
	tmdl_.Solve();
}

void OPUA::Solver::OpCOPTSolI::solveFixed()
{
	
}

void OPUA::Solver::OpCOPTSolI::setParam(const OPUA::Solver::OpConfig& cfg)
{
	for (auto iter = cfg.getCBegin<OPUA::OpInt>("OPUA.COPT"); iter != cfg.getCEnd<OPUA::OpInt>("OPUA.COPT"); ++iter)
		if (iter.ok())
			tmdl_.SetIntParam(cfgcvt_.getIntParam(iter.getKey()).c_str(), iter.getVal());
	for (auto iter = cfg.getCBegin<OPUA::OpFloat>("OPUA.COPT"); iter != cfg.getCEnd<OPUA::OpFloat>("OPUA.COPT"); ++iter)
		if (iter.ok())
			tmdl_.SetDblParam(cfgcvt_.getDoubleParam(iter.getKey()).c_str(), iter.getVal());
}

OPUA::OpLInt OPUA::Solver::OpCOPTSolI::getStatus() const
{
	OPUA::OpLInt status(0);
	auto tmpMdl(tmdl_);
	if (tmpMdl.GetIntAttr(COPT_INTATTR_ISMIP))
		status = tmpMdl.GetIntAttr(COPT_INTATTR_MIPSTATUS);
	else
		status = tmpMdl.GetIntAttr(COPT_INTATTR_LPSTATUS);
	return status;
}

OPUA::OpFloat OPUA::Solver::OpCOPTSolI::getObjValue() const
{
	OPUA::OpFloat objval(OPUA::Constant::NaN);
	auto tmpMdl(tmdl_);
	if (tmpMdl.GetIntAttr(COPT_INTATTR_ISMIP))
		objval = tmpMdl.GetDblAttr(COPT_DBLATTR_BESTOBJ);
	else
		objval = tmpMdl.GetDblAttr(COPT_DBLATTR_LPOBJVAL);
	return objval;
}

OPUA::OpFloat OPUA::Solver::OpCOPTSolI::getValue(OPUA::Variable::OpVar var) const
{
	auto coptvar(vardict_.at(var.getIndex()));
	return coptvar.Get(COPT_DBLINFO_VALUE);
}

OPUA::OpFloat OPUA::Solver::OpCOPTSolI::getValue(const OPUA::Expression::OpLinExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	return result;
}

OPUA::OpFloat OPUA::Solver::OpCOPTSolI::getValue(const OPUA::Expression::OpQuadExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar1()) * getValue(iter.getVar2());
	return result;
}

OPUA::OpFloat OPUA::Solver::OpCOPTSolI::getValue(OPUA::Objective::OpObj obj) const
{
	return getValue(obj.getLinExpr()) + getValue(obj.getQuadExpr());
}

OPUA::OpFloat OPUA::Solver::OpCOPTSolI::getDual(OPUA::Constraint::OpLinCon con) const
{
	auto coptcon(lcdict_.at(con.getIndex()));
	return coptcon.Get(COPT_DBLINFO_DUAL);
}

void OPUA::Solver::OpCOPTSolI::write(OPUA::OpStr path) const
{
	if (path.size())
	{
		auto tmpMdl(tmdl_);
		if (tmpMdl.GetIntAttr(COPT_INTATTR_ISMIP))
			tmpMdl.WriteMps(path.c_str());
		else
			tmpMdl.WriteLp(path.c_str());
	}
	else
	{
		// ��֧�����������̨
	}
}

OPUA::OpULInt OPUA::Solver::OpCOPTSolI::getMemoryUsage() const
{
	return sizeof(*this);
}

OPUA::Solver::OpCOPTSolI::OpCOPTSolI(OPUA::OpEnvI* env)
	: OpImplBase('S', env),
	tenv_(static_cast<Copt::IEnvr*>(nullptr)),
	tmdl_(nullptr)
{

}

OPUA::Solver::OpCOPTSolI::OpCOPTSolI(OPUA::OpEnvI* env, OPUA::Model::OpModel mdl)
	: OpImplBase('S', env),
	tenv_(static_cast<Copt::IEnvr*>(nullptr)),
	tmdl_(nullptr)
{
	extract(mdl);
}

OPUA::Solver::OpCOPTSolI::~OpCOPTSolI()
{
	clear();
}

/* OPUA::Solver::OpCOPTSol */

void OPUA::Solver::OpCOPTSol::extract(OPUA::Model::OpModel mdl)
{
	static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->extract(mdl);
}

void OPUA::Solver::OpCOPTSol::solve()
{
	static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->solve();
}

void OPUA::Solver::OpCOPTSol::solveFixed()
{
	static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->solveFixed();
}

void OPUA::Solver::OpCOPTSol::setParam(const OPUA::Solver::OpConfig& cfg)
{
	static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->setParam(cfg);
}

OPUA::OpLInt OPUA::Solver::OpCOPTSol::getStatus() const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getStatus();
}

OPUA::OpFloat OPUA::Solver::OpCOPTSol::getObjValue() const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getObjValue();
}

OPUA::OpFloat OPUA::Solver::OpCOPTSol::getValue(OPUA::Variable::OpVar var) const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getValue(var);
}

OPUA::OpFloat OPUA::Solver::OpCOPTSol::getValue(const OPUA::Expression::OpLinExpr& expr) const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getValue(expr);
}

OPUA::OpFloat OPUA::Solver::OpCOPTSol::getValue(const OPUA::Expression::OpQuadExpr& expr) const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getValue(expr);
}

OPUA::OpFloat OPUA::Solver::OpCOPTSol::getValue(OPUA::Objective::OpObj obj) const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getValue(obj);
}

OPUA::OpFloat OPUA::Solver::OpCOPTSol::getDual(OPUA::Constraint::OpLinCon con) const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getDual(con);
}

OPUA::Solver::OpCOPTSolI* OPUA::Solver::OpCOPTSol::getImpl() const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_);
}

void OPUA::Solver::OpCOPTSol::write(OPUA::OpStr path) const
{
	static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->write(path);
}

void OPUA::Solver::OpCOPTSol::release0()
{
	static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->release();
}

OPUA::OpBool OPUA::Solver::OpCOPTSol::operator==(const OPUA::Solver::OpCOPTSol& sol) const
{
	return impl_ == sol.impl_;
}

OPUA::OpBool OPUA::Solver::OpCOPTSol::operator!=(const OPUA::Solver::OpCOPTSol& sol) const
{
	return impl_ != sol.impl_;
}

OPUA::Solver::OpCOPTSol::OpCOPTSol()
{

}

OPUA::Solver::OpCOPTSol::OpCOPTSol(OPUA::Solver::OpCOPTSolI* impl)
{
	impl_ = impl;
}

OPUA::Solver::OpCOPTSol::OpCOPTSol(OPUA::OpEnv env)
{
	impl_ = new OPUA::Solver::OpCOPTSolI(env.getImpl());
}

OPUA::Solver::OpCOPTSol::OpCOPTSol(OPUA::OpEnv env, OPUA::Model::OpModel mdl)
{
	impl_ = new OPUA::Solver::OpCOPTSolI(env.getImpl(), mdl);
}

OPUA::Solver::OpCOPTSol::~OpCOPTSol()
{

}

#endif
