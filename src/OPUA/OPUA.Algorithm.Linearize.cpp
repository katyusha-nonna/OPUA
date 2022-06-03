#include "OPUA.Algorithm.h"
#include "OPUA.Math.h"
#include <algorithm>
#include <math.h>

using namespace OPUA;

void Algorithm::OpAlgoBigMBilinear1(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y, OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax)
{
	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;

	auto& auxVars = result.first;
	auto& auxCons = result.second;
	OpVar r(env, OpVarType::Con, ymin, ymax); // r
	auxVars.add(r);
	auxCons.add(OpLinCon(env, 0.0, r + w - y, 0.0)); // r = y - w
	auxCons.add(OpLinCon(env, 0.0, y - r - ymin * x, Constant::Infinity)); // y - r >= ymin * x
	auxCons.add(OpLinCon(env, 0.0, -y + r + ymax * x, Constant::Infinity)); // y - r <= ymax * x
	auxCons.add(OpLinCon(env, ymin, r + ymin * x, Constant::Infinity)); // r >= ymin * (1 - x)
	auxCons.add(OpLinCon(env, -ymax, -r - ymax * x, Constant::Infinity)); // r <= ymax * (1 - x)
}

Algorithm::OpLinearization Algorithm::OpAlgoBigMBilinear1(OpEnv env, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y, OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax)
{
	OpLinearization result = { Variable::OpVarArr(env), Constraint::OpLCArr(env) };
	OpAlgoBigMBilinear1(env, result, w, x, y, xmin, xmax, ymin, ymax);
	return result;
}

void Algorithm::OpAlgoBigMBilinear2(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y, OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax, OpULInt segment)
{
	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;

	segment = segment > 2 ? segment : 2;
	auto& auxVars = result.first;
	auto& auxCons = result.second;
	OpFloat delta((xmax - xmin) / (OpFloat)(std::pow((OpULInt)2, segment) -1)); // 分辨率
	std::vector<OpULInt> weights(segment, 1); // 二进制位权重
	std::vector<OpVar> u(segment); // 辅助变量
	std::vector<OpVar> prods(segment); // 乘积项b_{k}*y
	for (OpULInt k = 1; k < segment; k++)
		weights[k] = weights[k - 1] * 2;
	for (OpULInt k = 0; k < segment; k++)
	{
		u[k] = Variable::OpVar(env, OpVarType::Bool, 0.0, 1.0);
		auxVars.add(u[k]);
	}
	for (OpULInt k = 0; k < segment; k++)
	{
		prods[k] = Variable::OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity);
		auxVars.add(prods[k]);
	}
	// 形成w的二进制表达式
	{
		OpLinExpr wExpr(0.0);
		wExpr.addLinTerm(y, xmin);
		for (OpULInt k = 0; k < segment; k++)
			wExpr.addLinTerm(prods[k], delta * weights[k]);
		auxCons.add(OpLinCon(env, 0.0, wExpr - w, 0.0));
	}
	// 形成x的二进制表达式
	{
		OpLinExpr xExpr(xmin);
		for (OpULInt k = 0; k < segment; k++)
			xExpr.addLinTerm(u[k], delta * weights[k]);
		auxCons.add(OpLinCon(env, 0.0, xExpr - x, 0.0));
	}
	// 调用OpAlgoBigMBilinear1对所有双线性乘积项分别进行线性化
	for (OpULInt k = 0; k < segment; k++)
		OpAlgoBigMBilinear1(env, result, prods[k], u[k], y, 0.0, 1.0, ymin, ymax);
}

Algorithm::OpLinearization Algorithm::OpAlgoBigMBilinear2(OpEnv env, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y, OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax, OpULInt segment)
{
	OpLinearization result = { Variable::OpVarArr(env), Constraint::OpLCArr(env) };
	OpAlgoBigMBilinear2(env, result, w, x, y, xmin, xmax, ymin, ymax, segment);
	return result;
}

void Algorithm::OpAlgoBigMAbs(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, OpFloat xamax)
{
	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;

	auto& auxVars = result.first;
	auto& auxCons = result.second;
	OpVar xp(env, OpVarType::Con, 0.0, xamax);
	OpVar xn(env, OpVarType::Con, 0.0, xamax);
	OpVar u(env, OpVarType::Bool, 0.0, 1.0); // u
	auxVars.add(xp);
	auxVars.add(xn);
	auxVars.add(u);
	auxCons.add(OpLinCon(env, 0.0, xp + xn - w, 0.0)); // w = xp + xn
	auxCons.add(OpLinCon(env, 0.0, xp - xn - x, 0.0)); // x = xp - xn
	auxCons.add(OpLinCon(env, 0.0, -xp + xamax * u, Constant::Infinity)); // xp <= M * u
	auxCons.add(OpLinCon(env, -xamax, -xn - xamax * u, Constant::Infinity)); // xn <= M * (1 - u)
}

Algorithm::OpLinearization Algorithm::OpAlgoBigMAbs(OpEnv env, Variable::OpVar w, Variable::OpVar x, OpFloat xamax)
{
	OpLinearization result = { Variable::OpVarArr(env), Constraint::OpLCArr(env) };
	OpAlgoBigMAbs(env, result, w, x, xamax);
	return result;
}

void Algorithm::OpAlgoBigMMax(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y, OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax)
{
	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;

	OpFloat vmax = xmax > ymax ? xmax : ymax; // M1 = vmax - xmin, M2 = vmax - ymin
	auto& auxVars = result.first;
	auto& auxCons = result.second;
	OpVar u(env, OpVarType::Bool, 0.0, 1.0); // u
	auxVars.add(u);
	auxCons.add(OpLinCon(env, 0.0, w - x, Constant::Infinity)); // w >= x
	auxCons.add(OpLinCon(env, 0.0, w - y, Constant::Infinity)); // w >= y
	auxCons.add(OpLinCon(env, 0.0, x - w + (vmax - xmin) * u, Constant::Infinity)); // w <= x + M1 * u
	auxCons.add(OpLinCon(env, ymin - vmax, y - w + (ymin - vmax) * u, Constant::Infinity)); // w <= y + M2 * (1 - u)
}

