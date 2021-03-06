#pragma once

#include "OPUA.Type.h"
#include <fstream>
#include <iostream>
#include <functional>
#include <chrono>

namespace OPUA
{
	// 用于同时输出到控制台和文件的类
	class OpDisplay
	{
	private:
		OpStr file;
		std::ofstream* files;
	public:
		template<typename T>
		OpDisplay& operator <<(const T* str)
		{
			std::cout << str;
			if (files)
				(*files) << str;
			else
			{
				std::ofstream fs(file, std::ios::app | std::ios::out);
				fs << str;
			}
			return *this;
		}
		template<typename T>
		OpDisplay& operator <<(T str)
		{
			std::cout << str;
			if (files)
				(*files) << str;
			else
			{
				std::ofstream fs(file, std::ios::app | std::ios::out);
				fs << str;
			}
			return *this;
		}
		OpDisplay& newLine();
	public:
		OpDisplay(OpStr ifile)
			: file(ifile), files(nullptr) {}
		OpDisplay(std::ofstream& ifiles)
			: files(&ifiles), file("") {}
	};

	// 计时器(支持中途暂停计时和恢复计时)
	// 关于STL库中chrono的用法，详见 https://en.cppreference.com/w/cpp/header/chrono
	class OpTimer
	{
	protected:
		using TClock = std::chrono::high_resolution_clock;

		std::chrono::time_point<TClock> start_;
		OpFloat es_;
		OpBool startup_;
	public:
		OpBool startup() const; // 是否启动
		void start(); // 启动计时/恢复计时
		void reset(); // 重置计时
		void pause(); // 暂停计时
		void restart(); // 重启计时(=reset+start)
		OpFloat msecond() const; // 时长(毫秒计)
		OpFloat second() const; // 时长(秒计)
	public:
		OpTimer(OpBool startup = true); // 构造函数(默认自动计时)
	};

	// 离开作用域时自动清理内存
	class OpAutoFree
	{
	protected:
		std::function<void()> cleaner_;
	public:
		explicit OpAutoFree(const std::function<void()>& cleaner) : cleaner_(cleaner) {}
		OpAutoFree(const OpAutoFree&) = delete;
		OpAutoFree& operator=(const OpAutoFree&) = delete;
		~OpAutoFree() throw()
		{
			try
			{
				cleaner_();
			}
			catch (std::exception& e)
			{
				std::cerr << "OpAutoFree: " << e.what() << std::endl;
			}
		}
	};
}
