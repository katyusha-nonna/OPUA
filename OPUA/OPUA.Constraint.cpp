#include "OPUA.Constraint.h"
#include <iostream>

using namespace OPUA;

/* OPUA::Constraint::OpLinConI */

class Constraint::OpLinConI
	: public OpImplBase
{
protected:
	OpBool cstd_; // �Ƿ�Ϊ��׼����Լ��(expr <=/== rhs)
	OpConSense csense_; // Լ������
	OpFloat crval_; // Լ���Ҳ�����(�������Ч)
	Expression::OpLinExpr clexpr_; // Լ����������ʽ
	Expression::OpLinExpr crexpr_; // Լ���Ҳ������ʽ
	OpStr cname_;

	friend class OpLinCon;
protected:
	void setSense(OpConSense sense); // ����(�ı�)Լ���ķ���
	void setName(OpStr name); // ����Լ������
	void setExpr(const Expression::OpLinExpr& expr, OpBool lhs); // ����Լ����/�ұ��ʽ
	void standardize(OpBool simplify); // Լ����׼��

	OpConSense getSense() const; // ��ȡԼ���ķ���
	OpStr getName() const; // ��ȡԼ������
	const Expression::OpLinExpr& getExpr(OpBool lhs) const; // ��ȡԼ����/�ұ��ʽ
	OpFloat getRHS() const; // ��ȡԼ���Ҳ�����(����׼��Լ������Ч)
	OpBool isStandard() const; // �Ƿ�Ϊ��ע��Լ��
protected:
	OpLinConI(OpEnvI* env);
	OpLinConI(OpEnvI* env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs);
	OpLinConI(OpEnvI* env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs, OpStr name);
public:
	virtual ~OpLinConI();
};

void Constraint::OpLinConI::setSense(OpConSense sense)
{
	if (!locked_)
		csense_ = sense;
}

void Constraint::OpLinConI::setName(OpStr name)
{
	cname_ = name;
}

void Constraint::OpLinConI::setExpr(const Expression::OpLinExpr& expr, OpBool lhs)
{
	if (!locked_)
	{
		if (lhs)
			clexpr_ = expr;
		else
			crexpr_ = expr;
	}
}

void Constraint::OpLinConI::standardize(OpBool simplify)
{
	if (!locked_)
	{
		if (csense_ == OpConSense::Equal || csense_ == OpConSense::LessEqual)
		{
			crval_ = crexpr_.getConstant() - clexpr_.getConstant();
			clexpr_ -= crexpr_;
			clexpr_.setLinTerm(0.0);
			crexpr_.clear();
			if (simplify)
				clexpr_.simplify(std::abs(crval_ * Constant::RelEpsilon));
			cstd_ = true;
		}
		else if (csense_ == OpConSense::GreatEqual)
		{
			crval_ = clexpr_.getConstant() - crexpr_.getConstant();
			//clexpr_ = (crexpr_ -= clexpr_).getLinTerm();
			crexpr_ -= clexpr_;
			crexpr_.setLinTerm(0.0);
			std::swap(clexpr_, crexpr_);
			crexpr_.clear();
			if (simplify)
				clexpr_.simplify(std::abs(crval_ * Constant::RelEpsilon));
			csense_ = OpConSense::LessEqual;
			cstd_ = true;
		}
	}
}

Constraint::OpConSense Constraint::OpLinConI::getSense() const
{
	return csense_;
}

OpStr Constraint::OpLinConI::getName() const
{
	return cname_;
}

const Expression::OpLinExpr& Constraint::OpLinConI::getExpr(OpBool lhs) const
{
	return lhs ? clexpr_ : crexpr_;
}

OpFloat Constraint::OpLinConI::getRHS() const
{
	return crval_;
}

OpBool Constraint::OpLinConI::isStandard() const
{
	return cstd_;
}

Constraint::OpLinConI::OpLinConI(OpEnvI* env)
	: OpImplBase('C', env), 
	cstd_(false), 
	csense_(OpConSense::LessEqual), 
	crval_(0.0), 
	clexpr_(0.0), 
	crexpr_(0.0), 
	cname_("lc_" + std::to_string(idx_))
{

}

Constraint::OpLinConI::OpLinConI(OpEnvI* env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs)
	: OpImplBase('C', env),
	cstd_(false),
	csense_(sense),
	crval_(0.0),
	clexpr_(lhs),
	crexpr_(rhs),
	cname_("lc_" + std::to_string(idx_))
{

}

