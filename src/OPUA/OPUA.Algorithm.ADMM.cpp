#include "OPUA.Common.h"
#include "OPUA.Algorithm.h"
#include "OPUA.Exception.h"
#include <thread>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>

using namespace OPUA;

/* OpNHMNode */

// �Ƿֲ�ģ�ͽڵ�
class OpNHMNode
{
protected:
	OpLInt nidx_; // �ڵ�����
	OpEnv nenv_; // �ڵ�Ļ�������
	Expression::OpQuadExpr robj_; // �������Ż�Ŀ�꺯��ԭʼֵ
	Expression::OpQuadExpr aobj_; // �������Ż�Ŀ�꺯����ǰֵ
	Variable::OpVarArr var_; // ��������߱���
	Variable::OpVarArr ivar_; // �������������߱���(����var_)
	Constraint::OpLCArr con_; // ����������Լ������
	Constraint::OpQCArr qcon_; // ���������Լ������
	std::map<OpLInt, Variable::OpVarArr> shared_; // ������������������Ĺ�ͬ����
protected:
	OpFloat aobjsol_; // ������Ŀ�꺯����(������)
	OpFloat robjsol_; // ������Ŀ�꺯����(ԭʼ)
	Container::OpFloatArr varsol_; // ��������߱�����
	std::map<OpLInt, Container::OpFloatArr> slbd_; // (��ż����)��������������Է������lambda
	std::map<OpLInt, Container::OpFloatArr> srho_; // (��ż����)����������Ķ��η������rho

	friend class Algorithm::OpNonHierarchicalModelI;
	friend class Algorithm::OpAlgoADMM;
protected:
	void init(); // ��ʼ��(�������캯��ʹ��)
	void clear(); // ���ģ��(��������������ʹ��)
	void resetObj(); // ����Ŀ�꺯���ĵ�ǰֵ(����Ϊԭʼֵ)
	void initShared(OpLInt idx); // ��ʼ����������idx�Ĺ�ͬ����
	void initSolution();// ��ʼ��������
	void initLambda(OpLInt idx); // ��ʼ����������idx�����Է������lambda
	void initRho(OpLInt idx); // ��ʼ����������idx�Ķ��η������rho
	void addShared(OpLInt idx, Variable::OpVar var); // ��Ӷ�������idx�Ĺ�ͬ����
	void addShared(OpLInt idx, Variable::OpVarArr vars); // ��Ӷ�������idx�Ĺ�ͬ����
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
	void setLambda(OpLInt idx, Variable::OpVar var, OpFloat val); // ����������idx-����var��Ӧ�����Է������lambda
	void setLambda(OpLInt idx, OpULInt order, OpFloat val); // ����������idx-��order�����Է������lambda
	void setRho(OpLInt idx, Variable::OpVar var, OpFloat val); // ����������idx-����var��Ӧ�Ķ��η������rho
	void setRho(OpLInt idx, OpULInt order, OpFloat val); // ����������idx-��order�����η������rho
	Variable::OpVar getVar(OpULInt order) const; // ��ȡ��order������
	Variable::OpVarArr getVar() const; // ��ȡ����
	Variable::OpVarArr getIntVar() const; // ��ȡ��������
	Constraint::OpLCArr getLinCon() const; // ��ȡ����Լ��
	Constraint::OpQCArr getQuadCon() const; // ��ȡ����Լ��
	OpFloat getValue(Variable::OpVar var) const; // ��ȡ����var��Ӧ�ı�����
	OpFloat getValue(OpULInt order) const; // ��ȡ��order��������
	Container::OpFloatArr getValue() const; // ��ȡ������
	Variable::OpVarArr getShared(OpLInt idx) const; // ��ȡ������idx�Ĺ�ͬ����
	OpFloat getLambda(OpLInt idx, Variable::OpVar var) const; // ��ȡ������idx-����var��Ӧ�����Է������lambda
	OpFloat getLambda(OpLInt idx, OpULInt order) const; // ��ȡ������idx-��order�����Է������lambda
	Container::OpFloatArr getLambda(OpLInt idx) const;  // ��ȡ������idx-���Է������lambda
	OpFloat getRho(OpLInt idx, Variable::OpVar var) const; // ��ȡ������idx-����var��Ӧ�Ķ��η������rho
	OpFloat getRho(OpLInt idx, OpULInt order) const; // ��ȡ������idx-��order�����η������rho
	Container::OpFloatArr getRho(OpLInt idx) const; // ��ȡ������idx-���η������rho
	OpFloat getObjValue() const; // ��ȡԭʼĿ�꺯����
	const Expression::OpQuadExpr& getRawObj() const; // ��ȡԭʼĿ�꺯�����ʽ
	const Expression::OpQuadExpr& getActualObj() const; // ��ȡʵ��Ŀ�꺯��(������)���ʽ
	Expression::OpQuadExpr& getRawObj(); // ��ȡԭʼĿ�꺯�����ʽ
	Expression::OpQuadExpr& getActualObj(); // ��ȡʵ��Ŀ�꺯��(������)���ʽ
	OpBool isMIP() const; // �Ƿ�Ϊ��������
	void release(); // �ֶ��ͷ�
	void preRelease(); // �ͷ�ǰ׼��
protected:
	OpNHMNode() = delete;
	OpNHMNode(OpLInt idx, OpEnv env);
public:
	~OpNHMNode();
};

