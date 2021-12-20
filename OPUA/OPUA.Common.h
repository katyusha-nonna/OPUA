#pragma once

#include "OPUA.Type.h"
#include <fstream>
#include <iostream>
#include <functional>

namespace OPUA
{
	// ����ͬʱ���������̨���ļ�����
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

	// �뿪������ʱ�Զ������ڴ�
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
