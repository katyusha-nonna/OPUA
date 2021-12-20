#include "OPUA.Exception.h"

const char* OPUA::OpExcBase::what() const throw()
{
	return msg_.c_str();
}

OPUA::OpExcBase::OpExcBase()
	: msg_("OPUA_DEFAULT_EXC")
{

}

OPUA::OpExcBase::OpExcBase(OpStr message)
	: msg_(message)
{

}

OPUA::OpExcBase::~OpExcBase() throw()
{

}
