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

// �ֲ�ģ�ͽڵ�
class OpHMNode
{
protected:
	OpLInt nidx_; // �ڵ�����
	OpEnv nenv_; // �ڵ�Ļ�������
	Expression::OpQuadExpr robj_; // (���ģ��)�����Ż�Ŀ�꺯��ԭʼֵ
	Expression::OpQuadExpr aobj_; // (���ģ��)�����Ż�Ŀ�꺯����ǰֵ
	Variable::OpVarArr var_; // (���ģ��)������߱���
	Constraint::OpLCArr con_; // (���ģ��)��������Լ������
	Constraint::OpQCArr qcon_; // (���ģ��)�������Լ������
	Variable::OpVarArr resp_; // (����ģ��)���㷴��ָ��
	std::map<OpLInt, Variable::OpVarArr> trgt_; // (����ģ��)�������ָ��
protected:
	OpFloat aobjsol_; // ����Ŀ�꺯����(������)
	OpFloat robjsol_; // ����Ŀ�꺯����(ԭʼ)
	Container::OpFloatArr varsol_; // ������߱�����
	Container::OpFloatArr ulbd_;  // (��ż����)�ϲ�-�������Է������lambda
	Container::OpFloatArr urho_; // (��ż����)�ϲ�-������η������rho
	std::map<OpLInt, Container::OpFloatArr> llbd_; // (��ż����)�²�-�������Է������lambda
	std::map<OpLInt, Container::OpFloatArr> lrho_; // (��ż����)�²�-������η������rho

