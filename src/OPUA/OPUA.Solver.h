#pragma once

#include "OPUA.Model.h"
#define OPUA_CPX_VERSION_128
#define OPUA_GRB_VERSION_95
#define OPUA_SCIP_VERSION_701

#define OPUA_COMPILE_CPX
#define OPUA_COMPILE_GRB
#define OPUA_COMPILE_SCIP
#define OPUA_COMPILE_MSK

namespace OPUA
{
	namespace Solver
	{
		class OpConfig;
		class OpCPXCfgCvt;
		class OpGRBCfgCvt;
		class OpSCIPCfgCvt;

		class OpSolState;
		class OpCPXSolI;
		class OpCPXSol;
		class OpGRBSolI;
		class OpGRBSol;
		class OpSCIPSolI;
		class OpSCIPSol;
		class OpMSKSolI;
		class OpMSKSol;
		class OpAdapSol;

		// OPUA支持的求解器类型
		enum class OpSolType
		{
			Unknown,
			CPX,
			GRB,
			SCIP,
			MSK
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
			求解参数说明：
				OPUA.CPX.Advance / OpInt / 0 / [0, 3] / 注释：?
				OPUA.CPX.Barrier.Algorithm / OpInt / 0 / [0, 3] / 注释：1|2-初始不可行(速度较慢) / 3-标准内点法(速度较快)
				OPUA.CPX.Barrier.ColNonzeros / OpInt / 0 / [0, INF] / 注释：?
				OPUA.CPX.Barrier.ConvergeTol / OpFloat / 0.00000001 / [0.000000000001, INF] / 注释：对于LP和QP问题的收敛判据
				OPUA.CPX.Barrier.Crossover / OpInt / 0 / [-1, 2] / 注释：?
				OPUA.CPX.Barrier.Display / OpInt / 1 / [0, 2] / 注释：内点法计算过程中输出信息类型
				OPUA.CPX.Barrier.Limits.Corrections / OpLInt / 0 / [-1, INF] / 注释：?
				OPUA.CPX.Barrier.Limits.Growth / OpFloat / 1000000000000 / [1, INF] / 注释：?
				OPUA.CPX.Barrier.Limits.Iteration / OpLInt / 9223372036800000000 / [0, INF] / 注释：内点法迭代次数限制
				OPUA.CPX.Barrier.Limits.ObjRange / OpFloat / 1E+20 / [0, INF] / 注释：unbounded问题判据
				OPUA.CPX.Barrier.Ordering / OpInt / 0 / [0, 3] / 注释：?
				OPUA.CPX.Barrier.QCPConvergeTol / OpFloat / 0.0000001 / [0.000000000001, INF] / 注释：对于QCP问题的收敛判据
				OPUA.CPX.Barrier.StartAlg / OpInt / 1 / [1, 4] / 注释：初始内点选择方式
				OPUA.CPX.Simplex.Crash / OpInt / 0 / [-1, 1] / 注释：?
				OPUA.CPX.Simplex.Dgradient / OpInt / 0 / [0, 5] / 注释：?
				OPUA.CPX.Simplex.Display / OpInt / 1 / [0, 2] / 注释：单纯形法计算过程中输出信息类型
				OPUA.CPX.Simplex.Limits.Iterations / OpLInt / 9223372036800000000 / [0, INF] / 注释：单纯形法最大迭代次数

				...未完待续

		*/
		class OpCPXSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // 抽取OPUA模型，形成CPX模型对象
			virtual void solve(); // 执行求解
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
			求解参数说明：
			[1] OPUA.GRB.Termination(终止条件)
				OPUA.GRB.Termination.BarIterLimit / int / 1000 / [0, MAXINT] / 退出条件：内点法已迭代次数
				OPUA.GRB.Termination.BestBdStop / double / INF / [-INF, INF] / ?
				OPUA.GRB.Termination.BestObjStop / double / -INF / [-INF, INF] / 退出条件：目标函数最优估计值(比这个值小就可以退出)
				OPUA.GRB.Termination.Cutoff / double / INF / [-INF, INF] / ?
				OPUA.GRB.Termination.IterationLimit / double / INF / [0, INF] / 退出条件：单纯形法已迭代次数
				OPUA.GRB.Termination.NodeLimit / double / INF / [0, INF] / 退出条件：已遍历的MIP节点个数
				OPUA.GRB.Termination.SolutionLimit / int / INF / [1, MAXINT] / 退出条件：已找到的可行解个数
				OPUA.GRB.Termination.TimeLimit / double / INF / [0, INF] / 退出条件：求解已耗时