Constraint::OpLinConI::OpLinConI(OpEnvI* env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs, OpStr name)
	: OpImplBase('C', env),
	cstd_(false),
	csense_(sense),
	crval_(0.0),
	clexpr_(lhs),
	crexpr_(rhs),
	cname_(name)
{

}

Constraint::OpLinConI::~OpLinConI()
{

}

/* OPUA::Constraint::OpQuadConI */

class Constraint::OpQuadConI
	: public OpImplBase
{
protected:
	OpBool cstd_; // �Ƿ�Ϊ��׼����Լ��(expr <=/== rhs)
	OpConSense csense_; // Լ������
	OpFloat crval_; // Լ���Ҳ�����(�������Ч)
	Expression::OpQuadExpr clexpr_; // Լ����������ʽ
	Expression::OpQuadExpr crexpr_; // Լ���Ҳ������ʽ
	OpStr cname_;

	friend class OpQuadCon;
protected:
	void setSense(OpConSense sense); // ����(�ı�)Լ���ķ���
	void setName(OpStr name); // ����Լ������
	void setExpr(const Expression::OpQuadExpr& expr, OpBool lhs); // ����Լ����/�ұ��ʽ
	void standardize(OpBool simplify); // Լ����׼��

	OpConSense getSense() const; // ��ȡԼ���ķ���
	OpStr getName() const; // ��ȡԼ������
	const Expression::OpQuadExpr& getExpr(OpBool lhs) const; // ��ȡԼ����/�ұ��ʽ
	OpFloat getRHS() const; // ��ȡԼ���Ҳ�����(����׼��Լ������Ч)
	OpBool isStandard() const; // �Ƿ�Ϊ��ע��Լ��
protected:
	OpQuadConI(OpEnvI* env);
	OpQuadConI(OpEnvI* env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs);
	OpQuadConI(OpEnvI* env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs, OpStr name);
public:
	virtual ~OpQuadConI();
};

void Constraint::OpQuadConI::setSense(OpConSense sense)
{
	if (!locked_)
		csense_ = sense;
}

void Constraint::OpQuadConI::setName(OpStr name)
{
	cname_ = name;
}

void Constraint::OpQuadConI::setExpr(const Expression::OpQuadExpr& expr, OpBool lhs)
{
	if (!locked_)
	{
		if (lhs)
			clexpr_ = expr;
		else
			crexpr_ = expr;
	}
}

void Constraint::OpQuadConI::standardize(OpBool simplify)
{
	if (!locked_)
	{
		if (csense_ == OpConSense::Equal || csense_ == OpConSense::LessEqual)
		{
			crval_ = crexpr_.getConstant() - clexpr_.getConstant();
			clexpr_ -= crexpr_;
			clexpr_.setLinTerm(0.0);
			crexpr_.clear();
			if (simplify)
				clexpr_.simplify(std::abs(crval_ * Constant::RelEpsilon));
			cstd_ = true;
		}
		else if (csense_ == OpConSense::GreatEqual)
		{
			crval_ = clexpr_.getConstant() - crexpr_.getConstant();
			//clexpr_ = (crexpr_ -= clexpr_).getLinTerm();
			crexpr_ -= clexpr_;
			crexpr_.setLinTerm(0.0);
			std::swap(clexpr_, crexpr_);
			crexpr_.clear();
			if (simplify)
				clexpr_.simplify(std::abs(crval_ * Constant::RelEpsilon));
			csense_ = OpConSense::LessEqual;
			cstd_ = true;
		}
	}
}

Constraint::OpConSense Constraint::OpQuadConI::getSense() const
{
	return csense_;
}

OpStr Constraint::OpQuadConI::getName() const
{
	return cname_;
}

const Expression::OpQuadExpr& Constraint::OpQuadConI::getExpr(OpBool lhs) const
{
	return lhs ? clexpr_ : crexpr_;
}

OpFloat Constraint::OpQuadConI::getRHS() const
{
	return crval_;
}

OpBool Constraint::OpQuadConI::isStandard() const
{
	return cstd_;
}

Constraint::OpQuadConI::OpQuadConI(OpEnvI* env)
	: OpImplBase('C', env),
	cstd_(false),
	csense_(OpConSense::LessEqual),
	crval_(0.0),
	clexpr_(0.0),
	crexpr_(0.0),
	cname_("qc_" + std::to_string(idx_))
{

}

