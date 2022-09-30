#pragma once

#include "OPUA.Constant.h"
#include "OPUA.Environment.h"
#include "OPUA.Container.h"

namespace OPUA
{
	namespace Variable
	{
		class OpVarI;
		class OpVar;
		class OpPSDVarI;
		class OpPSDVar;

		typedef Container::OpArray<OpVar> OpVarArr; // ���߱�������
		typedef Container::OpArray<OpVarArr> OpVarMat; // ���߱�������
		typedef Container::OpDict<OpLInt, OpVar> OpVarIdxDict; // ���߱��������ֵ�
		typedef Container::OpArray<OpPSDVar> OpPSDVArr; // PSD���߱�������
		typedef Container::OpArray<OpPSDVArr> OpPSDVMat; // PSD���߱�������
		typedef Container::OpDict<OpLInt, OpPSDVar> OpPSDVIdxDict; // PSD���߱��������ֵ�

		// OpVarType����������
		enum class OpVarType
		{
			Bool, // 0-1����
			Int, // ��������
			Con, // ��������
			Sem, // ����������
		};

		OpStr VarType2Str(OpVarType type);
		std::ostream& operator<<(std::ostream& stream, OpVar var);
		std::ostream& operator<<(std::ostream& stream, OpPSDVar var);

		// OpVar��OPUA������
		class OpVar
			: public OpBase
		{
		public:
			void setLb(OpFloat lb); // ���ñ����½�
			void setUb(OpFloat ub); // ���ñ����Ͻ�
			void setBound(OpFloat lb, OpFloat ub); // ���ñ������½�
			void setType(OpVarType type); // ����(�ı�)����������
			void setFixedValue(OpFloat val); // ���ñ����̶�ֵ
			void removeFixedValue(); // ����̶�ֵ
			void setName(OpStr name); // ���ñ�������

			OpFloat getLb() const; // ��ȡ�����Ͻ�
			OpFloat getUb() const; // ��ȡ�����½�
			OpVarType getType() const; // ��ȡ��������
			OpBool getFixed() const; // ��ȡ�����Ƿ񱻹̶�
			OpFloat getFixedValue() const; // ��ȡ�����̶�ֵ
			OpStr getName() const; // ��ȡ��������
			OpVarI* getImpl() const; // ��ȡimpl
		public:
			OpBool operator==(const OpVar& var) const;
			OpBool operator!=(const OpVar& var) const;
		public:
			OpVar(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpVar(OpVarI* impl); // ��impl����
			OpVar(OpEnv env); // ��env����
			OpVar(OpEnv env, OpVarType type); // ��env���첢ָ�����ֲ���
			OpVar(OpEnv env, OpVarType type, OpFloat lb, OpFloat ub); // ��env���첢ָ�����ֲ���
			OpVar(OpEnv env, OpVarType type, OpFloat lb, OpFloat ub, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpVar();
		};

		// OpPSDVar��OPUA�������-������������
		class OpPSDVar
			: public OpBase
		{
		public:
			void setDim(OpULInt dim); // ���ñ���ά��
			void setName(OpStr name); // ���ñ�������
			OpULInt getDim() const; // ��ȡ����ά��
			OpStr getName() const; // ��ȡ��������
			OpPSDVarI* getImpl() const; // ��ȡimpl
		public:
			OpBool operator==(const OpPSDVar& var) const;
			OpBool operator!=(const OpPSDVar& var) const;
		public:
			OpPSDVar(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpPSDVar(OpPSDVarI* impl); // ��impl����
			OpPSDVar(OpEnv env); // ��env����
			OpPSDVar(OpEnv env, OpULInt dim); // ��env���첢ָ�����ֲ���
			OpPSDVar(OpEnv env, OpULInt dim, OpStr name); // ��env���첢ָ�����ֲ���
		};
	}
}