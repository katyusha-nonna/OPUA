#pragma once

#include "OPUA.Expression.h"

namespace OPUA
{
	namespace Objective
	{
		class OpObjI;
		class OpObj;

		typedef Container::OpArray<OpObj> OpObjArr; // Ŀ�꺯������
		typedef Container::OpDict<OpLInt, OpObj> OpObjIdxDict; // Ŀ�꺯�������ֵ�

		// OpObjSense��OPUAĿ�꺯������
		enum class OpObjSense
		{
			Min,
			Max
		};

		OpStr ObjSense2Str(OpObjSense sense); // ��Ŀ�꺯������ת��Ϊ�ַ�

		std::ostream& operator<<(std::ostream& stream, OpObj obj);
		OpObj OpMinimize(OpEnv env, const Expression::OpLinExpr& lexpr); // ��һ�����Ա��ʽ����Ŀ�꺯��
		OpObj OpMinimize(OpEnv env, const Expression::OpQuadExpr& qexpr); // ��һ�����α��ʽ����Ŀ�꺯��(����������)
		OpObj OpMaximize(OpEnv env, const Expression::OpLinExpr& lexpr); // ��һ�����Ա��ʽ����Ŀ�꺯��
		OpObj OpMaximize(OpEnv env, const Expression::OpQuadExpr& qexpr); // ��һ�����α��ʽ����Ŀ�꺯��(����������)

		// OpObj��OPUAĿ�꺯����
		class OpObj
			: public OpBase
		{
		public:
			void setLinExpr(const Expression::OpLinExpr& expr); // ������������ʽ
			void setQuadExpr(const Expression::OpQuadExpr& expr); // ���ö�������ʽ(����������)
			void setSense(OpObjSense sense); // ����Ŀ�꺯������
			void setName(OpStr name); // ����Ŀ�꺯������

			const Expression::OpLinExpr& getLinExpr() const; // ��ȡ��������ʽ
			const Expression::OpQuadExpr& getQuadExpr() const; // ��ȡ��������ʽ
			OpObjSense getSense() const; // ��ȡĿ�꺯������
			OpStr getName() const; // ��ȡĿ�꺯������
			OpBool isConstant() const; // �Ƿ�Ϊ����Ŀ�꺯��
			OpBool isQuad() const; // �Ƿ�Ϊ����Ŀ�꺯��
			OpObjI* getImpl() const; // ��ȡimpl
		public:
			OpBool operator==(const OpObj& obj) const;
			OpBool operator!=(const OpObj& obj) const;
		public:
			OpObj(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpObj(OpObjI* impl); // ��impl����
			OpObj(OpEnv env); // ��env����
			OpObj(OpEnv env, OpObjSense sense); // ��env���첢ָ�����ֲ���
			OpObj(OpEnv env, OpObjSense sense, const Expression::OpLinExpr& lexpr); // ��env���첢ָ�����ֲ���
			OpObj(OpEnv env, OpObjSense sense, const Expression::OpLinExpr& lexpr, const Expression::OpQuadExpr& qexpr); // ��env���첢ָ�����ֲ���
			OpObj(OpEnv env, OpObjSense sense, const Expression::OpLinExpr& lexpr, const Expression::OpQuadExpr& qexpr, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpObj();
		};
	}
}

