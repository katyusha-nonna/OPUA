#include "OPUA.Objective.h"

using namespace OPUA;

OpStr Objective::ObjSense2Str(OpObjSense sense)
{
	OpStr str(" ? ");
	if (sense == OpObjSense::Min)
		str = " Minimize ";
	else if (sense == OpObjSense::Max)
		str = " Maximize ";
	return str;
}

std::ostream& Objective::operator<<(std::ostream& stream, OpObj obj)
{
	stream << ObjSense2Str(obj.getSense());
	if (obj.isQuad())
	{
		stream << obj.getQuadExpr() << " + ";
	}
	stream << obj.getLinExpr();
	return stream;
}

Objective::OpObj Objective::OpMinimize(OpEnv env, const Expression::OpLinExpr& lexpr)
{
	return OpObj(env, OpObjSense::Min, lexpr);
}

Objective::OpObj Objective::OpMinimize(OpEnv env, const Expression::OpQuadExpr& qexpr)
{
	return OpObj(env, OpObjSense::Min, qexpr.getLinTerm() += qexpr.getConstant(), qexpr.getQuadTerm());
}

Objective::OpObj Objective::OpMaximize(OpEnv env, const Expression::OpLinExpr& lexpr)
{
	return OpObj(env, OpObjSense::Max, lexpr);
}

Objective::OpObj Objective::OpMaximize(OpEnv env, const Expression::OpQuadExpr& qexpr)
{
	return OpObj(env, OpObjSense::Max, qexpr.getLinTerm() += qexpr.getConstant(), qexpr.getQuadTerm());
}

/* OPUA::Objective::OpObjI */

class Objective::OpObjI
	: public OpImplBase
{
protected:
	OpObjSense osense_; // Ŀ�꺯������
	Expression::OpLinExpr olexpr_; // Ŀ�꺯��������
	Expression::OpQuadExpr oqexpr_; // Ŀ�꺯��������
	OpStr oname_; // Ŀ�꺯������

	friend class OpObj;
protected:
	void setLinExpr(const Expression::OpLinExpr& expr); // ������������ʽ
	void setQuadExpr(const Expression::OpQuadExpr& expr); // ���ö�������ʽ(�Զ�����������)
	void setSense(OpObjSense sense); // ����Ŀ�꺯������
	void setName(OpStr name); // ����Ŀ�꺯������

	const Expression::OpLinExpr& getLinExpr() const; // ��ȡ��������ʽ
	const Expression::OpQuadExpr& getQuadExpr() const; // ��ȡ��������ʽ
	OpObjSense getSense() const; // ��ȡĿ�꺯������
	OpStr getName() const; // ��ȡĿ�꺯������
	OpBool isConstant() const; // �Ƿ�Ϊ����Ŀ�꺯��
	OpBool isQuad() const; // �Ƿ�Ϊ����Ŀ�꺯��
	virtual OpULInt getMemoryUsage() const; // ��ȡ�ڴ�ռ��
protected:
	OpObjI(OpEnvI* env);
	OpObjI(OpEnvI* env, OpObjSense sense);
	OpObjI(OpEnvI* env, OpObjSense sense, const Expression::OpLinExpr& lexpr);
	OpObjI(OpEnvI* env, OpObjSense sense, const Expression::OpLinExpr& lexpr, const Expression::OpQuadExpr& qexpr);
	OpObjI(OpEnvI* env, OpObjSense sense, const Expression::OpLinExpr& lexpr, const Expression::OpQuadExpr& qexpr, OpStr name);
public:
	virtual ~OpObjI();
};

void Objective::OpObjI::setLinExpr(const Expression::OpLinExpr& expr)
{
	if (!locked_)
		olexpr_ = expr;
}

void Objective::OpObjI::setQuadExpr(const Expression::OpQuadExpr& expr)
{
	if (!locked_)
		oqexpr_ = expr.getQuadTerm();
}

void Objective::OpObjI::setSense(OpObjSense sense)
{
	if (!locked_)
		osense_ = sense;
}

void Objective::OpObjI::setName(OpStr name)
{
	oname_ = name;
}

const Expression::OpLinExpr& Objective::OpObjI::getLinExpr() const
{
	return olexpr_;
}

const Expression::OpQuadExpr& Objective::OpObjI::getQuadExpr() const
{
	return oqexpr_;
}

Objective::OpObjSense Objective::OpObjI::getSense() const
{
	return osense_;
}

OpStr Objective::OpObjI::getName() const
{
	return oname_;
}

OpBool Objective::OpObjI::isConstant() const
{
	return !(olexpr_.getSize() + oqexpr_.getSize());
}

OpBool Objective::OpObjI::isQuad() const
{
	return OpBool(oqexpr_.getSize());
}

OpULInt Objective::OpObjI::getMemoryUsage() const
{
	return sizeof(*this);
}

