#include "OPUA.Common.h"
#include "OPUA.Algorithm.h"
#include "OPUA.Exception.h"
#include <thread>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>

using namespace OPUA;

/* OpHMNode */

// 分层模型节点
class OpHMNode
{
protected:
	OpLInt nidx_; // 节点索引
	OpEnv nenv_; // 节点的环境变量
	Expression::OpQuadExpr robj_; // (设计模块)本层优化目标函数原始值
	Expression::OpQuadExpr aobj_; // (设计模块)本层优化目标函数当前值
	Variable::OpVarArr var_; // (设计模块)本层决策变量
	Constraint::OpLCArr con_; // (设计模块)本层线性约束条件
	Constraint::OpQCArr qcon_; // (设计模块)本层二次约束条件
	Variable::OpVarArr resp_; // (分析模块)本层反馈指标
	std::map<OpLInt, Variable::OpVarArr> trgt_; // (分析模块)本层设计指标
protected:
	OpFloat aobjsol_; // 本层目标函数解(含罚项)
	OpFloat robjsol_; // 本层目标函数解(原始)
	Container::OpFloatArr varsol_; // 本层决策变量解
	Container::OpFloatArr ulbd_;  // (对偶变量)上层-本层线性罚项乘子lambda
	Container::OpFloatArr urho_; // (对偶变量)上层-本层二次罚项乘子rho
	std::map<OpLInt, Container::OpFloatArr> llbd_; // (对偶变量)下层-本层线性罚项乘子lambda
	std::map<OpLInt, Container::OpFloatArr> lrho_; // (对偶变量)下层-本层二次罚项乘子rho

	friend class Algorithm::OpHierarchicalModelI;
	friend class Algorithm::OpAlgoATC;
protected:
	void init(); // 初始化(仅允许构造函数使用)
	void clear(); // 清空模型(仅允许析构函数使用)
	void resetObj(); // 重置目标函数的当前值(重置为原始值)
	void initTarget(OpLInt idx); // 初始化对idx的设计目标
	void initSolution();// 初始化解数组
	void initLambda(OpLInt idx); // 初始化对idx的线性罚项乘子lambda
	void initRho(OpLInt idx); // 初始化对idx的二次罚项乘子rho
	void addResponse(Variable::OpVar var); // 添加反馈指标
	void addResponse(Variable::OpVarArr vars); // 添加反馈指标
	void addTarget(OpLInt idx, Variable::OpVar var); // 添加对idx的设计目标
	void addTarget(OpLInt idx, Variable::OpVarArr vars); // 添加对idx的设计目标
	void addVar(Variable::OpVar var); // 添加变量
	void addVar(Variable::OpVarArr vars); // 添加变量
	void addCon(Constraint::OpLinCon con); // 添加约束
	void addCon(Constraint::OpLCArr cons); // 添加约束
	void addCon(Constraint::OpQuadCon con); // 添加约束
	void addCon(Constraint::OpQCArr cons); // 添加约束
	void setObj(const Expression::OpQuadExpr& expr); // 设置原始目标函数
	void updateObjVal(OpFloat val); // 更新原始目标函数解
	void setValue(Variable::OpVar var, OpFloat val);	// 设置变量解
	void setValue(OpULInt order, OpFloat val);	// 设置变量解
	void setUpperLambda(Variable::OpVar var, OpFloat val); // 设置上层-本层变量var对应的线性罚项乘子lambda
	void setUpperLambda(OpULInt order, OpFloat val); // 设置上层-本层第order个线性罚项乘子lambda
	void setUpperRho(Variable::OpVar var, OpFloat val); // 设置上层-本层变量var对应的二次罚项乘子rho
	void setUpperRho(OpULInt order, OpFloat val); // 设置上层-本层第order个二次罚项乘子rho
	void setLowerLambda(OpLInt idx, Variable::OpVar var, OpFloat val); // 设置下层idx-本层变量var对应的线性罚项乘子lambda
	void setLowerLambda(OpLInt idx, OpULInt order, OpFloat val); // 设置下层idx-本层第order个线性罚项乘子lambda
	void setLowerRho(OpLInt idx, Variable::OpVar var, OpFloat val); // 设置下层idx-本层变量var对应的二次罚项乘子rho
	void setLowerRho(OpLInt idx, OpULInt order, OpFloat val); // 设置下层idx-本层第order个二次罚项乘子rho
	Variable::OpVar getVar(OpULInt order) const; // 获取第order个变量
	Variable::OpVarArr getVar() const; // 获取变量
	Constraint::OpLCArr getLinCon() const; // 获取线性约束
	Constraint::OpQCArr getQuadCon() const; // 获取二次约束
	OpFloat getValue(Variable::OpVar var) const; // 获取变量var对应的变量解
	OpFloat getValue(OpULInt order) const; // 获取第order个变量解
	Container::OpFloatArr getValue() const; // 获取变量解
	Variable::OpVarArr getResponse() const; // 获取上层-本层的反馈指标
	Variable::OpVarArr getTarget(OpLInt idx) const; // 获取下层idx-本层的设计目标
	OpFloat getUpperLambda(Variable::OpVar var) const; // 获取上层-本层变量var对应的线性罚项乘子lambda
	OpFloat getUpperLambda(OpULInt order) const; // 获取上层-本层第order个线性罚项乘子lambda
	Container::OpFloatArr getUpperLambda() const;  // 获取上层-本层线性罚项乘子lambda
	OpFloat getUpperRho(Variable::OpVar var) const; // 获取上层-本层变量var对应的二次罚项乘子rho
	OpFloat getUpperRho(OpULInt order) const; // 获取上层-本层第order个二次罚项乘子rho
	Container::OpFloatArr getUpperRho() const;  // 获取上层-本层二次罚项乘子rho
	OpFloat getLowerLambda(OpLInt idx, Variable::OpVar var) const; // 获取下层idx-本层变量var对应的线性罚项乘子lambda
	OpFloat getLowerLambda(OpLInt idx, OpULInt order) const; // 获取下层idx-本层第order个线性罚项乘子lambda
	Container::OpFloatArr getLowerLambda(OpLInt idx) const;  // 获取下层idx-本层线性罚项乘子lambda
	OpFloat getLowerRho(OpLInt idx, Variable::OpVar var) const; // 获取下层idx-本层变量var对应的二次罚项乘子rho
	OpFloat getLowerRho(OpLInt idx, OpULInt order) const; // 获取下层idx-本层第order个二次罚项乘子rho
	Container::OpFloatArr getLowerRho(OpLInt idx) const; // 获取下层idx-本层二次罚项乘子rho
	OpFloat getObjValue() const; // 获取原始目标函数解
	const Expression::OpQuadExpr& getRawObj() const; // 获取原始目标函数表达式
	const Expression::OpQuadExpr& getActualObj() const; // 获取实际目标函数(含罚项)表达式
	Expression::OpQuadExpr& getRawObj(); // 获取原始目标函数表达式
	Expression::OpQuadExpr& getActualObj(); // 获取实际目标函数(含罚项)表达式
	void release(); // 手动释放
	void preRelease(); // 释放前准备
protected:
	OpHMNode() = delete;
	OpHMNode(OpLInt idx, OpEnv env);
public:
	~OpHMNode();
};

