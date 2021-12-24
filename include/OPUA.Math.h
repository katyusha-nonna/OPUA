#pragma once
#include "OPUA.Container.h"
#include "OPUA.Constant.h"

namespace OPUA
{
	namespace Math
	{
		OpFloat OpMax(OpFloat val1, OpFloat val2); // 较大值
		OpFloat OpMax(Container::OpFloatArr vals); // 较大值
		OpFloat OpMin(OpFloat val1, OpFloat val2); // 较小值
		OpFloat OpMin(Container::OpFloatArr vals); // 较大值
		OpFloat OpSum(Container::OpFloatArr vals); // 求和
	}
}