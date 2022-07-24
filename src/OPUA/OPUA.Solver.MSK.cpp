#include "OPUA.Solver.h"
#ifdef OPUA_COMPILE_MSK
#include <fusion.h>
#endif

using namespace OPUA;

#ifdef OPUA_COMPILE_MSK
/* OPUA::Solver::OpMSKCfgCvt */

// COPT配置翻译器
class Solver::OpMSKCfgCvt
{
protected:
	std::unordered_map<OpStr, OpStr> dpdict_;
	std::unordered_map<OpStr, OpStr> ipdict_;
	std::unordered_map<OpStr, OpStr> spdict_;
public:
	void init();
	OpStr getDoubleParam(OpStr key);
	OpStr getIntParam(OpStr key);
	OpStr getStringParam(OpStr key);
public:
	OpMSKCfgCvt();
};

void Solver::OpMSKCfgCvt::init()
{
#ifdef OPUA_MSK_VERSION_9320
	// 配置对应版本：9.3.20

#endif
}

OpStr Solver::OpMSKCfgCvt::getDoubleParam(OpStr key)
{
	return dpdict_.at(key);
}

OpStr Solver::OpMSKCfgCvt::getIntParam(OpStr key)
{
	return ipdict_.at(key);
}

OpStr Solver::OpMSKCfgCvt::getStringParam(OpStr key)
{
	return spdict_.at(key);
}

Solver::OpMSKCfgCvt::OpMSKCfgCvt()
{
	init();
}

/* OPUA::Solver::OpMSKSolI */

