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

		// OpModel��OPUA�Ż�ģ����
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
			void setObj(Objective::OpObj obj); // ���õ�Ŀ���Ż�Ŀ�꺯��
			void remove(Constraint::OpLinCon con); // �Ƴ�����Լ��
			void remove(Constraint::OpLCArr cons); // �Ƴ�����Լ����
			void remove(Constraint::OpQuadCon con); // �Ƴ�����Լ��
			void remove(Constraint::OpQCArr cons); // �Ƴ�����Լ����
			void remove(Constraint::OpSOSCon con); // �Ƴ�SOSԼ��
			void remove(Constraint::OpSOSArr cons); // �Ƴ�SOSԼ����
			void write(OpStr path); // ��ģ��д���ļ�
			void setName(OpStr name); // ��������
			OpStr getName() const; // ��ȡģ������
			OpModelI* getImpl() const; // ��ȡimpl
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
