#pragma once

#include "OPUA.Type.h"
#include <fstream>
#include <iostream>
#include <functional>
#include <chrono>

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

	// ��ʱ��(֧����;��ͣ��ʱ�ͻָ���ʱ)
	// ����STL����chrono���÷������ https://en.cppreference.com/w/cpp/header/chrono
	class OpTimer
	{
	protected:
		using TClock = std::chrono::high_resolution_clock;

		std::chrono::time_point<TClock> start_;
		OpFloat es_;
		OpBool startup_;
	public:
		OpBool startup() const; // �Ƿ�����
		void start(); // ������ʱ/�ָ���ʱ
		void reset(); // ���ü�ʱ
		void pause(); // ��ͣ��ʱ
		void restart(); // ������ʱ(=reset+start)
		OpFloat msecond() const; // ʱ��(�����)
		OpFloat second() const; // ʱ��(���)
	public:
		OpTimer(OpBool startup = true); // ���캯��(Ĭ���Զ���ʱ)
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
