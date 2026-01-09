#ifdef OPUA_COMPILE_SCIP
#include "OPUA.Solver.h"
#include "OPUA.Exception.h"
#include "OPUA.Common.h"
#include <objscip/objscip.h>
#include <objscip/objscipdefplugins.h>
#include <iostream>

using namespace OPUA;
#endif

#ifdef OPUA_COMPILE_SCIP

// 动态数组
template<typename T>
class OpSCIPDynArr
{
public:
	T* data_;
public:
	T* data() { return data_; }
	void release() { if (data_) delete data_; }
	void resize(OpLInt size) { release(); data_ = new T[size](); }
public:
	OpSCIPDynArr()
		: data_(nullptr) {}
	OpSCIPDynArr(OpLInt size)
		: data_(new T[size]()) {}
	~OpSCIPDynArr() { release(); }
};

/* OPUA::Solver::OpSCIPCfgCvt */
// SCIP配置翻译器
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
	// SCIP组件

	bool modified_;
	SCIP_OBJSENSE sense_; // SCIP目标函数类型
	SCIP* scip_; // SCIP模型/环境
	SCIP_Sol* sol_; // SCIP解
	std::vector<SCIP_VAR*> vars_; // SCIP变量表
	std::vector<SCIP_CONS*> cons_; // SCIP约束表
	std::vector<std::string> vnames_; // SCIP变量名称表
	std::vector<std::string> cnames_; // SCIP约束名称表
protected:
	// OPUA-SCIP映射组件

	std::unordered_map<Variable::OpVarI*, size_t> varidxs_; // 变量序号表
	std::unordered_map<Constraint::OpLinConI*, size_t> lcidxs_; // 线性约束序号表
	std::unordered_map<Constraint::OpQuadConI*, size_t> qcidxs_; // 二次约束序号表
	std::unordered_map<Constraint::OpNLConI*, size_t> nlcidxs_; // 非线性约束序号表
	Expression::OpLinExpr obj0_; // 当前目标函数

	friend class OpSCIPSol;
