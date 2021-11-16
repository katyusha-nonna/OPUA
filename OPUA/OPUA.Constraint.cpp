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
	cname_("c_" + std::to_string(idx_))
{

}

Constraint::OpLinConI::OpLinConI(OpEnvI* env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs)
	: OpImplBase('C', env),
	cstd_(false),
	csense_(sense),
	crval_(0.0),
	clexpr_(lhs),
	crexpr_(rhs),
	cname_("c_" + std::to_string(idx_))
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
	cname_("c_" + std::to_string(idx_))
{

}

Constraint::OpQuadConI::OpQuadConI(OpEnvI* env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs)
	: OpImplBase('C', env),
	cstd_(false),
	csense_(sense),
	crval_(0.0),
	clexpr_(lhs),
	crexpr_(rhs),
	cname_("c_" + std::to_string(idx_))
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

/* OPUA::Constraint::OpLinCon */

void Constraint::OpLinCon::setSense(OpConSense sense)
{
	impl_->setSense(sense);
}

void Constraint::OpLinCon::setName(OpStr name)
{
	impl_->setName(name);
}

void Constraint::OpLinCon::setExpr(const Expression::OpLinExpr& expr, OpBool lhs)
{
	impl_->setExpr(expr, lhs);
}

void Constraint::OpLinCon::standardize(OpBool simplify)
{
	impl_->standardize(simplify);
}

Constraint::OpConSense Constraint::OpLinCon::getSense() const
{
	return impl_->getSense();
}

OpStr Constraint::OpLinCon::getName() const
{
	return impl_->getName();
}

const Expression::OpLinExpr& Constraint::OpLinCon::getExpr(OpBool lhs) const
{
	return impl_->getExpr(lhs);
}

OpFloat Constraint::OpLinCon::getRHS() const
{
	return impl_->getRHS();
}

OpBool Constraint::OpLinCon::isStandard() const
{
	return impl_->isStandard();
}

OpBool Constraint::OpLinCon::isLocked() const
{
	return impl_->isLocked();
}

Constraint::OpLinConI* Constraint::OpLinCon::getImpl() const
{
	return impl_;
}

OpLInt Constraint::OpLinCon::getIndex() const
{
	return impl_->getIndex();
}

OpEnv Constraint::OpLinCon::getEnv() const
{
	return impl_ ? OpEnv(impl_->getEnv()) : OpEnv(nullptr);
}

void Constraint::OpLinCon::release()
{
	if (impl_)
	{
		impl_->release();
		impl_ = nullptr;
	}
}

void Constraint::OpLinCon::lock()
{
	impl_->lock();
}

