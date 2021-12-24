#pragma once
#include "OPUA.Type.h"

namespace OPUA
{
	namespace Constant
	{
		constexpr OpFloat Pi = 3.1415926535897932; /*圆周率\pi*/
		constexpr OpFloat HalfPi = 1.5707963267948966;  /*半圆周率\pi*/
		constexpr OpFloat NL = 2.7182818284590452;  /*自然对数的底e*/
		constexpr OpFloat Infinity = 1e100; /*无穷大*/
		constexpr OpFloat AbsEpsilon = 1e-12; /*绝对极小值(近似为0)*/
		constexpr OpFloat RelEpsilon = 1e-80; /*相对极小值(近似为0)*/
		constexpr OpFloat NaN = NAN; /*Not a Number*/
		constexpr OpULInt MaxSize = -1; /*索引最大值(无符号长整型最大数)*/

		OpBool IsInfinity(OpFloat val); // 是否为无穷大数
		OpBool IsEqual(OpFloat val1, OpFloat val2); // 是否相等
	}
}
