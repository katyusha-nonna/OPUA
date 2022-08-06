#include "OPUA.Common.h"
#include "OPUA.Algorithm.h"
#include "OPUA.Math.h"
#include "OPUA.Exception.h"
#include <thread>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>
using namespace OPUA;

/* OPUA::Algorithm::OpRobustModelI */

class Algorithm::OpRobustModelI
	: public OpImplBase
{
protected:
	// 模型名称
	OpStr mname_;
	// 是否已执行自动对偶推导
	OpBool adual_;
protected:
	// 第一阶段目标函数
	Expression::OpLinExpr fso_;
	// 第一阶段约束条件集
	Constraint::OpLCArr fsc_;
	// 第一节段特殊约束条件集(单独求解，不属于第一阶段约束，用于沟通第一阶段与第二阶段)
	Constraint::OpLCArr fssc_;
	Constraint::OpQCArr fssqc_;
	// 第一阶段决策变量集
	Variable::OpVarArr fsv_;
	// 第一阶段特殊决策变量集(包含在第一阶段决策变量集中)
	Variable::OpVarArr fssv_;
	// 第二阶段原问题目标函数
	Expression::OpLinExpr sspo_;
	// 第二阶段原问题约束集
	Constraint::OpLCArr sspc_;
	// 第二阶段原问题变量集
	Variable::OpVarArr sspv_;
	// 对偶目标函数
	Expression::OpQuadExpr ssdo_;
	// 第二阶段对偶问题约束集
	Constraint::OpLCArr ssdc_;
	// 第二阶段对偶问题变量集
	Variable::OpVarArr ssdv_;
	// 第二阶段原问题标识集(对于变量true表示非负，false表示无限制；对于约束true表示不等式，false表示等式)
	Container::OpDict<Variable::OpVarI*, OpBool> sspfv_;
	Container::OpDict<Constraint::OpLinConI*, OpBool> sspfc_;
	// 第二阶段对偶问题标识集(对于变量true表示非负，false表示无限制；对于约束true表示不等式，false表示等式)
	Container::OpDict<Variable::OpVarI*, OpBool> ssdfv_;
	Container::OpDict<Constraint::OpLinConI*, OpBool> ssdfc_;
	// 第二阶段原始-对偶映射表(原始约束->对偶变量、对偶约束->原始变量)
	Container::OpDict<Constraint::OpLinConI*, Variable::OpVarI*> sspdm_;
	// 不确定集变量
	Variable::OpVarArr uv_;
	// 不确定集约束
	Constraint::OpLCArr uc_;
protected:
	// 变量上下界(变量->{上界, 下界})
	Container::OpDict<Variable::OpVarI*, std::pair<OpFloat, OpFloat> > vbd_;
	// 具有明确Bound的变量集
	std::set<Variable::OpVarI*> ebdvs_;
protected:
	// 第一阶段决策变量的解
	Container::OpFloatArr fss_;
	// 第二阶段原问题决策变量的解
	Container::OpFloatArr ssps_;
	// 第二阶段对偶问题决策变量的解
	Container::OpFloatArr ssds_;
	// 不确定集的解
	Container::OpFloatArr us_;
	// 第一阶段目标函数的解
	OpFloat fsos_;
	// 第二阶段目标函数的解
	OpFloat ssos_;

	friend class Algorithm::OpRobustModel;
	friend class Algorithm::OpAlgoCCG;
protected:
	// 初始化(仅允许构造函数使用)
	void init();
	// 清空模型(仅允许析构函数使用)
	void clear();
	// 按阶段自动执行约束的标准化(在autoDual之前执行)
	void autoStd(RobustStageType type);
	// 自动推导对偶
	void autoDual();
	// 自动推导变量的上下界
	void autoBound();
	// 更新模型(初始化解集等)
	void update();
	// 初始化解数组
	void initSolution(RobustStageType type);
	// 添加变量&约束
	void add(Variable::OpVarArr vars, RobustStageType type, OpBool flag);
	void add(Variable::OpVar var, RobustStageType type, OpBool flag);
	void add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag);
	void add(Constraint::OpLinCon con, RobustStageType type, OpBool flag);
	void add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag);
	void add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag);
	// 设置目标函数
	void setObj(const Expression::OpLinExpr& expr, RobustStageType type);
	void setObj(const Expression::OpQuadExpr& expr, RobustStageType type);
	// 设置解
	void setValue(Variable::OpVar var, RobustStageType type, OpFloat val);
	// 设置边界
	void setBound(Variable::OpVar var, RobustStageType type, OpFloat val, OpBool lb);
	void setBound(Variable::OpVar var, RobustStageType type, OpFloat lbVal, OpFloat ubVal);
	// 获取解
	OpFloat getValue(Variable::OpVar var, RobustStageType type) const;
	// 获取目标函数解
	OpFloat getObjValue(RobustStageType type) const;
	// 获取变量&约束数目
	OpULInt getSize(RobustStageType type, OpBool isVar) const;
	// 获取边界
	OpFloat getBound(Variable::OpVar var, RobustStageType type, OpBool lb) const;
	// 导出模型
	void write(OpStr root);
	// 释放前准备
	void preRelease();
	// 获取内存占用
	virtual OpULInt getMemoryUsage() const;
protected:
	// 构造函数
	OpRobustModelI(OpEnvI* env);
	OpRobustModelI(OpEnvI* env, OpStr name);
public:
	// 析构函数
	virtual ~OpRobustModelI();
};

void Algorithm::OpRobustModelI::init()
{
	OpEnv localEnv(env_);
	// 第一阶段模型
	fso_ = Expression::OpLinExpr(0.0);
	fsc_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(fsc_.getImpl());
	fssc_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(fssc_.getImpl());
	fssqc_ = Constraint::OpQCArr(localEnv);
	localEnv.addManagement(fssqc_.getImpl());
	fsv_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(fsv_.getImpl());
	fssv_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(fssv_.getImpl());
	// 第二阶段模型
	sspo_ = Expression::OpLinExpr(0.0);
	sspc_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(sspc_.getImpl());
	sspv_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(sspv_.getImpl());
	ssdo_ = Expression::OpQuadExpr(0.0);
	ssdc_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(ssdc_.getImpl());
	ssdv_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(ssdv_.getImpl());
	sspfv_ = Container::OpDict<Variable::OpVarI*, OpBool>(localEnv);
	localEnv.addManagement(sspfv_.getImpl());
	sspfc_ = Container::OpDict<Constraint::OpLinConI*, OpBool>(localEnv);
	localEnv.addManagement(sspfc_.getImpl());
	ssdfv_ = Container::OpDict<Variable::OpVarI*, OpBool>(localEnv);
	localEnv.addManagement(ssdfv_.getImpl());
	ssdfc_ = Container::OpDict<Constraint::OpLinConI*, OpBool>(localEnv);
	localEnv.addManagement(ssdfc_.getImpl());
	sspdm_ = Container::OpDict<Constraint::OpLinConI*, Variable::OpVarI*>(localEnv);
	localEnv.addManagement(sspdm_.getImpl());
	// 不确定集模型
	uv_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(uv_.getImpl());
	uc_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(uc_.getImpl());
	// 变量边界
	vbd_ = Container::OpDict<Variable::OpVarI*, std::pair<OpFloat, OpFloat> >(localEnv);
	localEnv.addManagement(vbd_.getImpl());
	ebdvs_ = std::set<Variable::OpVarI*>();
	// 模型解
	fss_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(fss_.getImpl());
	ssps_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(ssps_.getImpl());
	ssds_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(ssds_.getImpl());
	us_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(us_.getImpl());
	fsos_ = 0.0;
	ssos_ = 0.0;
	// 其他
	adual_ = false;
}

void Algorithm::OpRobustModelI::clear()
{
	// 第一阶段模型
	fso_.clear();
	fsc_.release();
	fssc_.release();
	fssqc_.release();
	fsv_.release();
	fssv_.release();
	// 第二阶段模型
	sspo_.clear();
	sspc_.release();
	sspv_.release();
	ssdo_.clear();
	if (adual_)
	{
		ssdc_.releaseElements();
		ssdv_.releaseElements();
	}
	ssdc_.release();
	ssdv_.release();
	sspfv_.release();
	sspfc_.release();
	ssdfv_.release();
	ssdfc_.release();
	sspdm_.release();
	// 不确定集模型
	uv_.release();
	uc_.release();
	// 变量边界
	vbd_.release();
	ebdvs_.clear();
	// 模型解
	fss_.release();
	ssps_.release();
	ssds_.release();
	us_.release();
}

