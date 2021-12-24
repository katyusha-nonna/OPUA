#include "OPUA.Math.h"

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
