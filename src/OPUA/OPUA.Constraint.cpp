#include "OPUA.Constraint.h"
#include <iostream>

using namespace OPUA;

/* OPUA::Constraint::OpLinConI */

class Constraint::OpLinConI
	: public OpImplBase
{
protected:
	OpFloat clb_;
	OpFloat cub_;
	Expression::OpLinExpr cexpr_;
	OpStr cname_;

	friend class OpLinCon;
protected:
	void setName(OpStr name); // 设置约束名称
	void setExpr(const Expression::OpLinExpr& expr); // 设置约束表达式
	void setLb(OpFloat lb); // 设置约束下限
	void setUb(OpFloat ub); // 设置约束上限
	OpInt standardize(); // 约束标准化(-2-无效约束无法标准化 / -1-Range约束无法标准化 / 0-标准约束(等式或大于等于约束)无需标准化 / 1-已标准化)
	OpStr getName() const; // 获取约束名称
	const Expression::OpLinExpr& getExpr() const; // 获取约束表达式
	OpFloat getLb() const; // 获取约束下限
	OpFloat getUb() const; // 获取约束上限
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
protected:
	OpLinConI(OpEnvI* env);
	OpLinConI(OpEnvI* env, OpFloat lb, const Expression::OpLinExpr& expr, OpFloat ub);
	OpLinConI(OpEnvI* env, OpFloat lb, const Expression::OpLinExpr& expr, OpFloat ub, OpStr name);
public:
	virtual ~OpLinConI();
};

void Constraint::OpLinConI::setName(OpStr name)
{
	cname_ = name;
}

void Constraint::OpLinConI::setExpr(const Expression::OpLinExpr& expr)
{
	if (!locked_)
		cexpr_ = expr;
}

void Constraint::OpLinConI::setLb(OpFloat lb)
{
	if (!locked_)
		clb_ = lb;
}

void Constraint::OpLinConI::setUb(OpFloat ub)
{
	if (!locked_)
		cub_ = ub;
}

OpInt Constraint::OpLinConI::standardize()
{
	OpBool fl(false), fu(false), fr(false), feq(false), fgt(false), flt(false), fse(false);
	// 首先修正lb和ub，将nan或inf统一修正为inf
	if (Constant::IsNaN(clb_) || Constant::IsInfinity(clb_))
	{
		clb_ = -Constant::Infinity;
		fl = true;
	}
	if (Constant::IsNaN(cub_) || Constant::IsInfinity(cub_))
	{
		cub_ = Constant::Infinity;
		fu = true;
	}
	feq = Constant::IsEqual(clb_, cub_);
	fgt = fl ? false : true;
	flt = fu ? false : true;
	// 然后修正expr的常数项，将nan或inf统一修正为0
	if (Constant::IsNaN(cexpr_.getConstant()) || Constant::IsInfinity(cexpr_.getConstant()) || Constant::IsEqual(cexpr_.getConstant(), 0))
	{
		cexpr_.setLinTerm(0.0);
		fse = true;
	}
	// 判断是否为无效约束(仅消去expr的常数项)
	if (fl && fu)
	{
		cexpr_.setLinTerm(0.0);
		return -2;
	}
	// 然后判断是否为Range约束(仅消去expr的常数项)
	if (!fl && !fu && !feq)
		fr = true;
	if (fr)
	{
		if (!fse)
		{
			clb_ -= cexpr_.getConstant();
			cub_ -= cexpr_.getConstant();
			cexpr_.setLinTerm(0.0);
		}
		return -1;
	}
	// 然后判断是否为等式约束(仅消去expr的常数项)
	if (feq)
	{
		if (!fse)
		{
			clb_ -= cexpr_.getConstant();
			cub_ -= cexpr_.getConstant();
			cexpr_.setLinTerm(0.0);
		}		
		return 0;
	}
	// 然后判断是否为大于等于约束(仅消去expr的常数项)
	if (fgt)
	{
		if (!fse)
		{
			clb_-= cexpr_.getConstant();
			cexpr_.setLinTerm(0.0);
		}
		return 0;
	}
	// 然后判断是否为小于等于约束(消去expr的常数项并转换为大于等于约束)
	if (flt)
	{
		if (!fse)
		{
			cub_ -= cexpr_.getConstant();
			cexpr_.setLinTerm(0.0);
		}
		clb_ = -cub_;
		cub_ = Constant::Infinity;
		cexpr_.piecewiseInv();
		return 1;
	}
	return -2;
}

OpStr Constraint::OpLinConI::getName() const
{
	return cname_;
}

const Expression::OpLinExpr& Constraint::OpLinConI::getExpr() const
{
	return cexpr_;
}

OpFloat Constraint::OpLinConI::getLb() const
{
	return clb_;
}

OpFloat Constraint::OpLinConI::getUb() const
{
	return cub_;
}

OpULInt Constraint::OpLinConI::getMemoryUsage() const
{
	return sizeof(*this);
}

Constraint::OpLinConI::OpLinConI(OpEnvI* env)
	: OpImplBase('C', env),
	clb_(-Constant::Infinity),
	cub_(Constant::Infinity),
	cexpr_(0.0), 
	cname_("lc_" + std::to_string(idx_))
{

}

Constraint::OpLinConI::OpLinConI(OpEnvI* env, OpFloat lb, const Expression::OpLinExpr& expr, OpFloat ub)
	: OpImplBase('C', env),
	clb_(lb),
	cub_(ub),
	cexpr_(expr),
	cname_("lc_" + std::to_string(idx_))
{

}

Constraint::OpLinConI::OpLinConI(OpEnvI* env, OpFloat lb, const Expression::OpLinExpr& expr, OpFloat ub, OpStr name)
	: OpImplBase('C', env),
	clb_(lb),
	cub_(ub),
	cexpr_(expr),
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
	OpFloat clb_;
	OpFloat cub_;
	Expression::OpQuadExpr cexpr_;
	OpStr cname_;

	friend class OpQuadCon;
protected:
	void setName(OpStr name); // 设置约束名称
	void setExpr(const Expression::OpQuadExpr& expr); // 设置约束表达式
	void setLb(OpFloat lb); // 设置约束下限
	void setUb(OpFloat ub); // 设置约束上限
	OpStr getName() const; // 获取约束名称
	const Expression::OpQuadExpr& getExpr() const; // 获取约束表达式
	OpFloat getLb() const; // 获取约束下限
	OpFloat getUb() const; // 获取约束上限
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
protected:
	OpQuadConI(OpEnvI* env);
	OpQuadConI(OpEnvI* env, OpFloat lb, const Expression::OpQuadExpr& expr, OpFloat ub);
	OpQuadConI(OpEnvI* env, OpFloat lb, const Expression::OpQuadExpr& expr, OpFloat ub, OpStr name);
