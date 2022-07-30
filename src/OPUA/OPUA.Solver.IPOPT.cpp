#ifdef OPUA_COMPILE_IPOPT
#include "OPUA.Solver.h"
#include "OPUA.Exception.h"
#include <cppad/ipopt/solve.hpp>
#include <functional>

using namespace OPUA;
#endif

#ifdef OPUA_COMPILE_IPOPT
/* OPUA::Solver::OpIPOPTCfgCvt */

// IPOPT配置翻译器
class Solver::OpIPOPTCfgCvt
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
	OpIPOPTCfgCvt();
};

void Solver::OpIPOPTCfgCvt::init()
{
#ifdef OPUA_IPOPT_VERSION_3147
	// 配置对应版本：3.14.7
#endif
}

OpStr Solver::OpIPOPTCfgCvt::getDoubleParam(OpStr key)
{
	return dpdict_.at(key);
}

OpStr Solver::OpIPOPTCfgCvt::getIntParam(OpStr key)
{
	return ipdict_.at(key);
}

OpStr Solver::OpIPOPTCfgCvt::getStringParam(OpStr key)
{
	return spdict_.at(key);
}

Solver::OpIPOPTCfgCvt::OpIPOPTCfgCvt()
{

}

/* OPUA::Solver::OpIPOPTSolI */

class Solver::OpIPOPTSolI
	: public OpImplBase
{
protected:
	typedef CPPAD_TESTVECTOR(CppAD::AD<double>) ADvector;
	typedef CPPAD_TESTVECTOR(double) Dvector;

	class OpIPOPTFGEval
	{
	public:
		typedef CPPAD_TESTVECTOR(CppAD::AD<double>) ADvector;
		std::function<void(ADvector&, const ADvector&)> handle;
		void operator()(ADvector& f, const ADvector& x)
		{
			handle(f, x);
			return;
		}
		OpIPOPTFGEval() :handle(nullptr) {}
	};

	// CppAD组件
	bool* dir_; /*优化方向(min为true，max为false)*/
	size_t* nx_; /*变量数目*/
	size_t* ng_; /*约束数目*/
	Dvector* xi_; /*变量初值*/
	Dvector* xl_; /*变量下界*/
	Dvector* xu_; /*变量上界*/
	Dvector* gl_; /*约束下界*/
	Dvector* gu_; /*约束上界*/
	std::string* opts_; /*优化参数*/
	OpIPOPTFGEval* fg_; /*目标函数及约束函数*/
	CppAD::ipopt::solve_result<Dvector>* sol_; /*最优解*/
protected:
	// OPUA-IPOPT映射组件
	std::unordered_map<OpLInt, size_t> varidxs_; /*IPOPT变量表*/
	std::unordered_map<OpLInt, size_t> lcidxs_; /*IPOPT线性约束表*/
	std::unordered_map<OpLInt, size_t> qcidxs_; /*IPOPT二次约束表*/
	std::unordered_map<OpLInt, size_t> nlcidxs_; /*IPOPT非线性约束表*/
	OpIPOPTCfgCvt cfgcvt_; // 配置翻译器

	friend class OpIPOPTSol;
private:

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
	OpIPOPTSolI(OpEnvI* env);
	OpIPOPTSolI(OpEnvI* env, Model::OpModel mdl);
public:
	virtual ~OpIPOPTSolI();
};

void Solver::OpIPOPTSolI::init()
{
	dir_ = new bool(true);
	nx_ = new size_t(0);
	ng_ = new size_t(0);
	xi_ = new Dvector();
	xl_ = new Dvector();
	xu_ = new Dvector();
	gl_ = new Dvector();
	gu_ = new Dvector();
	opts_ = new std::string();
	fg_ = new OpIPOPTFGEval();
	sol_ = new CppAD::ipopt::solve_result<Dvector>();
}

void Solver::OpIPOPTSolI::clear()
{
	nlcidxs_.clear();
	qcidxs_.clear();
	lcidxs_.clear();
	varidxs_.clear();
	delete sol_;
	sol_ = nullptr;
	delete fg_;
	fg_ = nullptr;
	delete opts_;
	opts_ = nullptr;
	delete gu_;
	gu_ = nullptr;
	delete gl_;
	gl_ = nullptr;
	delete xu_;
	xu_ = nullptr;
	delete xl_;
	xl_ = nullptr;
	delete xi_;
	xi_ = nullptr;
	delete ng_;
	ng_ = nullptr;
	delete nx_;
	nx_ = nullptr;
	delete dir_;
	dir_ = nullptr;
}

