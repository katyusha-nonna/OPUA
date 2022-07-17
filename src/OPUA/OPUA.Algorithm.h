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

		// OPUA���׶ζೡ������滮ģ�ͽ׶�����
		enum class MSSPStageType
		{
			Unknown, // δ֪�׶�(����ΪĬ��ֵ����)
			FirstStage, // ��һ�׶�����
			SecondStagePrimal, // �ڶ��׶�ԭʼ����
			SecondStageDual // �ڶ��׶ζ�ż����
		};

		/*
			OPUA���׶ζೡ������滮������

			��׼��ʽ��
				��һ�׶Σ�

				�ڶ��׶�

			���У�
				�ڶ��׶ζ�żԼ��

		*/
		class OpMSSPModel
			: public OpBase
		{
		public:	
			void update(); // ����ģ��(��ʼ���⼯��)
			OpLInt addSubProb(); // ��Ӳ���ʼ��������(�Զ�����������)
			void addSubProb(OpLInt idx); // ��Ӳ���ʼ��������(�ֶ�����������)
			void add(Variable::OpVarArr vars); // ��ӱ���(������)
			void add(Variable::OpVar var); // ��ӱ���(������)
			void add(Constraint::OpLCArr cons); // ���Լ��(������)
			void add(Constraint::OpLinCon con); // ���Լ��(������)
			void add(Variable::OpVarArr vars, OpLInt idx); // ��ӱ���(������)
			void add(Variable::OpVar var, OpLInt idx); // ��ӱ���(������)
			void add(Constraint::OpLCArr cons, OpLInt idx); // ���Լ��(������)
			void add(Constraint::OpLinCon con, OpLInt idx);	// ���Լ��(������)
			void setObj(const Expression::OpLinExpr& expr); // ����Ŀ�꺯��(������)
			void setObj(const Expression::OpLinExpr& expr, OpLInt idx); // ����Ŀ�꺯��(������)
			void setValue(Variable::OpVar var, OpFloat val); // ���ý�(������)
			void setValue(Variable::OpVar var, OpFloat val, OpLInt idx); // ���ý�(������)
			void setDualValue(Constraint::OpLinCon con, OpFloat val, OpLInt idx); // ���ö�ż��(������)
			OpFloat getValue(Variable::OpVar var) const; // ��ȡ��(������)
			OpFloat getValue(Variable::OpVar var, OpLInt idx) const; // ��ȡ��(������)
			OpFloat getDualValue(Constraint::OpLinCon con, OpLInt idx) const; // ��ȡ��ż��(������)
			OpFloat getObjValue() const; // ��ȡĿ�꺯����(������)
			OpFloat getObjValue(OpLInt idx) const; // ��ȡĿ�꺯����(������)
			OpULInt getSize(OpBool isVar) const; // ��ȡ����&Լ����Ŀ(������)
			OpULInt getSize(OpBool isVar, OpLInt idx) const; // ��ȡ����&Լ����Ŀ(������)
			OpBool isSubProb(OpLInt idx) const; // idx�Ƿ�Ϊ�ڵ�ĺϷ�����
			void write(OpStr root); // ����ģ��
			OpMSSPModelI* getImpl() const; // ��ȡimpl
			void release(); // �ͷ��ڴ�
		public:
			OpBool operator==(const OpMSSPModel& model) const;
			OpBool operator!=(const OpMSSPModel& model) const;
		public:
			OpMSSPModel(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpMSSPModel(OpMSSPModelI* impl); // ��impl����
			OpMSSPModel(OpEnv env); // ��env����
			OpMSSPModel(OpEnv env, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpMSSPModel();
		};

		/*
			OPUA������������׶��漴�滮ģ�͵ľ���Benders�㷨
			������˵����
				OPUA.Algorithm.BD.IterMax / OpLInt / 1000 / [0, inf] / ע�ͣ�BD������������
				OPUA.Algorithm.BD.BDGap / OpFloat / 1e-5 / [0, 1] / ע�ͣ�BD�����о�
				OPUA.Algorithm.BD.LogOutput / OpBool / false / {true, false} / ע�ͣ��Ƿ������־
				OPUA.Algorithm.BD.LogOutputPath / OpStr / "Log.txt" / {any valid path} / ע�ͣ���־���·��(��Ҫ·���Ϸ�)
				OPUA.Algorithm.BD.FirstStageInitMode / OpLInt / 0 / {0, 1, 2} /  ע�ͣ�BDһ�׶α�����ʼ��ģʽ��0-0ֵ��ʼ�� / 1-������ֵ
				OPUA.Algorithm.BD.MIPSolverMode / OpChar / 'C' / {'G', 'C', 'S', 'M', 'T'} /  ע�ͣ�MIP�����ѡ��G-GRB / C-CPX / S-SCIP / M-MSK / T-COPT
			��ע��
				[1] �û���Ҫ��֤�ڶ��׶�����Ŀ�����(ΪԼ������ɳڱ���)��BD�㷨�������������⣬�����ɿ��и�
				[2] ���Ÿ���õ�����ʽ����
				[3] �û���Ҫ������������йر��������Ԥ��⹦��
		*/
		class OpAlgoBD
		{
		protected:
			OpMSSPModelI* mdl_; // ���׶ζೡ������滮ģ��

			struct OpBDIterInfo; // BD������Ϣ
		public:
			void extract(OpMSSPModel model); // ��ȡOPUA���׶ζೡ������滮ģ��
			OpBool solve(const Solver::OpConfig& config); // ���ģ��
		public:
			OpAlgoBD(); // Ĭ�Ϲ��캯��
			OpAlgoBD(OpMSSPModel model); // ���첢��ȡOPUAģ��
		public:
			~OpAlgoBD();
		};

		// ΪBD�㷨����Ĭ������
		void DefaultCfg4BD(Solver::OpConfig& config);
		// ΪBD�㷨����Ĭ������
		Solver::OpConfig DefaultCfg4BD();


		// OPUA���׶�³��ģ�ͽ׶�����
		enum class RobustStageType
		{
			Unknown, // δ֪�׶�(����ΪĬ��ֵ����)
			FirstStage, // ��һ�׶�����
			FirstStageSpecial, // ��һ�׶���������(�������Ż��������ڷ����Է�����)
			SecondStagePrimal, // �ڶ��׶�ԭʼ����
			SecondStageDual, // �ڶ��׶ζ�ż����
			Uncertainty // ��ȷ����
		};

		/*
			OPUA���׶�³��ģ����
			�ڶ��׶�ԭ����/��ż�������д�ɱ�׼��ʽ(��ѧ��ʽ�ϸ������¹�ʽ����)
			ע�⣺ʹ�á�<=������>=���������ɵ�����/����Լ�����뽫�����ͺ������ı��ʽ���з��룬�ҳ���Ӧ��Ϊ�������������lb<=expr��ub>=expr
						д��expr>=lbʵ���лᱻ����Ϊ-expr<=-lb���ᵼ��autoDualʧ�ܣ�������÷Ǳ�׼Լ����������ִ��autoDualǰ��ִ��autoStd
			��׼��ʽ��
				��һ�׶Σ�
					\min_{y} c^{T}y+\eta
					s.t. Ay>=d
							\eta>=b^{T}x^{l}: l=1,...,r
							Ey+Gx^{l}>=h-Mu_{l}: l=1,...,r
							y\in S_{y}, x^{l}\in S_{x}: l=1,...,r
				�ڶ��׶�
					\max_{u\in U}\min_{x} b^{T}x
					s.t. Gx>=h-Ey-Mu
							x\in S_{x}
			���У�
				�ڶ��׶ζ�żԼ��
					G^{T}\pi <=b
		*/
		class OpRobustModel
			: public OpBase
		{
		public:
			void autoStd(RobustStageType type); // ���׶��Զ�ִ��Լ���ı�׼��(��autoDual֮ǰִ��)
			void autoDual(); // �Զ��Ƶ���ż
			void autoBound(); // �Զ��Ƶ����������½�
			void update(); // ����ģ��(��ʼ���⼯)
			void add(Variable::OpVarArr vars, RobustStageType type, OpBool flag); // ��ӱ���
			void add(Variable::OpVar var, RobustStageType type, OpBool flag); // ��ӱ���
			void add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag); // ���Լ��
			void add(Constraint::OpLinCon con, RobustStageType type, OpBool flag); // ���Լ��
			void add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag); // ���Լ��
			void add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag); // ���Լ��	
			void setObj(const Expression::OpLinExpr& expr, RobustStageType type); // ����Ŀ�꺯��
			void setObj(const Expression::OpQuadExpr& expr, RobustStageType type); // ����Ŀ�꺯��
			void setValue(Variable::OpVar var, RobustStageType type, OpFloat val);	// ���ý�
			void setBound(Variable::OpVar var, RobustStageType type, OpFloat val, OpBool lb); // ���ñ߽�
			void setBound(Variable::OpVar var, RobustStageType type, OpFloat lbVal, OpFloat ubVal); // ���ñ߽�
			OpFloat getValue(Variable::OpVar var, RobustStageType type) const; // ��ȡ��
			OpFloat getObjValue(RobustStageType type) const; // ��ȡĿ�꺯����
			OpULInt getSize(RobustStageType type, OpBool isVar) const; // ��ȡ����&Լ����Ŀ
			OpFloat getBound(Variable::OpVar var, RobustStageType type, OpBool lb) const; // ��ȡ�߽�
			void write(OpStr root); // ����ģ��
			OpRobustModelI* getImpl() const; // ��ȡimpl
			void release(); // �ͷ��ڴ�
		public:
			OpBool operator==(const OpRobustModel& model) const;
			OpBool operator!=(const OpRobustModel& model) const;
		public:
			OpRobustModel(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpRobustModel(OpRobustModelI* impl); // ��impl����
			OpRobustModel(OpEnv env); // ��env����
			OpRobustModel(OpEnv env, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpRobustModel();
		};

		/*
			OPUA������������׶�³��ģ�͵�CCG�㷨
			������˵����
				OPUA.Algorithm.CCG.IterMax / OpLInt / 1000 / [0, inf] / ע�ͣ�CCG������������
				OPUA.Algorithm.CCG.CCGGap / OpFloat / 1e-5 / [0, 1] / ע�ͣ�CCG�����о�
				OPUA.Algorithm.CCG.CCGLBGap / OpFloat / 1e-5 / [0, 1] / ע�ͣ�CCG�½��޸����о�
				OPUA.Algorithm.CCG.CCGUBGap / OpFloat / 1e-5 / [0, 1] / ע�ͣ�CCG�Ͻ��޸����о�
				OPUA.Algorithm.CCG.LogOutput / OpBool / false / {true, false} / ע�ͣ��Ƿ������־
				OPUA.Algorithm.CCG.LogOutputPath / OpStr / "Log.txt" / {any valid path} / ע�ͣ���־���·��(��Ҫ·���Ϸ�)
				OPUA.Algorithm.CCG.SubProbSolveMode/ OpLInt / 0 / {0, 1, 2} / ע�ͣ�CCG���������ģʽ��0-OA(��ƽ��㷨) / 1-AD(���淽������㷨)
				OPUA.Algorithm.CCG.NoImprovementIterLimit / OpLInt / 3 / [2, inf] / ע�ͣ�CCG���½�����ۼ��޸��ƴ���(�������ƻ���Ϊ�Ѿ��޷������Ż����˳�)
				OPUA.Algorithm.CCG.FirstStageInitMode / OpLInt / 0 / {0, 1, 2} /  ע�ͣ�CCGһ�׶α�����ʼ��ģʽ��0-0ֵ��ʼ�� / 1-������ֵ / 2-ȷ���Խ�
				OPUA.Algorithm.CCG.GetDeterministicSolution / OpBool / false / {true, false} / ע�ͣ��Ƿ���Ϊȷ����ģ�����(��Ҫ������ȷ������ֵ)
				OPUA.Algorithm.CCG.MIPTimeLimit / OpFloat / 3600 / [0, inf] / ע�ͣ�MIP���ʱ������
				OPUA.Algorithm.CCG.MIPGap / OpFloat / 1e-5 / [0, 1] /  ע�ͣ�MIP��������о�
				OPUA.Algorithm.CCG.MIPSolverMode / OpChar / 'G' / {'G', 'C', 'S', 'M', 'T'} /  ע�ͣ�MIP�����ѡ��G-GRB / C-CPX / S-SCIP / M-MSK / T-COPT
		*/
		class OpAlgoCCG
		{
		protected:
			OpRobustModelI* mdl_; // ���׶�³��ģ��

			struct OpCCGIterInfo; // CCG������Ϣ
			struct OpOAIterInfo; // OA������Ϣ
			struct OpADIterInfo; // AD������Ϣ
		public:	
			void extract(OpRobustModel model); // ��ȡOPUA���׶�³��ģ��	
			OpBool solve(const Solver::OpConfig& config); // ���ģ��
		public:
			OpAlgoCCG(); // Ĭ�Ϲ��캯��
			OpAlgoCCG(OpRobustModel model); // ���첢��ȡOPUAģ��
		public:
			~OpAlgoCCG();
		};

		// ΪCCG�㷨����Ĭ������
		void DefaultCfg4CCG(Solver::OpConfig& config);
		// ΪCCG�㷨����Ĭ������
		Solver::OpConfig DefaultCfg4CCG();

		/*
			OPUA�ֲ�ģ����
			��׼��ʽ��
					��ϵͳ�Ż�ģ�ͣ�
						\min_{x_{ij}, r_{ij}, t_{(i+1)k}} f_{ij} + \lambda_{ij}^{T}(t_{ij}-r_{ij})
																		  + \sum\lambda_{(i+1)k}^{T}(t_{(i+1)k}-r_{(i+1)k})
																		  + ||\rho_{ij}\dot(t_{ij}-r_{ij})||_{2}^{2}
																		  + \sum ||\rho_{(i+1)k}\dot(t_{(i+1)k}-r_{(i+1)k})||_{2}^{2}
						s.t. g_{ij}(x_{ij}, r_{ij}, t_{(i+1)k})\leqslant 0
							 h_{ij}(x_{ij}, r_{ij}, t_{(i+1)k})=0

				���У�
																		 ||                                 / \
																		 || �ϼ�Ŀ��t_{ij}            || �ϼ����� r_{ij}
																		\ /                                 ||
																	------------------------------
																	|                ������ P_{ij}                 |      ���ģ�� f_{ij}(x_{ij}, r_{ij}, t_{(i+1)k})
																	|     �ֲ����� x_{ij}                         |
																	|     �ֲ�Ŀ�� f_{ij}                         |      ����ģ�� \pi(r_{ij}, t_{(i+1)k})
																	|     �ֲ�Լ�� g_{ij}, h_{ij}               |
																	------------------------------
																		 ||                                 / \
																		 || �¼�Ŀ�� t_{(i+1)k}    || �¼����� r_{(i+1)k ...
																		\ /                                 ||

				��ע��
					[1] ��ģ��Ϊ��״ģ�ͣ�һ���������Ӧһ��Ψһ���ϼ����⣬���㵽����¼����⣻
					[2] ������������ԽС��ʾ�㼶Խ��/ͬһ�㼶���ȼ�Խ�ߣ�һ�㰴����������˳��0(���ڵ�)����������������ʾ�ڵ㲻���ڣ�
					[3] ���²㼶����֮�����ָ��ͷ���ָ��Ķ�ӦĬ�Ϻʹ��˳��һ�£�
		*/
		class OpHierarchicalModel
			: public OpBase
		{
		public:	
			OpLInt initRoot(OpLInt idx = 0); // ��ʼ��ģ��(���ɸ��ڵ㣬Ĭ�ϴ�0��ʼ)������������
			void initSolution(); // ��ʼ����(��ģ���γ���Ϻ����ñ����⡢��ż������֮ǰִ��)
			OpLInt addLower(OpLInt idx1, OpLInt idx2 = -1); // ��һ��ģ��idx1���������������¼�ģ��idx2(Ĭ���Զ�����)������������
			void link(OpLInt idx1, OpLInt idx2, Variable::OpVar var1, Variable::OpVar var2); // ���idx1��idx2֮���һ�Թ�������var1��var2
			void link(OpLInt idx1, OpLInt idx2, Variable::OpVarArr vars1, Variable::OpVarArr vars2); // ���idx1��idx2֮���һ���������var1��var2
			void add(OpLInt idx, Variable::OpVar var); // ���idx�ľֲ����߱���
			void add(OpLInt idx, Variable::OpVarArr vars); // ���idx�ľֲ����߱���
			void add(OpLInt idx, Constraint::OpLinCon con); // ���idx�ľֲ�����Լ��
			void add(OpLInt idx, Constraint::OpLCArr cons); // ���idx�ľֲ�����Լ����
			void add(OpLInt idx, Constraint::OpQuadCon con); // ���idx�ľֲ�����Լ��
			void add(OpLInt idx, Constraint::OpQCArr cons); // ���idx�ľֲ�����Լ����
			void setObj(OpLInt idx, const Expression::OpQuadExpr& expr); // ����idx�ľֲ�Ŀ�꺯��
			void setValue(OpLInt idx, Variable::OpVar var, OpFloat val);	 // ����idx�б���var�Ľ�
			OpFloat getValue(OpLInt idx, Variable::OpVar var) const; // ��ȡidx�б���var�Ľ�
			OpFloat getObjValue(OpLInt idx) const; // ��ȡidxĿ�꺯����(idxȡ��ֵ�򷵻������)
			OpBool isRoot(OpLInt idx) const; // idx�Ƿ�Ϊ���ڵ�ĺϷ�����
			OpBool isLeaf(OpLInt idx) const; // idx�Ƿ�ΪҶ�ڵ�ĺϷ�����
			OpBool isNode(OpLInt idx) const; // idx�Ƿ�Ϊ�ڵ�ĺϷ�����
			void write(OpStr root); // ����ģ��
			OpHierarchicalModelI* getImpl() const; // ��ȡimpl
			void release(); // �ͷ��ڴ�
		public:
			OpBool operator==(const OpHierarchicalModel& model) const;
			OpBool operator!=(const OpHierarchicalModel& model) const;
		public:
			OpHierarchicalModel(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpHierarchicalModel(OpHierarchicalModelI* impl); // ��impl����
			OpHierarchicalModel(OpEnv env); // ��env����
			OpHierarchicalModel(OpEnv env, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpHierarchicalModel();
		};

		/*
			OPUA���������ֲ�ģ�͵�ATC�㷨
			������˵����
					OPUA.Algorithm.ATC.InitMode / OpLInt / 0 / {0, 1} / ע�ͣ����ӳ�ʼ��ģʽ(0-���Ӱ�ͳһֵ��ʼ�� / 1-���Ӱ�ָ��ֵ��ʼ��)
					OPUA.Algorithm.ATC.LambdaInitVal / OpFloat / 1 / [-inf, inf] / ע�ͣ����Գ���(ͳһ)��ʼֵ
					OPUA.Algorithm.ATC.RhoInitVal / OpFloat / 1 / [-inf, inf] / ע�ͣ����γ���(ͳһ)��ʼֵ
					OPUA.Algorithm.ATC.RhoUpdateFactor / OpFloat / 2 / [1, inf] / ע�ͣ����γ��Ӹ���ϵ��(����ֵ2-3֮��)
					OPUA.Algorithm.ATC.ATCIterMax / OpLInt / 1000 / [0, inf] / ע�ͣ�����������
					OPUA.Algorithm.ATC.ATCGap1 / OpFloat / 1e-3 / [0, 1] / ע�ͣ�ATC�����о�(��Ҫ������)
					OPUA.Algorithm.ATC.ATCGap2 / OpFloat / 1e-3 / [0, 1] / ע�ͣ�ATC�����о�(���������)
					OPUA.Algorithm.ATC.ATCShakeGap / OpFloat / 1e-3 / [0, 1] / ע�ͣ�ATC�𵴼�϶�о�
					OPUA.Algorithm.ATC.ATCShakeCount / OpLInt / 3 / [2, inf] / ע�ͣ�ATC�𵴴����о�
					OPUA.Algorithm.ATC.ATCShakeAutoQuit / OpBool / true / {true, false} / ע�ͣ�ATC���Զ��˳��Ŀ���
					OPUA.Algorithm.ATC.MIPSolverMode / OpChar / 'G' / {'G', 'C', 'S', 'M', 'T'} /  ע�ͣ�MIP�����ѡ��G-GRB / C-CPX / S-SCIP / M-MSK / T-COPT
		*/
		class OpAlgoATC
		{
		protected:
			OpHierarchicalModelI* mdl_; // �ֲ�ģ��

			struct OpATCIterInfo; // ATC������Ϣ
		public:
			void extract(OpHierarchicalModel model); // ��ȡOPUA�ֲ�ģ��	
			OpBool solve(const Solver::OpConfig& config); // ���ģ��
		public:
			OpAlgoATC(); // Ĭ�Ϲ��캯��
			OpAlgoATC(OpHierarchicalModel model); // ���첢��ȡOPUAģ��
		public:
			~OpAlgoATC();
		};

		// ΪATC�㷨����Ĭ������
		void DefaultCfg4ATC(Solver::OpConfig& config);
		// ΪATC�㷨����Ĭ������
		Solver::OpConfig DefaultCfg4ATC();

		/*
			�㷨���Bilinear(����*����)���Ի�
			�ɳڷ�ʽ��BigM
			��ѧ��ʽ��
				ԭԼ����ʽ��w = x * y			
			����˵����
				env / OpEnv / - / - / ��������
				result / OpLinearization& / - / - / ���Ի������еĸ��ӱ�����Լ��
				w / Variable::OpVar / - / - / ����˫�����Ҫ��Ϊ������������(OpVarType::Con)
				x / Variable::OpVar / - / - / ˫���������1��Ҫ��Ϊ0-1����(OpVarType::Bool)
				y / Variable::OpVar / - / - / ˫���������2��Ҫ��Ϊ������������(OpVarType::Con)
				xmin / OpFloat / - / [0, 1] / Ԥ����x������
				xmax / OpFloat / - / [0, 1] / Ԥ����x������
				ymin / OpFloat / - / [-inf, inf] / Ԥ����y������(ע�⣺���ֵ����ΪBigM����)
				ymax / OpFloat / - / [-inf, inf] / Ԥ����y������(ע�⣺���ֵ����ΪBigM����)
			�ر�˵����
				1. ����x��y������Ӷ����BoundԼ������Ĭ��xmin <= x <= xmax, ymin <= y <= ymax
				2. ����Լ��ȫ��Ϊ��׼��ʽ��expr >= lb / expr == lb��������÷Ǳ�׼Լ����������ִ��standardize
		*/
		void OpAlgoBigMBilinear1(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax);
		OpLinearization OpAlgoBigMBilinear1(OpEnv env, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax);

		/*
			�㷨���Bilinear(����*����)���Ի�
			�ɳڷ�ʽ��BigM
			��ѧ��ʽ��
				ԭԼ����ʽ��w = x * y
			����˵����
				env / OpEnv / - / - / ��������
				result / OpLinearization& / - / - / ���Ի������еĸ��ӱ�����Լ��
				w / Variable::OpVar / - / - / ����˫�����Ҫ��Ϊ������������(OpVarType::Con)
				x / Variable::OpVar / - / - / ˫���������1��Ҫ��Ϊ������������(OpVarType::Con)
				y / Variable::OpVar / - / - / ˫���������2��Ҫ��Ϊ������������(OpVarType::Con)
				xmin / OpFloat / - / [-inf, inf]] / Ԥ����x������(ע�⣺���ֵ����ΪBigM����)
				xmax / OpFloat / - / [-inf, inf] / Ԥ����x������(ע�⣺���ֵ����ΪBigM����)
				ymin / OpFloat / - / [-inf, inf] / Ԥ����y������(ע�⣺���ֵ����ΪBigM����)
				ymax / OpFloat / - / [-inf, inf] / Ԥ����y������(ע�⣺���ֵ����ΪBigM����)
				segment / OpULInt / - / [2, inf] / ��x���ж����Ʋ�����λ��
			�ر�˵����
				1. Ĭ�϶�x���ж����Ʋ�����segment��������������\delta x
				2. ����x��y������Ӷ����BoundԼ������Ĭ��xmin <= x <= xmax, ymin <= y <= ymax
				3. ����Լ��ȫ��Ϊ��׼��ʽ��expr >= lb / expr == lb��������÷Ǳ�׼Լ����������ִ��standardize
		*/
		void OpAlgoBigMBilinear2(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax, OpULInt segment);
		OpLinearization OpAlgoBigMBilinear2(OpEnv env, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax, OpULInt segment);

		/*
			�㷨���Bilinear���Ի�
			�ɳڷ�ʽ��McCormick
			��ѧ��ʽ��
				ԭԼ����ʽ��w = x * y
		*/
		//void OpAlgoMCBilinear(OpEnv env, OpLinearization& result);
		//OpLinearization OpAlgoMCBilinear(OpEnv env);

		/*
			�㷨���Abs�������Ի�
			�ɳڷ�ʽ��BigM
			��ѧ��ʽ��
				ԭԼ����ʽ��w = abs(x)
			����˵����
				env / OpEnv / - / - / ��������
				result / OpLinearization& / - / - / ���Ի������еĸ��ӱ�����Լ��
				w / Variable::OpVar / - / - / �������ֵ��
				x / Variable::OpVar / - / - / �������ֵ��������
				xamax / OpFloat / - / [0, inf] / Ԥ����abs(x)������(ע�⣺���ֵ����ΪBigM����)

			�ر�˵����
				1. ����x������Ӷ����BoundԼ������Ĭ��abs(x) <= xamax
				2. ����Լ��ȫ��Ϊ��׼��ʽ��expr >= lb / expr == lb��������÷Ǳ�׼Լ����������ִ��standardize
		*/
		void OpAlgoBigMAbs(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, OpFloat xamax);
		OpLinearization OpAlgoBigMAbs(OpEnv env, Variable::OpVar w, Variable::OpVar x, OpFloat xamax);

		/*
			�㷨���Max�������Ի�
			�ɳڷ�ʽ��BigM
			��ѧ��ʽ��
				ԭԼ����ʽ��w = max(x, y)
			����˵����
				env / OpEnv / - / - / ��������
				result / OpLinearization& / - / - / ���Ի������еĸ��ӱ�����Լ��
				w / Variable::OpVar / - / - / ����Max������
				x / Variable::OpVar / - / - / ����Max��������1
				y / Variable::OpVar / - / - / ����Max��������2
				xmin / OpFloat / - / [-inf, inf]] / Ԥ����x������(ע�⣺���ֵ����ΪBigM����)
				xmax / OpFloat / - / [-inf, inf] / Ԥ����x������(ע�⣺���ֵ����ΪBigM����)
				ymin / OpFloat / - / [-inf, inf] / Ԥ����y������(ע�⣺���ֵ����ΪBigM����)
				ymax / OpFloat / - / [-inf, inf] / Ԥ����y������(ע�⣺���ֵ����ΪBigM����)

			�ر�˵����
				1. ����x������Ӷ����BoundԼ������Ĭ��abs(x) <= xamax
				2. ����Լ��ȫ��Ϊ��׼��ʽ��expr >= lb / expr == lb��������÷Ǳ�׼Լ����������ִ��standardize
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
			�㷨���Min�������Ի�
			�ɳڷ�ʽ��BigM
			��ѧ��ʽ��
				ԭԼ����ʽ��w = min(x, y)
			����˵����
				env / OpEnv / - / - / ��������
				result / OpLinearization& / - / - / ���Ի������еĸ��ӱ�����Լ��
				w / Variable::OpVar / - / - / ����Min������
				x / Variable::OpVar / - / - / ����Min��������1
				y / Variable::OpVar / - / - / ����Min��������2
				xmin / OpFloat / - / [-inf, inf]] / Ԥ����x������(ע�⣺���ֵ����ΪBigM����)
				xmax / OpFloat / - / [-inf, inf] / Ԥ����x������(ע�⣺���ֵ����ΪBigM����)
				ymin / OpFloat / - / [-inf, inf] / Ԥ����y������(ע�⣺���ֵ����ΪBigM����)
				ymax / OpFloat / - / [-inf, inf] / Ԥ����y������(ע�⣺���ֵ����ΪBigM����)

			�ر�˵����
				1. ����x������Ӷ����BoundԼ������Ĭ��abs(x) <= xamax
				2. ����Լ��ȫ��Ϊ��׼��ʽ��expr >= lb / expr == lb��������÷Ǳ�׼Լ����������ִ��standardize
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
			Variable::OpVarArr primalVar; /*ԭʼ����*/
			Variable::OpVarArr dualVar; /*��ż����*/
			Variable::OpVarArr slackVar; /*�ɳڱ���*/
			Constraint::OpLCArr gradCon; /*�ݶ�Լ��*/
			Constraint::OpLCArr primalCon; /*ԭʼ������Լ��*/
			Constraint::OpLCArr dualCon; /*��ż������Լ��*/
			Constraint::OpLCArr csCon1; /*�����ɳ�Լ��(����)*/
			Constraint::OpQCArr csCon2; /*�����ɳ�Լ��(����)*/
		public:
			void init(OpEnv env); //��ʼ��
			void clear(OpBool deep); // ���(deepΪtrueʱ��ִ����������ͷ������е�����Ԫ��)
		public:
			OpAutoKKTSet();
			OpAutoKKTSet(OpEnv env);
		};

		/*
			�㷨����Զ�KKT�����Ƶ�
			�ɳڷ�ʽ��Bilinear(���ɳ�) / BigM
			��ѧ��ʽ��
				ԭ������ʽ��
					\min_{x,y} c^{T}*x+d^{T}*y
					s.t.   A*x+B*y>=b: \lambda
					       G*x+D*y=g: \mu
						   x>=0
				KKT������ʽ��
					A^{T}*\lambda-G^{T}*\mu<=c
					B^{T}*\lambda-D^{T}*\mu=d
					A*x+B*y>=b
					G*x+D*y=g
					\lambda>=0
					\lambda^{T}*(A*x+B*y-b)=0
			����˵����
				env / OpEnv / - / - / ��������
				result / OpAutoKKTSet& / - / - / KKT����(����������Լ��)
				X0 / Variable::OpVarArr / - / - / ԭ���������(�Ǹ�����)
				Y0 / Variable::OpVarArr / - / - / ԭ���������(�����Ʊ���)
				NEC0 / Constraint::OpLCArr / - / - / ԭ���ⲻ��ʽԼ��������
				EC0 / Constraint::OpLCArr / - / - / ԭ�����ʽԼ��������
				O0 / const Expression::OpLinExpr& / - / - / ԭ����Ŀ�꺯��
				M / Container::OpFloatArr / - / - / �����ɳ�Լ����BigM����
				relax / OpBool / - / {true, false} / �Ƿ�Ի����ɳ�Լ�������ɳ�
			�ر�˵����
				1. result��primalCon����[����ʽԼ������ʽԼ��]��˳�����У���ż����dualVar�Զ����ɲ���primalCon��˳���Ӧ��Ϊ[�Ǹ���ż�����������ƶ�ż����]��gradCon����[����ʽԼ������ʽԼ��]��˳������
				2. ԭ����Լ��NEC0��EC0����Ҫд�ɱ�׼��С�ڵ���Լ��A*x+B*y>=b���׼�ĵ�ʽԼ��G*x+D*y����Ӧ�Ķ�ż�����ֱ�Ϊlambda(�Ǹ�)��mu(������)
				3. ʹ�á�<=������>=���������ɵ�����Լ�����뽫�����ͺ������ı��ʽ���з��룬�ҳ���Ӧ��Ϊ�������������lb<=expr��ub>=exprд��expr>=lbʵ���лᱻ����Ϊ-expr<=-lb���ᵼ��AutoKKTʧ�ܣ���������÷Ǳ�׼Լ����������ִ��standardize
				4. relax��Ϊtrueʱ���Ի����ɳ�Լ������BigM�ɳ�(��Ҫ��������M����֤M������NEC0���)������result�е�csCon1���ɳڱ�������result������Ϊfalseʱ�������ɳ�Լ��Ϊ����Լ��������result�е�csCon2(M����Ϊ��)
		*/
		void OpAlgoAutoKKT(OpEnv env, OpAutoKKTSet& result, Variable::OpVarArr X0, Variable::OpVarArr Y0, Constraint::OpLCArr NEC0, Constraint::OpLCArr EC0, const Expression::OpLinExpr& O0,
			Container::OpFloatArr M, OpBool relax);
		OpAutoKKTSet OpAlgoAutoKKT(OpEnv env, Variable::OpVarArr X0, Variable::OpVarArr Y0, Constraint::OpLCArr NEC0, Constraint::OpLCArr EC0, const Expression::OpLinExpr& O0,
			Container::OpFloatArr M, OpBool relax);

		class OpAutoDualSet
		{
		public:
			Variable::OpVarArr dualVar; /*��ż�������*/
			Constraint::OpLCArr dualCon; /*��ż����Լ��*/
			Expression::OpLinExpr dualObj; /*��ż����Ŀ�꺯��*/
		public:
			void init(OpEnv env); //��ʼ��
			void clear(OpBool deep); // ���(deepΪtrueʱ��ִ����������ͷ������е�����Ԫ��)
		public:
			OpAutoDualSet();
			OpAutoDualSet(OpEnv env);
		};

		/*
			�㷨����Զ���ż�����Ƶ�
			��ѧ��ʽ��
				ԭ������ʽ��
					\min c^{T}*x+d^{T}*y
					s.t.   A*x+B*y>=b: \lambda
						   G*x+D*y=g: \mu
						   x>=0
				��ż������ʽ��
					\max_{\lambda,\mu} b^T*\lambda+g^T*\mu
					s.t.   A^{T}*\lambda+G^{T}*\mu<=c: x
						   B^{T}*\lambda+D^{T}*\mu=d: y
						   \lambda>=0
			����˵����
				env / OpEnv / - / - / ��������
				result / OpAutoDualSet& / - / - / ��ż����
				X0 / Variable::OpVarArr / - / - / ԭ���������(�Ǹ�����)
				Y0 / Variable::OpVarArr / - / - / ԭ���������(�����Ʊ���)
				NEC0 / Constraint::OpLCArr / - / - / ԭ���ⲻ��ʽԼ��������
				EC0 / Constraint::OpLCArr / - / - / ԭ�����ʽԼ��������
				O0 / const Expression::OpLinExpr& / - / - / ԭ����Ŀ�꺯��
			�ر�˵����
				1. result�ж�ż����dualVar�Զ����ɣ�����[�Ǹ���ż�����������ƶ�ż����]��˳�����У���[����ʽԼ������ʽԼ��]��˳���Ӧ
				2. result�ж�żԼ��dualCon�Զ����ɣ�����[����ʽԼ������ʽԼ��]��˳�����У�����[�Ǹ�ԭ���������������ԭ�������]��˳���Ӧ
				3. ԭ����Լ��NEC0��EC0����Ҫд�ɱ�׼��С�ڵ���Լ��A*x+B*y>=b���׼�ĵ�ʽԼ��G*x+D*y����Ӧ�Ķ�ż�����ֱ�Ϊlambda(�Ǹ�)��mu(������)
				4. ʹ�á�<=������>=���������ɵ�����Լ�����뽫�����ͺ������ı��ʽ���з��룬�ҳ���Ӧ��Ϊ�������������lb<=expr��ub>=exprд��expr>=lbʵ���лᱻ����Ϊ-expr<=-lb���ᵼ��AutoDualʧ�ܣ���������÷Ǳ�׼Լ����������ִ��standardize
		*/
		void OpAlgoAutoDual(OpEnv env, OpAutoDualSet& result, Variable::OpVarArr X0, Variable::OpVarArr Y0, Constraint::OpLCArr NEC0, Constraint::OpLCArr EC0, const Expression::OpLinExpr& O0);
		OpAutoDualSet OpAlgoAutoDual(OpEnv env, Variable::OpVarArr X0, Variable::OpVarArr Y0, Constraint::OpLCArr NEC0, Constraint::OpLCArr EC0, const Expression::OpLinExpr& O0);
	}
}