void OpNHMNode::init()
{
	OpEnv localEnv(nenv_);
	var_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(var_.getImpl());
	ivar_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(ivar_.getImpl());
	con_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(con_.getImpl());
	qcon_ = Constraint::OpQCArr(localEnv);
	localEnv.addManagement(qcon_.getImpl());
	varsol_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(varsol_.getImpl());
}

void OpNHMNode::clear()
{
	for (auto& c : srho_)
		c.second.release();
	for (auto& c : slbd_)
		c.second.release();
	varsol_.release();
	for (auto& c : shared_)
		c.second.release();
	qcon_.release();
	con_.release();
	ivar_.release();
	var_.release();
}

void OpNHMNode::resetObj()
{
	aobj_ = robj_;
}

void OpNHMNode::initShared(OpLInt idx)
{
	auto iter = shared_.find(idx);
	if (iter == shared_.end())
	{
		iter = shared_.emplace(idx, Variable::OpVarArr(nenv_)).first;
		nenv_.addManagement(iter->second.getImpl());
	}
}

void OpNHMNode::initSolution()
{
	// TODO����Ҫ�Ƚ�������������
	varsol_.setSize(var_.getSize());
	for (const auto& s : shared_)
	{
		initLambda(s.first);
		initRho(s.first);
		slbd_.at(s.first).setSize(s.second.getSize());
		srho_.at(s.first).setSize(s.second.getSize());
	}
}

void OpNHMNode::initLambda(OpLInt idx)
{
	auto iter = slbd_.find(idx);
	if (iter == slbd_.end())
	{
		iter = slbd_.emplace(idx, Container::OpFloatArr(nenv_)).first;
		nenv_.addManagement(iter->second.getImpl());
	}
}

void OpNHMNode::initRho(OpLInt idx)
{
	auto iter = srho_.find(idx);
	if (iter == srho_.end())
	{
		iter = srho_.emplace(idx, Container::OpFloatArr(nenv_)).first;
		nenv_.addManagement(iter->second.getImpl());
	}
}

void OpNHMNode::addShared(OpLInt idx, Variable::OpVar var)
{
	shared_.at(idx).add(var);
}

void OpNHMNode::addShared(OpLInt idx, Variable::OpVarArr vars)
{
	shared_.at(idx).add(vars);
}

void OpNHMNode::addVar(Variable::OpVar var)
{
	auto vtype(var.getType());
	var_.add(var);
	if (vtype == Variable::OpVarType::Bool || vtype == Variable::OpVarType::Int)
		ivar_.add(var);
}

void OpNHMNode::addVar(Variable::OpVarArr vars)
{
	for (OpULInt i = 0; i < vars.getSize(); i++)
		addVar(vars[i]);
}

void OpNHMNode::addCon(Constraint::OpLinCon con)
{
	con_.add(con);
}

void OpNHMNode::addCon(Constraint::OpLCArr cons)
{
	con_.add(cons);
}

void OpNHMNode::addCon(Constraint::OpQuadCon con)
{
	qcon_.add(con);
}

void OpNHMNode::addCon(Constraint::OpQCArr cons)
{
	qcon_.add(cons);
}

void OpNHMNode::setObj(const Expression::OpQuadExpr& expr)
{
	robj_ = expr;
}

void OpNHMNode::updateObjVal(OpFloat val)
{
	robjsol_ = val;
}

void OpNHMNode::setValue(Variable::OpVar var, OpFloat val)
{
	auto order = var_.find(var);
	if (var_.idxValid(order))
		varsol_[order] = val;
}

void OpNHMNode::setValue(OpULInt order, OpFloat val)
{
	varsol_[order] = val;
}

void OpNHMNode::setLambda(OpLInt idx, Variable::OpVar var, OpFloat val)
{
	const auto& sub = shared_.at(idx);
	auto order = sub.find(var);
	if (sub.idxValid(order))
		slbd_.at(idx)[order] = val;
}

void OpNHMNode::setLambda(OpLInt idx, OpULInt order, OpFloat val)
{
	slbd_.at(idx)[order] = val;
}

void OpNHMNode::setRho(OpLInt idx, Variable::OpVar var, OpFloat val)
{
	const auto& sub = shared_.at(idx);
	auto order = sub.find(var);
	if (sub.idxValid(order))
		srho_.at(idx)[order] = val;
}

void OpNHMNode::setRho(OpLInt idx, OpULInt order, OpFloat val)
{
	srho_.at(idx)[order] = val;
}

Variable::OpVar OpNHMNode::getVar(OpULInt order) const
{
	return var_[order];
}

Variable::OpVarArr OpNHMNode::getVar() const
{
	return var_;
}

Variable::OpVarArr OpNHMNode::getIntVar() const
{
	return ivar_;
}

Constraint::OpLCArr OpNHMNode::getLinCon() const
{
	return con_;
}

Constraint::OpQCArr OpNHMNode::getQuadCon() const
{
	return qcon_;
}

