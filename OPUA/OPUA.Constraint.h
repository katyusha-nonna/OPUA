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
		class OpCdtConI;
		class OpCdtCon;
		class OpLogicConI; // TODO: 逻辑约束(AND / OR / NOR)
		class OpLogicCon;

		typedef Container::OpArray<OpLinCon> OpLCArr; // 线性约束数组
		typedef Container::OpArray<OpQuadCon> OpQCArr; // 二次约束数组
		typedef Container::OpArray<OpSOSCon> OpSOSArr; // SOS约束数组
		typedef Container::OpArray<OpNLCon> OpNLCArr; // 非线性约束数组
		typedef Container::OpArray<OpCdtCon> OpCCArr; // 条件约束数组
		typedef Container::OpDict<OpLInt, OpLinCon> OpLCIdxDict; // 线性约束索引字典
		typedef Container::OpDict<OpLInt, OpQuadCon> OpQCIdxDict; // 二次约束索引字典
		typedef Container::OpDict<OpLInt, OpSOSCon> OpSOSIdxDict; // SOS约束索引字典
		typedef Container::OpDict<OpLInt, OpNLCon> OpNLCIdxDict; // 非线性约束索引字典
		typedef Container::OpDict<OpLInt, OpCdtCon> OpCCIdxDict; // 条件约束索引字典

		enum class OpConSense
		{
			Equal, // 等式约束
			LessEqual, // 小于等于约束
			GreatEqual, // 大于等于约束
			SOS1, // SOS1约束
			SOS2 // SOS2约束
		};

		// 警告，创建线性/二次约束时，不允许“常数 <=/>=/== 常数”，否则将返回空约束
		std::ostream& operator<<(std::ostream& stream, OpLinCon con);
		OpLinCon operator<=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);
		OpLinCon operator>=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);
		OpLinCon operator==(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);

		std::ostream& operator<<(std::ostream& stream, OpQuadCon con);
		OpQuadCon operator<=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);
		OpQuadCon operator>=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);
		OpQuadCon operator==(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);

		std::ostream& operator<<(std::ostream& stream, OpSOSCon con);

		// 警告，创建非线性约束时，传入的非线性表达式不能为空，否则将返回空约束
		std::ostream& operator<<(std::ostream& stream, OpNLCon con);
		OpNLCon operator==(const Expression::OpNLExpr& lhs, Variable::OpVar rhs);
		OpNLCon operator==(Variable::OpVar lhs, const Expression::OpNLExpr& rhs);

		// 警告，创建条件约束时，==左右两端约束/变量不能互换，左侧为条件，右侧为约束
		std::ostream& operator<<(std::ostream& stream, OpCdtCon con);
		OpCdtCon operator==(Variable::OpVar lhs, OpLinCon rhs);
		OpCdtCon operator==(OpLinCon lhs, OpLinCon rhs);

		// 警告，参数的顺序关系到操作数的位置，详见操作函数说明
		OpNLCon OpSum(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X, OpFloat a); // x1 = Sum(X, a)
		OpNLCon OpMax(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X, OpFloat a); // x1 = Max(X, a)
		OpNLCon OpMin(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X, OpFloat a); // x1 = Min(X, a)
		OpNLCon OpAbs(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = Abs(x2)
		OpNLCon OpSquare(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = x2^^2
		OpNLCon OpSqrt(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = x2^^0.5
		OpNLCon OpPow(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, OpFloat a); // x1 = x2^^a
		OpNLCon OpExp1(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = e^^x2
		OpNLCon OpExp2(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, OpFloat a); // x1 = a^^x2
		OpNLCon OpLog1(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = log (10) x2
		OpNLCon OpLog2(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = log (e) x2
		OpNLCon OpLog3(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, OpFloat a); // x1 = log (a) x2
		OpNLCon OpSin(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = sin(x2)
		OpNLCon OpCos(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = cos(x2)
		OpNLCon OpTan(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = tan(x2)

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
			OpBool operator==(const OpLinCon& con) const;
			OpBool operator!=(const OpLinCon& con) const;
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
			OpBool operator==(const OpQuadCon& con) const;
			OpBool operator!=(const OpQuadCon& con) const;
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
			OpBool operator==(const OpSOSCon& con) const;
			OpBool operator!=(const OpSOSCon& con) const;
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
		// 格式：x = expr
		class OpNLCon
			: public OpBase
		{
		public:
			void setName(OpStr name); // 设置约束名称
			void setVar(Variable::OpVar var); // 设置变量(约束左操作数)
			void setExpr(const Expression::OpNLExpr& expr); // 设置非线性表达式(约束右操作数)
			Expression::OpNLFunc getFunction() const; // 获取非线性约束的操作函数
			const Expression::OpNLExpr& getExpr() const; // 获取非线性表达式
			Variable::OpVar getVar() const; // 获取变量
			OpStr getName() const; // 获取约束名称
			OpNLConI* getImpl() const; // 获取impl
		public:
			OpBool operator==(const OpNLCon& con) const;
			OpBool operator!=(const OpNLCon& con) const;
		public:
			OpNLCon(); // 默认构造函数(默认为空)
			OpNLCon(OpNLConI* impl); // 从impl构造
			OpNLCon(OpEnv env); // 从env构造
			OpNLCon(OpEnv env, Variable::OpVar var, const Expression::OpNLExpr& expr); // 从env构造并指定部分参数
			OpNLCon(OpEnv env, Variable::OpVar var, const Expression::OpNLExpr& expr, OpStr name); // 从env构造并指定部分参数
		public:
			virtual ~OpNLCon();
		};

		// OpCdtCon：OPUA的条件约束类
		// 格式1(指示器约束)：ind -> con (当ind=1时，con生效；当ind=0时，con可以被违反)
		// 格式2(If-Then约束)：con1 -> con2 (当con1生效时，con2生效；当con1被违反时，con2可以被违反)
		class OpCdtCon
			: public OpBase
		{
		public:
			void setName(OpStr name); // 设置约束名称
			void setVar(Variable::OpVar var); // 设置指示器变量
			void setCon(OpLinCon con, OpBool con1); // 设置约束
			Variable::OpVar getVar() const; // 获取指示器变量
			OpLinCon getCon(OpBool con1) const; // 获取IF/Then约束
			OpBool isIndicator() const; // 是否为指示器约束
			OpStr getName() const; // 获取约束名称
			OpCdtConI* getImpl() const; // 获取impl
			void lock();  // 锁定条件约束(和普通的lock不同，会同时锁定con或con1&con2)
			void unlock();// 解锁条件约束(和普通的lock不同，会同时解锁con或con1&con2)
		public:
			OpBool operator==(const OpCdtCon& con) const;
			OpBool operator!=(const OpCdtCon& con) const;
		public:
			OpCdtCon(); // 默认构造函数(默认为空)
			OpCdtCon(OpCdtConI* impl); // 从impl构造
			OpCdtCon(OpEnv env); // 从env构造
			OpCdtCon(OpEnv env, Variable::OpVar ind, OpLinCon con); // 从env构造并指定部分参数
			OpCdtCon(OpEnv env, Variable::OpVar ind, OpLinCon con, OpStr name); // 从env构造并指定部分参数
			OpCdtCon(OpEnv env, OpLinCon con1, OpLinCon con2); // 从env构造并指定部分参数
			OpCdtCon(OpEnv env, OpLinCon con1, OpLinCon con2, OpStr name); // 从env构造并指定部分参数
		public:
			virtual ~OpCdtCon();
		};
	}
}
