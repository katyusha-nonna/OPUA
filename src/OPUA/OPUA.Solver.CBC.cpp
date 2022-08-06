#ifdef OPUA_COMPILE_CBC
#include "OPUA.Solver.h"
#include "OPUA.Exception.h"
#include <coin/OsiClpSolverInterface.hpp>
#include <coin/CbcModel.hpp>

using namespace OPUA;
#endif

#ifdef OPUA_COMPILE_CBC
/* OPUA::Solver::OpCBCCfgCvt */

// CBC配置翻译器
class Solver::OpCBCCfgCvt
{
protected:

public:
	void init();
public:
	OpCBCCfgCvt();
};

void Solver::OpCBCCfgCvt::init()
{
#ifdef OPUA_CBC_VERSION_2108
	// 配置对应版本：2.10.8

#endif
}

Solver::OpCBCCfgCvt::OpCBCCfgCvt()
{
	init();
}

/* OPUA::Solver::OpCBCSolI */

class Solver::OpCBCSolI
	: public OpImplBase
{
protected:
	class OpCBCMdlInfo
	{
	protected:
		int nv; // 变量计数器
		int nlc; // 线性约束计数器
		bool isMip; // 是否为混整问题

		friend class Solver::OpCBCSolI;
	protected:
		OpCBCMdlInfo();
	};

	// CBC组件模型
	OsiClpSolverInterface* bmip_;
	CbcModel* bmdl_;
	OpCBCMdlInfo* binfo_;
protected:
	// OPUA-CBC映射信息

	std::unordered_map<OpLInt, int> vardict_; // CBC变量表
	std::unordered_map<OpLInt, int> lcdict_; // CBC线性约束表
	OpCBCCfgCvt cfgcvt_; // 配置翻译器

	friend class OpCBCSol;
private:
	int addCBCVar(Variable::OpVar var); // 从OPUA变量创建一个CBC变量
	int addCBCLC(Constraint::OpLinCon con); // 从OPUA线性约束创建一个CBC线性约束
	void setCBCObj(Objective::OpObj obj); // 从OPUA目标函数对CBC目标函数进行设置
protected:
	void init(); // 初始化
	void clear(); // 清除所有组件与映射信息
	void extract(Model::OpModel mdl); // 抽取OPUA模型
	void solve(); // 求解模型
	void solveFixed(); // 固定整数变量解并求解模型
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
	OpCBCSolI(OpEnvI* env);
	OpCBCSolI(OpEnvI* env, Model::OpModel mdl);
public:
	virtual ~OpCBCSolI();
};

Solver::OpCBCSolI::OpCBCMdlInfo::OpCBCMdlInfo()
	:nv(0), nlc(0), isMip(false)
{
	
}

int Solver::OpCBCSolI::addCBCVar(Variable::OpVar var)
{
	int idx(binfo_->nv++);
	double lb(Constant::IsInfinity(var.getLb()) ? -COIN_DBL_MAX : var.getLb());
	double ub(Constant::IsInfinity(var.getUb()) ? COIN_DBL_MAX : var.getUb());
	bmip_->addCol(0, nullptr, nullptr, lb, ub, 0.0, var.getName());
	switch (var.getType())
	{
	case Variable::OpVarType::Bool:
		bmip_->setInteger(idx);
		bmip_->setColBounds(idx, 0.0, 1.0);
		binfo_->isMip = true;
		break;
	case Variable::OpVarType::Int:
		bmip_->setInteger(idx);
		binfo_->isMip = true;
		break;
	case Variable::OpVarType::Con:
		break;
	case Variable::OpVarType::Sem:
		throw OpExcBase("[Solver::OpCBCSolI::addCBCVar]: Exception->can not handle Variable::OpVarType::Sem");
		break;
	default:
		throw OpExcBase("[Solver::OpCBCSolI::addCBCVar]: Exception->can not handle other variable type");
		break;
	}
	return idx;
}

int Solver::OpCBCSolI::addCBCLC(Constraint::OpLinCon con)
{
	int idx(binfo_->nlc++);
	double lb(Constant::IsInfinity(con.getLb()) ? -COIN_DBL_MAX : con.getLb());
	double ub(Constant::IsInfinity(con.getUb()) ? COIN_DBL_MAX : con.getUb());
	auto& expr(con.getExpr());
	std::vector<int> ind(expr.getSize(), -1);
	std::vector<double> val(expr.getSize(), 0);
	int count(0);
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
	{
		ind[count] = vardict_.at(iter.getVar().getIndex());
		val[count] = iter.getCoeff();
		count++;
	}
	bmip_->addRow(ind.size(), ind.data(), val.data(), lb, ub);
	bmip_->setRowName(idx, con.getName());
	return idx;
}