OpFloat OpNHMNode::getValue(Variable::OpVar var) const
{
	OpFloat result(Constant::NaN);
	auto order = var_.find(var);
	if (var_.idxValid(order))
		result = varsol_[order];
	return result;
}

OpFloat OpNHMNode::getValue(OpULInt order) const
{
	return varsol_[order];
}

Container::OpFloatArr OpNHMNode::getValue() const
{
	return varsol_;
}

Variable::OpVarArr OpNHMNode::getShared(OpLInt idx) const
{
	return shared_.at(idx);
}

OpFloat OpNHMNode::getLambda(OpLInt idx, Variable::OpVar var) const
{
	OpFloat result(Constant::NaN);
	const auto& sub = shared_.at(idx);
	auto order = sub.find(var);
	if (sub.idxValid(order))
		result = slbd_.at(idx)[order];
	return result;
}

OpFloat OpNHMNode::getLambda(OpLInt idx, OpULInt order) const
{
	return slbd_.at(idx)[order];
}

Container::OpFloatArr OpNHMNode::getLambda(OpLInt idx) const
{
	return slbd_.at(idx);
}

OpFloat OpNHMNode::getRho(OpLInt idx, Variable::OpVar var) const
{
	OpFloat result(Constant::NaN);
	const auto& sub = shared_.at(idx);
	auto order = sub.find(var);
	if (sub.idxValid(order))
		result = srho_.at(idx)[order];
	return result;
}

OpFloat OpNHMNode::getRho(OpLInt idx, OpULInt order) const
{
	return srho_.at(idx)[order];
}

Container::OpFloatArr OpNHMNode::getRho(OpLInt idx) const
{
	return srho_.at(idx);
}

OpFloat OpNHMNode::getObjValue() const
{
	return robjsol_;
}

const Expression::OpQuadExpr& OpNHMNode::getRawObj() const
{
	return robj_;
}

const Expression::OpQuadExpr& OpNHMNode::getActualObj() const
{
	return aobj_;
}

Expression::OpQuadExpr& OpNHMNode::getRawObj()
{
	return robj_;
}

Expression::OpQuadExpr& OpNHMNode::getActualObj()
{
	return aobj_;
}

OpBool OpNHMNode::isMIP() const
{
	return ivar_.getSize() > 0;
}

void OpNHMNode::release()
{
	delete this;
}

void OpNHMNode::preRelease()
{
	OpEnv localEnv(nenv_);
	for (auto& c : srho_)
		localEnv.removeManagement(c.second.getImpl());
	for (auto& c : slbd_)
		localEnv.removeManagement(c.second.getImpl());
	localEnv.removeManagement(varsol_.getImpl());
	for (auto& c : shared_)
		localEnv.removeManagement(c.second.getImpl());
	localEnv.removeManagement(qcon_.getImpl());
	localEnv.removeManagement(con_.getImpl());
	localEnv.removeManagement(ivar_.getImpl());
	localEnv.removeManagement(var_.getImpl());
}

OpNHMNode::OpNHMNode(OpLInt idx, OpEnv env)
	: nidx_(idx),
	nenv_(env),
	robj_(0.0),
	aobj_(0.0),
	aobjsol_(Constant::NaN),
	robjsol_(Constant::NaN)
{
	init();
}

OpNHMNode::~OpNHMNode()
{
	clear();
}

// �Ƿֲ�ģ�ͽڵ��ϵ��
class OpNHMLink
{
protected:
	std::set<OpLInt> adj_; // ���ڽڵ�����

	friend class Algorithm::OpAlgoADMM;
	friend class Algorithm::OpNonHierarchicalModelI;
protected:
	OpNHMLink();
public:
	~OpNHMLink();
};

OpNHMLink::OpNHMLink()
{

}

OpNHMLink::~OpNHMLink()
{

}

/* OPUA:;Algorithm::OpNonHierarchicalModelI */

// �Ƿֲ�ģ��
class Algorithm::OpNonHierarchicalModelI
	: public OpImplBase
{
protected:
	// ģ������
	OpStr mname_;
	// ����ģ��(ģ������->ģ��)
	std::map<OpLInt, OpNHMNode*> nodes_;
	// �ڽӹ�ϵ��(ģ������->ģ���ڽӹ�ϵ)
	std::map<OpLInt, OpNHMLink*> links_;

	friend class Algorithm::OpNonHierarchicalModel;
	friend class Algorithm::OpAlgoADMM;
protected:
	// ��ʼ��(�������캯��ʹ��)
	void init();
	// ���ģ��(��������������ʹ��)
	void clear();
	// ��ʼ����(��ģ���γ���Ϻ����ñ����⡢��ż������֮ǰִ��)
	void initSolution();
	// ����������ģ��idx(Ĭ���Զ�����)������������
	OpLInt addSub(OpLInt idx);
	// ��ʼ��idx1��idx2֮��Ĺ���
	void link(OpLInt idx1, OpLInt idx2);
	// ���idx1��idx2֮���һ�Թ�������var1��var2
	void link(OpLInt idx1, OpLInt idx2, Variable::OpVar var1, Variable::OpVar var2, OpBool init);
	// ���idx1��idx2֮���һ���������var1��var2
	void link(OpLInt idx1, OpLInt idx2, Variable::OpVarArr vars1, Variable::OpVarArr vars2, OpBool init);
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
	OpNonHierarchicalModelI(OpEnvI* env);
	OpNonHierarchicalModelI(OpEnvI* env, OpStr name);
public:
	// ��������
	virtual ~OpNonHierarchicalModelI();
};