public:
	virtual ~OpQuadConI();
};

void Constraint::OpQuadConI::setName(OpStr name)
{
	cname_ = name;
}

void Constraint::OpQuadConI::setExpr(const Expression::OpQuadExpr& expr)
{
	if (!locked_)
		cexpr_ = expr;
}

void Constraint::OpQuadConI::setLb(OpFloat lb)
{
	if (!locked_)
		clb_ = lb;
}

void Constraint::OpQuadConI::setUb(OpFloat ub)
{
	if (!locked_)
		cub_ = ub;
}

OpStr Constraint::OpQuadConI::getName() const
{
	return cname_;
}

const Expression::OpQuadExpr& Constraint::OpQuadConI::getExpr() const
{
	return cexpr_;
}

OpFloat Constraint::OpQuadConI::getLb() const
{
	return clb_;
}

OpFloat Constraint::OpQuadConI::getUb() const
{
	return cub_;
}

OpULInt Constraint::OpQuadConI::getMemoryUsage() const
{
	return sizeof(*this);
}

Constraint::OpQuadConI::OpQuadConI(OpEnvI* env)
	: OpImplBase('C', env),
	clb_(-Constant::Infinity),
	cub_(Constant::Infinity),
	cexpr_(0.0),
	cname_("qc_" + std::to_string(idx_))
{

}

Constraint::OpQuadConI::OpQuadConI(OpEnvI* env, OpFloat lb, const Expression::OpQuadExpr& expr, OpFloat ub)
	: OpImplBase('C', env),
	clb_(lb),
	cub_(ub),
	cexpr_(expr),
	cname_("qc_" + std::to_string(idx_))
{

}

Constraint::OpQuadConI::OpQuadConI(OpEnvI* env, OpFloat lb, const Expression::OpQuadExpr& expr, OpFloat ub, OpStr name)
	: OpImplBase('C', env),
	clb_(lb),
	cub_(ub),
	cexpr_(expr),
	cname_(name)
{

}

Constraint::OpQuadConI::~OpQuadConI()
{

}

/* OPUA::Constraint::OpConicConI */

class Constraint::OpConicConI
	: public OpImplBase
{
protected:
	OpConicSense csense_; // 约束符号
	Expression::OpNLExpr cvars_; // 约束变量表
	OpStr cname_;

	friend class OpConicCon;
protected:
	void setSense(OpConicSense sense); // 设置(改变)锥集合的类型
	void setName(OpStr name); // 设置约束名称
	void addVar(Variable::OpVar var); // 向锥集合变量表中添加一个变量(有序)
	void addVar(Variable::OpVarArr vars); // 向锥集合变量表中添加一组变量(有序)
	void removeVar(Variable::OpVar var);  // 从锥集合变量表中移除一个变量
	void removeVar(Variable::OpVarArr vars);  // 从锥集合变量表中移除一组变量
	void removeVar(OpULInt n = 0); // 从锥集合变量表中移除末尾的多个变量
	OpConicSense getSense() const; // 获取锥集合的类型
	const Expression::OpNLExpr& getConicExpr() const; // 获取表示锥集合变量表的表达式
	OpStr getName() const; // 获取约束名称
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
protected:
	OpConicConI(OpEnvI* env); // 从env构造
	OpConicConI(OpEnvI* env, OpConicSense sense); // 从env构造并指定部分参数
	OpConicConI(OpEnvI* env, OpConicSense sense, const Expression::OpNLExpr& expr); // 从env构造并指定部分参数
	OpConicConI(OpEnvI* env, OpConicSense sense, const Expression::OpNLExpr& expr, OpStr name); // 从env构造并指定部分参数
	OpConicConI(OpEnvI* env, OpConicSense sense, Variable::OpVarArr vars); // 从env构造并指定部分参数
	OpConicConI(OpEnvI* env, OpConicSense sense, Variable::OpVarArr vars, OpStr name); // 从env构造并指定部分参数
public:
	virtual ~OpConicConI();
};

void Constraint::OpConicConI::setSense(OpConicSense sense)
{
	if (!locked_)
		csense_ = sense;
}

void Constraint::OpConicConI::setName(OpStr name)
{
	cname_ = name;
}

void Constraint::OpConicConI::addVar(Variable::OpVar var)
{
	if (!locked_)
		cvars_.addVar(var);
}

void Constraint::OpConicConI::addVar(Variable::OpVarArr vars)
{
	if (!locked_)
		cvars_.addVar(vars);
}

void Constraint::OpConicConI::removeVar(Variable::OpVar var)
{
	if (!locked_)
		cvars_.removeVar(var);
}

void Constraint::OpConicConI::removeVar(Variable::OpVarArr vars)
{
	if (!locked_)
		cvars_.removeVar(vars);
}

void Constraint::OpConicConI::removeVar(OpULInt n)
{
	if (!locked_)
		cvars_.removeVar(n);
}

Constraint::OpConicSense Constraint::OpConicConI::getSense() const
{
	return csense_;
}

const Expression::OpNLExpr& Constraint::OpConicConI::getConicExpr() const
{
	return cvars_;
}

OpStr Constraint::OpConicConI::getName() const
{
	return cname_;
}

OpULInt Constraint::OpConicConI::getMemoryUsage() const
{
	return sizeof(*this);
}

Constraint::OpConicConI::OpConicConI(OpEnvI* env)
	: OpImplBase('C', env),
	csense_(OpConicSense::Unknow),
	cvars_(Expression::OpNLFunc::Unknow),
	cname_("conic_" + std::to_string(idx_))
{

}

Constraint::OpConicConI::OpConicConI(OpEnvI* env, OpConicSense sense)
	: OpImplBase('C', env),
	csense_(sense),
	cvars_(Expression::OpNLFunc::Unknow),
	cname_("conic_" + std::to_string(idx_))
{

}

Constraint::OpConicConI::OpConicConI(OpEnvI* env, OpConicSense sense, const Expression::OpNLExpr& expr)
	: OpImplBase('C', env),
	csense_(sense),
	cvars_(expr),
	cname_("conic_" + std::to_string(idx_))
{

}

Constraint::OpConicConI::OpConicConI(OpEnvI* env, OpConicSense sense, const Expression::OpNLExpr& expr, OpStr name)
	: OpImplBase('C', env),
	csense_(sense),
	cvars_(expr),
	cname_(name)
{

}