void Constraint::OpLinCon::unlock()
{
	impl_->unlock();
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
	: impl_(nullptr)
{

}

Constraint::OpLinCon::OpLinCon(Constraint::OpLinConI* impl)
	: impl_(impl)
{

}

Constraint::OpLinCon::OpLinCon(OpEnv env)
	: impl_(new OpLinConI(env.getImpl()))
{

}

Constraint::OpLinCon::OpLinCon(OpEnv env, const Expression::OpLinExpr& lhs, Constraint::OpConSense sense, const Expression::OpLinExpr& rhs)
	: impl_(new OpLinConI(env.getImpl(), lhs, sense, rhs))
{

}

Constraint::OpLinCon::OpLinCon(OpEnv env, const Expression::OpLinExpr& lhs, Constraint::OpConSense sense, const Expression::OpLinExpr& rhs, OpStr name)
	: impl_(new OpLinConI(env.getImpl(), lhs, sense, rhs, name))
{

}

/* OPUA::Constraint::OpQuadCon */

void Constraint::OpQuadCon::setSense(OpConSense sense)
{
	impl_->setSense(sense);
}

void Constraint::OpQuadCon::setName(OpStr name)
{
	impl_->setName(name);
}

void Constraint::OpQuadCon::setExpr(const Expression::OpQuadExpr& expr, OpBool lhs)
{
	impl_->setExpr(expr, lhs);
}

void Constraint::OpQuadCon::standardize(OpBool simplify)
{
	impl_->standardize(simplify);
}

Constraint::OpConSense Constraint::OpQuadCon::getSense() const
{
	return impl_->getSense();
}

OpStr Constraint::OpQuadCon::getName() const
{
	return impl_->getName();
}

const Expression::OpQuadExpr& Constraint::OpQuadCon::getExpr(OpBool lhs) const
{
	return impl_->getExpr(lhs);
}

OpFloat Constraint::OpQuadCon::getRHS() const
{
	return impl_->getRHS();
}

OpBool Constraint::OpQuadCon::isStandard() const
{
	return impl_->isStandard();
}

OpBool Constraint::OpQuadCon::isLocked() const
{
	return impl_->isLocked();
}

Constraint::OpQuadConI* Constraint::OpQuadCon::getImpl() const
{
	return impl_;
}

OpLInt Constraint::OpQuadCon::getIndex() const
{
	return impl_->getIndex();
}

OpEnv Constraint::OpQuadCon::getEnv() const
{
	return impl_ ? OpEnv(impl_->getEnv()) : OpEnv(nullptr);
}

void Constraint::OpQuadCon::release()
{
	if (impl_)
	{
		impl_->release();
		impl_ = nullptr;
	}
}

void Constraint::OpQuadCon::lock()
{
	impl_->lock();
}

void Constraint::OpQuadCon::unlock()
{
	impl_->unlock();
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
	: impl_(nullptr)
{

}

Constraint::OpQuadCon::OpQuadCon(Constraint::OpQuadConI* impl)
	: impl_(impl)
{

}

Constraint::OpQuadCon::OpQuadCon(OpEnv env)
	: impl_(new OpQuadConI(env.getImpl()))
{

}

Constraint::OpQuadCon::OpQuadCon(OpEnv env, const Expression::OpQuadExpr& lhs, Constraint::OpConSense sense, const Expression::OpQuadExpr& rhs)
	: impl_(new OpQuadConI(env.getImpl(), lhs, sense, rhs))
{

}

Constraint::OpQuadCon::OpQuadCon(OpEnv env, const Expression::OpQuadExpr& lhs, Constraint::OpConSense sense, const Expression::OpQuadExpr& rhs, OpStr name)
	: impl_(new OpQuadConI(env.getImpl(), lhs, sense, rhs, name))
{

}

/* OPUA::Constraint::OpSOSCon */

void Constraint::OpSOSCon::setSense(OpConSense sense)
{
	impl_->setSense(sense);
}

void Constraint::OpSOSCon::setName(OpStr name)
{
	impl_->setName(name);
}

void Constraint::OpSOSCon::addVar(Variable::OpVar var, OpFloat weight)
{
	impl_->addVar(var, weight);
}

Constraint::OpConSense Constraint::OpSOSCon::getSense() const
{
	return impl_->getSense();
}

OpStr Constraint::OpSOSCon::getName() const
{
	return impl_->getName();
}

const Expression::OpLinExpr& Constraint::OpSOSCon::getSOSExpr() const
{
	return impl_->getSOSExpr();
}

void Constraint::OpSOSCon::removeVar(Variable::OpVar var)
{
	impl_->removeVar(var);
}

OpBool Constraint::OpSOSCon::isLocked() const
{
	return impl_->isLocked();
}

Constraint::OpSOSConI* Constraint::OpSOSCon::getImpl() const
{
	return impl_;
}

OpLInt Constraint::OpSOSCon::getIndex() const
{
	return impl_->getIndex();
}

OpEnv Constraint::OpSOSCon::getEnv() const
{
	return impl_ ? OpEnv(impl_->getEnv()) : OpEnv(nullptr);
}

void Constraint::OpSOSCon::release()
{
	if (impl_)
	{
		impl_->release();
		impl_ = nullptr;
	}
}

void Constraint::OpSOSCon::lock()
{
	impl_->lock();
}

void Constraint::OpSOSCon::unlock()
{
	impl_->unlock();
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
	: impl_(nullptr)
{

}

Constraint::OpSOSCon::OpSOSCon(OpSOSConI* impl)
	: impl_(impl)
{

}

Constraint::OpSOSCon::OpSOSCon(OpEnv env)
	: impl_(new OpSOSConI(env.getImpl()))
{

}

Constraint::OpSOSCon::OpSOSCon(OpEnv env, OpConSense sense)
	: impl_(new OpSOSConI(env.getImpl(), sense))
{

}

Constraint::OpSOSCon::OpSOSCon(OpEnv env, OpConSense sense, const Expression::OpLinExpr& expr)
	: impl_(new OpSOSConI(env.getImpl(), sense, expr))
{

}

Constraint::OpSOSCon::OpSOSCon(OpEnv env, OpConSense sense, const Expression::OpLinExpr& expr, OpStr name)
	: impl_(new OpSOSConI(env.getImpl(), sense, expr, name))
{

}

Constraint::OpSOSCon::OpSOSCon(OpEnv env, OpConSense sense, Variable::OpVarArr vars, Container::OpFloatArr weights)
	: impl_(new OpSOSConI(env.getImpl(), sense, vars, weights))
{

}

Constraint::OpSOSCon::OpSOSCon(OpEnv env, OpConSense sense, Variable::OpVarArr vars, Container::OpFloatArr weights, OpStr name)
	: impl_(new OpSOSConI(env.getImpl(), sense, vars, weights, name))
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
	stream << " )" << std::endl;
}


