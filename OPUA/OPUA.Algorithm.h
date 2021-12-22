#pragma once

#include "OPUA.Solver.h"

namespace OPUA
{
	namespace Algorithm
	{

		class OpRobustModelI;

		// OPUA两阶段鲁棒模型阶段类型
		enum class RobustStageType
		{
			Unknown, // 未知阶段(仅作为默认值保留)
			FirstStage, // 第一阶段问题
			FirstStageSpecial, // 第一阶段特殊问题(不参与优化，类似于非线性方程组)
			SecondStagePrimal, // 第二阶段原始问题
			SecondStageDual, // 第二阶段对偶问题
			Uncertainty // 不确定集
		};

		/*
			OPUA两阶段鲁棒模型类
			第二阶段原问题/对偶问题必须写成标准形式(数学形式严格按照如下公式所述)
			注意：使用“<=”、“>=”符号生成的线性/二次约束必须将常量和含变量的表达式进行分离，且常量应作为左操作数，即：lb<=expr、ub>=expr
						写成expr>=lb实际中会被加载为-expr<=-lb，会导致autoDual失败！
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
			void update(); // 更新模型(初始化解集)
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
			OpFloat getObjValue(RobustStageType type) const; // 获取目标函数解
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

		/*
			OPUA中用于求解两阶段鲁棒模型的CCG算法
			求解参数说明：
				OPUA.Algorithm.CCG.IterMax / OpLInt / 1000 / [0, inf] / 注释：CCG迭代次数上限
				OPUA.Algorithm.CCG.CCGGap / OpFloat / 1e-5 / [0, 1] / 注释：CCG收敛判据
				OPUA.Algorithm.CCG.CCGLBGap / OpFloat / 1e-5 / [0, 1] / 注释：CCG下界无改善判据
				OPUA.Algorithm.CCG.CCGUBGap / OpFloat / 1e-5 / [0, 1] / 注释：CCG上界无改善判据
				OPUA.Algorithm.CCG.LogOutput / OpBool / false / {true, false} / 注释：是否输出日志
				OPUA.Algorithm.CCG.LogOutputPath / OpStr / "Log.txt" / {any valid path} / 注释：日志输出路径(需要路径合法)
				OPUA.Algorithm.CCG.SubProbSolveMode/ OpLInt / 0 / {0, 1, 2} / 注释：CCG子问题求解模式：暂时不起作用
				OPUA.Algorithm.CCG.NoImprovementIterLimit / OpLInt / 3 / [2, inf] / 注释：CCG上下界最大累计无改善次数(超过限制会认为已经无法继续优化而退出)
				OPUA.Algorithm.CCG.FirstStageInitMode / OpLInt / 0 / {0, 1, 2} /  注释：CCG一阶段变量初始化模式：0-0值初始化 / 1-给定初值 / 2-确定性解
				OPUA.Algorithm.CCG.GetDeterministicSolution / OpBool / false / {true, false} / 注释：是否作为确定性模型求解(需要给定不确定集初值)
				OPUA.Algorithm.CCG.MIPTimeLimit / OpFloat / 3600 / [0, inf] / 注释：MIP求解时间上限
				OPUA.Algorithm.CCG.MIPGap / OpFloat / 1e-5 / [0, 1] /  注释：MIP求解收敛判据
				OPUA.Algorithm.CCG.MIPSolverMode / OpChar / 'G' / {'G', 'C', 'S', 'M'} /  注释：MIP求解器选择：G-GRB / C-CPX / S-SCIP / M-MSK
		*/
		class OpAlgoCCG
		{
		protected:
			OpRobustModelI* mdl_; // 两阶段鲁棒模型

			struct OpCCGIterInfo; // CCG迭代信息
			struct OpOAIterInfo; // OA迭代信息
		public:	
			void extract(OpRobustModel model); // 抽取OPUA两阶段鲁棒模型	
			OpBool solve(const Solver::OpConfig& config); // 求解模型
		public:
			OpAlgoCCG(); // 默认构造函数
			OpAlgoCCG(OpRobustModel model); // 构造并抽取OPUA模型
		public:
			~OpAlgoCCG();
		};

		// 为CCG算法生成默认配置
		void DefaultCfg4CCG(Solver::OpConfig& config);
		Solver::OpConfig DefaultCfg4CCG();
	}
}