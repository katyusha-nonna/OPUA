#include "OPUA.Variable.h"

using namespace OPUA;

/* OPUA::Variable::OpVarI */

class Variable::OpVarI
	: public OpImplBase
{
protected:
	OpVarType  vtype_; // 变量类型
	OpFloat vlb_; // 变量下界
	OpFloat vub_; // 变量上界
	OpStr vname_; // 变量名称

	friend class Variable::OpVar;
protected:
	void setLb(OpFloat lb); // 设置变量下界
	void setUb(OpFloat ub); // 设置变量上界
	void setBound(OpFloat lb, OpFloat ub); // 设置变量上下界
	void setType(OpVarType type); // 设置(改变)变量的类型
	void setName(OpStr name); // 设置变量名称

	OpFloat getLb() const; // 获取变量上界
	OpFloat getUb() const; // 获取变量下界
	OpVarType getType() const; // 获取变量类型
	OpStr getName() const; // 获取变量名称
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
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

OpULInt Variable::OpVarI::getMemoryUsage() const
{
	return sizeof(*this);
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
	return impl_ == var.impl_;
}

OpBool Variable::OpVar::operator!=(const OpVar& var) const
{
	return impl_ != var.impl_;
}

Variable::OpVar::OpVar()
{

}

Variable::OpVar::OpVar(OpVarI* impl)
{
	impl_ = impl;
}

Variable::OpVar::OpVar(OpEnv env)
{
	impl_ = new OpVarI(env.getImpl());
}

Variable::OpVar::OpVar(OpEnv env, OpVarType type)
{
	impl_ = new OpVarI(env.getImpl(), type);
}

Variable::OpVar::OpVar(OpEnv env, OpVarType type, OpFloat lb, OpFloat ub)
{
	impl_ = new OpVarI(env.getImpl(), type, lb, ub);
}

Variable::OpVar::OpVar(OpEnv env, OpVarType type, OpFloat lb, OpFloat ub, OpStr name)
{
	impl_ = new OpVarI(env.getImpl(), type, lb, ub, name);
}

Variable::OpVar::~OpVar()
{

}

/* OPUA::Variable::OpPSDVarI */

class Variable::OpPSDVarI
	: public OpImplBase
{
protected:
	OpULInt vdim_; // PSD变量维度
	OpStr vname_; // PSD变量名称

	friend class Variable::OpPSDVar;
protected:
	void setDim(OpULInt dim); // 设置变量维度
	void setName(OpStr name); // 设置变量名称

	OpULInt getDim() const; // 获取变量维度
	OpStr getName() const; // 获取变量名称
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
protected:
	OpPSDVarI(OpEnvI* env);
	OpPSDVarI(OpEnvI* env, OpULInt dim);
	OpPSDVarI(OpEnvI* env, OpULInt dim, OpStr name);
public:
	virtual ~OpPSDVarI();
};

void Variable::OpPSDVarI::setDim(OpULInt dim)
{
	if (!locked_)
		vdim_ = dim;
}

void Variable::OpPSDVarI::setName(OpStr name)
{
	vname_ = name;
}

OpULInt Variable::OpPSDVarI::getDim() const
{
	return vdim_;
}

OpStr Variable::OpPSDVarI::getName() const
{
	return vname_;
}

OpULInt Variable::OpPSDVarI::getMemoryUsage() const
{
	return sizeof(*this);
}

Variable::OpPSDVarI::OpPSDVarI(OpEnvI* env)
	: OpImplBase('V', env),
	vdim_(0),
	vname_("psd_" + std::to_string(idx_))
{

}

Variable::OpPSDVarI::OpPSDVarI(OpEnvI* env, OpULInt dim)
	: OpImplBase('V', env),
	vdim_(dim),
	vname_("psd_" + std::to_string(idx_))
{

}

Variable::OpPSDVarI::OpPSDVarI(OpEnvI* env, OpULInt dim, OpStr name)
	: OpImplBase('V', env),
	vdim_(dim),
	vname_(name)
{

}

Variable::OpPSDVarI::~OpPSDVarI()
{

}

/* OPUA::Variable::OpPSDVarI */

void Variable::OpPSDVar::setDim(OpULInt dim)
{
	static_cast<OpPSDVarI*>(impl_)->setDim(dim);
}

void Variable::OpPSDVar::setName(OpStr name)
{
	static_cast<OpPSDVarI*>(impl_)->setName(name);
}

OpULInt Variable::OpPSDVar::getDim() const
{
	return static_cast<OpPSDVarI*>(impl_)->getDim();
}

OpStr Variable::OpPSDVar::getName() const
{
	return static_cast<OpPSDVarI*>(impl_)->getName();
}

Variable::OpPSDVarI* Variable::OpPSDVar::getImpl() const
{
	return static_cast<OpPSDVarI*>(impl_);
}

OpBool Variable::OpPSDVar::operator==(const OpPSDVar& var) const
{
	return impl_ == var.impl_;
}

OpBool Variable::OpPSDVar::operator!=(const OpPSDVar& var) const
{
	return impl_ != var.impl_;
}

Variable::OpPSDVar::OpPSDVar()
{

}

Variable::OpPSDVar::OpPSDVar(OpPSDVarI* impl)
{
	impl_ = impl;
}

Variable::OpPSDVar::OpPSDVar(OpEnv env)
{
	impl_ = new OpPSDVarI(env.getImpl());
}

Variable::OpPSDVar::OpPSDVar(OpEnv env, OpULInt dim)
{
	impl_ = new OpPSDVarI(env.getImpl(), dim);
}

Variable::OpPSDVar::OpPSDVar(OpEnv env, OpULInt dim, OpStr name)
{
	impl_ = new OpPSDVarI(env.getImpl(), dim, name);
}

OpStr Variable::VarType2Str(OpVarType type)
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

std::ostream& Variable::operator<<(std::ostream& stream, OpVar var)
{
	stream << var.getName()
		<< "(" << VarType2Str(var.getType()) << ", "
		<< "[" << var.getLb() << ", " << var.getUb() << "])";
	return stream;
}