Constraint::OpQuadConI::OpQuadConI(OpEnvI* env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs)
	: OpImplBase('C', env),
	cstd_(false),
	csense_(sense),
	crval_(0.0),
	clexpr_(lhs),
	crexpr_(rhs),
	cname_("qc_" + std::to_string(idx_))
{

}

Constraint::OpQuadConI::OpQuadConI(OpEnvI* env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs, OpStr name)
	: OpImplBase('C', env),
	cstd_(false),
	csense_(sense),
	crval_(0.0),
	clexpr_(lhs),
	crexpr_(rhs),
	cname_(name)
{

}

Constraint::OpQuadConI::~OpQuadConI()
{

}

/* OPUA::Constraint::OpSOSConI */

class Constraint::OpSOSConI
	: public OpImplBase
{
protected:
	OpConSense csense_; // Լ������
	Expression::OpLinExpr cvars_; // SOS������(����*Ȩ�أ������Ա��ʽ�洢)
	OpStr cname_; // Լ������

	friend class OpSOSCon;
protected:
	void setSense(OpConSense sense); // ����(�ı�)Լ���ķ���
	void setName(OpStr name); // ����Լ������
	void addVar(Variable::OpVar var, OpFloat weight); // ��SOS�����һ������(�ظ������׷��Ȩ��)

	OpConSense getSense() const; // ��ȡԼ���ķ���
	OpStr getName() const; // ��ȡԼ������
	const Expression::OpLinExpr& getSOSExpr() const; // ��ȡ��ʾSOS���ϵı��ʽ(����*Ȩ��)
	void removeVar(Variable::OpVar var);  // ��SOS���Ƴ�һ������
protected:
	OpSOSConI(OpEnvI* env);
	OpSOSConI(OpEnvI* env, OpConSense sense);
	OpSOSConI(OpEnvI* env, OpConSense sense, const Expression::OpLinExpr& expr);
	OpSOSConI(OpEnvI* env, OpConSense sense, const Expression::OpLinExpr& expr, OpStr name);
	OpSOSConI(OpEnvI* env, OpConSense sense, Variable::OpVarArr vars, Container::OpFloatArr weights);
	OpSOSConI(OpEnvI* env, OpConSense sense, Variable::OpVarArr vars, Container::OpFloatArr weights, OpStr name);
public:
	~OpSOSConI();
};

void Constraint::OpSOSConI::setSense(OpConSense sense)
{
	if (!locked_)
		csense_ = sense;
}

void Constraint::OpSOSConI::setName(OpStr name)
{
	cname_ = name;
}

void Constraint::OpSOSConI::addVar(Variable::OpVar var, OpFloat weight)
{
	if (!locked_)
		cvars_.addLinTerm(var, weight);
}

Constraint::OpConSense Constraint::OpSOSConI::getSense() const
{
	return csense_;
}

OpStr Constraint::OpSOSConI::getName() const
{
	return cname_;
}

const Expression::OpLinExpr& Constraint::OpSOSConI::getSOSExpr() const
{
	return cvars_;
}

void Constraint::OpSOSConI::removeVar(Variable::OpVar var)
{
	if (!locked_)
		cvars_.removeVar(var);
}

Constraint::OpSOSConI::OpSOSConI(OpEnvI* env)
	: OpImplBase('C', env),
	csense_(OpConSense::SOS1),
	cvars_(0.0),
	cname_("sos_" + std::to_string(idx_))
{

}

Constraint::OpSOSConI::OpSOSConI(OpEnvI* env, OpConSense sense)
	: OpImplBase('C', env), 
	csense_(sense), 
	cvars_(0.0),
	cname_("sos_" + std::to_string(idx_))
{

}

Constraint::OpSOSConI::OpSOSConI(OpEnvI* env, OpConSense sense, const Expression::OpLinExpr& expr)
	: OpImplBase('C', env),
	csense_(sense),
	cvars_(expr),
	cname_("sos_" + std::to_string(idx_))
{

}

Constraint::OpSOSConI::OpSOSConI(OpEnvI* env, OpConSense sense, const Expression::OpLinExpr& expr, OpStr name)
	: OpImplBase('C', env),
	csense_(sense),
	cvars_(expr),
	cname_(name)
{

}

Constraint::OpSOSConI::OpSOSConI(OpEnvI* env, OpConSense sense, Variable::OpVarArr vars, Container::OpFloatArr weights)
	: OpImplBase('C', env),
	csense_(sense),
	cvars_(0.0),
	cname_("sos_" + std::to_string(idx_))
{
	for (OpULInt i = 0; i < vars.getSize(); i++)
		cvars_.addLinTerm(vars[i], weights[i]);
}

