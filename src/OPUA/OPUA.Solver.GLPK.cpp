#ifdef OPUA_COMPILE_GLPK
#include "OPUA.Solver.h"
#include "OPUA.Exception.h"
#include <glpk.h>

using namespace OPUA;
#endif

#ifdef OPUA_COMPILE_GLPK
/* OPUA::Solver::OpGLPKCfgCvt */

// GLPK配置翻译器
class Solver::OpGLPKCfgCvt
{
protected:

public:
	void init();
public:
	OpGLPKCfgCvt();
};

void Solver::OpGLPKCfgCvt::init()
{
#ifdef OPUA_GLPK_VERSION_50
	// 配置对应版本：5.0

#endif
}

Solver::OpGLPKCfgCvt::OpGLPKCfgCvt()
{
	init();
}

/* OPUA::Solver::OpGLPKSolI */

class Solver::OpGLPKSolI
	: public OpImplBase
{
protected:
	enum class OpGLPKProbSolType
	{
		Simplex,
		Interior,
		MIP
	};

	enum class OpGLPKMIPRootSolType
	{
		Simplex,
		Interior
	};

	// GLPK组件模型
	glp_prob* kmdl_;
	glp_smcp* kcfgs_;
	glp_iptcp* kcfgi_;
	glp_iocp* kcfgm_;
	OpGLPKProbSolType ktype_;
	OpGLPKMIPRootSolType krtype_;
protected:
	// OPUA-GLPK映射信息

	std::unordered_map<OpLInt, int> vardict_; // GLPK变量表
	std::unordered_map<OpLInt, int> lcdict_; // GLPK线性约束表
	OpGLPKCfgCvt cfgcvt_; // 配置翻译器

	friend class OpGLPKSol;
private:
	int typeConvert1(Variable::OpVarType type); // 将OPUA变量类型转换为GLPK约束类型
	int typeConvert2(OpFloat lb0, OpFloat ub0); // 将OPUA边界类型转换为GLPK边界类型
	int addGLPKVar(Variable::OpVar var); // 从OPUA变量创建一个GLPK变量
	int addGLPKLC(Constraint::OpLinCon con); // 从OPUA线性约束创建一个GLPK线性约束
	void setGLPKObj(Objective::OpObj obj); // 从OPUA目标函数对GLPK目标函数进行设置
protected:
	void init(); // 初始化
	void clear(); // 清除所有组件与映射信息
	void extract(Model::OpModel mdl); // 抽取OPUA模型
	void solveRoot(); // 求解根节点模型(不对外开放)
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
	OpGLPKSolI(OpEnvI* env);
	OpGLPKSolI(OpEnvI* env, Model::OpModel mdl);
public:
	virtual ~OpGLPKSolI();
};

int Solver::OpGLPKSolI::typeConvert1(Variable::OpVarType type)
{
	auto tmp(GLP_CV);
	switch (type)
	{
	case Variable::OpVarType::Bool:
		tmp = GLP_BV;
		break;
	case Variable::OpVarType::Int:
		tmp = GLP_IV;
		break;
	case Variable::OpVarType::Con:
		tmp = GLP_CV;
		break;
	case Variable::OpVarType::Sem:
		throw OpExcBase("[Solver::OpGLPKSolI::typeConvert1]: Exception->can not handle Variable::OpVarType::Sem");
		break;
	default:
		throw OpExcBase("[Solver::OpGLPKSolI::typeConvert1]: Exception->can not handle other variable type");
		break;
	}
	return tmp;
}

int Solver::OpGLPKSolI::typeConvert2(OpFloat lb0, OpFloat ub0)
{
	auto tmp(GLP_FR);
	auto lb(Constant::IsInfinity(lb0) ? false : true);
	auto ub(Constant::IsInfinity(ub0) ? false : true);
	if (lb && ub)
	{
		if (Constant::IsEqual(lb0, ub0))
			tmp = GLP_FX;
		else
			tmp = GLP_DB;
		//tmp = GLP_DB;
	}	
	else if (lb && !ub)
		tmp = GLP_LO;
	else if (!lb && ub)
		tmp = GLP_UP;
	else
		tmp = GLP_FR;
	return tmp;
}

int Solver::OpGLPKSolI::addGLPKVar(Variable::OpVar var)
{
	auto idx(glp_add_cols(kmdl_, 1));
	auto vtype(typeConvert1(var.getType()));
	glp_set_col_bnds(kmdl_, idx, typeConvert2(var.getLb(), var.getUb()), var.getLb(), var.getUb());
	glp_set_col_kind(kmdl_, idx, vtype);
	glp_set_col_name(kmdl_, idx, var.getName().c_str());
	if (vtype == GLP_BV || vtype == GLP_IV)
		ktype_ = OpGLPKProbSolType::MIP;
	return idx;
}

