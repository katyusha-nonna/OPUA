#pragma once

#include "OPUA.Constant.h"
#include "OPUA.Environment.h"
#include "OPUA.Container.h"

namespace OPUA
{
	namespace Variable
	{
		class OpVarI;
		class OpVar;

		typedef Container::OpArray<OpVar> OpVarArr; // 决策变量数组
		typedef Container::OpDict<OpLInt, OpVar> OpVarIdxDict; // 决策变量索引字典

		// OpVarType：变量类型
		enum class OpVarType
		{
			Bool, // 0-1变量
			Int, // 整数变量
			Con, // 连续变量
			Sem, // 半连续变量
		};

		OpStr VarType2Str(OpVarType type);
		std::ostream& operator<<(std::ostream& stream, OpVar var);

		// OpVar：OPUA变量类
		class OpVar
			: public OpBase
		{
		public:
			void setLb(OpFloat lb); // 设置变量下界
			void setUb(OpFloat ub); // 设置变量上界
			void setBound(OpFloat lb, OpFloat ub); // 设置变量上下界
			void setType(OpVarType type); // 设置(改变)变量的类型
			void setName(OpStr name); // 设置变量名称

			OpFloat getLb() const; // 获取变量上界
			OpFloat getUb() const; // 获取变量下界
			OpVarType getType() const; // 获取变量类型
			OpStr getName() const; // 获取变量名称
			OpVarI* getImpl() const; // 获取impl
		public:
			OpBool operator==(const OpVar& var) const;
			OpBool operator!=(const OpVar& var) const;
		public:
			OpVar(); // 默认构造函数(默认为空)
			OpVar(OpVarI* impl); // 从impl构造
			OpVar(OpEnv env); // 从env构造
			OpVar(OpEnv env, OpVarType type); // 从env构造并指定部分参数
			OpVar(OpEnv env, OpVarType type, OpFloat lb, OpFloat ub); // 从env构造并指定部分参数
			OpVar(OpEnv env, OpVarType type, OpFloat lb, OpFloat ub, OpStr name); // 从env构造并指定部分参数
		public:
			virtual ~OpVar();
		};
	}
}