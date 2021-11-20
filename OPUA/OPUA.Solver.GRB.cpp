#include "OPUA.Solver.h"
#include <Gurobi/include/gurobi_c++.h>

using namespace OPUA;

/* OPUA::Solver::OpGRBSolI */

class Solver::OpGRBSolI
	: public OpImplBase
{
protected:
	// GRB���ģ��

	GRBEnv* genv_; // GRB��������
	GRBModel* gmdl_; // GRBģ�Ͷ���
	OpStr gname_; // GRBģ������
protected:
	// OPUA-GRBӳ����Ϣ

	std::unordered_map<OpLInt, GRBVar> vardict_;
	std::unordered_map<OpLInt, GRBConstr> lcdict_;
	std::unordered_map<OpLInt, GRBQConstr> qcdict_;
	std::unordered_map<OpLInt, GRBSOS> scdict_;
	std::unordered_map<OpLInt, GRBGenConstr> nlcdict_;
	std::unordered_map<OpLInt, GRBGenConstr> modict_;

	friend class OpGRBSol;
private:
	GRBVar addGRBVar(Variable::OpVar var); // ��OPUA��������һ��GRB����
	char senseConvert1(Constraint::OpConSense sense); // ��OPUAԼ������ת��ΪGRBԼ������
	int senseConvert2(Constraint::OpConSense sense); // ��OPUAԼ������ת��ΪGRBԼ������
	GRBLinExpr addGRBLE(const Expression::OpLinExpr& expr); // ��OPUA���Ա��ʽ����һ��GRB���Ա��ʽ
	GRBQuadExpr addGRBQE(const Expression::OpQuadExpr& expr); // ��OPUA���α��ʽ����һ��GRB���α��ʽ
	GRBConstr addGRBLC(Constraint::OpLinCon con); // ��OPUA����Լ������һ��GRB����Լ��
	GRBQConstr addGRBQC(Constraint::OpQuadCon con); // ��OPUA����Լ������һ��GRB����Լ��
	GRBSOS addGRBSOS(Constraint::OpSOSCon con); // ��OPUASOSԼ������һ��GRBSOSԼ��
	GRBGenConstr addGRBGen(Constraint::OpNLCon con); // ��OPUA������Լ������һ��GRB������MaxԼ��
	GRBGenConstr addGRBGenMax(Constraint::OpNLCon con); // ��OPUA������Լ������һ��GRB������MaxԼ��
	GRBGenConstr addGRBGenMin(Constraint::OpNLCon con); // ��OPUA������Լ������һ��GRB������MaxԼ��
	GRBGenConstr addGRBGenAbs(Constraint::OpNLCon con); // ��OPUA������Լ������һ��GRB������MaxԼ��
protected:
	void init(); // ��ʼ��
	void clear(); // ������������ӳ����Ϣ
	void extract(Model::OpModel mdl); // ��ȡOPUAģ��
	void solve(); // ���ģ��
	void setConfig(OpStr key, OpBool val); // ��������(������)
	void setConfig(OpStr key, OpLInt val); // ��������(������)
	void setConfig(OpStr key, OpFloat val); // ��������(������)
	void setConfig(OpStr key, OpStr val); // ��������(�ַ���)
	OpFloat getValue(Variable::OpVar var); // ��ȡ�����Ľ�
	OpFloat getValue(const Expression::OpLinExpr& expr); // ��ȡ���ʽ�Ľ�(�ٶȽ���)
	OpFloat getValue(const Expression::OpQuadExpr& expr); // ��ȡ���ʽ�Ľ�(�ٶȽ���)
	OpFloat getValue(Objective::OpObj obj); // ��ȡĿ�꺯����(�ٶȽ���)
	OpFloat getDual(Constraint::OpLinCon con); // ��ȡ��ż��
protected:
	OpGRBSolI(OpEnvI* env);
	OpGRBSolI(OpEnvI* env, OpStr name);
	OpGRBSolI(OpEnvI* env, Model::OpModel mdl);
	OpGRBSolI(OpEnvI* env, Model::OpModel mdl, OpStr name);
public:
	virtual ~OpGRBSolI();
};