int Solver::OpGLPKSolI::addGLPKLC(Constraint::OpLinCon con)
{
	auto idx(glp_add_rows(kmdl_, 1));
	auto& expr(con.getExpr());
	std::vector<int> ind(1 + expr.getSize(), -1);
	std::vector<double> val(1 + expr.getSize(), 0);
	int count(1);
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
	{
		ind[count] = vardict_.at(iter.getVar().getIndex());
		val[count] = iter.getCoeff();
		count++;
	}
	glp_set_mat_row(kmdl_, idx, expr.getSize(), ind.data(), val.data());
	glp_set_row_bnds(kmdl_, idx, typeConvert2(con.getLb(), con.getUb()), con.getLb(), con.getUb());
	glp_set_row_name(kmdl_, idx, con.getName().c_str());
	return idx;
}

void Solver::OpGLPKSolI::setGLPKObj(Objective::OpObj obj)
{
	if (obj.getQuadExpr().getSize() > 0)
		throw OpExcBase("[Solver::OpGLPKSolI::setGLPKObj]: Exception->can not handle quadratic objective function");
	auto& expr(obj.getLinExpr());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		glp_set_obj_coef(kmdl_, vardict_.at(iter.getVar().getIndex()), iter.getCoeff());
	switch (obj.getSense())
	{
	case Objective::OpObjSense::Min:
		glp_set_obj_dir(kmdl_, GLP_MIN);
		break;
	case Objective::OpObjSense::Max:
		glp_set_obj_dir(kmdl_, GLP_MAX);
		break;
	default:
		throw OpExcBase("[Solver::OpGLPKSolI::setGLPKObj]: Exception->can not handle other objective function sense");
		break;
	}
}

void Solver::OpGLPKSolI::init()
{
	kmdl_ = glp_create_prob();
	kcfgs_ = new glp_smcp();
	glp_init_smcp(kcfgs_);
	kcfgi_ = new glp_iptcp();
	glp_init_iptcp(kcfgi_);
	kcfgm_ = new glp_iocp();
	glp_init_iocp(kcfgm_);
	ktype_ = OpGLPKProbSolType::Simplex;
	krtype_ = OpGLPKMIPRootSolType::Simplex;
}

void Solver::OpGLPKSolI::clear()
{
	lcdict_.clear();
	vardict_.clear();
	if (kmdl_)
		glp_delete_prob(kmdl_);
	kmdl_ = nullptr;
	delete kcfgs_;
	kcfgs_ = nullptr;
	delete kcfgi_;
	kcfgi_ = nullptr;
	delete kcfgm_;
	kcfgm_ = nullptr;
}

void Solver::OpGLPKSolI::extract(Model::OpModel mdl)
{
	try
	{
		// 首先清除原模型
		clear();
		init();
		// 加载现有模型
		for (auto iter = mdl.getCBegin<Variable::OpVar>(); iter != mdl.getCEnd<Variable::OpVar>(); ++iter)
			vardict_.emplace(iter.getKey(), addGLPKVar(iter.getVal()));
		for (auto iter = mdl.getCBegin<Variable::OpPSDVar>(); iter != mdl.getCEnd<Variable::OpPSDVar>(); ++iter)
			throw OpExcBase("[Solver::OpGLPKSolI::extract]: Exception->can not handle Variable::OpPSDVar");
		for (auto iter = mdl.getCBegin<Constraint::OpLinCon>(); iter != mdl.getCEnd<Constraint::OpLinCon>(); ++iter)
			lcdict_.emplace(iter.getKey(), addGLPKLC(iter.getVal()));
		for (auto iter = mdl.getCBegin<Constraint::OpQuadCon>(); iter != mdl.getCEnd<Constraint::OpQuadCon>(); ++iter)
			throw OpExcBase("[Solver::OpGLPKSolI::extract]: Exception->can not handle Constraint::OpQuadCon");
		for (auto iter = mdl.getCBegin<Constraint::OpConicCon>(); iter != mdl.getCEnd<Constraint::OpConicCon>(); ++iter)
			throw OpExcBase("[Solver::OpGLPKSolI::extract]: Exception->can not handle Constraint::OpConicCon");
		for (auto iter = mdl.getCBegin<Constraint::OpPSDCon>(); iter != mdl.getCEnd<Constraint::OpPSDCon>(); ++iter)
			throw OpExcBase("[Solver::OpGLPKSolI::extract]: Exception->can not handle Constraint::OpPSDCon");
		for (auto iter = mdl.getCBegin<Constraint::OpSOSCon>(); iter != mdl.getCEnd<Constraint::OpSOSCon>(); ++iter)
			throw OpExcBase("[Solver::OpGLPKSolI::extract]: Exception->can not handle Constraint::OpSOSCon");
		for (auto iter = mdl.getCBegin<Constraint::OpNLCon>(); iter != mdl.getCEnd<Constraint::OpNLCon>(); ++iter)
			throw OpExcBase("[Solver::OpGLPKSolI::extract]: Exception->can not handle Constraint::OpNLCon");
		for (auto iter = mdl.getCBegin<Constraint::OpCdtCon>(); iter != mdl.getCEnd<Constraint::OpCdtCon>(); ++iter)
			throw OpExcBase("[Solver::OpGLPKSolI::extract]: Exception->can not handle Constraint::OpCdtCon");
		setGLPKObj(mdl.getObj());
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
		throw OpExcBase("[Solver::OpGLPKSolI::extract]: Exception->unknow exception!");
	}
}

