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
				[2] OpPSDVar / PSD����
				[3] OpLinCon / ����Լ��
				[4] OpQuadCon / ����Լ��
				[5] OpConicCon / ׶Լ��
				[6] OpPSDCon / �붨Լ��
				[7] OpSOSCon / SOSԼ��
				[8] OpNLCon / ������Լ��
				[9] OpCdtCon / ����Լ��
				[10] OpObj / ��Ŀ���Ż�Ŀ�꺯��

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
			void add(Constraint::OpConicCon con); // ���׶Լ��
			void add(Constraint::OpConicArr cons); // ���׶Լ����
			void add(Constraint::OpPSDCon con); // ��Ӱ붨Լ��
			void add(Constraint::OpPSDCArr cons); // ��Ӱ붨Լ����
			void add(Constraint::OpSOSCon con); // ���SOSԼ��
			void add(Constraint::OpSOSArr cons); // ���SOSԼ����
			void add(Constraint::OpNLCon con); // ��ӷ�����Լ��
			void add(Constraint::OpNLCArr cons); // ��ӷ�����Լ����
			void add(Constraint::OpCdtCon con); // �������Լ��
			void add(Constraint::OpCCArr cons);  // �������Լ����
			void setObj(Objective::OpObj obj); // ���õ�Ŀ���Ż�Ŀ�꺯��
			Objective::OpObj getObj() const; // ��ȡ��Ŀ���Ż�Ŀ�꺯��
			void remove(Constraint::OpLinCon con); // �Ƴ�����Լ��
			void remove(Constraint::OpLCArr cons); // �Ƴ�����Լ����
			void remove(Constraint::OpQuadCon con); // �Ƴ�����Լ��
			void remove(Constraint::OpQCArr cons); // �Ƴ�����Լ����
			void remove(Constraint::OpConicCon con); // �Ƴ�׶Լ��
			void remove(Constraint::OpConicArr cons); // �Ƴ�׶Լ����
			void remove(Constraint::OpPSDCon con); // �Ƴ��붨Լ��
			void remove(Constraint::OpPSDCArr cons); // �Ƴ��붨Լ����
			void remove(Constraint::OpSOSCon con); // �Ƴ�SOSԼ��
			void remove(Constraint::OpSOSArr cons); // �Ƴ�SOSԼ����
			void remove(Constraint::OpNLCon con); // �Ƴ�������Լ��
			void remove(Constraint::OpNLCArr cons); // �Ƴ�������Լ����
			void remove(Constraint::OpCdtCon con); // �������Լ��
			void remove(Constraint::OpCCArr cons);  // �������Լ����
			void write(OpStr path) const; // ��ģ��д���ļ�
			OpULInt getSize(Variable::OpVar flag) const; // ��ȡ������Ŀ
			OpULInt getSize(Variable::OpPSDVar flag) const; // ��ȡPSD������Ŀ
			OpULInt getSize(Constraint::OpLinCon flag) const; // ��ȡ����Լ����Ŀ
			OpULInt getSize(Constraint::OpQuadCon flag) const; // ��ȡ����Լ����Ŀ
			OpULInt getSize(Constraint::OpConicCon flag) const; // ��ȡ׶Լ����Ŀ
			OpULInt getSize(Constraint::OpPSDCon flag) const; // ��ȡ�붨Լ����Ŀ
			OpULInt getSize(Constraint::OpSOSCon flag) const; // ��ȡSOSԼ����Ŀ
			OpULInt getSize(Constraint::OpNLCon flag) const; // ��ȡ������Լ����Ŀ
			OpULInt getSize(Constraint::OpCdtCon flag) const; // ��ȡ����Լ����Ŀ
			OpULInt getSize(Objective::OpObj flag) const; // ��ȡĿ�꺯����Ŀ
			template<typename T> inline auto  getSize() const { return getSize(T()); }
			void setName(OpStr name); // ��������
			OpStr getName() const; // ��ȡģ������
			OpModelI* getImpl() const; // ��ȡimpl
			void release(); // �ͷ��ڴ�
		public:
			Variable::OpVarIdxDict::OpDictCIter getCBegin(Variable::OpVar flag) const; // ��ȡ�����ֵ���׶˳���������
			Variable::OpPSDVIdxDict::OpDictCIter getCBegin(Variable::OpPSDVar flag) const; // ��ȡPSD�����ֵ���׶˳���������
			Constraint::OpLCIdxDict::OpDictCIter getCBegin(Constraint::OpLinCon flag) const; // ��ȡ����Լ���ֵ���׶˳���������
			Constraint::OpQCIdxDict::OpDictCIter getCBegin(Constraint::OpQuadCon flag) const; // ��ȡ����Լ���ֵ���׶˳���������
			Constraint::OpConicIdxDict::OpDictCIter getCBegin(Constraint::OpConicCon flag) const; // ��ȡ׶Լ���ֵ���׶˳���������
			Constraint::OpPSDCIdxDict::OpDictCIter getCBegin(Constraint::OpPSDCon flag) const; // ��ȡ�붨Լ���ֵ���׶˳���������
			Constraint::OpSOSIdxDict::OpDictCIter getCBegin(Constraint::OpSOSCon flag) const; // ��ȡSOSԼ���ֵ���׶˳���������
			Constraint::OpNLCIdxDict::OpDictCIter getCBegin(Constraint::OpNLCon flag) const; // ��ȡ������Լ���ֵ���׶˳���������
			Constraint::OpCCIdxDict::OpDictCIter getCBegin(Constraint::OpCdtCon flag) const; // ��ȡ����Լ���ֵ���׶˳���������
			Objective::OpObjIdxDict::OpDictCIter getCBegin(Objective::OpObj flag) const; // ��ȡ��Ŀ�꺯���ֵ���׶˳���������	
			template<typename T> inline auto  getCBegin() const { return getCBegin(T()); }

			Variable::OpVarIdxDict::OpDictCIter getCEnd(Variable::OpVar flag) const; // ��ȡ�����ֵ��ĩ�˳���������
			Variable::OpPSDVIdxDict::OpDictCIter getCEnd(Variable::OpPSDVar flag) const; // ��ȡPSD�����ֵ��ĩ�˳���������
			Constraint::OpLCIdxDict::OpDictCIter getCEnd(Constraint::OpLinCon flag) const; // ��ȡ����Լ���ֵ��ĩ�˳���������		
			Constraint::OpQCIdxDict::OpDictCIter getCEnd(Constraint::OpQuadCon flag) const; // ��ȡ����Լ���ֵ��ĩ�˳���������		
			Constraint::OpConicIdxDict::OpDictCIter getCEnd(Constraint::OpConicCon flag) const; // ��ȡ׶Լ���ֵ��ĩ�˳���������
			Constraint::OpPSDCIdxDict::OpDictCIter getCEnd(Constraint::OpPSDCon flag) const; // ��ȡ�붨Լ���ֵ��ĩ�˳���������
			Constraint::OpSOSIdxDict::OpDictCIter getCEnd(Constraint::OpSOSCon flag) const; // ��ȡSOSԼ���ֵ��ĩ�˳���������
			Constraint::OpNLCIdxDict::OpDictCIter getCEnd(Constraint::OpNLCon flag) const; // ��ȡ������Լ���ֵ��ĩ�˳���������
			Constraint::OpCCIdxDict::OpDictCIter getCEnd(Constraint::OpCdtCon flag) const; // ��ȡ����Լ���ֵ��ĩ�˳���������
			Objective::OpObjIdxDict::OpDictCIter getCEnd(Objective::OpObj flag) const; // ��ȡ��Ŀ�꺯���ֵ��ĩ�˳���������
			template<typename T> inline auto getCEnd() const { return getCEnd(T()); }
		public:
			OpBool operator==(const OpModel& model) const;
			OpBool operator!=(const OpModel& model) const;
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
