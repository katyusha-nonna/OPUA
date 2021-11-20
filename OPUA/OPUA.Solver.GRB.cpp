#include "OPUA.Solver.h"
#include <Gurobi/include/gurobi_c++.h>

using namespace OPUA;

/* OPUA::Solver::OpGRBSolI */

class Solver::OpGRBSolI
	: public OpImplBase
{
protected:
	// GRB组件模型

	GRBEnv* genv_; // GRB环境变量
	GRBModel* gmdl_; // GRB模型对象
	OpStr gname_; // GRB模型名称
protected:
	// OPUA-GRB映射信息

	std::unordered_map<OpLInt, GRBVar> vardict_;
	std::unordered_map<OpLInt, GRBConstr> lcdict_;
	std::unordered_map<OpLInt, GRBQConstr> qcdict_;
	std::unordered_map<OpLInt, GRBSOS> scdict_;
	std::unordered_map<OpLInt, GRBGenConstr> nlcdict_;
	std::unordered_map<OpLInt, GRBGenConstr> modict_;

	friend class OpGRBSol;
private:
	GRBVar addGRBVar(Variable::OpVar var); // 从OPUA变量创建一个GRB变量
	char senseConvert1(Constraint::OpConSense sense); // 将OPUA约束类型转换为GRB约束类型
	int senseConvert2(Constraint::OpConSense sense); // 将OPUA约束类型转换为GRB约束类型
	GRBLinExpr addGRBLE(const Expression::OpLinExpr& expr); // 由OPUA线性表达式创建一个GRB线性表达式
	GRBQuadExpr addGRBQE(const Expression::OpQuadExpr& expr); // 由OPUA二次表达式创建一个GRB二次表达式
	GRBConstr addGRBLC(Constraint::OpLinCon con); // 从OPUA线性约束创建一个GRB线性约束
	GRBQConstr addGRBQC(Constraint::OpQuadCon con); // 从OPUA二次约束创建一个GRB二次约束
	GRBSOS addGRBSOS(Constraint::OpSOSCon con); // 从OPUASOS约束创建一个GRBSOS约束
	GRBGenConstr addGRBGen(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Max约束
	GRBGenConstr addGRBGenMax(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Max约束
	GRBGenConstr addGRBGenMin(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Max约束
	GRBGenConstr addGRBGenAbs(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Max约束
protected:
	void init(); // 初始化
	void clear(); // 清除所有组件与映射信息
	void extract(Model::OpModel mdl); // 抽取OPUA模型
	void solve(); // 求解模型
	void setConfig(OpStr key, OpBool val); // 设置配置(布尔型)
	void setConfig(OpStr key, OpLInt val); // 设置配置(整数型)
	void setConfig(OpStr key, OpFloat val); // 设置配置(浮点型)
	void setConfig(OpStr key, OpStr val); // 设置配置(字符型)
	OpFloat getValue(Variable::OpVar var); // 获取变量的解
	OpFloat getValue(const Expression::OpLinExpr& expr); // 获取表达式的解(速度较慢)
	OpFloat getValue(const Expression::OpQuadExpr& expr); // 获取表达式的解(速度较慢)
	OpFloat getValue(Objective::OpObj obj); // 获取目标函数解(速度较慢)
	OpFloat getDual(Constraint::OpLinCon con); // 获取对偶解
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
	case Expression::OpNLFunc::Unknow: // 无法处理
		break;
	case Expression::OpNLFunc::Sum: // 无法处理
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
	case Expression::OpNLFunc::Square: // 无法处理
		break;
	case Expression::OpNLFunc::Sqrt: // 无法处理
		break;
	case Expression::OpNLFunc::Pow: // 无法处理
		break;
	case Expression::OpNLFunc::Exp1: // 无法处理
		break;
	case Expression::OpNLFunc::Exp2: // 无法处理
		break;
	case Expression::OpNLFunc::Log1: // 无法处理
		break;
	case Expression::OpNLFunc::Log2: // 无法处理
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
	// 首先清除原模型
	if (genv_)
	{
		clear();
		init();
	}
	// 加载现有模型
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
