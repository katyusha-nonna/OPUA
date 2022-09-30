#pragma once

#include "OPUA.Model.h"

#ifndef OPUA_CPX_VERSION_128
#define OPUA_CPX_VERSION_128
#endif

#ifndef OPUA_GRB_VERSION_95
#define OPUA_GRB_VERSION_95
#endif

#ifndef OPUA_SCIP_VERSION_701
#define OPUA_SCIP_VERSION_701
#endif

#ifndef OPUA_MSK_VERSION_9320
#define OPUA_MSK_VERSION_9320
#endif

#ifndef OPUA_COPT_VERSION_501
#define OPUA_COPT_VERSION_501
#endif

#ifndef OPUA_IPOPT_VERSION_3147
#define OPUA_IPOPT_VERSION_3147
#endif

#ifndef OPUA_GLPK_VERSION_50
#define OPUA_GLPK_VERSION_50
#endif

#ifndef OPUA_CBC_VERSION_2108
#define OPUA_CBC_VERSION_2108
#endif

namespace OPUA
{
	namespace Solver
	{
		class OpConfig;
		class OpCPXCfgCvt;
		class OpGRBCfgCvt;
		class OpSCIPCfgCvt;
		class OpMSKCfgCvt;
		class OpCOPTCfgCvt;
		class OpIPOPTCfgCvt;
		class OpGLPKCfgCvt;
		class OpCBCCfgCvt;

		class OpSolState;
		class OpCPXSolI;
		class OpCPXSol;
		class OpGRBSolI;
		class OpGRBSol;
		class OpSCIPSolI;
		class OpSCIPSol;
		class OpMSKSolI;
		class OpMSKSol;
		class OpCOPTSolI;
		class OpCOPTSol;
		class OpIPOPTSolI;
		class OpIPOPTSol;
		class OpGLPKSolI;
		class OpGLPKSol;
		class OpCBCSolI;
		class OpCBCSol;
		class OpAdapSol;

		/*
			注：[1] 固定变量当前版本仅对线性约束OpLinCon、二次约束OpQuadCon和目标函数OpObj生效，对其他约束无效，因此模型中含有固定变量时不允许出现其他约束
			支持固定变量功能的求解器接口如下：CPX / GRB / MSK / SCIP / COPT / GLPK / IPOPT / CBC
		*/

		// OPUA支持的求解器类型
		enum class OpSolType
		{
			Unknown,
			CPX,
			GRB,
			SCIP,
			MSK,
			COPT,
			IPOPT,
			GLPK,
			CBC
		};

		// OPUA求解状态
		enum class OpSolStatus
		{
			Unknown,
			Feasible,
			Optimal,
			Infeasible,
			Unbounded,
			InfeasibleOrUnbounded
		};

		// 通用配置器
		// 只接受六种类型(布尔、整型、长整型、浮点和字符和字符串)
		// 在注册/查询键值时，自动去除两侧的空白字符段
		class OpConfig
		{
		protected:
			std::unordered_map<OpStr, OpBool> bcfg_;
			std::unordered_map<OpStr, OpInt> icfg_;
			std::unordered_map<OpStr, OpLInt> lcfg_;
			std::unordered_map<OpStr, OpFloat> fcfg_;
			std::unordered_map<OpStr, OpChar> ccfg_;
			std::unordered_map<OpStr, OpStr> scfg_;

		protected:
			void removeStrSpace(OpStr& key) const; // 移除键值两侧空白字符
		public:
			// 注册配置(配置存在则覆盖旧配置)
			void regCfg(OpStr key, OpBool val);
			void regCfg(OpStr key, OpInt val);
			void regCfg(OpStr key, OpLInt val);
			void regCfg(OpStr key, OpFloat val);
			void regCfg(OpStr key, OpChar val);
			void regCfg(OpStr key, OpStr val);
			// 清除配置
			void clrCfg(OpStr key, OpBool flag);
			void clrCfg(OpStr key, OpInt flag);
			void clrCfg(OpStr key, OpLInt flag);
			void clrCfg(OpStr key, OpFloat flag);
			void clrCfg(OpStr key, OpChar flag);
			void clrCfg(OpStr key, OpStr flag);
			template<typename T> inline void clrCfg(OpStr key) { clrCfg(key, T()); }
			void clrAll();
			// 获取配置
			OpBool getCfg(OpStr key, OpBool flag) const;
			OpInt getCfg(OpStr key, OpInt flag) const;
			OpInt getCfg(OpStr key, OpLInt flag) const;
			OpFloat getCfg(OpStr key, OpFloat flag) const;
			OpInt getCfg(OpStr key, OpChar flag) const;
			OpStr getCfg(OpStr key, OpStr flag) const;
			template<typename T> inline auto getCfg(OpStr key) const { return getCfg(key, T()); }
		public:
			template<typename T>
			class OpCfgCIter
			{
			private:
				using KeyValPair = typename std::unordered_map<OpStr, T>::const_iterator;
				KeyValPair iter_;
				OpStr prefix_;

