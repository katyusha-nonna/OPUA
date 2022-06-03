#include "OPUA.Common.h"
#include "OPUA.Algorithm.h"
#include "OPUA.Math.h"
#include "OPUA.Exception.h"
#include <thread>
#include <set>
#include <map>
#include <sstream>
using namespace OPUA;

/* OPUA::Algorithm::OpMSSPModelI */

class OpMSSPMP
{
protected:
	// 第一阶段目标函数
	Expression::OpLinExpr fso_;
	// 第一阶段约束条件集
	Constraint::OpLCArr fsc_;
	// 第一阶段决策变量集
	Variable::OpVarArr fsv_;
	// 第一阶段决策变量的解
	Container::OpFloatArr fss_;
	// 第一阶段目标函数的解
	OpFloat fsos_;

	friend class Algorithm::OpMSSPModelI;
	friend class Algorithm::OpMSSPModel;
	friend class Algorithm::OpAlgoBD;
protected:
	// 初始化(仅允许构造函数使用)
	void init(OpEnvI* env);
	// 清空模型(仅允许析构函数使用)
	void clear(OpEnvI* env);
	// 更新模型(初始化解集等)
	void update(OpEnvI* env);
	// 初始化解数组
	void initSolution(OpEnvI* env);
	// 释放前准备
	void preRelease(OpEnvI* env);
	// 获取内存占用
	OpULInt getMemoryUsage() const;
protected:
	// 添加变量&约束(主问题)
	void add(Variable::OpVarArr vars);
	void add(Variable::OpVar var);
	void add(Constraint::OpLCArr cons);
	void add(Constraint::OpLinCon con);
	// 设置目标函数
	void setObj(const Expression::OpLinExpr& expr);
	// 设置解
	void setValue(Variable::OpVar var, OpFloat val);
	// 获取解
	OpFloat getValue(Variable::OpVar var) const;
	// 获取目标函数解
	OpFloat getObjValue() const;
	// 获取变量&约束数目
	OpULInt getSize(OpBool isVar) const;
public:
	OpMSSPMP();
	~OpMSSPMP();
};

void OpMSSPMP::init(OpEnvI* env)
{
	OpEnv localEnv(env);
	// 主问题
	fso_ = Expression::OpLinExpr(0.0);
	fsc_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(fsc_.getImpl());
	fsv_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(fsv_.getImpl());
	// 主问题解
	fss_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(fss_.getImpl());
	fsos_ = 0.0;
}

void OpMSSPMP::clear(OpEnvI* env)
{
	// 主问题
	fso_.clear();
	fsc_.release();
	fsv_.release();
	// 主问题解
	fss_.release();
}

void OpMSSPMP::update(OpEnvI* env)
{
	initSolution(env);
}

void OpMSSPMP::initSolution(OpEnvI* env)
{
	fss_.setSize(fsv_.getSize());
}

void OpMSSPMP::preRelease(OpEnvI* env)
{
	OpEnv localEnv(env);
	// 主问题
	localEnv.removeManagement(fsc_.getImpl());
	localEnv.removeManagement(fsv_.getImpl());
	// 主问题解
	localEnv.removeManagement(fss_.getImpl());
}

OpULInt OpMSSPMP::getMemoryUsage() const
{
	return sizeof(*this);
}

void OpMSSPMP::add(Variable::OpVarArr vars)
{
	fsv_.add(vars);
}

void OpMSSPMP::add(Variable::OpVar var)
{
	fsv_.add(var);
}

void OpMSSPMP::add(Constraint::OpLCArr cons)
{
	fsc_.add(cons);
}

void OpMSSPMP::add(Constraint::OpLinCon con)
{
	fsc_.add(con);
}

void OpMSSPMP::setObj(const Expression::OpLinExpr& expr)
{
	fso_ = expr;
}

void OpMSSPMP::setValue(Variable::OpVar var, OpFloat val)
{
	auto index(fsv_.find(var));
	if (fss_.idxValid(index))
		fss_[index] = val;
}

OpFloat OpMSSPMP::getValue(Variable::OpVar var) const
{
	OpFloat result(Constant::NaN);
	auto index(fsv_.find(var));
	if (fss_.idxValid(index))
		result = fss_[index];
	return result;
}

OpFloat OpMSSPMP::getObjValue() const
{
	return fsos_;
}

OpULInt OpMSSPMP::getSize(OpBool isVar) const
{
	return isVar ? fsv_.getSize() : fsc_.getSize();
}

OpMSSPMP::OpMSSPMP()
{

}

OpMSSPMP::~OpMSSPMP()
{

}

class OpMSSPSP
{
protected:
	// 第二阶段原问题目标函数
	Expression::OpLinExpr sspo_;
	// 第二阶段原问题约束集
	Constraint::OpLCArr sspc_;
	// 第二阶段原问题变量集
	Variable::OpVarArr sspv_;
	// 第二阶段原问题决策变量的解
	Container::OpFloatArr ssps_;
	// 第二阶段对偶问题决策变量的解
	Container::OpFloatArr ssds_;
	// 第二阶段目标函数的解
	OpFloat ssos_;

