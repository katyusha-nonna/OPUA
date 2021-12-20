#include "OPUA.Common.h"

OPUA::OpDisplay& OPUA::OpDisplay::newLine()
{
	std::cout << std::endl;
	if (files)
		(*files) << std::endl;
	else
	{
		std::ofstream fs(file, std::ios::app | std::ios::out);
		fs << std::endl;
	}
	return *this;
}