private:
	void initSCIP(); // 初始化SCIP环境
	void clearSCIP(); // 清理SCIP环境
	void clearMaps(); // 清理映射表
	void update(); // 更新SCIP模型
	SCIP_VARTYPE senseConvert1(Variable::OpVarType sense); // OPUA->SCIP变量类型转换
	SCIP_OBJSENSE senseConvert2(Objective::OpObjSense sense); // OPUA->SCIP优化目标类型转换
	std::string createNewExprOfSum(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式创建一个SCIP格式的Sum表达式
	std::string createNewExprOfAbs(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式创建一个SCIP格式的Abs表达式
	std::string createNewExprOfSquare(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式创建一个SCIP格式的Square表达式
	std::string createNewExprOfSqrt(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式创建一个SCIP格式的Sqrt表达式
	std::string createNewExprOfPow(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式创建一个SCIP格式的Pow表达式
	std::string createNewExprOfExp1(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式创建一个SCIP格式的Exp1表达式
	std::string createNewExprOfExp2(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式创建一个SCIP格式的Exp2表达式
	std::string createNewExprOfLog2(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式创建一个SCIP格式的Log2表达式
	std::string createNewExprOfSin(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式创建一个SCIP格式的Sin表达式
	std::string createNewExprOfCos(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式创建一个SCIP格式的Cos表达式
	//std::string createNewExprOfDiv(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式创建一个SCIP格式的Div表达式
	//std::string createNewExprOfMult(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式创建一个SCIP格式的Mult表达式
	std::string createNewExprOfVar(Variable::OpVar var); // 根据一个OPUA变量添加一个SCIP表达式
	std::string createNewExprOfNLE(const Expression::OpNLExpr& expr); // 根据一个OPUA非线性表达式添加一个SCIP表达式
protected:
	void init(); // 初始化(仅用于构造函数)
	void clear(); // 清除所有组件与映射信息(仅用于析构函数)
	void extract(Model::OpModel mdl); // 抽取OPUA模型
	void solve(); // 求解模型
	OpFloat getValue(Variable::OpVar var) const; // 获取变量的解
	OpFloat getValue(const Expression::OpLinExpr& expr) const; // 获取线性表达式的解
	OpFloat getValue(const Expression::OpQuadExpr& expr) const; // 获取二次表达式的解
	OpFloat getValue(Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
	OpFloat getObjValue(); // 获取目标函数的解
	SCIP_STATUS getStatus(); // 获取求解状态
	void write(OpStr path); // 
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
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
	vnames_ = std::vector<std::string>();
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
	cnames_ = std::vector<std::string>();
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
		vnames_.resize(varidxs_.size(), "");
		for (auto& v : varidxs_)
		{
			Variable::OpVar var(v.first);
			double lb(Constant::IsInfinity(var.getLb()) ? -SCIPinfinity(scip_) : var.getLb());
			double ub(Constant::IsInfinity(var.getUb()) ? SCIPinfinity(scip_) : var.getUb());
			vnames_[count] = "v_" + std::to_string(count) /* + "_" + var.getName()*/;
			SCIPcreateVar(scip_, &(vars_[count]), vnames_[count].c_str(), lb, ub, 0.0, senseConvert1(var.getType()),
				TRUE, FALSE, NULL, NULL, NULL, NULL, NULL);
			SCIPaddVar(scip_, vars_[count]);
			v.second = count;
			count++;
		}
		count = 0;
		cons_.resize(lcidxs_.size() + qcidxs_.size() + nlcidxs_.size(), nullptr);
		cnames_.resize(lcidxs_.size() + qcidxs_.size() + nlcidxs_.size(), "");
		// 解析线性约束
		for (auto& lc : lcidxs_)
		{
			auto& tmp(cons_[count]);
			Constraint::OpLinCon con(lc.first);
			double constItem(con.getExpr().getConstant());
			OpSCIPDynArr<SCIP_VAR*> vls(con.getExpr().getSize());
			OpSCIPDynArr<double> cfls(con.getExpr().getSize());
			cnames_[count] = "c_" + std::to_string(count) /* + "_" + con.getName()*/;
			size_t countl(0);
			for (auto iter = con.getExpr().getLBegin(); iter != con.getExpr().getLEnd(); ++iter)
			{
				auto var(iter.getVar());
				if (!var.getFixed())
					vls.data()[countl] = vars_[varidxs_.at(var.getImpl())], cfls.data()[countl] = iter.getCoeff(), countl++;
				else
					constItem += iter.getCoeff() * var.getFixedValue();
			}
			double lb(-SCIPinfinity(scip_)), ub(SCIPinfinity(scip_));
			if (!Constant::IsInfinity(con.getLb()))
				lb = con.getLb() - constItem;
			if (!Constant::IsInfinity(con.getUb()))
				ub = con.getUb() - constItem;
			//SCIPcreateConsLinear(scip_, &(tmp), cnames_[count].c_str(),
				//0, nullptr, nullptr, -SCIPinfinity(scip_), SCIPinfinity(scip_),
				//TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
			SCIPcreateConsBasicLinear(scip_, &(tmp), cnames_[count].c_str(),
				countl, vls.data(), cfls.data(), lb, ub);
			SCIPaddCons(scip_, tmp);
			lc.second = count;
			count++;
		}
		// 解析二次约束
		for (auto& qc : qcidxs_)
		{
			auto& tmp(cons_[count]);
			Constraint::OpQuadCon con(qc.first);
			double constItem(con.getExpr().getConstant());
			OpSCIPDynArr<SCIP_VAR*> vls(con.getExpr().getSize());
			OpSCIPDynArr<double> cfls(con.getExpr().getSize());
			OpSCIPDynArr<SCIP_VAR*> vq1s(con.getExpr().getSize());
			OpSCIPDynArr<SCIP_VAR*> vq2s(con.getExpr().getSize());
			OpSCIPDynArr<double> cfqs(con.getExpr().getSize());
			cnames_[count] = "c_" + std::to_string(count) /* + "_" + con.getName()*/;
			size_t countl(0), countq(0);
			for (auto iter = con.getExpr().getLBegin(); iter != con.getExpr().getLEnd(); ++iter)
			{
				auto var(iter.getVar());
				if (!var.getFixed())
					vls.data()[countl] = vars_[varidxs_.at(var.getImpl())], cfls.data()[countl] = iter.getCoeff(), countl++;
				else
					constItem += iter.getCoeff() * var.getFixedValue();
			}
			for (auto iter = con.getExpr().getQBegin(); iter != con.getExpr().getQEnd(); ++iter)
			{
				auto var1(iter.getVar1()), var2(iter.getVar2());
				if (var1.getImpl() == var2.getImpl())
				{
					if (!var1.getFixed())
						vls.data()[countl] = vars_[varidxs_.at(var1.getImpl())], cfls.data()[countl] = iter.getCoeff(), countl++;
					else
						constItem += iter.getCoeff() * var1.getFixedValue() * var1.getFixedValue();
				}
				else
				{
					auto fix1(var1.getFixed()), fix2(var2.getFixed());
					if (!fix1 && !fix2)
						vq1s.data()[countq] = vars_[varidxs_.at(var1.getImpl())], vq2s.data()[countq] = vars_[varidxs_.at(var2.getImpl())], cfqs.data()[countq] = iter.getCoeff(), countq++;
					else if (fix1 && !fix2)
						vls.data()[countl] = vars_[varidxs_.at(var2.getImpl())], cfls.data()[countl] = iter.getCoeff() * var1.getFixedValue(), countl++;
					else if (!fix1 && fix2)
						vls.data()[countl] = vars_[varidxs_.at(var1.getImpl())], cfls.data()[countl] = iter.getCoeff() * var2.getFixedValue(), countl++;
					else
						constItem += iter.getCoeff() * var1.getFixedValue() * var2.getFixedValue();
				}
			}
			double lb(-SCIPinfinity(scip_)), ub(SCIPinfinity(scip_));
			if (!Constant::IsInfinity(con.getLb()))
				lb = con.getLb() - constItem;
			if (!Constant::IsInfinity(con.getUb()))
				ub = con.getUb() - constItem;
			SCIPcreateConsBasicQuadraticNonlinear(scip_, &(tmp), cnames_[count].c_str(), 
				countl, vls.data(), cfls.data(), countq, vq1s.data(), vq2s.data(), cfqs.data(), lb, ub);
			SCIPaddCons(scip_, tmp);
			qc.second = count;
			count++;
		}
		// 非线性约束解析
		for (auto& nlc : nlcidxs_)
		{
			Constraint::OpNLCon con(nlc.first);
			cnames_[count] = "c_" + std::to_string(count) /* + "_" + con.getName()*/;
			std::string nlexpr0(createNewExprOfNLE(con.getExpr()));
			if (nlexpr0.empty())
				OpExcBase("[Solver::OpSCIPSolI::update]: Exception->create NL expression failed" + con.getName());
			std::string nlexpr("1.0*(" + nlexpr0 + ")");
			nlexpr += " - 1.0*";
			nlexpr += createNewExprOfVar(con.getVar());
			//std::cout << nlexpr << std::endl;
			SCIP_EXPR* expr(nullptr);
			SCIP_EXPR* simplified(nullptr);
			SCIP_Bool changed;
			SCIP_Bool infeasible;
			OpAutoFree guard([&]() {
				if (simplified)
					SCIPreleaseExpr(scip_, &simplified);
				SCIPreleaseExpr(scip_, &expr);
				});
			SCIPparseExpr(scip_, &expr, nlexpr.c_str(), NULL, NULL, NULL);
			//SCIPsimplifyExpr(scip_, expr, &simplified, &changed, &infeasible, NULL, NULL);
			//SCIPprintExpr(scip_, expr, NULL);
			auto& tmp(cons_[count]);
			SCIPcreateConsBasicNonlinear(scip_, &(tmp), cnames_[count].c_str(), expr/*simplified*/,
				0.0, 0.0);
			SCIPaddCons(scip_, tmp);
			nlc.second = count;		
			count++;
		}
		// 解析目标函数
		for (auto iter = obj0_.getLBegin(); iter != obj0_.getLEnd(); ++iter)
		{
			auto var(iter.getVar());
			if (!var.getFixed())
				SCIPchgVarObj(scip_, vars_[varidxs_.at(var.getImpl())], iter.getCoeff());
		}
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

std::string Solver::OpSCIPSolI::createNewExprOfSum(const Expression::OpNLExpr& expr)
{
	std::string nlexpr;
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
	{
		nlexpr += createNewExprOfVar(iter.getVar());
		nlexpr += " + ";
	}
	nlexpr += std::to_string(expr.getParam());
	return nlexpr;
}

std::string Solver::OpSCIPSolI::createNewExprOfAbs(const Expression::OpNLExpr& expr)
{
	std::string nlexpr;
	if(expr.getSize())
		nlexpr += "abs(" + createNewExprOfVar(expr.getVar(0)) + ")";
	return nlexpr;
}

std::string Solver::OpSCIPSolI::createNewExprOfSquare(const Expression::OpNLExpr& expr)
{
	std::string nlexpr;
	if (expr.getSize())
		nlexpr += "(" + createNewExprOfVar(expr.getVar(0)) + ")^2";
	return nlexpr;
}

std::string Solver::OpSCIPSolI::createNewExprOfSqrt(const Expression::OpNLExpr& expr)
{
	std::string nlexpr;
	if (expr.getSize())
		nlexpr += "(" + createNewExprOfVar(expr.getVar(0)) + ")^0.5";
	return nlexpr;
}

std::string Solver::OpSCIPSolI::createNewExprOfPow(const Expression::OpNLExpr& expr)
{
	std::string nlexpr;
	if (expr.getSize())
		nlexpr += "(" + createNewExprOfVar(expr.getVar(0)) + ")^" + std::to_string(expr.getParam());
	return nlexpr;
}

std::string Solver::OpSCIPSolI::createNewExprOfExp1(const Expression::OpNLExpr& expr)
{
	std::string nlexpr;
	if (expr.getSize())
		nlexpr += "exp(" + createNewExprOfVar(expr.getVar(0)) + ")";
	return nlexpr;
}

std::string Solver::OpSCIPSolI::createNewExprOfExp2(const Expression::OpNLExpr& expr)
{
	std::string nlexpr;
	if (expr.getSize())
		nlexpr += std::to_string(expr.getParam()) + "^(" + createNewExprOfVar(expr.getVar(0)) + ")";
	return nlexpr;
}

std::string Solver::OpSCIPSolI::createNewExprOfLog2(const Expression::OpNLExpr& expr)
{
	std::string nlexpr;
	if (expr.getSize())
		nlexpr += "log(" + createNewExprOfVar(expr.getVar(0)) + ")";
	return nlexpr;
}

std::string Solver::OpSCIPSolI::createNewExprOfSin(const Expression::OpNLExpr& expr)
{
	std::string nlexpr;
	if (expr.getSize())
		nlexpr += "sin(" + createNewExprOfVar(expr.getVar(0)) + ")";
	return nlexpr;
}

std::string Solver::OpSCIPSolI::createNewExprOfCos(const Expression::OpNLExpr& expr)
{
	std::string nlexpr;
	if (expr.getSize())
		nlexpr += "cos(" + createNewExprOfVar(expr.getVar(0)) + ")";
	return nlexpr;
}

std::string Solver::OpSCIPSolI::createNewExprOfVar(Variable::OpVar var)
{
	return "<" + vnames_[varidxs_.at(var.getImpl())] + ">";
}

std::string Solver::OpSCIPSolI::createNewExprOfNLE(const Expression::OpNLExpr& expr)
{
	std::string nlexpr;
	switch (expr.getFunction())
	{
	case OPUA::Expression::OpNLFunc::Unknow: // 无法处理
		throw OpExcBase("[Solver::OpSCIPSolI::createNewExprOfNLE]: Exception->can not handle Expression::OpNLFunc::Unknow");
		break;
	case OPUA::Expression::OpNLFunc::Sum:
		nlexpr = createNewExprOfSum(expr);
		break;
	case OPUA::Expression::OpNLFunc::Abs:
		nlexpr = createNewExprOfAbs(expr);
		break;
	case OPUA::Expression::OpNLFunc::Max:
		//rhs = createNewExprByOp2(expr, SCIP_EXPR_MAX, false);
		break;
	case OPUA::Expression::OpNLFunc::Min:
		//rhs = createNewExprByOp2(expr, SCIP_EXPR_MIN, false);
		break;
	case OPUA::Expression::OpNLFunc::Square:
		nlexpr = createNewExprOfSquare(expr);
		break;
	case OPUA::Expression::OpNLFunc::Sqrt:
		nlexpr = createNewExprOfSqrt(expr);
		break;
	case OPUA::Expression::OpNLFunc::Pow:
		nlexpr = createNewExprOfPow(expr);
		break;
	case OPUA::Expression::OpNLFunc::Exp1:
		nlexpr = createNewExprOfExp1(expr);
		break;
	case OPUA::Expression::OpNLFunc::Exp2:
		nlexpr = createNewExprOfExp2(expr);
		break;
	case OPUA::Expression::OpNLFunc::Log1:// 无法处理
		throw OpExcBase("[Solver::OpSCIPSolI::createNewExprOfNLE]: Exception->can not handle Expression::OpNLFunc::Log1");
		break;
	case OPUA::Expression::OpNLFunc::Log2:
		nlexpr = createNewExprOfLog2(expr);
		break;
	case OPUA::Expression::OpNLFunc::Log3:// 无法处理
		throw OpExcBase("[Solver::OpSCIPSolI::createNewExprOfNLE]: Exception->can not handle Expression::OpNLFunc::Log3");
		break;
	case OPUA::Expression::OpNLFunc::Sin:
		//rhs = createNewExprByOp(expr, SCIP_EXPR_SIN, false);
		break;
	case OPUA::Expression::OpNLFunc::Cos:
		//rhs = createNewExprByOp(expr, SCIP_EXPR_COS, false);
		break;
	case OPUA::Expression::OpNLFunc::Tan:
		throw OpExcBase("[Solver::OpSCIPSolI::createNewExprOfNLE]: Exception->can not handle Expression::OpNLFunc::Tan");
		break;
	default:
		throw OpExcBase("[Solver::OpSCIPSolI::createNewExprOfNLE]: Exception->can not handle other NL function");
		break;
	}
	return nlexpr;
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
	OpBool success(false);
	OpAutoFree guard([&]() {
		if (!success)
			clear();
		});
	try
	{
		// 首先清除原有模型
		clear();
		// 加载新模型
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
		// 更新SCIP模型
		update();
		success = true;
	}
	catch (OpExcBase& e)
	{
		throw e;
	}
	catch (std::exception& e)
	{
		throw e;
	}
	catch (...)
	{
		throw OpExcBase("[Solver::OpSCIPSolI::extract]: Exception->unknow exception!");
	}
}

void Solver::OpSCIPSolI::solve()
{
	if (scip_)
	{
		//SCIPsolve(scip_);
		SCIPsolveConcurrent(scip_);
		sol_ = SCIPgetBestSol(scip_);
	}
}

OpFloat Solver::OpSCIPSolI::getValue(Variable::OpVar var) const
{
	return var.getFixed() ? var.getFixedValue() : SCIPgetSolVal(scip_, sol_, vars_[varidxs_.at(var.getImpl())]);
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
			//FILE* fr(nullptr);
			//fr = fopen(path.c_str(), "wt+");
			//SCIPprintOrigProblem(scip_, fr, nullptr, FALSE);
			SCIPwriteOrigProblem(scip_, path.c_str(), nullptr, FALSE);
			//fclose(fr);
		}
		else
		{
			SCIPwriteOrigProblem(scip_, nullptr, nullptr, FALSE);
			//SCIPprintOrigProblem(scip_, nullptr, nullptr, FALSE);
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
