#pragma once

#include "OPUA.Solver.h"

namespace OPUA
{
	namespace Algorithm
	{
		class OpMSSPModelI;
		class OpMSSPModel;
		class OpAlgoBD;
		class OpRobustModelI;
		class OpRobustModel;
		class OpAlgoCCG;
		class OpHierarchicalModelI;
		class OpHierarchicalModel;
		class OpAlgoATC;

		class OpAutoKKTSet;
		class OpAutoDualSet;
		typedef std::pair<Variable::OpVarArr, Constraint::OpLCArr> OpLinearization;

		// OPUA两阶段多场景随机规划模型阶段类型
		enum class MSSPStageType
		{
			Unknown, // 未知阶段(仅作为默认值保留)
			FirstStage, // 第一阶段问题
			SecondStagePrimal, // 第二阶段原始问题
			SecondStageDual // 第二阶段对偶问题
		};

		/*
			OPUA两阶段多场景随机规划问题类

			标准形式：
				第一阶段：

				第二阶段

			其中：
				第二阶段对偶约束

		*/
		class OpMSSPModel
			: public OpBase
		{
		public:	
			void update(); // 更新模型(初始化解集等)
			OpLInt addSubProb(); // 添加并初始化子问题(自动分配问题编号)
			void addSubProb(OpLInt idx); // 添加并初始化子问题(手动分配问题编号)
			void add(Variable::OpVarArr vars); // 添加变量(主问题)
			void add(Variable::OpVar var); // 添加变量(主问题)
			void add(Constraint::OpLCArr cons); // 添加约束(主问题)
			void add(Constraint::OpLinCon con); // 添加约束(主问题)
			void add(Variable::OpVarArr vars, OpLInt idx); // 添加变量(子问题)
			void add(Variable::OpVar var, OpLInt idx); // 添加变量(子问题)
			void add(Constraint::OpLCArr cons, OpLInt idx); // 添加约束(子问题)
			void add(Constraint::OpLinCon con, OpLInt idx);	// 添加约束(子问题)
			void setObj(const Expression::OpLinExpr& expr); // 设置目标函数(主问题)
			void setObj(const Expression::OpLinExpr& expr, OpLInt idx); // 设置目标函数(子问题)
			void setValue(Variable::OpVar var, OpFloat val); // 设置解(主问题)
			void setValue(Variable::OpVar var, OpFloat val, OpLInt idx); // 设置解(子问题)
			void setDualValue(Constraint::OpLinCon con, OpFloat val, OpLInt idx); // 设置对偶解(子问题)
			OpFloat getValue(Variable::OpVar var) const; // 获取解(主问题)
			OpFloat getValue(Variable::OpVar var, OpLInt idx) const; // 获取解(子问题)
			OpFloat getDualValue(Constraint::OpLinCon con, OpLInt idx) const; // 获取对偶解(子问题)
			OpFloat getObjValue() const; // 获取目标函数解(主问题)
			OpFloat getObjValue(OpLInt idx) const; // 获取目标函数解(子问题)
			OpULInt getSize(OpBool isVar) const; // 获取变量&约束数目(主问题)
			OpULInt getSize(OpBool isVar, OpLInt idx) const; // 获取变量&约束数目(子问题)
			OpBool isSubProb(OpLInt idx) const; // idx是否为节点的合法索引
			void write(OpStr root); // 导出模型
			OpMSSPModelI* getImpl() const; // 获取impl
			void release(); // 释放内存
		public:
			OpBool operator==(const OpMSSPModel& model) const;
			OpBool operator!=(const OpMSSPModel& model) const;
		public:
			OpMSSPModel(); // 默认构造函数(默认为空)
			OpMSSPModel(OpMSSPModelI* impl); // 从impl构造
			OpMSSPModel(OpEnv env); // 从env构造
			OpMSSPModel(OpEnv env, OpStr name); // 从env构造并指定部分参数
		public:
			virtual ~OpMSSPModel();
		};

