#pragma once
#include "OPUA.Container.h"
#include "OPUA.Constant.h"

namespace OPUA
{
	namespace Math
	{
		OpFloat OpMax(OpFloat val1, OpFloat val2); // �ϴ�ֵ
		OpFloat OpMax(Container::OpFloatArr vals); // �ϴ�ֵ
		OpFloat OpMin(OpFloat val1, OpFloat val2); // ��Сֵ
		OpFloat OpMin(Container::OpFloatArr vals); // �ϴ�ֵ
		OpFloat OpSum(Container::OpFloatArr vals); // ���
	}
}