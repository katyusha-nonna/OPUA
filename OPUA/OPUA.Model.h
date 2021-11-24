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

		/*
			OpModel：OPUA优化模型类
			优化模型对以下组件使用字典进行分别存储:
				[1] OpVar / 变量 
				[2] OpLinCon / 线性约束
				[3] OpQuadCon / 二次约束
				[4] OpSOSCon / SOS约束
				[5] OpNLCon / 非线性约束
				[6] OpCdtCon / 条件约束
				[7] OpObj / 多目标优化目标函数

				迭代器可以遍历上述组件的字典，键值为索引
		*/
		class OpModel
			: public OpBase
		{
		public:
			void add(Constraint::OpLinCon con); // 添加线性约束
			void add(Constraint::OpLCArr cons); // 添加线性约束集
			void add(Constraint::OpQuadCon con); // 添加二次约束
			void add(Constraint::OpQCArr cons); // 添加二次约束集
			void add(Constraint::OpSOSCon con); // 添加SOS约束
			void add(Constraint::OpSOSArr cons); // 添加SOS约束集
			void add(Constraint::OpNLCon con); // 添加非线性约束
			void add(Constraint::OpNLCArr cons); // 添加非线性约束集
			void add(Constraint::OpCdtCon con); // 添加条件约束
			void add(Constraint::OpCCArr cons);  // 添加条件约束集
			void setObj(Objective::OpObj obj); // 设置单目标优化目标函数
			Objective::OpObj getObj(); // 获取单目标优化目标函数
			void remove(Constraint::OpLinCon con); // 移除线性约束
			void remove(Constraint::OpLCArr cons); // 移除线性约束集
			void remove(Constraint::OpQuadCon con); // 移除二次约束
			void remove(Constraint::OpQCArr cons); // 移除二次约束集
			void remove(Constraint::OpSOSCon con); // 移除SOS约束
			void remove(Constraint::OpSOSArr cons); // 移除SOS约束集
			void remove(Constraint::OpNLCon con); // 移除非线性约束
			void remove(Constraint::OpNLCArr cons); // 移除非线性约束集
			void remove(Constraint::OpCdtCon con); // 添加条件约束
			void remove(Constraint::OpCCArr cons);  // 添加条件约束集
			void write(OpStr path) const; // 将模型写入文件
			void setName(OpStr name); // 设置名称
			OpStr getName() const; // 获取模型名称
			OpModelI* getImpl() const; // 获取impl
		public:
			Variable::OpVarIdxDict::OpDictCIter getCBegin(Variable::OpVar flag) const; // 获取变量字典的首端常量迭代器
			Constraint::OpLCIdxDict::OpDictCIter getCBegin(Constraint::OpLinCon flag) const; // 获取线性约束字典的首端常量迭代器
			Constraint::OpQCIdxDict::OpDictCIter getCBegin(Constraint::OpQuadCon flag) const; // 获取二次约束字典的首端常量迭代器
			Constraint::OpSOSIdxDict::OpDictCIter getCBegin(Constraint::OpSOSCon flag) const; // 获取SOS约束字典的首端常量迭代器
			Constraint::OpNLCIdxDict::OpDictCIter getCBegin(Constraint::OpNLCon flag) const; // 获取非线性约束字典的首端常量迭代器
			Constraint::OpCCIdxDict::OpDictCIter getCBegin(Constraint::OpCdtCon flag) const; // 获取条件约束字典的首端常量迭代器
			Objective::OpObjIdxDict::OpDictCIter getCBegin(Objective::OpObj flag) const; // 获取多目标函数字典的首端常量迭代器	
			template<typename T> inline auto  getCBegin() const { return getCBegin(T()); }

			Variable::OpVarIdxDict::OpDictCIter getCEnd(Variable::OpVar flag) const; // 获取变量字典的末端常量迭代器		
			Constraint::OpLCIdxDict::OpDictCIter getCEnd(Constraint::OpLinCon flag) const; // 获取线性约束字典的末端常量迭代器		
			Constraint::OpQCIdxDict::OpDictCIter getCEnd(Constraint::OpQuadCon flag) const; // 获取二次约束字典的末端常量迭代器		
			Constraint::OpSOSIdxDict::OpDictCIter getCEnd(Constraint::OpSOSCon flag) const; // 获取SOS约束字典的末端常量迭代器
			Constraint::OpNLCIdxDict::OpDictCIter getCEnd(Constraint::OpNLCon flag) const; // 获取非线性约束字典的末端常量迭代器
			Constraint::OpCCIdxDict::OpDictCIter getCEnd(Constraint::OpCdtCon flag) const; // 获取条件约束字典的末端常量迭代器
			Objective::OpObjIdxDict::OpDictCIter getCEnd(Objective::OpObj flag) const; // 获取多目标函数字典的末端常量迭代器
			template<typename T> inline auto getCEnd() const { return getCEnd(T()); }
		public:
			OpBool operator==(const OpModel& model) const;
			OpBool operator!=(const OpModel& model) const;
		public:
			OpModel(); // 默认构造函数(默认为空)
			OpModel(OpModelI* impl); // 从impl构造
			OpModel(OpEnv env); // 从env构造
			OpModel(OpEnv env, OpStr name); // 从env构造并指定部分参数
		public:
			virtual ~OpModel();
		};
	}
}
