#pragma once

#include "OPUA.Type.h"
#include <exception>

namespace OPUA
{
	// ����Ŀ�������û������쳣�Ļ��ࡣ
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