			[2] OPUA.GRB.Tolerances(可行性&最优性收敛判定参数)
				OPUA.GRB.Tolerances.BarConvTol / double / 1e-8 / [0.0, 1.0] / 内点法收敛间隙
				OPUA.GRB.Tolerances.BarQCPConvTol / double / 1e-6 / [0.0, 1.0] / 内点法收敛间隙(QCP问题)
				OPUA.GRB.Tolerances.FeasibilityTol / double / 1e-6 / [1e-9, 1e-2] / 约束条件可行性判定间隙
				OPUA.GRB.Tolerances.IntFeasTol / double / 1e-5 / [1e-9, 1e-1] / 整数解可行性判定间隙
				OPUA.GRB.Tolerances.MarkowitzTol / double / 0.0078125 / [1e-4, 0.999] / ?
				OPUA.GRB.Tolerances.MIPGap / double / 1e-4 / [0, INF] / MIP问题相对收敛间隙
				OPUA.GRB.Tolerances.MIPGapAbs / double / 1e-10 / [0, INF] / MIP问题绝对收敛间隙
				OPUA.GRB.Tolerances.OptimalityTol / double / 1e-6 / [1e-9, 1e-2] / 对偶可行性判定间隙(原问题最优性判定间隙)
				OPUA.GRB.Tolerances.PSDTol / double / 1e-6 / [0, INF] / ?

			[3] OPUA.GRB.Simplex(单纯形法参数)
				OPUA.GRB.Simplex.InfUnbdInfo / int / 0 / [0, 1] / ?
				OPUA.GRB.Simplex.NormAdjust / int / -1 / [-1, 3] / ?
				OPUA.GRB.Simplex.ObjScale / double / 0.0 / [-1, INF] / ?
				OPUA.GRB.Simplex.PerturbValue / double / 0.0002 / [0, INF] / ?
				OPUA.GRB.Simplex.Quad / int / -1 / [-1, 1] / ?
				OPUA.GRB.Simplex.ScaleFlag / int / -1 / [-1, 3] / ?
				OPUA.GRB.Simplex.Sifting / int / -1 / [-1, 2] / ?
				OPUA.GRB.Simplex.SiftMethod / int / -1 / [-1, 2] / ?
				OPUA.GRB.Simplex.SimplexPricing / int / -1 / [-1, 3] / ?

			[4] OPUA.GRB.Barrier(内点法参数)
				OPUA.GRB.Barrier.BarCorrectors / int / -1 / [-1, MAXINT] / ?
				OPUA.GRB.Barrier.BarHomogeneous / int / -1 / [-1, 1] / ?
				OPUA.GRB.Barrier.BarOrder / int / -1 / [-1, 1] / ?
				OPUA.GRB.Barrier.Crossover / int / -1 / [-1, 4] / ?
				OPUA.GRB.Barrier.CrossoverBasis / int / 0 / [0, 1] / ?
				OPUA.GRB.Barrier.QCPDual / int / 0 / [0, 1] / ?