				friend class OpConfig;
			private:
				OpBool filter(OpStr key) const { return key.find(prefix_) != OpStr::npos; }
			public:
				OpBool operator==(const OpCfgCIter<T>& iter) const { return iter_ == iter.iter_; }
				OpBool operator!=(const OpCfgCIter<T>& iter) const { return iter_ != iter.iter_; }
				OpCfgCIter<T>& operator--() { --iter_; return *this; }
				OpCfgCIter<T>& operator++() { ++iter_; return *this; }
				OpBool ok() const { return filter(iter_->first); }
				const OpStr& getKey() const { return iter_->first; }
				const T& getVal() const { return iter_->second; }
			protected:
				OpCfgCIter(KeyValPair iter, OpStr prefix)
					: iter_(iter), prefix_(prefix) {}
			};

			// 迭代配置
			OpCfgCIter<OpBool> getCBegin(OpStr prefix, OpBool flag) const;
			OpCfgCIter<OpInt> getCBegin(OpStr prefix, OpInt flag) const;
			OpCfgCIter<OpLInt> getCBegin(OpStr prefix, OpLInt flag) const;
			OpCfgCIter<OpFloat> getCBegin(OpStr prefix, OpFloat flag) const;
			OpCfgCIter<OpChar> getCBegin(OpStr prefix, OpChar flag) const;
			OpCfgCIter<OpStr> getCBegin(OpStr prefix, OpStr flag) const;
			template<typename T> inline auto getCBegin(OpStr prefix) const { return getCBegin(prefix, T()); }

			OpCfgCIter<OpBool> getCEnd(OpStr prefix, OpBool flag) const;
			OpCfgCIter<OpInt> getCEnd(OpStr prefix, OpInt flag) const;
			OpCfgCIter<OpLInt> getCEnd(OpStr prefix, OpLInt flag) const;
			OpCfgCIter<OpFloat> getCEnd(OpStr prefix, OpFloat flag) const;
			OpCfgCIter<OpChar> getCEnd(OpStr prefix, OpChar flag) const;
			OpCfgCIter<OpStr> getCEnd(OpStr prefix, OpStr flag) const;
			template<typename T> inline auto getCEnd(OpStr prefix) const { return getCEnd(prefix, T()); }
		};