		/*
			OPUA中用于求解两阶段随即规划模型的经典Benders算法
			求解参数说明：
				OPUA.Algorithm.BD.IterMax / OpLInt / 1000 / [0, inf] / 注释：BD迭代次数上限
				OPUA.Algorithm.BD.BDGap / OpFloat / 1e-5 / [0, 1] / 注释：BD收敛判据
				OPUA.Algorithm.BD.LogOutput / OpBool / false / {true, false} / 注释：是否输出日志
				OPUA.Algorithm.BD.LogOutputPath / OpStr / "Log.txt" / {any valid path} / 注释：日志输出路径(需要路径合法)
				OPUA.Algorithm.BD.FirstStageInitMode / OpLInt / 0 / {0, 1, 2} /  注释：BD一阶段变量初始化模式：0-0值初始化 / 1-给定初值
				OPUA.Algorithm.BD.MIPSolverMode / OpChar / 'C' / {'G', 'C', 'S', 'M', 'T'} /  注释：MIP求解器选择：G-GRB / C-CPX / S-SCIP / M-MSK / T-COPT
			备注：
				[1] 用户需要保证第二阶段问题的可行性(为约束添加松弛变量)，BD算法不求解可行子问题，不生成可行割
				[2] 最优割采用单割形式构造
				[3] 用户需要在求解器配置中关闭求解器的预求解功能
		*/
		class OpAlgoBD
		{
		protected:
			OpMSSPModelI* mdl_; // 两阶段多场景随机规划模型

			struct OpBDIterInfo; // BD迭代信息
		public:
			void extract(OpMSSPModel model); // 抽取OPUA两阶段多场景随机规划模型
			OpBool solve(const Solver::OpConfig& config); // 求解模型
		public:
			OpAlgoBD(); // 默认构造函数
			OpAlgoBD(OpMSSPModel model); // 构造并抽取OPUA模型
		public:
			~OpAlgoBD();
		};