void Algorithm::OpNonHierarchicalModelI::init()
{

}

void Algorithm::OpNonHierarchicalModelI::clear()
{
	for (auto& node : nodes_)
		node.second->release(), node.second = nullptr;
}

void Algorithm::OpNonHierarchicalModelI::initSolution()
{
	for (auto& node : nodes_)
		node.second->initSolution();
}

OpLInt Algorithm::OpNonHierarchicalModelI::addSub(OpLInt idx)
{
	if (isNode(idx))
	{
		throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::addSub]: Exception->"
			+ std::to_string(idx)
			+ " exist");
	}
	if (idx < 0)
	{
		if (nodes_.empty())
			idx = 0; /*�׸�������Ĭ�Ϸ���0����*/
		else
			idx = nodes_.rbegin()->first + 1; /*���׸�������Ĭ�Ϸ��䵱ǰ�������+1*/
	}
	nodes_.emplace(idx, new OpNHMNode(idx, OpEnv(env_)));
	links_.emplace(idx, new OpNHMLink());
	return idx;
}

void Algorithm::OpNonHierarchicalModelI::link(OpLInt idx1, OpLInt idx2)
{
	auto iter1 = nodes_.find(idx1);
	auto iter2 = nodes_.find(idx2);
	if (iter1 == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx1)
			+ " non-exist");
	}
	if (iter2 == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::link]: Exception->"
			+ std::to_string(idx2)
			+ " non-exist");
	}
	auto& adj1 = links_.at(idx1)->adj_;
	auto& adj2 = links_.at(idx2)->adj_;
	if (adj1.find(idx2) == adj1.end())
	{
		iter1->second->initShared(idx2);
		iter2->second->initShared(idx1);
		adj1.emplace(idx2);
		adj2.emplace(idx1);
	}
}

void Algorithm::OpNonHierarchicalModelI::link(OpLInt idx1, OpLInt idx2, Variable::OpVar var1, Variable::OpVar var2, OpBool init)
{
	if (init)
		link(idx1, idx2);
	nodes_.at(idx1)->addShared(idx2, var1);
	nodes_.at(idx2)->addShared(idx1, var2);
}

void Algorithm::OpNonHierarchicalModelI::link(OpLInt idx1, OpLInt idx2, Variable::OpVarArr vars1, Variable::OpVarArr vars2, OpBool init)
{
	if (init)
		link(idx1, idx2);
	nodes_.at(idx1)->addShared(idx2, vars1);
	nodes_.at(idx2)->addShared(idx1, vars2);
}

void Algorithm::OpNonHierarchicalModelI::add(OpLInt idx, Variable::OpVar var)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::add]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->addVar(var);
}

void Algorithm::OpNonHierarchicalModelI::add(OpLInt idx, Variable::OpVarArr vars)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::add]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->addVar(vars);
}

void Algorithm::OpNonHierarchicalModelI::add(OpLInt idx, Constraint::OpLinCon con)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::add]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->addCon(con);
}

void Algorithm::OpNonHierarchicalModelI::add(OpLInt idx, Constraint::OpLCArr cons)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::add]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->addCon(cons);
}

void Algorithm::OpNonHierarchicalModelI::add(OpLInt idx, Constraint::OpQuadCon con)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::add]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->addCon(con);
}

void Algorithm::OpNonHierarchicalModelI::add(OpLInt idx, Constraint::OpQCArr cons)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::add]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->addCon(cons);
}

void Algorithm::OpNonHierarchicalModelI::setObj(OpLInt idx, const Expression::OpQuadExpr& expr)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::setObj]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->setObj(expr);
}

void Algorithm::OpNonHierarchicalModelI::setValue(OpLInt idx, Variable::OpVar var, OpFloat val)
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::setValue]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	iter->second->setValue(var, val);
}

OpFloat Algorithm::OpNonHierarchicalModelI::getValue(OpLInt idx, Variable::OpVar var) const
{
	auto iter = nodes_.find(idx);
	if (iter == nodes_.end())
	{
		throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::getValue]: Exception->"
			+ std::to_string(idx)
			+ " non-exist");
	}
	return iter->second->getValue(var);
}

OpFloat Algorithm::OpNonHierarchicalModelI::getObjValue(OpLInt idx) const
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
			throw OpExcBase("[Algorithm::OpNonHierarchicalModelI::getObjValue]: Exception->"
				+ std::to_string(idx)
				+ " non-exist");
		}
		return iter->second->getObjValue();
	}
}

OpBool Algorithm::OpNonHierarchicalModelI::isNode(OpLInt idx) const
{
	return links_.find(idx) != links_.end();
}

void Algorithm::OpNonHierarchicalModelI::write(OpStr root)
{
	// TODO������ģ��
}