Constraint::OpConicConI::OpConicConI(OpEnvI* env, OpConicSense sense, Variable::OpVarArr vars)
	: OpImplBase('C', env),
	csense_(sense),
	cvars_(Expression::OpNLFunc::Unknow, vars),
	cname_("conic_" + std::to_string(idx_))
{

}

Constraint::OpConicConI::OpConicConI(OpEnvI* env, OpConicSense sense, Variable::OpVarArr vars, OpStr name)
	: OpImplBase('C', env),
	csense_(sense),
	cvars_(Expression::OpNLFunc::Unknow, vars),
	cname_(name)
{

}

Constraint::OpConicConI::~OpConicConI()
{

}

/* OPUA::Constraint::OpPSDConI */

class Constraint::OpPSDConI
	: public OpImplBase
{
protected:
	OpFloat clb_;
	OpFloat cub_;
	Expression::OpPSDExpr cexpr_;
	OpStr cname_;

	friend class OpPSDCon;
protected:
	void setName(OpStr name); // 设置约束名称
	void setExpr(const Expression::OpPSDExpr& expr); // 设置约束表达式
	void setLb(OpFloat lb); // 设置约束下限
	void setUb(OpFloat ub); // 设置约束上限
	OpStr getName() const; // 获取约束名称
	const Expression::OpPSDExpr& getExpr() const; // 获取约束表达式
	OpFloat getLb() const; // 获取约束下限
	OpFloat getUb() const; // 获取约束上限
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
protected:
	OpPSDConI(OpEnvI* env); // 从env构造
	OpPSDConI(OpEnvI* env, OpFloat lb, const Expression::OpPSDExpr& expr, OpFloat ub); // 从env构造并指定部分参数
	OpPSDConI(OpEnvI* env, OpFloat lb, const Expression::OpPSDExpr& expr, OpFloat ub, OpStr name); // 从env构造并指定部分参数
public:
	virtual ~OpPSDConI();
};

void Constraint::OpPSDConI::setName(OpStr name)
{
	cname_ = name;
}

void Constraint::OpPSDConI::setExpr(const Expression::OpPSDExpr& expr)
{
	if (!locked_)
		cexpr_ = expr;
}

void Constraint::OpPSDConI::setLb(OpFloat lb)
{
	if (!locked_)
		clb_ = lb;
}

void Constraint::OpPSDConI::setUb(OpFloat ub)
{
	if (!locked_)
		cub_ = ub;
}

OpStr Constraint::OpPSDConI::getName() const
{
	return cname_;
}

const Expression::OpPSDExpr& Constraint::OpPSDConI::getExpr() const
{
	return cexpr_;
}

OpFloat Constraint::OpPSDConI::getLb() const
{
	return clb_;
}

OpFloat Constraint::OpPSDConI::getUb() const
{
	return cub_;
}

OpULInt Constraint::OpPSDConI::getMemoryUsage() const
{
	return sizeof(*this);
}

Constraint::OpPSDConI::OpPSDConI(OpEnvI* env)
	: OpImplBase('C', env),
	clb_(-Constant::Infinity),
	cub_(Constant::Infinity),
	cexpr_(0.0),
	cname_("psdc_" + std::to_string(idx_))
{

}

Constraint::OpPSDConI::OpPSDConI(OpEnvI* env, OpFloat lb, const Expression::OpPSDExpr& expr, OpFloat ub)
	: OpImplBase('C', env),
	clb_(lb),
	cub_(ub),
	cexpr_(expr),
	cname_("psdc_" + std::to_string(idx_))
{

}

Constraint::OpPSDConI::OpPSDConI(OpEnvI* env, OpFloat lb, const Expression::OpPSDExpr& expr, OpFloat ub, OpStr name)
	: OpImplBase('C', env),
	clb_(lb),
	cub_(ub),
	cexpr_(expr),
	cname_(name)
{

}

Constraint::OpPSDConI::~OpPSDConI()
{

}

/* OPUA::Constraint::OpSOSConI */