void OpHMNode::init()
{
	OpEnv localEnv(nenv_);
	var_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(var_.getImpl());
	con_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(con_.getImpl());
	qcon_ = Constraint::OpQCArr(localEnv);
	localEnv.addManagement(qcon_.getImpl());
	resp_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(resp_.getImpl());
	varsol_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(varsol_.getImpl());
	ulbd_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(ulbd_.getImpl());
	urho_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(urho_.getImpl());
}

void OpHMNode::clear()
{
	for (auto& c : lrho_)
		c.second.release();
	for (auto& c : llbd_)
		c.second.release();
	urho_.release();
	ulbd_.release();
	varsol_.release();
	for (auto& c : trgt_)
		c.second.release();
	resp_.release();
	qcon_.release();
	con_.release();
	var_.release();
}

void OpHMNode::resetObj()
{
	aobj_ = robj_;
}

void OpHMNode::initSolution()
{
	// TODO：需要先将各个数组清零
	varsol_.setSize(var_.getSize());
	ulbd_.setSize(resp_.getSize());
	urho_.setSize(resp_.getSize());
	for (const auto& t : trgt_)
	{
		initLambda(t.first);
		initRho(t.first);
		llbd_.at(t.first).setSize(t.second.getSize());
		lrho_.at(t.first).setSize(t.second.getSize());
	}
}

void OpHMNode::initTarget(OpLInt idx)
{
	auto iter = trgt_.find(idx);
	if (iter == trgt_.end())
	{
		iter = trgt_.emplace(idx, Variable::OpVarArr(nenv_)).first;
		nenv_.addManagement(iter->second.getImpl());
	}
}

void OpHMNode::initLambda(OpLInt idx)
{
	auto iter = llbd_.find(idx);
	if (iter == llbd_.end())
	{
		iter = llbd_.emplace(idx, Container::OpFloatArr(nenv_)).first;
		nenv_.addManagement(iter->second.getImpl());
	}
}

void OpHMNode::initRho(OpLInt idx)
{
	auto iter = lrho_.find(idx);
	if (iter == lrho_.end())
	{
		iter = lrho_.emplace(idx, Container::OpFloatArr(nenv_)).first;
		nenv_.addManagement(iter->second.getImpl());
	}
}

void OpHMNode::addResponse(Variable::OpVar var)
{
	resp_.add(var);
}

void OpHMNode::addResponse(Variable::OpVarArr vars)
{
	resp_.add(vars);
}

void OpHMNode::addTarget(OpLInt idx, Variable::OpVar var)
{
	trgt_.at(idx).add(var);
}

void OpHMNode::addTarget(OpLInt idx, Variable::OpVarArr vars)
{
	trgt_.at(idx).add(vars);
}

void OpHMNode::addVar(Variable::OpVar var)
{
	var_.add(var);
}

void OpHMNode::addVar(Variable::OpVarArr vars)
{
	var_.add(vars);
}

void OpHMNode::addCon(Constraint::OpLinCon con)
{
	con_.add(con);
}

void OpHMNode::addCon(Constraint::OpLCArr cons)
{
	con_.add(cons);
}

void OpHMNode::addCon(Constraint::OpQuadCon con)
{
	qcon_.add(con);
}

void OpHMNode::addCon(Constraint::OpQCArr cons)
{
	qcon_.add(cons);
}

void OpHMNode::setObj(const Expression::OpQuadExpr& expr)
{
	robj_ = expr;
}

void OpHMNode::updateObjVal(OpFloat val)
{
	robjsol_ = val;
}

void OpHMNode::setValue(Variable::OpVar var, OpFloat val)
{
	auto order = var_.find(var);
	if (var_.idxValid(order))
		varsol_[order] = val;
}

void OpHMNode::setValue(OpULInt order, OpFloat val)
{
	varsol_[order] = val;
}

void OpHMNode::setUpperLambda(Variable::OpVar var, OpFloat val)
{
	auto order = resp_.find(var);
	if (resp_.idxValid(order))
		ulbd_[order] = val;
}

void OpHMNode::setUpperLambda(OpULInt order, OpFloat val)
{
	ulbd_[order] = val;
}

void OpHMNode::setUpperRho(Variable::OpVar var, OpFloat val)
{
	auto order = resp_.find(var);
	if (resp_.idxValid(order))
		urho_[order] = val;
}

void OpHMNode::setUpperRho(OpULInt order, OpFloat val)
{
	urho_[order] = val;
}

void OpHMNode::setLowerLambda(OpLInt idx, Variable::OpVar var, OpFloat val)
{
	const auto& lower = trgt_.at(idx);
	auto order = lower.find(var);
	if (lower.idxValid(order))
		llbd_.at(idx)[order] = val;
}

void OpHMNode::setLowerLambda(OpLInt idx, OpULInt order, OpFloat val)
{
	llbd_.at(idx)[order] = val;
}

void OpHMNode::setLowerRho(OpLInt idx, Variable::OpVar var, OpFloat val)
{
	const auto& lower = trgt_.at(idx);
	auto order = lower.find(var);
	if (lower.idxValid(order))
		lrho_.at(idx)[order] = val;
}

void OpHMNode::setLowerRho(OpLInt idx, OpULInt order, OpFloat val)
{
	lrho_.at(idx)[order] = val;
}

Variable::OpVar OpHMNode::getVar(OpULInt order) const
{
	return var_[order];
}

Variable::OpVarArr OpHMNode::getVar() const
{
	return var_;
}

Constraint::OpLCArr OpHMNode::getLinCon() const
{
	return con_;
}
Constraint::OpQCArr OpHMNode::getQuadCon() const
{
	return qcon_;
}

