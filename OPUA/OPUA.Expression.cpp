#include "OPUA.Expression.h"
#include <algorithm>

using namespace OPUA;

std::ostream& Expression::operator<<(std::ostream& stream, const Expression::OpLinExpr& expr)
{
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		stream << iter.getCoeff() << " * " << iter.getVar().getName() << " + ";
	stream << expr.getConstant();
	return stream;
}

Expression::OpLinExpr Expression::operator+(const OpLinExpr& expr1, const OpLinExpr& expr2)
{
	OpLinExpr result(expr1);
	result += expr2;
	return result;
}

Expression::OpLinExpr Expression::operator+(const OpLinExpr& expr)
{
	return expr;
}

Expression::OpLinExpr Expression::operator+(Variable::OpVar var1, Variable::OpVar var2)
{
	OpLinExpr result(var1, 1.0);
	result.addLinTerm(var2, 1.0);
	return result;
}

Expression::OpLinExpr Expression::operator+(Variable::OpVar var, OpFloat val)
{
	OpLinExpr result(val);
	result.addLinTerm(var, 1.0);
	return result;
}

Expression::OpLinExpr Expression::operator+(OpFloat val, Variable::OpVar var)
{
	OpLinExpr result(val);
	result.addLinTerm(var, 1.0);
	return result;
}

Expression::OpLinExpr Expression::operator-(const OpLinExpr& expr1, const OpLinExpr& expr2)
{
	OpLinExpr result(expr1);
	result -= expr2;
	return result;
}

Expression::OpLinExpr Expression::operator-(const OpLinExpr& expr)
{
	OpLinExpr result(0.0);
	result -= expr;
	return result;
}

Expression::OpLinExpr Expression::operator-(Variable::OpVar var)
{
	return OpLinExpr(var, -1.0);
}

Expression::OpLinExpr Expression::operator-(Variable::OpVar var1, Variable::OpVar var2)
{
	OpLinExpr result(var1, 1.0);
	result.addLinTerm(var2, -1.0);
	return result;
}

Expression::OpLinExpr Expression::operator-(Variable::OpVar var, OpFloat val)
{
	OpLinExpr result(var, 1.0);
	result.addLinTerm(-val);
	return result;
}

Expression::OpLinExpr Expression::operator-(OpFloat val, Variable::OpVar var)
{
	OpLinExpr result(val);
	result.addLinTerm(var, -1.0);
	return result;
}

Expression::OpLinExpr Expression::operator*(OpFloat val, Variable::OpVar var)
{
	OpLinExpr result(var, 1.0);
	result *= val;
	return result;
}

Expression::OpLinExpr Expression::operator*(Variable::OpVar var, OpFloat val)
{
	OpLinExpr result(var, 1.0);
	result *= val;
	return result;
}

Expression::OpLinExpr Expression::operator*(const OpLinExpr& expr, OpFloat val)
{
	OpLinExpr result(expr);
	result *= val;
	return result;
}

Expression::OpLinExpr Expression::operator*(OpFloat val, const OpLinExpr& expr)
{
	OpLinExpr result(expr);
	result *= val;
	return result;
}

Expression::OpLinExpr Expression::operator/(Variable::OpVar var, OpFloat val)
{
	OpLinExpr result(var, 1.0);
	result /= val;
	return result;
}

Expression::OpLinExpr Expression::operator/(const OpLinExpr& expr, OpFloat val)
{
	OpLinExpr result(expr);
	result /= val;
	return result;
}

std::ostream& Expression::operator<<(std::ostream& stream, const Expression::OpQuadExpr& expr)
{
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		stream << iter.getCoeff() << " * " << iter.getVar1().getName() << " * " << iter.getVar2().getName() << " + ";
	stream << expr.linexpr_;
	return stream;
}

Expression::OpQuadExpr Expression::operator+(const OpQuadExpr& expr1, const OpQuadExpr& expr2)
{
	OpQuadExpr result(expr1);
	result += expr2;
	return result;
}

Expression::OpQuadExpr Expression::operator-(const OpQuadExpr& expr1, const OpQuadExpr& expr2)
{
	OpQuadExpr result(expr1);
	result -= expr2;
	return result;
}

Expression::OpQuadExpr Expression::operator+(const OpQuadExpr& expr)
{
	return expr;
}

Expression::OpQuadExpr Expression::operator-(const OpQuadExpr& expr)
{
	OpQuadExpr result(0.0);
	result -= expr;
	return result;
}

Expression::OpQuadExpr Expression::operator*(const OpQuadExpr& expr, OpFloat val)
{
	OpQuadExpr result(expr);
	result *= val;
	return result;
}