void Algorithm::OpNonHierarchicalModelI::preRelease()
{
	for (auto& node : nodes_)
		node.second->preRelease();
}

OpULInt Algorithm::OpNonHierarchicalModelI::getMemoryUsage() const
{
	return sizeof(*this);
}

Algorithm::OpNonHierarchicalModelI::OpNonHierarchicalModelI(OpEnvI* env)
	: OpImplBase('M', env),
	mname_("NonHierarchicalModel_" + std::to_string(idx_))
{
	init();
}

Algorithm::OpNonHierarchicalModelI::OpNonHierarchicalModelI(OpEnvI* env, OpStr name)
	: OpImplBase('M', env),
	mname_(name)
{
	init();
}

Algorithm::OpNonHierarchicalModelI::~OpNonHierarchicalModelI()
{
	clear();
}

/* OPUA:;Algorithm::OpNonHierarchicalModel */

void Algorithm::OpNonHierarchicalModel::initSolution()
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->initSolution();
}

OpLInt Algorithm::OpNonHierarchicalModel::addSub(OpLInt idx)
{
	return static_cast<OpNonHierarchicalModelI*>(impl_)->addSub(idx);
}

void Algorithm::OpNonHierarchicalModel::link(OpLInt idx1, OpLInt idx2)
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->link(idx1, idx2);
}

void Algorithm::OpNonHierarchicalModel::link(OpLInt idx1, OpLInt idx2, Variable::OpVar var1, Variable::OpVar var2, OpBool init)
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->link(idx1, idx2, var1, var2, init);
}

void Algorithm::OpNonHierarchicalModel::link(OpLInt idx1, OpLInt idx2, Variable::OpVarArr vars1, Variable::OpVarArr vars2, OpBool init)
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->link(idx1, idx2, vars1, vars2, init);
}

void Algorithm::OpNonHierarchicalModel::add(OpLInt idx, Variable::OpVar var)
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->add(idx, var);
}

void Algorithm::OpNonHierarchicalModel::add(OpLInt idx, Variable::OpVarArr vars)
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->add(idx, vars);
}

void Algorithm::OpNonHierarchicalModel::add(OpLInt idx, Constraint::OpLinCon con)
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->add(idx, con);
}

void Algorithm::OpNonHierarchicalModel::add(OpLInt idx, Constraint::OpLCArr cons)
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->add(idx, cons);
}

void Algorithm::OpNonHierarchicalModel::add(OpLInt idx, Constraint::OpQuadCon con)
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->add(idx, con);
}

void Algorithm::OpNonHierarchicalModel::add(OpLInt idx, Constraint::OpQCArr cons)
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->add(idx, cons);
}

void Algorithm::OpNonHierarchicalModel::setObj(OpLInt idx, const Expression::OpQuadExpr& expr)
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->setObj(idx, expr);
}

void Algorithm::OpNonHierarchicalModel::setValue(OpLInt idx, Variable::OpVar var, OpFloat val)
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->setValue(idx, var, val);
}

OpFloat Algorithm::OpNonHierarchicalModel::getValue(OpLInt idx, Variable::OpVar var) const
{
	return static_cast<OpNonHierarchicalModelI*>(impl_)->getValue(idx, var);
}

OpFloat Algorithm::OpNonHierarchicalModel::getObjValue(OpLInt idx) const
{
	return static_cast<OpNonHierarchicalModelI*>(impl_)->getObjValue(idx);
}

OpBool Algorithm::OpNonHierarchicalModel::isNode(OpLInt idx) const
{
	return static_cast<OpNonHierarchicalModelI*>(impl_)->isNode(idx);
}

void Algorithm::OpNonHierarchicalModel::write(OpStr root)
{
	static_cast<OpNonHierarchicalModelI*>(impl_)->write(root);
}

Algorithm::OpNonHierarchicalModelI* Algorithm::OpNonHierarchicalModel::getImpl() const
{
	return static_cast<OpNonHierarchicalModelI*>(impl_);
}

void Algorithm::OpNonHierarchicalModel::release()
{
	// ������д��release()��������ΪOpNonHierarchicalModelI�а�����OPUA���
	// ������ִ��preRelease()������Щ��������ü�������֤OpNonHierarchicalModelI������������ʱ���������ͷ���Щ���
	if (impl_)
	{
		static_cast<OpNonHierarchicalModelI*>(impl_)->preRelease();
		static_cast<OpNonHierarchicalModelI*>(impl_)->release();
		impl_ = nullptr;
	}
}

OpBool Algorithm::OpNonHierarchicalModel::operator==(const OpNonHierarchicalModel& model) const
{
	return impl_ == model.impl_;
}

OpBool Algorithm::OpNonHierarchicalModel::operator!=(const OpNonHierarchicalModel& model) const
{
	return impl_ != model.impl_;
}

Algorithm::OpNonHierarchicalModel::OpNonHierarchicalModel()
{

}

Algorithm::OpNonHierarchicalModel::OpNonHierarchicalModel(OpNonHierarchicalModelI* impl)
{
	impl_ = impl;
}