void Solver::OpIPOPTSolI::extract(Model::OpModel mdl)
{
	try
	{
		// 首先清除原模型
		clear();
		init();
		// 统计数目
		const auto NX(mdl.getSize<Variable::OpVar>());
		const auto NC1(mdl.getSize<Constraint::OpLinCon>());
		const auto NC2(mdl.getSize<Constraint::OpQuadCon>());
		const auto NC(NC1 + NC2);
		// 加载变量
		xi_->resize(NX);
		xl_->resize(NX);
		xu_->resize(NX);
		for (auto iter = mdl.getCBegin<Variable::OpVar>(); iter != mdl.getCEnd<Variable::OpVar>(); ++iter)
		{
			auto var(iter.getVal());
			switch (var.getType())
			{
			case Variable::OpVarType::Bool:
				throw OpExcBase("[Solver::OpIPOPTSolI::extract]: Exception->can not handle Variable::OpVarType::Bool");
				break;
			case Variable::OpVarType::Int:
				throw OpExcBase("[Solver::OpIPOPTSolI::extract]: Exception->can not handle Variable::OpVarType::Int");
				break;
			case Variable::OpVarType::Con:
				break;
			case Variable::OpVarType::Sem:
				throw OpExcBase("[Solver::OpIPOPTSolI::extract]: Exception->can not handle Variable::OpVarType::Sem");
				break;
			default:
				throw OpExcBase("[Solver::OpIPOPTSolI::extract]: Exception->can not handle other variable type");
				break;
			}
			varidxs_.emplace(iter.getKey(), *nx_);
			(*xi_)[*nx_] = 0.0; // 初值暂时统一设为0
			(*xl_)[*nx_] = var.getLb();
			(*xu_)[*nx_] = var.getUb();
			(*nx_)++;
		}
		for (auto iter = mdl.getCBegin<Variable::OpPSDVar>(); iter != mdl.getCEnd<Variable::OpPSDVar>(); ++iter)
			throw OpExcBase("[Solver::OpIPOPTSolI::extract]: Exception->can not handle Variable::OpPSDVar");
		// 加载约束边界
		gl_->resize(NC);
		gu_->resize(NC);
		for (auto iter = mdl.getCBegin<Constraint::OpLinCon>(); iter != mdl.getCEnd<Constraint::OpLinCon>(); ++iter)
		{
			auto con(iter.getVal());
			lcidxs_.emplace(iter.getKey(), *ng_);
			(*gl_)[*ng_] = con.getLb();
			(*gu_)[*ng_] = con.getUb();
			(*ng_)++;
		}
		for (auto iter = mdl.getCBegin<Constraint::OpQuadCon>(); iter != mdl.getCEnd<Constraint::OpQuadCon>(); ++iter)
		{
			auto con(iter.getVal());
			qcidxs_.emplace(iter.getKey(), *ng_);
			(*gl_)[*ng_] = con.getLb();
			(*gu_)[*ng_] = con.getUb();
			(*ng_)++;
		}
		for (auto iter = mdl.getCBegin<Constraint::OpConicCon>(); iter != mdl.getCEnd<Constraint::OpConicCon>(); ++iter)
			throw OpExcBase("[Solver::OpIPOPTSolI::extract]: Exception->can not handle Constraint::OpConicCon");
		for (auto iter = mdl.getCBegin<Constraint::OpPSDCon>(); iter != mdl.getCEnd<Constraint::OpPSDCon>(); ++iter)
			throw OpExcBase("[Solver::OpIPOPTSolI::extract]: Exception->can not handle Constraint::OpPSDCon");
		for (auto iter = mdl.getCBegin<Constraint::OpSOSCon>(); iter != mdl.getCEnd<Constraint::OpSOSCon>(); ++iter)
			throw OpExcBase("[Solver::OpIPOPTSolI::extract]: Exception->can not handle Constraint::OpSOSCon");
		for (auto iter = mdl.getCBegin<Constraint::OpNLCon>(); iter != mdl.getCEnd<Constraint::OpNLCon>(); ++iter)
			throw OpExcBase("[Solver::OpIPOPTSolI::extract]: Exception->can not handle Constraint::OpNLCon");
		for (auto iter = mdl.getCBegin<Constraint::OpCdtCon>(); iter != mdl.getCEnd<Constraint::OpCdtCon>(); ++iter)
			throw OpExcBase("[Solver::OpIPOPTSolI::extract]: Exception->can not handle Constraint::OpCdtCon");
		// 加载目标函数和约束表达式
		switch (mdl.getObj().getSense())
		{
		case Objective::OpObjSense::Min:
			*dir_ = true;
			break;
		case Objective::OpObjSense::Max:
			*dir_ = false;
			break;
		default:
			throw OpExcBase("[Solver::OpIPOPTSolI::extract]: Exception->can not handle other objective function sense");
			break;
		}
		fg_->handle = [&, mdl](ADvector& f, const ADvector& x) ->void {
			assert(f.size() == *ng_ + 1);
			assert(x.size() == *nx_);
			size_t count(1);
			// 目标函数
			auto& obj(f[0]);
			auto obj0(mdl.getObj().getLinExpr() + mdl.getObj().getQuadExpr());
			obj = CppAD::AD<double>(obj0.getConstant());
			for (auto iter = obj0.getLBegin(); iter != obj0.getLEnd(); ++iter)
				obj += iter.getCoeff() * x[varidxs_.at(iter.getVar().getIndex())];
			for (auto iter = obj0.getQBegin(); iter != obj0.getQEnd(); ++iter)
				obj += iter.getCoeff() * x[varidxs_.at(iter.getVar1().getIndex())] * x[varidxs_.at(iter.getVar2().getIndex())];
			if (!(*dir_))
				obj *= -1;
			// 线性约束
			for (auto iter = mdl.getCBegin<Constraint::OpLinCon>(); iter != mdl.getCEnd<Constraint::OpLinCon>(); ++iter)
			{
				auto con(iter.getVal());
				auto& gExpr(f[count]);
				gExpr = CppAD::AD<double>(con.getExpr().getConstant());
				for (auto iter = con.getExpr().getLBegin(); iter != con.getExpr().getLEnd(); ++iter)
					gExpr += iter.getCoeff() * x[varidxs_.at(iter.getVar().getIndex())];
				count++;
			}
			// 二次约束
			for (auto iter = mdl.getCBegin<Constraint::OpQuadCon>(); iter != mdl.getCEnd<Constraint::OpQuadCon>(); ++iter)
			{
				auto con(iter.getVal());
				auto& gExpr(f[count]);
				gExpr = CppAD::AD<double>(con.getExpr().getConstant());
				for (auto iter = con.getExpr().getLBegin(); iter != con.getExpr().getLEnd(); ++iter)
					gExpr += iter.getCoeff() * x[varidxs_.at(iter.getVar().getIndex())];
				for (auto iter = con.getExpr().getQBegin(); iter != con.getExpr().getQEnd(); ++iter)
					gExpr += iter.getCoeff() * x[varidxs_.at(iter.getVar1().getIndex())] * x[varidxs_.at(iter.getVar2().getIndex())];
				count++;
			}
		};
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
		throw OpExcBase("[Solver::OpIPOPTSolI::extract]: Exception->unknow exception!");
	}
}