Constraint::OpSOSConI::OpSOSConI(OpEnvI* env, OpConSense sense, Variable::OpVarArr vars, Container::OpFloatArr weights, OpStr name)
	: OpImplBase('C', env),
	csense_(sense),
	cvars_(0.0),
	cname_(name)
{
	for (OpULInt i = 0; i < vars.getSize(); i++)
		cvars_.addLinTerm(vars[i], weights[i]);
}

Constraint::OpSOSConI::~OpSOSConI()
{

}

/* OPUA::Constraint::OpNLConI */

class Constraint::OpNLConI
	: public OpImplBase
{
protected:
	Variable::OpVar clvar_; // Լ���������
	Expression::OpNLExpr crexpr_; // Լ���Ҳ������ʽ

	OpStr cname_;

	friend class OpNLCon;
protected:
	void setName(OpStr name); // ����Լ������
	void setVar(Variable::OpVar var); // �������������
	void setExpr(const Expression::OpNLExpr& expr); // �����Ҳ������ʽ
	Variable::OpVar getVar() const; // ��ȡ�Ҳ�������
	const Expression::OpNLExpr& getExpr() const; // ��ȡ��������ʽ
	Expression::OpNLFunc getFunction() const; // ��ȡԼ���Ĳ�������
	OpStr getName() const; // ��ȡԼ������
protected:
	OpNLConI(OpEnvI* env);
	OpNLConI(OpEnvI* env, Variable::OpVar var, const Expression::OpNLExpr& expr);
	OpNLConI(OpEnvI* env, Variable::OpVar var, const Expression::OpNLExpr& expr, OpStr name);
public:
	virtual ~OpNLConI();
};

void Constraint::OpNLConI::setName(OpStr name)
{
	cname_ = name;
}

void Constraint::OpNLConI::setVar(Variable::OpVar var)
{
	if (!locked_)
		clvar_ = var;
}

void Constraint::OpNLConI::setExpr(const Expression::OpNLExpr& expr)
{
	if (!locked_)
		crexpr_ = expr;
}

Variable::OpVar Constraint::OpNLConI::getVar() const
{
	return clvar_;
}

const Expression::OpNLExpr& Constraint::OpNLConI::getExpr() const
{
	return crexpr_;
}

Expression::OpNLFunc Constraint::OpNLConI::getFunction() const
{
	return crexpr_.getFunction();
}

OpStr Constraint::OpNLConI::getName() const
{
	return cname_;
}

Constraint::OpNLConI::OpNLConI(OpEnvI* env)
	: OpImplBase('C', env),
	clvar_(),
	crexpr_(),
	cname_("nlc_" + std::to_string(idx_))
{

}

Constraint::OpNLConI::OpNLConI(OpEnvI* env, Variable::OpVar var, const Expression::OpNLExpr& expr)
	: OpImplBase('C', env),
	clvar_(var),
	crexpr_(expr),
	cname_("nlc_" + std::to_string(idx_))
{

}

Constraint::OpNLConI::OpNLConI(OpEnvI* env, Variable::OpVar var, const Expression::OpNLExpr& expr, OpStr name)
	: OpImplBase('C', env),
	clvar_(var),
	crexpr_(expr),
	cname_(name)
{

}

Constraint::OpNLConI::~OpNLConI()
{

}

/* OPUA::Constraint::OpLinCon */

void Constraint::OpLinCon::setSense(OpConSense sense)
{
	static_cast<OpLinConI*>(impl_)->setSense(sense);
}

void Constraint::OpLinCon::setName(OpStr name)
{
	static_cast<OpLinConI*>(impl_)->setName(name);
}

void Constraint::OpLinCon::setExpr(const Expression::OpLinExpr& expr, OpBool lhs)
{
	static_cast<OpLinConI*>(impl_)->setExpr(expr, lhs);
}

void Constraint::OpLinCon::standardize(OpBool simplify)
{
	static_cast<OpLinConI*>(impl_)->standardize(simplify);
}

Constraint::OpConSense Constraint::OpLinCon::getSense() const
{
	return static_cast<OpLinConI*>(impl_)->getSense();
}

OpStr Constraint::OpLinCon::getName() const
{
	return static_cast<OpLinConI*>(impl_)->getName();
}

const Expression::OpLinExpr& Constraint::OpLinCon::getExpr(OpBool lhs) const
{
	return static_cast<OpLinConI*>(impl_)->getExpr(lhs);
}

