#pragma once

#include "OPUA.Type.h"

namespace OPUA
{
	class OpEnvI;
	class OpEnv;
	class OpImplBase;

	// OpEnv：OPUA环境变量
	// 负责所有组件的内存管理
	class OpEnv
	{
	protected:
		OpEnvI* impl_;
	public:
		void release(); // 释放所有模型组件(必须要手动执行)
		void init(OpStr name); // 初始化(初始不为空先自动执行释放)
		OpEnvI* getImpl(); // 获取Impl
	public:
		OpEnv(); // 默认构造函数(默认为空)
		OpEnv(OpEnvI* impl); // 从Impl构造
		OpEnv(OpBool init, OpStr name); // 初始化构造
	};

	// OpImplBase：所有Impl的基类
	// 原则上来说：初始化时，Impl自身先完成初始化，交由Env接管；释放时，Impl先向Env提交释放请求，由Env释放自身
	// 被锁定的对象部分的修改功能受限，详细内容参考各组件模块中说明
	class OpImplBase
	{
	protected:
		/*
		组件类别:
			V / 变量
			E / 表达式(表达式暂时不进行托管)
			C / 约束
			O / 目标函数
			M / 模型
			S / 求解器
			T / 容器
			U / 未知
		*/
		OpBool locked_; // 是否被锁定
		OpChar class_; // 组件类别
		OpLInt idx_; // 组件索引

		OpEnvI* env_; // 环境变量

		friend class OpEnvI;
	protected:
		void lock(); // 锁定组件
		void unlock(); // 解锁组件
		OpBool isLocked() const; // 是否被锁定
		void release(); // 释放组件本身(手动释放)
		OpChar getClass() const; // 获取组件类别
		OpLInt getIndex() const; // 获取组件索引
		OpEnvI* getEnv() const; // 获取环境变量
	protected:
		OpImplBase(OpChar cls, OpEnvI* env);
	public:
		virtual ~OpImplBase(); // 释放组件本身(被动释放)
	};
}