void Solver::OpGLPKSolI::solveRoot()
{
	switch (krtype_)
	{
	case OpGLPKMIPRootSolType::Simplex:
		glp_simplex(kmdl_, kcfgs_);
		break;
	case OpGLPKMIPRootSolType::Interior:
		glp_interior(kmdl_, kcfgi_);
		break;
	default:
		break;
	}
}

void Solver::OpGLPKSolI::solve()
{
	switch (ktype_)
	{
	case OpGLPKProbSolType::Simplex:
		glp_simplex(kmdl_, kcfgs_);
		break;
	case OpGLPKProbSolType::Interior:
		glp_interior(kmdl_, kcfgi_);
		break;
	case OpGLPKProbSolType::MIP:
		solveRoot();
		glp_intopt(kmdl_, kcfgm_);
		break;
	default:
		break;
	}
}

void Solver::OpGLPKSolI::solveFixed()
{
	// 暂无实现
}

void Solver::OpGLPKSolI::setParam(const OpConfig& cfg)
{

}

OpLInt Solver::OpGLPKSolI::getStatus() const
{
	auto result(GLP_UNDEF);
	switch (ktype_)
	{
	case OpGLPKProbSolType::Simplex:
		result = glp_get_status(kmdl_);
		break;
	case OpGLPKProbSolType::Interior:
		result = glp_ipt_status(kmdl_);
		break;
	case OpGLPKProbSolType::MIP:
		result = glp_mip_status(kmdl_);
		break;
	default:
		break;
	}
	return result;
}

OpFloat Solver::OpGLPKSolI::getObjValue() const
{
	auto result(Constant::NaN);
	switch (ktype_)
	{
	case OpGLPKProbSolType::Simplex:
		result = glp_get_obj_val(kmdl_);
		break;
	case OpGLPKProbSolType::Interior:
		result = glp_ipt_obj_val(kmdl_);
		break;
	case OpGLPKProbSolType::MIP:
		result = glp_mip_obj_val(kmdl_);
		break;
	default:
		break;
	}
	return result;
}

OpFloat Solver::OpGLPKSolI::getValue(Variable::OpVar var) const
{
	auto result(Constant::NaN);
	switch (ktype_)
	{
	case OpGLPKProbSolType::Simplex:
		result = glp_get_col_prim(kmdl_, vardict_.at(var.getIndex()));
		break;
	case OpGLPKProbSolType::Interior:
		result = glp_ipt_col_prim(kmdl_, vardict_.at(var.getIndex()));
		break;
	case OpGLPKProbSolType::MIP:
		result = glp_mip_col_val(kmdl_, vardict_.at(var.getIndex()));
		break;
	default:
		break;
	}
	return result;
}

OpFloat Solver::OpGLPKSolI::getValue(const Expression::OpLinExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	return result;
}

OpFloat Solver::OpGLPKSolI::getValue(const Expression::OpQuadExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar1()) * getValue(iter.getVar2());
	return result;
}

OpFloat Solver::OpGLPKSolI::getValue(Objective::OpObj obj) const
{
	return getValue(obj.getLinExpr()) + getValue(obj.getQuadExpr());
}

OpFloat Solver::OpGLPKSolI::getDual(Constraint::OpLinCon con) const
{
	auto result(Constant::NaN);
	switch (ktype_)
	{
	case OpGLPKProbSolType::Simplex:
		result = glp_get_row_dual(kmdl_, lcdict_.at(con.getIndex()));
		break;
	case OpGLPKProbSolType::Interior:
		result = glp_ipt_row_dual(kmdl_, lcdict_.at(con.getIndex()));
		break;
	case OpGLPKProbSolType::MIP:
		break;
	default:
		break;
	}
	return result;
}