OpFloat OpHMNode::getValue(Variable::OpVar var) const
{
	OpFloat result(Constant::NaN);
	auto order = var_.find(var);
	if (var_.idxValid(order))
		result = varsol_[order];
	return result;
}

OpFloat OpHMNode::getValue(OpULInt order) const
{
	return varsol_[order];
}

Container::OpFloatArr OpHMNode::getValue() const
{
	return varsol_;
}

Variable::OpVarArr OpHMNode::getResponse() const
{
	return resp_;
}

Variable::OpVarArr OpHMNode::getTarget(OpLInt idx) const
{
	return trgt_.at(idx);
}

OpFloat OpHMNode::getUpperLambda(Variable::OpVar var) const
{
	OpFloat result(Constant::NaN);
	auto order = resp_.find(var);
	if (resp_.idxValid(order))
		result = ulbd_[order];
	return result;
}

OpFloat OpHMNode::getUpperLambda(OpULInt order) const
{
	return ulbd_[order];
}

Container::OpFloatArr OpHMNode::getUpperLambda() const
{
	return ulbd_;
}

OpFloat OpHMNode::getUpperRho(Variable::OpVar var) const
{
	OpFloat result(Constant::NaN);
	auto order = resp_.find(var);
	if (resp_.idxValid(order))
		result = urho_[order];
	return result;
}

OpFloat OpHMNode::getUpperRho(OpULInt order) const
{
	return urho_[order];
}

Container::OpFloatArr OpHMNode::getUpperRho() const
{
	return urho_;
}

OpFloat OpHMNode::getLowerLambda(OpLInt idx, Variable::OpVar var) const
{
	OpFloat result(Constant::NaN);
	const auto& lower = trgt_.at(idx);
	auto order = lower.find(var);
	if (lower.idxValid(order))
		result = llbd_.at(idx)[order];
	return result;
}

OpFloat OpHMNode::getLowerLambda(OpLInt idx, OpULInt order) const
{
	return llbd_.at(idx)[order];
}

Container::OpFloatArr OpHMNode::getLowerLambda(OpLInt idx) const
{
	return llbd_.at(idx);
}

OpFloat OpHMNode::getLowerRho(OpLInt idx, Variable::OpVar var) const
{
	OpFloat result(Constant::NaN);
	const auto& lower = trgt_.at(idx);
	auto order = lower.find(var);
	if (lower.idxValid(order))
		result = lrho_.at(idx)[order];
	return result;
}

OpFloat OpHMNode::getLowerRho(OpLInt idx, OpULInt order) const
{
	return lrho_.at(idx)[order];
}

Container::OpFloatArr OpHMNode::getLowerRho(OpLInt idx) const
{
	return lrho_.at(idx);
}

OpFloat OpHMNode::getObjValue() const
{
	return robjsol_;
}

const Expression::OpQuadExpr& OpHMNode::getRawObj() const
{
	return robj_;
}

const Expression::OpQuadExpr& OpHMNode::getActualObj() const
{
	return aobj_;
}

Expression::OpQuadExpr& OpHMNode::getRawObj()
{
	return robj_;
}

Expression::OpQuadExpr& OpHMNode::getActualObj()
{
	return aobj_;
}

void OpHMNode::release()
{
	delete this;
}

void OpHMNode::preRelease()
{
	OpEnv localEnv(nenv_);
	for (auto& c : lrho_)
		localEnv.removeManagement(c.second.getImpl());
	for (auto& c : llbd_)
		localEnv.removeManagement(c.second.getImpl());
	localEnv.removeManagement(urho_.getImpl());
	localEnv.removeManagement(ulbd_.getImpl());
	localEnv.removeManagement(varsol_.getImpl());
	for (auto& c : trgt_)
		localEnv.removeManagement(c.second.getImpl());
	localEnv.removeManagement(resp_.getImpl());
	localEnv.removeManagement(qcon_.getImpl());
	localEnv.removeManagement(con_.getImpl());
	localEnv.removeManagement(var_.getImpl());
}

OpHMNode::OpHMNode(OpLInt idx, OpEnv env)
	: nidx_(idx),
	nenv_(env),
	robj_(0.0),
	aobj_(0.0),
	aobjsol_(Constant::NaN),
	robjsol_(Constant::NaN)
{
	init();
}

OpHMNode::~OpHMNode()
{
	clear();
}

/* OpHMLink */

// 分层模型节点关系表
class OpHMLink
{
protected:
	OpLInt upper_; // 上层模型索引(小于0表示不存在上层模型)
	std::set<OpLInt> lower_; // 下层模型索引

	friend class Algorithm::OpAlgoATC;
	friend class Algorithm::OpHierarchicalModelI;
protected:
	OpHMLink();
public:
	~OpHMLink();
};

OpHMLink::OpHMLink()
	: upper_(-1)
{

}

OpHMLink::~OpHMLink()
{

}

/* OPUA:;Algorithm::OpHierarchicalModelI */

// 分层模型
class Algorithm::OpHierarchicalModelI
	: public OpImplBase
{
protected:
	// 模型名称
	OpStr mname_;
	// 各层模型(模型索引->模型)
	std::map<OpLInt, OpHMNode*> nodes_;
	// 层级关系表(模型索引->模型层级关系)
	std::map<OpLInt, OpHMLink*> links_;

	friend class Algorithm::OpHierarchicalModel;
	friend class Algorithm::OpAlgoATC;
protected:
	// 初始化(仅允许构造函数使用)
	void init();
	// 清空模型(仅允许析构函数使用)
	void clear();
	// 初始化解(在模型形成完毕后，设置变量解、对偶变量解之前执行)
	void initSolution();
	// 初始化模型(生成根节点)，并返回索引
	OpLInt initRoot(OpLInt idx);
	// 由一层模型idx1出发，生成他的下级模型idx2(默认自动分配)，并返回索引
	OpLInt addLower(OpLInt idx1, OpLInt idx2);
	// 添加idx1和idx2之间的一对关联变量var1和var2
	void link(OpLInt idx1, OpLInt idx2, Variable::OpVar var1, Variable::OpVar var2);
	// 添加idx1和idx2之间的一组关联变量var1和var2
	void link(OpLInt idx1, OpLInt idx2, Variable::OpVarArr vars1, Variable::OpVarArr vars2);
	// 添加idx的局部决策变量
	void add(OpLInt idx, Variable::OpVar var);
	// 添加idx的局部决策变量
	void add(OpLInt idx, Variable::OpVarArr vars);
	// 添加idx的局部线性约束
	void add(OpLInt idx, Constraint::OpLinCon con);
	// 添加idx的局部线性约束集
	void add(OpLInt idx, Constraint::OpLCArr cons);
	// 添加idx的局部二次约束
	void add(OpLInt idx, Constraint::OpQuadCon con);
	// 添加idx的局部二次约束集
	void add(OpLInt idx, Constraint::OpQCArr cons);
	// 设置idx的局部目标函数
	void setObj(OpLInt idx, const Expression::OpQuadExpr& expr);
	// 设置idx中变量var的解
	void setValue(OpLInt idx, Variable::OpVar var, OpFloat val);	
	// 获取idx中变量var对应的变量解
	OpFloat getValue(OpLInt idx, Variable::OpVar var) const;
	// 获取原始目标函数解(idx取负值则返回整体解)
	OpFloat getObjValue(OpLInt idx) const;
	// idx是否为根节点的合法索引
	OpBool isRoot(OpLInt idx) const;
	// idx是否为叶节点的合法索引
	OpBool isLeaf(OpLInt idx) const;
	// idx是否为节点的合法索引
	OpBool isNode(OpLInt idx) const;
	// 导出模型
	void write(OpStr root);
	// 释放前准备
	void preRelease();
	// 获取内存占用
	virtual OpULInt getMemoryUsage() const;
protected:
	// 构造函数
	OpHierarchicalModelI(OpEnvI* env);
	OpHierarchicalModelI(OpEnvI* env, OpStr name);
public:
	// 析构函数
	virtual ~OpHierarchicalModelI();
};

