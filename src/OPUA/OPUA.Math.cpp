#include "OPUA.Math.h"
#include "OPUA.Exception.h"

#ifdef OPUA_COMPILE_EIGEN
//矩阵运算包
#include <Eigen>
#endif

#include <random>
#include <set>

using namespace OPUA;

OpFloat Math::OpMax(OpFloat val1, OpFloat val2)
{
	return val1 > val2 ? val1 : val2;
}

OpFloat Math::OpMax(Container::OpFloatArr vals)
{
	OpFloat result(-Constant::Infinity);
	for (auto it = vals.getCBegin(); it != vals.getCEnd(); ++it)
		if (it.getVal() > result)
			result = it.getVal();
	return result;
}

OpFloat Math::OpMin(OpFloat val1, OpFloat val2)
{
	return val1 < val2 ? val1 : val2;
}

OpFloat Math::OpMin(Container::OpFloatArr vals)
{
	OpFloat result(Constant::Infinity);
	for (auto it = vals.getCBegin(); it != vals.getCEnd(); ++it)
		if (it.getVal() < result)
			result = it.getVal();
	return result;
}

OpFloat Math::OpSum(Container::OpFloatArr vals)
{
	OpFloat result(0.0);
	for (auto it = vals.getCBegin(); it != vals.getCEnd(); ++it)
		result += it.getVal();
	return result;
}

void Math::OpMathQPWLF(Container::OpFloatArr param, OpFloat xmin, OpFloat xmax, OpFloat a, OpFloat b, OpFloat c, OpULInt segment)
{
	if (xmin > xmax)
		throw OpExcBase("[OPUA::Math::OpMathQPWLF]Exception: xmin > xmax");
	param.setSize(0);
	param.setSize(segment * 2);
	OpFloat delta = (xmax - xmin) / segment;
	OpFloat x1(0.0), x2(0.0), y1(0.0), y2(0.0);
	for (OpULInt i = 0; i < segment; i++)
	{
		x1 = xmin + i * delta;
		x2 = x1 + delta;
		y1 = (a * x1 + b) * x1 + c;
		y2 = (a * x2 + b) * x2 + c;
		param[i] = (y2 - y1) / delta;
		param[i + 1] = (x2 * y1 - x1 * y2) / delta;
	}
}

Container::OpFloatArr Math::OpMathQPWLF(OpEnv env, OpFloat xmin, OpFloat xmax, OpFloat a, OpFloat b, OpFloat c, OpULInt segment)
{
	Container::OpFloatArr result(env);
	OpMathQPWLF(result, xmin, xmax, a, b, c, segment);
	return result;
}

#ifdef OPUA_COMPILE_EIGEN
void Math::OpMathMCPWLF(Container::OpFloatArr param, Container::OpFloatArr X, Container::OpFloatArr Y, OpULInt numPoint, OpULInt lenPoint, OpULInt segment, OpULInt iterMax, OpBool randInit)
{
	param.setSize(0);
	typedef Eigen::Map<Eigen::VectorXd> EMVx;
	typedef Eigen::VectorXd EVx;
	typedef Eigen::MatrixXd EMx;
	// Step1 初始化
	std::vector<std::vector<OpFloat> > result(segment);
	for (auto& c : result)
		c.resize(lenPoint);
	std::vector<std::vector<OpFloat> > points(numPoint);
	for (OpULInt i = 0; i < numPoint; i++)
	{
		auto& v(points[i]);
		v.reserve(lenPoint);
		OpULInt j0(i * lenPoint);
		for (OpULInt j = 0; j < lenPoint; j++)
			v[j] = X[j + j0];
	}
	std::vector<std::set<OpULInt> > Pm(segment);
	std::vector<OpBool> valid(segment, true);
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_int_distribution<OpULInt> dis(0, segment - 1);
	for (OpULInt n = 0; n < numPoint; n++)
		Pm[dis(engine)].emplace(n);
	OpULInt iter(0);
	OpBool flag1(false), flag2(true);
	while (!flag1)
	{
		// Step2 计算最小二乘估计
		for (OpULInt m = 0; m < segment; m++)
		{
			EMx A(EMx::Zero(lenPoint, lenPoint));
			EVx b(EVx::Zero(lenPoint));
			EMVx c(result[m].data(), lenPoint);
			for (auto& n : Pm[m])
			{
				EMVx x(points[n].data(), lenPoint);
				A += x * x.transpose();
				b += Y[n] * x;
			}
			if (valid[m])
				c = A.colPivHouseholderQr().solve(b);
			else
				c.setZero();
		}
		// Step3 更新Pm
		flag2 = true;
		for (OpULInt n = 0; n < numPoint; n++)
		{
			EMVx x(points[n].data(), lenPoint);
			std::vector<OpFloat> dot(segment);
			for (OpULInt m = 0; m < segment; m++)
			{
				if (valid[m])
					dot[m] = EMVx(result[m].data(), lenPoint).dot(x);
				else
					dot[m] = -Constant::Infinity;
			}
			auto i = std::max_element(dot.begin(), dot.end()) - dot.begin();
			if (Pm[i].find(n) == Pm[i].end())
			{
				for (auto& P : Pm)
					P.erase(n);
				Pm[i].emplace(n);
				flag2 = false;
			}
		}
		for (OpULInt m = 0; m < segment; m++)
			if (Pm[m].size() > 0)
				valid[m] = true;
			else
				valid[m] = false;
		// Step4 判断收敛
		if (iter++ > iterMax)
			flag1 = true;
		if (flag2)
			flag1 = true;
	}
	OpULInt validNum(0);
	for (auto& flag : valid)
		if (flag)
			validNum++;
	if (validNum == 0)
		return;
	param.setSize(lenPoint * validNum);
	validNum = 0;
	for (OpULInt i = 0; i < segment; i++)
	{
		if (valid[i])
		{
			OpULInt j0(validNum * lenPoint);
			for (OpULInt j = 0; j < lenPoint; j++)
				param[j + j0] = result[i][j];
			validNum++;
		}
	}
}

Container::OpFloatArr Math::OpMathMCPWLF(OpEnv env, Container::OpFloatArr X, Container::OpFloatArr Y, OpULInt numPoint, OpULInt lenPoint, OpULInt segment, OpULInt iterMax, OpBool randInit)
{
	Container::OpFloatArr result(env);
	OpMathMCPWLF(result, X, Y, numPoint, lenPoint, segment, iterMax, randInit);
	return result;
}
#endif
