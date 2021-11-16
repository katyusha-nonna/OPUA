#pragma once

#include "OPUA.Expression.h"

namespace OPUA
{
	namespace Constraint
	{
		class OpLinConI;
		class OpLinCon;
		class OpQuadConI;
		class OpQuadCon;
		class OpNLConI;
		class OpNLCon;
		class OpSOSConI;
		class OpSOSCon;

		typedef Container::OpArray<OpLinCon> OpLCArr; // 线性约束数组
		typedef Container::OpArray<OpQuadCon> OpQCArr; // 二次约束数组
		typedef Container::OpArray<OpSOSCon> OpSOSArr; // SOS约束数组
		typedef Container::OpDict<OpLInt, OpLinCon> OpLCIdxDict; // 线性约束索引字典
		typedef Container::OpDict<OpLInt, OpQuadCon> OpQCIdxDict; // 二次约束索引字典
		typedef Container::OpDict<OpLInt, OpSOSCon> OpSOSIdxDict; // SOS约束索引字典

		// 警告，不允许“常数 <=/>=/== 常数”，否则将返回空约束

		std::ostream& operator<<(std::ostream& stream, OpLinCon con);
		OpLinCon operator<=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);
		OpLinCon operator>=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);
		OpLinCon operator==(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);

		std::ostream& operator<<(std::ostream& stream, OpQuadCon con);
		OpQuadCon operator<=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);
		OpQuadCon operator>=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);
		OpQuadCon operator==(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);

		std::ostream& operator<<(std::ostream& stream, OpSOSCon con);

		enum class OpConSense
		{
			Equal, // 等式约束
			LessEqual, // 小于等于约束
			GreatEqual, // 大于等于约束
			SOS1, // SOS1约束
			SOS2 // SOS2约束
		};

		OpStr ConSense2Str(OpConSense sense); // 将约束类型转换为字符

		class OpLinCon
		{
		protected:
			OpLinConI* impl_;
		public:
			void setSense(OpConSense sense); // 设置(改变)约束的符号
			void setName(OpStr name); // 设置约束名称
			void setExpr(const Expression::OpLinExpr& expr, OpBool lhs); // 设置约束左/右表达式
			void standardize(OpBool simplify = true); // 约束标准化

			OpConSense getSense() const; // 获取约束的符号
			OpStr getName() const; // 获取约束名称
			const Expression::OpLinExpr& getExpr(OpBool lhs) const; // 获取约束左/右表达式
			OpFloat getRHS() const; // 获取约束右操作数(仅标准化约束下有效)
			OpBool isStandard() const; // 是否为标注化约束
			OpBool isLocked() const; // 是否被锁定
			OpLinConI* getImpl() const; // 获取impl
			OpLInt getIndex() const; // 获取索引
			OpEnv getEnv() const; // 获取环境变量
			void release(); // 释放变量(手动释放)
			void lock(); // 锁定约束(锁定后不能更改表达式和符号，或执行标准化)
			void unlock(); // 解锁
		public:
			OpBool operator==(const OpLinCon& con);
			OpBool operator!=(const OpLinCon& con);
		public:
			OpLinCon(); // 默认构造函数(默认为空)
			OpLinCon(OpLinConI* impl); // 从impl构造
			OpLinCon(OpEnv env); // 从env构造
			OpLinCon(OpEnv env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs); // 从env构造并指定部分参数
			OpLinCon(OpEnv env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs, OpStr name); // 从env构造并指定部分参数
		};

		class OpQuadCon
		{
		protected:
			OpQuadConI* impl_;
		public:
			void setSense(OpConSense sense); // 设置(改变)约束的符号
			void setName(OpStr name); // 设置约束名称
			void setExpr(const Expression::OpQuadExpr& expr, OpBool lhs); // 设置约束左/右表达式
			void standardize(OpBool simplify = true); // 约束标准化

			OpConSense getSense() const; // 获取约束的符号
			OpStr getName() const; // 获取约束名称
			const Expression::OpQuadExpr& getExpr(OpBool lhs) const; // 获取约束左/右表达式
			OpFloat getRHS() const; // 获取约束右操作数(仅标准化约束下有效)
			OpBool isStandard() const; // 是否为标注化约束
			OpBool isLocked() const; // 是否被锁定
			OpQuadConI* getImpl() const; // 获取impl
			OpLInt getIndex() const; // 获取索引
			OpEnv getEnv() const; // 获取环境变量
			void release(); // 释放变量(手动释放)
			void lock(); // 锁定约束(锁定后不能更改表达式和符号，或执行标准化)
			void unlock(); // 解锁
		public:
			OpBool operator==(const OpQuadCon& con);
			OpBool operator!=(const OpQuadCon& con);
		public:
			OpQuadCon(); // 默认构造函数(默认为空)
			OpQuadCon(OpQuadConI* impl); // 从impl构造
			OpQuadCon(OpEnv env); // 从env构造
			OpQuadCon(OpEnv env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs); // 从env构造并指定部分参数
			OpQuadCon(OpEnv env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs, OpStr name); // 从env构造并指定部分参数
		};

		// OpSOSCon：OPUA的SOS约束类
		// SOS(special ordered sets, 特殊顺序集)
		class OpSOSCon
		{
		protected:
			OpSOSConI* impl_;
		public:
			void setSense(OpConSense sense); // 设置(改变)约束的符号
			void setName(OpStr name); // 设置约束名称
			void addVar(Variable::OpVar var, OpFloat weight = 1.0); // 向SOS中添加一个变量(重复添加则追加权重)
			
			OpConSense getSense() const; // 获取约束的符号
			OpStr getName() const; // 获取约束名称		
			const Expression::OpLinExpr& getSOSExpr() const; // 获取表示SOS集合的表达式(变量*权重)
			void removeVar(Variable::OpVar var);  // 从SOS中移除一个变量
			OpBool isLocked() const; // 是否被锁定
			OpSOSConI* getImpl() const; // 获取impl
			OpLInt getIndex() const; // 获取索引
			OpEnv getEnv() const; // 获取环境变量
			void release(); // 释放变量(手动释放)
			void lock(); // 锁定约束(锁定后不能更改变量和符号)
			void unlock(); // 解锁
		public:
			OpBool operator==(const OpSOSCon& con);
			OpBool operator!=(const OpSOSCon& con);
		public:
			OpSOSCon(); // 默认构造函数(默认为空)
			OpSOSCon(OpSOSConI* impl); // 从impl构造
			OpSOSCon(OpEnv env); // 从env构造
			OpSOSCon(OpEnv env, OpConSense sense); // 从env构造并指定部分参数
			OpSOSCon(OpEnv env, OpConSense sense, const Expression::OpLinExpr& expr); // 从env构造并指定部分参数(基于SOS表达式)
			OpSOSCon(OpEnv env, OpConSense sense, const Expression::OpLinExpr& expr, OpStr name); // 从env构造并指定部分参数(基于SOS表达式)
			OpSOSCon(OpEnv env, OpConSense sense, Variable::OpVarArr vars, Container::OpFloatArr weights); // 从env构造并指定部分参数
			OpSOSCon(OpEnv env, OpConSense sense, Variable::OpVarArr vars, Container::OpFloatArr weights, OpStr name); // 从env构造并指定部分参数
		};
	}
}