OpFloat Constraint::OpLinCon::getRHS() const
{
	return static_cast<OpLinConI*>(impl_)->getRHS();
}

OpBool Constraint::OpLinCon::isStandard() const
{
	return static_cast<OpLinConI*>(impl_)->isStandard();
}

Constraint::OpLinConI* Constraint::OpLinCon::getImpl() const
{
	return static_cast<OpLinConI*>(impl_);
}

OpBool Constraint::OpLinCon::operator==(const OpLinCon& con)
{
	return impl_ == con.getImpl();
}

OpBool Constraint::OpLinCon::operator!=(const OpLinCon& con)
{
	return impl_ != con.getImpl();
}

Constraint::OpLinCon::OpLinCon()
{

}

Constraint::OpLinCon::OpLinCon(Constraint::OpLinConI* impl)
{
	impl_ = impl;
}

Constraint::OpLinCon::OpLinCon(OpEnv env)
{
	impl_ = new OpLinConI(env.getImpl());
}

Constraint::OpLinCon::OpLinCon(OpEnv env, const Expression::OpLinExpr& lhs, Constraint::OpConSense sense, const Expression::OpLinExpr& rhs)
{
	impl_ = new OpLinConI(env.getImpl(), lhs, sense, rhs);
}

Constraint::OpLinCon::OpLinCon(OpEnv env, const Expression::OpLinExpr& lhs, Constraint::OpConSense sense, const Expression::OpLinExpr& rhs, OpStr name)
{
	impl_ = new OpLinConI(env.getImpl(), lhs, sense, rhs, name);
}

OPUA::Constraint::OpLinCon::~OpLinCon()
{

}

/* OPUA::Constraint::OpQuadCon */

void Constraint::OpQuadCon::setSense(OpConSense sense)
{
	static_cast<OpQuadConI*>(impl_)->setSense(sense);
}

void Constraint::OpQuadCon::setName(OpStr name)
{
	static_cast<OpQuadConI*>(impl_)->setName(name);
}

void Constraint::OpQuadCon::setExpr(const Expression::OpQuadExpr& expr, OpBool lhs)
{
	static_cast<OpQuadConI*>(impl_)->setExpr(expr, lhs);
}

void Constraint::OpQuadCon::standardize(OpBool simplify)
{
	static_cast<OpQuadConI*>(impl_)->standardize(simplify);
}

Constraint::OpConSense Constraint::OpQuadCon::getSense() const
{
	return static_cast<OpQuadConI*>(impl_)->getSense();
}

OpStr Constraint::OpQuadCon::getName() const
{
	return static_cast<OpQuadConI*>(impl_)->getName();
}

const Expression::OpQuadExpr& Constraint::OpQuadCon::getExpr(OpBool lhs) const
{
	return static_cast<OpQuadConI*>(impl_)->getExpr(lhs);
}

OpFloat Constraint::OpQuadCon::getRHS() const
{
	return static_cast<OpQuadConI*>(impl_)->getRHS();
}

OpBool Constraint::OpQuadCon::isStandard() const
{
	return static_cast<OpQuadConI*>(impl_)->isStandard();
}

Constraint::OpQuadConI* Constraint::OpQuadCon::getImpl() const
{
	return static_cast<OpQuadConI*>(impl_);
}

OpBool Constraint::OpQuadCon::operator==(const OpQuadCon& con)
{
	return impl_ == con.getImpl();
}

OpBool Constraint::OpQuadCon::operator!=(const OpQuadCon& con)
{
	return impl_ != con.getImpl();
}

Constraint::OpQuadCon::OpQuadCon()
{

}

Constraint::OpQuadCon::OpQuadCon(Constraint::OpQuadConI* impl)
{
	impl_ = impl;
}

Constraint::OpQuadCon::OpQuadCon(OpEnv env)
{
	impl_ = new OpQuadConI(env.getImpl());
}

Constraint::OpQuadCon::OpQuadCon(OpEnv env, const Expression::OpQuadExpr& lhs, Constraint::OpConSense sense, const Expression::OpQuadExpr& rhs)
{
	impl_ = new OpQuadConI(env.getImpl(), lhs, sense, rhs);
}

Constraint::OpQuadCon::OpQuadCon(OpEnv env, const Expression::OpQuadExpr& lhs, Constraint::OpConSense sense, const Expression::OpQuadExpr& rhs, OpStr name)
{
	impl_ = new OpQuadConI(env.getImpl(), lhs, sense, rhs, name);
}

