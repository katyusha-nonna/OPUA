#pragma once

#include "OPUA.Variable.h"
#include "OPUA.Constraint.h"
#include "OPUA.Objective.h"

namespace OPUA
{
	namespace Model
	{
		class OpModelI;
		class OpModel;

		/*
			OpModel��OPUA�Ż�ģ����
			�Ż�ģ�Ͷ��������ʹ���ֵ���зֱ�洢:
				[1] OpVar / ���� 
				[2] OpLinCon / ����Լ��
				[3] OpQuadCon / ����Լ��
				[4] OpSOSCon / SOSԼ��
				[5] OpNLCon / ������Լ��
				[6] OpObj / ��Ŀ���Ż�Ŀ�꺯��

				���������Ա�������������ֵ䣬��ֵΪ����
		*/
		class OpModel
			: public OpBase
		{
		public:
			void add(Constraint::OpLinCon con); // �������Լ��
			void add(Constraint::OpLCArr cons); // �������Լ����
			void add(Constraint::OpQuadCon con); // ��Ӷ���Լ��
			void add(Constraint::OpQCArr cons); // ��Ӷ���Լ����
			void add(Constraint::OpSOSCon con); // ���SOSԼ��
			void add(Constraint::OpSOSArr cons); // ���SOSԼ����
			void add(Constraint::OpNLCon con); // ��ӷ�����Լ��
			void add(Constraint::OpNLCArr cons); // ��ӷ�����Լ����
			void setObj(Objective::OpObj obj); // ���õ�Ŀ���Ż�Ŀ�꺯��
			Objective::OpObj getObj(); // ��ȡ��Ŀ���Ż�Ŀ�꺯��
			void remove(Constraint::OpLinCon con); // �Ƴ�����Լ��
			void remove(Constraint::OpLCArr cons); // �Ƴ�����Լ����
			void remove(Constraint::OpQuadCon con); // �Ƴ�����Լ��
			void remove(Constraint::OpQCArr cons); // �Ƴ�����Լ����
			void remove(Constraint::OpSOSCon con); // �Ƴ�SOSԼ��
			void remove(Constraint::OpSOSArr cons); // �Ƴ�SOSԼ����
			void remove(Constraint::OpNLCon con); // �Ƴ�������Լ��
			void remove(Constraint::OpNLCArr cons); // �Ƴ�������Լ����
			void write(OpStr path) const; // ��ģ��д���ļ�
			void setName(OpStr name); // ��������
			OpStr getName() const; // ��ȡģ������
			OpModelI* getImpl() const; // ��ȡimpl
		public:
			Variable::OpVarIdxDict::OpDictCIter getVBegin(); // ��ȡ�����ֵ���׶˳���������
			Variable::OpVarIdxDict::OpDictCIter getVEnd(); // ��ȡ�����ֵ��ĩ�˳���������
			Constraint::OpLCIdxDict::OpDictCIter getLCBegin(); // ��ȡ����Լ���ֵ���׶˳���������
			Constraint::OpLCIdxDict::OpDictCIter getLCEnd(); // ��ȡ����Լ���ֵ��ĩ�˳���������
			Constraint::OpQCIdxDict::OpDictCIter getQCBegin(); // ��ȡ����Լ���ֵ���׶˳���������
			Constraint::OpQCIdxDict::OpDictCIter getQCEnd(); // ��ȡ����Լ���ֵ��ĩ�˳���������
			Constraint::OpSOSIdxDict::OpDictCIter getSOSBegin(); // ��ȡSOSԼ���ֵ���׶˳���������
			Constraint::OpSOSIdxDict::OpDictCIter getSOSEnd(); // ��ȡSOSԼ���ֵ��ĩ�˳���������
			Constraint::OpNLCIdxDict::OpDictCIter getNLCBegin(); // ��ȡ������Լ���ֵ���׶˳���������
			Constraint::OpNLCIdxDict::OpDictCIter getNLCEnd(); // ��ȡ������Լ���ֵ��ĩ�˳���������
			Objective::OpObjIdxDict::OpDictCIter getOBegin(); // ��ȡ��Ŀ�꺯���ֵ���׶˳���������
			Objective::OpObjIdxDict::OpDictCIter getOEnd(); // ��ȡ��Ŀ�꺯���ֵ��ĩ�˳���������
		public:
			OpBool operator==(const OpModel& model);
			OpBool operator!=(const OpModel& model);
		public:
			OpModel(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpModel(OpModelI* impl); // ��impl����
			OpModel(OpEnv env); // ��env����
			OpModel(OpEnv env, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpModel();
		};
	}
}