Algorithm::OpNonHierarchicalModel::OpNonHierarchicalModel(OpEnv env)
{
	impl_ = new OpNonHierarchicalModelI(env.getImpl());
}

Algorithm::OpNonHierarchicalModel::OpNonHierarchicalModel(OpEnv env, OpStr name)
{
	impl_ = new OpNonHierarchicalModelI(env.getImpl(), name);
}

Algorithm::OpNonHierarchicalModel::~OpNonHierarchicalModel()
{

}

/* OPUA:;Algorithm::OpAlgoADMM */

struct Algorithm::OpAlgoADMM::OpADMMIterInfo
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
	OpFloat admmGap1; // ADMM��Ҫ����������
	OpFloat admmGap2;; // ADMM�������������
	OpFloat admmShakeGap; // ADMM���о�
	OpFloat lambdaInitValue; // һ�η������ͳһ��ʼ����ֵ
	OpFloat rhoInitValue; // ���η������ͳһ��ʼ����ֵ

	std::set<OpLInt> nodeIdxs; //��������ڵ�����
	std::map<std::pair<OpLInt, OpLInt>, std::vector<OpFloat> > cost; //��ǰ�������нڵ����������C(������ʾ��ǰ�ڵ㣬ӳ��ֵΪ����������϶)
	Objective::OpObj objtmp; // ��ʱOPUAĿ�꺯������(���ڸ���model��Ŀ�꺯��ʱ�û���)
	std::map<OpLInt, Objective::OpObj> objs; // ���ڵ�OPUAģ�Ͷ���
	std::map<OpLInt, Model::OpModel> models; // ���ڵ�OPUAģ�Ͷ���

	void init(const OpNonHierarchicalModelI* mdl, const Solver::OpConfig& config); // ��ʼ��
	void clear(); // ����
	void printIterInfo(std::ostream& stream, const std::thread::id& tid, OpFloat timecost); // ��ӡ������Ϣ
};

void Algorithm::OpAlgoADMM::OpADMMIterInfo::init(const OpNonHierarchicalModelI* mdl, const Solver::OpConfig& config)
{
	OpEnv localEnv(mdl->env_);
	shake = false;
	shakeQuit = config.getCfg<OpBool>("OPUA.Algorithm.ADMM.ADMMShakeAutoQuit");
	initMode = config.getCfg<OpLInt>("OPUA.Algorithm.ADMM.InitMode");
	iter = 1;
	iterMax = config.getCfg<OpLInt>("OPUA.Algorithm.ADMM.ADMMIterMax");
	shakeCount = 0;
	shakeLimit = config.getCfg<OpLInt>("OPUA.Algorithm.ADMM.ADMMShakeCount");
	NCC = 1;
	SCC = 1;
	objVal0 = Constant::Infinity;
	objVal1 = Constant::Infinity;
	cMax0 = Constant::Infinity;
	cMax1 = Constant::Infinity;
	admmGap1 = config.getCfg<OpFloat>("OPUA.Algorithm.ADMM.ADMMGap1");
	admmGap2 = config.getCfg<OpFloat>("OPUA.Algorithm.ADMM.ADMMGap2");
	admmShakeGap = config.getCfg<OpFloat>("OPUA.Algorithm.ADMM.ADMMShakeGap");
	lambdaInitValue = config.getCfg<OpFloat>("OPUA.Algorithm.ADMM.LambdaInitVal");
	rhoInitValue = config.getCfg<OpFloat>("OPUA.Algorithm.ADMM.RhoInitVal");
	solverMode = Solver::CharSolType2OpSolType(config.getCfg<OpChar>("OPUA.Algorithm.ADMM.MIPSolverMode"));
	for (const auto& node : mdl->nodes_)
	{
		nodeIdxs.emplace(node.first);
		for (const auto& snode : mdl->links_.at(node.first)->adj_)
			cost.emplace(std::make_pair(node.first, snode), std::vector<OpFloat>(mdl->nodes_.at(node.first)->getShared(snode).getSize(), Constant::Infinity));
		objs.emplace(node.first, Objective::OpObj(localEnv, Objective::OpObjSense::Min));
		models.emplace(node.first, Model::OpModel(localEnv, "ADMM_Model_" + std::to_string(node.first)));
	}	
	objtmp = Objective::OpMinimize(localEnv, Expression::OpLinExpr(0.0));
}

void Algorithm::OpAlgoADMM::OpADMMIterInfo::clear()
{
	// �ͷ�OPUA���
	for (auto& cp : models)
		cp.second.release();
	for (auto& cp : objs)
		cp.second.release();
	objtmp.release();
}

void Algorithm::OpAlgoADMM::OpADMMIterInfo::printIterInfo(std::ostream& stream, const std::thread::id& tid, OpFloat timecost)
{
	stream << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << "): ITER\tNCC\tSCC\tTime" << std::endl;
	stream << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << "): "
		<< iter << '\t'
		<< NCC << '\t'
		<< SCC << '\t'
		<< timecost << std::endl;
}

void Algorithm::OpAlgoADMM::extract(OpNonHierarchicalModel model)
{
	mdl_ = model.getImpl();
}