GRBVar Solver::OpGRBSolI::addGRBVar(Variable::OpVar var)
{
	char vtype(GRB_CONTINUOUS);
	switch (var.getType())
	{
	case Variable::OpVarType::Bool:
		vtype = GRB_BINARY;
		break;
	case Variable::OpVarType::Int:
		vtype = GRB_INTEGER;
		break;
	case Variable::OpVarType::Con:
		vtype = GRB_CONTINUOUS;
		break;
	case Variable::OpVarType::Sem:
		vtype = GRB_SEMICONT;
		break;
	default:
		break;
	}
	return gmdl_->addVar(var.getLb(), var.getUb(), 0.0, vtype, var.getName());
}

char Solver::OpGRBSolI::senseConvert1(Constraint::OpConSense sense)
{
	auto tmp(GRB_EQUAL);
	switch (sense)
	{
	case OPUA::Constraint::OpConSense::Equal:
		tmp = GRB_EQUAL;
		break;
	case OPUA::Constraint::OpConSense::LessEqual:
		tmp = GRB_LESS_EQUAL;
		break;
	case OPUA::Constraint::OpConSense::GreatEqual:
		tmp = GRB_GREATER_EQUAL;
		break;
	default:
		break;
	}
	return tmp;
}

int Solver::OpGRBSolI::senseConvert2(Constraint::OpConSense sense)
{
	auto tmp(GRB_SOS_TYPE1);
	switch (sense)
	{
	case OPUA::Constraint::OpConSense::SOS1:
		tmp = GRB_SOS_TYPE1;
		break;
	case OPUA::Constraint::OpConSense::SOS2:
		tmp = GRB_SOS_TYPE2;
		break;
	default:
		break;
	}
	return tmp;
}

