#pragma once

#include "OPUA.Model.h"

namespace OPUA
{
	namespace Solver
	{
		class OpConfig;
		class OpCPXSolI;
		class OpCPXSol;
		class OpGRBSolI;
		class OpGRBSol;
		class OpSCIPSolI;
		class OpSCIPSol;
		class OpMSKSolI;
		class OpMSKSol;

		// 通用配置器
		// 只接受四种类型(布尔、整数、浮点和字符串)
		class OpConfig
		{
		protected:
			std::unordered_map<OpStr, OpBool> bcfg_;
			std::unordered_map<OpStr, OpLInt> icfg_;
			std::unordered_map<OpStr, OpFloat> fcfg_;
			std::unordered_map<OpStr, OpStr> scfg_;
		public:
			// 注册配置(配置存在则覆盖旧配置)
			void RegisterConfig(OpStr key, OpBool val);
			void RegisterConfig(OpStr key, OpLInt val);
			void RegisterConfig(OpStr key, OpFloat val);
			void RegisterConfig(OpStr key, OpStr val);
			// 清除配置
			void ClearConfig(OpStr key, OpBool flag);
			void ClearConfig(OpStr key, OpLInt flag);
			void ClearConfig(OpStr key, OpFloat flag);
			void ClearConfig(OpStr key, OpStr flag);
			template<typename T> void ClearConfig(OpStr key) { ClearConfig(key, T()); }
			void ClearAll();
			// 获取配置
			OpBool GetConfig(OpStr key, OpBool flag) const;
			OpLInt GetConfig(OpStr key, OpLInt flag) const;
			OpFloat GetConfig(OpStr key, OpFloat flag) const;
			OpStr GetConfig(OpStr key, OpStr flag) const;
			template<typename T> auto GetConfig(OpStr key) const { return GetConfig(key, T()); }
		};

		class OpGRBSol
			: public OpBase
		{
		public:
			void extract(Model::OpModel mdl); // 抽取OPUA模型，形成GRB模型对象
			void solve(); // 执行求解
			void set(OpStr key, OpBool val); // 设置配置(布尔型)
			void set(OpStr key, OpLInt val); // 设置配置(整数型)
			void set(OpStr key, OpFloat val); // 设置配置(浮点型)
			void set(OpStr key, OpStr val); // 设置配置(字符型)
			OpStr getStatue(); // 获取求解状态
			OpFloat getValue(Variable::OpVar var); // 获取变量的解
			OpFloat getValue(const Expression::OpLinExpr& expr); // 获取表达式的解(速度较慢)
			OpFloat getValue(const Expression::OpQuadExpr& expr); // 获取表达式的解(速度较慢)
			OpFloat getValue(Objective::OpObj obj); // 获取目标函数解(速度较慢)
			OpFloat getDual(Constraint::OpLinCon con); // 获取对偶解
		public:
			OpGRBSol(); // 默认构造函数(默认为空)
			OpGRBSol(OpGRBSolI* impl); // 从impl构造
			OpGRBSol(OpEnv env); // 从env构造
			OpGRBSol(OpEnv env, OpStr name); // 从env构造并指定部分参数
			OpGRBSol(OpEnv env, Model::OpModel mdl); // 从env构造并指定部分参数
			OpGRBSol(OpEnv env, Model::OpModel mdl, OpStr name); // 从env构造并指定部分参数
		public:
			virtual ~OpGRBSol();
		};
	}
}