		// 用于实现Solver自适应接口OpAdapSol的状态类
		// 所有的求解器接口都要继承这个类并实现基本的访问函数
		class OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl) = 0;
			virtual void solve() = 0;
			virtual void solveFixed() = 0;
			virtual void setParam(const OpConfig& cfg) = 0;
			virtual OpLInt getStatus() const = 0;
			virtual OpFloat getObjValue() const = 0;
			virtual OpFloat getValue(Variable::OpVar var) const = 0;
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const = 0;
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const = 0;
			virtual OpFloat getValue(Objective::OpObj obj) const = 0;
			virtual OpFloat getDual(Constraint::OpLinCon con) const = 0;
			virtual void write(OpStr path) const = 0;
			virtual void release0() = 0; // 避免与release重名
		public:
			OpSolState();
			virtual ~OpSolState();
		};

		/*
			OpCPXBSol：求解器Cplex的接口类
			求解参数说明见doc/OPUA.Solver.CPX_API.md
		*/
		class OpCPXSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // 抽取OPUA模型，形成CPX模型对象
			virtual void solve(); // 执行求解
			virtual void solveFixed(); // 固定整数变量解并执行求解
			virtual void setParam(const OpConfig& cfg); // 设置配置
			virtual OpLInt getStatus() const; // 获取求解状态
			virtual OpFloat getObjValue() const; // 获取目标函数解
			virtual OpFloat getValue(Variable::OpVar var) const; // 获取变量的解
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // 获取对偶解
			OpCPXSolI* getImpl() const; // 获取Impl
			virtual void write(OpStr path) const; // 将模型写入文件
			virtual void release0(); // 释放内存(这个接口是给OpAdapSol用的，手动释放请调用release())
		public:
			OpBool operator==(const OpCPXSol& sol) const;
			OpBool operator!=(const OpCPXSol& sol) const;
		public:
			OpCPXSol(); // 默认构造函数(默认为空)
			OpCPXSol(OpCPXSolI* impl); // 从impl构造
			OpCPXSol(OpEnv env); // 从env构造
			OpCPXSol(OpEnv env, Model::OpModel mdl); // 从env构造并指定部分参数
		public:
			virtual ~OpCPXSol();
		};

		/*
			OpGRBSol：求解器Gurobi的接口类
			求解参数说明见doc/OPUA.Solver.GRB_API.md
		*/
		class OpGRBSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // 抽取OPUA模型，形成GRB模型对象
			virtual void solve(); // 执行求解
			virtual void solveFixed(); // 固定整数变量解并执行求解
			virtual void setParam(const OpConfig& cfg); // 设置配置
			virtual OpLInt getStatus() const; // 获取求解状态
			virtual OpFloat getObjValue() const; // 获取目标函数解
			virtual OpFloat getValue(Variable::OpVar var) const; // 获取变量的解
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // 获取对偶解
			OpGRBSolI* getImpl() const; // 获取Impl
			virtual void write(OpStr path) const; // 将模型写入文件
			virtual void release0(); // 释放内存(这个接口是给OpAdapSol用的，手动释放请调用release())
		public:
			OpBool operator==(const OpGRBSol& sol) const;
			OpBool operator!=(const OpGRBSol& sol) const;
		public:
			OpGRBSol(); // 默认构造函数(默认为空)
			OpGRBSol(OpGRBSolI* impl); // 从impl构造
			OpGRBSol(OpEnv env); // 从env构造
			OpGRBSol(OpEnv env, Model::OpModel mdl); // 从env构造并指定部分参数
		public:
			virtual ~OpGRBSol();
		};

		/*
			OpSCIPSol：求解器SCIP的接口类
			求解参数说明见doc/OPUA.Solver.SCIP_API.md
		*/
		class OpSCIPSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // 抽取OPUA模型，形成SCIP模型对象
			virtual void solve(); // 执行求解
			virtual void solveFixed(); // 固定整数变量解并执行求解
			virtual void setParam(const OpConfig& cfg); // 设置配置
			virtual OpLInt getStatus() const; // 获取求解状态
			virtual OpFloat getObjValue() const; // 获取目标函数解(SCIP的目标函数必须为线性且忽略常数项)
			virtual OpFloat getValue(Variable::OpVar var) const; // 获取变量的解
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // 获取对偶解
			OpSCIPSolI* getImpl() const; // 获取Impl
			virtual void write(OpStr path) const; // 将模型写入文件
			virtual void release0(); // 释放内存(这个接口是给OpAdapSol用的，手动释放请调用release())
		public:
			OpBool operator==(const OpSCIPSol& sol) const;
			OpBool operator!=(const OpSCIPSol& sol) const;
		public:
			OpSCIPSol(); // 默认构造函数(默认为空)
			OpSCIPSol(OpSCIPSolI* impl); // 从impl构造
			OpSCIPSol(OpEnv env); // 从env构造
			OpSCIPSol(OpEnv env, Model::OpModel mdl); // 从env构造并指定部分参数
		public:
			virtual ~OpSCIPSol();
		};

		/*
			OpMSKSol：求解器Mosek的接口类
			求解参数说明见doc/OPUA.Solver.MSK_API.md
		*/
		class OpMSKSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // 抽取OPUA模型，形成MSK模型对象
			virtual void solve(); // 执行求解
			virtual void solveFixed(); // 固定整数变量解并执行求解
			virtual void setParam(const OpConfig& cfg); // 设置配置
			virtual OpLInt getStatus() const; // 获取求解状态
			virtual OpFloat getObjValue() const; // 获取目标函数解
			virtual OpFloat getValue(Variable::OpVar var) const; // 获取变量的解
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // 获取对偶解
			OpMSKSolI* getImpl() const; // 获取Impl
			virtual void write(OpStr path) const; // 将模型写入文件
			virtual void release0(); // 释放内存(这个接口是给OpAdapSol用的，手动释放请调用release())
		public:
			OpBool operator==(const OpMSKSol& sol) const;
			OpBool operator!=(const OpMSKSol& sol) const;
		public:
			OpMSKSol(); // 默认构造函数(默认为空)
			OpMSKSol(OpMSKSolI* impl); // 从impl构造
			OpMSKSol(OpEnv env); // 从env构造
			OpMSKSol(OpEnv env, Model::OpModel mdl); // 从env构造并指定部分参数
		public:
			virtual ~OpMSKSol();
		};

		/*
			OpCOPTSol：求解器COPT的接口类
			求解参数说明见doc/OPUA.Solver.COPT_API.md
		*/
		class OpCOPTSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // 抽取OPUA模型，形成COPT模型对象
			virtual void solve(); // 执行求解
			virtual void solveFixed(); // 固定整数变量解并执行求解
			virtual void setParam(const OpConfig& cfg); // 设置配置
			virtual OpLInt getStatus() const; // 获取求解状态
			virtual OpFloat getObjValue() const; // 获取目标函数解
			virtual OpFloat getValue(Variable::OpVar var) const; // 获取变量的解
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // 获取对偶解
			OpCOPTSolI* getImpl() const; // 获取Impl
			virtual void write(OpStr path) const; // 将模型写入文件
			virtual void release0(); // 释放内存(这个接口是给OpAdapSol用的，手动释放请调用release())
		public:
			OpBool operator==(const OpCOPTSol& sol) const;
			OpBool operator!=(const OpCOPTSol& sol) const;
		public:
			OpCOPTSol(); // 默认构造函数(默认为空)
			OpCOPTSol(OpCOPTSolI* impl); // 从impl构造
			OpCOPTSol(OpEnv env); // 从env构造
			OpCOPTSol(OpEnv env, Model::OpModel mdl); // 从env构造并指定部分参数
		public:
			virtual ~OpCOPTSol();
		};

		/*
			OpIPOPTSol：求解器IPOPT的接口类
			求解参数说明见doc/OPUA.Solver.IPOPT_API.md
		*/
		class OpIPOPTSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // 抽取OPUA模型，形成IPOPT模型对象
			virtual void solve(); // 执行求解
			virtual void solveFixed(); // 固定整数变量解并执行求解
			virtual void setParam(const OpConfig& cfg); // 设置配置
			virtual OpLInt getStatus() const; // 获取求解状态
			virtual OpFloat getObjValue() const; // 获取目标函数解
			virtual OpFloat getValue(Variable::OpVar var) const; // 获取变量的解
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // 获取对偶解
			OpIPOPTSolI* getImpl() const; // 获取Impl
			virtual void write(OpStr path) const; // 将模型写入文件
			virtual void release0(); // 释放内存(这个接口是给OpAdapSol用的，手动释放请调用release())
		public:
			OpBool operator==(const OpIPOPTSol& sol) const;
			OpBool operator!=(const OpIPOPTSol& sol) const;
		public:
			OpIPOPTSol(); // 默认构造函数(默认为空)
			OpIPOPTSol(OpIPOPTSolI* impl); // 从impl构造
			OpIPOPTSol(OpEnv env); // 从env构造
			OpIPOPTSol(OpEnv env, Model::OpModel mdl); // 从env构造并指定部分参数
		public:
			virtual ~OpIPOPTSol();
		};

		/*
			OpGLPKSol：求解器GLPK的接口类
			求解参数说明见doc/OPUA.Solver.GLPK_API.md
		*/
		class OpGLPKSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // 抽取OPUA模型，形成GLPK模型对象
			virtual void solve(); // 执行求解
			virtual void solveFixed(); // 固定整数变量解并执行求解
			virtual void setParam(const OpConfig& cfg); // 设置配置
			virtual OpLInt getStatus() const; // 获取求解状态
			virtual OpFloat getObjValue() const; // 获取目标函数解
			virtual OpFloat getValue(Variable::OpVar var) const; // 获取变量的解
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // 获取对偶解
			OpGLPKSolI* getImpl() const; // 获取Impl
			virtual void write(OpStr path) const; // 将模型写入文件
			virtual void release0(); // 释放内存(这个接口是给OpAdapSol用的，手动释放请调用release())
		public:
			OpBool operator==(const OpGLPKSol& sol) const;
			OpBool operator!=(const OpGLPKSol& sol) const;
		public:
			OpGLPKSol(); // 默认构造函数(默认为空)
			OpGLPKSol(OpGLPKSolI* impl); // 从impl构造
			OpGLPKSol(OpEnv env); // 从env构造
			OpGLPKSol(OpEnv env, Model::OpModel mdl); // 从env构造并指定部分参数
		public:
			virtual ~OpGLPKSol();
		};

		/*
			OpCBCSol：求解器CBC的接口类
			求解参数说明见doc/OPUA.Solver.CBC_API.md
		*/
		class OpCBCSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // 抽取OPUA模型，形成CBC模型对象
			virtual void solve(); // 执行求解
			virtual void solveFixed(); // 固定整数变量解并执行求解
			virtual void setParam(const OpConfig& cfg); // 设置配置
			virtual OpLInt getStatus() const; // 获取求解状态(注：CBC的求解状态代码被OPUA重新定义)
			virtual OpFloat getObjValue() const; // 获取目标函数解
			virtual OpFloat getValue(Variable::OpVar var) const; // 获取变量的解
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // 获取表达式的解(速度较慢)
			virtual OpFloat getValue(Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // 获取对偶解
			OpCBCSolI* getImpl() const; // 获取Impl
			virtual void write(OpStr path) const; // 将模型写入文件
			virtual void release0(); // 释放内存(这个接口是给OpAdapSol用的，手动释放请调用release())
		public:
			OpBool operator==(const OpCBCSol& sol) const;
			OpBool operator!=(const OpCBCSol& sol) const;
		public:
			OpCBCSol(); // 默认构造函数(默认为空)
			OpCBCSol(OpCBCSolI* impl); // 从impl构造
			OpCBCSol(OpEnv env); // 从env构造
			OpCBCSol(OpEnv env, Model::OpModel mdl); // 从env构造并指定部分参数
		public:
			virtual ~OpCBCSol();
		};

		/*
			OpAdapSol：求解器自适应接口类
		*/
		class OpAdapSol
		{
		protected:
			const OpSolType stype_;
			OpSolState* rsolver_;
		public:
			void extract(Model::OpModel mdl);
			void solve();
			void solveFixed();
			void setParam(const OpConfig& cfg);
			OpSolType getSolType() const;
			OpLInt getStatus() const;
			OpFloat getObjValue() const;
			OpFloat getValue(Variable::OpVar var) const;
			OpFloat getValue(const Expression::OpLinExpr& expr) const;
			OpFloat getValue(const Expression::OpQuadExpr& expr) const;
			OpFloat getValue(Objective::OpObj obj) const;
			OpFloat getDual(Constraint::OpLinCon con) const;
			void write(OpStr path) const;
			void release();
		public:
			OpAdapSol& operator=(const OpAdapSol& solver);
		public:
			OpAdapSol() = delete;
			OpAdapSol(const OpAdapSol& solver);
			OpAdapSol(OpAdapSol&& solver);
			OpAdapSol(OpSolType type, OpEnv env);
			OpAdapSol(OpSolType type, OpEnv env, Model::OpModel mdl);
			~OpAdapSol();
		};

		// 求解状态转换函数(CPX)
		OpSolStatus IntCPXStatus2OpStatus(OpLInt status);
		// 求解状态转换函数(GRB)
		OpSolStatus IntGRBStatus2OpStatus(OpLInt status);
		// 求解状态转换函数(SCIP)
		OpSolStatus IntSCIPStatus2OpStatus(OpLInt status);
		// 求解状态转换函数(MSK)
		OpSolStatus IntMSKStatus2OpStatus(OpLInt status);
		// 求解状态转换函数(COPT)
		OpSolStatus IntCOPTStatus2OpStatus(OpLInt status);
		// 求解状态转换函数(IPOPT)
		OpSolStatus IntIPOPTStatus2OpStatus(OpLInt status);
		// 求解状态转换函数(GLPK)
		OpSolStatus IntGLPKStatus2OpStatus(OpLInt status);
		// 求解状态转换函数(CBC)
		OpSolStatus IntCBCStatus2OpStatus(OpLInt status);
		// 求解状态转换函数(通用)
		OpSolStatus IntStatus2OpStatus(OpSolType stype, OpLInt status);

		/*
			求解器缩写代码转换函数
			G - GRB
			C - CPX
			S - SCIP
			M - MSK
			T - COPT
			K - GLPK
			B - CBC
			I - IPOPT
		*/
		OpSolType CharSolType2OpSolType(OpChar type);
	}
}