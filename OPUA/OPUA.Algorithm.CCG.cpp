#include "OPUA.Common.h"
#include "OPUA.Algorithm.h"
#include "OPUA.Math.h"
#include <thread>
#include <set>

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
protected:
	// 初始化(仅允许构造函数使用)
	void init();
	// 清空模型(仅允许析构函数使用)
	void clear();
	// 自动推导对偶
	void autoDual();
	// 自动推导变量的上下界
	void autoBound();
	// 添加变量&约束
	void add(Variable::OpVarArr vars, RobustStageType type, OpBool flag);
	void add(Variable::OpVar var, RobustStageType type, OpBool flag);
	void add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag);
	void add(Constraint::OpLinCon con, RobustStageType type, OpBool flag);
	void add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag);
	void add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag);
	// 设置目标函数
	void setObj(Expression::OpLinExpr expr, RobustStageType type);
	void setObj(Expression::OpQuadExpr expr, RobustStageType type);
	// 设置解
	void setValue(Variable::OpVar var, RobustStageType type, OpFloat val);
	// 设置边界
	void setBound(Variable::OpVar var, RobustStageType type, OpFloat val, OpBool lb);
	void setBound(Variable::OpVar var, RobustStageType type, OpFloat lbVal, OpFloat ubVal);
	// 获取解
	OpFloat getValue(Variable::OpVar var, RobustStageType type) const;
	// 获取变量&约束数目
	OpULInt getSize(RobustStageType type, OpBool isVar) const;
	// 获取边界
	OpFloat getBound(Variable::OpVar var, RobustStageType type, OpBool lb) const;
	// 导出模型
	void write(OpStr root);
	// 释放前准备
	void preRelease();
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

void Algorithm::OpRobustModelI::add(Variable::OpVarArr vars, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStage)
	{
		fsv_.add(vars);
	}
	else if (type == RobustStageType::FirstStageSpecial)
	{
		fssv_.add(vars);
	}
	else if (type == RobustStageType::SecondStagePrimal)
	{
		sspv_.add(vars);
		for (int i = 0; i < vars.getSize(); i++)
			sspfv_.add(vars[i].getImpl(), flag);
	}
	else if (type == RobustStageType::SecondStageDual)
	{
		ssdv_.add(vars);
		for (int i = 0; i < vars.getSize(); i++)
			ssdfv_.add(vars[i].getImpl(), flag);
	}
	else if (type == RobustStageType::Uncertainty)
	{
		uv_.add(vars);
	}
	else
		return;
}

void Algorithm::OpRobustModelI::add(Variable::OpVar var, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStage)
	{
		fsv_.add(var);
	}
	else if (type == RobustStageType::FirstStageSpecial)
	{
		fssv_.add(var);
	}
	else if (type == RobustStageType::SecondStagePrimal)
	{
		sspv_.add(var);
		sspfv_.add(var.getImpl(), flag);
	}
	else if (type == RobustStageType::SecondStageDual)
	{
		ssdv_.add(var);
		ssdfv_.add(var.getImpl(), flag);
	}
	else if (type == RobustStageType::Uncertainty)
	{
		uv_.add(var);
	}
	else
		return;
}

void Algorithm::OpRobustModelI::add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStage)
	{
		fsc_.add(cons);
	}
	else if (type == RobustStageType::FirstStageSpecial)
	{
		fssc_.add(cons);
	}
	else if (type == RobustStageType::SecondStagePrimal)
	{
		sspc_.add(cons);
		for (int i = 0; i < cons.getSize(); i++)
			sspfc_.add(cons[i].getImpl(), flag);
	}
	else if (type == RobustStageType::SecondStageDual)
	{
		ssdc_.add(cons);
		for (int i = 0; i < cons.getSize(); i++)
			ssdfc_.add(cons[i].getImpl(), flag);
	}
	else if (type == RobustStageType::Uncertainty)
	{
		uc_.add(cons);
	}
	else
		return;
}

void Algorithm::OpRobustModelI::add(Constraint::OpLinCon con, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStage)
	{
		fsc_.add(con);
	}
	else if (type == RobustStageType::FirstStageSpecial)
	{
		fssc_.add(con);
	}
	else if (type == RobustStageType::SecondStagePrimal)
	{
		sspc_.add(con);
		sspfc_.add(con.getImpl(), flag);
	}
	else if (type == RobustStageType::SecondStageDual)
	{
		ssdc_.add(con);
		ssdfc_.add(con.getImpl(), flag);
	}
	else if (type == RobustStageType::Uncertainty)
	{
		uc_.add(con);
	}
	else
		return;
}

