#pragma once
#include "OPUA.Type.h"

namespace OPUA
{
	namespace Constant
	{
		constexpr OpFloat Pi = 3.1415926535897932; /*圆周率\pi*/
		constexpr OpFloat HalfPi = 1.5707963267948966;  /*半圆周率\pi*/
		constexpr OpFloat Infinity = 1e100; /*无穷大*/
		constexpr OpFloat AbsEpsilon = 1e-10; /*绝对极小值(近似为0)*/
		constexpr OpFloat RelEpsilon = 1e-80; /*相对极小值(近似为0)*/
	}
}