void Solver::OpIPOPTSolI::solve()
{
	CppAD::ipopt::solve<Dvector, OpIPOPTFGEval>(*opts_, *xi_, *xl_, *xu_, *gl_, *gu_, *fg_, *sol_);
}

void Solver::OpIPOPTSolI::solveFixed()
{
	// 暂无实现
}

void Solver::OpIPOPTSolI::setParam(const OpConfig& cfg)
{
	if (opts_)
	{
		for (auto iter = cfg.getCBegin<OpInt>("OPUA.IPOPT"); iter != cfg.getCEnd<OpInt>("OPUA.IPOPT"); ++iter)
			if (iter.ok())
				(*opts_) += cfgcvt_.getIntParam(iter.getKey()) + "    " + std::to_string(iter.getVal()) + '\n';
		for (auto iter = cfg.getCBegin<OpFloat>("OPUA.IPOPT"); iter != cfg.getCEnd<OpFloat>("OPUA.IPOPT"); ++iter)
			if (iter.ok())
				(*opts_) += cfgcvt_.getDoubleParam(iter.getKey()) + "    " + std::to_string(iter.getVal()) + '\n';
		for (auto iter = cfg.getCBegin<OpStr>("OPUA.IPOPT"); iter != cfg.getCEnd<OpStr>("OPUA.IPOPT"); ++iter)
			if (iter.ok())
				(*opts_) += cfgcvt_.getIntParam(iter.getKey()) + "    " + iter.getVal() + '\n';
	}
}

OpLInt Solver::OpIPOPTSolI::getStatus() const
{
	return sol_->status;
}

OpFloat Solver::OpIPOPTSolI::getObjValue() const
{
	return (*dir_) ? sol_->obj_value : -sol_->obj_value;
}

OpFloat Solver::OpIPOPTSolI::getValue(Variable::OpVar var) const
{
	return sol_->x[varidxs_.at(var.getIndex())];
}

OpFloat Solver::OpIPOPTSolI::getValue(const Expression::OpLinExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	return result;
}

OpFloat Solver::OpIPOPTSolI::getValue(const Expression::OpQuadExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar1()) * getValue(iter.getVar2());
	return result;
}

OpFloat Solver::OpIPOPTSolI::getValue(Objective::OpObj obj) const
{
	return getValue(obj.getLinExpr()) + getValue(obj.getQuadExpr());
}