class Constraint::OpSOSConI
	: public OpImplBase
{
protected:
	OpConSense csense_; // 约束符号
	Expression::OpLinExpr cvars_; // SOS变量表(变量*权重，以线性表达式存储)
	OpStr cname_; // 约束名称

	friend class OpSOSCon;
protected:
	void setSense(OpConSense sense); // 设置(改变)约束的符号
	void setName(OpStr name); // 设置约束名称
	void addVar(Variable::OpVar var, OpFloat weight); // 向SOS中添加一个变量(重复添加则追加权重)
	void addVar(Variable::OpVarArr vars, Container::OpFloatArr weights); // 向SOS中添加一组变量(重复添加则追加权重)
	OpConSense getSense() const; // 获取约束的符号
	OpStr getName() const; // 获取约束名称
	const Expression::OpLinExpr& getSOSExpr() const; // 获取表示SOS集合的表达式(变量*权重)
	void removeVar(Variable::OpVar var);  // 从SOS中移除一个变量
	void removeVar(Variable::OpVarArr vars);  // 从SOS中移除一组变量
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
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

void Constraint::OpSOSConI::addVar(Variable::OpVarArr vars, Container::OpFloatArr weights)
{
	if (!locked_)
		for (OpULInt i = 0; i < vars.getSize(); i++)
			cvars_.addLinTerm(vars[i], weights[i]);
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

void Constraint::OpSOSConI::removeVar(Variable::OpVarArr vars)
{
	if (!locked_)
		for (OpULInt i = 0; i < vars.getSize(); i++)
			cvars_.removeVar(vars[i]);
}

OpULInt Constraint::OpSOSConI::getMemoryUsage() const
{
	return sizeof(*this);
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
	Variable::OpVar clvar_; // 约束左操作数
	Expression::OpNLExpr crexpr_; // 约束右操作表达式

	OpStr cname_;

	friend class OpNLCon;
protected:
	void setName(OpStr name); // 设置约束名称
	void setVar(Variable::OpVar var); // 设置左操作变量
	void setExpr(const Expression::OpNLExpr& expr); // 设置右操作表达式
	Variable::OpVar getVar() const; // 获取右操作变量
	const Expression::OpNLExpr& getExpr() const; // 获取左操作表达式
	Expression::OpNLFunc getFunction() const; // 获取约束的操作函数
	OpStr getName() const; // 获取约束名称
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
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

OpULInt Constraint::OpNLConI::getMemoryUsage() const
{
	return sizeof(*this);
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

/* OPUA::Constraint::OpCdtConI */

class Constraint::OpCdtConI
	: public OpImplBase
{
protected:
	OpBool cic_; // 是否为指示器约束
	Variable::OpVar cind_; // 指示器变量
	OpLinCon cif_; // IF约束
	OpLinCon cthen_; // Then约束
	OpStr cname_;

	friend class OpCdtCon;
protected:
	void setName(OpStr name); // 设置约束名称
	void setVar(Variable::OpVar var); // 设置指示器变量
	void setCon(OpLinCon con, OpBool ifCon); // 设置约束
	Variable::OpVar getVar() const; // 获取指示器变量
	OpLinCon getCon(OpBool ifCon) const; // 获取IF/Then约束
	OpBool isIndicator() const; // 是否为指示器约束
	OpStr getName() const; // 获取约束名称
	void lock(); // 锁定条件约束
	void unlock(); // 解锁条件约束
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
protected:
	OpCdtConI(OpEnvI* env);
	OpCdtConI(OpEnvI* env, Variable::OpVar indicator, OpLinCon thenCon);
	OpCdtConI(OpEnvI* env, Variable::OpVar indicator, OpLinCon thenCon, OpStr name);
	OpCdtConI(OpEnvI* env, OpLinCon ifCon, OpLinCon thenCon);
	OpCdtConI(OpEnvI* env, OpLinCon ifCon, OpLinCon thenCon, OpStr name);
public:
	virtual ~OpCdtConI();
};

void Constraint::OpCdtConI::setName(OpStr name)
{
	cname_ = name;
}

void Constraint::OpCdtConI::setVar(Variable::OpVar var)
{
	if (!locked_)
	{
		cind_ = var;
		cif_ = OpLinCon();
		cic_ = true;
	}
}

void Constraint::OpCdtConI::setCon(OpLinCon con, OpBool ifCon)
{
	if (!locked_)
	{
		if (ifCon)
		{
			cif_ = con;
			cind_ = Variable::OpVar();
			cic_ = false;
		}
		else
		{
			cthen_ = con;
		}
	}
}

Variable::OpVar Constraint::OpCdtConI::getVar() const
{
	return cind_;
}

Constraint::OpLinCon Constraint::OpCdtConI::getCon(OpBool ifCon) const
{
	return ifCon ? cif_ : cthen_;
}

OpBool Constraint::OpCdtConI::isIndicator() const
{
	return cic_;
}

OpStr Constraint::OpCdtConI::getName() const
{
	return cname_;
}

void Constraint::OpCdtConI::lock()
{
	OpImplBase::lock();
	if (!isIndicator())
		cif_.lock();
	cthen_.lock();
}

void Constraint::OpCdtConI::unlock()
{
	cthen_.unlock();
	if (!isIndicator())
		cif_.unlock();
	OpImplBase::unlock();
}

OpULInt Constraint::OpCdtConI::getMemoryUsage() const
{
	return sizeof(*this);
}

Constraint::OpCdtConI::OpCdtConI(OpEnvI* env)
	: OpImplBase('C', env),
	cic_(false), 
	cind_(), 
	cif_(), 
	cthen_(), 
	cname_("cc_" + std::to_string(idx_))
{

}

Constraint::OpCdtConI::OpCdtConI(OpEnvI* env, Variable::OpVar indicator, OpLinCon thenCon)
	: OpImplBase('C', env),
	cic_(true),
	cind_(indicator),
	cif_(),
	cthen_(thenCon),
	cname_("cc_" + std::to_string(idx_))
{

}

Constraint::OpCdtConI::OpCdtConI(OpEnvI* env, Variable::OpVar indicator, OpLinCon thenCon, OpStr name)
	: OpImplBase('C', env),
	cic_(true),
	cind_(indicator),
	cif_(),
	cthen_(thenCon),
	cname_(name)
{

}

Constraint::OpCdtConI::OpCdtConI(OpEnvI* env, OpLinCon ifCon, OpLinCon thenCon)
	: OpImplBase('C', env),
	cic_(false),
	cind_(),
	cif_(ifCon),
	cthen_(thenCon),
	cname_("cc_" + std::to_string(idx_))
{

}

Constraint::OpCdtConI::OpCdtConI(OpEnvI* env, OpLinCon ifCon, OpLinCon thenCon, OpStr name)
	: OpImplBase('C', env),
	cic_(false),
	cind_(),
	cif_(ifCon),
	cthen_(thenCon),
	cname_(name)
{

}

Constraint::OpCdtConI::~OpCdtConI()
{

}

/* OPUA::Constraint::OpLinCon */

void Constraint::OpLinCon::setName(OpStr name)
{
	static_cast<OpLinConI*>(impl_)->setName(name);
}

void Constraint::OpLinCon::setExpr(const Expression::OpLinExpr& expr)
{
	static_cast<OpLinConI*>(impl_)->setExpr(expr);
}

void Constraint::OpLinCon::setLb(OpFloat lb)
{
	static_cast<OpLinConI*>(impl_)->setLb(lb);
}

void Constraint::OpLinCon::setUb(OpFloat ub)
{
	static_cast<OpLinConI*>(impl_)->setUb(ub);
}

OpInt Constraint::OpLinCon::standardize()
{
	return static_cast<OpLinConI*>(impl_)->standardize();
}

OpStr Constraint::OpLinCon::getName() const
{
	return static_cast<OpLinConI*>(impl_)->getName();
}

const Expression::OpLinExpr& Constraint::OpLinCon::getExpr() const
{
	return static_cast<OpLinConI*>(impl_)->getExpr();
}

OpFloat Constraint::OpLinCon::getLb() const
{
	return static_cast<OpLinConI*>(impl_)->getLb();
}

OpFloat Constraint::OpLinCon::getUb() const
{
	return static_cast<OpLinConI*>(impl_)->getUb();
}

Constraint::OpLinConI* Constraint::OpLinCon::getImpl() const
{
	return static_cast<OpLinConI*>(impl_);
}

OpBool Constraint::OpLinCon::operator==(const OpLinCon& con) const
{
	return impl_ == con.impl_;
}

OpBool Constraint::OpLinCon::operator!=(const OpLinCon& con) const
{
	return impl_ != con.impl_;
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

Constraint::OpLinCon::OpLinCon(OpEnv env, OpFloat lb, const Expression::OpLinExpr& expr, OpFloat ub)
{
	impl_ = new OpLinConI(env.getImpl(), lb, expr, ub);
}

Constraint::OpLinCon::OpLinCon(OpEnv env, OpFloat lb, const Expression::OpLinExpr& expr, OpFloat ub, OpStr name)
{
	impl_ = new OpLinConI(env.getImpl(), lb, expr, ub, name);
}

Constraint::OpLinCon::~OpLinCon()
{

}

/* OPUA::Constraint::OpQuadCon */

void Constraint::OpQuadCon::setName(OpStr name)
{
	static_cast<OpQuadConI*>(impl_)->setName(name);
}

void Constraint::OpQuadCon::setExpr(const Expression::OpQuadExpr& expr)
{
	static_cast<OpQuadConI*>(impl_)->setExpr(expr);
}

void Constraint::OpQuadCon::setLb(OpFloat lb)
{
	static_cast<OpQuadConI*>(impl_)->setLb(lb);
}

void Constraint::OpQuadCon::setUb(OpFloat ub)
{
	static_cast<OpQuadConI*>(impl_)->setUb(ub);
}

OpStr Constraint::OpQuadCon::getName() const
{
	return static_cast<OpQuadConI*>(impl_)->getName();
}

const Expression::OpQuadExpr& Constraint::OpQuadCon::getExpr() const
{
	return static_cast<OpQuadConI*>(impl_)->getExpr();
}

OpFloat Constraint::OpQuadCon::getLb() const
{
	return static_cast<OpQuadConI*>(impl_)->getLb();
}

OpFloat Constraint::OpQuadCon::getUb() const
{
	return static_cast<OpQuadConI*>(impl_)->getUb();
}

Constraint::OpQuadConI* Constraint::OpQuadCon::getImpl() const
{
	return static_cast<OpQuadConI*>(impl_);
}

OpBool Constraint::OpQuadCon::operator==(const OpQuadCon& con) const
{
	return impl_ == con.impl_;
}

OpBool Constraint::OpQuadCon::operator!=(const OpQuadCon& con) const
{
	return impl_ != con.impl_;
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

Constraint::OpQuadCon::OpQuadCon(OpEnv env, OpFloat lb, const Expression::OpQuadExpr& expr, OpFloat ub)
{
	impl_ = new OpQuadConI(env.getImpl(), lb, expr, ub);
}

Constraint::OpQuadCon::OpQuadCon(OpEnv env, OpFloat lb, const Expression::OpQuadExpr& expr, OpFloat ub, OpStr name)
{
	impl_ = new OpQuadConI(env.getImpl(), lb, expr, ub, name);
}

Constraint::OpQuadCon::~OpQuadCon()
{

}

/* OPUA::Constraint::OpConicCon */

void Constraint::OpConicCon::setSense(OpConicSense sense)
{
	static_cast<OpConicConI*>(impl_)->setSense(sense);
}

void Constraint::OpConicCon::setName(OpStr name)
{
	static_cast<OpConicConI*>(impl_)->setName(name);
}

void Constraint::OpConicCon::addVar(Variable::OpVar var)
{
	static_cast<OpConicConI*>(impl_)->addVar(var);
}

void Constraint::OpConicCon::addVar(Variable::OpVarArr vars)
{
	static_cast<OpConicConI*>(impl_)->addVar(vars);
}

void Constraint::OpConicCon::removeVar(Variable::OpVar var)
{
	static_cast<OpConicConI*>(impl_)->removeVar(var);
}

void Constraint::OpConicCon::removeVar(Variable::OpVarArr vars)
{
	static_cast<OpConicConI*>(impl_)->removeVar(vars);
}

void Constraint::OpConicCon::removeVar(OpULInt n)
{
	static_cast<OpConicConI*>(impl_)->removeVar(n);
}

Constraint::OpConicSense Constraint::OpConicCon::getSense() const
{
	return static_cast<OpConicConI*>(impl_)->getSense();
}

const Expression::OpNLExpr& Constraint::OpConicCon::getConicExpr() const
{
	return static_cast<OpConicConI*>(impl_)->getConicExpr();
}

OpStr Constraint::OpConicCon::getName() const
{
	return static_cast<OpConicConI*>(impl_)->getName();
}

Constraint::OpConicConI* Constraint::OpConicCon::getImpl() const
{
	return static_cast<OpConicConI*>(impl_);
}

OpBool Constraint::OpConicCon::operator==(const OpConicCon& con) const
{
	return impl_ == con.impl_;
}

OpBool Constraint::OpConicCon::operator!=(const OpConicCon& con) const
{
	return impl_ != con.impl_;
}

Constraint::OpConicCon::OpConicCon()
{

}

Constraint::OpConicCon::OpConicCon(OpConicConI* impl)
{
	impl_ = impl;
}

Constraint::OpConicCon::OpConicCon(OpEnv env)
{
	impl_ = new OpConicConI(env.getImpl());
}

Constraint::OpConicCon::OpConicCon(OpEnv env, OpConicSense sense)
{
	impl_ = new OpConicConI(env.getImpl());
}

Constraint::OpConicCon::OpConicCon(OpEnv env, OpConicSense sense, const Expression::OpNLExpr& expr)
{
	impl_ = new OpConicConI(env.getImpl(), sense, expr);
}

Constraint::OpConicCon::OpConicCon(OpEnv env, OpConicSense sense, const Expression::OpNLExpr& expr, OpStr name)
{
	impl_ = new OpConicConI(env.getImpl(), sense, expr, name);
}

Constraint::OpConicCon::OpConicCon(OpEnv env, OpConicSense sense, Variable::OpVarArr vars)
{
	impl_ = new OpConicConI(env.getImpl(), sense, vars);
}

Constraint::OpConicCon::OpConicCon(OpEnv env, OpConicSense sense, Variable::OpVarArr vars, OpStr name)
{
	impl_ = new OpConicConI(env.getImpl(), sense, vars, name);
}

Constraint::OpConicCon::~OpConicCon()
{

}

/* OPUA::Constraint::OpPSDCon */

void Constraint::OpPSDCon::setName(OpStr name)
{
	static_cast<OpPSDConI*>(impl_)->setName(name);
}

void Constraint::OpPSDCon::setExpr(const Expression::OpPSDExpr& expr)
{
	static_cast<OpPSDConI*>(impl_)->setExpr(expr);
}

void Constraint::OpPSDCon::setLb(OpFloat lb)
{
	static_cast<OpPSDConI*>(impl_)->setLb(lb);
}

void Constraint::OpPSDCon::setUb(OpFloat ub)
{
	static_cast<OpPSDConI*>(impl_)->setUb(ub);
}

OpStr Constraint::OpPSDCon::getName() const
{
	return static_cast<OpPSDConI*>(impl_)->getName();
}

const Expression::OpPSDExpr& Constraint::OpPSDCon::getExpr() const
{
	return static_cast<OpPSDConI*>(impl_)->getExpr();
}

OpFloat Constraint::OpPSDCon::getLb() const
{
	return static_cast<OpPSDConI*>(impl_)->getLb();
}

OpFloat Constraint::OpPSDCon::getUb() const
{
	return static_cast<OpPSDConI*>(impl_)->getUb();
}

Constraint::OpPSDConI* Constraint::OpPSDCon::getImpl() const
{
	return static_cast<OpPSDConI*>(impl_);
}

OpBool Constraint::OpPSDCon::operator==(const OpPSDCon& con) const
{
	return impl_ == con.impl_;
}

OpBool Constraint::OpPSDCon::operator!=(const OpPSDCon& con) const
{
	return impl_ != con.impl_;
}

Constraint::OpPSDCon::OpPSDCon()
{

}

Constraint::OpPSDCon::OpPSDCon(OpPSDConI* impl)
{
	impl_ = impl;
}

Constraint::OpPSDCon::OpPSDCon(OpEnv env)
{
	impl_ = new OpPSDConI(env.getImpl());
}

Constraint::OpPSDCon::OpPSDCon(OpEnv env, OpFloat lb, const Expression::OpPSDExpr& expr, OpFloat ub)
{
	impl_ = new OpPSDConI(env.getImpl(), lb, expr, ub);
}

Constraint::OpPSDCon::OpPSDCon(OpEnv env, OpFloat lb, const Expression::OpPSDExpr& expr, OpFloat ub, OpStr name)
{
	impl_ = new OpPSDConI(env.getImpl(), lb, expr, ub, name);
}

Constraint::OpPSDCon::~OpPSDCon()
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

void Constraint::OpSOSCon::addVar(Variable::OpVarArr vars, Container::OpFloatArr weights)
{
	static_cast<OpSOSConI*>(impl_)->addVar(vars, weights);
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

void Constraint::OpSOSCon::removeVar(Variable::OpVarArr vars)
{
	static_cast<OpSOSConI*>(impl_)->removeVar(vars);
}

Constraint::OpSOSConI* Constraint::OpSOSCon::getImpl() const
{
	return static_cast<OpSOSConI*>(impl_);
}

OpBool Constraint::OpSOSCon::operator==(const OpSOSCon& con) const
{
	return impl_ == con.impl_;
}

OpBool Constraint::OpSOSCon::operator!=(const OpSOSCon& con) const
{
	return impl_ != con.impl_;
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

Constraint::OpSOSCon::~OpSOSCon()
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

OpBool Constraint::OpNLCon::operator==(const OpNLCon& con) const
{
	return impl_ == con.impl_;
}

OpBool Constraint::OpNLCon::operator!=(const OpNLCon& con) const
{
	return impl_ != con.impl_;
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

/* OPUA::Constraint::OpCdtCon */

void Constraint::OpCdtCon::setName(OpStr name)
{
	static_cast<OpCdtConI*>(impl_)->setName(name);
}

void Constraint::OpCdtCon::setVar(Variable::OpVar var)
{
	static_cast<OpCdtConI*>(impl_)->setVar(var);
}

void Constraint::OpCdtCon::setCon(OpLinCon con, OpBool con1)
{
	static_cast<OpCdtConI*>(impl_)->setCon(con, con1);
}

Variable::OpVar Constraint::OpCdtCon::getVar() const
{
	return static_cast<OpCdtConI*>(impl_)->getVar();
}

Constraint::OpLinCon Constraint::OpCdtCon::getCon(OpBool con1) const
{
	return static_cast<OpCdtConI*>(impl_)->getCon(con1);
}

OpBool Constraint::OpCdtCon::isIndicator() const
{
	return static_cast<OpCdtConI*>(impl_)->isIndicator();
}

OpStr Constraint::OpCdtCon::getName() const
{
	return static_cast<OpCdtConI*>(impl_)->getName();
}

Constraint::OpCdtConI* Constraint::OpCdtCon::getImpl() const
{
	return static_cast<OpCdtConI*>(impl_);
}

void Constraint::OpCdtCon::lock()
{
	static_cast<OpCdtConI*>(impl_)->lock();
}

void Constraint::OpCdtCon::unlock()
{
	static_cast<OpCdtConI*>(impl_)->unlock();
}

OpBool Constraint::OpCdtCon::operator==(const OpCdtCon& con) const
{
	return impl_ == con.impl_;
}

OpBool Constraint::OpCdtCon::operator!=(const OpCdtCon& con) const
{
	return impl_ != con.impl_;
}

Constraint::OpCdtCon::OpCdtCon()
{

}

Constraint::OpCdtCon::OpCdtCon(OpCdtConI* impl)
{
	impl_ = impl;
}

Constraint::OpCdtCon::OpCdtCon(OpEnv env)
{
	impl_ = new OpCdtConI(env.getImpl());
}

Constraint::OpCdtCon::OpCdtCon(OpEnv env, Variable::OpVar ind, OpLinCon con)
{
	impl_ = new OpCdtConI(env.getImpl(), ind, con);
}

Constraint::OpCdtCon::OpCdtCon(OpEnv env, Variable::OpVar ind, OpLinCon con, OpStr name)
{
	impl_ = new OpCdtConI(env.getImpl(), ind, con, name);
}

Constraint::OpCdtCon::OpCdtCon(OpEnv env, OpLinCon con1, OpLinCon con2)
{
	impl_ = new OpCdtConI(env.getImpl(), con1, con2);
}

Constraint::OpCdtCon::OpCdtCon(OpEnv env, OpLinCon con1, OpLinCon con2, OpStr name)
{
	impl_ = new OpCdtConI(env.getImpl(), con1, con2, name);
}

Constraint::OpCdtCon::~OpCdtCon()
{

}

/* OPUA::Constraint */

OpStr Constraint::ConSense2Str(OpConSense sense)
{
	OpStr str;
	switch (sense)
	{
	case OpConSense::Equal:
		str = " == ";
		break;
	case OpConSense::LessEqual:
		str = " <= ";
		break;
	case OpConSense::GreatEqual:
		str = " >= ";
		break;
	case OpConSense::SOS1:
		str = " SOS1 ";
		break;
	case OpConSense::SOS2:
		str = " SOS2 ";
		break;
	default:
		str = " ? ";
		break;
	}
	return str;
}

std::ostream& Constraint::operator<<(std::ostream& stream, OpLinCon con)
{
	if (!Constant::IsNInfinity(con.getLb()))
		stream << con.getLb() << " <= ";
	else
		stream << "-inf <= ";
	stream << con.getExpr();
	if (!Constant::IsPInfinity(con.getUb()))
		stream << " <= " << con.getUb();
	else
		stream << " <= inf";
	return stream;
}

Constraint::OpLinCon Constraint::operator<=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs)
{
	OpLinCon con(nullptr);
	OpFloat lb(lhs.getConstant() - rhs.getConstant());
	OpFloat ub(Constant::Infinity);
	auto expr(rhs - lhs);
	expr.setLinTerm(0.0);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpLinCon(OpEnv(lenv), lb, expr, ub);
	}
	else if (!lenv && renv)
		con = OpLinCon(OpEnv(renv), lb, expr, ub);
	else if (lenv && !renv)
		con = OpLinCon(OpEnv(lenv), lb, expr, ub);
	return con;
}

Constraint::OpLinCon Constraint::operator<=(OpLinCon lhs, OpFloat rhs)
{
	if (lhs.getImpl())
		lhs.setUb(rhs);
	return lhs;
}

Constraint::OpLinCon Constraint::operator>=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs)
{
	OpLinCon con(nullptr);
	OpFloat lb(-Constant::Infinity);
	OpFloat ub(lhs.getConstant() - rhs.getConstant());
	auto expr(rhs - lhs);
	expr.setLinTerm(0.0);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpLinCon(OpEnv(lenv), lb, expr, ub);
	}
	else if (!lenv && renv)
		con = OpLinCon(OpEnv(renv), lb, expr, ub);
	else if (lenv && !renv)
		con = OpLinCon(OpEnv(lenv), lb, expr, ub);
	return con;
}

Constraint::OpLinCon Constraint::operator>=(OpLinCon lhs, OpFloat rhs)
{
	if (lhs.getImpl())
		lhs.setLb(rhs);
	return lhs;
}

Constraint::OpLinCon Constraint::operator==(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs)
{
	OpLinCon con(nullptr);
	OpFloat lb(lhs.getConstant() - rhs.getConstant());
	OpFloat ub(lb);
	auto expr(rhs - lhs);
	expr.setLinTerm(0.0);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpLinCon(OpEnv(lenv), lb, expr, ub);
	}
	else if (!lenv && renv)
		con = OpLinCon(OpEnv(renv), lb, expr, ub);
	else if (lenv && !renv)
		con = OpLinCon(OpEnv(lenv), lb, expr, ub);
	return con;
}

std::ostream& Constraint::operator<<(std::ostream& stream, OpQuadCon con)
{
	if (!Constant::IsNInfinity(con.getLb()))
		stream << con.getLb() << " <= ";
	else
		stream << "-inf <= ";
	stream << con.getExpr();
	if (!Constant::IsPInfinity(con.getUb()))
		stream << " <= " << con.getUb();
	else
		stream << " <= inf";
	return stream;
}

Constraint::OpQuadCon Constraint::operator<=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs)
{
	OpQuadCon con(nullptr);
	OpFloat lb(lhs.getConstant() - rhs.getConstant());
	OpFloat ub(Constant::Infinity);
	auto expr(rhs - lhs);
	expr.setLinTerm(0.0);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpQuadCon(OpEnv(lenv), lb, expr, ub);
	}
	else if (!lenv && renv)
		con = OpQuadCon(OpEnv(renv), lb, expr, ub);
	else if (lenv && !renv)
		con = OpQuadCon(OpEnv(lenv), lb, expr, ub);
	return con;
}

Constraint::OpQuadCon Constraint::operator<=(OpQuadCon lhs, OpFloat rhs)
{
	if (lhs.getImpl())
		lhs.setUb(rhs);
	return lhs;
}

Constraint::OpQuadCon Constraint::operator>=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs)
{
	OpQuadCon con(nullptr);
	OpFloat lb(-Constant::Infinity);
	OpFloat ub(lhs.getConstant() - rhs.getConstant());
	auto expr(rhs - lhs);
	expr.setLinTerm(0.0);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpQuadCon(OpEnv(lenv), lb, expr, ub);
	}
	else if (!lenv && renv)
		con = OpQuadCon(OpEnv(renv), lb, expr, ub);
	else if (lenv && !renv)
		con = OpQuadCon(OpEnv(lenv), lb, expr, ub);
	return con;
}

