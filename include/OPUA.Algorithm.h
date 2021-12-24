#pragma once

#include "OPUA.Solver.h"

namespace OPUA
{
	namespace Algorithm
	{
		class OpRobustModelI;
		class OpRobustModel;
		class OpAlgoCCG;
		typedef std::pair<Variable::OpVarArr, Constraint::OpLCArr> OpLinearization;

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
						д��expr>=lbʵ���лᱻ����Ϊ-expr<=-lb���ᵼ��autoDualʧ�ܣ�
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
			void autoDual(); // �Զ��Ƶ���ż
			void autoBound(); // �Զ��Ƶ����������½�
			void update(); // ����ģ��(��ʼ���⼯)
			void add(Variable::OpVarArr vars, RobustStageType type, OpBool flag); // ��ӱ���
			void add(Variable::OpVar var, RobustStageType type, OpBool flag); // ��ӱ���
			void add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag); // ���Լ��
			void add(Constraint::OpLinCon con, RobustStageType type, OpBool flag); // ���Լ��
			void add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag); // ���Լ��
			void add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag); // ���Լ��	
			void setObj(Expression::OpLinExpr expr, RobustStageType type); // ����Ŀ�꺯��
			void setObj(Expression::OpQuadExpr expr, RobustStageType type); // ����Ŀ�꺯��
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
				OPUA.Algorithm.CCG.SubProbSolveMode/ OpLInt / 0 / {0, 1, 2} / ע�ͣ�CCG���������ģʽ����ʱ��������
				OPUA.Algorithm.CCG.NoImprovementIterLimit / OpLInt / 3 / [2, inf] / ע�ͣ�CCG���½�����ۼ��޸��ƴ���(�������ƻ���Ϊ�Ѿ��޷������Ż����˳�)
				OPUA.Algorithm.CCG.FirstStageInitMode / OpLInt / 0 / {0, 1, 2} /  ע�ͣ�CCGһ�׶α�����ʼ��ģʽ��0-0ֵ��ʼ�� / 1-������ֵ / 2-ȷ���Խ�
				OPUA.Algorithm.CCG.GetDeterministicSolution / OpBool / false / {true, false} / ע�ͣ��Ƿ���Ϊȷ����ģ�����(��Ҫ������ȷ������ֵ)
				OPUA.Algorithm.CCG.MIPTimeLimit / OpFloat / 3600 / [0, inf] / ע�ͣ�MIP���ʱ������
				OPUA.Algorithm.CCG.MIPGap / OpFloat / 1e-5 / [0, 1] /  ע�ͣ�MIP��������о�
				OPUA.Algorithm.CCG.MIPSolverMode / OpChar / 'G' / {'G', 'C', 'S', 'M'} /  ע�ͣ�MIP�����ѡ��G-GRB / C-CPX / S-SCIP / M-MSK
		*/
		class OpAlgoCCG
		{
		protected:
			OpRobustModelI* mdl_; // ���׶�³��ģ��

			struct OpCCGIterInfo; // CCG������Ϣ
			struct OpOAIterInfo; // OA������Ϣ
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
				2. ����Լ��ȫ��Ϊ��׼��ʽ��expr >= lb / expr == lb
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
				3. ����Լ��ȫ��Ϊ��׼��ʽ��expr >= lb / expr == lb
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
				2. ����Լ��ȫ��Ϊ��׼��ʽ��expr >= lb / expr == lb
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
				2. ����Լ��ȫ��Ϊ��׼��ʽ��expr >= lb / expr == lb
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
				2. ����Լ��ȫ��Ϊ��׼��ʽ��expr >= lb / expr == lb
		*/
		void OpAlgoBigMMin(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax);
		OpLinearization OpAlgoBigMMin(OpEnv env, Variable::OpVar w, Variable::OpVar x, Variable::OpVar y,
			OpFloat xmin, OpFloat xmax, OpFloat ymin, OpFloat ymax);
		void OpAlgoBigMMin(OpEnv env, OpLinearization& result, Variable::OpVar w, Variable::OpVarArr X,
			Container::OpFloatArr Xmin, Container::OpFloatArr Xmax);
		OpLinearization OpAlgoBigMMin(OpEnv env, Variable::OpVar w, Variable::OpVarArr X,
			Container::OpFloatArr Xmin, Container::OpFloatArr Xmax);
	}
}