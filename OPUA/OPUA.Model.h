#pragma once

#include "OPUA.Variable.h"
#include "OPUA.Constraint.h"
#include "OPUA.Objective.h"

namespace OPUA
{
	namespace Model
	{
		class OpModelI;
		class OpModel;

		// OpModel：OPUA优化模型类
		class OpModel
		{
		protected:
			OpModelI* impl_;
		public:
			void add(Constraint::OpLinCon con); // 添加线性约束
			void add(Constraint::OpLCArr cons); // 添加线性约束集
			void add(Constraint::OpQuadCon con); // 添加二次约束
			void add(Constraint::OpQCArr cons); // 添加二次约束集
			void add(Constraint::OpSOSCon con); // 添加SOS约束
			void add(Constraint::OpSOSArr cons); // 添加SOS约束集
			void setObj(Objective::OpObj obj); // 设置单目标优化目标函数
			void remove(Constraint::OpLinCon con); // 移除线性约束
			void remove(Constraint::OpLCArr cons); // 移除线性约束集
			void remove(Constraint::OpQuadCon con); // 移除二次约束
			void remove(Constraint::OpQCArr cons); // 移除二次约束集
			void remove(Constraint::OpSOSCon con); // 移除SOS约束
			void remove(Constraint::OpSOSArr cons); // 移除SOS约束集
			void write(OpStr path);
			void setName(OpStr name); // 设置名称
			OpStr getName() const; // 获取模型名称
			OpBool isLocked() const; // 是否被锁定
			OpModelI* getImpl() const; // 获取impl
			OpEnv getEnv() const; // 获取环境变量
			void release(); // 释放变量(手动释放)
			void lock(); // 锁定模型
			void unlock(); // 解锁
		public:
			OpBool operator==(const OpModel& model);
			OpBool operator!=(const OpModel& model);
		public:
			OpModel(); // 默认构造函数(默认为空)
			OpModel(OpModelI* impl); // 从impl构造
			OpModel(OpEnv env); // 从env构造
			OpModel(OpEnv env, OpStr name); // 从env构造并指定部分参数
		};
	}
}