void Algorithm::OpHierarchicalModelI::init()
{

}

void Algorithm::OpHierarchicalModelI::clear()
{
	for (auto& node : nodes_)
		node.second->release(), node.second = nullptr;
}

void Algorithm::OpHierarchicalModelI::initSolution()
{
	for (auto& node : nodes_)
		node.second->initSolution();
}

OpLInt Algorithm::OpHierarchicalModelI::initRoot(OpLInt idx)
{
	if (idx < 0)
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::initRoot]: Exception->root index must be postive");
	}
	if (nodes_.size())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::initRoot]: Exception->re-initialization");
	}
	nodes_.emplace(idx, new OpHMNode(idx, OpEnv(env_)));
	links_.emplace(idx, new OpHMLink());
	return idx;
}

OpLInt Algorithm::OpHierarchicalModelI::addLower(OpLInt idx1, OpLInt idx2)
{
	if (!isNode(idx1))
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::addLower]: Exception->"
			+ std::to_string(idx1)
			+ " non-exist");
	}
	if (isNode(idx2))
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::addLower]: Exception->"
			+ std::to_string(idx2)
			+ " exist");
	}
	if (idx2 < 0)
	{
		idx2 = nodes_.rbegin()->first + 1; /*默认分配当前最大索引+1*/
	}
	nodes_.emplace(idx2, new OpHMNode(idx2, OpEnv(env_)));
	nodes_.at(idx1)->initTarget(idx2);
	links_.emplace(idx2, new OpHMLink());
	links_.at(idx2)->upper_ = idx1;
	links_.at(idx1)->lower_.emplace(idx2);
	return idx2;
}

void Algorithm::OpHierarchicalModelI::link(OpLInt idx1, OpLInt idx2, Variable::OpVar var1, Variable::OpVar var2)
{
	auto iter1 = nodes_.find(idx1);
	auto iter2 = nodes_.find(idx2);
	if (iter1 == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx1)
			+ " non-exist");
	}
	if (iter2 == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx2)
			+ " non-exist");
	}
	iter1->second->addTarget(idx2, var1);
	iter2->second->addResponse(var2);
}

void Algorithm::OpHierarchicalModelI::link(OpLInt idx1, OpLInt idx2, Variable::OpVarArr vars1, Variable::OpVarArr vars2)
{
	auto iter1 = nodes_.find(idx1);
	auto iter2 = nodes_.find(idx2);
	if (iter1 == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx1)
			+ " non-exist");
	}
	if (iter2 == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx2)
			+ " non-exist");
	}
	if (vars1.getSize() != vars2.getSize())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->lengths are not equal ");
	}
	iter1->second->addTarget(idx2, vars1);
	iter2->second->addResponse(vars2);
}

void Algorithm::OpHierarchicalModelI::add(OpLInt idx, Variable::OpVar var)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->addVar(var);
}

void Algorithm::OpHierarchicalModelI::add(OpLInt idx, Variable::OpVarArr vars)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->addVar(vars);
}

void Algorithm::OpHierarchicalModelI::add(OpLInt idx, Constraint::OpLinCon con)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->addCon(con);
}

void Algorithm::OpHierarchicalModelI::add(OpLInt idx, Constraint::OpLCArr cons)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->addCon(cons);
}

void Algorithm::OpHierarchicalModelI::add(OpLInt idx, Constraint::OpQuadCon con)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->addCon(con);
}

void Algorithm::OpHierarchicalModelI::add(OpLInt idx, Constraint::OpQCArr cons)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->addCon(cons);
}

void Algorithm::OpHierarchicalModelI::setObj(OpLInt idx, const Expression::OpQuadExpr& expr)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->setObj(expr);
}

void Algorithm::OpHierarchicalModelI::setValue(OpLInt idx, Variable::OpVar var, OpFloat val)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->setValue(var, val);
}

OpFloat Algorithm::OpHierarchicalModelI::getValue(OpLInt idx, Variable::OpVar var) const
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	return iter->second->getValue(var);
}

OpFloat Algorithm::OpHierarchicalModelI::getObjValue(OpLInt idx) const
{
	if (idx < 0)
	{
		OpFloat result(0.0);
		for (const auto& node : nodes_)
			result += node.second->getObjValue();
		return result;
	}
	else
	{
		auto iter = nodes_.find(idx);
		if (iter == nodes_.end())
		{
			throw OpExcBase("[Algorithm::OpHierarchicalModelI::link]: Exception->"
				+ std::to_string(idx)
				+ " non-exist");
		}
		return iter->second->getObjValue();
	}
}

OpBool Algorithm::OpHierarchicalModelI::isRoot(OpLInt idx) const
{
	auto iter = links_.find(idx);
	if (iter == links_.end())
		return false;
	else
		return iter->second->upper_ < 0;
}

OpBool Algorithm::OpHierarchicalModelI::isLeaf(OpLInt idx) const
{
	auto iter = links_.find(idx);
	if (iter == links_.end())
		return false;
	else
		return iter->second->lower_.empty();
}

