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
		class OpSOSConI;
		class OpSOSCon;
		class OpNLConI;
		class OpNLCon;

		typedef Container::OpArray<OpLinCon> OpLCArr; // 线性约束数组
		typedef Container::OpArray<OpQuadCon> OpQCArr; // 二次约束数组
		typedef Container::OpArray<OpSOSCon> OpSOSArr; // SOS约束数组
		typedef Container::OpArray<OpNLCon> OpNLCArr; // 非线性约束数组
		typedef Container::OpDict<OpLInt, OpLinCon> OpLCIdxDict; // 线性约束索引字典
		typedef Container::OpDict<OpLInt, OpQuadCon> OpQCIdxDict; // 二次约束索引字典
		typedef Container::OpDict<OpLInt, OpSOSCon> OpSOSIdxDict; // SOS约束索引字典
		typedef Container::OpDict<OpLInt, OpNLCon> OpNLCIdxDict; // 非线性约束索引字典

		enum class OpConSense
		{
			Equal, // 等式约束
			LessEqual, // 小于等于约束
			GreatEqual, // 大于等于约束
			SOS1, // SOS1约束
			SOS2 // SOS2约束
		};

		enum class OpConFunc
		{
			Sum, // 求和 x1 = Sum(x2, x3, ...)
			Abs, // 绝对值 x1 = abs(x2)
			Max, // 取最大值 x1 = max(x2, x3, ...)
			Min, // 取最小值 x1 = min(x2, x3, ...)
			Square, // 平方 x1 = x2^^2
			Sqrt, // 平方根 x1 = sqrt(x2)
			Pow, // 幂 x1 = x2^^x3
			Exp1, // 指数 x1 = e^^x2
			Exp2, // 指数 x1 = x2^^x3
			Log1, // 对数 x1 = log (10) x2
			Log2, // 对数 x1 = log (e) x2
			Log3, // 对数 x1 = log (x2) x3
			Sin, // 正弦 x1 = sin(x2)
			Cos, // 余弦 x1 = cos(x2)
			Tan, // 正切 x1 = tan(x2)
		};

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

		// 警告，参数的顺序关系到操作数的位置，详见操作函数说明
		OpNLCon OpSum(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X);
		OpNLCon OpMax(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X);
		OpNLCon OpMin(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X);
		OpNLCon OpAbs(OpEnv env, Variable::OpVar x1, Variable::OpVar x2);
		OpNLCon OpSquare(OpEnv env, Variable::OpVar x1, Variable::OpVar x2);
		OpNLCon OpSqrt(OpEnv env, Variable::OpVar x1, Variable::OpVar x2);
		OpNLCon OpPow(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, Variable::OpVar x3);
		OpNLCon OpExp1(OpEnv env, Variable::OpVar x1, Variable::OpVar x2);
		OpNLCon OpExp2(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, Variable::OpVar x3);
		OpNLCon OpLog1(OpEnv env, Variable::OpVar x1, Variable::OpVar x2);
		OpNLCon OpLog2(OpEnv env, Variable::OpVar x1, Variable::OpVar x2);
		OpNLCon OpLog3(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, Variable::OpVar x3);
		OpNLCon OpSin(OpEnv env, Variable::OpVar x1, Variable::OpVar x2);
		OpNLCon OpCos(OpEnv env, Variable::OpVar x1, Variable::OpVar x2);
		OpNLCon OpTan(OpEnv env, Variable::OpVar x1, Variable::OpVar x2);

		OpStr ConSense2Str(OpConSense sense); // 将约束类型转换为字符

		class OpLinCon
			: public OpBase
		{
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
			OpLinConI* getImpl() const; // 获取impl
		public:
			OpBool operator==(const OpLinCon& con);
			OpBool operator!=(const OpLinCon& con);
		public:
			OpLinCon(); // 默认构造函数(默认为空)
			OpLinCon(OpLinConI* impl); // 从impl构造
			OpLinCon(OpEnv env); // 从env构造
			OpLinCon(OpEnv env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs); // 从env构造并指定部分参数
			OpLinCon(OpEnv env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs, OpStr name); // 从env构造并指定部分参数
		public:
			virtual ~OpLinCon();
		};

		class OpQuadCon
			: public OpBase
		{
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
			OpQuadConI* getImpl() const; // 获取impl
		public:
			OpBool operator==(const OpQuadCon& con);
			OpBool operator!=(const OpQuadCon& con);
		public:
			OpQuadCon(); // 默认构造函数(默认为空)
			OpQuadCon(OpQuadConI* impl); // 从impl构造
			OpQuadCon(OpEnv env); // 从env构造
			OpQuadCon(OpEnv env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs); // 从env构造并指定部分参数
			OpQuadCon(OpEnv env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs, OpStr name); // 从env构造并指定部分参数
		public:
			virtual ~OpQuadCon();
		};

		// OpSOSCon：OPUA的SOS约束类
		// SOS(special ordered sets, 特殊顺序集)
		class OpSOSCon
			: public OpBase
		{
		public:
			void setSense(OpConSense sense); // 设置(改变)约束的符号
			void setName(OpStr name); // 设置约束名称
			void addVar(Variable::OpVar var, OpFloat weight = 1.0); // 向SOS中添加一个变量(重复添加则追加权重)
			
			OpConSense getSense() const; // 获取约束的符号
			OpStr getName() const; // 获取约束名称		
			const Expression::OpLinExpr& getSOSExpr() const; // 获取表示SOS集合的表达式(变量*权重)
			void removeVar(Variable::OpVar var);  // 从SOS中移除一个变量
			OpSOSConI* getImpl() const; // 获取impl
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
		public:
			virtual ~OpSOSCon();
		};

		// OpNLCon：OPUA的非线性约束类
		class OpNLCon
			: public OpBase
		{
		public:
			void setFunction(OpConFunc func); // 设置(改变)约束的操作函数
			void setName(OpStr name); // 设置约束名称
			void addVar(Variable::OpVar var); // 添加操作数(有顺序区别)
			OpConFunc getFunction() const; // 获取约束的操作函数
			OpStr getName() const; // 获取约束名称
			OpNLConI* getImpl() const; // 获取impl
		public:
			Variable::OpVarArr::OpArrCIter getCBegin(); // 获取变量数组的常量首端迭代器
			Variable::OpVarArr::OpArrCIter getCEnd(); // 获取变量数组的常量末端迭代器
		public:
			OpBool operator==(const OpNLCon& con);
			OpBool operator!=(const OpNLCon& con);
		public:
			OpNLCon(); // 默认构造函数(默认为空)
			OpNLCon(OpNLConI* impl); // 从impl构造
			OpNLCon(OpEnv env); // 从env构造
			OpNLCon(OpEnv env, OpConFunc func); // 从env构造并指定部分参数
			OpNLCon(OpEnv env, OpConFunc func, OpStr name); // 从env构造并指定部分参数
		public:
			virtual ~OpNLCon();
		};
	}
}