class Solver::OpMSKSolI
	: public OpImplBase
{
protected:
	// MSK组件模型
	mosek::fusion::Model::t mmdl_; /*MSK模型对象*/
protected:
	// OPUA-MSK映射信息
	std::unordered_map<OpLInt, mosek::fusion::Variable::t> vardict_; // MSK变量表
	std::unordered_map<OpLInt, mosek::fusion::Constraint::t> lcdict_; // MSK线性约束表
	std::unordered_map<OpLInt, mosek::fusion::Constraint::t> qcdict_; // MSK二次约束表
	OpMSKCfgCvt cfgcvt_; // 配置翻译器

	friend class OpMSKSol;
private:
	mosek::fusion::Variable::t addMSKVar(Variable::OpVar var); // 从OPUA自动变量创建一个MSK变量
	mosek::fusion::Variable::t addMSKConVar(Variable::OpVar var); // 从OPUA变量创建一个MSK连续变量
	mosek::fusion::Variable::t addMSKIntVar(Variable::OpVar var); // 从OPUA变量创建一个MSK整数变量
	mosek::fusion::Variable::t addMSKBoolVar(Variable::OpVar var); // 从OPUA变量创建一个MSK布尔变量
	mosek::fusion::Expression::t addMSKLE(const Expression::OpLinExpr& expr); // 由OPUA线性表达式创建一个MSK线性表达式
	mosek::fusion::Expression::t addMSKQE(const Expression::OpQuadExpr& expr); // 由OPUA二次表达式创建一个MSK二次表达式
	mosek::fusion::Constraint::t addMSKLC(Constraint::OpLinCon con); // 从OPUA线性约束创建一个MSK线性约束

protected:
	void init(); // 初始化
	void clear(); // 清除所有组件与映射信息
	void extract(Model::OpModel mdl); // 抽取OPUA模型
	void solve(); // 求解模型
	void solveFixed(); // 固定整数变量解并执行求解
	void setParam(const OpConfig& cfg); // 设置配置
	OpLInt getStatus() const; // 获取求解状态
	OpFloat getObjValue() const; // 获取目标函数解
	OpFloat getValue(Variable::OpVar var) const; // 获取变量的解
	OpFloat getValue(const Expression::OpLinExpr& expr) const; // 获取表达式的解(速度较慢)
	OpFloat getValue(const Expression::OpQuadExpr& expr) const; // 获取表达式的解(速度较慢)
	OpFloat getValue(Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
	OpFloat getDual(Constraint::OpLinCon con) const; // 获取对偶解
	void write(OpStr path) const; // 将模型写入文件
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
protected:
	OpMSKSolI(OpEnvI* env);
	OpMSKSolI(OpEnvI* env, Model::OpModel mdl);
public:
	virtual ~OpMSKSolI();
};

mosek::fusion::Variable::t Solver::OpMSKSolI::addMSKVar(Variable::OpVar var)
{
	mosek::fusion::Variable::t mvar(nullptr);
	switch (var.getType())
	{
	case OPUA::Variable::OpVarType::Bool:
		mvar = addMSKBoolVar(var);
		break;
	case OPUA::Variable::OpVarType::Int:
		mvar = addMSKIntVar(var);
		break;
	case OPUA::Variable::OpVarType::Con:
		mvar = addMSKConVar(var);
		break;
	default:
		break;
	}
	return mvar;
}

mosek::fusion::Variable::t Solver::OpMSKSolI::addMSKConVar(Variable::OpVar var)
{
	mosek::fusion::Variable::t mvar(nullptr);
	auto lb(Constant::IsInfinity(var.getLb()) ? false : true);
	auto ub(Constant::IsInfinity(var.getUb()) ? false : true);
	if (lb && ub)
		mvar = mmdl_->variable(var.getName(), 1, mosek::fusion::Domain::inRange(var.getLb(), var.getUb()));
	else if (lb && !ub)
		mvar = mmdl_->variable(var.getName(), 1, mosek::fusion::Domain::greaterThan(var.getLb()));
	else if (!lb && ub)
		mvar = mmdl_->variable(var.getName(), 1, mosek::fusion::Domain::lessThan(var.getUb()));
	else
		mvar = mmdl_->variable(var.getName(), 1, mosek::fusion::Domain::unbounded());
	return mvar;
}

mosek::fusion::Variable::t Solver::OpMSKSolI::addMSKIntVar(Variable::OpVar var)
{
	mosek::fusion::Variable::t mvar(nullptr);
	auto lb(Constant::IsInfinity(var.getLb()) ? false : true);
	auto ub(Constant::IsInfinity(var.getUb()) ? false : true);
	if (lb && ub)
		mvar = mmdl_->variable(var.getName(), 1, mosek::fusion::Domain::integral(mosek::fusion::Domain::inRange(var.getLb(), var.getUb())));
	else if (lb && !ub)
		mvar = mmdl_->variable(var.getName(), 1, mosek::fusion::Domain::integral(mosek::fusion::Domain::greaterThan(var.getLb())));
	else if (!lb && ub)
		mvar = mmdl_->variable(var.getName(), 1, mosek::fusion::Domain::integral(mosek::fusion::Domain::lessThan(var.getUb())));
	else
		mvar = mmdl_->variable(var.getName(), 1, mosek::fusion::Domain::integral(mosek::fusion::Domain::unbounded()));
	return mvar;
}

mosek::fusion::Variable::t Solver::OpMSKSolI::addMSKBoolVar(Variable::OpVar var)
{
	return mmdl_->variable(var.getName(), 1, mosek::fusion::Domain::binary());
}

mosek::fusion::Expression::t Solver::OpMSKSolI::addMSKLE(const Expression::OpLinExpr& expr)
{
	auto coeffs = monty::new_array_ptr<double, 1>(expr.getSize());
	auto vars = monty::new_array_ptr<mosek::fusion::Expression::t, 1>(expr.getSize());
	decltype(vars)::element_type::index_type count(0);
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
	{
		(*coeffs)[count] = iter.getCoeff();
		(*vars)[count] = vardict_.at(iter.getVar().getIndex());
		count++;
	}
	return mosek::fusion::Expr::add(
		mosek::fusion::Expr::dot(mosek::fusion::Expr::vstack(vars), coeffs),
		mosek::fusion::Expr::constTerm(expr.getConstant()));
}

mosek::fusion::Expression::t Solver::OpMSKSolI::addMSKQE(const Expression::OpQuadExpr& expr)
{
	return mosek::fusion::Expression::t(nullptr);
}

mosek::fusion::Constraint::t Solver::OpMSKSolI::addMSKLC(Constraint::OpLinCon con)
{
	mosek::fusion::Constraint::t mcon(nullptr);
	auto lb(Constant::IsInfinity(con.getLb()) ? false : true);
	auto ub(Constant::IsInfinity(con.getUb()) ? false : true);
	if (lb && ub)
		mcon = mmdl_->constraint(addMSKLE(con.getExpr()), mosek::fusion::Domain::inRange(con.getLb(), con.getUb()));
	else if (lb && !ub)
		mcon = mmdl_->constraint(addMSKLE(con.getExpr()), mosek::fusion::Domain::greaterThan(con.getLb()));
	else if (!lb && ub)
		mcon = mmdl_->constraint(addMSKLE(con.getExpr()), mosek::fusion::Domain::lessThan(con.getUb()));
	else
		mcon = mmdl_->constraint(addMSKLE(con.getExpr()), mosek::fusion::Domain::unbounded());
	return mcon;
}

void Solver::OpMSKSolI::init()
{
	mmdl_ = new mosek::fusion::Model("");
}

void Solver::OpMSKSolI::clear()
{
	qcdict_.clear();
	lcdict_.clear();
	vardict_.clear();
	if (mmdl_.get())
		mmdl_->dispose();
	mmdl_ = nullptr;
}

void Solver::OpMSKSolI::extract(Model::OpModel mdl)
{
	// 首先清除原模型
	clear();
	init();
	// 加载现有模型
	for (auto iter = mdl.getCBegin<Variable::OpVar>(); iter != mdl.getCEnd<Variable::OpVar>(); ++iter)
		vardict_.emplace(iter.getKey(), addMSKVar(iter.getVal()));
	for (auto iter = mdl.getCBegin<Constraint::OpLinCon>(); iter != mdl.getCEnd<Constraint::OpLinCon>(); ++iter)
		lcdict_.emplace(iter.getKey(), addMSKLC(iter.getVal()));
	switch (mdl.getObj().getSense())
	{
	case Objective::OpObjSense::Min:
		mmdl_->objective(mosek::fusion::ObjectiveSense::Minimize, addMSKLE(mdl.getObj().getLinExpr()));
		break;
	case Objective::OpObjSense::Max:
		mmdl_->objective(mosek::fusion::ObjectiveSense::Maximize, addMSKLE(mdl.getObj().getLinExpr()));
		break;
	default:
		break;
	}
}

void Solver::OpMSKSolI::solve()
{
	mmdl_->solve();
}

void Solver::OpMSKSolI::solveFixed()
{
	// 暂无实现
}

void Solver::OpMSKSolI::setParam(const OpConfig& cfg)
{
	if (mmdl_.get())
	{
		for (auto iter = cfg.getCBegin<OpInt>("OPUA.MSK"); iter != cfg.getCEnd<OpInt>("OPUA.MSK"); ++iter)
			if (iter.ok())
				mmdl_->setSolverParam(cfgcvt_.getIntParam(iter.getKey()), iter.getVal());
		for (auto iter = cfg.getCBegin<OpFloat>("OPUA.MSK"); iter != cfg.getCEnd<OpFloat>("OPUA.MSK"); ++iter)
			if (iter.ok())
				mmdl_->setSolverParam(cfgcvt_.getDoubleParam(iter.getKey()), iter.getVal());
		for (auto iter = cfg.getCBegin<OpStr>("OPUA.MSK"); iter != cfg.getCEnd<OpStr>("OPUA.MSK"); ++iter)
			if (iter.ok())
				mmdl_->setSolverParam(cfgcvt_.getStringParam(iter.getKey()), iter.getVal());
	}
}

OpLInt Solver::OpMSKSolI::getStatus() const
{
	return static_cast<OpLInt>(mmdl_->getPrimalSolutionStatus());
}

OpFloat Solver::OpMSKSolI::getObjValue() const
{
	return mmdl_->primalObjValue();
}

OpFloat Solver::OpMSKSolI::getValue(Variable::OpVar var) const
{
	return (*vardict_.at(var.getIndex())->level())[0];
}

OpFloat Solver::OpMSKSolI::getValue(const Expression::OpLinExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	return result;
}

OpFloat Solver::OpMSKSolI::getValue(const Expression::OpQuadExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar1()) * getValue(iter.getVar2());
	return result;
}

