#pragma once

#include "OPUA.Type.h"
#include <exception>

namespace OPUA
{
	// 此项目中所有用户定义异常的基类。
	class OpExcBase
		: public std::exception 
	{
	protected:
		OpStr msg_;
	public:
		const OpChar* what() const throw() override;
	public:
		OpExcBase();
		explicit OpExcBase(OpStr message);
		~OpExcBase() throw () override;		
	};
}