Constraint::OpQuadCon Constraint::operator>=(OpQuadCon lhs, OpFloat rhs)
{
	if (lhs.getImpl())
		lhs.setLb(rhs);
	return lhs;
}

Constraint::OpQuadCon Constraint::operator==(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs)
{
	OpQuadCon con(nullptr);
	OpFloat lb(lhs.getConstant() - rhs.getConstant());
	OpFloat ub(lb);
	auto expr(rhs - lhs);
	expr.setLinTerm(0.0);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpQuadCon(OpEnv(lenv), lb, expr, ub);
	}
	else if (!lenv && renv)
		con = OpQuadCon(OpEnv(renv), lb, expr, ub);
	else if (lenv && !renv)
		con = OpQuadCon(OpEnv(lenv), lb, expr, ub);
	return con;
}

Constraint::OpQuadCon Constraint::OpStdSOCByQC(OpEnv env, Variable::OpVarArr X)
{
	OpQuadCon con(nullptr);
	if (X.getSize() >= 2)
	{
		Expression::OpQuadExpr expr(0.0);
		expr.addQuadTerm(X[0], X[0], 1.0);
		for (OpULInt i = 1; i < X.getSize(); i++)
			expr.addQuadTerm(X[i], X[i], -1.0);
		con = OpQuadCon(env, 0.0, expr, Constant::Infinity);
	}
	return con;
}

