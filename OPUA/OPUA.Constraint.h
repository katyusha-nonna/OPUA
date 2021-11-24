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
		class OpCdtConI;
		class OpCdtCon;
		class OpLogicConI; // TODO: �߼�Լ��(AND / OR / NOR)
		class OpLogicCon;

		typedef Container::OpArray<OpLinCon> OpLCArr; // ����Լ������
		typedef Container::OpArray<OpQuadCon> OpQCArr; // ����Լ������
		typedef Container::OpArray<OpSOSCon> OpSOSArr; // SOSԼ������
		typedef Container::OpArray<OpNLCon> OpNLCArr; // ������Լ������
		typedef Container::OpArray<OpCdtCon> OpCCArr; // ����Լ������
		typedef Container::OpDict<OpLInt, OpLinCon> OpLCIdxDict; // ����Լ�������ֵ�
		typedef Container::OpDict<OpLInt, OpQuadCon> OpQCIdxDict; // ����Լ�������ֵ�
		typedef Container::OpDict<OpLInt, OpSOSCon> OpSOSIdxDict; // SOSԼ�������ֵ�
		typedef Container::OpDict<OpLInt, OpNLCon> OpNLCIdxDict; // ������Լ�������ֵ�
		typedef Container::OpDict<OpLInt, OpCdtCon> OpCCIdxDict; // ����Լ�������ֵ�

		enum class OpConSense
		{
			Equal, // ��ʽԼ��
			LessEqual, // С�ڵ���Լ��
			GreatEqual, // ���ڵ���Լ��
			SOS1, // SOS1Լ��
			SOS2 // SOS2Լ��
		};

		// ���棬��������/����Լ��ʱ������������ <=/>=/== �����������򽫷��ؿ�Լ��
		std::ostream& operator<<(std::ostream& stream, OpLinCon con);
		OpLinCon operator<=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);
		OpLinCon operator>=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);
		OpLinCon operator==(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);

		std::ostream& operator<<(std::ostream& stream, OpQuadCon con);
		OpQuadCon operator<=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);
		OpQuadCon operator>=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);
		OpQuadCon operator==(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);

		std::ostream& operator<<(std::ostream& stream, OpSOSCon con);

		// ���棬����������Լ��ʱ������ķ����Ա��ʽ����Ϊ�գ����򽫷��ؿ�Լ��
		std::ostream& operator<<(std::ostream& stream, OpNLCon con);
		OpNLCon operator==(const Expression::OpNLExpr& lhs, Variable::OpVar rhs);
		OpNLCon operator==(Variable::OpVar lhs, const Expression::OpNLExpr& rhs);

		// ���棬��������Լ��ʱ��==��������Լ��/�������ܻ��������Ϊ�������Ҳ�ΪԼ��
		std::ostream& operator<<(std::ostream& stream, OpCdtCon con);
		OpCdtCon operator==(Variable::OpVar lhs, OpLinCon rhs);
		OpCdtCon operator==(OpLinCon lhs, OpLinCon rhs);

		// ���棬������˳���ϵ����������λ�ã������������˵��
		OpNLCon OpSum(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X, OpFloat a); // x1 = Sum(X, a)
		OpNLCon OpMax(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X, OpFloat a); // x1 = Max(X, a)
		OpNLCon OpMin(OpEnv env, Variable::OpVar x1, Variable::OpVarArr X, OpFloat a); // x1 = Min(X, a)
		OpNLCon OpAbs(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = Abs(x2)
		OpNLCon OpSquare(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = x2^^2
		OpNLCon OpSqrt(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = x2^^0.5
		OpNLCon OpPow(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, OpFloat a); // x1 = x2^^a
		OpNLCon OpExp1(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = e^^x2
		OpNLCon OpExp2(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, OpFloat a); // x1 = a^^x2
		OpNLCon OpLog1(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = log (10) x2
		OpNLCon OpLog2(OpEnv env, Variable::OpVar x1, Variable::OpVar x2); // x1 = log (e) x2
		OpNLCon OpLog3(OpEnv env, Variable::OpVar x1, Variable::OpVar x2, OpFloat a); // x1 = log (a) x2
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
			OpBool operator==(const OpLinCon& con) const;
			OpBool operator!=(const OpLinCon& con) const;
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
			OpBool operator==(const OpQuadCon& con) const;
			OpBool operator!=(const OpQuadCon& con) const;
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
			OpBool operator==(const OpSOSCon& con) const;
			OpBool operator!=(const OpSOSCon& con) const;
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
			OpBool operator==(const OpNLCon& con) const;
			OpBool operator!=(const OpNLCon& con) const;
		public:
			OpNLCon(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpNLCon(OpNLConI* impl); // ��impl����
			OpNLCon(OpEnv env); // ��env����
			OpNLCon(OpEnv env, Variable::OpVar var, const Expression::OpNLExpr& expr); // ��env���첢ָ�����ֲ���
			OpNLCon(OpEnv env, Variable::OpVar var, const Expression::OpNLExpr& expr, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpNLCon();
		};

		// OpCdtCon��OPUA������Լ����
		// ��ʽ1(ָʾ��Լ��)��ind -> con (��ind=1ʱ��con��Ч����ind=0ʱ��con���Ա�Υ��)
		// ��ʽ2(If-ThenԼ��)��con1 -> con2 (��con1��Чʱ��con2��Ч����con1��Υ��ʱ��con2���Ա�Υ��)
		class OpCdtCon
			: public OpBase
		{
		public:
			void setName(OpStr name); // ����Լ������
			void setVar(Variable::OpVar var); // ����ָʾ������
			void setCon(OpLinCon con, OpBool con1); // ����Լ��
			Variable::OpVar getVar() const; // ��ȡָʾ������
			OpLinCon getCon(OpBool con1) const; // ��ȡIF/ThenԼ��
			OpBool isIndicator() const; // �Ƿ�Ϊָʾ��Լ��
			OpStr getName() const; // ��ȡԼ������
			OpCdtConI* getImpl() const; // ��ȡimpl
			void lock();  // ��������Լ��(����ͨ��lock��ͬ����ͬʱ����con��con1&con2)
			void unlock();// ��������Լ��(����ͨ��lock��ͬ����ͬʱ����con��con1&con2)
		public:
			OpBool operator==(const OpCdtCon& con) const;
			OpBool operator!=(const OpCdtCon& con) const;
		public:
			OpCdtCon(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpCdtCon(OpCdtConI* impl); // ��impl����
			OpCdtCon(OpEnv env); // ��env����
			OpCdtCon(OpEnv env, Variable::OpVar ind, OpLinCon con); // ��env���첢ָ�����ֲ���
			OpCdtCon(OpEnv env, Variable::OpVar ind, OpLinCon con, OpStr name); // ��env���첢ָ�����ֲ���
			OpCdtCon(OpEnv env, OpLinCon con1, OpLinCon con2); // ��env���첢ָ�����ֲ���
			OpCdtCon(OpEnv env, OpLinCon con1, OpLinCon con2, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpCdtCon();
		};
	}
}