	friend class Algorithm::OpHierarchicalModelI;
	friend class Algorithm::OpAlgoATC;
protected:
	void init(); // ��ʼ��(�������캯��ʹ��)
	void clear(); // ���ģ��(��������������ʹ��)
	void resetObj(); // ����Ŀ�꺯���ĵ�ǰֵ(����Ϊԭʼֵ)
	void initTarget(OpLInt idx); // ��ʼ����idx�����Ŀ��
	void initSolution();// ��ʼ��������
	void initLambda(OpLInt idx); // ��ʼ����idx�����Է������lambda
	void initRho(OpLInt idx); // ��ʼ����idx�Ķ��η������rho
	void addResponse(Variable::OpVar var); // ��ӷ���ָ��
	void addResponse(Variable::OpVarArr vars); // ��ӷ���ָ��
	void addTarget(OpLInt idx, Variable::OpVar var); // ��Ӷ�idx�����Ŀ��
	void addTarget(OpLInt idx, Variable::OpVarArr vars); // ��Ӷ�idx�����Ŀ��
	void addVar(Variable::OpVar var); // ��ӱ���
	void addVar(Variable::OpVarArr vars); // ��ӱ���
	void addCon(Constraint::OpLinCon con); // ���Լ��
	void addCon(Constraint::OpLCArr cons); // ���Լ��
	void addCon(Constraint::OpQuadCon con); // ���Լ��
	void addCon(Constraint::OpQCArr cons); // ���Լ��
	void setObj(const Expression::OpQuadExpr& expr); // ����ԭʼĿ�꺯��
	void updateObjVal(OpFloat val); // ����ԭʼĿ�꺯����
	void setValue(Variable::OpVar var, OpFloat val);	// ���ñ�����
	void setValue(OpULInt order, OpFloat val);	// ���ñ�����
	void setUpperLambda(Variable::OpVar var, OpFloat val); // �����ϲ�-�������var��Ӧ�����Է������lambda
	void setUpperLambda(OpULInt order, OpFloat val); // �����ϲ�-�����order�����Է������lambda
	void setUpperRho(Variable::OpVar var, OpFloat val); // �����ϲ�-�������var��Ӧ�Ķ��η������rho
	void setUpperRho(OpULInt order, OpFloat val); // �����ϲ�-�����order�����η������rho
	void setLowerLambda(OpLInt idx, Variable::OpVar var, OpFloat val); // �����²�idx-�������var��Ӧ�����Է������lambda
	void setLowerLambda(OpLInt idx, OpULInt order, OpFloat val); // �����²�idx-�����order�����Է������lambda
	void setLowerRho(OpLInt idx, Variable::OpVar var, OpFloat val); // �����²�idx-�������var��Ӧ�Ķ��η������rho
	void setLowerRho(OpLInt idx, OpULInt order, OpFloat val); // �����²�idx-�����order�����η������rho
	Variable::OpVar getVar(OpULInt order) const; // ��ȡ��order������
	Variable::OpVarArr getVar() const; // ��ȡ����
	Constraint::OpLCArr getLinCon() const; // ��ȡ����Լ��
	Constraint::OpQCArr getQuadCon() const; // ��ȡ����Լ��
	OpFloat getValue(Variable::OpVar var) const; // ��ȡ����var��Ӧ�ı�����
	OpFloat getValue(OpULInt order) const; // ��ȡ��order��������
	Container::OpFloatArr getValue() const; // ��ȡ������
	Variable::OpVarArr getResponse() const; // ��ȡ�ϲ�-����ķ���ָ��
	Variable::OpVarArr getTarget(OpLInt idx) const; // ��ȡ�²�idx-��������Ŀ��
	OpFloat getUpperLambda(Variable::OpVar var) const; // ��ȡ�ϲ�-�������var��Ӧ�����Է������lambda
	OpFloat getUpperLambda(OpULInt order) const; // ��ȡ�ϲ�-�����order�����Է������lambda
	Container::OpFloatArr getUpperLambda() const;  // ��ȡ�ϲ�-�������Է������lambda
	OpFloat getUpperRho(Variable::OpVar var) const; // ��ȡ�ϲ�-�������var��Ӧ�Ķ��η������rho
	OpFloat getUpperRho(OpULInt order) const; // ��ȡ�ϲ�-�����order�����η������rho
	Container::OpFloatArr getUpperRho() const;  // ��ȡ�ϲ�-������η������rho
	OpFloat getLowerLambda(OpLInt idx, Variable::OpVar var) const; // ��ȡ�²�idx-�������var��Ӧ�����Է������lambda
	OpFloat getLowerLambda(OpLInt idx, OpULInt order) const; // ��ȡ�²�idx-�����order�����Է������lambda
	Container::OpFloatArr getLowerLambda(OpLInt idx) const;  // ��ȡ�²�idx-�������Է������lambda
	OpFloat getLowerRho(OpLInt idx, Variable::OpVar var) const; // ��ȡ�²�idx-�������var��Ӧ�Ķ��η������rho
	OpFloat getLowerRho(OpLInt idx, OpULInt order) const; // ��ȡ�²�idx-�����order�����η������rho
	Container::OpFloatArr getLowerRho(OpLInt idx) const; // ��ȡ�²�idx-������η������rho
	OpFloat getObjValue() const; // ��ȡԭʼĿ�꺯����
	const Expression::OpQuadExpr& getRawObj() const; // ��ȡԭʼĿ�꺯�����ʽ
	const Expression::OpQuadExpr& getActualObj() const; // ��ȡʵ��Ŀ�꺯��(������)���ʽ
	Expression::OpQuadExpr& getRawObj(); // ��ȡԭʼĿ�꺯�����ʽ
	Expression::OpQuadExpr& getActualObj(); // ��ȡʵ��Ŀ�꺯��(������)���ʽ
	void release(); // �ֶ��ͷ�
	void preRelease(); // �ͷ�ǰ׼��
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
	// TODO����Ҫ�Ƚ�������������
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

// �ֲ�ģ�ͽڵ��ϵ��
class OpHMLink
{
protected:
	OpLInt upper_; // �ϲ�ģ������(С��0��ʾ�������ϲ�ģ��)
	std::set<OpLInt> lower_; // �²�ģ������

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

// �ֲ�ģ��
class Algorithm::OpHierarchicalModelI
	: public OpImplBase
{
protected:
	// ģ������
	OpStr mname_;
	// ����ģ��(ģ������->ģ��)
	std::map<OpLInt, OpHMNode*> nodes_;
	// �㼶��ϵ��(ģ������->ģ�Ͳ㼶��ϵ)
	std::map<OpLInt, OpHMLink*> links_;

	friend class Algorithm::OpHierarchicalModel;
	friend class Algorithm::OpAlgoATC;
protected:
	// ��ʼ��(�������캯��ʹ��)
	void init();
	// ���ģ��(��������������ʹ��)
	void clear();
	// ��ʼ����(��ģ���γ���Ϻ����ñ����⡢��ż������֮ǰִ��)
	void initSolution();
	// ��ʼ��ģ��(���ɸ��ڵ�)������������
	OpLInt initRoot(OpLInt idx);
	// ��һ��ģ��idx1���������������¼�ģ��idx2(Ĭ���Զ�����)������������
	OpLInt addLower(OpLInt idx1, OpLInt idx2);
	// ���idx1��idx2֮���һ�Թ�������var1��var2
	void link(OpLInt idx1, OpLInt idx2, Variable::OpVar var1, Variable::OpVar var2);
	// ���idx1��idx2֮���һ���������var1��var2
	void link(OpLInt idx1, OpLInt idx2, Variable::OpVarArr vars1, Variable::OpVarArr vars2);
	// ���idx�ľֲ����߱���
	void add(OpLInt idx, Variable::OpVar var);
	// ���idx�ľֲ����߱���
	void add(OpLInt idx, Variable::OpVarArr vars);
	// ���idx�ľֲ�����Լ��
	void add(OpLInt idx, Constraint::OpLinCon con);
	// ���idx�ľֲ�����Լ����
	void add(OpLInt idx, Constraint::OpLCArr cons);
	// ���idx�ľֲ�����Լ��
	void add(OpLInt idx, Constraint::OpQuadCon con);
	// ���idx�ľֲ�����Լ����
	void add(OpLInt idx, Constraint::OpQCArr cons);
	// ����idx�ľֲ�Ŀ�꺯��
	void setObj(OpLInt idx, const Expression::OpQuadExpr& expr);
	// ����idx�б���var�Ľ�
	void setValue(OpLInt idx, Variable::OpVar var, OpFloat val);	
	// ��ȡidx�б���var��Ӧ�ı�����
	OpFloat getValue(OpLInt idx, Variable::OpVar var) const;
	// ��ȡԭʼĿ�꺯����(idxȡ��ֵ�򷵻������)
	OpFloat getObjValue(OpLInt idx) const;
	// idx�Ƿ�Ϊ���ڵ�ĺϷ�����
	OpBool isRoot(OpLInt idx) const;
	// idx�Ƿ�ΪҶ�ڵ�ĺϷ�����
	OpBool isLeaf(OpLInt idx) const;
	// idx�Ƿ�Ϊ�ڵ�ĺϷ�����
	OpBool isNode(OpLInt idx) const;
	// ����ģ��
	void write(OpStr root);
	// �ͷ�ǰ׼��
	void preRelease();
	// ��ȡ�ڴ�ռ��
	virtual OpULInt getMemoryUsage() const;
protected:
	// ���캯��
	OpHierarchicalModelI(OpEnvI* env);
	OpHierarchicalModelI(OpEnvI* env, OpStr name);
public:
	// ��������
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
		idx2 = nodes_.rbegin()->first + 1; /*Ĭ�Ϸ��䵱ǰ�������+1*/
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
	// TODO������ģ��
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
	// ������д��release()��������ΪOpRobustModelI�а�����OPUA���
	// ������ִ��preRelease()������Щ��������ü�������֤OpHierarchicalModelI������������ʱ���������ͷ���Щ���
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
	OpBool shake; //��ǰ�Ƿ������
	OpBool shakeQuit; // �����𵴺��Ƿ��˳�
	Solver::OpSolType solverMode; // �����ģʽ
	OpLInt initMode; // ���ӳ�ʼ��ģʽѡ��
	OpLInt iter; //��ǰ��������
	OpLInt iterMax; // ����������
	OpLInt shakeCount; //�𵴼�����
	OpLInt shakeLimit; // ����𵴴���
	OpFloat NCC; //��Ҫ�������о�
	OpFloat SCC; //��Ҫ�������о�
	OpFloat objVal0, objVal1; //��һ��/��ǰ��Ŀ�꺯��ֵ
	OpFloat cMax0, cMax1; //��һ��/��ǰ�Ĺ�������(ƽ��)������ֵ
	OpFloat atcGap1; // ATC��Ҫ����������
	OpFloat atcGap2;; // ATC�������������
	OpFloat atcShakeGap; // ATC���о�
	OpFloat rhoUpdateFactor; // ���η���ϵ�����³���
	OpFloat lambdaInitValue; // һ�η������ͳһ��ʼ����ֵ
	OpFloat rhoInitValue; // ���η������ͳһ��ʼ����ֵ

	std::set<OpLInt> nodeIdxs; //��������ڵ�����
	std::map<OpLInt, std::vector<OpFloat> > cost; //��ǰ�������нڵ����������C(������ʾ��ǰ�ڵ㣬ӳ��ֵΪ�ϼ�Ŀ��/������϶)
	Objective::OpObj objtmp; // ��ʱOPUAĿ�꺯������(���ڸ���model��Ŀ�꺯��ʱ�û���)
	std::map<OpLInt, Objective::OpObj> objs; // ���ڵ�OPUAģ�Ͷ���
	std::map<OpLInt, Model::OpModel> models; // ���ڵ�OPUAģ�Ͷ���

	void init(const OpHierarchicalModelI* mdl, const Solver::OpConfig& config); // ��ʼ��
	void clear(); // ����
	void printIterInfo(std::ostream& stream, const std::thread::id& tid, OpFloat timecost); // ��ӡ������Ϣ
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
	// �ͷ�OPUA���
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

	// ����ɹ��ı�־
	OpBool successful = false;
	// ��������
	OpEnv localEnv(mdl_->env_);
	std::thread::id tid = std::this_thread::get_id();
	std::cout << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << ")MemoryUsage(start): " << localEnv.getMemoryUsage() << std::endl;
	// ����������Ϣ
	OpATCIterInfo info;
	info.init(mdl_, config);
	// �ػ�����
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

	// lambda����ʼ��
	auto init = [&]() {
		// ��ʼ������
		switch (info.initMode)
		{
		case 0: /*��ͳһֵ��ʼ��*/
			for (const auto& idx : info.nodeIdxs)
			{
				auto node = mdl_->nodes_.at(idx);
				const auto& upperIdx = mdl_->links_.at(idx)->upper_;
				const auto& lowerIdxs = mdl_->links_.at(idx)->lower_;
				node->initSolution();
				if (upperIdx >= 0)
				{
					// ����һ�γ��ӳ�ֵ->�ϼ�Ŀ��/����
					auto lbd = node->getUpperLambda();
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						lbd[i] = info.lambdaInitValue;
					// ���ö��γ��ӳ�ֵ->�ϼ�Ŀ�� / ����
					auto rho = node->getUpperRho();
					for (OpULInt i = 0; i < rho.getSize(); i++)
						rho[i] = info.rhoInitValue;
				}
				for (const auto& lowerIdx : lowerIdxs)
				{
					// ����һ�γ��ӳ�ֵ->�¼�Ŀ��/����
					auto lbd = node->getLowerLambda(lowerIdx);
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						lbd[i] = info.lambdaInitValue;
					// ���ö��γ��ӳ�ֵ->�¼�Ŀ��/����
					auto rho = node->getLowerRho(lowerIdx);
					for (OpULInt i = 0; i < rho.getSize(); i++)
						rho[i] = info.rhoInitValue;
				}
			}
			break;
		case 1: /*���г�ʼ������������*/
			break;
		default:
			break;
		}
		// ���Լ��
		for (const auto& idx : info.nodeIdxs)
		{
			auto node = mdl_->nodes_.at(idx);
			auto& curModel = info.models.at(idx);
			curModel.add(node->getLinCon());
			curModel.add(node->getQuadCon());
		}
	};
	// lambda�����¸�������
	auto updateModel = [&]() {
		if (info.iter == 1) /*��ʼ����*/
		{
			// ��һ�μ��㣬����ӷ���
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
		else /*�ǳ�ʼ����*/
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
				// ����һ�η���&���η���->�ϼ�Ŀ��/����
				if (upperIdx >= 0)
				{
					auto upperNode = mdl_->nodes_.at(upperIdx);
					auto upperTgt = upperNode->getTarget(idx);
					auto curResp = node->getResponse();
					auto lbd = node->getUpperLambda();
					auto rho = node->getUpperRho();
					std::vector<OpLinExpr> deviation(lbd.getSize(), 0.0);
					// ����c
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						//deviation[i] += upperNode->getValue(upperTgt[i]) - curResp[i]; /*ԭ����·�ʽ������Ч�ʵ�*/
						deviation[i] += (upperNode->getValue(upperTgt[i]) + node->getValue(curResp[i])) / 2 - curResp[i];
					// ����һ�η���
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						curObjExpr += lbd[i] * deviation[i];
					// ������η���
					for (OpULInt i = 0; i < rho.getSize(); i++)
						curObjExpr += rho[i] * rho[i] * deviation[i] * deviation[i];
				}
				// ����һ�η���&���η���->�¼�Ŀ��/����
				for (const auto& lowerIdx : lowerIdxs)
				{
					auto lowerNode = mdl_->nodes_.at(lowerIdx);
					auto lowerResp = lowerNode->getResponse();
					auto curTgt = node->getTarget(lowerIdx);
					auto lbd = node->getLowerLambda(lowerIdx);
					auto rho = node->getLowerRho(lowerIdx);
					std::vector<OpLinExpr> deviation(lbd.getSize(), 0.0);
					// ����c
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						//deviation[i] += curTgt[i] - lowerNode->getValue(lowerResp[i]);
						deviation[i] += curTgt[i] - (node->getValue(curTgt[i]) + lowerNode->getValue(lowerResp[i])) / 2;
					// ����һ�η���
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						curObjExpr += lbd[i] * deviation[i];
					// ������η���
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
	// lambda������������
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
				// ����Ŀ�꺯����
				node->updateObjVal(curSolver.getValue(node->getRawObj()));
				// ���±�����
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
	// lambda�����¹���������϶
	auto updateNodeC = [&]() {
		const auto& idxs = info.nodeIdxs;
		for (const auto& idx : idxs) /*�Ӹ��ڵ㿪ʼ��ֻ�����ϼ�Ŀ��/������C*/
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
	// lambda�����¶�ż
	auto updateDual = [&]() {
		const auto& idxs = info.nodeIdxs;
		for (const auto& idx : idxs)
		{
			auto node = mdl_->nodes_.at(idx);
			const auto& upperIdx = mdl_->links_.at(idx)->upper_;
			const auto& lowerIdxs = mdl_->links_.at(idx)->lower_;
			// ����һ��/���γ���->�ϼ�Ŀ��/����
			if (upperIdx >= 0)
			{
				auto& deviation = info.cost.at(idx);
				auto lbd = node->getUpperLambda();
				auto rho = node->getUpperRho();
				// ����һ�γ���
				for (OpULInt i = 0; i < lbd.getSize(); i++)
					lbd[i] += 2 * rho[i] * rho[i] * deviation[i];
				// ���¶��γ���
				for (OpULInt i = 0; i < rho.getSize(); i++)
					rho[i] *= info.rhoUpdateFactor;
			}
			// ����һ��/���γ���->�¼�Ŀ��/����
			for (const auto& lowerIdx : lowerIdxs)
			{
				auto& deviation = info.cost.at(lowerIdx);
				auto lbd = node->getLowerLambda(lowerIdx);
				auto rho = node->getLowerRho(lowerIdx);
				// ����һ�γ���
				for (OpULInt i = 0; i < lbd.getSize(); i++)
					lbd[i] += 2 * rho[i] * rho[i] * deviation[i];
				// ���¶��γ���
				for (OpULInt i = 0; i < rho.getSize(); i++)
					rho[i] *= info.rhoUpdateFactor;
			}
		}
	};
	// lambda�����������о�
	auto updateLog = [&]() {
		const auto& idxs = info.nodeIdxs;
		// ���±�Ҫ���о�
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
		// ���³�����о�
		info.objVal0 = info.objVal1;
		info.objVal1 = 0;
		for (const auto& idx : idxs)
			info.objVal1 += mdl_->nodes_.at(idx)->getObjValue();
		info.SCC = std::abs(info.objVal1 - info.objVal0) / std::abs(info.objVal1);
		// �������о�
		if (std::abs(info.cMax1 - info.cMax0) < info.atcShakeGap)
			info.shakeCount++;
		else
			info.shakeCount = 0;
		if (info.shakeCount >= info.shakeLimit)
			info.shake = true;
		else
			info.shake = false;
	};
	// lambda���ж��˳�����
	auto quit = [&]() {
		return (info.iter > info.iterMax) ||
			(info.NCC < info.atcGap1&& info.SCC < info.atcGap2) ||
			(info.shakeQuit && info.shake);
	};

	try
	{	
		// ��ʱ��
		OpTimer globalWatch(true);
		OpTimer localWatch(true);
		std::cout << "[OPUA::Algorithm::OpAlgoATC::solvee](Thread: " << tid << "): Start" << std::endl;
		std::cout << "[OPUA::Algorithm::OpAlgoATC::solve](Thread: " << tid << "): init" << std::endl;
		// ��ʼ��
		init();
		// ����
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
	// ����������������(����Ĭ��ֵ)
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
