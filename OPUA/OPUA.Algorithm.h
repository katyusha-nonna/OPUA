#pragma once

#include "OPUA.Model.h"

namespace OPUA
{
	namespace Algorithm
	{

		class OpRobustModelI;

		// OPUA���׶�³��ģ�ͽ׶�����
		enum class RobustStageType
		{
			FirstStage,
			FirstStageSpecial,
			SecondStagePrimal,
			SecondStageDual,
			Uncertainty,
			Unknown
		};

		/*
			OPUA���׶�³��ģ����
			�ڶ��׶�ԭ����/��ż�������д�ɱ�׼��ʽ	
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
	}
}