void Solver::OpGLPKSolI::write(OpStr path) const
{
	if (path.size())
	{
		glp_write_prob(kmdl_, 0, path.c_str());
	}
	else
	{
		// 不支持输出到控制台
	}
}

OpULInt Solver::OpGLPKSolI::getMemoryUsage() const
{
	return sizeof(*this);
}

Solver::OpGLPKSolI::OpGLPKSolI(OpEnvI* env)
	: OpImplBase('S', env),
	kmdl_(nullptr),
	kcfgs_(nullptr),
	kcfgi_(nullptr),
	kcfgm_(nullptr),
	ktype_(OpGLPKProbSolType::Simplex),
	krtype_(OpGLPKMIPRootSolType::Simplex)
{

}

Solver::OpGLPKSolI::OpGLPKSolI(OpEnvI* env, Model::OpModel mdl)
	: OpImplBase('S', env),
	kmdl_(nullptr),
	kcfgs_(nullptr),
	kcfgi_(nullptr),
	kcfgm_(nullptr),
	ktype_(OpGLPKProbSolType::Simplex),
	krtype_(OpGLPKMIPRootSolType::Simplex)
{
	extract(mdl);
}

Solver::OpGLPKSolI::~OpGLPKSolI()
{
	clear();
}

/* OPUA::Solver::OpGLPKSol */

void Solver::OpGLPKSol::extract(Model::OpModel mdl)
{
	static_cast<OpGLPKSolI*>(impl_)->extract(mdl);
}

void Solver::OpGLPKSol::solve()
{
	static_cast<OpGLPKSolI*>(impl_)->solve();
}

void Solver::OpGLPKSol::solveFixed()
{
	static_cast<OpGLPKSolI*>(impl_)->solveFixed();
}

void Solver::OpGLPKSol::setParam(const OpConfig& cfg)
{
	static_cast<OpGLPKSolI*>(impl_)->setParam(cfg);
}

OpLInt Solver::OpGLPKSol::getStatus() const
{
	return static_cast<OpGLPKSolI*>(impl_)->getStatus();
}

OpFloat Solver::OpGLPKSol::getObjValue() const
{
	return static_cast<OpGLPKSolI*>(impl_)->getObjValue();
}

OpFloat Solver::OpGLPKSol::getValue(Variable::OpVar var) const
{
	return static_cast<OpGLPKSolI*>(impl_)->getValue(var);
}

OpFloat Solver::OpGLPKSol::getValue(const Expression::OpLinExpr& expr) const
{
	return static_cast<OpGLPKSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpGLPKSol::getValue(const Expression::OpQuadExpr& expr) const
{
	return static_cast<OpGLPKSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpGLPKSol::getValue(Objective::OpObj obj) const
{
	return static_cast<OpGLPKSolI*>(impl_)->getValue(obj);
}

OpFloat Solver::OpGLPKSol::getDual(Constraint::OpLinCon con) const
{
	return static_cast<OpGLPKSolI*>(impl_)->getDual(con);
}

Solver::OpGLPKSolI* Solver::OpGLPKSol::getImpl() const
{
	return static_cast<OpGLPKSolI*>(impl_);
}

void Solver::OpGLPKSol::write(OpStr path) const
{
	static_cast<OpGLPKSolI*>(impl_)->write(path);
}

void Solver::OpGLPKSol::release0()
{
	static_cast<OpGLPKSolI*>(impl_)->release();
}

OpBool Solver::OpGLPKSol::operator==(const OpGLPKSol& sol) const
{
	return impl_ == sol.impl_;
}

OpBool Solver::OpGLPKSol::operator!=(const OpGLPKSol& sol) const
{
	return impl_ != sol.impl_;
}

Solver::OpGLPKSol::OpGLPKSol()
{

}

Solver::OpGLPKSol::OpGLPKSol(OpGLPKSolI* impl)
{
	impl_ = impl;
}

Solver::OpGLPKSol::OpGLPKSol(OpEnv env)
{
	impl_ = new OpGLPKSolI(env.getImpl());
}

Solver::OpGLPKSol::OpGLPKSol(OpEnv env, Model::OpModel mdl)
{
	impl_ = new OpGLPKSolI(env.getImpl(), mdl);
}

Solver::OpGLPKSol::~OpGLPKSol()
{

}
#endif
