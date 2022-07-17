#include "OPUA.Constant.h"

using namespace OPUA;

OpBool OPUA::Constant::IsPInfinity(OpFloat val)
{
	return IsEqual(val, Constant::Infinity) || val > Constant::Infinity || isinf(val);
}

OpBool OPUA::Constant::IsNInfinity(OpFloat val)
{
	return IsEqual(val, -Constant::Infinity) || val < -Constant::Infinity || isinf(val);
}

OpBool Constant::IsInfinity(OpFloat val)
{
	//return val >= Constant::Infinity || val <= -Constant::Infinity;
	return IsPInfinity(val) || IsNInfinity(val);
}

OpBool Constant::IsEqual(OpFloat val1, OpFloat val2)
{
	double tmp(val1 - val2);
	return -Constant::AbsEpsilon <= tmp && tmp <= Constant::AbsEpsilon;
}

OpBool OPUA::Constant::IsNaN(OpFloat val)
{
	return isnan(val);
}
