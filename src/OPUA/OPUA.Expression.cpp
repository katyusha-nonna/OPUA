#include "OPUA.Expression.h"
#include <iostream>
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
	OpLinExpr result(expr);
	result.piecewiseInv();
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
	OpQuadExpr result(expr);
	result.piecewiseInv();
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

std::ostream& Expression::operator<<(std::ostream& stream, const OpDenseMat& mat)
{
	OpULInt count(0);
	stream << "[ ";
	for (OpULInt i = 0; i < mat.dim_ - 1; i++)
	{
		for (OpULInt j = 0; j < mat.dim_ - 1; j++)
			stream << mat.data_[count++] << ", ";	
		stream << mat.data_[count++] << "; ";
	}
	for (OpULInt j = 0; j < mat.dim_ - 1; j++)
		stream << mat.data_[count++] << ", ";
	stream << mat.data_[count++] << " ]";
	return stream;
}

std::ostream& Expression::operator<<(std::ostream& stream, const OpSpraseMat& mat)
{
	stream << "[ ";
	for (OpULInt i = 0; i < mat.dim_; i++)
	{
		if (mat.off_[i] == mat.off_[i + 1])
		{
			for (OpULInt j = 0; j < mat.dim_ - 1; j++)
				stream << "0, ";
			stream << '0' << (i < mat.dim_ ? "; " : " ");
		}
		else
		{
			for (OpULInt j = 0; j < mat.col_[mat.off_[i]]; j++)
				stream << "0, ";
			for (OpULInt j = mat.off_[i]; j < mat.off_[i + 1]; j++)
			{
				if (mat.col_[j] < mat.dim_ - 1)
				{
					stream << mat.data_[j] << ", ";
					if (j < mat.off_[i + 1] - 1)
					{
						for (OpULInt m = mat.col_[j] + 1; m < mat.col_[j + 1]; m++)
							stream << "0, ";
					}
					else
					{
						for (OpULInt m = mat.col_[j] + 1; m < mat.dim_ - 1; m++)
							stream << "0, ";
						stream << '0' << (i < mat.dim_ ? "; " : " ");
					}
				}
				else
				{
					stream << mat.data_[j] << (i < mat.dim_ ? "; " : " ");
				}
			}
		}
	}
	stream << " ]";
	return stream;
}

std::ostream& Expression::operator<<(std::ostream& stream, const OpPSDExpr& expr)
{
	for (auto iter = expr.getPSDBegin(); iter != expr.getPSDEnd(); ++iter)
		stream << iter.getCoeff() << " * " << iter.getVar().getName() << " + ";
	stream << expr.linexpr_;
	return stream;
}

Expression::OpPSDExpr Expression::operator+(const OpPSDExpr& expr1, const OpPSDExpr& expr2)
{
	OpPSDExpr result(expr1);
	result += expr2;
	return result;
}

Expression::OpPSDExpr Expression::operator-(const OpPSDExpr& expr1, const OpPSDExpr& expr2)
{
	OpPSDExpr result(expr1);
	result -= expr2;
	return result;
}

Expression::OpPSDExpr Expression::operator+(const OpPSDExpr& expr)
{
	return expr;
}

Expression::OpPSDExpr Expression::operator-(const OpPSDExpr& expr)
{
	OpPSDExpr result(expr);
	result.piecewiseInv();
	return result;
}

Expression::OpPSDExpr Expression::operator*(const OpPSDExpr& expr, OpFloat val)
{
	OpPSDExpr result(expr);
	result *= val;
	return result;
}

Expression::OpPSDExpr Expression::operator*(OpFloat val, const OpPSDExpr& expr)
{
	OpPSDExpr result(expr);
	result *= val;
	return result;
}

Expression::OpPSDExpr Expression::operator*(Variable::OpPSDVar var, const OpPSDRealMat& mat)
{
	OpPSDExpr result(0.0);
	result.addPSDTerm(var, mat);
	return result;
}