OpBool Algorithm::OpHierarchicalModelI::isNode(OpLInt idx) const
{
	auto iter = links_.find(idx);
	if (iter == links_.end())
		return false;
	else
		return true;
}

void Algorithm::OpHierarchicalModelI::write(OpStr root)
{
	// TODO：导出模型
}

void Algorithm::OpHierarchicalModelI::preRelease()
{
	for (auto& node : nodes_)
		node.second->preRelease();
}

OpULInt Algorithm::OpHierarchicalModelI::getMemoryUsage() const
{
	return sizeof(*this);
}

Algorithm::OpHierarchicalModelI::OpHierarchicalModelI(OpEnvI* env)
	: OpImplBase('M', env),
	mname_("HierarchicalModel_" + std::to_string(idx_))
{
	init();
}

Algorithm::OpHierarchicalModelI::OpHierarchicalModelI(OpEnvI* env, OpStr name)
	: OpImplBase('M', env),
	mname_(name)
{
	init();
}

Algorithm::OpHierarchicalModelI::~OpHierarchicalModelI()
{
	clear();
}

/* OPUA:;Algorithm::OpHierarchicalModelI */

OpLInt Algorithm::OpHierarchicalModel::initRoot(OpLInt idx)
{
	return static_cast<OpHierarchicalModelI*>(impl_)->initRoot(idx);
}

void Algorithm::OpHierarchicalModel::initSolution()
{
	static_cast<OpHierarchicalModelI*>(impl_)->initSolution();
}

OpLInt Algorithm::OpHierarchicalModel::addLower(OpLInt idx1, OpLInt idx2)
{
	return static_cast<OpHierarchicalModelI*>(impl_)->addLower(idx1, idx2);
}

void Algorithm::OpHierarchicalModel::link(OpLInt idx1, OpLInt idx2, Variable::OpVar var1, Variable::OpVar var2)
{
	static_cast<OpHierarchicalModelI*>(impl_)->link(idx1, idx2, var1, var2);
}

void Algorithm::OpHierarchicalModel::link(OpLInt idx1, OpLInt idx2, Variable::OpVarArr vars1, Variable::OpVarArr vars2)
{
	static_cast<OpHierarchicalModelI*>(impl_)->link(idx1, idx2, vars1, vars2);
}

void Algorithm::OpHierarchicalModel::add(OpLInt idx, Variable::OpVar var)
{
	static_cast<OpHierarchicalModelI*>(impl_)->add(idx, var);
}

void Algorithm::OpHierarchicalModel::add(OpLInt idx, Variable::OpVarArr vars)
{
	static_cast<OpHierarchicalModelI*>(impl_)->add(idx, vars);
}

void Algorithm::OpHierarchicalModel::add(OpLInt idx, Constraint::OpLinCon con)
{
	static_cast<OpHierarchicalModelI*>(impl_)->add(idx, con);
}

void Algorithm::OpHierarchicalModel::add(OpLInt idx, Constraint::OpLCArr cons)
{
	static_cast<OpHierarchicalModelI*>(impl_)->add(idx, cons);
}

void Algorithm::OpHierarchicalModel::add(OpLInt idx, Constraint::OpQuadCon con)
{
	static_cast<OpHierarchicalModelI*>(impl_)->add(idx, con);
}

void Algorithm::OpHierarchicalModel::add(OpLInt idx, Constraint::OpQCArr cons)
{
	static_cast<OpHierarchicalModelI*>(impl_)->add(idx, cons);
}

void Algorithm::OpHierarchicalModel::setObj(OpLInt idx, const Expression::OpQuadExpr& expr)
{
	static_cast<OpHierarchicalModelI*>(impl_)->setObj(idx, expr);
}

void Algorithm::OpHierarchicalModel::setValue(OpLInt idx, Variable::OpVar var, OpFloat val)
{
	static_cast<OpHierarchicalModelI*>(impl_)->setValue(idx, var, val);
}

OpFloat Algorithm::OpHierarchicalModel::getValue(OpLInt idx, Variable::OpVar var) const
{
	return static_cast<OpHierarchicalModelI*>(impl_)->getValue(idx, var);
}

OpFloat Algorithm::OpHierarchicalModel::getObjValue(OpLInt idx) const
{
	return static_cast<OpHierarchicalModelI*>(impl_)->getObjValue(idx);
}

OpBool Algorithm::OpHierarchicalModel::isRoot(OpLInt idx) const
{
	return static_cast<OpHierarchicalModelI*>(impl_)->isRoot(idx);
}

OpBool Algorithm::OpHierarchicalModel::isLeaf(OpLInt idx) const
{
	return static_cast<OpHierarchicalModelI*>(impl_)->isLeaf(idx);
}

OpBool Algorithm::OpHierarchicalModel::isNode(OpLInt idx) const
{
	return static_cast<OpHierarchicalModelI*>(impl_)->isNode(idx);
}

void Algorithm::OpHierarchicalModel::write(OpStr root)
{
	static_cast<OpHierarchicalModelI*>(impl_)->write(root);
}

Algorithm::OpHierarchicalModelI* Algorithm::OpHierarchicalModel::getImpl() const
{
	return static_cast<OpHierarchicalModelI*>(impl_);
}

void Algorithm::OpHierarchicalModel::release()
{
	// 这里重写了release()方法，因为OpRobustModelI中包含了OPUA组件
	// 必须先执行preRelease()减少这些组件的引用计数，保证OpHierarchicalModelI调用析构函数时，能正常释放这些组件
	if (impl_)
	{
		static_cast<OpHierarchicalModelI*>(impl_)->preRelease();
		static_cast<OpHierarchicalModelI*>(impl_)->release();
		impl_ = nullptr;
	}
}

OpBool Algorithm::OpHierarchicalModel::operator==(const OpHierarchicalModel& model) const
{
	return impl_ == model.impl_;
}

OpBool Algorithm::OpHierarchicalModel::operator!=(const OpHierarchicalModel& model) const
{
	return impl_ != model.impl_;
}

Algorithm::OpHierarchicalModel::OpHierarchicalModel()
{

}

Algorithm::OpHierarchicalModel::OpHierarchicalModel(OpHierarchicalModelI* impl)
{
	impl_ = impl;
}

Algorithm::OpHierarchicalModel::OpHierarchicalModel(OpEnv env)
{
	impl_ = new OpHierarchicalModelI(env.getImpl());
}