OpFloat Solver::OpIPOPTSolI::getDual(Constraint::OpLinCon con) const
{
	return sol_->lambda[lcidxs_.at(con.getIndex())];
}

void Solver::OpIPOPTSolI::write(OpStr path) const
{
	// 暂无实现
}

OpULInt Solver::OpIPOPTSolI::getMemoryUsage() const
{
	return sizeof(*this);
}

Solver::OpIPOPTSolI::OpIPOPTSolI(OpEnvI* env)
	: OpImplBase('S', env),
	dir_(nullptr),
	nx_(nullptr),
	ng_(nullptr),
	xi_(nullptr),
	xl_(nullptr),
	xu_(nullptr),
	gl_(nullptr),
	gu_(nullptr),
	opts_(nullptr),
	fg_(nullptr),
	sol_(nullptr)
{

}

Solver::OpIPOPTSolI::OpIPOPTSolI(OpEnvI* env, Model::OpModel mdl)
	: OpImplBase('S', env),
	dir_(nullptr),
	nx_(nullptr),
	ng_(nullptr),
	xi_(nullptr),
	xl_(nullptr),
	xu_(nullptr),
	gl_(nullptr),
	gu_(nullptr),
	opts_(nullptr),
	fg_(nullptr),
	sol_(nullptr)
{
	extract(mdl);
}

Solver::OpIPOPTSolI::~OpIPOPTSolI()
{
	clear();
}

/* OPUA::Solver::OpIPOPTSol */

void Solver::OpIPOPTSol::extract(Model::OpModel mdl)
{
	static_cast<Solver::OpIPOPTSolI*>(impl_)->extract(mdl);
}

void Solver::OpIPOPTSol::solve()
{
	static_cast<Solver::OpIPOPTSolI*>(impl_)->solve();
}

void Solver::OpIPOPTSol::solveFixed()
{
	static_cast<Solver::OpIPOPTSolI*>(impl_)->solveFixed();
}

void Solver::OpIPOPTSol::setParam(const OpConfig& cfg)
{
	static_cast<Solver::OpIPOPTSolI*>(impl_)->setParam(cfg);
}

OpLInt Solver::OpIPOPTSol::getStatus() const
{
	return static_cast<Solver::OpIPOPTSolI*>(impl_)->getStatus();
}

OpFloat Solver::OpIPOPTSol::getObjValue() const
{
	return static_cast<Solver::OpIPOPTSolI*>(impl_)->getObjValue();
}

OpFloat Solver::OpIPOPTSol::getValue(Variable::OpVar var) const
{
	return static_cast<Solver::OpIPOPTSolI*>(impl_)->getValue(var);
}

OpFloat Solver::OpIPOPTSol::getValue(const Expression::OpLinExpr& expr) const
{
	return static_cast<Solver::OpIPOPTSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpIPOPTSol::getValue(const Expression::OpQuadExpr& expr) const
{
	return static_cast<Solver::OpIPOPTSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpIPOPTSol::getValue(Objective::OpObj obj) const
{
	return static_cast<Solver::OpIPOPTSolI*>(impl_)->getValue(obj);
}

OpFloat Solver::OpIPOPTSol::getDual(Constraint::OpLinCon con) const
{
	return static_cast<Solver::OpIPOPTSolI*>(impl_)->getDual(con);
}

Solver::OpIPOPTSolI* Solver::OpIPOPTSol::getImpl() const
{
	return static_cast<Solver::OpIPOPTSolI*>(impl_);
}

void Solver::OpIPOPTSol::write(OpStr path) const
{
	static_cast<Solver::OpIPOPTSolI*>(impl_)->write(path);
}

void Solver::OpIPOPTSol::release0()
{
	static_cast<Solver::OpIPOPTSolI*>(impl_)->release();
}

OpBool Solver::OpIPOPTSol::operator==(const OpIPOPTSol& sol) const
{
	return impl_ == sol.impl_;
}

OpBool Solver::OpIPOPTSol::operator!=(const OpIPOPTSol& sol) const
{
	return impl_ != sol.impl_;
}

Solver::OpIPOPTSol::OpIPOPTSol()
{

}

Solver::OpIPOPTSol::OpIPOPTSol(OpIPOPTSolI* impl)
{
	impl_ = impl;
}

Solver::OpIPOPTSol::OpIPOPTSol(OpEnv env)
{
	impl_ = new Solver::OpIPOPTSolI(env.getImpl());
}

Solver::OpIPOPTSol::OpIPOPTSol(OpEnv env, Model::OpModel mdl)
{
	impl_ = new Solver::OpIPOPTSolI(env.getImpl(), mdl);
}

Solver::OpIPOPTSol::~OpIPOPTSol()
{

}
#endif