GRBLinExpr Solver::OpGRBSolI::addGRBLE(const Expression::OpLinExpr& expr)
{
	GRBLinExpr tmp(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		tmp += iter.getCoeff() * vardict_.at(iter.getVar().getIndex());
	return tmp;
}

GRBQuadExpr Solver::OpGRBSolI::addGRBQE(const Expression::OpQuadExpr& expr)
{
	GRBQuadExpr tmp(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		tmp += iter.getCoeff() * vardict_.at(iter.getVar().getIndex());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		tmp += iter.getCoeff() * vardict_.at(iter.getVar1().getIndex()) * vardict_.at(iter.getVar2().getIndex());
	return tmp;
}

GRBConstr Solver::OpGRBSolI::addGRBLC(Constraint::OpLinCon con)
{
	GRBLinExpr lhs(addGRBLE(con.getExpr(true)));
	GRBLinExpr rhs(con.isStandard() ? con.getRHS() : addGRBLE(con.getExpr(false)));
	return gmdl_->addConstr(lhs, senseConvert1(con.getSense()), rhs, con.getName());
}

GRBQConstr Solver::OpGRBSolI::addGRBQC(Constraint::OpQuadCon con)
{
	GRBQuadExpr lhs(addGRBQE(con.getExpr(true)));
	GRBQuadExpr rhs(con.isStandard() ? con.getRHS() : addGRBQE(con.getExpr(false)));
	return gmdl_->addQConstr(lhs, senseConvert1(con.getSense()), rhs, con.getName());
}

GRBSOS Solver::OpGRBSolI::addGRBSOS(Constraint::OpSOSCon con)
{
	auto& expr(con.getSOSExpr());
	int len(expr.getSize());
	std::vector<GRBVar> vars;
	std::vector<double> weights;
	vars.reserve(len);
	weights.reserve(len);
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
	{
		weights.emplace_back(iter.getCoeff());
		vars.emplace_back(vardict_.at(iter.getVar().getIndex()));
	}
	return gmdl_->addSOS(vars.data(), weights.data(), len, senseConvert2(con.getSense()));
}

GRBGenConstr Solver::OpGRBSolI::addGRBGen(Constraint::OpNLCon con)
{
	GRBGenConstr tmp;
	switch (con.getFunction())
	{
	case Expression::OpNLFunc::Unknow: // �޷�����
		break;
	case Expression::OpNLFunc::Sum: // �޷�����
		break;
	case Expression::OpNLFunc::Abs:
		tmp = addGRBGenAbs(con);
		break;
	case Expression::OpNLFunc::Max:
		tmp = addGRBGenMax(con);
		break;
	case Expression::OpNLFunc::Min:
		tmp = addGRBGenMin(con);
		break;
	case Expression::OpNLFunc::Square: // �޷�����
		break;
	case Expression::OpNLFunc::Sqrt: // �޷�����
		break;
	case Expression::OpNLFunc::Pow: // �޷�����
		break;
	case Expression::OpNLFunc::Exp1: // �޷�����
		break;
	case Expression::OpNLFunc::Exp2: // �޷�����
		break;
	case Expression::OpNLFunc::Log1: // �޷�����
		break;
	case Expression::OpNLFunc::Log2: // �޷�����
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

GRBGenConstr Solver::OpGRBSolI::addGRBGenMax(Constraint::OpNLCon con)
{
	auto& expr(con.getExpr());
	int len(expr.getSize());
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	std::vector<GRBVar> vars;
	vars.reserve(len);
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
		vars.emplace_back(vardict_.at(iter.getVar().getIndex()));
	return gmdl_->addGenConstrMax(var0, vars.data(), len, -GRB_INFINITY, con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenMin(Constraint::OpNLCon con)
{
	auto& expr(con.getExpr());
	int len(expr.getSize());
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	std::vector<GRBVar> vars;
	vars.reserve(len);
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
		vars.emplace_back(vardict_.at(iter.getVar().getIndex()));
	return gmdl_->addGenConstrMin(var0, vars.data(), len, GRB_INFINITY, con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenAbs(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrAbs(var0, var1, con.getName());
}

void Solver::OpGRBSolI::init()
{
	genv_ = new GRBEnv();
	gmdl_ = new GRBModel(*genv_);
}

void Solver::OpGRBSolI::clear()
{
	vardict_.clear();
	lcdict_.clear();
	qcdict_.clear();
	scdict_.clear();
	nlcdict_.clear();
	modict_.clear();
	delete gmdl_;
	gmdl_ = nullptr;
	delete genv_;
	genv_ = nullptr;
}

void Solver::OpGRBSolI::extract(Model::OpModel mdl)
{
	// �������ԭģ��
	if (genv_)
	{
		clear();
		init();
	}
	// ��������ģ��
	for (auto iter = mdl.getVBegin(); iter != mdl.getVEnd(); ++iter)
		vardict_.emplace(iter.getKey(), addGRBVar(iter.getVal()));
	for (auto iter = mdl.getLCBegin(); iter != mdl.getLCEnd(); ++iter)
		lcdict_.emplace(iter.getKey(), addGRBLC(iter.getVal()));
	for (auto iter = mdl.getQCBegin(); iter != mdl.getQCEnd(); ++iter)
		qcdict_.emplace(iter.getKey(), addGRBQC(iter.getVal()));
	for (auto iter = mdl.getSOSBegin(); iter != mdl.getSOSEnd(); ++iter)
		scdict_.emplace(iter.getKey(), addGRBSOS(iter.getVal()));
	for (auto iter = mdl.getNLCBegin(); iter != mdl.getNLCEnd(); ++iter)
		nlcdict_.emplace(iter.getKey(), addGRBGen(iter.getVal()));
	gmdl_->setObjective(addGRBQE(mdl.getObj().getQuadExpr()) + addGRBLE(mdl.getObj().getLinExpr()));
}

void Solver::OpGRBSolI::solve()
{
	gmdl_->optimize();
}

void Solver::OpGRBSolI::setConfig(OpStr key, OpBool val)
{

}

void Solver::OpGRBSolI::setConfig(OpStr key, OpLInt val)
{

}

void Solver::OpGRBSolI::setConfig(OpStr key, OpFloat val)
{

}

void Solver::OpGRBSolI::setConfig(OpStr key, OpStr val)
{

}

OpFloat Solver::OpGRBSolI::getValue(Variable::OpVar var)
{
	return vardict_.at(var.getIndex()).get(GRB_DoubleAttr::GRB_DoubleAttr_X);
}

OpFloat Solver::OpGRBSolI::getValue(const Expression::OpLinExpr& expr)
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	return result;
}

OpFloat Solver::OpGRBSolI::getValue(const Expression::OpQuadExpr& expr)
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar1()) * getValue(iter.getVar2());
	return result;
}

OpFloat Solver::OpGRBSolI::getValue(Objective::OpObj obj)
{
	return getValue(obj.getLinExpr()) + getValue(obj.getQuadExpr());
}

OpFloat Solver::OpGRBSolI::getDual(Constraint::OpLinCon con)
{
	return lcdict_.at(con.getIndex()).get(GRB_DoubleAttr::GRB_DoubleAttr_Pi);
}

Solver::OpGRBSolI::OpGRBSolI(OpEnvI* env)
	: OpImplBase('S', env),
	genv_(nullptr), 
	gmdl_(nullptr),
	gname_("GRB_Sol_" + std::to_string(idx_))
{
	init();
}

Solver::OpGRBSolI::OpGRBSolI(OpEnvI* env, OpStr name)
	: OpImplBase('S', env),
	genv_(nullptr),
	gmdl_(nullptr),
	gname_(name)
{
	init();
}

Solver::OpGRBSolI::OpGRBSolI(OpEnvI* env, Model::OpModel mdl)
	: OpImplBase('S', env),
	genv_(nullptr),
	gmdl_(nullptr),
	gname_("GRB_Sol_" + std::to_string(idx_))
{
	init();
	extract(mdl);
}

Solver::OpGRBSolI::OpGRBSolI(OpEnvI* env, Model::OpModel mdl, OpStr name)
	: OpImplBase('S', env),
	genv_(nullptr),
	gmdl_(nullptr),
	gname_(name)
{
	init();
	extract(mdl);
}

Solver::OpGRBSolI::~OpGRBSolI()
{
	clear();
}

/* OPUA::Solver::OpGRBSol */


void Solver::OpGRBSol::extract(Model::OpModel mdl)
{
	static_cast<OpGRBSolI*>(impl_)->extract(mdl);
}

void Solver::OpGRBSol::solve()
{
	static_cast<OpGRBSolI*>(impl_)->solve();
}

void Solver::OpGRBSol::set(OpStr key, OpBool val)
{
	static_cast<OpGRBSolI*>(impl_)->setConfig(key, val);
}

void Solver::OpGRBSol::set(OpStr key, OpLInt val)
{
	static_cast<OpGRBSolI*>(impl_)->setConfig(key, val);
}

void Solver::OpGRBSol::set(OpStr key, OpFloat val)
{
	static_cast<OpGRBSolI*>(impl_)->setConfig(key, val);
}

void Solver::OpGRBSol::set(OpStr key, OpStr val)
{
	static_cast<OpGRBSolI*>(impl_)->setConfig(key, val);
}

OpFloat Solver::OpGRBSol::getValue(Variable::OpVar var)
{
	return static_cast<OpGRBSolI*>(impl_) ->getValue(var);
}

OpFloat Solver::OpGRBSol::getValue(const Expression::OpLinExpr& expr)
{
	return static_cast<OpGRBSolI*>(impl_) ->getValue(expr);
}

OpFloat Solver::OpGRBSol::getValue(const Expression::OpQuadExpr& expr)
{
	return static_cast<OpGRBSolI*>(impl_) ->getValue(expr);
}

OpFloat OPUA::Solver::OpGRBSol::getValue(Objective::OpObj obj)
{
	return static_cast<OpGRBSolI*>(impl_) ->getValue(obj);
}

OpFloat Solver::OpGRBSol::getDual(Constraint::OpLinCon con)
{
	return static_cast<OpGRBSolI*>(impl_) ->getDual(con);
}

Solver::OpGRBSol::OpGRBSol()
{

}

Solver::OpGRBSol::OpGRBSol(OpGRBSolI* impl)
{
	impl_ = impl;
}

Solver::OpGRBSol::OpGRBSol(OpEnv env)
{
	impl_ = new OpGRBSolI(env.getImpl());
}

Solver::OpGRBSol::OpGRBSol(OpEnv env, OpStr name)
{
	impl_ = new OpGRBSolI(env.getImpl(), name);
}

Solver::OpGRBSol::OpGRBSol(OpEnv env, Model::OpModel mdl)
{
	impl_ = new OpGRBSolI(env.getImpl(), mdl);
}

Solver::OpGRBSol::OpGRBSol(OpEnv env, Model::OpModel mdl, OpStr name)
{
	impl_ = new OpGRBSolI(env.getImpl(), mdl, name);
}

Solver::OpGRBSol::~OpGRBSol()
{

}
