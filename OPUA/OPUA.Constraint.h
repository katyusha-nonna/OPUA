#pragma once

#include "OPUA.Expression.h"

namespace OPUA
{
	namespace Constraint
	{
		class OpLinConI;
		class OpLinCon;
		class OpQuadConI;
		class OpQuadCon;
		class OpNLConI;
		class OpNLCon;
		class OpSOSConI;
		class OpSOSCon;

		typedef Container::OpArray<OpLinCon> OpLCArr; // ����Լ������
		typedef Container::OpArray<OpQuadCon> OpQCArr; // ����Լ������
		typedef Container::OpArray<OpSOSCon> OpSOSArr; // SOSԼ������
		typedef Container::OpDict<OpLInt, OpLinCon> OpLCIdxDict; // ����Լ�������ֵ�
		typedef Container::OpDict<OpLInt, OpQuadCon> OpQCIdxDict; // ����Լ�������ֵ�
		typedef Container::OpDict<OpLInt, OpSOSCon> OpSOSIdxDict; // SOSԼ�������ֵ�

		// ���棬���������� <=/>=/== �����������򽫷��ؿ�Լ��

		std::ostream& operator<<(std::ostream& stream, OpLinCon con);
		OpLinCon operator<=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);
		OpLinCon operator>=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);
		OpLinCon operator==(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);

		std::ostream& operator<<(std::ostream& stream, OpQuadCon con);
		OpQuadCon operator<=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);
		OpQuadCon operator>=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);
		OpQuadCon operator==(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);

		std::ostream& operator<<(std::ostream& stream, OpSOSCon con);

		enum class OpConSense
		{
			Equal, // ��ʽԼ��
			LessEqual, // С�ڵ���Լ��
			GreatEqual, // ���ڵ���Լ��
			SOS1, // SOS1Լ��
			SOS2 // SOS2Լ��
		};

		OpStr ConSense2Str(OpConSense sense); // ��Լ������ת��Ϊ�ַ�

		class OpLinCon
		{
		protected:
			OpLinConI* impl_;
		public:
			void setSense(OpConSense sense); // ����(�ı�)Լ���ķ���
			void setName(OpStr name); // ����Լ������
			void setExpr(const Expression::OpLinExpr& expr, OpBool lhs); // ����Լ����/�ұ��ʽ
			void standardize(OpBool simplify = true); // Լ����׼��

			OpConSense getSense() const; // ��ȡԼ���ķ���
			OpStr getName() const; // ��ȡԼ������
			const Expression::OpLinExpr& getExpr(OpBool lhs) const; // ��ȡԼ����/�ұ��ʽ
			OpFloat getRHS() const; // ��ȡԼ���Ҳ�����(����׼��Լ������Ч)
			OpBool isStandard() const; // �Ƿ�Ϊ��ע��Լ��
			OpBool isLocked() const; // �Ƿ�����
			OpLinConI* getImpl() const; // ��ȡimpl
			OpLInt getIndex() const; // ��ȡ����
			OpEnv getEnv() const; // ��ȡ��������
			void release(); // �ͷű���(�ֶ��ͷ�)
			void lock(); // ����Լ��(�������ܸ��ı��ʽ�ͷ��ţ���ִ�б�׼��)
			void unlock(); // ����
		public:
			OpBool operator==(const OpLinCon& con);
			OpBool operator!=(const OpLinCon& con);
		public:
			OpLinCon(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpLinCon(OpLinConI* impl); // ��impl����
			OpLinCon(OpEnv env); // ��env����
			OpLinCon(OpEnv env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs); // ��env���첢ָ�����ֲ���
			OpLinCon(OpEnv env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs, OpStr name); // ��env���첢ָ�����ֲ���
		};

		class OpQuadCon
		{
		protected:
			OpQuadConI* impl_;
		public:
			void setSense(OpConSense sense); // ����(�ı�)Լ���ķ���
			void setName(OpStr name); // ����Լ������
			void setExpr(const Expression::OpQuadExpr& expr, OpBool lhs); // ����Լ����/�ұ��ʽ
			void standardize(OpBool simplify = true); // Լ����׼��

			OpConSense getSense() const; // ��ȡԼ���ķ���
			OpStr getName() const; // ��ȡԼ������
			const Expression::OpQuadExpr& getExpr(OpBool lhs) const; // ��ȡԼ����/�ұ��ʽ
			OpFloat getRHS() const; // ��ȡԼ���Ҳ�����(����׼��Լ������Ч)
			OpBool isStandard() const; // �Ƿ�Ϊ��ע��Լ��
			OpBool isLocked() const; // �Ƿ�����
			OpQuadConI* getImpl() const; // ��ȡimpl
			OpLInt getIndex() const; // ��ȡ����
			OpEnv getEnv() const; // ��ȡ��������
			void release(); // �ͷű���(�ֶ��ͷ�)
			void lock(); // ����Լ��(�������ܸ��ı��ʽ�ͷ��ţ���ִ�б�׼��)
			void unlock(); // ����
		public:
			OpBool operator==(const OpQuadCon& con);
			OpBool operator!=(const OpQuadCon& con);
		public:
			OpQuadCon(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpQuadCon(OpQuadConI* impl); // ��impl����
			OpQuadCon(OpEnv env); // ��env����
			OpQuadCon(OpEnv env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs); // ��env���첢ָ�����ֲ���
			OpQuadCon(OpEnv env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs, OpStr name); // ��env���첢ָ�����ֲ���
		};

		// OpSOSCon��OPUA��SOSԼ����
		// SOS(special ordered sets, ����˳��)
		class OpSOSCon
		{
		protected:
			OpSOSConI* impl_;
		public:
			void setSense(OpConSense sense); // ����(�ı�)Լ���ķ���
			void setName(OpStr name); // ����Լ������
			void addVar(Variable::OpVar var, OpFloat weight = 1.0); // ��SOS�����һ������(�ظ������׷��Ȩ��)
			
			OpConSense getSense() const; // ��ȡԼ���ķ���
			OpStr getName() const; // ��ȡԼ������		
			const Expression::OpLinExpr& getSOSExpr() const; // ��ȡ��ʾSOS���ϵı��ʽ(����*Ȩ��)
			void removeVar(Variable::OpVar var);  // ��SOS���Ƴ�һ������
			OpBool isLocked() const; // �Ƿ�����
			OpSOSConI* getImpl() const; // ��ȡimpl
			OpLInt getIndex() const; // ��ȡ����
			OpEnv getEnv() const; // ��ȡ��������
			void release(); // �ͷű���(�ֶ��ͷ�)
			void lock(); // ����Լ��(�������ܸ��ı����ͷ���)
			void unlock(); // ����
		public:
			OpBool operator==(const OpSOSCon& con);
			OpBool operator!=(const OpSOSCon& con);
		public:
			OpSOSCon(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpSOSCon(OpSOSConI* impl); // ��impl����
			OpSOSCon(OpEnv env); // ��env����
			OpSOSCon(OpEnv env, OpConSense sense); // ��env���첢ָ�����ֲ���
			OpSOSCon(OpEnv env, OpConSense sense, const Expression::OpLinExpr& expr); // ��env���첢ָ�����ֲ���(����SOS���ʽ)
			OpSOSCon(OpEnv env, OpConSense sense, const Expression::OpLinExpr& expr, OpStr name); // ��env���첢ָ�����ֲ���(����SOS���ʽ)
			OpSOSCon(OpEnv env, OpConSense sense, Variable::OpVarArr vars, Container::OpFloatArr weights); // ��env���첢ָ�����ֲ���
			OpSOSCon(OpEnv env, OpConSense sense, Variable::OpVarArr vars, Container::OpFloatArr weights, OpStr name); // ��env���첢ָ�����ֲ���
		};
	}
}