			[5] OPUA.GRB.MIP(混合整数求解参数)
				OPUA.GRB.MIP.BranchDir / int / 0 / [-1, 1] / 分支定界方向选择(0-自动 / -1-优先遍历下分支 / 1-优先遍历上分支)
				OPUA.GRB.MIP.ConcurrentJobs / int / 0 / [0, MAXINT] / 并行求解器任务数
				OPUA.GRB.MIP.ConcurrentMIP / int / 1 / [0, MAXINT] / 并行工作的MIP求解器数
				OPUA.GRB.MIP.ConcurrentSettings / string / "" / 并行环境配置参数
				OPUA.GRB.MIP.DegenMoves / int / -1 / [-1, MAXINT] / ?
				OPUA.GRB.MIP.Disconnected / int / -1 / [-1, 2] / ?
				OPUA.GRB.MIP.DistributedMIPJobs / int / 0 / [0, MAXINT] / ?
				OPUA.GRB.MIP.Heuristics / double / 0.05 / [0, 1] / 使用启发式方法探索MIP可行解的耗时程度(占总求解时间的比例)
				OPUA.GRB.MIP.ImproveStartGap / double / 0.0 / [0.0, INF] / MIP求解过程中开始切换寻优策略的相对收敛间隙起点(RelGap小于该值时，切换寻优策略)
				OPUA.GRB.MIP.ImproveStartNodes / double / INF / [0.0, INF] / 效果同ImproveStartGap，条件改为已搜索的MIP节点数量
				OPUA.GRB.MIP.ImproveStartTime / double / INF / [0.0, INF] / 效果同ImproveStartGap，条件改为已耗时
				OPUA.GRB.MIP.LazyConstraints / int / 0 / [0, 1] / 启用Lazy约束时必须开启
				OPUA.GRB.MIP.MinRelNodes / int / -1/ [-1, MAXINT] / ?
				OPUA.GRB.MIP.MIPFocus / int / 0 / [0, 3] / 影响寻优策略的配置，数值越大越倾向于搜寻最优解，数值越小越倾向于搜寻可行解
				OPUA.GRB.MIP.MIQCPMethod / -1 / [-1, 1] / MIQCP求解模式(-1-自动选择 / 0-在每个节点上求解QCP松弛 / 1-使用线性化的外逼近法)
				OPUA.GRB.MIP.NodefileDir / string / "" / ?
				OPUA.GRB.MIP.NodefileStart / double / INF / [0, INF] / ?
				OPUA.GRB.MIP.NodeMethod / int / -1 / [-1, 2] / MIP节点上求解松弛问题的算法选择模式(-1-自动 / 0-原始单纯形 / 1-对偶单纯形 / 2-内点法)
				OPUA.GRB.MIP.NonConvex / int / -1 / [-1, 2] / 如果MIP中有双线性项等非线性部分，需要打开此开关，否则报错
				OPUA.GRB.MIP.NoRelHeurTime / double / 0 / [0, INF] / ?
				OPUA.GRB.MIP.NoRelHeurWork / double / 0 / [0, INF] / ?
				OPUA.GRB.MIP.PartitionPlace / int / 15 / [0, 31] / 位开关：详见GRB用户手册
				OPUA.GRB.MIP.PumpPassese / int / -1 / [-1, MAXINT] / ?
				OPUA.GRB.MIP.RINS / int / -1 / [-1, MAXINT] / RINS探测频率(-1-自动 / other-探测频率上限)
				OPUA.GRB.MIP.SolFiles / string / "" / 当前解的输出文件路径
				OPUA.GRB.MIP.SolutionNumber / int / 0 / [0, MAXINT] / 当MIP存在多解时，被选中的MIP解的序号
				OPUA.GRB.MIP.StartNodeLimit / int / -1/ [-3, MAXINT] / ?
				OPUA.GRB.MIP.StartNumber / int / 0 / [-1, MAXINT] / 当存在多个MIP起点时，被选中的MIP起点的序号
				OPUA.GRB.MIP.SubMIPNodes / int / 500 / [-1, MAXINT] / ?
				OPUA.GRB.MIP.Symmetry / int / -1 / [-1, 2] / ?
				OPUA.GRB.MIP.VarBranch / int / -1 / [-1, 3] / 分支定界时选择变量的策略，详见GRB用户手册
				OPUA.GRB.MIP.ZeroObjNodes  / int / -1 / [-1, MAXINT] / ?

		*/
		class OpGRBSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // 抽取OPUA模型，形成GRB模型对象
			virtual void solve(); // 执行求解
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
			求解参数说明：
		*/
		class OpSCIPSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // 抽取OPUA模型，形成SCIP模型对象
			virtual void solve(); // 执行求解
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
		// 求解状态转换函数(通用)
		OpSolStatus IntStatus2OpStatus(OpSolType stype, OpLInt status);
	}
}