Constraint::OpQuadCon Constraint::OpStdRSOCByQC(OpEnv env, Variable::OpVarArr X)
{
	OpQuadCon con(nullptr);
	if (X.getSize() >= 3)
	{
		Expression::OpQuadExpr expr(0.0);
		expr.addQuadTerm(X[0], X[1], 2.0);
		for (OpULInt i = 2; i < X.getSize(); i++)
			expr.addQuadTerm(X[i], X[i], -1.0);
		con = OpQuadCon(env, 0.0, expr, Constant::Infinity);
	}
	return con;
}

std::ostream& Constraint::operator<<(std::ostream& stream, OpConicCon con)
{
	auto& expr(con.getConicExpr());
	switch (con.getSense())
	{
	case OpConicSense::Unknow:
		break;
	case OpConicSense::SOC:
		stream << expr.getVar(0).getName() << " >= ( ";
		for (OpULInt i = 1; i < expr.getSize() - 1; i++)
			stream << expr.getVar(i).getName() << "^^2 + ";
		stream << expr.getVar(expr.getSize() - 1).getName() << "^^2 ) ^^0.5";
		break;
	case OpConicSense::RSOC:
		stream << "2 * " << expr.getVar(0).getName() << " * " << expr.getVar(1).getName() << " >= ";
		for (OpULInt i = 2; i < expr.getSize() - 1; i++)
			stream << expr.getVar(i).getName() << "^^2 + ";
		stream << expr.getVar(expr.getSize() - 1).getName() << "^^2";
		break;
	case OpConicSense::PC:
		stream << expr.getVar(0).getName() << "^^" << expr.getParam() << " * " << expr.getVar(1).getName() << "^^" << (1 - expr.getParam()) << " >= ( ";
		for (OpULInt i = 2; i < expr.getSize() - 1; i++)
			stream << expr.getVar(i).getName() << "^^2 + ";
		stream << expr.getVar(expr.getSize() - 1).getName() << "^^2 ) ^^0.5";
		break;
	case OpConicSense::DPC:
		stream << "( " << expr.getVar(0).getName() << " / " << expr.getParam() << " )^^" << expr.getParam() << " * "
			<< "( " << expr.getVar(1).getName() << " / " << (1 - expr.getParam()) << " )^^" << (1 - expr.getParam()) << " >= ( ";
		for (OpULInt i = 2; i < expr.getSize() - 1; i++)
			stream << expr.getVar(i).getName() << "^^2 + ";
		stream << expr.getVar(expr.getSize() - 1).getName() << "^^2 ) ^^0.5";
		break;
	case OpConicSense::EC:
		stream << expr.getVar(0).getName() << " >= " 
			<< expr.getVar(1).getName() << " * Exp1 ( " << expr.getVar(2).getName() << " / " << expr.getVar(1).getName() << " )";
		break;
	case OpConicSense::DEC:
		stream << expr.getVar(0).getName() << " >= -1 * "
			<< expr.getVar(2).getName() << " * Exp1 ( -1 ) * Exp1 ( " << expr.getVar(1).getName() << " / " << expr.getVar(2).getName() << " )";
		break;
	default:
		break;
	}
	return stream;
}