void Algorithm::OpRobustModelI::autoStd(RobustStageType type)
{
	auto stdCvt = [](auto& conArray) {
		for (OpULInt i = 0; i < conArray.getSize(); i++)
		{
			auto& con(conArray[i]);
			if (con.isLocked())
			{
				con.unlock();
				con.standardize();
				con.lock();
			}
			else
				con.standardize();
		}
	};
	switch (type)
	{
	case OPUA::Algorithm::RobustStageType::Unknown:
		break;
	case OPUA::Algorithm::RobustStageType::FirstStage:
		stdCvt(fsc_);
		break;
	case OPUA::Algorithm::RobustStageType::FirstStageSpecial:
		stdCvt(fssc_);
		break;
	case OPUA::Algorithm::RobustStageType::SecondStagePrimal:
		stdCvt(sspc_);
		break;
	case OPUA::Algorithm::RobustStageType::SecondStageDual:
		stdCvt(ssdc_);
		break;
	case OPUA::Algorithm::RobustStageType::Uncertainty:
		stdCvt(uc_);
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::autoDual()
{
	// 执行该函数需要子问题为线性问题(可以包含整数变量但相关表达式需要为线性)
	// 仅对第二阶段原始问题进行对偶推导
	// 手动设置：sspo_, sspc_, sspv_, sspfc_, sspfv_
	// 自动生成：ssdo_, ssdc_, ssdv_, ssdfc_, ssdfv_, sspdm_

	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;

	// 环境变量
	OpEnv localEnv(env_);
	std::thread::id tid = std::this_thread::get_id();
	std::cout << "[OPUA::Algorithm::OpRobustModel::autoDual](Thread: " << tid << ")MemoryUsage(start): " << localEnv.getMemoryUsage() << std::endl;
	// (需要用后清除的)临时变量
	OpULInt numLambda(0), numMu(0), numNeq(0), numEq(0); // 统计值
	std::vector<OpVar> lambda; // 不等式约束对偶变量集(不需要释放)
	std::vector<OpVar> mu; // 等式约束对偶变量集(不需要释放)
	std::unordered_map<OpVarI*, std::set<OpULInt> > primalVarAtCon; // 决策变量出现的约束范围查询表
	std::unordered_map<OpVarI*, OpFloat> primalObjCoefMap; // 目标函数系数查询表
	std::unordered_map<OpLinConI*, OpLinExpr> primalRHS; // 右端向量查询表
	// 守护对象(自动释放内存)
	OpAutoFree guard([&]() {
		std::cout << "[OPUA::Algorithm::OpRobustModel::autoDual](Thread: " << tid << ")MemoryUsage(end): " << localEnv.getMemoryUsage() << std::endl;
	});

	// 创建对偶决策变量&绑定原始约束条件与对偶决策变量
	for (auto it = sspfc_.getCBegin(); it != sspfc_.getCEnd(); ++it)
	{
		if (it.getVal())
		{
			auto dualVar = OpVar(localEnv, OpVarType::Con, 0, Constant::Infinity, "lambda_" + std::to_string(numLambda));
			lambda.emplace_back(dualVar);
			ssdfv_.add(dualVar.getImpl(), true);
			sspdm_.add(it.getKey(), dualVar.getImpl());
			++numLambda;
		}
		else
		{
			auto dualVar = OpVar(localEnv, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "mu_" + std::to_string(numMu));
			mu.emplace_back(dualVar.getImpl());
			ssdfv_.add(dualVar.getImpl(), false);
			sspdm_.add(it.getKey(), dualVar.getImpl());
			++numMu;
		}
	}
	for (auto it = sspfv_.getCBegin(); it != sspfv_.getCEnd(); ++it)
	{
		if (it.getVal())
			++numNeq;
		else
			++numEq;
	}
	for (const auto& dv : lambda)
		ssdv_.add(dv);
	for (const auto& dv : mu)
		ssdv_.add(dv);
	// 形成决策变量出现的约束范围查询表
	for (OpULInt i = 0; i < sspc_.getSize(); i++)
		for (auto it = sspc_[i].getExpr().getLBegin(); it != sspc_[i].getExpr().getLEnd(); ++it)
			primalVarAtCon[it.getVar().getImpl()].emplace(i);
	// 形成原始问题目标函数系数查询表
	for (auto it = sspo_.getLBegin(); it != sspo_.getLEnd(); ++it)
		primalObjCoefMap.emplace(it.getVar().getImpl(), it.getCoeff());
	// 形成原始问题右端向量查询表(这个右端向量并不只是LB，还可能包括由非第二阶段决策变量构成的表达式)
	for (OpULInt i = 0; i < sspc_.getSize(); i++)
	{
		const auto& primalCon = sspc_[i];
		auto& rhs = primalRHS.emplace(primalCon.getImpl(), 0.0).first->second;
		rhs.addLinTerm(primalCon.getLb());
		for (auto it = primalCon.getExpr().getLBegin(); it != primalCon.getExpr().getLEnd(); ++it)
		{
			auto var = it.getVar();
			if (!sspfv_.has(var.getImpl()))
				rhs.addLinTerm(var, -it.getCoeff());
		}
	}
	// 根据第二阶段原始问题决策变量集，逐个推导对偶约束条件
	for (OpULInt i = 0; i < sspv_.getSize(); i++)
	{
		const auto& primalVar = sspv_[i];
		OpFloat dualRHS = primalObjCoefMap.find(primalVar.getImpl()) != primalObjCoefMap.end() ?
			primalObjCoefMap.at(primalVar.getImpl()) : 0.0;
		OpLinExpr dualExpr(0.0);
		// 这里进行了优化，事先统计每个变量出现的约束的范围以减少迭代的次数
		if (primalVarAtCon.find(primalVar.getImpl()) != primalVarAtCon.end())
		{
			for (auto& j : primalVarAtCon.at(primalVar.getImpl()))
			{
				const auto& primalCon = sspc_[j];
				for (auto it = primalCon.getExpr().getLBegin(); it != primalCon.getExpr().getLEnd(); ++it)
				{
					if (it.getVar().getImpl() == primalVar.getImpl())
					{
						dualExpr.addLinTerm(sspdm_.getVal(primalCon.getImpl()), it.getCoeff());
						break;
					}
				}
			}
		}
		if (sspfv_.getVal(primalVar.getImpl()))
		{
			OpLinCon dualCon(localEnv, -Constant::Infinity, dualExpr, dualRHS);
			ssdfc_.add(dualCon.getImpl(), true);
			sspdm_.add(dualCon.getImpl(), primalVar.getImpl());
			ssdc_.add(dualCon);
		}
		else
		{
			OpLinCon dualCon(localEnv, dualRHS, dualExpr, dualRHS);
			ssdfc_.add(dualCon.getImpl(), false);
			sspdm_.add(dualCon.getImpl(), primalVar.getImpl());
			ssdc_.add(dualCon);
		}
	}
	// 根据右端向量推导对偶目标函数(可能产生二次项)
	// 这里增加了自动化简，避免出现系数为0
	for (OpULInt i = 0; i < sspc_.getSize(); i++)
	{
		const auto& primalCon = sspc_[i];
		ssdo_ += primalRHS.at(primalCon.getImpl()) * OpVar(sspdm_.getVal(primalCon.getImpl()));
	}
	ssdo_.simplify();
}

void Algorithm::OpRobustModelI::autoBound()
{
	// 自动推导原始变量/对偶变量的上下界
	// 目前的原则是仅将约束形式的Bound记录下来

	// 环境变量
	OpEnv localEnv(env_);
	std::thread::id tid = std::this_thread::get_id();
	std::cout << "[OPUA::Algorithm::OpRobustModel::autoBound](Thread: " << tid << ")MemoryUsage(start): " << localEnv.getMemoryUsage() << std::endl;
	// 守护对象(自动释放内存)
	OpAutoFree guard([&]() {
		std::cout << "[OPUA::Algorithm::OpRobustModel::autoBound](Thread: " << tid << ")MemoryUsage(end): " << localEnv.getMemoryUsage() << std::endl;
	});
	// 首先根据变量本身的lb和ub来初始化
	auto init = [&](auto& varArray) {
		for (OpULInt i = 0; i < varArray.getSize(); i++)
		{
			auto& var = varArray[i];
			vbd_[var.getImpl()] = { var.getLb(), var.getUb() };
		}
	};
	init(fsv_);
	init(sspv_);
	init(ssdv_);
	init(uv_);
	// 然后巡访所有的约束，找出bound约束并记录
	auto findBound = [&](auto& conArray) {
		for (OpULInt i = 0; i < conArray.getSize(); i++)
		{
			const auto& con = conArray[i];
			if (con.getExpr().getSize() == 1)
			{
				auto var = con.getExpr().getLBegin().getVar();
				auto coef = con.getExpr().getLBegin().getCoeff();
				auto& bound = vbd_[var.getImpl()];
				if (coef > 0)
				{
					bound.first = Math::OpMax(bound.first, con.getLb() / coef);
					bound.second = Math::OpMin(bound.second, con.getUb() / coef);
				}
				else if (coef < 0)
				{
					bound.first = Math::OpMax(bound.first, con.getUb() / coef);
					bound.second = Math::OpMin(bound.second, con.getLb() / coef);
				}
			}
		}
	};
	findBound(fsc_);
	findBound(sspc_);
	findBound(ssdc_);
	findBound(uc_);
}

void Algorithm::OpRobustModelI::update()
{
	initSolution(RobustStageType::FirstStage);
	initSolution(RobustStageType::SecondStagePrimal);
	initSolution(RobustStageType::SecondStageDual);
	initSolution(RobustStageType::Uncertainty);
}

void Algorithm::OpRobustModelI::initSolution(RobustStageType type)
{
	switch (type)
	{
	case RobustStageType::FirstStage:
		fss_.setSize(fsv_.getSize());
		break;
	case RobustStageType::SecondStagePrimal:
		ssps_.setSize(sspv_.getSize());
		break;
	case RobustStageType::SecondStageDual:
		ssds_.setSize(ssdv_.getSize());
		break;
	case RobustStageType::Uncertainty:
		us_.setSize(uv_.getSize());
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::add(Variable::OpVarArr vars, RobustStageType type, OpBool flag)
{
	switch (type)
	{
	case RobustStageType::FirstStage:
		fsv_.add(vars);
		break;
	case RobustStageType::FirstStageSpecial:
		fssv_.add(vars);
		break;
	case RobustStageType::SecondStagePrimal:
		sspv_.add(vars);
		for (OpULInt i = 0; i < vars.getSize(); i++)
			sspfv_.add(vars[i].getImpl(), flag);
		break;
	case RobustStageType::SecondStageDual:
		ssdv_.add(vars);
		for (OpULInt i = 0; i < vars.getSize(); i++)
			ssdfv_.add(vars[i].getImpl(), flag);
		break;
	case RobustStageType::Uncertainty:
		uv_.add(vars);
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::add(Variable::OpVar var, RobustStageType type, OpBool flag)
{
	switch (type)
	{
	case RobustStageType::FirstStage:
		fsv_.add(var);
		break;
	case RobustStageType::FirstStageSpecial:
		fssv_.add(var);
		break;
	case RobustStageType::SecondStagePrimal:
		sspv_.add(var);
		sspfv_.add(var.getImpl(), flag);
		break;
	case RobustStageType::SecondStageDual:
		ssdv_.add(var);
		ssdfv_.add(var.getImpl(), flag);
		break;
	case RobustStageType::Uncertainty:
		uv_.add(var);
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag)
{
	switch (type)
	{
	case RobustStageType::FirstStage:
		fsc_.add(cons);
		break;
	case RobustStageType::FirstStageSpecial:
		fssc_.add(cons);
		break;
	case RobustStageType::SecondStagePrimal:
		sspc_.add(cons);
		for (OpULInt i = 0; i < cons.getSize(); i++)
			sspfc_.add(cons[i].getImpl(), flag);
		break;
	case RobustStageType::SecondStageDual:
		ssdc_.add(cons);
		for (OpULInt i = 0; i < cons.getSize(); i++)
			ssdfc_.add(cons[i].getImpl(), flag);
		break;
	case RobustStageType::Uncertainty:
		uc_.add(cons);
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::add(Constraint::OpLinCon con, RobustStageType type, OpBool flag)
{
	switch (type)
	{
	case RobustStageType::FirstStage:
		fsc_.add(con);
		break;
	case RobustStageType::FirstStageSpecial:
		fssc_.add(con);
		break;
	case RobustStageType::SecondStagePrimal:
		sspc_.add(con);
		sspfc_.add(con.getImpl(), flag);
		break;
	case RobustStageType::SecondStageDual:
		ssdc_.add(con);
		ssdfc_.add(con.getImpl(), flag);
		break;
	case RobustStageType::Uncertainty:
		uc_.add(con);
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStageSpecial)
		fssqc_.add(cons);
}

void Algorithm::OpRobustModelI::add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStageSpecial)
		fssqc_.add(con);
}

void Algorithm::OpRobustModelI::setObj(const Expression::OpLinExpr& expr, RobustStageType type)
{
	switch (type)
	{
	case RobustStageType::FirstStage:
		fso_ = expr;
		break;
	case RobustStageType::SecondStagePrimal:
		sspo_ = expr;
		break;
	case RobustStageType::SecondStageDual:
		ssdo_ = expr;
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::setObj(const Expression::OpQuadExpr& expr, RobustStageType type)
{
	if (type == RobustStageType::SecondStageDual)
		ssdo_ = expr;
}

void Algorithm::OpRobustModelI::setValue(Variable::OpVar var, RobustStageType type, OpFloat val)
{
	auto setVal = [](auto& varArray, auto& valArray, auto var, auto val) {
		auto index = varArray.find(var); // 如果找不到相应的var，find函数返回size
		if (valArray.idxValid(index))
			valArray[index] = val;
	};
	switch (type)
	{
	case RobustStageType::FirstStage:
		setVal(fsv_, fss_, var, val);
		break;
	case RobustStageType::SecondStagePrimal:
		setVal(sspv_, ssps_, var, val);
		break;
	case RobustStageType::SecondStageDual:
		setVal(ssdv_, ssds_, var, val);
		break;
	case RobustStageType::Uncertainty:
		setVal(uv_, us_, var, val);
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::setBound(Variable::OpVar var, RobustStageType type, OpFloat val, OpBool lb)
{
	if (lb)
		vbd_[var.getImpl()].first = val;
	else
		vbd_[var.getImpl()].second = val;
}

void Algorithm::OpRobustModelI::setBound(Variable::OpVar var, RobustStageType type, OpFloat lbVal, OpFloat ubVal)
{
	setBound(var, type, lbVal, true);
	setBound(var, type, ubVal, false);
}

OpFloat Algorithm::OpRobustModelI::getValue(Variable::OpVar var, RobustStageType type) const
{
	OpFloat result(Constant::NaN);
	OpULInt index(Constant::MaxSize);
	switch (type)
	{
	case RobustStageType::FirstStage:
		index = fsv_.find(var);
		if (fss_.idxValid(index))
			result = fss_[index];
		break;
	case RobustStageType::SecondStagePrimal:
		index = sspv_.find(var);
		if (ssps_.idxValid(index))
			result = ssps_[index];
		break;
	case RobustStageType::SecondStageDual:
		index = ssdv_.find(var);
		if (ssds_.idxValid(index))
			result = ssds_[index];
		break;
	case RobustStageType::Uncertainty:
		index = uv_.find(var);
		if (us_.idxValid(index))
			result = us_[index];
		break;
	default:
		break;
	}
	return result;
}

OpFloat Algorithm::OpRobustModelI::getObjValue(RobustStageType type) const
{
	OpFloat result(Constant::NaN);
	switch (type)
	{
	case RobustStageType::Unknown:
		result = fsos_ + ssos_;
		break;
	case RobustStageType::FirstStage:
		result = fsos_;
		break;
	case RobustStageType::SecondStagePrimal:
		result = ssos_;
		break;
	case RobustStageType::SecondStageDual:
		result = ssos_;
		break;
	default:
		break;
	}
	return result;
}

OpULInt Algorithm::OpRobustModelI::getSize(RobustStageType type, OpBool isVar) const
{
	OpULInt result(0);
	switch (type)
	{
	case RobustStageType::FirstStage:
		result = isVar ? fsv_.getSize() : fsc_.getSize();
		break;
	case RobustStageType::FirstStageSpecial:
		result = isVar ? fssv_.getSize() : fssc_.getSize() + fssqc_.getSize();
		break;
	case RobustStageType::SecondStagePrimal:
		result = isVar ? sspv_.getSize() : sspc_.getSize();
		break;
	case RobustStageType::SecondStageDual:
		result = isVar ? ssdv_.getSize() : ssdc_.getSize();
		break;
	case RobustStageType::Uncertainty:
		result = isVar ? uv_.getSize() : uc_.getSize();
		break;
	default:
		break;
	}
	return result;
}

OpFloat Algorithm::OpRobustModelI::getBound(Variable::OpVar var, RobustStageType type, OpBool lb) const
{
	OpFloat bound(Constant::NaN);
	if (vbd_.has(var.getImpl()))
		bound = lb ? vbd_.getVal(var.getImpl()).first : vbd_.getVal(var.getImpl()).second;
	return bound;
}

void Algorithm::OpRobustModelI::write(OpStr root)
{
	using namespace Objective;
	using namespace Model;

	// 环境变量
	OpEnv localEnv(env_);
	std::thread::id tid = std::this_thread::get_id();
	std::cout << "[OPUA::Algorithm::OpRobustModel::write](Thread: " << tid << ")MemoryUsage(start): " << localEnv.getMemoryUsage() << std::endl;
	// 导出模型文件需要的MODEL对象和CPLEX对象
	OpModel model1(localEnv), model2(localEnv), model3(localEnv);
	model1.setName(mname_.c_str());
	model2.setName(mname_.c_str());
	model3.setName(mname_.c_str());
	OpObj obj1(OpMinimize(localEnv, fso_)),
		obj2(OpMinimize(localEnv, sspo_)),
		obj3(OpMaximize(localEnv, ssdo_));
	// 守护对象(自动释放内存)
	OpAutoFree guard([&]() {
		try
		{
			model1.release();
			model2.release();
			model3.release();
			obj1.release();
			obj2.release();
			obj3.release();
			std::cout << "[OPUA::Algorithm::OpRobustModel::write](Thread: " << tid << ")MemoryUsage(end): " << localEnv.getMemoryUsage() << std::endl;
		}
		catch (std::exception& ex)
		{
			std::cerr << ex.what() << std::endl;
		}
	});

	// 创建第一阶段和第二阶段模型
	model1.setObj(obj1);
	model1.add(fsc_);
	model2.setObj(obj2);
	model2.add(sspc_);
	model3.setObj(obj3);
	model3.add(ssdc_);
	// 导出模型
	std::string path1(root + "/" + mname_ + "_FS.lp"),
		path2(root + "/" + mname_ + "_SSP.lp"),
		path3(root + "/" + mname_ + "_SSD.lp");
	//model1.write(path1.c_str());
	//model2.write(path2.c_str());
	//model3.write(path3.c_str());
	model1.write("");
	model2.write("");
	model3.write("");
}

void Algorithm::OpRobustModelI::preRelease()
{
	OpEnv localEnv(env_);
	// 第一阶段模型
	localEnv.removeManagement(fsc_.getImpl());
	localEnv.removeManagement(fssc_.getImpl());
	localEnv.removeManagement(fssqc_.getImpl());
	localEnv.removeManagement(fsv_.getImpl());
	localEnv.removeManagement(fssv_.getImpl());
	// 第二阶段模型
	localEnv.removeManagement(sspc_.getImpl());
	localEnv.removeManagement(sspv_.getImpl());
	localEnv.removeManagement(ssdc_.getImpl());
	localEnv.removeManagement(ssdv_.getImpl());
	localEnv.removeManagement(sspfv_.getImpl());
	localEnv.removeManagement(sspfc_.getImpl());
	localEnv.removeManagement(ssdfv_.getImpl());
	localEnv.removeManagement(ssdfc_.getImpl());
	localEnv.removeManagement(sspdm_.getImpl());
	// 不确定集模型
	localEnv.removeManagement(uv_.getImpl());
	localEnv.removeManagement(uc_.getImpl());
	// 变量边界
	localEnv.removeManagement(vbd_.getImpl());
	// 模型解
	localEnv.removeManagement(fss_.getImpl());
	localEnv.removeManagement(ssps_.getImpl());
	localEnv.removeManagement(ssds_.getImpl());
	localEnv.removeManagement(us_.getImpl());
}

OpULInt Algorithm::OpRobustModelI::getMemoryUsage() const
{
	return sizeof(*this);
}

Algorithm::OpRobustModelI::OpRobustModelI(OpEnvI* env)
	: OpImplBase('M', env),
	mname_("RobustModel_" + std::to_string(idx_)),
	adual_(false)
{
	init();
}

Algorithm::OpRobustModelI::OpRobustModelI(OpEnvI* env, OpStr name)
	: OpImplBase('M', env),
	mname_(name),
	adual_(false)
{
	init();
}

Algorithm::OpRobustModelI::~OpRobustModelI()
{
	clear();
}

/* OPUA::Algorithm::OpRobustModel */

void Algorithm::OpRobustModel::autoStd(RobustStageType type)
{
	static_cast<OpRobustModelI*>(impl_)->autoStd(type);
}

void Algorithm::OpRobustModel::autoDual()
{
	static_cast<OpRobustModelI*>(impl_)->autoDual();
}

void Algorithm::OpRobustModel::autoBound()
{
	static_cast<OpRobustModelI*>(impl_)->autoBound();
}

void Algorithm::OpRobustModel::update()
{
	static_cast<OpRobustModelI*>(impl_)->update();
}

void Algorithm::OpRobustModel::add(Variable::OpVarArr vars, RobustStageType type, OpBool flag)
{
	static_cast<OpRobustModelI*>(impl_)->add(vars, type, flag);
}

void Algorithm::OpRobustModel::add(Variable::OpVar var, RobustStageType type, OpBool flag)
{
	static_cast<OpRobustModelI*>(impl_)->add(var, type, flag);
}

void Algorithm::OpRobustModel::add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag)
{
	static_cast<OpRobustModelI*>(impl_)->add(cons, type, flag);
}

void Algorithm::OpRobustModel::add(Constraint::OpLinCon con, RobustStageType type, OpBool flag)
{
	static_cast<OpRobustModelI*>(impl_)->add(con, type, flag);
}

void Algorithm::OpRobustModel::add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag)
{
	static_cast<OpRobustModelI*>(impl_)->add(cons, type, flag);
}

void Algorithm::OpRobustModel::add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag)
{
	static_cast<OpRobustModelI*>(impl_)->add(con, type, flag);
}

void OPUA::Algorithm::OpRobustModel::setObj(const Expression::OpLinExpr& expr, RobustStageType type)
{
	static_cast<OpRobustModelI*>(impl_)->setObj(expr, type);
}

void OPUA::Algorithm::OpRobustModel::setObj(const Expression::OpQuadExpr& expr, RobustStageType type)
{
	static_cast<OpRobustModelI*>(impl_)->setObj(expr, type);
}

void Algorithm::OpRobustModel::setValue(Variable::OpVar var, RobustStageType type, OpFloat val)
{
	static_cast<OpRobustModelI*>(impl_)->setValue(var, type, val);
}

void Algorithm::OpRobustModel::setBound(Variable::OpVar var, RobustStageType type, OpFloat val, OpBool lb)
{
	static_cast<OpRobustModelI*>(impl_)->setBound(var, type, val, lb);
}

void Algorithm::OpRobustModel::setBound(Variable::OpVar var, RobustStageType type, OpFloat lbVal, OpFloat ubVal)
{
	static_cast<OpRobustModelI*>(impl_)->setBound(var, type, lbVal, ubVal);
}

OpFloat Algorithm::OpRobustModel::getValue(Variable::OpVar var, RobustStageType type) const
{
	return static_cast<OpRobustModelI*>(impl_)->getValue(var, type);
}

OpFloat OPUA::Algorithm::OpRobustModel::getObjValue(RobustStageType type) const
{
	return static_cast<OpRobustModelI*>(impl_)->getObjValue(type);
}

OpULInt Algorithm::OpRobustModel::getSize(RobustStageType type, OpBool isVar) const
{
	return static_cast<OpRobustModelI*>(impl_)->getSize(type, isVar);
}

OpFloat Algorithm::OpRobustModel::getBound(Variable::OpVar var, RobustStageType type, OpBool lb) const
{
	return static_cast<OpRobustModelI*>(impl_)->getBound(var, type, lb);
}

void Algorithm::OpRobustModel::write(OpStr root)
{
	static_cast<OpRobustModelI*>(impl_)->write(root);
}

Algorithm::OpRobustModelI* Algorithm::OpRobustModel::getImpl() const
{
	return static_cast<OpRobustModelI*>(impl_);
}

void Algorithm::OpRobustModel::release()
{
	// 这里重写了release()方法，因为OpRobustModelI中包含了OPUA组件
	// 必须先执行preRelease()减少这些组件的引用计数，保证OpRobustModelI调用析构函数时，能正常释放这些组件
	if (impl_)
	{
		static_cast<OpRobustModelI*>(impl_)->preRelease();
		static_cast<OpRobustModelI*>(impl_)->release();
		impl_ = nullptr;
	}
}

OpBool Algorithm::OpRobustModel::operator==(const OpRobustModel& model) const
{
	return impl_ == model.getImpl();
}

OpBool Algorithm::OpRobustModel::operator!=(const OpRobustModel& model) const
{
	return impl_ != model.getImpl();
}

Algorithm::OpRobustModel::OpRobustModel()
{

}

Algorithm::OpRobustModel::OpRobustModel(OpRobustModelI* impl)
{
	impl_ = impl;
}

Algorithm::OpRobustModel::OpRobustModel(OpEnv env)
{
	impl_ = new OpRobustModelI(env.getImpl());
}

Algorithm::OpRobustModel::OpRobustModel(OpEnv env, OpStr name)
{
	impl_ = new OpRobustModelI(env.getImpl(), name);
}

Algorithm::OpRobustModel::~OpRobustModel()
{

}

/* OPUA::Algorithm::OpAlgoCCG */

struct Algorithm::OpAlgoCCG::OpCCGIterInfo
{
	OpBool needSolveRobust; // 是否需要求解鲁棒问题(False-视为确定性问题进行求解)
	Solver::OpSolType solverMode; // 求解器模式
	Solver::OpSolStatus masterStatus; // 主问题求解状态
	Solver::OpSolStatus subStatus; // 子问题求解状态
	Solver::OpSolStatus deterministicStatus; // 确定性问题求解状态
	OpLInt initMode; // 初始化模式选择
	OpLInt subProbSolveMode; // 子问题求解模式选择(暂时不起作用，固定使用OA)
	OpLInt iter; // 当前迭代次数
	OpLInt iterMax; // 最大迭代次数
	OpLInt iterNoImpr; // 无改善的累计迭代次数
	OpLInt iterNoImprLimt; // 无改善的最大累计迭代次数
	OpFloat etaVal; // 主问题辅助变量解(子问题最优解)
	OpFloat lb; // 问题下界
	OpFloat ub; // 问题上界
	OpFloat epsilon; //  收敛阈值
	OpFloat prevLb; // 上一次迭代的问题下界
	OpFloat prevUb; // 上一次迭代的问题上界
	OpFloat lbEpsilon; // 问题下界收敛阈值
	OpFloat ubEpsilon; // 问题上界收敛阈值

	Variable::OpVar eta; // 主问题辅助变量(子问题最优解)(需要释放)
	Variable::OpVarArr masterAddVars; // 主问题在迭代中动态生成的变量(需要释放)
	Constraint::OpLCArr masterAddCons; // 主问题在迭代中动态生成的约束(需要释放)
	Objective::OpObj masterObj; // 基本主问题的目标函数(需要释放)
	Model::OpModel masterModel; // 基本主问题模型对象(需要释放)
	Constraint::OpLCArr deterministicAddCons; // 确定性问题动态生成的约束(需要释放)
	Objective::OpObj deterministicObj; // 确定性问题的目标函数(需要释放)
	Model::OpModel deterministicModel; // 确定性问题模型对象(需要释放)

	void init(const OpRobustModelI* mdl, const Solver::OpConfig& config); // 初始化
	void clear(); // 清理
	void printIterInfo(std::ostream& stream, const std::thread::id& tid); // 打印迭代信息
};

void Algorithm::OpAlgoCCG::OpCCGIterInfo::init(const OpRobustModelI* mdl, const Solver::OpConfig& config)
{
	OpEnv localEnv(mdl->env_);
	needSolveRobust = (mdl->uc_.getSize()) && (!config.getCfg<OpBool>("OPUA.Algorithm.CCG.GetDeterministicSolution"));
	masterStatus = Solver::OpSolStatus::Unknown;
	subStatus = Solver::OpSolStatus::Unknown;
	deterministicStatus = Solver::OpSolStatus::Unknown;
	initMode = config.getCfg<OpLInt>("OPUA.Algorithm.CCG.FirstStageInitMode");
	subProbSolveMode = config.getCfg<OpLInt>("OPUA.Algorithm.CCG.SubProbSolveMode"); // 这项配置暂时不起作用
	iter = 0;
	iterMax = config.getCfg<OpLInt>("OPUA.Algorithm.CCG.IterMax");
	iterNoImpr = 0;
	iterNoImprLimt = std::max(2, config.getCfg<OpLInt>("OPUA.Algorithm.CCG.NoImprovementIterLimit"));
	etaVal = 0.0;
	lb = -Constant::Infinity;
	ub = Constant::Infinity;
	epsilon = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGGap");
	prevLb = -Constant::Infinity;
	prevUb = Constant::Infinity;
	lbEpsilon = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGLBGap");
	ubEpsilon = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGUBGap");

	eta = Variable::OpVar(localEnv, Variable::OpVarType::Con, 0, Constant::Infinity, "eta");
	masterAddVars = Variable::OpVarArr(localEnv);
	masterAddCons = Constraint::OpLCArr(localEnv);
	masterObj = Objective::OpMinimize(localEnv, mdl->fso_ + eta);
	masterModel = Model::OpModel(localEnv, "CCG_Master_Prob");
	deterministicAddCons = Constraint::OpLCArr(localEnv);
	deterministicObj = Objective::OpObj(localEnv, Objective::OpObjSense::Min);
	deterministicModel = Model::OpModel(localEnv, "CCG_Deterministic_Prob");
	solverMode = Solver::CharSolType2OpSolType(config.getCfg<OpChar>("OPUA.Algorithm.CCG.MIPSolverMode"));
}

void Algorithm::OpAlgoCCG::OpCCGIterInfo::clear()
{
	// 释放主问题OPUA组件
	deterministicModel.release();
	deterministicObj.release();
	deterministicAddCons.releaseElements(); // 这部分是CCG迭代产生的变量，需要进行释放
	deterministicAddCons.release();
	masterModel.release();
	masterObj.release();
	masterAddCons.releaseElements(); // 这部分是CCG迭代产生的变量，需要进行释放
	masterAddCons.release();
	masterAddVars.releaseElements(); // 这部分是CCG迭代产生的变量，需要进行释放
	masterAddVars.release();
	eta.release();
}

void Algorithm::OpAlgoCCG::OpCCGIterInfo::printIterInfo(std::ostream& stream, const std::thread::id& tid)
{
	stream << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")ITER\tLB\tUB\tGAP" << std::endl;
	stream << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")"
		<< iter << '\t'
		<< lb << '\t'
		<< ub << '\t'
		<< ub - lb << std::endl;
}

struct Algorithm::OpAlgoCCG::OpOAIterInfo
{
	Solver::OpSolStatus oaSubStatus; // OA子问题求解状态
	Solver::OpSolStatus oaMasterStatus; // OA主问题求解状态
	OpLInt oaIter; // OA迭代次数
	OpLInt oaIterMax; // OA迭代次数上限
	OpLInt oaIterNoImpr; // OA无改善累计迭代次数
	OpLInt oaIterNoImprLimt; // OA无改善累计迭代次数上限
	OpFloat oaGap; // OA相对收敛间隙
	OpFloat oaLbGap; // OA下界相对收敛间隙(无改善判据)
	OpFloat oaUbGap; // OA上界相对收敛间隙(无改善判据)
	OpFloat oaUb; // 当前OA上界
	OpFloat oaLb; // 当前OA下界
	OpFloat oaPrevUb; // 上一次迭代的OA上界
	OpFloat oaPrevLb; // 上一次迭代的OA下界
	OpFloat betaVal; // OA主问题目标函数中子问题目标函数解

	Expression::OpLinExpr oaSubBasicObjExpr; // OA子问题基本目标函数表达式(不含双线性项的部分)
	Constraint::OpLCArr oaSubAddCons; // OA子问题中的动态生成的约束(需要释放)
	Objective::OpObj oaSubObj; // OA子问题目标函数对象(需要释放)
	Objective::OpObj oaSubTmpObj;  // OA子问题临时目标函数对象(用于中途更换目标函数)(需要释放)
	Model::OpModel oaSubModel; // OA子问题模型对象(需要释放)
	Variable::OpVar beta; // OA主问题目标函数中子问题估计值(需要释放)
	Constraint::OpLCArr oaMasterAddCons; // OA主问题中的动态生成的约束(需要释放)
	Objective::OpObj oaMasterObj; // OA主问题目标函数对象(需要释放)
	Model::OpModel oaMasterModel; // OA主问题模型对象(需要释放)

	void init(const OpRobustModelI* mdl, const Solver::OpConfig& config); // 初始化
	void clear(); // 清理
	void printIterInfo(std::ostream& stream, const std::thread::id& tid); // 打印迭代信息
};


void Algorithm::OpAlgoCCG::OpOAIterInfo::init(const OpRobustModelI* mdl, const Solver::OpConfig& config)
{
	// TODO：将OA与CCG分离，OA拥有自己单独的配置前缀
	OpEnv localEnv(mdl->env_);
	oaSubStatus = Solver::OpSolStatus::Unknown;
	oaMasterStatus = Solver::OpSolStatus::Unknown;
	oaIter = 1;
	oaIterMax = 100;
	oaIterNoImpr = 0;
	oaIterNoImprLimt = std::max(2, config.getCfg<OpLInt>("OPUA.Algorithm.CCG.NoImprovementIterLimit"));
	oaGap = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGGap");
	oaLbGap = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGLBGap");
	oaUbGap = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGUBGap");
	oaUb = Constant::Infinity;
	oaLb = -Constant::Infinity;
	oaPrevUb = Constant::Infinity;
	oaPrevLb = -Constant::Infinity;
	betaVal = 0.0;

	oaSubBasicObjExpr = Expression::OpLinExpr(0.0);
	oaSubAddCons = Constraint::OpLCArr(localEnv);
	oaSubObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	oaSubTmpObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	oaSubModel = Model::OpModel(localEnv);
	beta = Variable::OpVar(localEnv);
	oaMasterAddCons = Constraint::OpLCArr(localEnv);
	oaMasterObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	oaMasterModel = Model::OpModel(localEnv);
}

void Algorithm::OpAlgoCCG::OpOAIterInfo::clear()
{
	oaSubModel.release();
	oaSubTmpObj.release();
	oaSubObj.release();
	oaSubAddCons.releaseElements();
	oaSubAddCons.release();
	oaSubBasicObjExpr.clear();

	oaMasterModel.release();
	oaMasterObj.release();
	oaMasterAddCons.releaseElements();
	oaMasterAddCons.release();
	beta.release();
}

void Algorithm::OpAlgoCCG::OpOAIterInfo::printIterInfo(std::ostream& stream, const std::thread::id& tid)
{
	stream << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")OA: ITER\tLB\tUB\tGAP" << std::endl;
	stream << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")OA: "
		<< oaIter << '\t'
		<< oaLb << '\t'
		<< oaUb << '\t'
		<< oaUb - oaLb << std::endl;
}

struct Algorithm::OpAlgoCCG::OpADIterInfo
{
	Solver::OpSolStatus adSubStatus; // AD子问题求解状态
	Solver::OpSolStatus adMasterStatus; // AD主问题求解状态
	OpLInt adIter; // AD迭代次数
	OpLInt adIterMax; // AD迭代次数上限
	OpLInt adIterNoImpr; // AD无改善累计迭代次数
	OpLInt adIterNoImprLimt; // AD无改善累计迭代次数上限
	OpFloat adGap; // AD相对收敛间隙
	OpFloat adLbGap; // AD下界相对收敛间隙(无改善判据)
	OpFloat adUbGap; // AD上界相对收敛间隙(无改善判据)
	OpFloat adUb; // 当前AD上界
	OpFloat adLb; // 当前AD下界
	OpFloat adPrevUb; // 上一次迭代的AD上界
	OpFloat adPrevLb; // 上一次迭代的AD下界

	Objective::OpObj adSubObj; // AD子问题目标函数对象(需要释放)
	Objective::OpObj adSubTmpObj;  // AD子问题临时目标函数对象(用于中途更换目标函数)(需要释放)
	Model::OpModel adSubModel; // AD子问题模型对象(需要释放)
	Objective::OpObj adMasterObj; // AD主问题目标函数对象(需要释放)
	Objective::OpObj adMasterTmpObj;  // AD主问题临时目标函数对象(用于中途更换目标函数)(需要释放)
	Model::OpModel adMasterModel; // AD主问题模型对象(需要释放)

	void init(const OpRobustModelI* mdl, const Solver::OpConfig& config); // 初始化
	void clear(); // 清理
	void printIterInfo(std::ostream& stream, const std::thread::id& tid); // 打印迭代信息
};

void Algorithm::OpAlgoCCG::OpADIterInfo::init(const OpRobustModelI* mdl, const Solver::OpConfig& config)
{
	OpEnv localEnv(mdl->env_);
	adSubStatus = Solver::OpSolStatus::Unknown;
	adMasterStatus = Solver::OpSolStatus::Unknown;
	adIter = 1;
	adIterMax = 100;
	adIterNoImpr = 0;
	adIterNoImprLimt = std::max(2, config.getCfg<OpLInt>("OPUA.Algorithm.CCG.NoImprovementIterLimit"));
	adGap = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGGap");
	adLbGap = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGLBGap");
	adUbGap = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGUBGap");
	adUb = Constant::Infinity;
	adLb = -Constant::Infinity;
	adPrevUb = Constant::Infinity;
	adPrevLb = -Constant::Infinity;

	adSubObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	adSubTmpObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	adSubModel = Model::OpModel(localEnv);
	adMasterObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	adMasterTmpObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	adMasterModel = Model::OpModel(localEnv);
}

void Algorithm::OpAlgoCCG::OpADIterInfo::clear()
{
	adMasterModel.release();
	adMasterTmpObj.release();
	adMasterObj.release();
	adSubModel.release();
	adSubTmpObj.release();
	adSubObj.release();
}

void Algorithm::OpAlgoCCG::OpADIterInfo::printIterInfo(std::ostream& stream, const std::thread::id& tid)
{
	stream << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")AD: ITER\tLB\tUB\tGAP" << std::endl;
	stream << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")AD: "
		<< adIter << '\t'
		<< adLb << '\t'
		<< adUb << '\t'
		<< adUb - adLb << std::endl;
}

void Algorithm::OpAlgoCCG::extract(OpRobustModel model)
{
	mdl_ = model.getImpl();
}

OpBool Algorithm::OpAlgoCCG::solve(const Solver::OpConfig& config)
{
	using namespace Container;
	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;
	using namespace Model;
	using namespace Solver;

	// 计算成功的标志
	OpBool successful = false;
	// 环境变量
	OpEnv localEnv(mdl_->env_);
	std::thread::id tid = std::this_thread::get_id();
	std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")MemoryUsage(start): " << localEnv.getMemoryUsage() << std::endl;
	// 基本迭代信息
	OpCCGIterInfo info;
	info.init(mdl_, config);
	// 守护对象
	OpAutoFree guard([&]() {
		try
		{
			info.clear();
			std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")MemoryUsage(end): " << localEnv.getMemoryUsage() << std::endl;
		}
		catch (std::exception& ex)
		{
			std::cerr << ex.what() << std::endl;
		}
	});

	// lambda对象，用途：初始化
	auto init = [&]() {
		if (info.needSolveRobust)
		{
			switch (info.initMode)
			{
			case 0:
				mdl_->fss_.setSize(mdl_->fsv_.getSize());
				break;
			case 1:
				break;
			case 2:
				mdl_->fss_.setSize(mdl_->fsv_.getSize());
				break;
			default:
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->wrong init mode";
				throw OpExcBase(msg.str());
				break;
			}
			mdl_->ssds_.setSize(mdl_->ssdv_.getSize());
			mdl_->ssps_.setSize(mdl_->sspv_.getSize());
			//mdl_->us_.setSize(mdl_->uc_.getSize());
			info.iter++;
		}
		else
		{
			mdl_->fss_.setSize(mdl_->fsv_.getSize());
			mdl_->ssps_.setSize(mdl_->sspv_.getSize());
		}
	};
	// lambda对象，用途：生成基本主问题
	auto createBasicMasterModel = [&]() {
		// 加入主问题目标函数
		info.masterModel.setObj(info.masterObj);
		// 加入主问题约束条件(变量自动加入)
		info.masterModel.add(mdl_->fsc_);
	};
	// lambda对象，用途：固定不确定集，求解一阶段+二阶段原问题，生成确定性解
	auto createDeterministicSol1 = [&]() {

	};
	// lambda对象，用途：固定一阶段变量和不确定集，求解二阶段原问题，生成确定性解
	// 警告：不可反复调用(因为obj重复设置无效)
	auto createDeterministicSol2 = [&]() {
		auto deterministicSolver = OpAdapSol(info.solverMode, localEnv);
		OpAutoFree localGuard([&]() {
			try
			{
				deterministicSolver.release();
			}
			catch (std::exception& ex)
			{
				std::cerr << ex.what() << std::endl;
			}
		});
		// 统计一阶段变量/不确定集解
		std::unordered_map<OpVarI*, OpFloat> firstVarSols;
		for (OpULInt i = 0; i < mdl_->fsv_.getSize(); i++)
			firstVarSols.emplace(mdl_->fsv_[i].getImpl(), mdl_->fss_[i]);
		for (OpULInt i = 0; i < mdl_->uv_.getSize(); i++)
			firstVarSols.emplace(mdl_->uv_[i].getImpl(), mdl_->us_[i]);
		// 创建固定一阶段变量&不确定集变量的二阶段原问题约束条件
		for (OpULInt i = 0; i < mdl_->sspc_.getSize(); i++)
		{
			const auto& con = mdl_->sspc_[i];
			OpLinExpr expr(con.getExpr().getConstant());
			for (auto it = con.getExpr().getLBegin(); it != con.getExpr().getLEnd(); ++it)
			{
				auto iter = firstVarSols.find(it.getVar().getImpl());
				if (iter != firstVarSols.end())
					expr.addLinTerm(iter->second * it.getCoeff());
				else
					expr.addLinTerm(it.getVar(), it.getCoeff());
			}
			if (expr.getSize())
			{
				auto newCon = OpLinCon(localEnv, con.getLb(), expr, con.getUb());
				if (newCon.getImpl())
				{
					info.deterministicAddCons.add(newCon);
					info.deterministicModel.add(newCon);
				}
			}
		}
		// 形成固定一阶段变量&不确定集变量的二阶段目标函数
		{
			OpLinExpr newObjExpr(mdl_->sspo_.getConstant());
			for (auto it = mdl_->sspo_.getLBegin(); it != mdl_->sspo_.getLEnd(); ++it)
			{
				auto iter = firstVarSols.find(it.getVar().getImpl());
				if (iter != firstVarSols.end())
					newObjExpr.addLinTerm(iter->second * it.getCoeff());
				else
					newObjExpr.addLinTerm(it.getVar(), it.getCoeff());
			}
			info.deterministicObj.setLinExpr(newObjExpr);
			info.deterministicModel.setObj(info.deterministicObj);
		}
		// 求解固定一阶段变量&不确定集变量的二阶段模型
		deterministicSolver.extract(info.deterministicModel);
		deterministicSolver.setParam(config);
		deterministicSolver.solve();
		info.deterministicStatus = IntStatus2OpStatus(info.solverMode, deterministicSolver.getStatus());
		// 取值
		if (info.deterministicStatus == OpSolStatus::Optimal)
		{
			for (OpULInt i = 0; i < mdl_->sspv_.getSize(); i++)
				mdl_->ssps_[i] = deterministicSolver.getValue(mdl_->sspv_[i]);
		}
		else
		{
			std::stringstream msg;
			msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->resolve sub primal model failed";
			throw OpExcBase(msg.str());
		}
		std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")resolve sub primal model finished" << std::endl;
	};
	// lambda对象，用途：更新主问题特殊变量集的解
	auto updateMasterSpecialSolution = [&](OpLInt k) {
		if (!mdl_->fssv_.getSize())
			return;
		// 特殊变量的模型
		auto specialCons = OpLCArr(localEnv);
		auto specialQCons = OpQCArr(localEnv);
		auto specialModel = OpModel(localEnv);
		auto specialSolver = OpAdapSol(info.solverMode, localEnv);
		OpAutoFree localGuard([&]() {
			try
			{
				specialSolver.release();
				specialModel.release();
				specialQCons.releaseElements();
				specialQCons.release();
				specialCons.releaseElements();
				specialCons.release();
			}
			catch (std::exception& ex)
			{
				std::cerr << ex.what() << std::endl;
			}
		});
		// 统计一阶段变量解
		std::unordered_map<OpVarI*, OpFloat> firstVarSols;
		for (OpULInt i = 0; i < mdl_->fsv_.getSize(); i++)
			firstVarSols.emplace(mdl_->fsv_[i].getImpl(), mdl_->fss_[i]);
		for (OpULInt i = 0; i < mdl_->fssv_.getSize(); i++)
			firstVarSols.erase(mdl_->fssv_[i].getImpl());
		// 创建并加入一阶段特殊线性约束(变量自动加入)
		for (OpULInt i = 0; i < mdl_->fssc_.getSize(); i++)
		{
			const auto& con = mdl_->fssc_[i];
			OpLinExpr expr(con.getExpr().getConstant());
			for (auto it = con.getExpr().getLBegin(); it != con.getExpr().getLEnd(); ++it)
			{
				auto iter = firstVarSols.find(it.getVar().getImpl());
				if (iter != firstVarSols.end())
					expr += iter->second * it.getCoeff();
				else
					expr += it.getVar() * it.getCoeff();
			}
			if (expr.getSize())
				specialCons.add(OpLinCon(localEnv, con.getLb(), expr, con.getUb()));
		}
		specialModel.add(specialCons);
		// 创建并加入一阶段特殊二次约束(变量自动加入)
		for (OpULInt i = 0; i < mdl_->fssqc_.getSize(); i++)
		{
			const auto& con = mdl_->fssqc_[i];
			OpQuadExpr expr(con.getExpr().getConstant());
			for (auto it = con.getExpr().getLBegin(); it != con.getExpr().getLEnd(); ++it)
			{
				auto iter = firstVarSols.find(it.getVar().getImpl());
				if (iter != firstVarSols.end())
					expr.addLinTerm(iter->second * it.getCoeff());
				else
					expr.addLinTerm(it.getVar(), it.getCoeff());
			}
			for (auto it = con.getExpr().getQBegin(); it != con.getExpr().getQEnd(); ++it)
			{
				auto var1 = it.getVar1(), var2 = it.getVar2();
				auto iter1 = firstVarSols.find(var1.getImpl()), iter2 = firstVarSols.find(var2.getImpl());
				if (iter1 == firstVarSols.end() && iter2 == firstVarSols.end())
					expr.addQuadTerm(var1, var2, it.getCoeff());
				else if (iter1 == firstVarSols.end() && iter2 != firstVarSols.end())
					expr.addLinTerm(var1, iter2->second * it.getCoeff());
				else if (iter1 != firstVarSols.end() && iter2 == firstVarSols.end())
					expr.addLinTerm(var2, iter1->second * it.getCoeff());
				else if (iter1 != firstVarSols.end() && iter2 != firstVarSols.end())
					expr.addLinTerm(iter1->second * iter2->second * it.getCoeff());
			}
			if (expr.getSize())
				specialQCons.add(OpQuadCon(localEnv, con.getLb(), expr, con.getUb()));
		}
		specialModel.add(specialQCons);
		// 求解&取值
		specialSolver.extract(specialModel);
		specialSolver.setParam(config);
		specialSolver.solve();
		auto specialStatus = IntStatus2OpStatus(info.solverMode, specialSolver.getStatus());
		if (specialStatus == OpSolStatus::Optimal)
		{
			for (OpULInt i = 0; i < mdl_->fssv_.getSize(); i++)
				mdl_->fss_[mdl_->fsv_.find(mdl_->fssv_[i])] = specialSolver.getValue(mdl_->fssv_[i]);
		}
		else
		{
			std::stringstream msg;
			msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->re-solve special cons failed";
			throw OpExcBase(msg.str());
		}
		std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")re-solve special cons finished" << std::endl;
	};
	// lambda对象，用途：更新主问题的解
	auto updateMasterSolution = [&](OpLInt k) {
		if (k > 1)
		{
			auto masterSolver = OpAdapSol(info.solverMode, localEnv);
			OpAutoFree localGuard([&]() {
				try
				{
					masterSolver.release();
				}
				catch (std::exception& ex)
				{
					std::cerr << ex.what() << std::endl;
				}
			});

			masterSolver.extract(info.masterModel);
			masterSolver.setParam(config);
			masterSolver.solve();
			auto masterStatus = IntStatus2OpStatus(info.solverMode, masterSolver.getStatus());
			if (masterStatus == OpSolStatus::Optimal)
			{
				info.masterStatus = OpSolStatus::Optimal;
				info.prevLb = info.lb;
				info.lb = masterSolver.getValue(info.masterObj);
				info.etaVal = masterSolver.getValue(info.eta);
				for (OpULInt i = 0; i < mdl_->fsv_.getSize(); i++)
					mdl_->fss_[i] = masterSolver.getValue(mdl_->fsv_[i]);
				updateMasterSpecialSolution(k);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->solve master model failed";
				throw OpExcBase(msg.str());
			}
		}
		else
		{
			switch (info.initMode)
			{
			case 0:
				for (int i = 0; i < mdl_->fss_.getSize(); i++)
					mdl_->fss_[i] = 0; // 以0值作为初始解
				updateMasterSpecialSolution(k);
				break;
			case 1:
				break; // 使用自定义解
			case 2:
				createDeterministicSol1(); // 使用确定性解
				updateMasterSpecialSolution(k);
				break;
			}
		}
	};
	// lambda对象，用途：更新子问题的解(OA迭代)
	auto updateSubSolution1 = [&](OpLInt k) {
		// 创建并初始化OA迭代的基本信息
		OpOAIterInfo oaInfo;
		oaInfo.init(mdl_, config);
		Solver::OpAdapSol oaSubSolver(info.solverMode, localEnv); // OA子问题求解器(需要释放)
		Solver::OpAdapSol oaMasterSolver(info.solverMode, localEnv); // OA主问题求解器(需要释放)
		// 一些统计变量
		std::unordered_map<OpVarI*, OpFloat> firstVarSols; // 一阶段变量解
		std::unordered_map<OpVarI*, OpFloat> unSetSol; // (双线性项中)不确定集变量解 
		std::unordered_map<OpVarI*, OpFloat> dualSetSol; // (双线性项中)对偶变量解
		std::unordered_map<OpVarI*, OpVar> oaSubVars; // OA子问题变量集
		std::unordered_map<OpVarI*, OpVar> oaMasterVars; // OA主问题变量集
		std::vector<std::tuple<OpVarI*, OpVarI*, OpFloat> > bilinears; // 双线性项({不确定变量, 对偶变量, 系数})
		std::vector<std::pair<OpVarI*, OpFloat> > linears; // 线性项({不确定变量, 系数})
		// 自动释放内存
		OpAutoFree localGuard([&]() {
			try
			{
				oaSubSolver.release();
				oaMasterSolver.release();
				oaInfo.clear();
			}
			catch (std::exception& ex)
			{
				std::cerr << ex.what() << std::endl;
			}
		});

		// 统计一阶段变量解
		for (OpULInt i = 0; i < mdl_->fsv_.getSize(); i++)
			firstVarSols.emplace(mdl_->fsv_[i].getImpl(), mdl_->fss_[i]);
		// 一些通用的函数
		auto createOAVars = [&](OpVarArr& varArr, std::unordered_map<OpVarI*, OpVar>& vars) { /* 从鲁棒模型中抽取OA变量集合(便于统计) */
			for (OpULInt i = 0; i < varArr.getSize(); i++)
				vars.emplace(varArr[i].getImpl(), varArr[i]);
		};
		auto createOACons = [&](OpLCArr& conArr, OpModel& model, OpLCArr& conSave) { /* 创建OA迭代主子问题的约束 */
			for (OpULInt i = 0; i < conArr.getSize(); i++)
			{
				const auto& con = conArr[i];
				OpLinExpr expr(con.getExpr().getConstant());
				for (auto it = con.getExpr().getLBegin(); it != con.getExpr().getLEnd(); ++it)
				{
					auto iter = firstVarSols.find(it.getVar().getImpl());
					if (iter != firstVarSols.end())
						expr.addLinTerm(iter->second * it.getCoeff());
					else
						expr.addLinTerm(it.getVar(), it.getCoeff());
				}
				if (expr.getSize())
				{
					auto newCon = OpLinCon(localEnv, con.getLb(), expr, con.getUb());
					if (newCon.getImpl())
					{
						conSave.add(newCon);
						model.add(newCon);
					}
				}			
			}
		};
		auto oaQuit = [&]() { /* 判定OA迭代的退出条件 */
			if (oaInfo.oaIter > 1 ?  /* 非第一次迭代：需要统计上下界累计无改善的次数 */
				(std::abs(oaInfo.oaLb - oaInfo.oaPrevLb) / std::abs(oaInfo.oaPrevLb) <= oaInfo.oaLbGap /* 下界无改善判据 */
					&& std::abs(oaInfo.oaUb - oaInfo.oaPrevUb) / std::abs(oaInfo.oaPrevUb) <= oaInfo.oaUbGap) /* 上界无改善判据 */
				: false /* 第一次迭代：不对上下界是否各有所改善进行统计 */)
				oaInfo.oaIterNoImpr++;
			else
				oaInfo.oaIterNoImpr = 0;
			return (oaInfo.oaIter > oaInfo.oaIterMax
				|| oaInfo.oaIterNoImpr > oaInfo.oaIterNoImprLimt
				|| std::abs(oaInfo.oaUb - oaInfo.oaLb) / std::abs(oaInfo.oaUb) < oaInfo.oaGap);
		};
		// 为子问题创建变量(只是抽取并形成集合，方便统计，并非重新创建变量)
		createOAVars(mdl_->ssdv_, oaSubVars);
		// 为子问题添加约束条件(变量自动加入)
		createOACons(mdl_->ssdc_, oaInfo.oaSubModel, oaInfo.oaSubAddCons);
		// 为子问题创建基本目标函数
		for (auto it = mdl_->ssdo_.getLBegin(); it != mdl_->ssdo_.getLEnd(); ++it)
		{
			auto iter1 = firstVarSols.find(it.getVar().getImpl());
			auto iter2 = oaSubVars.find(it.getVar().getImpl());
			if (iter1 != firstVarSols.end() && iter2 == oaSubVars.end())
				oaInfo.oaSubBasicObjExpr.addLinTerm(iter1->second * it.getCoeff());
			else if (iter1 == firstVarSols.end() && iter2 != oaSubVars.end())
				oaInfo.oaSubBasicObjExpr.addLinTerm(iter2->second, it.getCoeff());
			else
				linears.emplace_back(it.getVar().getImpl(), it.getCoeff());
		}
		for (auto it = mdl_->ssdo_.getQBegin(); it != mdl_->ssdo_.getQEnd(); ++it)
		{
			auto var1Ptr = it.getVar1().getImpl(), var2Ptr = it.getVar2().getImpl();
			auto iter1 = firstVarSols.find(var1Ptr), iter2 = firstVarSols.find(var2Ptr);
			if (iter1 != firstVarSols.end() && iter2 == firstVarSols.end())
				oaInfo.oaSubBasicObjExpr.addLinTerm(oaSubVars.at(var2Ptr), iter1->second * it.getCoeff()); /*线性项*/
			else if (iter1 == firstVarSols.end() && iter2 != firstVarSols.end())
				oaInfo.oaSubBasicObjExpr.addLinTerm(oaSubVars.at(var1Ptr), iter2->second * it.getCoeff()); /*线性项*/
			else /* 双线性项*/
			{
				if (oaSubVars.find(var1Ptr) == oaSubVars.end())
					bilinears.emplace_back(var1Ptr, var2Ptr, it.getCoeff());
				else
					bilinears.emplace_back(var2Ptr, var1Ptr, it.getCoeff());
			}
		}
		// 形成不确定集初值
		//mdl_->autoBound();
		//for (auto& l : linears)
		//	unSetSol.try_emplace(l.first, mdl_->getBound(l.first, RobustStageType::Uncertainty, true)); // 不确定集的初值统一使用变量下限
		//for (auto& bl : bilinears)
		//	unSetSol.try_emplace(std::get<0>(bl), mdl_->getBound(std::get<0>(bl), RobustStageType::Uncertainty, true)); // 不确定集的初值统一使用变量下限
		for (auto& l : linears)
			unSetSol.try_emplace(l.first, mdl_->getValue(l.first, RobustStageType::Uncertainty));
		for (auto& bl : bilinears)
			unSetSol.try_emplace(std::get<0>(bl), mdl_->getValue(std::get<0>(bl), RobustStageType::Uncertainty));
		// 为主问题创建变量(只是抽取并形成集合，方便统计，并非重新创建变量)
		createOAVars(mdl_->ssdv_, oaMasterVars);
		createOAVars(mdl_->uv_, oaMasterVars);
		// 为主问题生成约束
		createOACons(mdl_->ssdc_, oaInfo.oaMasterModel, oaInfo.oaMasterAddCons);
		createOACons(mdl_->uc_, oaInfo.oaMasterModel, oaInfo.oaMasterAddCons);
		// 为主问题创建目标函数
		{
			OpLinExpr oaMasterObjExpr(oaInfo.beta);
			for (auto it = mdl_->ssdo_.getLBegin(); it != mdl_->ssdo_.getLEnd(); ++it)
			{
				auto iter = firstVarSols.find(it.getVar().getImpl());
				if (iter != firstVarSols.end())
					oaMasterObjExpr.addLinTerm(iter->second * it.getCoeff());
				else
					oaMasterObjExpr.addLinTerm(oaMasterVars.at(it.getVar().getImpl()), it.getCoeff());
			}
			for (auto it = mdl_->ssdo_.getQBegin(); it != mdl_->ssdo_.getQEnd(); ++it)
			{
				auto var1Ptr = it.getVar1().getImpl(), var2Ptr = it.getVar2().getImpl();
				auto iter1 = firstVarSols.find(var1Ptr), iter2 = firstVarSols.find(var2Ptr);
				if (iter1 != firstVarSols.end() && iter2 == firstVarSols.end())
					oaMasterObjExpr.addLinTerm(oaMasterVars.at(var2Ptr), iter1->second * it.getCoeff()); /*线性项*/
				else if (iter1 == firstVarSols.end() && iter2 != firstVarSols.end())
					oaMasterObjExpr.addLinTerm(oaMasterVars.at(var1Ptr), iter2->second * it.getCoeff()); /*线性项*/
				else
					; /*双线性项已用beta代替*/
			}
			oaInfo.oaMasterObj.setLinExpr(oaMasterObjExpr);
			oaInfo.oaMasterModel.setObj(oaInfo.oaMasterObj);
		}

		// 迭代求解
		while (!oaQuit())
		{
			// 生成子问题目标函数
			{
				OpLinExpr oaSubObjExpr(oaInfo.oaSubBasicObjExpr);
				for (auto& l : linears)
					oaSubObjExpr += l.second * unSetSol.at(l.first);
				for (auto& bl : bilinears)
					oaSubObjExpr += std::get<2>(bl) * unSetSol.at(std::get<0>(bl)) * oaSubVars.at(std::get<1>(bl));
				oaInfo.oaSubModel.setObj(oaInfo.oaSubTmpObj); // 这里为了更换目标函数，先设置一个空的临时目标函数
				oaInfo.oaSubObj.setLinExpr(oaSubObjExpr); // 此时原目标函数已经解锁，可以正常更换表达式
				oaInfo.oaSubModel.setObj(oaInfo.oaSubObj); // 最后再将原目标函数对象重新加入模型
			}
			// 子问题求解
			oaSubSolver.extract(oaInfo.oaSubModel);
			oaSubSolver.setParam(config);
			oaSubSolver.solve();
			oaInfo.oaSubStatus = IntStatus2OpStatus(info.solverMode, oaSubSolver.getStatus());
			// 子问题取值&更新下界
			if (oaInfo.oaSubStatus == OpSolStatus::Optimal)
			{
				// 取(双线性项中)对偶变量解
				for (auto& bl : bilinears)
					dualSetSol[std::get<1>(bl)] = oaSubSolver.getValue(oaSubVars.at(std::get<1>(bl)));
				// 更新Lb
				oaInfo.oaPrevLb = oaInfo.oaLb;
				oaInfo.oaLb = oaSubSolver.getValue(oaInfo.oaSubObj);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->solve OA sub model failed";
				throw OpExcBase(msg.str());
			}
			// 生成并添加主问题最优割
			{
				OpLinExpr cutExpr(0.0);
				for (auto& bl : bilinears)
				{
					auto unPtr = std::get<0>(bl), dualPtr = std::get<1>(bl);
					auto coef = std::get<2>(bl);
					cutExpr.addLinTerm(-coef * dualSetSol.at(dualPtr) * unSetSol.at(unPtr));
					cutExpr.addLinTerm(oaMasterVars.at(unPtr), coef * dualSetSol.at(dualPtr));
					cutExpr.addLinTerm(oaMasterVars.at(dualPtr), coef * unSetSol.at(unPtr));
				}
				if (cutExpr.getSize())
				{
					auto newCut = oaInfo.beta <= cutExpr;
					if (newCut.getImpl())
					{
						oaInfo.oaMasterAddCons.add(newCut);
						oaInfo.oaMasterModel.add(newCut);
					}
				}
			}
			// 主问题求解
			oaMasterSolver.extract(oaInfo.oaMasterModel);
			oaMasterSolver.setParam(config);
			oaMasterSolver.solve();
			oaInfo.oaMasterStatus = IntStatus2OpStatus(info.solverMode, oaMasterSolver.getStatus());
			// 主问题取值&更新上界
			if (oaInfo.oaMasterStatus == OpSolStatus::Optimal)
			{
				// 取不确定变量解
				for (auto& l : linears)
					unSetSol[l.first] = oaMasterSolver.getValue(oaMasterVars.at(l.first));
				for (auto& bl : bilinears)
					unSetSol[std::get<0>(bl)] = oaMasterSolver.getValue(oaMasterVars.at(std::get<0>(bl)));
				// 更新Ub
				oaInfo.oaPrevUb = oaInfo.oaUb;
				oaInfo.oaUb = oaMasterSolver.getValue(oaInfo.oaMasterObj);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->solve OA master model failed";
				throw OpExcBase(msg.str());
			}
			// 打印OA迭代信息
			oaInfo.printIterInfo(std::cout, tid);
			// 更新OA迭代信息
			oaInfo.oaIter++;
		}

		// 更新子问题解(不确定集解&对偶解)
		for (OpULInt i = 0; i < mdl_->ssdv_.getSize(); i++)
			mdl_->ssds_[i] = oaMasterSolver.getValue(mdl_->ssdv_[i]);
		for (OpULInt i = 0; i < mdl_->uv_.getSize(); i++)
			mdl_->us_[i] = oaMasterSolver.getValue(mdl_->uv_[i]);
		// 更新CCG上界
		info.prevUb = info.ub;
		if (k > 1)
		{
			auto actualObj = oaInfo.oaUb + mdl_->sspo_.getConstant();
			for (auto it = mdl_->sspo_.getLBegin(); it != mdl_->sspo_.getLEnd(); ++it)
			{
				auto iter = oaMasterVars.find(it.getVar().getImpl());
				if (iter != oaMasterVars.end())
					actualObj += it.getCoeff() * oaMasterSolver.getValue(iter->second);
			}
			info.ub = info.lb - info.etaVal + actualObj;
		}
		info.subStatus = OpSolStatus::Optimal;
	};
	// lambda对象，用途：更新子问题的解(交替迭代)
	auto updateSubSolution2 = [&](OpLInt k) {
		// 创建并初始化AD迭代的基本信息
		OpADIterInfo adInfo;
		adInfo.init(mdl_, config);
		Solver::OpAdapSol adSubSolver(info.solverMode, localEnv); // AD子问题求解器(需要释放)
		Solver::OpAdapSol adMasterSolver(info.solverMode, localEnv); // AD主问题求解器(需要释放)
		// 一些统计变量
		std::unordered_map<OpVarI*, OpFloat> firstVarSols; // 一阶段变量解
		std::unordered_map<OpVarI*, OpFloat> unSetSol; // 不确定集变量解 
		std::unordered_map<OpVarI*, OpFloat> dualSetSol; // 对偶变量解
		// 自动释放内存
		OpAutoFree localGuard([&]() {
			try
			{
				adSubSolver.release();
				adMasterSolver.release();
				adInfo.clear();
			}
			catch (std::exception& ex)
			{
				std::cerr << ex.what() << std::endl;
			}
		});

		// 一些通用的函数
		auto adQuit = [&]() { /* 判定AD迭代的退出条件 */
			if (adInfo.adIter > 1 ?  /* 非第一次迭代：需要统计上下界累计无改善的次数 */
				(std::abs(adInfo.adLb - adInfo.adPrevLb) / std::abs(adInfo.adPrevLb) <= adInfo.adLbGap /* 下界无改善判据 */
					&& std::abs(adInfo.adUb - adInfo.adPrevUb) / std::abs(adInfo.adPrevUb) <= adInfo.adUbGap) /* 上界无改善判据 */
				: false /* 第一次迭代：不对上下界是否各有所改善进行统计 */)
				adInfo.adIterNoImpr++;
			else
				adInfo.adIterNoImpr = 0;
			return (adInfo.adIter > adInfo.adIterMax
				|| adInfo.adIterNoImpr > adInfo.adIterNoImprLimt
				|| std::abs(adInfo.adUb - adInfo.adLb) / std::abs(adInfo.adUb) < adInfo.adGap);
		};
		// 统计一阶段变量解
		for (OpULInt i = 0; i < mdl_->fsv_.getSize(); i++)
			firstVarSols.emplace(mdl_->fsv_[i].getImpl(), mdl_->fss_[i]);
		// 统计不确定集解
		for (OpULInt i = 0; i < mdl_->uv_.getSize(); i++)
			unSetSol.emplace(mdl_->uv_[i].getImpl(), mdl_->us_[i]);
		// 初始化对偶解
		for (OpULInt i = 0; i < mdl_->ssdv_.getSize(); i++)
			dualSetSol.emplace(mdl_->ssdv_[i].getImpl(), 0);
		// 构建主问题(主问题只包含不确定集约束)
		adInfo.adMasterModel.add(mdl_->uc_);
		// 构建子问题(子问题只包含对偶约束)
		adInfo.adSubModel.add(mdl_->ssdc_);

		// 迭代求解
		while (!adQuit())
		{
			// 构建子问题目标函数
			{
				OpLinExpr adSubObjExpr(0);
				for (auto it = mdl_->ssdo_.getLBegin(); it != mdl_->ssdo_.getLEnd(); ++it)
						adSubObjExpr.addLinTerm(it.getVar(), it.getCoeff()); /* 线性项->系数*对偶变量 */
				for (auto it = mdl_->ssdo_.getQBegin(); it != mdl_->ssdo_.getQEnd(); ++it)
				{
					auto var1Ptr = it.getVar1().getImpl(), var2Ptr = it.getVar2().getImpl();
					auto iter1 = firstVarSols.find(var1Ptr), iter2 = firstVarSols.find(var2Ptr);
					if (iter1 != firstVarSols.end() && iter2 == firstVarSols.end())
						adSubObjExpr.addLinTerm(var2Ptr, iter1->second * it.getCoeff()); /* 线性项->系数*一阶段变量*对偶变量 */
					else if (iter1 == firstVarSols.end() && iter2 != firstVarSols.end())
						adSubObjExpr.addLinTerm(var1Ptr, iter2->second * it.getCoeff()); /* 线性项->系数*对偶变量*一阶段变量 */
					else
					{
						auto iter3 = unSetSol.find(var1Ptr), iter4 = unSetSol.find(var2Ptr);
						if (iter3 != unSetSol.end() && iter4 == unSetSol.end())
							adSubObjExpr.addLinTerm(var2Ptr, iter3->second * it.getCoeff()); /* 线性项->系数*不确定集变量*对偶变量 */
						else if (iter3 == unSetSol.end() && iter4 != unSetSol.end())
							adSubObjExpr.addLinTerm(var1Ptr, iter4->second * it.getCoeff()); /* 线性项->系数*对偶变量*不确定集变量 */
						else
							; /*不存在这项*/
					}
				}
				adInfo.adSubModel.setObj(adInfo.adSubTmpObj); // 这里为了更换目标函数，先设置一个空的临时目标函数
				adInfo.adSubObj.setLinExpr(adSubObjExpr); // 此时原目标函数已经解锁，可以正常更换表达式
				adInfo.adSubModel.setObj(adInfo.adSubObj); // 最后再将原目标函数对象重新加入模型
			}
			// 子问题求解
			adSubSolver.extract(adInfo.adSubModel);
			adSubSolver.setParam(config);
			adSubSolver.solve();
			adInfo.adSubStatus = IntStatus2OpStatus(info.solverMode, adSubSolver.getStatus());
			// 子问题取值&更新下界
			if (adInfo.adSubStatus == OpSolStatus::Optimal)
			{
				// 更新对偶变量解
				for (auto& lbd : dualSetSol)
					lbd.second = adSubSolver.getValue(lbd.first);
				// 更新Lb
				adInfo.adPrevLb = adInfo.adLb;
				adInfo.adLb = adSubSolver.getValue(adInfo.adSubObj);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->solve AD sub model failed";
				throw OpExcBase(msg.str());
			}
			// 构造主问题目标函数
			{
				OpLinExpr adMasterObjExpr(0);
				for (auto it = mdl_->ssdo_.getLBegin(); it != mdl_->ssdo_.getLEnd(); ++it)
					adMasterObjExpr.addLinTerm(dualSetSol.at(it.getVar().getImpl()) * it.getCoeff()); /* 常数项->系数*对偶变量 */
				for (auto it = mdl_->ssdo_.getQBegin(); it != mdl_->ssdo_.getQEnd(); ++it)
				{
					auto var1Ptr = it.getVar1().getImpl(), var2Ptr = it.getVar2().getImpl();
					auto iter1 = firstVarSols.find(var1Ptr), iter2 = firstVarSols.find(var2Ptr);
					if (iter1 != firstVarSols.end() && iter2 == firstVarSols.end())
						adMasterObjExpr.addLinTerm(iter1->second * dualSetSol.at(var2Ptr) * it.getCoeff()); /* 常数项->系数*一阶段变量*对偶变量 */
					else if (iter1 == firstVarSols.end() && iter2 != firstVarSols.end())
						adMasterObjExpr.addLinTerm(dualSetSol.at(var1Ptr) * iter2->second * it.getCoeff()); /* 常数项->系数*对偶变量*一阶段变量 */
					else
					{
						auto iter3 = dualSetSol.find(var1Ptr), iter4 = dualSetSol.find(var2Ptr);
						if (iter3 != dualSetSol.end() && iter4 == dualSetSol.end())
							adMasterObjExpr.addLinTerm(var2Ptr, iter3->second * it.getCoeff()); /* 线性项->系数*对偶变量*不确定集变量 */
						else if (iter3 == dualSetSol.end() && iter4 != dualSetSol.end())
							adMasterObjExpr.addLinTerm(var1Ptr, iter4->second * it.getCoeff()); /* 线性项->系数*不确定集变量*对偶变量 */
						else
							; /*不存在这项*/
					}
				}
				adInfo.adMasterModel.setObj(adInfo.adMasterTmpObj); // 这里为了更换目标函数，先设置一个空的临时目标函数
				adInfo.adMasterObj.setLinExpr(adMasterObjExpr); // 此时原目标函数已经解锁，可以正常更换表达式
				adInfo.adMasterModel.setObj(adInfo.adMasterObj); // 最后再将原目标函数对象重新加入模型
			}
			// 主问题求解
			adMasterSolver.extract(adInfo.adMasterModel);
			adMasterSolver.setParam(config);
			adMasterSolver.solve();
			adInfo.adMasterStatus = IntStatus2OpStatus(info.solverMode, adMasterSolver.getStatus());
			// 主问题取值&更新上界
			if (adInfo.adMasterStatus == OpSolStatus::Optimal)
			{
				// 更新不确定集解
				for (auto& un : unSetSol)
					un.second = adMasterSolver.getValue(un.first);
				// 更新Ub
				adInfo.adPrevUb = adInfo.adUb;
				adInfo.adUb = adMasterSolver.getValue(adInfo.adMasterObj);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->solve AD master model failed";
				throw OpExcBase(msg.str());
			}
			// 打印AD迭代信息
			adInfo.printIterInfo(std::cout, tid);
			// 更新OA迭代信息
			adInfo.adIter++;
		}
		// 更新CCG子问题解(不确定集解&对偶解)
		for (OpULInt i = 0; i < mdl_->ssdv_.getSize(); i++)
			mdl_->ssds_[i] = dualSetSol.at(mdl_->ssdv_[i].getImpl());
		for (OpULInt i = 0; i < mdl_->uv_.getSize(); i++)
			mdl_->us_[i] = unSetSol.at(mdl_->uv_[i].getImpl());
		// 更新CCG上界
		info.prevUb = info.ub;
		if (k > 1)
		{
			auto actualObj = adInfo.adUb + mdl_->sspo_.getConstant();
			for (auto it = mdl_->sspo_.getLBegin(); it != mdl_->sspo_.getLEnd(); ++it)
			{
				auto iter = unSetSol.find(it.getVar().getImpl());
				if (iter != unSetSol.end())
					actualObj += it.getCoeff() * iter->second;
			}
			info.ub = info.lb - info.etaVal + actualObj;
		}
		info.subStatus = OpSolStatus::Optimal;
	};
	// lambda对象，用途：子->主迭代，生成并添加主问题割
	auto createAndAddCuts = [&](OpLInt k) {
		// 首先复制变量并保存
		std::unordered_map<OpVarI*, OpVar> subVars;
		for (OpULInt i = 0; i < mdl_->sspv_.getSize(); i++)
		{
			const auto& var = mdl_->sspv_[i];
			subVars.emplace(var.getImpl(), OpVar(localEnv, var.getType(), var.getLb(), var.getUb(), var.getName() + "_" + std::to_string(k)));
		}
		for (const auto& v : subVars)
			info.masterAddVars.add(v.second);
		// 统计不确定集解
		std::unordered_map<OpVarI*, OpFloat> uncertaintyVarSols;
		for (OpULInt i = 0; i < mdl_->uv_.getSize(); i++)
			uncertaintyVarSols.emplace(mdl_->uv_[i].getImpl(), mdl_->us_[i]);
		// 形成新的最优割
		if (info.subStatus == OpSolStatus::Optimal)
		{
			OpLinExpr newObj(mdl_->sspo_.getConstant());
			for (auto it = mdl_->sspo_.getLBegin(); it != mdl_->sspo_.getLEnd(); ++it)
			{
				auto iter = uncertaintyVarSols.find(it.getVar().getImpl());
				if (iter != uncertaintyVarSols.end())
					newObj.addLinTerm(iter->second * it.getCoeff());
				else
					newObj.addLinTerm(subVars.at(it.getVar().getImpl()), it.getCoeff());
			}
			auto newCut = info.eta >= newObj;
			newCut.setName("ccg_cut_" + std::to_string(k));
			//auto newCut = OpLinCon(localEnv, -Constant::Infinity, newObj - info.eta, 0, "ccg_cut_" + std::to_string(k)); // 另一种写法
			if (newCut.getImpl())
			{
				info.masterAddCons.add(newCut);
				info.masterModel.add(newCut);
			}
		}
		// 形成新的可行割(重构约束)
		for (OpULInt i = 0; i < mdl_->sspc_.getSize(); i++)
		{
			const auto& originalCon = mdl_->sspc_[i];
			OpLinExpr newConExpr(originalCon.getExpr().getConstant());
			for (auto it = originalCon.getExpr().getLBegin(); it != originalCon.getExpr().getLEnd(); ++it)
			{
				auto varPtr = it.getVar().getImpl();
				if (subVars.find(varPtr) != subVars.end())
					newConExpr.addLinTerm(subVars.at(varPtr), it.getCoeff()); // 二阶段原问题变量
				else if (uncertaintyVarSols.find(varPtr) != uncertaintyVarSols.end())
					newConExpr.addLinTerm(it.getCoeff() * uncertaintyVarSols.at(varPtr)); // 不确定集变量
				else
					newConExpr.addLinTerm(it.getVar(), it.getCoeff()); // 一阶段变量
			}
			if (newConExpr.getSize())
			{
				OpLinCon newCon;
				if (mdl_->sspfc_.getVal(originalCon.getImpl()))
					newCon = newConExpr >= originalCon.getLb();
				else
					newCon = newConExpr == originalCon.getLb();
				if (newCon.getImpl())
				{
					info.masterAddCons.add(newCon);
					info.masterModel.add(newCon);
				}
			}
		}
	};
	// lambda对象，判断是否满足退出条件
	auto quit = [&]() {
		if (info.iter > 1 ? (std::abs(info.lb - info.prevLb) / std::abs(info.prevLb) <= info.lbEpsilon && std::abs(info.ub - info.prevUb) / std::abs(info.prevUb) <= info.ubEpsilon) : false)
			info.iterNoImpr++;
		else
			info.iterNoImpr = 0;
		return ((info.ub - info.lb) / (std::abs(info.ub) > info.epsilon ? std::abs(info.ub) : 1.0) <= info.epsilon)
			|| (info.iter > info.iterMax)
			|| (info.iterNoImpr >= info.iterNoImprLimt - 1);
	};

	try
	{
		// 计时器
		OpTimer watch(true);
		// 初始化
		init();
		switch (info.subProbSolveMode)
		{
		case 0:
			std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")sub problem mode: OA" << std::endl;
			break;
		case 1:
			std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")sub problem mode: AD" << std::endl;
			break;
		default:
			break;
		}
		if (info.needSolveRobust)
		{
			// 初始化基本主问题
			createBasicMasterModel();
			// 外迭代
			while (!quit())
			{
				// 更新主问题解
				updateMasterSolution(info.iter);
				// 更新子问题解
				switch (info.subProbSolveMode)
				{
				case 0:
					updateSubSolution1(info.iter);
					break;
				case 1:
					updateSubSolution2(info.iter);
					break;
				default:
					break;
				}
				// 创建子问题-主问题最优割
				createAndAddCuts(info.iter);
				// 打印CCG迭代信息
				info.printIterInfo(std::cout, tid);
				// 更新CCG迭代参数
				info.iter++;
			}
			// 重新求解第二阶段问题以获取原始变量解
			createDeterministicSol2();
			std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Time(s): " << watch.second() << std::endl;
		}
		else
		{
			createDeterministicSol1();
		}
		// 输出结果
		mdl_->fsos_ = info.lb - info.etaVal;
		mdl_->ssos_ = info.etaVal;
		successful = true;
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception: unknown" << std::endl;
	}
	return successful;
}

Algorithm::OpAlgoCCG::OpAlgoCCG()
	: mdl_(nullptr)
{

}

Algorithm::OpAlgoCCG::OpAlgoCCG(OpRobustModel model)
	: mdl_(model.getImpl())
{

}

Algorithm::OpAlgoCCG::~OpAlgoCCG()
{

}


void Algorithm::DefaultCfg4CCG(Solver::OpConfig& config)
{
	// 建立基本的配置项(分配默认值)
	config.regCfg("OPUA.Algorithm.CCG.IterMax", OpLInt(1000));
	config.regCfg("OPUA.Algorithm.CCG.CCGGap", OpFloat(1e-5));
	config.regCfg("OPUA.Algorithm.CCG.CCGLBGap", OpFloat(1e-5));
	config.regCfg("OPUA.Algorithm.CCG.CCGUBGap", OpFloat(1e-5));
	config.regCfg("OPUA.Algorithm.CCG.LogOutput", OpBool(false));
	config.regCfg("OPUA.Algorithm.CCG.LogOutputPath", OpStr("Log.txt"));
	config.regCfg("OPUA.Algorithm.CCG.SubProbSolveMode", OpLInt(0));
	config.regCfg("OPUA.Algorithm.CCG.NoImprovementIterLimit", OpLInt(3));
	config.regCfg("OPUA.Algorithm.CCG.FirstStageInitMode", OpLInt(0)); // 0-0值初始化 / 1-给定初值 / 2-确定性解
	config.regCfg("OPUA.Algorithm.CCG.GetDeterministicSolution", OpBool(false));
	config.regCfg("OPUA.Algorithm.CCG.MIPTimeLimit", OpFloat(3600));
	config.regCfg("OPUA.Algorithm.CCG.MIPGap", OpFloat(1e-5));
	config.regCfg("OPUA.Algorithm.CCG.MIPSolverMode", OpChar('G')); // MIP求解器选择：G-GRB / C-CPX / S-SCIP / M-MSK
}

Solver::OpConfig Algorithm::DefaultCfg4CCG()
{
	Solver::OpConfig config;
	DefaultCfg4CCG(config);
	return config;
}