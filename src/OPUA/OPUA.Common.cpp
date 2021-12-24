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

OPUA::OpBool OPUA::OpTimer::startup() const
{
	return startup_;
}

void OPUA::OpTimer::start()
{
	if (!startup_)
	{
		start_ = TClock::now();
		startup_ = true;
	}
}

void OPUA::OpTimer::reset()
{
	es_ = 0;
	startup_ = false;
}

void OPUA::OpTimer::pause()
{
	if (startup_)
	{
		es_ = second();
		startup_ = false;
	}
}

void OPUA::OpTimer::restart()
{
	reset();
	start();
}

OPUA::OpFloat OPUA::OpTimer::msecond() const
{
	return second() * 1000;
}

OPUA::OpFloat OPUA::OpTimer::second() const
{
	return es_ + std::chrono::duration_cast<std::chrono::duration<OpFloat> >(TClock::now() - start_).count();
}

OPUA::OpTimer::OpTimer(OpBool startup)
	: startup_(false)
{
	if (startup)
		restart();
}