std::ostream& Constraint::operator<<(std::ostream& stream, OpPSDCon con)
{
	if (!Constant::IsNInfinity(con.getLb()))
		stream << con.getLb() << " <= ";
	else
		stream << "-inf <= ";
	stream << con.getExpr();
	if (!Constant::IsPInfinity(con.getUb()))
		stream << " <= " << con.getUb();
	else
		stream << " <= inf";
	return stream;
}

Constraint::OpPSDCon Constraint::operator<=(const Expression::OpPSDExpr& lhs, const Expression::OpPSDExpr& rhs)
{
	OpPSDCon con(nullptr);
	OpFloat lb(lhs.getConstant() - rhs.getConstant());
	OpFloat ub(Constant::Infinity);
	auto expr(rhs - lhs);
	expr.setLinTerm(0.0);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpPSDCon(OpEnv(lenv), lb, expr, ub);
	}
	else if (!lenv && renv)
		con = OpPSDCon(OpEnv(renv), lb, expr, ub);
	else if (lenv && !renv)
		con = OpPSDCon(OpEnv(lenv), lb, expr, ub);
	return con;
}

Constraint::OpPSDCon Constraint::operator<=(OpPSDCon lhs, OpFloat rhs)
{
	if (lhs.getImpl())
		lhs.setUb(rhs);
	return lhs;
}