Algorithm::OpHierarchicalModel::OpHierarchicalModel(OpEnv env, OpStr name)
{
	impl_ = new OpHierarchicalModelI(env.getImpl(), name);
}

Algorithm::OpHierarchicalModel::~OpHierarchicalModel()
{

}

/* OPUA::Algorithm::OpAlgoATC */

struct Algorithm::OpAlgoATC::OpATCIterInfo
{
	OpBool shake; //当前是否产生震荡
	OpBool shakeQuit; // 产生震荡后是否退出
	Solver::OpSolType solverMode; // 求解器模式
	OpLInt initMode; // 乘子初始化模式选择
	OpLInt iter; //当前迭代次数
	OpLInt iterMax; // 最大迭代次数
	OpLInt shakeCount; //震荡计数器
	OpLInt shakeLimit; // 最大震荡次数
	OpFloat NCC; //必要性收敛判据
	OpFloat SCC; //必要性收敛判据
	OpFloat objVal0, objVal1; //上一次/当前的目标函数值
	OpFloat cMax0, cMax1; //上一次/当前的关联变量(平均)误差最大值
	OpFloat atcGap1; // ATC必要性收敛条件
	OpFloat atcGap2;; // ATC充分性收敛条件
	OpFloat atcShakeGap; // ATC震荡判据
	OpFloat rhoUpdateFactor; // 二次罚项系数更新乘子
	OpFloat lambdaInitValue; // 一次罚项乘子统一初始化初值
	OpFloat rhoInitValue; // 二次罚项乘子统一初始化初值

	std::set<OpLInt> nodeIdxs; //所有问题节点的序号
	std::map<OpLInt, std::vector<OpFloat> > cost; //当前迭代所有节点关联变量的C(索引表示当前节点，映射值为上级目标/反馈间隙)
	Objective::OpObj objtmp; // 临时OPUA目标函数对象(用于更新model的目标函数时置换用)
	std::map<OpLInt, Objective::OpObj> objs; // 各节点OPUA模型对象
	std::map<OpLInt, Model::OpModel> models; // 各节点OPUA模型对象

	void init(const OpHierarchicalModelI* mdl, const Solver::OpConfig& config); // 初始化
	void clear(); // 清理
	void printIterInfo(std::ostream& stream, const std::thread::id& tid, OpFloat timecost); // 打印迭代信息
};

void Algorithm::OpAlgoATC::OpATCIterInfo::init(const OpHierarchicalModelI* mdl, const Solver::OpConfig& config)
{
	OpEnv localEnv(mdl->env_);
	shake = false;
	shakeQuit = config.getCfg<OpBool>("OPUA.Algorithm.ATC.ATCShakeAutoQuit");
	initMode = config.getCfg<OpLInt>("OPUA.Algorithm.ATC.InitMode");
	iter = 1;
	iterMax = config.getCfg<OpLInt>("OPUA.Algorithm.ATC.ATCIterMax");
	shakeCount = 0;
	shakeLimit = config.getCfg<OpLInt>("OPUA.Algorithm.ATC.ATCShakeCount");
	NCC = 1;
	SCC = 1;
	objVal0 = Constant::Infinity;
	objVal1 = Constant::Infinity;
	cMax0 = Constant::Infinity;
	cMax1 = Constant::Infinity;
	atcGap1 = config.getCfg<OpFloat>("OPUA.Algorithm.ATC.ATCGap1");
	atcGap2 = config.getCfg<OpFloat>("OPUA.Algorithm.ATC.ATCGap2");
	atcShakeGap = config.getCfg<OpFloat>("OPUA.Algorithm.ATC.ATCShakeGap");
	rhoUpdateFactor = config.getCfg<OpFloat>("OPUA.Algorithm.ATC.RhoUpdateFactor");
	lambdaInitValue = config.getCfg<OpFloat>("OPUA.Algorithm.ATC.LambdaInitVal");
	rhoInitValue = config.getCfg<OpFloat>("OPUA.Algorithm.ATC.RhoInitVal");
	solverMode = Solver::CharSolType2OpSolType(config.getCfg<OpChar>("OPUA.Algorithm.ATC.MIPSolverMode"));
	for (const auto& node : mdl->nodes_)
	{
		nodeIdxs.emplace(node.first);
		cost.emplace(node.first, std::vector<OpFloat>(node.second->getResponse().getSize(), Constant::Infinity));
		objs.emplace(node.first, Objective::OpObj(localEnv, Objective::OpObjSense::Min));
		models.emplace(node.first, Model::OpModel(localEnv, "ATC_Model_" + std::to_string(node.first)));
	}
	objtmp = Objective::OpMinimize(localEnv, Expression::OpLinExpr(0.0));
}

void Algorithm::OpAlgoATC::OpATCIterInfo::clear()
{
	// 释放OPUA组件
	for (auto& cp : models)
		cp.second.release();
	for (auto& cp : objs)
		cp.second.release();
	objtmp.release();
}

void Algorithm::OpAlgoATC::OpATCIterInfo::printIterInfo(std::ostream& stream, const std::thread::id& tid, OpFloat timecost)
{
	stream << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << "): ITER\tNCC\tSCC\tTime" << std::endl;
	stream << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << "): "
		<< iter << '\t'
		<< NCC << '\t'
		<< SCC << '\t'
		<< timecost << std::endl;
}

void Algorithm::OpAlgoATC::extract(OpHierarchicalModel model)
{
	mdl_ = model.getImpl();
}