OpFloat Solver::OpMSKSolI::getValue(Objective::OpObj obj) const
{
	// 目前只包含线性项
	return getValue(obj.getLinExpr());
}

OpFloat Solver::OpMSKSolI::getDual(Constraint::OpLinCon con) const
{
	return (*lcdict_.at(con.getIndex())->dual())[0];
}

void Solver::OpMSKSolI::write(OpStr path) const
{
	if (path.size())
	{
		mmdl_->writeTask(path);
	}
	else
	{
		// 不支持输出到控制台
	}
}

OpULInt Solver::OpMSKSolI::getMemoryUsage() const
{
	return sizeof(*this);
}

Solver::OpMSKSolI::OpMSKSolI(OpEnvI* env)
	: OpImplBase('S', env),
	mmdl_(nullptr)
{

}

Solver::OpMSKSolI::OpMSKSolI(OpEnvI* env, Model::OpModel mdl)
	: OpImplBase('S', env),
	mmdl_(nullptr)
{
	extract(mdl);
}

Solver::OpMSKSolI::~OpMSKSolI()
{
	clear();
}

/* OPUA::Solver::OpMSKSol */

void Solver::OpMSKSol::extract(Model::OpModel mdl)
{
	static_cast<Solver::OpMSKSolI*>(impl_)->extract(mdl);
}