Constraint::OpPSDCon Constraint::operator>=(const Expression::OpPSDExpr& lhs, const Expression::OpPSDExpr& rhs)
{
	OpPSDCon con(nullptr);
	OpFloat lb(-Constant::Infinity);
	OpFloat ub(lhs.getConstant() - rhs.getConstant());
	auto expr(rhs - lhs);
	expr.setLinTerm(0.0);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpPSDCon(OpEnv(lenv), lb, expr, ub);
	}
	else if (!lenv && renv)
		con = OpPSDCon(OpEnv(renv), lb, expr, ub);
	else if (lenv && !renv)
		con = OpPSDCon(OpEnv(lenv), lb, expr, ub);
	return con;
}

Constraint::OpPSDCon Constraint::operator>=(OpPSDCon lhs, OpFloat rhs)
{
	if (lhs.getImpl())
		lhs.setLb(rhs);
	return lhs;
}

Constraint::OpPSDCon Constraint::operator==(const Expression::OpPSDExpr& lhs, const Expression::OpPSDExpr& rhs)
{
	OpPSDCon con(nullptr);
	OpFloat lb(lhs.getConstant() - rhs.getConstant());
	OpFloat ub(lb);
	auto expr(rhs - lhs);
	expr.setLinTerm(0.0);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv)
	{
		if (lenv == renv)
			con = OpPSDCon(OpEnv(lenv), lb, expr, ub);
	}
	else if (!lenv && renv)
		con = OpPSDCon(OpEnv(renv), lb, expr, ub);
	else if (lenv && !renv)
		con = OpPSDCon(OpEnv(lenv), lb, expr, ub);
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
	if (lenv && renv && lenv == renv)
		con = OpNLCon(OpEnv(lenv), rhs, lhs);
	return con;
}

Constraint::OpNLCon Constraint::operator==(Variable::OpVar lhs, const Expression::OpNLExpr& rhs)
{
	OpNLCon con(nullptr);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv && lenv == renv)
		con = OpNLCon(OpEnv(lenv), lhs, rhs);
	return con;
}

std::ostream& Constraint::operator<<(std::ostream& stream, OpCdtCon con)
{
	stream << "( ";
	if (con.isIndicator())
		stream << con.getVar().getName() << " == 1";
	else
		stream << con.getCon(true);
	stream << " ) -> " << con.getCon(false) << " )";
	return stream;
}

Constraint::OpCdtCon Constraint::operator>>(Variable::OpVar lhs, OpLinCon rhs)
{
	OpCdtCon con(nullptr);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv && lenv == renv)
		con = OpCdtCon(OpEnv(lenv), lhs, rhs);
	return con;
}

Constraint::OpCdtCon Constraint::operator>>(OpLinCon lhs, OpLinCon rhs)
{
	OpCdtCon con(nullptr);
	auto lenv(lhs.getEnv().getImpl()), renv(rhs.getEnv().getImpl());
	if (lenv && renv && lenv == renv)
		con = OpCdtCon(OpEnv(lenv), lhs, rhs);
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