OpBool Algorithm::OpAlgoATC::solve(const Solver::OpConfig& config)
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
	std::cout << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << ")MemoryUsage(start): " << localEnv.getMemoryUsage() << std::endl;
	// 基本迭代信息
	OpATCIterInfo info;
	info.init(mdl_, config);
	// 守护对象
	OpAutoFree guard([&]() {
		try
		{
			info.clear();
			std::cout << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << ")MemoryUsage(end): " << localEnv.getMemoryUsage() << std::endl;
		}
		catch (std::exception& ex)
		{
			std::cerr << ex.what() << std::endl;
		}
	});

	// lambda：初始化
	auto init = [&]() {
		// 初始化乘子
		switch (info.initMode)
		{
		case 0: /*按统一值初始化*/
			for (const auto& idx : info.nodeIdxs)
			{
				auto node = mdl_->nodes_.at(idx);
				const auto& upperIdx = mdl_->links_.at(idx)->upper_;
				const auto& lowerIdxs = mdl_->links_.at(idx)->lower_;
				node->initSolution();
				if (upperIdx >= 0)
				{
					// 设置一次乘子初值->上级目标/反馈
					auto lbd = node->getUpperLambda();
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						lbd[i] = info.lambdaInitValue;
					// 设置二次乘子初值->上级目标 / 反馈
					auto rho = node->getUpperRho();
					for (OpULInt i = 0; i < rho.getSize(); i++)
						rho[i] = info.rhoInitValue;
				}
				for (const auto& lowerIdx : lowerIdxs)
				{
					// 设置一次乘子初值->下级目标/反馈
					auto lbd = node->getLowerLambda(lowerIdx);
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						lbd[i] = info.lambdaInitValue;
					// 设置二次乘子初值->下级目标/反馈
					auto rho = node->getLowerRho(lowerIdx);
					for (OpULInt i = 0; i < rho.getSize(); i++)
						rho[i] = info.rhoInitValue;
				}
			}
			break;
		case 1: /*自行初始化，不做处理*/
			break;
		default:
			break;
		}
		// 添加约束
		for (const auto& idx : info.nodeIdxs)
		{
			auto node = mdl_->nodes_.at(idx);
			auto& curModel = info.models.at(idx);
			curModel.add(node->getLinCon());
			curModel.add(node->getQuadCon());
		}
	};
	// lambda：更新各层问题
	auto updateModel = [&]() {
		if (info.iter == 1) /*初始问题*/
		{
			// 第一次计算，不添加罚项
			const auto& idxs = info.nodeIdxs;
			for (const auto& idx : idxs)
			{
				auto node = mdl_->nodes_.at(idx);
				auto& curModel = info.models.at(idx);
				auto& curObj = info.objs.at(idx);
				const auto& curObjExpr = node->getActualObj();
				node->resetObj();
				curModel.setObj(info.objtmp);
				curObj.setLinExpr(curObjExpr.getLinTerm() + curObjExpr.getConstant());
				curObj.setQuadExpr(curObjExpr.getQuadTerm());
				curModel.setObj(curObj);
			}
		}
		else /*非初始问题*/
		{
			const auto& idxs = info.nodeIdxs;
			for (const auto& idx : idxs)
			{
				auto node = mdl_->nodes_.at(idx);
				auto& curModel = info.models.at(idx);
				auto& curObj = info.objs.at(idx);
				auto& curObjExpr = node->getActualObj();
				const auto& upperIdx = mdl_->links_.at(idx)->upper_;
				const auto& lowerIdxs = mdl_->links_.at(idx)->lower_;
				node->resetObj();
				// 计算一次罚项&二次罚项->上级目标/反馈
				if (upperIdx >= 0)
				{
					auto upperNode = mdl_->nodes_.at(upperIdx);
					auto upperTgt = upperNode->getTarget(idx);
					auto curResp = node->getResponse();
					auto lbd = node->getUpperLambda();
					auto rho = node->getUpperRho();
					std::vector<OpLinExpr> deviation(lbd.getSize(), 0.0);
					// 计算c
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						//deviation[i] += upperNode->getValue(upperTgt[i]) - curResp[i]; /*原版更新方式，收敛效率低*/
						deviation[i] += (upperNode->getValue(upperTgt[i]) + node->getValue(curResp[i])) / 2 - curResp[i];
					// 计算一次罚项
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						curObjExpr += lbd[i] * deviation[i];
					// 计算二次罚项
					for (OpULInt i = 0; i < rho.getSize(); i++)
						curObjExpr += rho[i] * rho[i] * deviation[i] * deviation[i];
				}
				// 计算一次罚项&二次罚项->下级目标/反馈
				for (const auto& lowerIdx : lowerIdxs)
				{
					auto lowerNode = mdl_->nodes_.at(lowerIdx);
					auto lowerResp = lowerNode->getResponse();
					auto curTgt = node->getTarget(lowerIdx);
					auto lbd = node->getLowerLambda(lowerIdx);
					auto rho = node->getLowerRho(lowerIdx);
					std::vector<OpLinExpr> deviation(lbd.getSize(), 0.0);
					// 计算c
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						//deviation[i] += curTgt[i] - lowerNode->getValue(lowerResp[i]);
						deviation[i] += curTgt[i] - (node->getValue(curTgt[i]) + lowerNode->getValue(lowerResp[i])) / 2;
					// 计算一次罚项
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						curObjExpr += lbd[i] * deviation[i];
					// 计算二次罚项
					for (OpULInt i = 0; i < rho.getSize(); i++)
						curObjExpr += rho[i] * rho[i] * deviation[i] * deviation[i];
				}
				curModel.setObj(info.objtmp);
				curObj.setLinExpr(curObjExpr.getLinTerm() + curObjExpr.getConstant());
				curObj.setQuadExpr(curObjExpr.getQuadTerm());
				curModel.setObj(curObj);
			}
		}
	};
	// lambda：求解各层问题
	auto solveModel = [&]() {
		const auto& idxs = info.nodeIdxs;
		for (const auto& idx : idxs)
		{
			auto& curModel = info.models.at(idx);
			OpAdapSol curSolver = OpAdapSol(info.solverMode, localEnv);
			OpAutoFree localGuard([&]() {
				try
				{
					curSolver.release();
				}
				catch (std::exception& ex)
				{
					std::cerr << ex.what() << std::endl;
				}
			});
			curSolver.extract(curModel);
			curSolver.setParam(config);
			curSolver.solve();
			//std::cout << curSolver.getStatus() << std::endl;
			auto curStatue = IntStatus2OpStatus(info.solverMode, curSolver.getStatus());
			if (curStatue == OpSolStatus::Optimal)
			{
				auto node = mdl_->nodes_.at(idx);
				auto curVars = node->getVar();
				auto curSols = node->getValue();
				// 更新目标函数解
				node->updateObjVal(curSolver.getValue(node->getRawObj()));
				// 更新变量解
				for (OpULInt i = 0; i < curVars.getSize(); i++)
					curSols[i] = curSolver.getValue(curVars[i]);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << ")Exception->solve "
					<< idx
					<< "(Iter: "
					<< info.iter
					<< ") failed";
				throw OpExcBase(msg.str());
			}
		}
	};
	// lambda：更新关联变量间隙
	auto updateNodeC = [&]() {
		const auto& idxs = info.nodeIdxs;
		for (const auto& idx : idxs) /*从根节点开始，只计算上级目标/反馈的C*/
		{
			const auto& upperIdx = mdl_->links_.at(idx)->upper_;
			if (upperIdx >= 0)
			{
				auto upperNode = mdl_->nodes_.at(upperIdx);
				auto curNode = mdl_->nodes_.at(idx);
				auto upperTgt = upperNode->getTarget(idx);
				auto curResp = curNode->getResponse();
				auto& deviation = info.cost.at(idx);
				for (OpULInt i = 0; i < curResp.getSize(); i++)
					deviation[i] = upperNode->getValue(upperTgt[i]) - curNode->getValue(curResp[i]);
			}
		}
	};
	// lambda：更新对偶
	auto updateDual = [&]() {
		const auto& idxs = info.nodeIdxs;
		for (const auto& idx : idxs)
		{
			auto node = mdl_->nodes_.at(idx);
			const auto& upperIdx = mdl_->links_.at(idx)->upper_;
			const auto& lowerIdxs = mdl_->links_.at(idx)->lower_;
			// 更新一次/二次乘子->上级目标/反馈
			if (upperIdx >= 0)
			{
				auto& deviation = info.cost.at(idx);
				auto lbd = node->getUpperLambda();
				auto rho = node->getUpperRho();
				// 更新一次乘子
				for (OpULInt i = 0; i < lbd.getSize(); i++)
					lbd[i] += 2 * rho[i] * rho[i] * deviation[i];
				// 更新二次乘子
				for (OpULInt i = 0; i < rho.getSize(); i++)
					rho[i] *= info.rhoUpdateFactor;
			}
			// 更新一次/二次乘子->下级目标/反馈
			for (const auto& lowerIdx : lowerIdxs)
			{
				auto& deviation = info.cost.at(lowerIdx);
				auto lbd = node->getLowerLambda(lowerIdx);
				auto rho = node->getLowerRho(lowerIdx);
				// 更新一次乘子
				for (OpULInt i = 0; i < lbd.getSize(); i++)
					lbd[i] += 2 * rho[i] * rho[i] * deviation[i];
				// 更新二次乘子
				for (OpULInt i = 0; i < rho.getSize(); i++)
					rho[i] *= info.rhoUpdateFactor;
			}
		}
	};
	// lambda：更新收敛判据
	auto updateLog = [&]() {
		const auto& idxs = info.nodeIdxs;
		// 更新必要性判据
		info.cMax0 = info.cMax1;
		info.cMax1 = Constant::Infinity;
		for (const auto& idx : idxs)
		{
			auto& deviation = info.cost.at(idx);
			if (deviation.size() > 0)
			{
				OpFloat avrg(0);
				for (const auto& c : deviation)
					avrg += std::abs(c);
				avrg /= deviation.size();
				info.cMax1 = std::min(info.cMax1, avrg);
			}
		}
		info.NCC = info.cMax1;
		// 更新充分性判据
		info.objVal0 = info.objVal1;
		info.objVal1 = 0;
		for (const auto& idx : idxs)
			info.objVal1 += mdl_->nodes_.at(idx)->getObjValue();
		info.SCC = std::abs(info.objVal1 - info.objVal0) / std::abs(info.objVal1);
		// 更新震荡判据
		if (std::abs(info.cMax1 - info.cMax0) < info.atcShakeGap)
			info.shakeCount++;
		else
			info.shakeCount = 0;
		if (info.shakeCount >= info.shakeLimit)
			info.shake = true;
		else
			info.shake = false;
	};
	// lambda：判断退出条件
	auto quit = [&]() {
		return (info.iter > info.iterMax) ||
			(info.NCC < info.atcGap1&& info.SCC < info.atcGap2) ||
			(info.shakeQuit && info.shake);
	};

	try
	{	
		// 计时器
		OpTimer globalWatch(true);
		OpTimer localWatch(true);
		std::cout << "[OPUA::Algorithm::OpAlgoATC::solvee](Thread: " << tid << "): Start" << std::endl;
		std::cout << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << "): init" << std::endl;
		// 初始化
		init();
		// 迭代
		while (!quit())
		{
			localWatch.restart();
			std::cout << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << "): update models" << std::endl;
			updateModel();
			std::cout << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << "): solve models(serial)" << std::endl;
			solveModel();
			std::cout << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << "): update deviation of associated variables" << std::endl;
			updateNodeC();
			//print();
			std::cout << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << "): update dual info" << std::endl;
			updateDual();
			std::cout << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << "): update convergence criterion" << std::endl;
			updateLog();
			info.printIterInfo(std::cout, tid, localWatch.second());
			info.iter++;
		}
		std::cout << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << "): Finish" << std::endl;
		std::cout << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << "): Time: " << globalWatch.second() << std::endl;
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

Algorithm::OpAlgoATC::OpAlgoATC()
	: mdl_(nullptr)
{

}

Algorithm::OpAlgoATC::OpAlgoATC(OpHierarchicalModel model)
	: mdl_(model.getImpl())
{

}

Algorithm::OpAlgoATC::~OpAlgoATC()
{

}


void Algorithm::DefaultCfg4ATC(Solver::OpConfig& config)
{
	// 建立基本的配置项(分配默认值)
	config.regCfg("OPUA.Algorithm.ATC.InitMode", OpLInt(0));
	config.regCfg("OPUA.Algorithm.ATC.LambdaInitVal", OpFloat(1));
	config.regCfg("OPUA.Algorithm.ATC.RhoInitVal", OpFloat(1));
	config.regCfg("OPUA.Algorithm.ATC.RhoUpdateFactor", OpFloat(2));
	config.regCfg("OPUA.Algorithm.ATC.ATCIterMax", OpLInt(1000));
	config.regCfg("OPUA.Algorithm.ATC.ATCGap1", OpFloat(1e-3));
	config.regCfg("OPUA.Algorithm.ATC.ATCGap2", OpFloat(1e-3));
	config.regCfg("OPUA.Algorithm.ATC.ATCShakeGap", OpFloat(1e-3));
	config.regCfg("OPUA.Algorithm.ATC.ATCShakeCount", OpLInt(3));
	config.regCfg("OPUA.Algorithm.ATC.ATCShakeAutoQuit", OpBool(true));
	config.regCfg("OPUA.Algorithm.ATC.MIPSolverMode", OpChar('G'));
}

Solver::OpConfig Algorithm::DefaultCfg4ATC()
{
	Solver::OpConfig config;
	DefaultCfg4ATC(config);
	return config;
}
