#pragma once

#include "OPUA.Expression.h"

namespace OPUA
{
	namespace Objective
	{
		class OpObjI;
		class OpObj;

		typedef Container::OpArray<OpObj> OpObjArr; // 目标函数数组
		typedef Container::OpDict<OpLInt, OpObj> OpObjIdxDict; // 目标函数索引字典

		// OpObjSense：OPUA目标函数符号
		enum class OpObjSense
		{
			Min,
			Max
		};

		OpStr ObjSense2Str(OpObjSense sense); // 将目标函数类型转换为字符

		std::ostream& operator<<(std::ostream& stream, OpObj obj);
		OpObj OpMinimize(OpEnv env, const Expression::OpLinExpr& lexpr); // 从一个线性表达式创建目标函数
		OpObj OpMinimize(OpEnv env, const Expression::OpQuadExpr& qexpr); // 从一个二次表达式创建目标函数(包含二次项)
		OpObj OpMaximize(OpEnv env, const Expression::OpLinExpr& lexpr); // 从一个线性表达式创建目标函数
		OpObj OpMaximize(OpEnv env, const Expression::OpQuadExpr& qexpr); // 从一个二次表达式创建目标函数(包含二次项)

		// OpObj：OPUA目标函数类
		class OpObj
		{
		protected:
			OpObjI* impl_;
		public:
			void setLinExpr(const Expression::OpLinExpr& expr); // 设置线性项表达式
			void setQuadExpr(const Expression::OpQuadExpr& expr); // 设置二次项表达式(忽略线性项)
			void setSense(OpObjSense sense); // 设置目标函数符号
			void setName(OpStr name); // 设置目标函数名称

			const Expression::OpLinExpr& getLinExpr() const; // 获取线性项表达式
			const Expression::OpQuadExpr& getQuadExpr() const; // 获取二次项表达式
			OpObjSense getSense() const; // 获取目标函数符号
			OpStr getName() const; // 获取目标函数名称
			OpBool isConstant() const; // 是否为常量目标函数
			OpBool isQuad() const; // 是否为二次目标函数
			OpBool isLocked() const; // 是否被锁定
			OpObjI* getImpl() const; // 获取impl
			OpLInt getIndex() const; // 获取索引
			OpEnv getEnv() const; // 获取环境变量
			void release(); // 释放变量(手动释放)
			void lock(); // 锁定目标函数(锁定后禁止修改表达式和符号)
			void unlock(); // 解锁
		public:
			OpBool operator==(const OpObj& obj);
			OpBool operator!=(const OpObj& obj);
		public:
			OpObj(); // 默认构造函数(默认为空)
			OpObj(OpObjI* impl); // 从impl构造
			OpObj(OpEnv env); // 从env构造
			OpObj(OpEnv env, OpObjSense sense); // 从env构造并指定部分参数
			OpObj(OpEnv env, OpObjSense sense, const Expression::OpLinExpr& lexpr); // 从env构造并指定部分参数
			OpObj(OpEnv env, OpObjSense sense, const Expression::OpLinExpr& lexpr, const Expression::OpQuadExpr& qexpr); // 从env构造并指定部分参数
			OpObj(OpEnv env, OpObjSense sense, const Expression::OpLinExpr& lexpr, const Expression::OpQuadExpr& qexpr, OpStr name); // 从env构造并指定部分参数
		};
	}
}

