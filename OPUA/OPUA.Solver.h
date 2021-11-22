#pragma once

#include "OPUA.Model.h"

namespace OPUA
{
	namespace Solver
	{
		class OpConfig;
		class OpGRBCfgCvt;

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

			friend class OpCPXSolI;
			friend class OpGRBSolI;
			friend class OpMSKSolI;
			friend class OpSCIPSolI;
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
			: public OpBase
		{
		public:
			void extract(Model::OpModel mdl); // 抽取OPUA模型，形成GRB模型对象
			void solve(); // 执行求解
			void setParam(const OpConfig& cfg); // 设置配置
			OpLInt getStatus(); // 获取求解状态
			OpFloat getValue(Variable::OpVar var); // 获取变量的解
			OpFloat getValue(const Expression::OpLinExpr& expr); // 获取表达式的解(速度较慢)
			OpFloat getValue(const Expression::OpQuadExpr& expr); // 获取表达式的解(速度较慢)
			OpFloat getValue(Objective::OpObj obj); // 获取目标函数解(速度较慢)
			OpFloat getDual(Constraint::OpLinCon con); // 获取对偶解
		public:
			OpGRBSol(); // 默认构造函数(默认为空)
			OpGRBSol(OpGRBSolI* impl); // 从impl构造
			OpGRBSol(OpEnv env); // 从env构造
			OpGRBSol(OpEnv env, Model::OpModel mdl); // 从env构造并指定部分参数
		public:
			virtual ~OpGRBSol();
		};
	}
}