void Solver::OpCBCSolI::setCBCObj(Objective::OpObj obj)
{
	if (obj.getQuadExpr().getSize() > 0)
		throw OpExcBase("[Solver::OpCBCSolI::setCBCObj]: Exception->can not handle quadratic objective function");
	auto& expr(obj.getLinExpr());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		bmip_->setObjCoeff(vardict_.at(iter.getVar().getIndex()), iter.getCoeff());
	switch (obj.getSense())
	{
	case Objective::OpObjSense::Min:
		bmip_->setObjSense(1.0);
		break;
	case Objective::OpObjSense::Max:
		bmip_->setObjSense(-1.0);
		break;
	default:
		throw OpExcBase("[Solver::OpGLPKSolI::setGLPKObj]: Exception->can not handle other objective function sense");
		break;
	}
}

void Solver::OpCBCSolI::init()
{
	bmip_ = new OsiClpSolverInterface();
	bmdl_ = nullptr;
	binfo_ = new OpCBCMdlInfo();
}

void Solver::OpCBCSolI::clear()
{
	lcdict_.clear();
	vardict_.clear();
	delete binfo_;
	binfo_ = nullptr;
	delete bmdl_;
	bmdl_ = nullptr;
	delete bmip_;
	bmip_ = nullptr;
}

void Solver::OpCBCSolI::extract(Model::OpModel mdl)
{
	try
	{
		// 首先清除原模型
		clear();
		init();
		// 加载现有模型
		for (auto iter = mdl.getCBegin<Variable::OpVar>(); iter != mdl.getCEnd<Variable::OpVar>(); ++iter)
			vardict_.emplace(iter.getKey(), addCBCVar(iter.getVal()));
		for (auto iter = mdl.getCBegin<Variable::OpPSDVar>(); iter != mdl.getCEnd<Variable::OpPSDVar>(); ++iter)
			throw OpExcBase("[Solver::OpCBCSolI::extract]: Exception->can not handle Variable::OpPSDVar");
		for (auto iter = mdl.getCBegin<Constraint::OpLinCon>(); iter != mdl.getCEnd<Constraint::OpLinCon>(); ++iter)
			lcdict_.emplace(iter.getKey(), addCBCLC(iter.getVal()));
		for (auto iter = mdl.getCBegin<Constraint::OpQuadCon>(); iter != mdl.getCEnd<Constraint::OpQuadCon>(); ++iter)
			throw OpExcBase("[Solver::OpCBCSolI::extract]: Exception->can not handle Constraint::OpQuadCon");
		for (auto iter = mdl.getCBegin<Constraint::OpConicCon>(); iter != mdl.getCEnd<Constraint::OpConicCon>(); ++iter)
			throw OpExcBase("[Solver::OpCBCSolI::extract]: Exception->can not handle Constraint::OpConicCon");
		for (auto iter = mdl.getCBegin<Constraint::OpPSDCon>(); iter != mdl.getCEnd<Constraint::OpPSDCon>(); ++iter)
			throw OpExcBase("[Solver::OpCBCSolI::extract]: Exception->can not handle Constraint::OpPSDCon");
		for (auto iter = mdl.getCBegin<Constraint::OpSOSCon>(); iter != mdl.getCEnd<Constraint::OpSOSCon>(); ++iter)
			throw OpExcBase("[Solver::OpCBCSolI::extract]: Exception->can not handle Constraint::OpSOSCon");
		for (auto iter = mdl.getCBegin<Constraint::OpNLCon>(); iter != mdl.getCEnd<Constraint::OpNLCon>(); ++iter)
			throw OpExcBase("[Solver::OpCBCSolI::extract]: Exception->can not handle Constraint::OpNLCon");
		for (auto iter = mdl.getCBegin<Constraint::OpCdtCon>(); iter != mdl.getCEnd<Constraint::OpCdtCon>(); ++iter)
			throw OpExcBase("[Solver::OpCBCSolI::extract]: Exception->can not handle Constraint::OpCdtCon");
		setCBCObj(mdl.getObj());
		bmdl_ = new CbcModel(*bmip_);
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
		throw OpExcBase("[Solver::OpCBCSolI::extract]: Exception->unknow exception!");
	}
}

void Solver::OpCBCSolI::solve()
{
	bmdl_->initialSolve();
	if (binfo_->isMip)
		bmdl_->branchAndBound();
}

void Solver::OpCBCSolI::solveFixed()
{
	// 暂无实现
}

void Solver::OpCBCSolI::setParam(const OpConfig& cfg)
{
	// 暂无实现
}

OpLInt Solver::OpCBCSolI::getStatus() const
{
	OpLInt tmp(bmdl_->status());
	if (tmp < 0)
	{
		if (bmdl_->isInitialSolveProvenOptimal())
			tmp = 2; /* Optimal - 2 */
		else if (bmdl_->isInitialSolveProvenPrimalInfeasible())
			tmp = 3; /* Infeasible - 3 */
		else if (bmdl_->isInitialSolveProvenDualInfeasible())
			tmp = 4; /* Unbounded - 4 */
		else
			tmp = 0; /* Unknown - 0 */
	}
	else if (tmp == 0)
	{
		if (bmdl_->isProvenOptimal())
			tmp = 2; /* Optimal - 2 */
		else if (bmdl_->isProvenInfeasible())
			tmp = 3; /* Infeasible - 3 */
		else if (bmdl_->isProvenDualInfeasible())
			tmp = 4; /* Unbounded - 4 */
		else
			tmp = 0; /* Unknown - 0 */
	}
	else if (tmp == 1)
	{
		if (bmdl_->getSolutionCount() > 0)
			tmp = 1; /* Feasible - 1 */
		else
			tmp = 0; /* Unknown - 0 */
	}
	else
		tmp = 0; /* Unknown - 0 */
	return tmp;
}