OPUA::Constraint::OpQuadCon::~OpQuadCon()
{

}

/* OPUA::Constraint::OpSOSCon */

void Constraint::OpSOSCon::setSense(OpConSense sense)
{
	static_cast<OpSOSConI*>(impl_)->setSense(sense);
}

void Constraint::OpSOSCon::setName(OpStr name)
{
	static_cast<OpSOSConI*>(impl_)->setName(name);
}

void Constraint::OpSOSCon::addVar(Variable::OpVar var, OpFloat weight)
{
	static_cast<OpSOSConI*>(impl_)->addVar(var, weight);
}

Constraint::OpConSense Constraint::OpSOSCon::getSense() const
{
	return static_cast<OpSOSConI*>(impl_)->getSense();
}

OpStr Constraint::OpSOSCon::getName() const
{
	return static_cast<OpSOSConI*>(impl_)->getName();
}

const Expression::OpLinExpr& Constraint::OpSOSCon::getSOSExpr() const
{
	return static_cast<OpSOSConI*>(impl_)->getSOSExpr();
}

void Constraint::OpSOSCon::removeVar(Variable::OpVar var)
{
	static_cast<OpSOSConI*>(impl_)->removeVar(var);
}

Constraint::OpSOSConI* Constraint::OpSOSCon::getImpl() const
{
	return static_cast<OpSOSConI*>(impl_);
}

OpBool Constraint::OpSOSCon::operator==(const OpSOSCon& con)
{
	return impl_ == con.getImpl();
}

OpBool Constraint::OpSOSCon::operator!=(const OpSOSCon& con)
{
	return impl_ != con.getImpl();
}

Constraint::OpSOSCon::OpSOSCon()
{

}

Constraint::OpSOSCon::OpSOSCon(OpSOSConI* impl)
{
	impl_ = impl;
}

Constraint::OpSOSCon::OpSOSCon(OpEnv env)
{
	impl_ = new OpSOSConI(env.getImpl());
}

Constraint::OpSOSCon::OpSOSCon(OpEnv env, OpConSense sense)
{
	impl_ = new OpSOSConI(env.getImpl(), sense);
}

Constraint::OpSOSCon::OpSOSCon(OpEnv env, OpConSense sense, const Expression::OpLinExpr& expr)
{
	impl_ = new OpSOSConI(env.getImpl(), sense, expr);
}

Constraint::OpSOSCon::OpSOSCon(OpEnv env, OpConSense sense, const Expression::OpLinExpr& expr, OpStr name)
{
	impl_ = new OpSOSConI(env.getImpl(), sense, expr, name);
}

Constraint::OpSOSCon::OpSOSCon(OpEnv env, OpConSense sense, Variable::OpVarArr vars, Container::OpFloatArr weights)
{
	impl_ = new OpSOSConI(env.getImpl(), sense, vars, weights);
}

Constraint::OpSOSCon::OpSOSCon(OpEnv env, OpConSense sense, Variable::OpVarArr vars, Container::OpFloatArr weights, OpStr name)
{
	impl_ = new OpSOSConI(env.getImpl(), sense, vars, weights, name);
}

OPUA::Constraint::OpSOSCon::~OpSOSCon()
{

}

/* OPUA::Constraint::OpNLCon */

void Constraint::OpNLCon::setName(OpStr name)
{
	static_cast<OpNLConI*>(impl_)->setName(name);
}

void Constraint::OpNLCon::setVar(Variable::OpVar var)
{
	static_cast<OpNLConI*>(impl_)->setVar(var);
}

void Constraint::OpNLCon::setExpr(const Expression::OpNLExpr& expr)
{
	static_cast<OpNLConI*>(impl_)->setExpr(expr);
}

Expression::OpNLFunc Constraint::OpNLCon::getFunction() const
{
	return static_cast<OpNLConI*>(impl_)->getFunction();
}

const Expression::OpNLExpr& Constraint::OpNLCon::getExpr() const
{
	return static_cast<OpNLConI*>(impl_)->getExpr();
}

Variable::OpVar Constraint::OpNLCon::getVar() const
{
	return static_cast<OpNLConI*>(impl_)->getVar();
}

OpStr Constraint::OpNLCon::getName() const
{
	return static_cast<OpNLConI*>(impl_)->getName();
}

Constraint::OpNLConI* Constraint::OpNLCon::getImpl() const
{
	return static_cast<OpNLConI*>(impl_);
}