void Solver::OpMSKSol::solve()
{
	static_cast<Solver::OpMSKSolI*>(impl_)->solve();
}

void Solver::OpMSKSol::solveFixed()
{
	static_cast<Solver::OpMSKSolI*>(impl_)->solveFixed();
}

void Solver::OpMSKSol::setParam(const OpConfig& cfg)
{
	static_cast<Solver::OpMSKSolI*>(impl_)->setParam(cfg);
}

OpLInt Solver::OpMSKSol::getStatus() const
{
	return static_cast<Solver::OpMSKSolI*>(impl_)->getStatus();
}

OpFloat Solver::OpMSKSol::getObjValue() const
{
	return static_cast<Solver::OpMSKSolI*>(impl_)->getObjValue();
}

OpFloat Solver::OpMSKSol::getValue(Variable::OpVar var) const
{
	return static_cast<Solver::OpMSKSolI*>(impl_)->getValue(var);
}

OpFloat Solver::OpMSKSol::getValue(const Expression::OpLinExpr& expr) const
{
	return static_cast<Solver::OpMSKSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpMSKSol::getValue(const Expression::OpQuadExpr& expr) const
{
	return static_cast<Solver::OpMSKSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpMSKSol::getValue(Objective::OpObj obj) const
{
	return static_cast<Solver::OpMSKSolI*>(impl_)->getValue(obj);
}

OpFloat Solver::OpMSKSol::getDual(Constraint::OpLinCon con) const
{
	return static_cast<Solver::OpMSKSolI*>(impl_)->getDual(con);
}

Solver::OpMSKSolI* Solver::OpMSKSol::getImpl() const
{
	return static_cast<Solver::OpMSKSolI*>(impl_);
}

void Solver::OpMSKSol::write(OpStr path) const
{
	static_cast<Solver::OpMSKSolI*>(impl_)->write(path);
}

void Solver::OpMSKSol::release0()
{
	static_cast<Solver::OpMSKSolI*>(impl_)->release();
}

OpBool Solver::OpMSKSol::operator==(const OpMSKSol& sol) const
{
	return impl_ == sol.impl_;
}

OpBool Solver::OpMSKSol::operator!=(const OpMSKSol& sol) const
{
	return impl_ != sol.impl_;
}

Solver::OpMSKSol::OpMSKSol()
{

}

Solver::OpMSKSol::OpMSKSol(OpMSKSolI* impl)
{
	impl_ = impl;
}

Solver::OpMSKSol::OpMSKSol(OpEnv env)
{
	impl_ = new OPUA::Solver::OpMSKSolI(env.getImpl());
}

Solver::OpMSKSol::OpMSKSol(OpEnv env, Model::OpModel mdl)
{
	impl_ = new OPUA::Solver::OpMSKSolI(env.getImpl(), mdl);
}

Solver::OpMSKSol::~OpMSKSol()
{

}

#endif

