#include "OPUA.Common.h"
#include "OPUA.Exception.h"
#include <sstream>
#include <fstream>
//#include <experimental/filesystem>

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

void OPUA::OpCfgParser::Load(std::istream& inputStream)
{
	//加载中的缓冲区
	std::string buffer{};
	std::stringstream ss{};
	size_t lineNumber = 0;
	while (getline(inputStream, buffer))
	{
		lineNumber++;
		ss.clear();
		ss.str(buffer);
		std::string key{};
		char ch;
		if (!(ss >> key)) continue;
		if (key[0] == '#') continue;
		if (!(ss >> ch) || ch != '=')
			throw OpExcBase("[OPUA::OpCfgParser::Load]invalid config: line " + std::to_string(lineNumber) + "。");
		std::string value{};
		if (!(ss >> value))
			throw OpExcBase("[OPUA::OpCfgParser::Load]invalid config: line " + std::to_string(lineNumber) + "。");
		// ISSUE 目前配置值中不能包含空格，否则会在空格处截断。
		entries[key] = value;
	}
}

OPUA::OpStr OPUA::OpCfgParser::getStr(const OpStr& key, const OpStr& defaultValue) const
{
	auto v = entries.find(key);
	if (v == entries.end()) return defaultValue;
	return v->second;
}

OPUA::OpChar OPUA::OpCfgParser::getChr(const OpStr& key, const OpChar& defaultValue) const
{
	return defaultValue;
}

OPUA::OpInt OPUA::OpCfgParser::getInt(const OpStr& key, OpInt defaultValue) const
{
	auto v = entries.find(key);
	if (v == entries.end()) return defaultValue;
	try
	{
		return std::stoi(v->second);
	}
	catch (const std::exception&)
	{
		std::throw_with_nested(OpExcBase("[OPUA::OpCfgParser::getInt]can not convert to OpInt: " + key));
	}
}

OPUA::OpLInt OPUA::OpCfgParser::getLInt(const OpStr& key, OpLInt defaultValue) const
{
	auto v = entries.find(key);
	if (v == entries.end()) return defaultValue;
	try
	{
		return std::stoll(v->second);
	}
	catch (const std::exception&)
	{
		std::throw_with_nested(OpExcBase("[OPUA::OpCfgParser::getLInt]can not convert to OpLInt: " + key));
	}
}

OPUA::OpFloat OPUA::OpCfgParser::getFloat(const OpStr& key, OpFloat defaultValue) const
{
	auto v = entries.find(key);
	if (v == entries.end()) return defaultValue;
	try
	{
		return std::stod(v->second);
	}
	catch (const std::exception&)
	{
		std::throw_with_nested(OpExcBase("[OPUA::OpCfgParser::getFloat]can not convert to OpFloat: " + key));
	}
}

OPUA::OpBool OPUA::OpCfgParser::getBool(const OpStr& key, OpBool defaultValue) const
{
	auto v = entries.find(key);
	if (v == entries.end()) return defaultValue;
	try
	{
		return OPUA::OpBool(std::stoll(v->second));
	}
	catch (const std::exception&)
	{
		std::throw_with_nested(OpExcBase("[OPUA::OpCfgParser::getBool]can not convert to OpBool: " + key));
	}
}

OPUA::OpCfgParser::OpCfgParser(std::istream& inputStream)
	: entries()
{
	Load(inputStream);
}

template <typename TStream, typename TPath>
TStream OpenAndValidate(const TPath arg1)
{
	auto fs = TStream(arg1);
	if (!fs) 
	{
		throw OPUA::OpExcBase("[OPUA::OpenAndValidate]can not open");
	}
	return fs;
}

OPUA::OpCfgParser::OpCfgParser(std::experimental::filesystem::v1::path filePath)
	: entries()
{
	auto ifs = OpenAndValidate<std::ifstream>(filePath);
	Load(ifs);
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