Expression::OpPSDExpr Expression::operator*(const OpPSDRealMat& mat, Variable::OpPSDVar var)
{
	OpPSDExpr result(0.0);
	result.addPSDTerm(var, mat);
	return result;
}

Expression::OpPSDExpr Expression::operator/(const OpPSDExpr& expr, OpFloat val)
{
	OpPSDExpr result(expr);
	result /= val;
	return result;
}

OpStr Expression::NLFunc2Str(OpNLFunc func)
{
	OpStr str;
	switch (func)
	{
	case OpNLFunc::Unknow:
		str = " ? ";
		break;
	case OpNLFunc::Sum:
		str = " Sum ";
		break;
	case OpNLFunc::Abs:
		str = " Abs ";
		break;
	case OpNLFunc::Max:
		str = " Max ";
		break;
	case OpNLFunc::Min:
		str = " Min ";
		break;
	case OpNLFunc::Square:
		str = " Square ";
		break;
	case OpNLFunc::Sqrt:
		str = " Sqrt ";
		break;
	case OpNLFunc::Pow:
		str = " Pow ";
		break;
	case OpNLFunc::Exp1:
		str = " Exp1 ";
		break;
	case OpNLFunc::Exp2:
		str = " Exp2 ";
		break;
	case OpNLFunc::Log1:
		str = " Log1 ";
		break;
	case OpNLFunc::Log2:
		str = " Log2 ";
		break;
	case OpNLFunc::Log3:
		str = " Log3 ";
		break;
	case OpNLFunc::Sin:
		str = " Sin ";
		break;
	case OpNLFunc::Cos:
		str = " Cos ";
		break;
	case OpNLFunc::Tan:
		str = " Tan ";
		break;
	default:
		str = " ? ";
		break;
	}
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

void Expression::OpLinExpr::piecewiseProd(OpFloat val)
{
	for (auto& lin : linterm_)
		lin.second *= val;
	constant_ *= val;
}

void Expression::OpLinExpr::piecewiseDiv(OpFloat val)
{
	for (auto& lin : linterm_)
		lin.second /= val;
	constant_ /= val;
}

void Expression::OpLinExpr::piecewiseInv()
{
	for (auto& lin : linterm_)
		lin.second = -lin.second;
	constant_ = -constant_;
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

void Expression::OpQuadExpr::piecewiseProd(OpFloat val)
{
	linexpr_.piecewiseProd(val);
	for (auto& q : quadterm_)
		q.second *= val;
}

void Expression::OpQuadExpr::piecewiseDiv(OpFloat val)
{
	linexpr_.piecewiseDiv(val);
	for (auto& q : quadterm_)
		q.second /= val;
}

void Expression::OpQuadExpr::piecewiseInv()
{
	linexpr_.piecewiseInv();
	for (auto& q : quadterm_)
		q.second = -q.second;
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

/* OPUA::Expression::OpDenseMat */

OpBool Expression::OpDenseMat::isLegal(OpULInt row, OpULInt col) const
{
	return (row < dim_) && (col < dim_);
}

OpULInt Expression::OpDenseMat::coord2Idx(OpULInt row, OpULInt col) const
{
	return row > 0 ? (dim_ * row + col) : col;
}

OpULInt Expression::OpDenseMat::getDim() const
{
	return dim_;
}

OpFloat Expression::OpDenseMat::getVal(OpULInt row, OpULInt col) const
{
	return isLegal(row, col) ? data_[coord2Idx(row, col)] : Constant::NaN;
}

OpFloat& Expression::OpDenseMat::getValRef(OpULInt row, OpULInt col)
{
	return data_.at(isLegal(row, col) ? coord2Idx(row, col) : data_.size());
}

OpFloat Expression::OpDenseMat::trace() const
{
	OpFloat tr(0);
	for (OpULInt i = 0; i < dim_; i++)
		tr += getVal(i, i);
	return tr;
}

OpBool Expression::OpDenseMat::isZeros(OpFloat zero) const
{
	OpBool tmp(true);
	for (auto& c : data_)
	{
		if (std::abs(c) >= zero)
		{
			tmp = false;
			break;
		}
	}
	return tmp;
}

void Expression::OpDenseMat::setDim(OpULInt dim)
{
	dim_ = dim;
	data_.resize(dim_ * dim_, 0);
}

void Expression::OpDenseMat::setVal(OpULInt row, OpULInt col, OpFloat val)
{
	data_.at(coord2Idx(row, col)) = val;
}

void Expression::OpDenseMat::setVal(OpULInt dim, Container::OpFloatArr vals)
{
	setDim(dim);
	for (OpULInt i = 0; i < dim * dim; i++)
		data_[i] = vals[i];
}

void Expression::OpDenseMat::toZeros()
{
	data_ = DMValTab(dim_, 0);
}

void Expression::OpDenseMat::toZeros(OpULInt dim)
{
	dim_ = dim;
	toZeros();
}

void Expression::OpDenseMat::toEyes()
{
	data_ = DMValTab(dim_, 0);
	for (OpULInt i = 0; i < dim_; i++)
		data_[coord2Idx(i, i)] = 1;
}

void Expression::OpDenseMat::toEyes(OpULInt dim)
{
	dim_ = dim;
	toEyes();
}

void Expression::OpDenseMat::toOnes()
{
	data_ = DMValTab(dim_, 1);
}

void Expression::OpDenseMat::toOnes(OpULInt dim)
{
	dim_ = dim;
	toOnes();
}

void Expression::OpDenseMat::piecewiseProd(OpFloat val)
{
	for (auto& c : data_)
		c *= val;
}

void Expression::OpDenseMat::piecewiseDiv(OpFloat val)
{
	for (auto& c : data_)
		c /= val;
}

void Expression::OpDenseMat::piecewiseInv()
{
	for (auto& c : data_)
		c = -c;
}

Expression::OpDenseMat& Expression::OpDenseMat::operator+=(const OpDenseMat& mat)
{
	if (dim_ == mat.dim_)
		for (OpULInt i = 0; i < dim_ * dim_; i++)
			data_[i] += mat.data_[i];
	return *this;
}

Expression::OpDenseMat& Expression::OpDenseMat::operator-=(const OpDenseMat& mat)
{
	if (dim_ == mat.dim_)
		for (OpULInt i = 0; i < dim_ * dim_; i++)
			data_[i] -= mat.data_[i];
	return *this;
}

Expression::OpDenseMat& Expression::OpDenseMat::operator*=(OpFloat val)
{
	for (auto& c : data_)
		c *= val;
	return *this;
}

Expression::OpDenseMat& Expression::OpDenseMat::operator/=(OpFloat val)
{
	for (auto& c : data_)
		c /= val;
	return *this;
}

Expression::OpDenseMat::OpDenseMat()
	: dim_(0),
	data_()
{

}

Expression::OpDenseMat::OpDenseMat(OpULInt dim)
	: dim_(dim),
	data_(dim * dim, 0)
{

}

Expression::OpDenseMat::OpDenseMat(OpULInt dim, Container::OpFloatArr vals)
	: dim_(dim),
	data_()
{
	setVal(dim, vals);
}

/* OPUA::Expression::OpSpraseMat */

OpBool Expression::OpSpraseMat::isLegal(OpULInt row, OpULInt col) const
{
	return (row < dim_) && (col < dim_);
}

OpULInt Expression::OpSpraseMat::coord2Idx(OpULInt row, OpULInt col) const
{
	// 注：默认索引合法
	const auto iter0(col_.begin());
	const auto iter1(iter0 + off_[row]);
	const auto iter2(iter0 + off_[row + 1]);
	// 使用效率更高的二分查找，元素不存在将返回col_.size()
	const auto& iter = std::lower_bound(iter1, iter2, col);
	return (iter == iter2 || (*iter) != col) ? col_.end() - iter0 : iter - iter0;
}

OpULInt Expression::OpSpraseMat::insert(OpULInt row, OpULInt col, OpFloat val)
{
	// 注：默认索引合法且元素不存在
	OpULInt offset(0);
	auto iter0(col_.begin());
	auto iter1(iter0 + off_[row]);
	auto iter2(iter0 + off_[row + 1]);
	// 计算插入位置
	while (iter1 != iter2)
	{
		if ((*iter1) < col)
			++iter1;
		else
			break;
	}
	offset = iter1 - iter0;
	// 调整表
	col_.insert(iter1, col);
	data_.insert(data_.begin() + offset, val);
	for (OpULInt r = row + 1; r < dim_ + 1; r++)
		off_[r] += 1;
	return offset;
}

OpULInt Expression::OpSpraseMat::getDim() const
{
	return dim_;
}

OpULInt Expression::OpSpraseMat::getNZ() const
{
	return col_.size();
}

OpFloat Expression::OpSpraseMat::getVal(OpULInt row, OpULInt col) const
{
	OpFloat tmp(Constant::NaN);
	if (isLegal(row, col))
	{
		OpULInt idx(coord2Idx(row, col));
		if (idx != col_.size())
			tmp = data_[idx];
		else
			tmp = 0;
	}
	return tmp;
}

OpFloat& Expression::OpSpraseMat::getValRef(OpULInt row, OpULInt col)
{
	if (!isLegal(row, col))
		return data_.at(data_.size());
	OpULInt idx(coord2Idx(row, col));
	if (idx == col_.size())
		idx = insert(row, col, 0);
	return data_.at(idx);
}

OpFloat Expression::OpSpraseMat::trace() const
{
	OpFloat tr(0);
	for (OpULInt i = 0; i < dim_; i++)
		tr += getVal(i, i);
	return tr;
}

OpBool Expression::OpSpraseMat::isZeros(OpFloat zero) const
{
	OpBool tmp(true);
	for (auto& c : data_)
	{
		if (std::abs(c) >= zero)
		{
			tmp = false;
			break;
		}
	}
	return tmp;
}

void Expression::OpSpraseMat::setDim(OpULInt dim)
{
	dim_ = dim;
	clear();
}

void Expression::OpSpraseMat::setVal(OpULInt row, OpULInt col, OpFloat val)
{
	getValRef(row, col) = val;
}

void Expression::OpSpraseMat::setVal(OpULInt dim, Container::OpULIntArr rows, Container::OpULIntArr cols, Container::OpFloatArr vals)
{
	setDim(dim);
	OpULInt nonZeros(rows.getSize());
	SMIdxTab row(nonZeros, dim_);
	col_.resize(nonZeros, dim_);
	data_.resize(nonZeros, 0);
	for (OpULInt i = 0; i < nonZeros; i++)
	{
		off_[rows[i]] += 1;
		row[i] = rows[i];
		col_[i] = cols[i];
		data_[i] = vals[i];
	}
	// 排序(先行号再列号从小到大)
	if (nonZeros > 1)
	{
		auto compare = [](auto& a, auto& b, auto i, auto j) {
			return a[i] != a[j] ? a[i] < a[j] : b[i] < b[j];
		};
		auto adjust = [&compare](auto& a, auto& b, auto& c, auto i, auto n) {
			auto parent(i);
			auto child(2 * i + 1);
			while (child < n)
			{
				if (child + 1 < n && compare(a, b, child, child + 1))
					child++;
				if (compare(a, b, parent, child))
				{
					std::swap(a[parent], a[child]);
					std::swap(b[parent], b[child]);
					std::swap(c[parent], c[child]);
					parent = child;
				}
				child = child * 2 + 1;
			}
		};
		for (OpULInt i = nonZeros / 2 - 1; i > 0; i--)
			adjust(row, col_, data_, i, nonZeros);
		adjust(row, col_, data_, 0, nonZeros);
		for (OpULInt i = nonZeros - 1; i > 0; i--)
		{
			std::swap(row[0], row[i]);
			std::swap(col_[0], col_[i]);
			std::swap(data_[0], data_[i]);
			adjust(row, col_, data_, 0, i);
		}
	}
	// 去重(重复项相加)
	OpULInt uniques(1);
	for (OpULInt i = 1; i < nonZeros; i++)
	{
		if ((row[i] != row[i - 1]) || (col_[i] != col_[i - 1]))
		{
			col_[uniques] = col_[i];
			data_[uniques] = data_[i];
			uniques++;
		}
		else
		{
			off_[row[i]] -= 1;
			data_[uniques - 1] += data_[i];
		}
	}
	col_.resize(uniques);
	data_.resize(uniques);
	// 重构off_
	std::swap(off_[0], off_[dim_]);
	for (OpULInt i = 1; i < dim_; i++)
	{
		auto tmp = off_[i];
		off_[i] = off_[i - 1] + off_[dim_];
		off_[dim_] = tmp;
	}
	off_[dim_] += dim_ > 0 ? off_[dim_ - 1] : 0;
}

void Expression::OpSpraseMat::clear()
{
	col_ = SMIdxTab(0);
	off_ = SMIdxTab(dim_ + 1, 0);
	data_ = SMValTab(0);
}

void Expression::OpSpraseMat::toZeros()
{
	clear();
}

void Expression::OpSpraseMat::toZeros(OpULInt dim)
{
	dim_ = dim;
	toZeros();
}

void Expression::OpSpraseMat::toEyes()
{
	clear();
	col_ = SMIdxTab(dim_, 0);
	data_= SMValTab(dim_, 1);
	for (OpULInt i = 0; i < dim_; i++)
	{
		col_[i] = i;
		off_[i + 1] = off_[i] + 1;
	}
}

void Expression::OpSpraseMat::toEyes(OpULInt dim)
{
	dim_ = dim;
	toEyes();
}

void Expression::OpSpraseMat::toOnes()
{
	clear();
	col_ = SMIdxTab(dim_ * dim_, 0);
	data_ = SMValTab(dim_ * dim_, 1);
	OpULInt count(0);
	for (OpULInt i = 0; i < dim_; i++)
	{
		for (OpULInt j = 0; j < dim_; j++)
			col_[count++] = j;
		off_[i + 1] = off_[i] + dim_;
	}
}

void Expression::OpSpraseMat::toOnes(OpULInt dim)
{
	dim_ = dim;
	toOnes();
}

void Expression::OpSpraseMat::piecewiseProd(OpFloat val)
{
	for (auto& c : data_)
		c *= val;
}

void Expression::OpSpraseMat::piecewiseDiv(OpFloat val)
{
	for (auto& c : data_)
		c /= val;
}

void Expression::OpSpraseMat::piecewiseInv()
{
	for (auto& c : data_)
		c = -c;
}

Expression::OpSpraseMat& Expression::OpSpraseMat::operator+=(const OpSpraseMat& mat)
{
	if (dim_ == mat.dim_)
		for (OpULInt i = 0; i < dim_; i++)
			for (OpULInt j = mat.off_[i]; j < mat.off_[i + 1]; j++)
				getValRef(i, mat.col_[j]) += mat.data_[j];
	return *this;
}

Expression::OpSpraseMat& Expression::OpSpraseMat::operator-=(const OpSpraseMat& mat)
{
	if (dim_ == mat.dim_)
		for (OpULInt i = 0; i < dim_; i++)
			for (OpULInt j = mat.off_[i]; j < mat.off_[i + 1]; j++)
				getValRef(i, mat.col_[j]) -= mat.data_[j];
	return *this;
}


Expression::OpSpraseMat& Expression::OpSpraseMat::operator*=(OpFloat val)
{
	for (auto& c : data_)
		c *= val;
	return *this;
}

Expression::OpSpraseMat& Expression::OpSpraseMat::operator/=(OpFloat val)
{
	for (auto& c : data_)
		c /= val;
	return *this;
}

OpBool Expression::OpSpraseMat::OpSMIter::operator==(const OpSMIter& iter) const
{
	return citer_ == iter.citer_ && viter_ == iter.viter_;
}

OpBool Expression::OpSpraseMat::OpSMIter::operator!=(const OpSMIter& iter) const
{
	return citer_ != iter.citer_ || viter_ != iter.viter_;
}

Expression::OpSpraseMat::OpSMIter& Expression::OpSpraseMat::OpSMIter::operator--()
{
	--citer_;
	--viter_;
	return *this;
}

Expression::OpSpraseMat::OpSMIter& Expression::OpSpraseMat::OpSMIter::operator++()
{
	++citer_;
	++viter_;
	return *this;
}

OpULInt Expression::OpSpraseMat::OpSMIter::getCol() const
{
	return *citer_;
}

OpFloat Expression::OpSpraseMat::OpSMIter::getVal() const
{
	return *viter_;
}

Expression::OpSpraseMat::OpSMIter::OpSMIter(SMIdxTerm col, SMValTerm val)
	: citer_(col),
	viter_(val)
{

}

Expression::OpSpraseMat::OpSMIter Expression::OpSpraseMat::getNZBegin(OpULInt row) const
{
	auto citer(col_.begin());
	auto viter(data_.begin());
	std::advance(col_.begin(), off_.at(row));
	std::advance(data_.begin(), off_.at(row));
	return OpSMIter(citer, viter);
}

Expression::OpSpraseMat::OpSMIter Expression::OpSpraseMat::getNZEnd(OpULInt row) const
{
	auto citer(col_.begin());
	auto viter(data_.begin());
	std::advance(col_.begin(), off_.at(row + 1));
	std::advance(data_.begin(), off_.at(row + 1));
	return OpSMIter(citer, viter);
}

Expression::OpSpraseMat::OpSpraseMat()
{
	setDim(0);
}

Expression::OpSpraseMat::OpSpraseMat(OpULInt dim)
{
	setDim(dim);
}

Expression::OpSpraseMat::OpSpraseMat(OpULInt dim, Container::OpULIntArr rows, Container::OpULIntArr cols, Container::OpFloatArr vals)
{
	setVal(dim, rows, cols, vals);
}


/* OPUA::Expression::OpPSDExpr */

OpULInt Expression::OpPSDExpr::getSize() const
{
	return linexpr_.getSize() + psdterm_.size();
}

OpEnv Expression::OpPSDExpr::getEnv() const
{
	OpEnv env(nullptr);
	if (linexpr_.getSize())
		env = linexpr_.getEnv();
	else if (psdterm_.size())
	{
		for (auto& q : psdterm_)
		{
			env = Variable::OpPSDVar(q.first).getEnv();
			break;
		}
	}
	return env;
}

OpFloat Expression::OpPSDExpr::getCoeff(Variable::OpVar var) const
{
	return linexpr_.getCoeff(var);
}

const Expression::OpPSDRealMat& Expression::OpPSDExpr::getCoeff(Variable::OpPSDVar var) const
{
	return psdterm_.at(var.getImpl());
}

Expression::OpLinExpr Expression::OpPSDExpr::getLinTerm() const
{
	return linexpr_.getLinTerm();
}

Expression::OpPSDExpr Expression::OpPSDExpr::getPSDTerm() const
{
	OpPSDExpr tmp(0.0);
	tmp.psdterm_ = psdterm_;
	return tmp;
}

OpFloat Expression::OpPSDExpr::getConstant() const
{
	return linexpr_.getConstant();
}

void Expression::OpPSDExpr::setLinTerm(OpFloat val)
{
	linexpr_.setLinTerm(val);
}

void Expression::OpPSDExpr::setLinTerm(Variable::OpVar var, OpFloat coeff)
{
	linexpr_.setLinTerm(var, coeff);
}

void Expression::OpPSDExpr::setPSDTerm(Variable::OpPSDVar var, const OpPSDRealMat& coeff)
{
	auto impl(var.getImpl());
	auto iter = psdterm_.find(impl);
	if (iter == psdterm_.end())
		psdterm_.emplace(impl, coeff);
	else
		iter->second = coeff;
}

void Expression::OpPSDExpr::addLinTerm(OpFloat val)
{
	linexpr_.addLinTerm(val);
}

void Expression::OpPSDExpr::addLinTerm(Variable::OpVar var, OpFloat coeff)
{
	linexpr_.addLinTerm(var, coeff);
}

void Expression::OpPSDExpr::addPSDTerm(Variable::OpPSDVar var, const OpPSDRealMat& coeff)
{
	auto impl(var.getImpl());
	auto iter = psdterm_.find(impl);
	if (iter == psdterm_.end())
		psdterm_.emplace(impl, coeff);
	else
		iter->second += coeff;
}

void Expression::OpPSDExpr::subPSDTerm(Variable::OpPSDVar var, const OpPSDRealMat& coeff)
{
	auto impl(var.getImpl());
	auto iter = psdterm_.find(impl);
	if (iter == psdterm_.end())
		psdterm_.emplace(impl, coeff).first->second.piecewiseInv();
	else
		iter->second -= coeff;
}

void Expression::OpPSDExpr::removeLinTerm(Variable::OpVar var)
{
	linexpr_.removeVar(var);
}

void Expression::OpPSDExpr::removePSDTerm(Variable::OpPSDVar var)
{
	psdterm_.erase(var.getImpl());
}

void Expression::OpPSDExpr::clear()
{
	linexpr_.clear();
	psdterm_.clear();
}

void Expression::OpPSDExpr::simplify(OpFloat zero)
{
	for (auto it = psdterm_.begin(); it != psdterm_.end();)
	{
		if (it->second.isZeros(zero))
			it = psdterm_.erase(it);
		else
			++it;
	}
	linexpr_.simplify(zero);
}

void Expression::OpPSDExpr::piecewiseProd(OpFloat val)
{
	linexpr_.piecewiseProd(val);
	for (auto& p : psdterm_)
		p.second.piecewiseProd(val);
}

void Expression::OpPSDExpr::piecewiseDiv(OpFloat val)
{
	linexpr_.piecewiseDiv(val);
	for (auto& p : psdterm_)
		p.second.piecewiseDiv(val);
}

void Expression::OpPSDExpr::piecewiseInv()
{
	linexpr_.piecewiseInv();
	for (auto& p : psdterm_)
		p.second.piecewiseInv();
}

Expression::OpPSDExpr& Expression::OpPSDExpr::operator+=(const OpPSDExpr& expr)
{
	linexpr_ += expr.linexpr_;
	for (auto& p : expr.psdterm_)
		addPSDTerm(p.first, p.second);
	return *this;
}

Expression::OpPSDExpr& Expression::OpPSDExpr::operator-=(const OpPSDExpr& expr)
{
	linexpr_ -= expr.linexpr_;
	for (auto& p : expr.psdterm_)
		subPSDTerm(p.first, p.second);
	return *this;
}

Expression::OpPSDExpr& Expression::OpPSDExpr::operator*=(OpFloat val)
{
	linexpr_ *= val;
	for (auto& p : psdterm_)
		p.second *= val;
	return *this;
}

Expression::OpPSDExpr& Expression::OpPSDExpr::operator/=(OpFloat val)
{
	linexpr_ /= val;
	for (auto& p : psdterm_)
		p.second /= val;
	return *this;
}


OpBool Expression::OpPSDExpr::OpPSDEIter::operator==(const OpPSDEIter& iter) const
{
	return iter_ == iter.iter_;
}

OpBool Expression::OpPSDExpr::OpPSDEIter::operator!=(const OpPSDEIter& iter) const
{
	return iter_ != iter.iter_;
}

Expression::OpPSDExpr::OpPSDEIter& Expression::OpPSDExpr::OpPSDEIter::operator--()
{
	--iter_;
	return *this;
}

Expression::OpPSDExpr::OpPSDEIter& Expression::OpPSDExpr::OpPSDEIter::operator++()
{
	++iter_;
	return *this;
}

Variable::OpPSDVar Expression::OpPSDExpr::OpPSDEIter::getVar() const
{
	return Variable::OpPSDVar(iter_->first);
}

const Expression::OpPSDRealMat& Expression::OpPSDExpr::OpPSDEIter::getCoeff() const
{
	return iter_->second;
}

Expression::OpPSDExpr::OpPSDEIter::OpPSDEIter(PSDTerm iter)
	: iter_(iter)
{

}

Expression::OpLinExpr::OpLEIter Expression::OpPSDExpr::getLBegin() const
{
	return linexpr_.getLBegin();
}

Expression::OpLinExpr::OpLEIter Expression::OpPSDExpr::getLEnd() const
{
	return linexpr_.getLEnd();
}

Expression::OpPSDExpr::OpPSDEIter Expression::OpPSDExpr::getPSDBegin() const
{
	return OpPSDEIter(psdterm_.begin());
}

Expression::OpPSDExpr::OpPSDEIter Expression::OpPSDExpr::getPSDEnd() const
{
	return OpPSDEIter(psdterm_.end());
}

Expression::OpPSDExpr::OpPSDExpr(OpFloat constant)
	: linexpr_(constant)
{

}

Expression::OpPSDExpr::OpPSDExpr(Variable::OpVar var, OpFloat coeff)
	: linexpr_(var, coeff)
{

}

Expression::OpPSDExpr::OpPSDExpr(const OpLinExpr& expr)
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

OpFloat Expression::OpNLExpr::getParam() const
{
	return param_;
}

Variable::OpVar Expression::OpNLExpr::getVar(OpULInt order) const
{
	return Variable::OpVar(nlterm_.at(order));
}

void Expression::OpNLExpr::setFunction(OpNLFunc func)
{
	func_ = func;
}

void Expression::OpNLExpr::setParam(OpFloat param)
{
	param_ = param;
}

void Expression::OpNLExpr::addVar(Variable::OpVar var)
{
	nlterm_.emplace_back(var.getImpl());
}

void Expression::OpNLExpr::addVar(Variable::OpVarArr vars)
{
	nlterm_.reserve(nlterm_.size() + vars.getSize());
	for (OpULInt i = 0; i < vars.getSize(); i++)
		nlterm_.emplace_back(vars[i].getImpl());
}

void Expression::OpNLExpr::removeVar(Variable::OpVar var)
{
	nlterm_.erase(std::remove(nlterm_.begin(), nlterm_.end(), var.getImpl()), nlterm_.end());
}

void Expression::OpNLExpr::removeVar(Variable::OpVarArr vars)
{
	for (OpULInt i = 0; i < vars.getSize(); i++)
		removeVar(vars[i]);
}

void Expression::OpNLExpr::removeVar(OpULInt n)
{
	auto pos1 = nlterm_.begin();
	auto pos2 = nlterm_.end();
	std::advance(pos1, nlterm_.size() > n ? nlterm_.size() - n : 0);
	if (pos1 != pos2)
		nlterm_.erase(pos1, pos2);
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

Expression::OpNLExpr::OpNLExpr(OpNLFunc func, OpFloat param, Variable::OpVarArr vars)
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