OpBool Constraint::OpNLCon::operator==(const OpNLCon& con)
{
	return impl_ == con.getImpl();
}

OpBool Constraint::OpNLCon::operator!=(const OpNLCon& con)
{
	return impl_ != con.getImpl();
}

Constraint::OpNLCon::OpNLCon()
{

}

Constraint::OpNLCon::OpNLCon(OpNLConI* impl)
{
	impl_ = impl;
}

Constraint::OpNLCon::OpNLCon(OpEnv env)
{
	impl_ = new OpNLConI(env.getImpl());
}

Constraint::OpNLCon::OpNLCon(OpEnv env, Variable::OpVar var, const Expression::OpNLExpr& expr)
{
	impl_ = new OpNLConI(env.getImpl(), var, expr);
}

Constraint::OpNLCon::OpNLCon(OpEnv env, Variable::OpVar var, const Expression::OpNLExpr& expr, OpStr name)
{
	impl_ = new OpNLConI(env.getImpl(), var, expr, name);
}

Constraint::OpNLCon::~OpNLCon()
{

}

OpStr Constraint::ConSense2Str(OpConSense sense)
{
	OpStr str(" ? ");
	if (sense == OpConSense::Equal)
		str = " == ";
	else if (sense == OpConSense::LessEqual)
		str = " <= ";
	else if (sense == OpConSense::GreatEqual)
		str = " >= ";
	else if (sense == OpConSense::SOS1)
		str = " SOS1 ";
	else if (sense == OpConSense::SOS2)
		str = " SOS2 ";
	return str;
}

std::ostream& Constraint::operator<<(std::ostream& stream, OpLinCon con)
{
	stream << con.getExpr(true);
	stream << ConSense2Str(con.getSense());
	if (con.isStandard())
		stream << con.getRHS();
	else
		stream << con.getExpr(false);
	return stream;
}

Constraint::OpLinCon Constraint::operator<=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs)
{
	OpLinCon con(nullptr);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpLinCon(OpEnv(lenv), lhs, OpConSense::LessEqual, rhs);
	}
	else if (!lenv && renv)
		con = OpLinCon(OpEnv(renv), lhs, OpConSense::LessEqual, rhs);
	else if (lenv && !renv)
		con = OpLinCon(OpEnv(lenv), lhs, OpConSense::LessEqual, rhs);
	return con;
}

Constraint::OpLinCon Constraint::operator>=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs)
{
	OpLinCon con(nullptr);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpLinCon(OpEnv(lenv), lhs, OpConSense::GreatEqual, rhs);
	}
	else if (!lenv && renv)
		con = OpLinCon(OpEnv(renv), lhs, OpConSense::GreatEqual, rhs);
	else if (lenv && !renv)
		con = OpLinCon(OpEnv(lenv), lhs, OpConSense::GreatEqual, rhs);
	return con;
}

Constraint::OpLinCon Constraint::operator==(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs)
{
	OpLinCon con(nullptr);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpLinCon(OpEnv(lenv), lhs, OpConSense::Equal, rhs);
	}
	else if (!lenv && renv)
		con = OpLinCon(OpEnv(renv), lhs, OpConSense::Equal, rhs);
	else if (lenv && !renv)
		con = OpLinCon(OpEnv(lenv), lhs, OpConSense::Equal, rhs);
	return con;
}

std::ostream& Constraint::operator<<(std::ostream& stream, OpQuadCon con)
{
	stream << con.getExpr(true);
	stream << ConSense2Str(con.getSense());
	if (con.isStandard())
		stream << con.getRHS();
	else
		stream << con.getExpr(false);
	return stream;
}

Constraint::OpQuadCon Constraint::operator<=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs)
{
	OpQuadCon con(nullptr);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpQuadCon(OpEnv(lenv), lhs, OpConSense::LessEqual, rhs);
	}
	else if (!lenv && renv)
		con = OpQuadCon(OpEnv(renv), lhs, OpConSense::LessEqual, rhs);
	else if (lenv && !renv)
		con = OpQuadCon(OpEnv(lenv), lhs, OpConSense::LessEqual, rhs);
	return con;
}

Constraint::OpQuadCon Constraint::operator>=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs)
{
	OpQuadCon con(nullptr);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpQuadCon(OpEnv(lenv), lhs, OpConSense::GreatEqual, rhs);
	}
	else if (!lenv && renv)
		con = OpQuadCon(OpEnv(renv), lhs, OpConSense::GreatEqual, rhs);
	else if (lenv && !renv)
		con = OpQuadCon(OpEnv(lenv), lhs, OpConSense::GreatEqual, rhs);
	return con;
}

