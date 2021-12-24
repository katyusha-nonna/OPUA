#include "OPUA.Constant.h"

using namespace OPUA;

OpBool Constant::IsInfinity(OpFloat val)
{
	//return val >= Constant::Infinity || val <= -Constant::Infinity;
	return IsEqual(val, Constant::Infinity) || IsEqual(val, -Constant::Infinity);
}

OpBool Constant::IsEqual(OpFloat val1, OpFloat val2)
{
	double tmp(val1 - val2);
	return -Constant::AbsEpsilon <= tmp && tmp <= Constant::AbsEpsilon;
}
