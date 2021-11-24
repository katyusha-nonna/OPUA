#include "OPUA.Variable.h"

using namespace OPUA;

/* OPUA::Variable::OpVarI */

class Variable::OpVarI
	: public OpImplBase
{
protected:
	OpVarType  vtype_; // ��������
	OpFloat vlb_; // �����½�
	OpFloat vub_; // �����Ͻ�
	OpStr vname_; // ��������

	friend class Variable::OpVar;
protected:
	void setLb(OpFloat lb); // ���ñ����½�
	void setUb(OpFloat ub); // ���ñ����Ͻ�
	void setBound(OpFloat lb, OpFloat ub); // ���ñ������½�
	void setType(OpVarType type); // ����(�ı�)����������
	void setName(OpStr name); // ���ñ�������

	OpFloat getLb() const; // ��ȡ�����Ͻ�
	OpFloat getUb() const; // ��ȡ�����½�
	OpVarType getType() const; // ��ȡ��������
	OpStr getName() const; // ��ȡ��������
protected:
	OpVarI(OpEnvI* env);
	OpVarI(OpEnvI* env, OpVarType type);
	OpVarI(OpEnvI* env, OpVarType type, OpFloat lb, OpFloat ub);
	OpVarI(OpEnvI* env, OpVarType type, OpFloat lb, OpFloat ub, OpStr name);	
public:
	virtual ~OpVarI();
};

void Variable::OpVarI::setLb(OpFloat lb)
{
	if (!locked_)
		vlb_ = lb;
}

void Variable::OpVarI::setUb(OpFloat ub)
{
	if (!locked_)
		vub_ = ub;
}

void Variable::OpVarI::setBound(OpFloat lb, OpFloat ub)
{
	if (!locked_)
		vlb_ = lb, vub_ = ub;
}

void Variable::OpVarI::setType(OpVarType type)
{
	if (!locked_)
		vtype_ = type;
}

void Variable::OpVarI::setName(OpStr name)
{
	vname_ = name;
}

OpFloat Variable::OpVarI::getLb() const
{
	return vlb_;
}

OpFloat Variable::OpVarI::getUb() const
{
	return vub_;
}

Variable::OpVarType Variable::OpVarI::getType() const
{
	return vtype_;
}

OpStr Variable::OpVarI::getName() const
{
	return vname_;
}

Variable::OpVarI::OpVarI(OpEnvI* env)
	: OpImplBase('V', env), 
	vtype_(OpVarType::Con), 
	vlb_(-Constant::Infinity), 
	vub_(Constant::Infinity), 
	vname_("x_" + std::to_string(idx_))
{

}

Variable::OpVarI::OpVarI(OpEnvI* env, OpVarType type)
	: OpImplBase('V', env), 
	vtype_(type), 
	vlb_(-Constant::Infinity), 
	vub_(Constant::Infinity), 
	vname_("x_" + std::to_string(idx_))
{

}

Variable::OpVarI::OpVarI(OpEnvI* env, OpVarType type, OpFloat lb, OpFloat ub)
	: OpImplBase('V', env), 
	vtype_(type), 
	vlb_(lb),
	vub_(ub), 
	vname_("x_" + std::to_string(idx_))
{

}

Variable::OpVarI::OpVarI(OpEnvI* env, OpVarType type, OpFloat lb, OpFloat ub, OpStr name)
	: OpImplBase('V', env), 
	vtype_(type), 
	vlb_(lb), 
	vub_(ub), 
	vname_(name)
{

}

Variable::OpVarI::~OpVarI()
{
	
}

/* OPUA::Variable::OpVar */

void Variable::OpVar::setLb(OpFloat lb)
{
	static_cast<OpVarI*>(impl_)->setLb(lb);
}

void Variable::OpVar::setUb(OpFloat ub)
{
	static_cast<OpVarI*>(impl_)->setUb(ub);
}

void Variable::OpVar::setBound(OpFloat lb, OpFloat ub)
{
	static_cast<OpVarI*>(impl_)->setBound(lb, ub);
}

void Variable::OpVar::setType(OpVarType type)
{
	static_cast<OpVarI*>(impl_)->setType(type);
}

void Variable::OpVar::setName(OpStr name)
{
	static_cast<OpVarI*>(impl_)->setName(name);
}

OpFloat Variable::OpVar::getLb() const
{
	return static_cast<OpVarI*>(impl_)->getLb();
}

OpFloat Variable::OpVar::getUb() const
{
	return static_cast<OpVarI*>(impl_)->getUb();
}

Variable::OpVarType Variable::OpVar::getType() const
{
	return static_cast<OpVarI*>(impl_)->getType();
}

OpStr Variable::OpVar::getName() const
{
	return static_cast<OpVarI*>(impl_)->getName();
}

Variable::OpVarI* Variable::OpVar::getImpl() const
{
	return static_cast<OpVarI*>(impl_);
}

OpBool Variable::OpVar::operator==(const OpVar& var) const
{
	return impl_ == var.getImpl();
}

OpBool Variable::OpVar::operator!=(const OpVar& var) const
{
	return impl_ != var.getImpl();
}

OPUA::Variable::OpVar::OpVar()
{

}

OPUA::Variable::OpVar::OpVar(OpVarI* impl)
{
	impl_ = impl;
}

OPUA::Variable::OpVar::OpVar(OpEnv env)
{
	impl_ = new OpVarI(env.getImpl());
}

OPUA::Variable::OpVar::OpVar(OpEnv env, OpVarType type)
{
	impl_ = new OpVarI(env.getImpl(), type);
}

OPUA::Variable::OpVar::OpVar(OpEnv env, OpVarType type, OpFloat lb, OpFloat ub)
{
	impl_ = new OpVarI(env.getImpl(), type, lb, ub);
}

OPUA::Variable::OpVar::OpVar(OpEnv env, OpVarType type, OpFloat lb, OpFloat ub, OpStr name)
{
	impl_ = new OpVarI(env.getImpl(), type, lb, ub, name);
}

OPUA::Variable::OpVar::~OpVar()
{

}

OpStr OPUA::Variable::VarType2Str(OpVarType type)
{
	OpStr str(" ? ");
	if (type == OpVarType::Bool)
		str = " Bool ";
	else if (type == OpVarType::Int)
		str = " Int ";
	else if (type == OpVarType::Con)
		str = " Con ";
	else if (type == OpVarType::Sem)
		str = " Sem ";
	return str;
}

std::ostream& OPUA::Variable::operator<<(std::ostream& stream, OpVar var)
{
	stream << var.getName()
		<< "(" << VarType2Str(var.getType()) << ", "
		<< "[" << var.getLb() << ", " << var.getUb() << "])";
	return stream;
}