Objective::OpObjI::OpObjI(OpEnvI* env)
	: OpImplBase('O', env), 
	osense_(OpObjSense::Min), 
	olexpr_(0.0), 
	oqexpr_(0.0), 
	oname_("obj_" + std::to_string(idx_))
{

}

Objective::OpObjI::OpObjI(OpEnvI* env, OpObjSense sense)
	: OpImplBase('O', env),
	osense_(sense),
	olexpr_(0.0),
	oqexpr_(0.0),
	oname_("obj_" + std::to_string(idx_))
{

}

Objective::OpObjI::OpObjI(OpEnvI* env, OpObjSense sense, const Expression::OpLinExpr& lexpr)
	: OpImplBase('O', env),
	osense_(sense),
	olexpr_(lexpr),
	oqexpr_(0.0),
	oname_("obj_" + std::to_string(idx_))
{

}

Objective::OpObjI::OpObjI(OpEnvI* env, OpObjSense sense, const Expression::OpLinExpr& lexpr, const Expression::OpQuadExpr& qexpr)
	: OpImplBase('O', env),
	osense_(sense),
	olexpr_(lexpr),
	oqexpr_(qexpr),
	oname_("obj_" + std::to_string(idx_))
{

}

Objective::OpObjI::OpObjI(OpEnvI* env, OpObjSense sense, const Expression::OpLinExpr& lexpr, const Expression::OpQuadExpr& qexpr, OpStr name)
	: OpImplBase('O', env),
	osense_(sense),
	olexpr_(lexpr),
	oqexpr_(qexpr),
	oname_(name)
{

}

Objective::OpObjI::~OpObjI()
{

}

/* OPUA::Objective::OpObj */

void Objective::OpObj::setLinExpr(const Expression::OpLinExpr& expr)
{
	static_cast<OpObjI*>(impl_)->setLinExpr(expr);
}

void Objective::OpObj::setQuadExpr(const Expression::OpQuadExpr& expr)
{
	static_cast<OpObjI*>(impl_)->setQuadExpr(expr);
}

void Objective::OpObj::setSense(OpObjSense sense)
{
	static_cast<OpObjI*>(impl_)->setSense(sense);
}

void Objective::OpObj::setName(OpStr name)
{
	static_cast<OpObjI*>(impl_)->setName(name);
}

const Expression::OpLinExpr& Objective::OpObj::getLinExpr() const
{
	return static_cast<OpObjI*>(impl_)->getLinExpr();
}

const Expression::OpQuadExpr& Objective::OpObj::getQuadExpr() const
{
	return static_cast<OpObjI*>(impl_)->getQuadExpr();
}

Objective::OpObjSense Objective::OpObj::getSense() const
{
	return static_cast<OpObjI*>(impl_)->getSense();
}

OpStr Objective::OpObj::getName() const
{
	return static_cast<OpObjI*>(impl_)->getName();
}

OpBool Objective::OpObj::isConstant() const
{
	return static_cast<OpObjI*>(impl_)->isConstant();
}

OpBool Objective::OpObj::isQuad() const
{
	return static_cast<OpObjI*>(impl_)->isQuad();
}

Objective::OpObjI* Objective::OpObj::getImpl() const
{
	return static_cast<OpObjI*>(impl_);
}

OpBool Objective::OpObj::operator==(const OpObj& obj) const
{
	return impl_ == obj.impl_;
}

OpBool Objective::OpObj::operator!=(const OpObj& obj) const
{
	return impl_ != obj.impl_;
}

Objective::OpObj::OpObj()
{

}

Objective::OpObj::OpObj(OpObjI* impl)
{
	impl_ = impl;
}

Objective::OpObj::OpObj(OpEnv env)
{
	impl_ = new OpObjI(env.getImpl());
}

Objective::OpObj::OpObj(OpEnv env, OpObjSense sense)
{
	impl_ = new OpObjI(env.getImpl(), sense);
}

Objective::OpObj::OpObj(OpEnv env, OpObjSense sense, const Expression::OpLinExpr& lexpr)
{
	impl_ = new OpObjI(env.getImpl(), sense, lexpr);
}

Objective::OpObj::OpObj(OpEnv env, OpObjSense sense, const Expression::OpLinExpr& lexpr, const Expression::OpQuadExpr& qexpr)
{
	impl_ = new OpObjI(env.getImpl(), sense, lexpr, qexpr);
}

Objective::OpObj::OpObj(OpEnv env, OpObjSense sense, const Expression::OpLinExpr& lexpr, const Expression::OpQuadExpr& qexpr, OpStr name)
{
	impl_ = new OpObjI(env.getImpl(), sense, lexpr, qexpr, name);
}

OPUA::Objective::OpObj::~OpObj()
{

}
