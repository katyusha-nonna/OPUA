#pragma once
#include "OPUA.Type.h"

namespace OPUA
{
	namespace Constant
	{
		constexpr OpFloat Pi = 3.1415926535897932; /*Բ����\pi*/
		constexpr OpFloat HalfPi = 1.5707963267948966;  /*��Բ����\pi*/
		constexpr OpFloat NL = 2.7182818284590452;  /*��Ȼ�����ĵ�e*/
		constexpr OpFloat Infinity = 1e100; /*�����*/
		constexpr OpFloat AbsEpsilon = 1e-12; /*���Լ�Сֵ(����Ϊ0)*/
		constexpr OpFloat RelEpsilon = 1e-80; /*��Լ�Сֵ(����Ϊ0)*/
		constexpr OpFloat NaN = NAN; /*Not a Number*/
		constexpr OpULInt MaxSize = -1; /*�������ֵ(�޷��ų����������)*/

		OpBool IsInfinity(OpFloat val); // �Ƿ�Ϊ�������
		OpBool IsEqual(OpFloat val1, OpFloat val2); // �Ƿ����
	}
}