Algorithm::OpLinearization Algorithm::OpAlgoBigMMax(OpEnv env, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y, OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax)
{
	OpLinearization result = { Variable::OpVarArr(env), Constraint::OpLCArr(env) };
	OpAlgoBigMMax(env, result, w, x, y, xmin, xmax, ymin, ymax);
	return result;
}

void Algorithm::OpAlgoBigMMax(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVarArr X, Container::OpFloatArr Xmin, Container::OpFloatArr Xmax)
{
	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;

	const auto N = X.getSize();
	//assert(X.getSize() == Xmin.getSize());
	//assert(X.getSize() == Xmax.getSize());
	OpFloat vmax = Math::OpMax(Xmax); // Mi = vmax - Xmin_i
	auto& auxVars = result.first;
	auto& auxCons = result.second;
	std::vector<OpVar> u(N);
	for (OpULInt i = 0; i < N; i++)
	{
		u[i] = Variable::OpVar(env, OpVarType::Bool, 0.0, 1.0);
		auxVars.add(u[i]);
	}
	for (OpULInt i = 0; i < N; i++)
		auxCons.add(OpLinCon(env, 0.0, w - X[i], Constant::Infinity)); // w >= Xi
	for (OpULInt i = 0; i < N; i++)
		auxCons.add(OpLinCon(env, Xmin[i] - vmax, X[i] - w + (Xmin[i] - vmax) * u[i], Constant::Infinity)); // w <= Xi + Mi * (1 - ui)
	{
		OpLinExpr uSum(0.0);
		for (OpULInt i = 0; i < N; i++)
			uSum.addLinTerm(u[i], 1.0);
		auxCons.add(OpLinCon(env, 1.0, uSum, Constant::Infinity)); // Sum(u) >= 1
	}
}

Algorithm::OpLinearization Algorithm::OpAlgoBigMMax(OpEnv env, Variable::OpVar w, Variable::OpVarArr X, Container::OpFloatArr Xmin, Container::OpFloatArr Xmax)
{
	OpLinearization result = { Variable::OpVarArr(env), Constraint::OpLCArr(env) };
	OpAlgoBigMMax(env, result, w, X, Xmin, Xmax);
	return result;
}

void Algorithm::OpAlgoBigMMin(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y, OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax)
{
	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;

	OpFloat vmin = xmin < ymin ? xmin : ymin; // M1 = xmax - vmin, M2 = ymax - vmin
	auto& auxVars = result.first;
	auto& auxCons = result.second;
	OpVar u(env, OpVarType::Bool, 0.0, 1.0); // u
	auxVars.add(u);
	auxCons.add(OpLinCon(env, 0.0, x - w, Constant::Infinity)); // w <= x
	auxCons.add(OpLinCon(env, 0.0, y - w, Constant::Infinity)); // w <= y
	auxCons.add(OpLinCon(env, 0.0, w - x + (xmax - vmin) * u, Constant::Infinity)); // w >= x - M1 * u
	auxCons.add(OpLinCon(env, vmin - ymax, w - y + (vmin - ymax) * u, Constant::Infinity)); // w >= y - M2 * (1 - u)
}

Algorithm::OpLinearization Algorithm::OpAlgoBigMMin(OpEnv env, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y, OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax)
{
	OpLinearization result = { Variable::OpVarArr(env), Constraint::OpLCArr(env) };
	OpAlgoBigMMin(env, result, w, x, y, xmin, xmax, ymin, ymax);
	return result;
}

void Algorithm::OpAlgoBigMMin(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVarArr X, Container::OpFloatArr Xmin, Container::OpFloatArr Xmax)
{
	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;

	const auto N = X.getSize();
	//assert(X.getSize() == Xmin.getSize());
	//assert(X.getSize() == Xmax.getSize());
	OpFloat vmin = Math::OpMin(Xmin); // Mi = Xmax_i - vmin
	auto& auxVars = result.first;
	auto& auxCons = result.second;
	std::vector<OpVar> u(N);
	for (OpULInt i = 0; i < N; i++)
	{
		u[i] = Variable::OpVar(env, OpVarType::Bool, 0.0, 1.0);
		auxVars.add(u[i]);
	}
	for (OpULInt i = 0; i < N; i++)
		auxCons.add(OpLinCon(env, 0.0, X[i] - w, Constant::Infinity)); // w <= Xi
	for (OpULInt i = 0; i < N; i++)
		auxCons.add(OpLinCon(env, vmin - Xmax[i], w - X[i] + (vmin - Xmax[i]) * u[i], Constant::Infinity)); // w >= Xi - Mi * (1 - ui)
	{
		OpLinExpr uSum(0.0);
		for (OpULInt i = 0; i < N; i++)
			uSum.addLinTerm(u[i], 1.0);
		auxCons.add(OpLinCon(env, 1.0, uSum, Constant::Infinity)); // Sum(u) >= 1
	}
}

Algorithm::OpLinearization Algorithm::OpAlgoBigMMin(OpEnv env, Variable::OpVar w, Variable::OpVarArr X, Container::OpFloatArr Xmin, Container::OpFloatArr Xmax)
{
	OpLinearization result = { Variable::OpVarArr(env), Constraint::OpLCArr(env) };
	OpAlgoBigMMin(env, result, w, X, Xmin, Xmax);
	return result;
}