		// 为BD算法生成默认配置
		void DefaultCfg4BD(Solver::OpConfig& config);
		// 为BD算法生成默认配置
		Solver::OpConfig DefaultCfg4BD();


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
						写成expr>=lb实际中会被加载为-expr<=-lb，会导致autoDual失败！如果采用非标准约束，必须在执行autoDual前先执行autoStd
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
			void autoStd(RobustStageType type); // 按阶段自动执行约束的标准化(在autoDual之前执行)
			void autoDual(); // 自动推导对偶
			void autoBound(); // 自动推导变量的上下界
			void update(); // 更新模型(初始化解集)
			void add(Variable::OpVarArr vars, RobustStageType type, OpBool flag); // 添加变量
			void add(Variable::OpVar var, RobustStageType type, OpBool flag); // 添加变量
			void add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag); // 添加约束
			void add(Constraint::OpLinCon con, RobustStageType type, OpBool flag); // 添加约束
			void add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag); // 添加约束
			void add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag); // 添加约束	
			void setObj(const Expression::OpLinExpr& expr, RobustStageType type); // 设置目标函数
			void setObj(const Expression::OpQuadExpr& expr, RobustStageType type); // 设置目标函数
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
				OPUA.Algorithm.CCG.SubProbSolveMode/ OpLInt / 0 / {0, 1, 2} / 注释：CCG子问题求解模式：0-OA(外逼近算法) / 1-AD(交替方向迭代算法)
				OPUA.Algorithm.CCG.NoImprovementIterLimit / OpLInt / 3 / [2, inf] / 注释：CCG上下界最大累计无改善次数(超过限制会认为已经无法继续优化而退出)
				OPUA.Algorithm.CCG.FirstStageInitMode / OpLInt / 0 / {0, 1, 2} /  注释：CCG一阶段变量初始化模式：0-0值初始化 / 1-给定初值 / 2-确定性解
				OPUA.Algorithm.CCG.GetDeterministicSolution / OpBool / false / {true, false} / 注释：是否作为确定性模型求解(需要给定不确定集初值)
				OPUA.Algorithm.CCG.MIPTimeLimit / OpFloat / 3600 / [0, inf] / 注释：MIP求解时间上限
				OPUA.Algorithm.CCG.MIPGap / OpFloat / 1e-5 / [0, 1] /  注释：MIP求解收敛判据
				OPUA.Algorithm.CCG.MIPSolverMode / OpChar / 'G' / {'G', 'C', 'S', 'M', 'T'} /  注释：MIP求解器选择：G-GRB / C-CPX / S-SCIP / M-MSK / T-COPT
		*/
		class OpAlgoCCG
		{
		protected:
			OpRobustModelI* mdl_; // 两阶段鲁棒模型

			struct OpCCGIterInfo; // CCG迭代信息
			struct OpOAIterInfo; // OA迭代信息
			struct OpADIterInfo; // AD迭代信息
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
		// 为CCG算法生成默认配置
		Solver::OpConfig DefaultCfg4CCG();

		/*
			OPUA分层模型类
			标准形式：
					子系统优化模型：
						\min_{x_{ij}, r_{ij}, t_{(i+1)k}} f_{ij} + \lambda_{ij}^{T}(t_{ij}-r_{ij})
																		  + \sum\lambda_{(i+1)k}^{T}(t_{(i+1)k}-r_{(i+1)k})
																		  + ||\rho_{ij}\dot(t_{ij}-r_{ij})||_{2}^{2}
																		  + \sum ||\rho_{(i+1)k}\dot(t_{(i+1)k}-r_{(i+1)k})||_{2}^{2}
						s.t. g_{ij}(x_{ij}, r_{ij}, t_{(i+1)k})\leqslant 0
							 h_{ij}(x_{ij}, r_{ij}, t_{(i+1)k})=0

				其中：
																		 ||                                 / \
																		 || 上级目标t_{ij}            || 上级反馈 r_{ij}
																		\ /                                 ||
																	------------------------------
																	|                子问题 P_{ij}                 |      设计模块 f_{ij}(x_{ij}, r_{ij}, t_{(i+1)k})
																	|     局部变量 x_{ij}                         |
																	|     局部目标 f_{ij}                         |      分析模块 \pi(r_{ij}, t_{(i+1)k})
																	|     局部约束 g_{ij}, h_{ij}               |
																	------------------------------
																		 ||                                 / \
																		 || 下级目标 t_{(i+1)k}    || 下级反馈 r_{(i+1)k ...
																		\ /                                 ||

				备注：
					[1] 该模型为树状模型，一个子问题对应一个唯一的上级问题，和零到多个下级问题；
					[2] 问题索引数字越小表示层级越高/同一层级优先级越高，一般按照问题生成顺从0(根节点)分配索引；负数表示节点不存在；
					[3] 上下层级问题之间设计指标和反馈指标的对应默认和存放顺序一致；
		*/
		class OpHierarchicalModel
			: public OpBase
		{
		public:	
			OpLInt initRoot(OpLInt idx = 0); // 初始化模型(生成根节点，默认从0开始)，并返回索引
			void initSolution(); // 初始化解(在模型形成完毕后，设置变量解、对偶变量解之前执行)
			OpLInt addLower(OpLInt idx1, OpLInt idx2 = -1); // 由一层模型idx1出发，生成他的下级模型idx2(默认自动分配)，并返回索引
			void link(OpLInt idx1, OpLInt idx2, Variable::OpVar var1, Variable::OpVar var2); // 添加idx1和idx2之间的一对关联变量var1和var2
			void link(OpLInt idx1, OpLInt idx2, Variable::OpVarArr vars1, Variable::OpVarArr vars2); // 添加idx1和idx2之间的一组关联变量var1和var2
			void add(OpLInt idx, Variable::OpVar var); // 添加idx的局部决策变量
			void add(OpLInt idx, Variable::OpVarArr vars); // 添加idx的局部决策变量
			void add(OpLInt idx, Constraint::OpLinCon con); // 添加idx的局部线性约束
			void add(OpLInt idx, Constraint::OpLCArr cons); // 添加idx的局部线性约束集
			void add(OpLInt idx, Constraint::OpQuadCon con); // 添加idx的局部二次约束
			void add(OpLInt idx, Constraint::OpQCArr cons); // 添加idx的局部二次约束集
			void setObj(OpLInt idx, const Expression::OpQuadExpr& expr); // 设置idx的局部目标函数
			void setValue(OpLInt idx, Variable::OpVar var, OpFloat val);	 // 设置idx中变量var的解
			OpFloat getValue(OpLInt idx, Variable::OpVar var) const; // 获取idx中变量var的解
			OpFloat getObjValue(OpLInt idx) const; // 获取idx目标函数解(idx取负值则返回整体解)
			OpBool isRoot(OpLInt idx) const; // idx是否为根节点的合法索引
			OpBool isLeaf(OpLInt idx) const; // idx是否为叶节点的合法索引
			OpBool isNode(OpLInt idx) const; // idx是否为节点的合法索引
			void write(OpStr root); // 导出模型
			OpHierarchicalModelI* getImpl() const; // 获取impl
			void release(); // 释放内存
		public:
			OpBool operator==(const OpHierarchicalModel& model) const;
			OpBool operator!=(const OpHierarchicalModel& model) const;
		public:
			OpHierarchicalModel(); // 默认构造函数(默认为空)
			OpHierarchicalModel(OpHierarchicalModelI* impl); // 从impl构造
			OpHierarchicalModel(OpEnv env); // 从env构造
			OpHierarchicalModel(OpEnv env, OpStr name); // 从env构造并指定部分参数
		public:
			virtual ~OpHierarchicalModel();
		};

		/*
			OPUA中用于求解分层模型的ATC算法
			求解参数说明：
					OPUA.Algorithm.ATC.InitMode / OpLInt / 0 / {0, 1} / 注释：乘子初始化模式(0-乘子按统一值初始化 / 1-乘子按指定值初始化)
					OPUA.Algorithm.ATC.LambdaInitVal / OpFloat / 1 / [-inf, inf] / 注释：线性乘子(统一)初始值
					OPUA.Algorithm.ATC.RhoInitVal / OpFloat / 1 / [-inf, inf] / 注释：二次乘子(统一)初始值
					OPUA.Algorithm.ATC.RhoUpdateFactor / OpFloat / 2 / [1, inf] / 注释：二次乘子更新系数(建议值2-3之间)
					OPUA.Algorithm.ATC.ATCIterMax / OpLInt / 1000 / [0, inf] / 注释：最大迭代次数
					OPUA.Algorithm.ATC.ATCGap1 / OpFloat / 1e-3 / [0, 1] / 注释：ATC收敛判据(必要性条件)
					OPUA.Algorithm.ATC.ATCGap2 / OpFloat / 1e-3 / [0, 1] / 注释：ATC收敛判据(充分性条件)
					OPUA.Algorithm.ATC.ATCShakeGap / OpFloat / 1e-3 / [0, 1] / 注释：ATC震荡间隙判据
					OPUA.Algorithm.ATC.ATCShakeCount / OpLInt / 3 / [2, inf] / 注释：ATC震荡次数判据
					OPUA.Algorithm.ATC.ATCShakeAutoQuit / OpBool / true / {true, false} / 注释：ATC震荡自动退出的开关
					OPUA.Algorithm.ATC.MIPSolverMode / OpChar / 'G' / {'G', 'C', 'S', 'M', 'T'} /  注释：MIP求解器选择：G-GRB / C-CPX / S-SCIP / M-MSK / T-COPT
		*/
		class OpAlgoATC
		{
		protected:
			OpHierarchicalModelI* mdl_; // 分层模型

			struct OpATCIterInfo; // ATC迭代信息
		public:
			void extract(OpHierarchicalModel model); // 抽取OPUA分层模型	
			OpBool solve(const Solver::OpConfig& config); // 求解模型
		public:
			OpAlgoATC(); // 默认构造函数
			OpAlgoATC(OpHierarchicalModel model); // 构造并抽取OPUA模型
		public:
			~OpAlgoATC();
		};

		// 为ATC算法生成默认配置
		void DefaultCfg4ATC(Solver::OpConfig& config);
		// 为ATC算法生成默认配置
		Solver::OpConfig DefaultCfg4ATC();

		/*
			算法类别：Bilinear(整数*连续)线性化
			松弛方式：BigM
			数学形式：
				原约束形式：w = x * y			
			参数说明：
				env / OpEnv / - / - / 环境变量
				result / OpLinearization& / - / - / 线性化过程中的附加变量和约束
				w / Variable::OpVar / - / - / 代表双线性项，要求为连续变量类型(OpVarType::Con)
				x / Variable::OpVar / - / - / 双线性项变量1，要求为0-1变量(OpVarType::Bool)
				y / Variable::OpVar / - / - / 双线性项变量2，要求为连续变量类型(OpVarType::Con)
				xmin / OpFloat / - / [0, 1] / 预估的x的下限
				xmax / OpFloat / - / [0, 1] / 预估的x的上限
				ymin / OpFloat / - / [-inf, inf] / 预估的y的下限(注意：这个值会作为BigM参数)
				ymax / OpFloat / - / [-inf, inf] / 预估的y的上限(注意：这个值会作为BigM参数)
			特别说明：
				1. 对于x和y，不添加额外的Bound约束，即默认xmin <= x <= xmax, ymin <= y <= ymax
				2. 辅助约束全部为标准形式：expr >= lb / expr == lb；如果采用非标准约束，必须先执行standardize
		*/
		void OpAlgoBigMBilinear1(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax);
		OpLinearization OpAlgoBigMBilinear1(OpEnv env, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax);

		/*
			算法类别：Bilinear(连续*连续)线性化
			松弛方式：BigM
			数学形式：
				原约束形式：w = x * y
			参数说明：
				env / OpEnv / - / - / 环境变量
				result / OpLinearization& / - / - / 线性化过程中的附加变量和约束
				w / Variable::OpVar / - / - / 代表双线性项，要求为连续变量类型(OpVarType::Con)
				x / Variable::OpVar / - / - / 双线性项变量1，要求为连续变量类型(OpVarType::Con)
				y / Variable::OpVar / - / - / 双线性项变量2，要求为连续变量类型(OpVarType::Con)
				xmin / OpFloat / - / [-inf, inf]] / 预估的x的下限(注意：这个值会作为BigM参数)
				xmax / OpFloat / - / [-inf, inf] / 预估的x的上限(注意：这个值会作为BigM参数)
				ymin / OpFloat / - / [-inf, inf] / 预估的y的下限(注意：这个值会作为BigM参数)
				ymax / OpFloat / - / [-inf, inf] / 预估的y的上限(注意：这个值会作为BigM参数)
				segment / OpULInt / - / [2, inf] / 对x进行二进制采样的位数
			特别说明：
				1. 默认对x进行二进制采样，segment将决定采样精度\delta x
				2. 对于x和y，不添加额外的Bound约束，即默认xmin <= x <= xmax, ymin <= y <= ymax
				3. 辅助约束全部为标准形式：expr >= lb / expr == lb；如果采用非标准约束，必须先执行standardize
		*/
		void OpAlgoBigMBilinear2(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax, OpULInt segment);
		OpLinearization OpAlgoBigMBilinear2(OpEnv env, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax, OpULInt segment);

		/*
			算法类别：Bilinear线性化
			松弛方式：McCormick
			数学形式：
				原约束形式：w = x * y
		*/
		//void OpAlgoMCBilinear(OpEnv env, OpLinearization& result);
		//OpLinearization OpAlgoMCBilinear(OpEnv env);

		/*
			算法类别：Abs函数线性化
			松弛方式：BigM
			数学形式：
				原约束形式：w = abs(x)
			参数说明：
				env / OpEnv / - / - / 环境变量
				result / OpLinearization& / - / - / 线性化过程中的附加变量和约束
				w / Variable::OpVar / - / - / 代表绝对值项
				x / Variable::OpVar / - / - / 代表绝对值函数参数
				xamax / OpFloat / - / [0, inf] / 预估的abs(x)的上限(注意：这个值会作为BigM参数)

			特别说明：
				1. 对于x，不添加额外的Bound约束，即默认abs(x) <= xamax
				2. 辅助约束全部为标准形式：expr >= lb / expr == lb；如果采用非标准约束，必须先执行standardize
		*/
		void OpAlgoBigMAbs(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, OpFloat xamax);
		OpLinearization OpAlgoBigMAbs(OpEnv env, Variable::OpVar w, Variable::OpVar x, OpFloat xamax);

		/*
			算法类别：Max函数线性化
			松弛方式：BigM
			数学形式：
				原约束形式：w = max(x, y)
			参数说明：
				env / OpEnv / - / - / 环境变量
				result / OpLinearization& / - / - / 线性化过程中的附加变量和约束
				w / Variable::OpVar / - / - / 代表Max函数项
				x / Variable::OpVar / - / - / 代表Max函数参数1
				y / Variable::OpVar / - / - / 代表Max函数参数2
				xmin / OpFloat / - / [-inf, inf]] / 预估的x的下限(注意：这个值会作为BigM参数)
				xmax / OpFloat / - / [-inf, inf] / 预估的x的上限(注意：这个值会作为BigM参数)
				ymin / OpFloat / - / [-inf, inf] / 预估的y的下限(注意：这个值会作为BigM参数)
				ymax / OpFloat / - / [-inf, inf] / 预估的y的上限(注意：这个值会作为BigM参数)

			特别说明：
				1. 对于x，不添加额外的Bound约束，即默认abs(x) <= xamax
				2. 辅助约束全部为标准形式：expr >= lb / expr == lb；如果采用非标准约束，必须先执行standardize
		*/
		void OpAlgoBigMMax(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax);
		OpLinearization OpAlgoBigMMax(OpEnv env, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax);
		void OpAlgoBigMMax(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVarArr X,
			Container::OpFloatArr Xmin, Container::OpFloatArr Xmax);
		OpLinearization OpAlgoBigMMax(OpEnv env, Variable::OpVar w, Variable::OpVarArr X,
			Container::OpFloatArr Xmin, Container::OpFloatArr Xmax);

		/*
			算法类别：Min函数线性化
			松弛方式：BigM
			数学形式：
				原约束形式：w = min(x, y)
			参数说明：
				env / OpEnv / - / - / 环境变量
				result / OpLinearization& / - / - / 线性化过程中的附加变量和约束
				w / Variable::OpVar / - / - / 代表Min函数项
				x / Variable::OpVar / - / - / 代表Min函数参数1
				y / Variable::OpVar / - / - / 代表Min函数参数2
				xmin / OpFloat / - / [-inf, inf]] / 预估的x的下限(注意：这个值会作为BigM参数)
				xmax / OpFloat / - / [-inf, inf] / 预估的x的上限(注意：这个值会作为BigM参数)
				ymin / OpFloat / - / [-inf, inf] / 预估的y的下限(注意：这个值会作为BigM参数)
				ymax / OpFloat / - / [-inf, inf] / 预估的y的上限(注意：这个值会作为BigM参数)

			特别说明：
				1. 对于x，不添加额外的Bound约束，即默认abs(x) <= xamax
				2. 辅助约束全部为标准形式：expr >= lb / expr == lb；如果采用非标准约束，必须先执行standardize
		*/
		void OpAlgoBigMMin(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax);
		OpLinearization OpAlgoBigMMin(OpEnv env, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax);
		void OpAlgoBigMMin(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVarArr X,
			Container::OpFloatArr Xmin, Container::OpFloatArr Xmax);
		OpLinearization OpAlgoBigMMin(OpEnv env, Variable::OpVar w, Variable::OpVarArr X,
			Container::OpFloatArr Xmin, Container::OpFloatArr Xmax);

		class OpAutoKKTSet
		{
		public:
			Variable::OpVarArr primalVar; /*原始变量*/
			Variable::OpVarArr dualVar; /*对偶变量*/
			Variable::OpVarArr slackVar; /*松弛变量*/
			Constraint::OpLCArr gradCon; /*梯度约束*/
			Constraint::OpLCArr primalCon; /*原始可行性约束*/
			Constraint::OpLCArr dualCon; /*对偶可行性约束*/
			Constraint::OpLCArr csCon1; /*互补松弛约束(线性)*/
			Constraint::OpQCArr csCon2; /*互补松弛约束(二次)*/
		public:
			void init(OpEnv env); //初始化
			void clear(OpBool deep); // 清空(deep为true时将执行深度清理，释放数组中的所有元素)
		public:
			OpAutoKKTSet();
			OpAutoKKTSet(OpEnv env);
		};

		/*
			算法类别：自动KKT条件推导
			松弛方式：Bilinear(不松驰) / BigM
			数学形式：
				原问题形式：
					\min_{x,y} c^{T}*x+d^{T}*y
					s.t.   A*x+B*y>=b: \lambda
					       G*x+D*y=g: \mu
						   x>=0
				KKT条件形式：
					A^{T}*\lambda-G^{T}*\mu<=c
					B^{T}*\lambda-D^{T}*\mu=d
					A*x+B*y>=b
					G*x+D*y=g
					\lambda>=0
					\lambda^{T}*(A*x+B*y-b)=0
			参数说明：
				env / OpEnv / - / - / 环境变量
				result / OpAutoKKTSet& / - / - / KKT条件(包含变量和约束)
				X0 / Variable::OpVarArr / - / - / 原问题变量集(非负变量)
				Y0 / Variable::OpVarArr / - / - / 原问题变量集(无限制变量)
				NEC0 / Constraint::OpLCArr / - / - / 原问题不等式约束条件集
				EC0 / Constraint::OpLCArr / - / - / 原问题等式约束条件集
				O0 / const Expression::OpLinExpr& / - / - / 原问题目标函数
				M / Container::OpFloatArr / - / - / 互补松弛约束的BigM参数
				relax / OpBool / - / {true, false} / 是否对互补松弛约束进行松弛
			特别说明：
				1. result中primalCon按照[不等式约束，等式约束]的顺序排列，对偶变量dualVar自动生成并与primalCon的顺序对应，为[非负对偶变量，无限制对偶变量]；gradCon按照[不等式约束，等式约束]的顺序排列
				2. 原问题约束NEC0和EC0必须要写成标准的小于等于约束A*x+B*y>=b或标准的等式约束G*x+D*y，对应的对偶变量分别为lambda(非负)和mu(无限制)
				3. 使用“<=”、“>=”符号生成的线性约束必须将常量和含变量的表达式进行分离，且常量应作为左操作数，即：lb<=expr、ub>=expr写成expr>=lb实际中会被加载为-expr<=-lb，会导致AutoKKT失败！；如果采用非标准约束，必须先执行standardize
				4. relax设为true时，对互补松弛约束进行BigM松弛(需要自行设置M并保证M长度与NEC0相等)并放入result中的csCon1，松弛变量加入result；设置为false时，互补松弛约束为二次约束并放入result中的csCon2(M可以为空)
		*/
		void OpAlgoAutoKKT(OpEnv env, OpAutoKKTSet& result, Variable::OpVarArr X0, Variable::OpVarArr Y0, Constraint::OpLCArr NEC0, Constraint::OpLCArr EC0, const Expression::OpLinExpr& O0,
			Container::OpFloatArr M, OpBool relax);
		OpAutoKKTSet OpAlgoAutoKKT(OpEnv env, Variable::OpVarArr X0, Variable::OpVarArr Y0, Constraint::OpLCArr NEC0, Constraint::OpLCArr EC0, const Expression::OpLinExpr& O0,
			Container::OpFloatArr M, OpBool relax);

		class OpAutoDualSet
		{
		public:
			Variable::OpVarArr dualVar; /*对偶问题变量*/
			Constraint::OpLCArr dualCon; /*对偶问题约束*/
			Expression::OpLinExpr dualObj; /*对偶问题目标函数*/
		public:
			void init(OpEnv env); //初始化
			void clear(OpBool deep); // 清空(deep为true时将执行深度清理，释放数组中的所有元素)
		public:
			OpAutoDualSet();
			OpAutoDualSet(OpEnv env);
		};

		/*
			算法类别：自动对偶问题推导
			数学形式：
				原问题形式：
					\min c^{T}*x+d^{T}*y
					s.t.   A*x+B*y>=b: \lambda
						   G*x+D*y=g: \mu
						   x>=0
				对偶问题形式：
					\max_{\lambda,\mu} b^T*\lambda+g^T*\mu
					s.t.   A^{T}*\lambda+G^{T}*\mu<=c: x
						   B^{T}*\lambda+D^{T}*\mu=d: y
						   \lambda>=0
			参数说明：
				env / OpEnv / - / - / 环境变量
				result / OpAutoDualSet& / - / - / 对偶问题
				X0 / Variable::OpVarArr / - / - / 原问题变量集(非负变量)
				Y0 / Variable::OpVarArr / - / - / 原问题变量集(无限制变量)
				NEC0 / Constraint::OpLCArr / - / - / 原问题不等式约束条件集
				EC0 / Constraint::OpLCArr / - / - / 原问题等式约束条件集
				O0 / const Expression::OpLinExpr& / - / - / 原问题目标函数
			特别说明：
				1. result中对偶变量dualVar自动生成，按照[非负对偶变量，无限制对偶变量]的顺序排列，并[不等式约束，等式约束]的顺序对应
				2. result中对偶约束dualCon自动生成，按照[不等式约束，等式约束]的顺序排列，并与[非负原问题变量，无限制原问题变量]的顺序对应
				3. 原问题约束NEC0和EC0必须要写成标准的小于等于约束A*x+B*y>=b或标准的等式约束G*x+D*y，对应的对偶变量分别为lambda(非负)和mu(无限制)
				4. 使用“<=”、“>=”符号生成的线性约束必须将常量和含变量的表达式进行分离，且常量应作为左操作数，即：lb<=expr、ub>=expr写成expr>=lb实际中会被加载为-expr<=-lb，会导致AutoDual失败！；如果采用非标准约束，必须先执行standardize
		*/
		void OpAlgoAutoDual(OpEnv env, OpAutoDualSet& result, Variable::OpVarArr X0, Variable::OpVarArr Y0, Constraint::OpLCArr NEC0, Constraint::OpLCArr EC0, const Expression::OpLinExpr& O0);
		OpAutoDualSet OpAlgoAutoDual(OpEnv env, Variable::OpVarArr X0, Variable::OpVarArr Y0, Constraint::OpLCArr NEC0, Constraint::OpLCArr EC0, const Expression::OpLinExpr& O0);
	}
}