OpFloat Solver::OpCBCSolI::getObjValue() const
{
	return bmdl_->getObjValue();
}

OpFloat Solver::OpCBCSolI::getValue(Variable::OpVar var) const
{
	return bmdl_->getColSolution()[vardict_.at(var.getIndex())];
}

OpFloat Solver::OpCBCSolI::getValue(const Expression::OpLinExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	return result;
}

OpFloat Solver::OpCBCSolI::getValue(const Expression::OpQuadExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar1()) * getValue(iter.getVar2());
	return result;
}

OpFloat Solver::OpCBCSolI::getValue(Objective::OpObj obj) const
{
	return getValue(obj.getLinExpr()) + getValue(obj.getQuadExpr());
}

OpFloat Solver::OpCBCSolI::getDual(Constraint::OpLinCon con) const
{
	return bmdl_->getRowPrice()[lcdict_.at(con.getIndex())];
}

void Solver::OpCBCSolI::write(OpStr path) const
{
	if (path.size())
	{
		bmip_->writeLp(path.c_str());
	}
	else
	{
		// 不支持输出到控制台
	}
}

OpULInt Solver::OpCBCSolI::getMemoryUsage() const
{
	return sizeof(*this);
}

Solver::OpCBCSolI::OpCBCSolI(OpEnvI* env)
	: OpImplBase('S', env),
	bmip_(nullptr),
	bmdl_(nullptr),
	binfo_(nullptr)
{

}

Solver::OpCBCSolI::OpCBCSolI(OpEnvI* env, Model::OpModel mdl)
	: OpImplBase('S', env),
	bmip_(nullptr),
	bmdl_(nullptr),
	binfo_(nullptr)
{
	extract(mdl);
}

Solver::OpCBCSolI::~OpCBCSolI()
{
	clear();
}

/* OPUA::Solver::OpCBCSol */

void Solver::OpCBCSol::extract(Model::OpModel mdl)
{
	static_cast<OpCBCSolI*>(impl_)->extract(mdl);
}

void Solver::OpCBCSol::solve()
{
	static_cast<OpCBCSolI*>(impl_)->solve();
}

void Solver::OpCBCSol::solveFixed()
{
	static_cast<OpCBCSolI*>(impl_)->solveFixed();
}

void Solver::OpCBCSol::setParam(const OpConfig& cfg)
{
	static_cast<OpCBCSolI*>(impl_)->setParam(cfg);
}

OpLInt Solver::OpCBCSol::getStatus() const
{
	return static_cast<OpCBCSolI*>(impl_)->getStatus();
}

OpFloat Solver::OpCBCSol::getObjValue() const
{
	return static_cast<OpCBCSolI*>(impl_)->getObjValue();
}

OpFloat Solver::OpCBCSol::getValue(Variable::OpVar var) const
{
	return static_cast<OpCBCSolI*>(impl_)->getValue(var);
}

OpFloat Solver::OpCBCSol::getValue(const Expression::OpLinExpr& expr) const
{
	return static_cast<OpCBCSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpCBCSol::getValue(const Expression::OpQuadExpr& expr) const
{
	return static_cast<OpCBCSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpCBCSol::getValue(Objective::OpObj obj) const
{
	return static_cast<OpCBCSolI*>(impl_)->getValue(obj);
}

OpFloat Solver::OpCBCSol::getDual(Constraint::OpLinCon con) const
{
	return static_cast<OpCBCSolI*>(impl_)->getDual(con);
}

Solver::OpCBCSolI* Solver::OpCBCSol::getImpl() const
{
	return static_cast<OpCBCSolI*>(impl_);
}

void Solver::OpCBCSol::write(OpStr path) const
{
	static_cast<OpCBCSolI*>(impl_)->write(path);
}

void Solver::OpCBCSol::release0()
{
	static_cast<OpCBCSolI*>(impl_)->release();
}

OpBool Solver::OpCBCSol::operator==(const OpCBCSol& sol) const
{
	return impl_ == sol.impl_;
}

OpBool Solver::OpCBCSol::operator!=(const OpCBCSol& sol) const
{
	return impl_ != sol.impl_;
}

Solver::OpCBCSol::OpCBCSol()
{

}

Solver::OpCBCSol::OpCBCSol(OpCBCSolI* impl)
{
	impl_ = impl;
}

Solver::OpCBCSol::OpCBCSol(OpEnv env)
{
	impl_ = new OpCBCSolI(env.getImpl());
}

Solver::OpCBCSol::OpCBCSol(OpEnv env, Model::OpModel mdl)
{
	impl_ = new OpCBCSolI(env.getImpl(), mdl);
}

Solver::OpCBCSol::~OpCBCSol()
{

}
#endif

