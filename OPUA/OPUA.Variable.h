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

		typedef Container::OpArray<OpVar> OpVarArr; // ���߱�������
		typedef Container::OpDict<OpLInt, OpVar> OpVarIdxDict; // ���߱��������ֵ�

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

		// OpVar��OPUA������
		class OpVar
			: public OpBase
		{
		public:
			void setLb(OpFloat lb); // ���ñ����½�
			void setUb(OpFloat ub); // ���ñ����Ͻ�
			void setBound(OpFloat lb, OpFloat ub); // ���ñ������½�
			void setType(OpVarType type); // ����(�ı�)����������
			void setName(OpStr name); // ���ñ�������

			OpFloat getLb() const; // ��ȡ�����Ͻ�
			OpFloat getUb() const; // ��ȡ�����½�
			OpVarType getType() const; // ��ȡ��������
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
	}
}