OpBool Algorithm::OpAlgoADMM::solve(const Solver::OpConfig& config)
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
	std::cout << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << ")MemoryUsage(start): " << localEnv.getMemoryUsage() << std::endl;
	// ����������Ϣ
	OpADMMIterInfo info;
	info.init(mdl_, config);
	// �ػ�����
	OpAutoFree guard([&]() {
		try
		{
			info.clear();
			std::cout << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << ")MemoryUsage(end): " << localEnv.getMemoryUsage() << std::endl;
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
				node->initSolution();
				for (const auto& sidx : mdl_->links_.at(idx)->adj_)
				{
					// ����һ�γ��ӳ�ֵ
					auto lbd = node->getLambda(sidx);
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						lbd[i] = info.lambdaInitValue;
					// ���ö��γ��ӳ�ֵ
					auto rho = node->getRho(sidx);
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
			for (const auto& idx : info.nodeIdxs)
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
			for (const auto& idx : info.nodeIdxs)
			{
				auto node = mdl_->nodes_.at(idx);
				auto& curModel = info.models.at(idx);
				auto& curObj = info.objs.at(idx);
				auto& curObjExpr = node->getActualObj();
				node->resetObj();
				// ����һ�η���&���η���
				for (const auto& sidx : mdl_->links_.at(idx)->adj_)
				{
					auto snode = mdl_->nodes_.at(sidx);
					auto shared1 = node->getShared(sidx);
					auto shared2 = snode->getShared(idx);
					auto lbd = node->getLambda(sidx);
					auto rho = node->getRho(sidx);
					std::vector<OpLinExpr> deviation(lbd.getSize(), 0.0);
					// ����c
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						deviation[i] += shared1[i] - (node->getValue(shared1[i]) + snode->getValue(shared2[i])) * 0.5;
					// ����һ�η���
					for (OpULInt i = 0; i < lbd.getSize(); i++)
						curObjExpr += lbd[i] * deviation[i];
					// ������η���
					for (OpULInt i = 0; i < rho.getSize(); i++)
						curObjExpr += 0.5 * rho[i] * deviation[i] * deviation[i];
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
		for (const auto& idx : info.nodeIdxs)
		{
			auto& curModel = info.models.at(idx);
			//curModel.write("");
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
				msg << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << ")Exception->solve "
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
		for (auto& deviation : info.cost)
		{
			auto idx1(deviation.first.first), idx2(deviation.first.second);
			auto node1(mdl_->nodes_.at(idx1)), node2(mdl_->nodes_.at(idx2));
			auto shared1(node1->getShared(idx2)), shared2(node2->getShared(idx1));
			for (OpULInt i = 0; i < shared1.getSize(); i++)
				deviation.second[i] = 0.5 * (node1->getValue(shared1[i]) - node2->getValue(shared2[i]));
		}
	};
	// lambda�����¶�ż
	auto updateDual = [&]() {
		for (auto& deviation : info.cost)
		{
			auto idx1(deviation.first.first), idx2(deviation.first.second);
			auto node(mdl_->nodes_.at(idx1));
			auto lbd(node->getLambda(idx2));
			auto rho(node->getRho(idx2));
			// ����һ�γ���(�ݲ����¶��γ���)
			for (OpULInt i = 0; i < lbd.getSize(); i++)
				lbd[i] += rho[i] * deviation.second[i];
		}
	};
	// lambda�������������̶�����
	auto solveSharedFixed = [&]() {
		// �̶���������
		if (info.iter != 1) /*�ǳ�ʼ����*/
		{
			for (const auto& idx : info.nodeIdxs)
			{
				auto node = mdl_->nodes_.at(idx);
				for (const auto& sidx : mdl_->links_.at(idx)->adj_)
				{
					auto shared = node->getShared(sidx);
					const auto& deviation = info.cost.at(std::make_pair(idx, sidx));
					for (OpULInt i = 0; i < shared.getSize(); i++)
					{
						shared[i].unlock();
						shared[i].setFixedValue(node->getValue(shared[i]) - deviation[i]);
						shared[i].lock();
					}
				}
			}
		}
		// ����������������
		solveModel();
		// ����̶���������
		if (info.iter != 1) /*�ǳ�ʼ����*/
		{
			for (const auto& idx : info.nodeIdxs)
			{
				auto node = mdl_->nodes_.at(idx);
				for (const auto& sidx : mdl_->links_.at(idx)->adj_)
				{
					auto shared = node->getShared(sidx);
					for (OpULInt i = 0; i < shared.getSize(); i++)
					{
						shared[i].unlock();
						shared[i].removeFixedValue();
						shared[i].lock();
					}
				}
			}
		}	
	};
	// lambda��������������̶�����
	auto solveIntFixed = [&]() {
		// �̶���������
		for (const auto& idx : info.nodeIdxs)
		{
			auto node = mdl_->nodes_.at(idx);
			for (const auto& sidx : mdl_->links_.at(idx)->adj_)
			{
				auto intVar = node->getIntVar();
				for (OpULInt i = 0; i < intVar.getSize(); i++)
				{
					intVar[i].unlock();
					intVar[i].setFixedValue(node->getValue(intVar[i]));
					intVar[i].lock();
				}
			}
		}
		// ���ֲ�ʽ�ɳ�������
		solveModel();
		// ����̶���������
		for (const auto& idx : info.nodeIdxs)
		{
			auto node = mdl_->nodes_.at(idx);
			for (const auto& sidx : mdl_->links_.at(idx)->adj_)
			{
				auto intVar = node->getIntVar();
				for (OpULInt i = 0; i < intVar.getSize(); i++)
				{
					intVar[i].unlock();
					intVar[i].removeFixedValue();
					intVar[i].lock();
				}
			}
		}
	};
	// lambda�����������о�
	auto updateLog = [&]() {
		// ���±�Ҫ���о�
		info.cMax0 = info.cMax1;
		info.cMax1 = Constant::Infinity;
		for (const auto& deviation : info.cost)
		{
			if (deviation.second.size() > 0)
			{
				OpFloat avrg(0);
				for (const auto& c : deviation.second)
					avrg += std::abs(c);
				avrg /= deviation.second.size();
				info.cMax1 = std::min(info.cMax1, avrg);
			}
		}
		info.NCC = info.cMax1;
		// ���³�����о�
		info.objVal0 = info.objVal1;
		info.objVal1 = 0;
		for (const auto& idx : info.nodeIdxs)
			info.objVal1 += mdl_->nodes_.at(idx)->getObjValue();
		info.SCC = std::abs(info.objVal1 - info.objVal0) / std::abs(info.objVal1);
		// �������о�
		if (std::abs(info.cMax1 - info.cMax0) < info.admmShakeGap)
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
			(info.NCC < info.admmGap1&& info.SCC < info.admmGap2) ||
			(info.shakeQuit && info.shake);
	};
	// lambda����ӡ�м���
	auto print = [&]() {
		for (const auto& idx : info.nodeIdxs)
		{
			auto node = mdl_->nodes_.at(idx);
			auto vars = node->getVar();
			std::cout << "sub model " << idx << ": " << std::endl;
			for (OpULInt i = 0; i < vars.getSize(); i++)
				std::cout << vars[i].getName() << ": " << node->getValue(vars[i]) << std::endl;
		}
	};

	try
	{
		// ��ʱ��
		OpTimer globalWatch(true);
		OpTimer localWatch(true);
		std::cout << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << "): Start" << std::endl;
		std::cout << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << "): init" << std::endl;
		// ��ʼ��
		init();
		// ����
		while (!quit())
		{
			localWatch.restart();
			std::cout << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << "): update models" << std::endl;
			updateModel();
			std::cout << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << "): solve models with fixed associated-variables " << std::endl;
			solveSharedFixed();
			std::cout << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << "): solve models with fixed integer-variables " << std::endl;
			solveIntFixed();
			std::cout << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << "): update deviation of associated-variables" << std::endl;
			updateNodeC();
			std::cout << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << "): update dual info" << std::endl;
			updateDual();
			std::cout << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << "): update convergence criterion" << std::endl;
			updateLog();
			info.printIterInfo(std::cout, tid, localWatch.second());
			info.iter++;
		}
		std::cout << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << "): Finish" << std::endl;
		std::cout << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << "): Time: " << globalWatch.second() << std::endl;
		successful = true;
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "[OPUA::Algorithm::OpAlgoADMM::solve](Thread: " << tid << ")Exception: unknown" << std::endl;
	}
	return successful;
}

Algorithm::OpAlgoADMM::OpAlgoADMM()
	: mdl_(nullptr)
{

}

Algorithm::OpAlgoADMM::OpAlgoADMM(OpNonHierarchicalModel model)
	: mdl_(model.getImpl())
{

}

Algorithm::OpAlgoADMM::~OpAlgoADMM()
{

}

void Algorithm::DefaultCfg4ADMM(Solver::OpConfig& config)
{
	config.regCfg("OPUA.Algorithm.ADMM.InitMode", OpLInt(0));
	config.regCfg("OPUA.Algorithm.ADMM.LambdaInitVal", OpFloat(1));
	config.regCfg("OPUA.Algorithm.ADMM.RhoInitVal", OpFloat(0.5));
	config.regCfg("OPUA.Algorithm.ADMM.ADMMIterMax", OpLInt(1000));
	config.regCfg("OPUA.Algorithm.ADMM.ADMMGap1", OpFloat(1e-3));
	config.regCfg("OPUA.Algorithm.ADMM.ADMMGap2", OpFloat(1e-3));
	config.regCfg("OPUA.Algorithm.ADMM.ADMMShakeGap", OpFloat(1e-3));
	config.regCfg("OPUA.Algorithm.ADMM.ADMMShakeCount", OpLInt(3));
	config.regCfg("OPUA.Algorithm.ADMM.ADMMShakeAutoQuit", OpBool(true));
	config.regCfg("OPUA.Algorithm.ADMM.MIPSolverMode", OpChar('G'));
}

Solver::OpConfig Algorithm::DefaultCfg4ADMM()
{
	Solver::OpConfig config;
	DefaultCfg4ADMM(config);
	return config;
}