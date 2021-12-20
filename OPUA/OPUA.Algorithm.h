#pragma once

#include "OPUA.Model.h"

namespace OPUA
{
	namespace Algorithm
	{

		class OpRobustModelI;

		// OPUA两阶段鲁棒模型阶段类型
		enum class RobustStageType
		{
			FirstStage,
			FirstStageSpecial,
			SecondStagePrimal,
			SecondStageDual,
			Uncertainty,
			Unknown
		};

		/*
			OPUA两阶段鲁棒模型类
			第二阶段原问题/对偶问题必须写成标准形式	
			标准形式：
				第一阶段：
					\min_{y} c^{T}y+\eta
					s.t. Ay>=d
							\eta>=b^{T}x^{l}: l=1,...,r
							Ey+Gx^{l}>=h-Mu_{l}: l=1,...,r
							y\in S_{y}, x^{l}\in S_{x}: l=1,...,r
				第二阶段
					\max_{u\in U}\min_{x} b^{T}x
					s.t. Gx>=h-Ey-Mu
							x\in S_{x}
			其中：
				第二阶段对偶约束
					G^{T}\pi <=b
		*/
		class OpRobustModel
			: public OpBase
		{
		public:		
			void autoDual(); // 自动推导对偶
			void autoBound(); // 自动推导变量的上下界
			void add(Variable::OpVarArr vars, RobustStageType type, OpBool flag); // 添加变量
			void add(Variable::OpVar var, RobustStageType type, OpBool flag); // 添加变量
			void add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag); // 添加约束
			void add(Constraint::OpLinCon con, RobustStageType type, OpBool flag); // 添加约束
			void add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag); // 添加约束
			void add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag); // 添加约束	
			void setObj(Expression::OpLinExpr expr, RobustStageType type); // 设置目标函数
			void setObj(Expression::OpQuadExpr expr, RobustStageType type); // 设置目标函数
			void setValue(Variable::OpVar var, RobustStageType type, OpFloat val);	// 设置解
			void setBound(Variable::OpVar var, RobustStageType type, OpFloat val, OpBool lb); // 设置边界
			void setBound(Variable::OpVar var, RobustStageType type, OpFloat lbVal, OpFloat ubVal); // 设置边界
			OpFloat getValue(Variable::OpVar var, RobustStageType type) const; // 获取解
			OpULInt getSize(RobustStageType type, OpBool isVar) const; // 获取变量&约束数目
			OpFloat getBound(Variable::OpVar var, RobustStageType type, OpBool lb) const; // 获取边界
			void write(OpStr root); // 导出模型
			OpRobustModelI* getImpl() const; // 获取impl
			void release(); // 释放内存
		public:
			OpBool operator==(const OpRobustModel& model) const;
			OpBool operator!=(const OpRobustModel& model) const;
		public:
			OpRobustModel(); // 默认构造函数(默认为空)
			OpRobustModel(OpRobustModelI* impl); // 从impl构造
			OpRobustModel(OpEnv env); // 从env构造
			OpRobustModel(OpEnv env, OpStr name); // 从env构造并指定部分参数
		public:
			virtual ~OpRobustModel();
		};
	}
}