	friend class Algorithm::OpMSSPModelI;
	friend class Algorithm::OpMSSPModel;
	friend class Algorithm::OpAlgoBD;
protected:
	// 初始化(仅允许构造函数使用)
	void init(OpEnvI* env);
	// 清空模型(仅允许析构函数使用)
	void clear(OpEnvI* env);
	// 更新模型(初始化解集等)
	void update(OpEnvI* env);
	// 初始化解数组
	void initSolution(OpEnvI* env);
	// 释放前准备
	void preRelease(OpEnvI* env);
	// 获取内存占用
	OpULInt getMemoryUsage() const;
protected:
	// 添加变量&约束(主问题)
	void add(Variable::OpVarArr vars);
	void add(Variable::OpVar var);
	void add(Constraint::OpLCArr cons);
	void add(Constraint::OpLinCon con);
	// 设置目标函数
	void setObj(const Expression::OpLinExpr& expr);
	// 设置解
	void setValue(Variable::OpVar var, OpFloat val);
	void setDualValue(Constraint::OpLinCon con, OpFloat val);
	// 获取解
	OpFloat getValue(Variable::OpVar var) const;
	OpFloat getDualValue(Constraint::OpLinCon con) const;
	// 获取目标函数解
	OpFloat getObjValue() const;
	// 获取变量&约束数目
	OpULInt getSize(OpBool isVar) const;
public:
	OpMSSPSP();
	~OpMSSPSP();
};

void OpMSSPSP::init(OpEnvI* env)
{
	OpEnv localEnv(env);
	// 子问题
	sspo_ = Expression::OpLinExpr(0.0);
	sspc_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(sspc_.getImpl());
	sspv_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(sspv_.getImpl());
	// 子问题解
	ssps_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(ssps_.getImpl());
	ssds_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(ssds_.getImpl());
	ssos_ = 0.0;
}

void OpMSSPSP::clear(OpEnvI* env)
{
	// 子问题
	sspo_.clear();
	sspc_.release();
	sspv_.release();
	// 子问题解
	ssps_.release();
	ssds_.release();
}

void OpMSSPSP::update(OpEnvI* env)
{
	initSolution(env);
}

void OpMSSPSP::initSolution(OpEnvI* env)
{
	ssps_.setSize(sspv_.getSize());
	ssds_.setSize(sspc_.getSize());
}

void OpMSSPSP::preRelease(OpEnvI* env)
{
	OpEnv localEnv(env);
	// 子问题
	localEnv.removeManagement(sspc_.getImpl());
	localEnv.removeManagement(sspv_.getImpl());
	// 子问题解
	localEnv.removeManagement(ssps_.getImpl());
	localEnv.removeManagement(ssds_.getImpl());
}

OpULInt OpMSSPSP::getMemoryUsage() const
{
	return sizeof(*this);
}

void OpMSSPSP::add(Variable::OpVarArr vars)
{
	sspv_.add(vars);
}

void OpMSSPSP::add(Variable::OpVar var)
{
	sspv_.add(var);
}

void OpMSSPSP::add(Constraint::OpLCArr cons)
{
	sspc_.add(cons);
}

void OpMSSPSP::add(Constraint::OpLinCon con)
{
	sspc_.add(con);
}

void OpMSSPSP::setObj(const Expression::OpLinExpr& expr)
{
	sspo_ = expr;
}

void OpMSSPSP::setValue(Variable::OpVar var, OpFloat val)
{
	auto index(sspv_.find(var));
	if (ssps_.idxValid(index))
		ssps_[index] = val;
}

void OpMSSPSP::setDualValue(Constraint::OpLinCon con, OpFloat val)
{
	auto index(sspc_.find(con));
	if (ssds_.idxValid(index))
		ssds_[index] = val;
}

OpFloat OpMSSPSP::getValue(Variable::OpVar var) const
{
	OpFloat result(Constant::NaN);
	auto index(sspv_.find(var));
	if (ssps_.idxValid(index))
		result = ssps_[index];
	return result;
}

OpFloat OpMSSPSP::getDualValue(Constraint::OpLinCon con) const
{
	OpFloat result(Constant::NaN);
	auto index(sspc_.find(con));
	if (ssds_.idxValid(index))
		result = ssds_[index];
	return result;
}

OpFloat OpMSSPSP::getObjValue() const
{
	return ssos_;
}

OpULInt OpMSSPSP::getSize(OpBool isVar) const
{
	return isVar ? sspv_.getSize() : sspc_.getSize();
}

OpMSSPSP::OpMSSPSP()
{

}

OpMSSPSP::~OpMSSPSP()
{

}