void Algorithm::OpRobustModelI::add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStageSpecial)
	{
		fssqc_.add(cons);
	}
	else
		return;
}

void Algorithm::OpRobustModelI::add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStageSpecial)
	{
		fssqc_.add(con);
	}
	else
		return;
}

void Algorithm::OpRobustModelI::setObj(Expression::OpLinExpr expr, RobustStageType type)
{
	if (type == RobustStageType::FirstStage)
	{
		fso_ = expr;
	}
	else if (type == RobustStageType::SecondStagePrimal)
	{
		sspo_ = expr;
	}
	else if (type == RobustStageType::SecondStageDual)
	{
		ssdo_ = expr;
	}
	else
		return;
}

void Algorithm::OpRobustModelI::setObj(Expression::OpQuadExpr expr, RobustStageType type)
{
	if (type == RobustStageType::SecondStageDual)
	{
		ssdo_ = expr;
	}
	else
		return;
}

void Algorithm::OpRobustModelI::setValue(Variable::OpVar var, RobustStageType type, OpFloat val)
{
	auto setVal = [](auto& varArray, auto& valArray, auto var, auto val) {
		auto index = varArray.find(var); // 如果找不到相应的var，find函数返回size
		if (valArray.idxValid(index))
			valArray[index] = val;
	};
	if (type == RobustStageType::FirstStage)
		setVal(fsv_, fss_, var, val);
	else if (type == RobustStageType::SecondStagePrimal)
		setVal(sspv_, ssps_, var, val);
	else if (type == RobustStageType::SecondStageDual)
		setVal(ssdv_, ssds_, var, val);
	else if (type == RobustStageType::Uncertainty)
		setVal(uv_, us_, var, val);
	else
		return;
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
	OpFloat result(0);
	if (type == RobustStageType::FirstStage)
	{
		auto index = fsv_.find(var);
		if (fsv_.idxValid(index))
			result = fss_[index];
	}
	else if (type == RobustStageType::SecondStagePrimal)
	{
		auto index = sspv_.find(var);
		if (sspv_.idxValid(index))
			result = ssps_[index];
	}
	else if (type == RobustStageType::SecondStageDual)
	{
		auto index = ssdv_.find(var);
		if (ssdv_.idxValid(index))
			result = ssds_[index];
	}
	else if (type == RobustStageType::Uncertainty)
	{
		auto index = uv_.find(var);
		if (uv_.idxValid(index))
			result = us_[index];
	}
	return result;
}

OpULInt Algorithm::OpRobustModelI::getSize(RobustStageType type, OpBool isVar) const
{
	OpULInt result(0);
	if (type == RobustStageType::FirstStage)
		result = isVar ? fsv_.getSize() : fsc_.getSize();
	else if (type == RobustStageType::FirstStageSpecial)
		result = isVar ? fssv_.getSize() : fssc_.getSize() + fssqc_.getSize();
	else if (type == RobustStageType::SecondStagePrimal)
		result = isVar ? sspv_.getSize() : sspc_.getSize();
	else if (type == RobustStageType::SecondStageDual)
		result = isVar ? ssdv_.getSize() : ssdc_.getSize();
	else if (type == RobustStageType::Uncertainty)
		result = isVar ? uv_.getSize() : uc_.getSize();
	return result;
}

OpFloat Algorithm::OpRobustModelI::getBound(Variable::OpVar var, RobustStageType type, OpBool lb) const
{
	OpFloat bound(lb ? -Constant::Infinity : Constant::Infinity);
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

void Algorithm::OpRobustModel::autoDual()
{
	static_cast<OpRobustModelI*>(impl_)->autoDual();
}

void Algorithm::OpRobustModel::autoBound()
{
	static_cast<OpRobustModelI*>(impl_)->autoBound();
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

void OPUA::Algorithm::OpRobustModel::setObj(Expression::OpLinExpr expr, RobustStageType type)
{
	static_cast<OpRobustModelI*>(impl_)->setObj(expr, type);
}

void OPUA::Algorithm::OpRobustModel::setObj(Expression::OpQuadExpr expr, RobustStageType type)
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