Expression::OpQuadExpr Expression::operator*(OpFloat val, const OpQuadExpr& expr)
{
	OpQuadExpr result(expr);
	result *= val;
	return result;
}

Expression::OpQuadExpr Expression::operator*(Variable::OpVar var1, Variable::OpVar var2)
{
	OpQuadExpr result(0.0);
	result.addQuadTerm(var1, var2, 1.0);
	return result;
}

Expression::OpQuadExpr Expression::operator*(Variable::OpVar var, const OpLinExpr& expr)
{
	OpQuadExpr result(0.0);
	for (auto& iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result.addQuadTerm(var, iter.getVar(), iter.getCoeff());
	result.addLinTerm(var, expr.getConstant());
	return result;
}

Expression::OpQuadExpr Expression::operator*(const OpLinExpr& expr, Variable::OpVar var)
{
	OpQuadExpr result(0.0);
	for (auto& iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result.addQuadTerm(var, iter.getVar(), iter.getCoeff());
	result.addLinTerm(var, expr.getConstant());
	return result;
}

Expression::OpQuadExpr Expression::operator*(const OpLinExpr& expr1, const OpLinExpr& expr2)
{
	OpFloat c1(expr1.getConstant()), c2(expr2.getConstant());
	OpQuadExpr result(c1 * c2);
	// 二次项
	for (auto iter1 = expr1.getLBegin(); iter1 != expr1.getLEnd(); ++iter1)
		for (auto iter2 = expr2.getLBegin(); iter2 != expr2.getLEnd(); ++iter2)
			result.addQuadTerm(iter1.getVar(), iter2.getVar(), iter1.getCoeff() * iter2.getCoeff()); 
	// 线性项
	for (auto iter1 = expr1.getLBegin(); iter1 != expr1.getLEnd(); ++iter1)
		result.addLinTerm(iter1.getVar(), iter1.getCoeff() * c2);
	for (auto iter2 = expr2.getLBegin(); iter2 != expr2.getLEnd(); ++iter2)
		result.addLinTerm(iter2.getVar(), iter2.getCoeff() * c1);
	return result;
}

Expression::OpQuadExpr Expression::operator/(const OpQuadExpr& expr, OpFloat val)
{
	OpQuadExpr result(expr);
	result /= val;
	return result;
}

OpStr OPUA::Expression::NLFunc2Str(OpNLFunc func)
{
	OpStr str(" ? ");
	if (func == OpNLFunc::Sum)
		str = " Sum ";
	else if (func == OpNLFunc::Abs)
		str = " Abs ";
	else if (func == OpNLFunc::Max)
		str = " Max ";
	else if (func == OpNLFunc::Min)
		str = " Min ";
	else if (func == OpNLFunc::Square)
		str = " Square ";
	else if (func == OpNLFunc::Sqrt)
		str = " Sqrt ";
	else if (func == OpNLFunc::Pow)
		str = " Pow ";
	else if (func == OpNLFunc::Exp1)
		str = " Exp1 ";
	else if (func == OpNLFunc::Exp2)
		str = " Exp2 ";
	else if (func == OpNLFunc::Log1)
		str = " Log1 ";
	else if (func == OpNLFunc::Log2)
		str = " Log2 ";
	else if (func == OpNLFunc::Log3)
		str = " Log3 ";
	else if (func == OpNLFunc::Sin)
		str = " Sin ";
	else if (func == OpNLFunc::Cos)
		str = " Cos ";
	else if (func == OpNLFunc::Tan)
		str = " Tan ";
	return str;
}

/* OPUA::Expression::OpLinExpr */

OpULInt Expression::OpLinExpr::getSize() const
{
	return linterm_.size();
}

OpEnv Expression::OpLinExpr::getEnv() const
{
	OpEnv env(nullptr);
	if (getSize())
	{
		for (auto& lin : linterm_)
		{
			env = Variable::OpVar(lin.first).getEnv();
			break;
		}
	}
	return env;
}

OpFloat Expression::OpLinExpr::getCoeff(Variable::OpVar var) const
{
	auto iter = linterm_.find(var.getImpl());
	if (iter != linterm_.end())
		return iter->second;
	else
		return 0.0;
}

Expression::OpLinExpr Expression::OpLinExpr::getLinTerm() const
{
	OpLinExpr tmp(0.0);
	tmp.linterm_ = linterm_;
	return tmp;
}

OpFloat Expression::OpLinExpr::getConstant() const
{
	return constant_;
}

void Expression::OpLinExpr::setLinTerm(OpFloat val)
{
	constant_ = val;
}

void Expression::OpLinExpr::setLinTerm(Variable::OpVar var, OpFloat coeff)
{
	linterm_[var.getImpl()] = coeff;
}

void Expression::OpLinExpr::addLinTerm(OpFloat val)
{
	constant_ += val;
}

void Expression::OpLinExpr::addLinTerm(Variable::OpVar var, OpFloat coeff)
{
	linterm_[var.getImpl()] += coeff;
}

void Expression::OpLinExpr::removeVar(Variable::OpVar var)
{
	linterm_.erase(var.getImpl());
}

void Expression::OpLinExpr::clear()
{
	linterm_.clear();
	constant_ = 0.0;
}

void Expression::OpLinExpr::simplify(OpFloat zero)
{
	for (auto it = linterm_.begin(); it != linterm_.end();)
	{
		if (std::abs(it->second) < zero)
			it = linterm_.erase(it);
		else
			++it;
	}
}

Expression::OpLinExpr& Expression::OpLinExpr::operator+=(const OpLinExpr& expr)
{
	for (auto& lin : expr.linterm_)
		linterm_[lin.first] += lin.second;
	constant_ += expr.constant_;
	return *this;
}

Expression::OpLinExpr& Expression::OpLinExpr::operator-=(const OpLinExpr& expr)
{
	for (auto& lin : expr.linterm_)
		linterm_[lin.first] -= lin.second;
	constant_ -= expr.constant_;
	return *this;
}

Expression::OpLinExpr& Expression::OpLinExpr::operator*=(OpFloat val)
{
	for (auto& lin : linterm_)
		lin.second *= val;
	constant_ *= val;
	return *this;
}

Expression::OpLinExpr& Expression::OpLinExpr::operator/=(OpFloat val)
{
	for (auto& lin : linterm_)
		lin.second /= val;
	constant_ /= val;
	return *this;
}

OpBool Expression::OpLinExpr::OpLEIter::operator==(const OpLEIter& iter) const
{
	return iter_ == iter.iter_;
}

OpBool Expression::OpLinExpr::OpLEIter::operator!=(const OpLEIter& iter) const
{
	return iter_ != iter.iter_;
}

Expression::OpLinExpr::OpLEIter& Expression::OpLinExpr::OpLEIter::operator--()
{
	--iter_;
	return *this;
}

Expression::OpLinExpr::OpLEIter& Expression::OpLinExpr::OpLEIter::operator++()
{
	++iter_;
	return *this;
}

Variable::OpVar Expression::OpLinExpr::OpLEIter::getVar() const
{
	return Variable::OpVar(iter_->first);
}

OpFloat Expression::OpLinExpr::OpLEIter::getCoeff() const
{
	return iter_->second;
}

Expression::OpLinExpr::OpLEIter::OpLEIter(LinTerm iter)
	: iter_(iter)
{

}

Expression::OpLinExpr::OpLEIter Expression::OpLinExpr::getLBegin() const
{
	return OpLEIter(linterm_.cbegin());
}

Expression::OpLinExpr::OpLEIter Expression::OpLinExpr::getLEnd() const
{
	return OpLEIter(linterm_.cend());
}

Expression::OpLinExpr::OpLinExpr(OpFloat constant)
	: constant_(constant)
{

}

Expression::OpLinExpr::OpLinExpr(Variable::OpVar var, OpFloat coeff)
	: constant_(0.0)
{
	addLinTerm(var, coeff);
}

/* OPUA::Expression::OpQuadExpr */

OpULInt Expression::OpQuadExpr::getSize() const
{
	return linexpr_.getSize() + quadterm_.size();
}

OpEnv Expression::OpQuadExpr::getEnv() const
{
	OpEnv env(nullptr);
	if (linexpr_.getSize())
		env = linexpr_.getEnv();
	else if (quadterm_.size())
	{
		for (auto & q : quadterm_)
		{
			env = Variable::OpVar(q.first.first).getEnv();
			break;
		}
	}
	return env;
}

OpFloat Expression::OpQuadExpr::getCoeff(Variable::OpVar var) const
{
	return linexpr_.getCoeff(var);
}

OpFloat Expression::OpQuadExpr::getCoeff(Variable::OpVar var1, Variable::OpVar var2) const
{
	auto iter = quadterm_.find({ var1.getImpl(), var2.getImpl() });
	if (iter != quadterm_.end())
		return iter->second;
	else
		return 0.0;
}

Expression::OpLinExpr Expression::OpQuadExpr::getLinTerm() const
{
	return linexpr_.getLinTerm();
}

Expression::OpQuadExpr Expression::OpQuadExpr::getQuadTerm() const
{
	OpQuadExpr tmp(0.0);
	tmp.quadterm_ = quadterm_;
	return tmp;
}

OpFloat Expression::OpQuadExpr::getConstant() const
{
	return linexpr_.getConstant();
}

void Expression::OpQuadExpr::setLinTerm(OpFloat val)
{
	linexpr_.setLinTerm(val);
}

void Expression::OpQuadExpr::setLinTerm(Variable::OpVar var, OpFloat coeff)
{
	linexpr_.setLinTerm(var, coeff);
}

void Expression::OpQuadExpr::setQuadTerm(Variable::OpVar var1, Variable::OpVar var2, OpFloat coeff)
{
	quadterm_[{var1.getImpl(), var2.getImpl()}] = coeff;
}

void Expression::OpQuadExpr::addLinTerm(OpFloat val)
{
	linexpr_.addLinTerm(val);
}

void Expression::OpQuadExpr::addLinTerm(Variable::OpVar var, OpFloat coeff)
{
	linexpr_.addLinTerm(var, coeff);
}

void Expression::OpQuadExpr::addQuadTerm(Variable::OpVar var1, Variable::OpVar var2, OpFloat coeff)
{
	quadterm_[{var1.getImpl(), var2.getImpl()}] += coeff;
}

void Expression::OpQuadExpr::removeVar(Variable::OpVar var)
{
	linexpr_.removeVar(var);
	auto impl = var.getImpl();
	for (auto it = quadterm_.begin(); it != quadterm_.end();)
	{
		if (it->first.first == impl || it->first.second == impl)
			it = quadterm_.erase(it);
		else
			++it;
	}
}

void Expression::OpQuadExpr::removeLinTerm(Variable::OpVar var)
{
	linexpr_.removeVar(var);
}

void Expression::OpQuadExpr::removeQuadTerm(Variable::OpVar var1, Variable::OpVar var2)
{
	quadterm_.erase({ var1.getImpl(), var2.getImpl() });
}

void Expression::OpQuadExpr::clear()
{
	linexpr_.clear();
	quadterm_.clear();
}

void Expression::OpQuadExpr::simplify(OpFloat zero)
{
	for (auto it = quadterm_.begin(); it != quadterm_.end();)
	{
		if (std::abs(it->second) < zero)
			it = quadterm_.erase(it);
		else
			++it;
	}
	linexpr_.simplify(zero);
}

Expression::OpQuadExpr& Expression::OpQuadExpr::operator+=(const OpQuadExpr& expr)
{
	linexpr_ += expr.linexpr_;
	for (auto& q : expr.quadterm_)
		quadterm_[q.first] += q.second;
	return *this;
}

Expression::OpQuadExpr& Expression::OpQuadExpr::operator-=(const OpQuadExpr& expr)
{
	linexpr_ -= expr.linexpr_;
	for (auto& q : expr.quadterm_)
		quadterm_[q.first] -= q.second;
	return *this;
}

Expression::OpQuadExpr& Expression::OpQuadExpr::operator*=(OpFloat val)
{
	linexpr_ *= val;
	for (auto& q : quadterm_)
		q.second *= val;
	return *this;
}

Expression::OpQuadExpr& Expression::OpQuadExpr::operator/=(OpFloat val)
{
	linexpr_ /= val;
	for (auto& q : quadterm_)
		q.second /= val;
	return *this;
}

OpBool Expression::OpQuadExpr::OpQEIter::operator==(const OpQEIter& iter) const
{
	return iter_ == iter.iter_;
}

OpBool Expression::OpQuadExpr::OpQEIter::operator!=(const OpQEIter& iter) const
{
	return iter_ != iter.iter_;
}

Expression::OpQuadExpr::OpQEIter& Expression::OpQuadExpr::OpQEIter::operator--()
{
	--iter_;
	return *this;
}

Expression::OpQuadExpr::OpQEIter& Expression::OpQuadExpr::OpQEIter::operator++()
{
	++iter_;
	return *this;
}

Variable::OpVar Expression::OpQuadExpr::OpQEIter::getVar1() const
{
	return Variable::OpVar(iter_->first.first);
}

Variable::OpVar Expression::OpQuadExpr::OpQEIter::getVar2() const
{
	return Variable::OpVar(iter_->first.second);
}

OpFloat Expression::OpQuadExpr::OpQEIter::getCoeff() const
{
	return iter_->second;
}

Expression::OpQuadExpr::OpQEIter::OpQEIter(QuadTerm iter)
	: iter_(iter)
{

}

Expression::OpLinExpr::OpLEIter Expression::OpQuadExpr::getLBegin() const
{
	return linexpr_.getLBegin();
}

Expression::OpLinExpr::OpLEIter Expression::OpQuadExpr::getLEnd() const
{
	return linexpr_.getLEnd();
}

Expression::OpQuadExpr::OpQEIter Expression::OpQuadExpr::getQBegin() const
{
	return OpQEIter(quadterm_.begin());
}

Expression::OpQuadExpr::OpQEIter Expression::OpQuadExpr::getQEnd() const
{
	return OpQEIter(quadterm_.end());
}

Expression::OpQuadExpr::OpQuadExpr(OpFloat constant)
	: linexpr_(constant)
{

}

Expression::OpQuadExpr::OpQuadExpr(Variable::OpVar var, OpFloat coeff)
	: linexpr_(var, coeff)
{

}

Expression::OpQuadExpr::OpQuadExpr(const OpLinExpr& expr)
	: linexpr_(expr)
{

}

/* OPUA::Expression::OpNLExpr */

OpULInt Expression::OpNLExpr::getSize() const
{
	return nlterm_.size();
}

OpEnv Expression::OpNLExpr::getEnv() const
{
	OpEnv env(nullptr);
	if (getSize())
	{
		for (auto& nl : nlterm_)
		{
			env = Variable::OpVar(nl).getEnv();
			break;
		}
	}
	return env;
}

Expression::OpNLFunc Expression::OpNLExpr::getFunction() const
{
	return func_;
}

OpFloat OPUA::Expression::OpNLExpr::getParam() const
{
	return param_;
}

Variable::OpVar OPUA::Expression::OpNLExpr::getVar(OpULInt order) const
{
	return Variable::OpVar(nlterm_.at(order));
}

void Expression::OpNLExpr::setFunction(OpNLFunc func)
{
	func_ = func;
}

void OPUA::Expression::OpNLExpr::setParam(OpFloat param)
{
	param_ = param;
}

void Expression::OpNLExpr::addVar(Variable::OpVar var)
{
	nlterm_.emplace_back(var.getImpl());
}

void Expression::OpNLExpr::removeVar(Variable::OpVar var)
{
	nlterm_.erase(std::remove(nlterm_.begin(), nlterm_.end(), var.getImpl()), nlterm_.end());
}

void Expression::OpNLExpr::clear()
{
	nlterm_.clear();
	func_ = OpNLFunc::Unknow;
}

Expression::OpNLExpr::OpNLEIter Expression::OpNLExpr::getNLBegin() const
{
	return OpNLEIter(nlterm_.cbegin());
}

Expression::OpNLExpr::OpNLEIter Expression::OpNLExpr::getNLEnd() const
{
	return OpNLEIter(nlterm_.cend());
}

Expression::OpNLExpr::OpNLExpr(OpNLFunc func)
	: func_(func),
	param_(0.0)
{

}

Expression::OpNLExpr::OpNLExpr(OpNLFunc func, Variable::OpVarArr vars)
	: func_(func),
	param_(0.0)
{
	for (auto iter = vars.getCBegin(); iter != vars.getCEnd(); ++iter)
		addVar(iter.getVal());
}

OPUA::Expression::OpNLExpr::OpNLExpr(OpNLFunc func, OpFloat param, Variable::OpVarArr vars)
	: func_(func),
	param_(param)
{
	for (auto iter = vars.getCBegin(); iter != vars.getCEnd(); ++iter)
		addVar(iter.getVal());
}

OpBool Expression::OpNLExpr::OpNLEIter::operator==(const OpNLEIter& iter) const
{
	return iter_ == iter.iter_;
}

OpBool Expression::OpNLExpr::OpNLEIter::operator!=(const OpNLEIter& iter) const
{
	return iter_ != iter.iter_;
}

Expression::OpNLExpr::OpNLEIter& Expression::OpNLExpr::OpNLEIter::operator--()
{
	--iter_;
	return *this;
}

Expression::OpNLExpr::OpNLEIter& Expression::OpNLExpr::OpNLEIter::operator++()
{
	++iter_;
	return *this;
}

Variable::OpVar Expression::OpNLExpr::OpNLEIter::getVar() const
{
	return Variable::OpVar(*iter_);
}

Expression::OpNLExpr::OpNLEIter::OpNLEIter(NLTerm iter)
	: iter_(iter)
{

}