class Algorithm::OpMSSPModelI
	: public OpImplBase
{
protected:
	// 模型名称
	OpStr mname_;
protected:
	OpMSSPMP mp_;
	std::map<OpLInt, OpMSSPSP> sps_;

	friend class Algorithm::OpMSSPModel;
	friend class Algorithm::OpAlgoBD;
protected:
	// 初始化(仅允许构造函数使用)
	void init();
	// 清空模型(仅允许析构函数使用)
	void clear();
	// 更新模型(初始化解集等)
	void update();
	// 添加一个子问题
	OpLInt addSubProb();
	void addSubProb(OpLInt idx);
	// 添加变量&约束(主问题)
	void add(Variable::OpVarArr vars);
	void add(Variable::OpVar var);
	void add(Constraint::OpLCArr cons);
	void add(Constraint::OpLinCon con);
	void add(Variable::OpVarArr vars, OpLInt idx);
	void add(Variable::OpVar var, OpLInt idx);
	void add(Constraint::OpLCArr cons, OpLInt idx);
	void add(Constraint::OpLinCon con, OpLInt idx);
	// 设置目标函数
	void setObj(const Expression::OpLinExpr& expr);
	void setObj(const Expression::OpLinExpr& expr, OpLInt idx);
	// 设置解
	void setValue(Variable::OpVar var, OpFloat val);
	void setValue(Variable::OpVar var, OpFloat val, OpLInt idx);
	void setDualValue(Constraint::OpLinCon con, OpFloat val, OpLInt idx);
	// 获取解
	OpFloat getValue(Variable::OpVar var) const;
	OpFloat getValue(Variable::OpVar var, OpLInt idx) const;
	OpFloat getDualValue(Constraint::OpLinCon con, OpLInt idx) const;
	// 获取目标函数解
	OpFloat getObjValue() const;
	OpFloat getObjValue(OpLInt idx) const;
	// 获取变量&约束数目
	OpULInt getSize(OpBool isVar) const;
	OpULInt getSize(OpBool isVar, OpLInt idx) const;
	// idx是否为节点的合法索引
	OpBool isSubProb(OpLInt idx) const;
	// 导出模型
	void write(OpStr root);
	// 释放前准备
	void preRelease();
	// 获取内存占用
	virtual OpULInt getMemoryUsage() const;
protected:
	// 构造函数
	OpMSSPModelI(OpEnvI* env);
	OpMSSPModelI(OpEnvI* env, OpStr name);
public:
	// 析构函数
	virtual ~OpMSSPModelI();
};

void Algorithm::OpMSSPModelI::init()
{
	mp_.init(env_);
}

void Algorithm::OpMSSPModelI::clear()
{
	mp_.clear(env_);
	for (auto& s : sps_)
		s.second.clear(env_);
	sps_.clear();
}

void Algorithm::OpMSSPModelI::update()
{
	mp_.update(env_);
	for (auto& s : sps_)
		s.second.update(env_);
}

OpLInt Algorithm::OpMSSPModelI::addSubProb()
{
	OpLInt idx(-1);
	idx = sps_.empty() ? 0 : (sps_.rbegin()->first + 1);
	addSubProb(idx);
	return idx;
}

void Algorithm::OpMSSPModelI::addSubProb(OpLInt idx)
{
	if(isSubProb(idx))
	{
		throw OpExcBase("[Algorithm::OpMSSPModelI::addSubProb]: Exception->"
			+ std::to_string(idx)
			+ " exist");
	}
	auto& sub = sps_[idx];
	sub.init(env_);
}

void Algorithm::OpMSSPModelI::add(Variable::OpVarArr vars)
{
	mp_.add(vars);
}

void Algorithm::OpMSSPModelI::add(Variable::OpVar var)
{
	mp_.add(var);
}

void Algorithm::OpMSSPModelI::add(Constraint::OpLCArr cons)
{
	mp_.add(cons);
}

void Algorithm::OpMSSPModelI::add(Constraint::OpLinCon con)
{
	mp_.add(con);
}