Constraint::OpQuadCon Constraint::operator==(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs)
{
	OpQuadCon con(nullptr);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpQuadCon(OpEnv(lenv), lhs, OpConSense::Equal, rhs);
	}
	else if (!lenv && renv)
		con = OpQuadCon(OpEnv(renv), lhs, OpConSense::Equal, rhs);
	else if (lenv && !renv)
		con = OpQuadCon(OpEnv(lenv), lhs, OpConSense::Equal, rhs);
	return con;
}

std::ostream& Constraint::operator<<(std::ostream& stream, OpSOSCon con)
{
	stream << ConSense2Str(con.getSense()) << "( ";
	auto& vars(con.getSOSExpr());
	for (auto iter = vars.getLBegin(); iter != vars.getLEnd(); ++iter)
	{
		if (iter != vars.getLBegin())
			stream << ", ";
		stream << iter.getVar().getName();
	}
	stream << " )";
	return stream;
}

std::ostream& Constraint::operator<<(std::ostream& stream, OpNLCon con)
{
	stream << con.getVar().getName() << " == ";
	auto& expr(con.getExpr());
	stream << Expression::NLFunc2Str(expr.getFunction()) << "( ";
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
	{
		if (iter != expr.getNLBegin())
			stream << ", ";
		stream << iter.getVar().getName();
	}
	stream << " )";
	return stream;
}

Constraint::OpNLCon Constraint::operator==(const Expression::OpNLExpr& lhs, Variable::OpVar rhs)
{
	OpNLCon con(nullptr);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpNLCon(OpEnv(lenv), rhs, lhs);
	}
	else if (!lenv && renv)
		con = OpNLCon(OpEnv(renv), rhs, lhs);
	else if (lenv && !renv)
		con = OpNLCon(OpEnv(lenv), rhs, lhs);
	return con;
}

Constraint::OpNLCon Constraint::operator==(Variable::OpVar lhs, const Expression::OpNLExpr& rhs)
{
	OpNLCon con(nullptr);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpNLCon(OpEnv(lenv), lhs, rhs);
	}
	else if (!lenv && renv)
		con = OpNLCon(OpEnv(renv), lhs, rhs);
	else if (lenv && !renv)
		con = OpNLCon(OpEnv(lenv), lhs, rhs);
	return con;
}

Constraint::OpNLCon Constraint::OpSum(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X, OpFloat a)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Sum, a, X);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpMax(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X, OpFloat a)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Max, a, X);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpMin(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X, OpFloat a)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Min, a, X);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpAbs(OpEnv env, Variable::OpVar x1, Variable::OpVar x2)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Abs);
	expr.addVar(x2);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpSquare(OpEnv env, Variable::OpVar x1, Variable::OpVar x2)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Square);
	expr.addVar(x2);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpSqrt(OpEnv env, Variable::OpVar x1, Variable::OpVar x2)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Sqrt);
	expr.addVar(x2);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpPow(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, OpFloat a)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Pow);
	expr.addVar(x2);
	expr.setParam(a);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpExp1(OpEnv env, Variable::OpVar x1, Variable::OpVar x2)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Exp1);
	expr.addVar(x2);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpExp2(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, OpFloat a)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Exp2);
	expr.addVar(x2);
	expr.setParam(a);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpLog1(OpEnv env, Variable::OpVar x1, Variable::OpVar x2)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Log1);
	expr.addVar(x2);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpLog2(OpEnv env, Variable::OpVar x1, Variable::OpVar x2)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Log2);
	expr.addVar(x2);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpLog3(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, OpFloat a)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Log3);
	expr.addVar(x2);
	expr.setParam(a);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpSin(OpEnv env, Variable::OpVar x1, Variable::OpVar x2)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Sin);
	expr.addVar(x2);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpCos(OpEnv env, Variable::OpVar x1, Variable::OpVar x2)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Cos);
	expr.addVar(x2);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}

Constraint::OpNLCon Constraint::OpTan(OpEnv env, Variable::OpVar x1, Variable::OpVar x2)
{
	Expression::OpNLExpr expr(Expression::OpNLFunc::Tan);
	expr.addVar(x2);
	OpNLCon tmp(env);
	tmp.setVar(x1);
	tmp.setExpr(expr);
	tmp.lock();
	return tmp;
}
