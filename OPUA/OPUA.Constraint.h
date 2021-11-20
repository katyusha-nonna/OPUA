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
		class OpSOSConI;
		class OpSOSCon;
		class OpNLConI;
		class OpNLCon;

		typedef Container::OpArray<OpLinCon> OpLCArr; // ����Լ������
		typedef Container::OpArray<OpQuadCon> OpQCArr; // ����Լ������
		typedef Container::OpArray<OpSOSCon> OpSOSArr; // SOSԼ������
		typedef Container::OpArray<OpNLCon> OpNLCArr; // ������Լ������
		typedef Container::OpDict<OpLInt, OpLinCon> OpLCIdxDict; // ����Լ�������ֵ�
		typedef Container::OpDict<OpLInt, OpQuadCon> OpQCIdxDict; // ����Լ�������ֵ�
		typedef Container::OpDict<OpLInt, OpSOSCon> OpSOSIdxDict; // SOSԼ�������ֵ�
		typedef Container::OpDict<OpLInt, OpNLCon> OpNLCIdxDict; // ������Լ�������ֵ�

		enum class OpConSense
		{
			Equal, // ��ʽԼ��
			LessEqual, // С�ڵ���Լ��
			GreatEqual, // ���ڵ���Լ��
			SOS1, // SOS1Լ��
			SOS2 // SOS2Լ��
		};

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

		std::ostream& operator<<(std::ostream& stream, OpNLCon con);
		OpNLCon operator==(const Expression::OpNLExpr& lhs, Variable::OpVar rhs);
		OpNLCon operator==(Variable::OpVar lhs, const Expression::OpNLExpr& rhs);

		// ���棬������˳���ϵ����������λ�ã������������˵��
		OpNLCon OpSum(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X); // x1 = Sum(X)
		OpNLCon OpMax(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X); // x1 = Max(X)
		OpNLCon OpMin(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X); // x1 = Min(X)
		OpNLCon OpAbs(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = Abs(x2)
		OpNLCon OpSquare(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = x2^^2
		OpNLCon OpSqrt(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = x2^^0.5
		OpNLCon OpPow(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, Variable::OpVar x3); // x1 = x2^^x3
		OpNLCon OpExp1(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = e^^x2
		OpNLCon OpExp2(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, Variable::OpVar x3); // x1 = x2^^x3
		OpNLCon OpLog1(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = log (10) x2
		OpNLCon OpLog2(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = log (e) x2
		OpNLCon OpLog3(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, Variable::OpVar x3); // x1 = log (x2) x3
		OpNLCon OpSin(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = sin(x2)
		OpNLCon OpCos(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = cos(x2)
		OpNLCon OpTan(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = tan(x2)

		OpStr ConSense2Str(OpConSense sense); // ��Լ������ת��Ϊ�ַ�

		class OpLinCon
			: public OpBase
		{
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
			OpLinConI* getImpl() const; // ��ȡimpl
		public:
			OpBool operator==(const OpLinCon& con);
			OpBool operator!=(const OpLinCon& con);
		public:
			OpLinCon(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpLinCon(OpLinConI* impl); // ��impl����
			OpLinCon(OpEnv env); // ��env����
			OpLinCon(OpEnv env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs); // ��env���첢ָ�����ֲ���
			OpLinCon(OpEnv env, const Expression::OpLinExpr& lhs, OpConSense sense, const Expression::OpLinExpr& rhs, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpLinCon();
		};

		class OpQuadCon
			: public OpBase
		{
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
			OpQuadConI* getImpl() const; // ��ȡimpl
		public:
			OpBool operator==(const OpQuadCon& con);
			OpBool operator!=(const OpQuadCon& con);
		public:
			OpQuadCon(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpQuadCon(OpQuadConI* impl); // ��impl����
			OpQuadCon(OpEnv env); // ��env����
			OpQuadCon(OpEnv env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs); // ��env���첢ָ�����ֲ���
			OpQuadCon(OpEnv env, const Expression::OpQuadExpr& lhs, OpConSense sense, const Expression::OpQuadExpr& rhs, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpQuadCon();
		};

		// OpSOSCon��OPUA��SOSԼ����
		// SOS(special ordered sets, ����˳��)
		class OpSOSCon
			: public OpBase
		{
		public:
			void setSense(OpConSense sense); // ����(�ı�)Լ���ķ���
			void setName(OpStr name); // ����Լ������
			void addVar(Variable::OpVar var, OpFloat weight = 1.0); // ��SOS�����һ������(�ظ������׷��Ȩ��)
			
			OpConSense getSense() const; // ��ȡԼ���ķ���
			OpStr getName() const; // ��ȡԼ������		
			const Expression::OpLinExpr& getSOSExpr() const; // ��ȡ��ʾSOS���ϵı��ʽ(����*Ȩ��)
			void removeVar(Variable::OpVar var);  // ��SOS���Ƴ�һ������
			OpSOSConI* getImpl() const; // ��ȡimpl
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
		public:
			virtual ~OpSOSCon();
		};

		// OpNLCon��OPUA�ķ�����Լ����
		// ��ʽ��x = expr
		class OpNLCon
			: public OpBase
		{
		public:
			void setName(OpStr name); // ����Լ������
			void setVar(Variable::OpVar var); // ���ñ���(Լ���������)
			void setExpr(const Expression::OpNLExpr& expr); // ���÷����Ա��ʽ(Լ���Ҳ�����)
			Expression::OpNLFunc getFunction() const; // ��ȡ������Լ���Ĳ�������
			const Expression::OpNLExpr& getExpr() const; // ��ȡ�����Ա��ʽ
			Variable::OpVar getVar() const; // ��ȡ����
			OpStr getName() const; // ��ȡԼ������
			OpNLConI* getImpl() const; // ��ȡimpl
		public:
			OpBool operator==(const OpNLCon& con);
			OpBool operator!=(const OpNLCon& con);
		public:
			OpNLCon(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpNLCon(OpNLConI* impl); // ��impl����
			OpNLCon(OpEnv env); // ��env����
			OpNLCon(OpEnv env, Variable::OpVar var, const Expression::OpNLExpr& expr); // ��env���첢ָ�����ֲ���
			OpNLCon(OpEnv env, Variable::OpVar var, const Expression::OpNLExpr& expr, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpNLCon();
		};
	}
}