void Algorithm::OpMSSPModelI::add(Variable::OpVarArr vars, OpLInt idx)
{
	auto iter = sps_.find(idx);
	if (iter == sps_.end())
	{
		throw OpExcBase("[Algorithm::OpMSSPModelI::add]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second.add(vars);
}

void Algorithm::OpMSSPModelI::add(Variable::OpVar var, OpLInt idx)
{
	auto iter = sps_.find(idx);
	if (iter == sps_.end())
	{
		throw OpExcBase("[Algorithm::OpMSSPModelI::add]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second.add(var);
}

void Algorithm::OpMSSPModelI::add(Constraint::OpLCArr cons, OpLInt idx)
{
	auto iter = sps_.find(idx);
	if (iter == sps_.end())
	{
		throw OpExcBase("[Algorithm::OpMSSPModelI::add]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second.add(cons);
}

void Algorithm::OpMSSPModelI::add(Constraint::OpLinCon con, OpLInt idx)
{
	auto iter = sps_.find(idx);
	if (iter == sps_.end())
	{
		throw OpExcBase("[Algorithm::OpMSSPModelI::add]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second.add(con);
}

void Algorithm::OpMSSPModelI::setObj(const Expression::OpLinExpr& expr)
{
	mp_.setObj(expr);
}

void Algorithm::OpMSSPModelI::setObj(const Expression::OpLinExpr& expr, OpLInt idx)
{
	auto iter = sps_.find(idx);
	if (iter == sps_.end())
	{
		throw OpExcBase("[Algorithm::OpMSSPModelI::setObj]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second.setObj(expr);
}

void Algorithm::OpMSSPModelI::setValue(Variable::OpVar var, OpFloat val)
{
	mp_.setValue(var, val);
}

void Algorithm::OpMSSPModelI::setValue(Variable::OpVar var, OpFloat val, OpLInt idx)
{
	auto iter = sps_.find(idx);
	if (iter == sps_.end())
	{
		throw OpExcBase("[Algorithm::OpMSSPModelI::setValue]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second.setValue(var, val);
}

void Algorithm::OpMSSPModelI::setDualValue(Constraint::OpLinCon con, OpFloat val, OpLInt idx)
{
	auto iter = sps_.find(idx);
	if (iter == sps_.end())
	{
		throw OpExcBase("[Algorithm::OpMSSPModelI::setDualValue]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second.setDualValue(con, val);
}

OpFloat Algorithm::OpMSSPModelI::getValue(Variable::OpVar var) const
{
	return mp_.getValue(var);
}

OpFloat Algorithm::OpMSSPModelI::getValue(Variable::OpVar var, OpLInt idx) const
{
	auto iter = sps_.find(idx);
	if (iter == sps_.end())
	{
		throw OpExcBase("[Algorithm::OpMSSPModelI::getValue]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	return iter->second.getValue(var);
}

OpFloat Algorithm::OpMSSPModelI::getDualValue(Constraint::OpLinCon con, OpLInt idx) const
{
	auto iter = sps_.find(idx);
	if (iter == sps_.end())
	{
		throw OpExcBase("[Algorithm::OpMSSPModelI::getValue]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	return iter->second.getDualValue(con);
}

OpFloat Algorithm::OpMSSPModelI::getObjValue() const
{
	return mp_.getObjValue();
}

OpFloat Algorithm::OpMSSPModelI::getObjValue(OpLInt idx) const
{
	auto iter = sps_.find(idx);
	if (iter == sps_.end())
	{
		throw OpExcBase("[Algorithm::OpMSSPModelI::getObjValue]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	return iter->second.getObjValue();
}

OpULInt Algorithm::OpMSSPModelI::getSize(OpBool isVar) const
{
	return mp_.getSize(isVar);
}

OpULInt Algorithm::OpMSSPModelI::getSize(OpBool isVar, OpLInt idx) const
{
	auto iter = sps_.find(idx);
	if (iter == sps_.end())
	{
		throw OpExcBase("[Algorithm::OpMSSPModelI::getSize]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	return iter->second.getSize(isVar);
}

OpBool Algorithm::OpMSSPModelI::isSubProb(OpLInt idx) const
{
	auto iter = sps_.find(idx);
	if (iter == sps_.end())
		return false;
	else
		return true;
}

void Algorithm::OpMSSPModelI::write(OpStr root)
{
	// TODO：暂未实现
}

void Algorithm::OpMSSPModelI::preRelease()
{
	mp_.preRelease(env_);
	for (auto& s : sps_)
		s.second.preRelease(env_);
}

OpULInt Algorithm::OpMSSPModelI::getMemoryUsage() const
{
	//OpULInt result(sizeof(*this));
	//result += mp_.getMemoryUsage();
	//for (auto& s : sps_)
	//	result += s.second.getMemoryUsage();
	//return result;
	return sizeof(*this);
}

Algorithm::OpMSSPModelI::OpMSSPModelI(OpEnvI* env)
	: OpImplBase('M', env),
	mname_("MSSPModel_" + std::to_string(idx_))
{
	init();
}

Algorithm::OpMSSPModelI::OpMSSPModelI(OpEnvI* env, OpStr name)
	: OpImplBase('M', env),
	mname_(name)
{
	init();
}

Algorithm::OpMSSPModelI::~OpMSSPModelI()
{
	clear();
}

/* OPUA::Algorithm::OpMSSPModel */

void Algorithm::OpMSSPModel::update()
{
	static_cast<OpMSSPModelI*>(impl_)->update();
}

OpLInt Algorithm::OpMSSPModel::addSubProb()
{
	return static_cast<OpMSSPModelI*>(impl_)->addSubProb();
}

void Algorithm::OpMSSPModel::addSubProb(OpLInt idx)
{
	static_cast<OpMSSPModelI*>(impl_)->addSubProb(idx);
}

void Algorithm::OpMSSPModel::add(Variable::OpVarArr vars)
{
	static_cast<OpMSSPModelI*>(impl_)->add(vars);
}

void Algorithm::OpMSSPModel::add(Variable::OpVar var)
{
	static_cast<OpMSSPModelI*>(impl_)->add(var);
}

void Algorithm::OpMSSPModel::add(Constraint::OpLCArr cons)
{
	static_cast<OpMSSPModelI*>(impl_)->add(cons);
}

void Algorithm::OpMSSPModel::add(Constraint::OpLinCon con)
{
	static_cast<OpMSSPModelI*>(impl_)->add(con);
}

void Algorithm::OpMSSPModel::add(Variable::OpVarArr vars, OpLInt idx)
{
	static_cast<OpMSSPModelI*>(impl_)->add(vars, idx);
}

void Algorithm::OpMSSPModel::add(Variable::OpVar var, OpLInt idx)
{
	static_cast<OpMSSPModelI*>(impl_)->add(var, idx);
}

void Algorithm::OpMSSPModel::add(Constraint::OpLCArr cons, OpLInt idx)
{
	static_cast<OpMSSPModelI*>(impl_)->add(cons, idx);
}

void Algorithm::OpMSSPModel::add(Constraint::OpLinCon con, OpLInt idx)
{
	static_cast<OpMSSPModelI*>(impl_)->add(con, idx);
}

void Algorithm::OpMSSPModel::setObj(const Expression::OpLinExpr& expr)
{
	static_cast<OpMSSPModelI*>(impl_)->setObj(expr);
}

void Algorithm::OpMSSPModel::setObj(const Expression::OpLinExpr& expr, OpLInt idx)
{
	static_cast<OpMSSPModelI*>(impl_)->setObj(expr, idx);
}

void Algorithm::OpMSSPModel::setValue(Variable::OpVar var, OpFloat val)
{
	static_cast<OpMSSPModelI*>(impl_)->setValue(var, val);
}

void Algorithm::OpMSSPModel::setValue(Variable::OpVar var, OpFloat val, OpLInt idx)
{
	static_cast<OpMSSPModelI*>(impl_)->setValue(var, val, idx);
}

void Algorithm::OpMSSPModel::setDualValue(Constraint::OpLinCon con, OpFloat val, OpLInt idx)
{
	static_cast<OpMSSPModelI*>(impl_)->setDualValue(con, val, idx);
}

OpFloat Algorithm::OpMSSPModel::getValue(Variable::OpVar var) const
{
	return static_cast<OpMSSPModelI*>(impl_)->getValue(var);
}

OpFloat Algorithm::OpMSSPModel::getValue(Variable::OpVar var, OpLInt idx) const
{
	return static_cast<OpMSSPModelI*>(impl_)->getValue(var, idx);
}

OpFloat Algorithm::OpMSSPModel::getDualValue(Constraint::OpLinCon con, OpLInt idx) const
{
	return static_cast<OpMSSPModelI*>(impl_)->getDualValue(con, idx);
}

OpFloat Algorithm::OpMSSPModel::getObjValue() const
{
	return static_cast<OpMSSPModelI*>(impl_)->getObjValue();
}

OpFloat Algorithm::OpMSSPModel::getObjValue(OpLInt idx) const
{
	return static_cast<OpMSSPModelI*>(impl_)->getObjValue(idx);
}

OpULInt Algorithm::OpMSSPModel::getSize(OpBool isVar) const
{
	return static_cast<OpMSSPModelI*>(impl_)->getSize(isVar);
}

OpULInt Algorithm::OpMSSPModel::getSize(OpBool isVar, OpLInt idx) const
{
	return static_cast<OpMSSPModelI*>(impl_)->getSize(isVar, idx);
}

OpBool Algorithm::OpMSSPModel::isSubProb(OpLInt idx) const
{
	return static_cast<OpMSSPModelI*>(impl_)->isSubProb(idx);
}

void Algorithm::OpMSSPModel::write(OpStr root)
{
	static_cast<OpMSSPModelI*>(impl_)->write(root);
}

Algorithm::OpMSSPModelI* Algorithm::OpMSSPModel::getImpl() const
{
	return static_cast<OpMSSPModelI*>(impl_);
}

void Algorithm::OpMSSPModel::release()
{
	// 这里重写了release()方法，因为OpRobustModelI中包含了OPUA组件
	// 必须先执行preRelease()减少这些组件的引用计数，保证OpRobustModelI调用析构函数时，能正常释放这些组件
	if (impl_)
	{
		static_cast<OpMSSPModelI*>(impl_)->preRelease();
		static_cast<OpMSSPModelI*>(impl_)->release();
		impl_ = nullptr;
	}
}

OpBool Algorithm::OpMSSPModel::operator==(const OpMSSPModel& model) const
{
	return impl_ == model.impl_;
}

OpBool Algorithm::OpMSSPModel::operator!=(const OpMSSPModel& model) const
{
	return impl_ != model.impl_;
}

Algorithm::OpMSSPModel::OpMSSPModel()
{

}

Algorithm::OpMSSPModel::OpMSSPModel(OpMSSPModelI* impl)
{
	impl_ = impl;
}

Algorithm::OpMSSPModel::OpMSSPModel(OpEnv env)
{
	impl_= new OpMSSPModelI(env.getImpl());
}

Algorithm::OpMSSPModel::OpMSSPModel(OpEnv env, OpStr name)
{
	impl_ = new OpMSSPModelI(env.getImpl(), name);
}

Algorithm::OpMSSPModel::~OpMSSPModel()
{

}

/* OPUA::Algorithm::OpAlgoBD */

struct Algorithm::OpAlgoBD::OpBDIterInfo
{
	std::unordered_map<Variable::OpVarI*, Variable::OpVarType> masterVarType;
	std::map<OpLInt, Solver::OpSolStatus> subStatus; // 子问题求解状态
	std::vector<OpLInt> subIdxs; // 子问题编号集
	Solver::OpSolStatus masterStatus; // 主问题求解状态
	Solver::OpSolType solverMode; // 求解器模式
	OpLInt initMode; // 初始化模式选择
	OpLInt iter; // 当前迭代次数
	OpLInt iterMax; // 最大迭代次数
	OpFloat etaVal; // 主问题辅助变量解(子问题最优解)
	OpFloat etaActVal; // 子问题最优解
	OpFloat lb; // 问题下界
	OpFloat ub; // 问题上界
	OpFloat epsilon; //  收敛阈值
	OpFloat prevLb; // 上一次迭代的问题下界
	OpFloat prevUb; // 上一次迭代的问题上界

	Variable::OpVar eta; // 主问题辅助变量(子问题最优解)(需要释放)
	Constraint::OpLCArr masterAddCons; // 主问题在迭代中动态生成的约束(需要释放)
	Objective::OpObj masterObj; // 基本主问题的目标函数(需要释放)
	Model::OpModel masterModel; // 基本主问题模型对象(需要释放)
	std::map<OpLInt, std::vector<OpFloat> > subFixedDualVals; // 子问题中固定约束的对偶解(顺序和固定约束的顺序一致)
	std::map<OpLInt, Constraint::OpLCArr>  subAddCons; // 子问题在迭代中动态生成的约束(顺序和主问题变量顺序一致)(需要释放)
	std::map<OpLInt, Objective::OpObj> subObjs; // 基本子问题的目标函数(需要释放)
	std::map<OpLInt, Model::OpModel> subModels; // 基本子问题模型对象(需要释放)	

	void init(const OpMSSPModelI* mdl, const Solver::OpConfig& config); // 初始化
	void clear(); // 清理
	void printIterInfo(std::ostream& stream, const std::thread::id& tid); // 打印迭代信息
};


void Algorithm::OpAlgoBD::OpBDIterInfo::init(const OpMSSPModelI* mdl, const Solver::OpConfig& config)
{
	OpEnv localEnv(mdl->env_);
	for (OpULInt i = 0; i < mdl->mp_.fsv_.getSize(); i++)
		masterVarType.emplace(mdl->mp_.fsv_[i].getImpl(), mdl->mp_.fsv_[i].getType());
	subIdxs.reserve(mdl->sps_.size());
	for (auto& s : mdl->sps_)
		subIdxs.emplace_back(s.first);
	for (auto& s : subIdxs)
		subStatus.emplace(s, Solver::OpSolStatus::Unknown);
	masterStatus = Solver::OpSolStatus::Unknown;
	initMode = config.getCfg<OpLInt>("OPUA.Algorithm.BD.FirstStageInitMode");
	iter = 0;
	iterMax = config.getCfg<OpLInt>("OPUA.Algorithm.BD.IterMax");
	etaVal = -Constant::Infinity;
	etaActVal = -Constant::Infinity;
	lb = -Constant::Infinity;
	ub = Constant::Infinity;
	epsilon = config.getCfg<OpFloat>("OPUA.Algorithm.BD.BDGap");
	prevLb = -Constant::Infinity;
	prevUb = Constant::Infinity;

	eta = Variable::OpVar(localEnv, Variable::OpVarType::Con, 0, Constant::Infinity, "eta");
	masterAddCons = Constraint::OpLCArr(localEnv);
	masterObj = Objective::OpMinimize(localEnv, mdl->mp_.fso_ + eta);
	masterModel = Model::OpModel(localEnv, "BD_Master_Prob");
	for (auto& s : subIdxs)
	{
		subFixedDualVals.emplace(std::make_pair(s, std::vector<OpFloat>(mdl->mp_.fsv_.getSize())));
		subAddCons.emplace(std::make_pair(s, Constraint::OpLCArr(localEnv)));
		subObjs.emplace(std::make_pair(s, Objective::OpMinimize(localEnv, mdl->sps_.at(s).sspo_)));
		subModels.emplace(std::make_pair(s, Model::OpModel(localEnv, "BD_Sub_Prob_" + std::to_string(s))));
	}
	
	switch (config.getCfg<OpChar>("OPUA.Algorithm.BD.MIPSolverMode"))
	{
	case 'C':
		solverMode = Solver::OpSolType::CPX;
		break;
	case 'G':
		solverMode = Solver::OpSolType::GRB;
		break;
	case 'S':
		solverMode = Solver::OpSolType::SCIP;
		break;
	case 'M':
		solverMode = Solver::OpSolType::MSK;
		break;
	default:
		solverMode = Solver::OpSolType::GRB;
		break;
	}
}

void Algorithm::OpAlgoBD::OpBDIterInfo::clear()
{
	for (auto& s : subModels)
		s.second.release();
	for (auto& s : subObjs)
		s.second.release();
	for (auto& s : subAddCons)
	{
		s.second.releaseElements();
		s.second.release();
	}
	masterModel.release();
	masterObj.release();
	masterAddCons.releaseElements();
	masterAddCons.release();
	eta.release();
}

void Algorithm::OpAlgoBD::OpBDIterInfo::printIterInfo(std::ostream& stream, const std::thread::id& tid)
{
	stream << "[OPUA::Algorithm::OpAlgoBD::solve](Thread: " << tid << ")ITER\tLB\tUB\tGAP" << std::endl;
	stream << "[OPUA::Algorithm::OpAlgoBD::solve](Thread: " << tid << ")"
		<< iter << '\t'
		<< lb << '\t'
		<< ub << '\t'
		<< ub - lb << std::endl;
}

void Algorithm::OpAlgoBD::extract(OpMSSPModel model)
{
	mdl_ = model.getImpl();
}

OpBool Algorithm::OpAlgoBD::solve(const Solver::OpConfig& config)
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
	std::cout << "[OPUA::Algorithm::OpAlgoBD::solve](Thread: " << tid << ")MemoryUsage(start): " << localEnv.getMemoryUsage() << std::endl;
	// 基本迭代信息
	OpBDIterInfo info;
	info.init(mdl_, config);
	// 守护对象
	OpAutoFree guard([&]() {
		try
		{
			info.clear();
			std::cout << "[OPUA::Algorithm::OpAlgoBD::solve](Thread: " << tid << ")MemoryUsage(end): " << localEnv.getMemoryUsage() << std::endl;
		}
		catch (std::exception& ex)
		{
			std::cerr << ex.what() << std::endl;
		}
	});

	// lambda对象，用途：松弛主问题变量
	auto relaxMPVar = [&]() {
		for (OpULInt i = 0; i < mdl_->mp_.fsv_.getSize(); i++)
		{
			auto& var(mdl_->mp_.fsv_[i]);
			var.unlock();
			var.setType(Variable::OpVarType::Con);
			var.lock();
		}
	};
	// lambda对象，用途：还原主问题变量
	auto recoverMpVar = [&]() {
		for (OpULInt i = 0; i < mdl_->mp_.fsv_.getSize(); i++)
		{
			auto& var(mdl_->mp_.fsv_[i]);
			var.unlock();
			var.setType(info.masterVarType.at(var.getImpl()));
			var.lock();
		}
	};
	// lambda对象，用途：初始化
	auto init = [&]() {
		switch (info.initMode)
		{
		case 0:
			mdl_->mp_.fss_.setSize(mdl_->mp_.fsv_.getSize());
			break;
		case 1:
			break;
		default:
			std::stringstream msg;
			msg << "[OPUA::Algorithm::OpAlgoBD::solve](Thread: " << tid << ")Exception->wrong init mode";
			throw OpExcBase(msg.str());
			break;
		}
		for (auto& s : info.subIdxs)
		{
			auto& sp(mdl_->sps_.at(s));
			sp.ssps_.setSize(sp.sspv_.getSize());
			sp.ssds_.setSize(sp.sspc_.getSize());
		}
		info.iter++;
	};
	// lambda对象，用途：生成基本主问题
	auto createBasicMasterModel = [&]() {
		// 加入主问题目标函数
		info.masterModel.setObj(info.masterObj);
		// 加入主问题约束条件(变量自动加入)
		info.masterModel.add(mdl_->mp_.fsc_);
	};
	// lambda对象，用途：生成基本子问题
	auto createBasicSubModel = [&]() {
		for (auto& s : info.subIdxs)
		{
			auto& sp(info.subModels.at(s));
			// 加入子问题目标函数
			sp.setObj(info.subObjs.at(s));
			// 加入主问题约束条件(变量自动加入)
			sp.add(mdl_->sps_.at(s).sspc_);
		}
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
				mdl_->mp_.fsos_ = info.lb - info.etaVal;
				for (OpULInt i = 0; i < mdl_->mp_.fsv_.getSize(); i++)
					mdl_->mp_.fss_[i] = masterSolver.getValue(mdl_->mp_.fsv_[i]);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoBD::solve](Thread: " << tid << ")Exception->solve master model failed";
				throw OpExcBase(msg.str());
			}
		}
		else
		{
			switch (info.initMode)
			{
			case 0:
				for (int i = 0; i < mdl_->mp_.fss_.getSize(); i++)
					mdl_->mp_.fss_[i] = 0; // 以0值作为初始解
				break;
			case 1:
				break; // 使用自定义解
			default:
				break;
			}
		}
	};
	// lambda对象，用途：更新子问题的解
	auto updateSubSolution = [&](OpLInt k) {
		OpFloat subObjVal(0);
		relaxMPVar();
		for (auto& s : info.subIdxs)
		{
			auto subSolver = OpAdapSol(info.solverMode, localEnv);
			auto& subMdl = info.subModels.at(s);
			auto& subOMdl = mdl_->sps_.at(s);
			OpAutoFree localGuard([&]() {
				try
				{
					subSolver.release();
				}
				catch (std::exception& ex)
				{
					std::cerr << ex.what() << std::endl;
				}
			});
			subSolver.extract(subMdl);
			subSolver.setParam(config);
			subSolver.solve();
			auto subStatus = IntStatus2OpStatus(info.solverMode, subSolver.getStatus());
			if (subStatus == OpSolStatus::Optimal)
			{
				info.subStatus.at(s) = OpSolStatus::Optimal;
				auto curObjVal(subSolver.getValue(info.subObjs.at(s)));
				subOMdl.ssos_ = curObjVal;
				subObjVal += curObjVal;
				for (OpULInt i = 0; i < subOMdl.sspv_.getSize(); i++)
					subOMdl.ssps_[i] = subSolver.getValue(subOMdl.sspv_[i]);
				for (OpULInt i = 0; i < subOMdl.sspc_.getSize(); i++)
					subOMdl.ssds_[i] = subSolver.getDual(subOMdl.sspc_[i]);
				auto& fixedDualVals(info.subFixedDualVals.at(s));
				auto& fixedCons(info.subAddCons.at(s));
				for (OpULInt i = 0; i < fixedCons.getSize(); i++)
					fixedDualVals[i] = subSolver.getDual(fixedCons[i]);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoBD::solve](Thread: " << tid << ")Exception->solve sub model failed";
				throw OpExcBase(msg.str());
			}
		}
		recoverMpVar();
		info.etaActVal = subObjVal;
		info.prevUb = info.ub;
		if (k > 1)
			info.ub = info.lb - info.etaVal + subObjVal;
	};
	// lambda对象，用途：子问题向主问题返回最优割
	auto createAndAddCuts = [&](OpLInt k) {
		OpLinExpr newObj(info.etaActVal);
		for (auto& s : info.subIdxs)
		{
			auto& fixedDualVals(info.subFixedDualVals.at(s));
			for (OpULInt i = 0; i < mdl_->mp_.fsv_.getSize(); i++)
			{
				newObj.addLinTerm(mdl_->mp_.fsv_[i], fixedDualVals[i]);
				newObj.addLinTerm(-mdl_->mp_.fss_[i] * fixedDualVals[i]);
			}
		}
		auto newCut(info.eta >= newObj);
		info.masterAddCons.add(newCut);
		info.masterModel.add(newCut);
	};
	// lambda对象，用途：主问题向子问题返回整数变量固定约束
	auto createAndAddFixed = [&](OpLInt k) {
		if (k > 1)
		{
			// 重写约束
			for (auto& s : info.subIdxs)
			{
				auto& fixedCons(info.subAddCons.at(s));
				auto& subMdl(info.subModels.at(s));
				
				for (OpULInt i = 0; i < mdl_->mp_.fsv_.getSize(); i++)
				{
					auto& curFixed(fixedCons[i]);
					curFixed.unlock();
					curFixed.setLb(mdl_->mp_.fss_[i]);
					curFixed.setUb(mdl_->mp_.fss_[i]);
					curFixed.lock();
				}
			}
		}
		else
		{
			// 创建约束
			for (auto& s : info.subIdxs)
			{
				auto& fixedCons(info.subAddCons.at(s));
				for (OpULInt i = 0; i < mdl_->mp_.fsv_.getSize(); i++)
				{
					OpLinCon newFixed(localEnv, mdl_->mp_.fss_[i], mdl_->mp_.fsv_[i], mdl_->mp_.fss_[i]);
					fixedCons.add(newFixed);
				}
				info.subModels.at(s).add(fixedCons);
			}
		}
	};
	// lambda对象，判断是否满足退出条件
	auto quit = [&]() {
		return ((info.ub - info.lb) / (std::abs(info.ub) > info.epsilon ? std::abs(info.ub) : 1.0) <= info.epsilon)
			|| (info.iter > info.iterMax);
	};

	try
	{
		// 计时器
		OpTimer watch(true);
		// 初始化
		init();
		// 初始化基本主问题
		createBasicMasterModel();
		// 初始化基本子问题
		createBasicSubModel();
		// 外迭代
		while (!quit())
		{
			// 更新主问题解
			updateMasterSolution(info.iter);
			// 创建主问题-子问题整数固定约束
			createAndAddFixed(info.iter);
			// 更新子问题解
			updateSubSolution(info.iter);
			// 创建子问题-主问题最优割
			createAndAddCuts(info.iter);
			// 打印BD迭代信息
			info.printIterInfo(std::cout, tid);
			//std::cout << "y: " << mdl_->mp_.fss_ << std::endl;
			//for (auto& s : info.subIdxs)
			//{
			//	std::cout << "x(primal): " << mdl_->sps_.at(s).ssps_ << std::endl;
			//	std::cout << "x(dual): " << mdl_->sps_.at(s).ssds_ << std::endl;
			//}
			// 更新BD迭代参数
			info.iter++;
		}
		std::cout << "[OPUA::Algorithm::OpAlgoBD::solve](Thread: " << tid << ")Time(s): " << watch.second() << std::endl;
		successful = true;
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "[OPUA::Algorithm::OpAlgoBD::solve](Thread: " << tid << ")Exception->unknown" << std::endl;
	}
	return successful;
}

Algorithm::OpAlgoBD::OpAlgoBD()
	: mdl_(nullptr)
{

}

Algorithm::OpAlgoBD::OpAlgoBD(OpMSSPModel model)
	: mdl_(model.getImpl())
{

}

Algorithm::OpAlgoBD::~OpAlgoBD()
{

}

void Algorithm::DefaultCfg4BD(Solver::OpConfig& config)
{
	// 建立基本的配置项(分配默认值)
	config.regCfg("OPUA.Algorithm.BD.IterMax", OpLInt(1000));
	config.regCfg("OPUA.Algorithm.BD.BDGap", OpFloat(1e-5));
	config.regCfg("OPUA.Algorithm.BD.LogOutput", OpBool(false));
	config.regCfg("OPUA.Algorithm.BD.LogOutputPath", OpStr("Log.txt"));
	config.regCfg("OPUA.Algorithm.BD.FirstStageInitMode", OpLInt(0)); // 0-0值初始化 / 1-给定初值
	config.regCfg("OPUA.Algorithm.BD.MIPSolverMode", OpChar('C')); // MIP求解器选择：G-GRB / C-CPX / S-SCIP / M-MSK
}

Solver::OpConfig Algorithm::DefaultCfg4BD()
{
	Solver::OpConfig config;
	DefaultCfg4BD(config);
	return config;
}
