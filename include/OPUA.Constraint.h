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
		class OpConicConI;
		class OpConicCon;
		class OpPSDConI;
		class OpPSDCon;
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
		typedef Container::OpArray<OpConicCon> OpConicArr; // ׶Լ������
		typedef Container::OpArray<OpPSDCon> OpPSDCArr; // �붨Լ������
		typedef Container::OpArray<OpSOSCon> OpSOSArr; // SOSԼ������
		typedef Container::OpArray<OpNLCon> OpNLCArr; // ������Լ������
		typedef Container::OpArray<OpCdtCon> OpCCArr; // ����Լ������
		typedef Container::OpDict<OpLInt, OpLinCon> OpLCIdxDict; // ����Լ�������ֵ�
		typedef Container::OpDict<OpLInt, OpQuadCon> OpQCIdxDict; // ����Լ�������ֵ�
		typedef Container::OpDict<OpLInt, OpConicCon> OpConicIdxDict; // ׶Լ�������ֵ�
		typedef Container::OpDict<OpLInt, OpPSDCon> OpPSDCIdxDict; // �붨Լ�������ֵ�
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

		// OPUA׶����
		enum class OpConicSense
		{
			Unknow, // δ֪
			SOC, // ��׼����׶ x0>=Sqrt(Sum(xj^^2))
			RSOC, // ��ת����׶ 2*x0*x1>=Sum(xj^^2)
			PC, // ��׶ x0^^(a)*x1^^(1-a)>=Sqrt(Sum(xj^^2))
			DPC, // ��ż��׶ (x0/a)^^(a)*(x1/1-a)^^(1-a)>=Sqrt(Sum(xj^^2))
			EC, // ָ��׶ x0>=x1*Exp(x2/x1)
			DEC // ��żָ��׶ x0>=-x2*Exp(x1/x2)
		};

		// ���棬��������/����Լ��ʱ������������ <=/>=/== �����������򽫷��ؿ�Լ��
		std::ostream& operator<<(std::ostream& stream, OpLinCon con);
		OpLinCon operator<=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);
		OpLinCon operator<=(OpLinCon lhs, OpFloat rhs);
		OpLinCon operator>=(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);
		OpLinCon operator>=(OpLinCon lhs, OpFloat rhs);
		OpLinCon operator==(const Expression::OpLinExpr& lhs, const Expression::OpLinExpr& rhs);

		std::ostream& operator<<(std::ostream& stream, OpQuadCon con);
		OpQuadCon operator<=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);
		OpQuadCon operator<=(OpQuadCon lhs, OpFloat rhs);
		OpQuadCon operator>=(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);
		OpQuadCon operator>=(OpQuadCon lhs, OpFloat rhs);
		OpQuadCon operator==(const Expression::OpQuadExpr& lhs, const Expression::OpQuadExpr& rhs);
		// ���棬������׼����׶Լ��ʱ��X��������Ϊ2�����򽫷��ؿ�Լ��
		OpQuadCon OpStdSOCByQC(OpEnv env, Variable::OpVarArr X); // ��׼����׶Լ��(����Լ����ʽ)��x0>=Sqrt(Sum(xj^^2))
		// ���棬������׼��ת����׶Լ��ʱ��X��������Ϊ3�����򽫷��ؿ�Լ��
		OpQuadCon OpStdRSOCByQC(OpEnv env, Variable::OpVarArr X); // ��׼��ת����׶Լ��(����Լ����ʽ)��2*x0*x1>=Sum(xj^^2)

		std::ostream& operator<<(std::ostream& stream, OpConicCon con);

		// ���棬����PSDԼ��ʱ�������PSD���ʽ����Ϊ�գ����򽫷��ؿ�Լ��
		std::ostream& operator<<(std::ostream& stream, OpPSDCon con);
		OpPSDCon operator<=(const Expression::OpPSDExpr& lhs, const Expression::OpPSDExpr& rhs);
		OpPSDCon operator<=(OpPSDCon lhs, OpFloat rhs);
		OpPSDCon operator>=(const Expression::OpPSDExpr& lhs, const Expression::OpPSDExpr& rhs);
		OpPSDCon operator>=(OpPSDCon lhs, OpFloat rhs);
		OpPSDCon operator==(const Expression::OpPSDExpr& lhs, const Expression::OpPSDExpr& rhs);

		std::ostream& operator<<(std::ostream& stream, OpSOSCon con);

		// ���棬����������Լ��ʱ������ķ����Ա��ʽ����Ϊ�գ����򽫷��ؿ�Լ��
		std::ostream& operator<<(std::ostream& stream, OpNLCon con);
		OpNLCon operator==(const Expression::OpNLExpr& lhs, Variable::OpVar rhs);
		OpNLCon operator==(Variable::OpVar lhs, const Expression::OpNLExpr& rhs);

		// ���棬��������Լ��ʱ��->��������Լ��/�������ܻ��������Ϊ�������Ҳ�ΪԼ��
		std::ostream& operator<<(std::ostream& stream, OpCdtCon con);
		OpCdtCon operator>>(Variable::OpVar lhs, OpLinCon rhs);
		OpCdtCon operator>>(OpLinCon lhs, OpLinCon rhs);

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

		// OpLinCon��OPUA������Լ����
		// ��ʽ��lb <= expr <= ub
		class OpLinCon
			: public OpBase
		{
		public:
			void setName(OpStr name); // ����Լ������
			void setExpr(const Expression::OpLinExpr& expr); // ����Լ�����ʽ
			void setLb(OpFloat lb); // ����Լ������
			void setUb(OpFloat ub); // ����Լ������
			OpInt standardize(); // Լ����׼��(-2-��ЧԼ��(���˶�Ϊinf��nan)�޷���׼�� / -1-RangeԼ���޷���׼�� / 0-��׼Լ��(��ʽ����ڵ���Լ��)�����׼�� / 1-�ѱ�׼��)
			OpStr getName() const; // ��ȡԼ������
			const Expression::OpLinExpr& getExpr() const; // ��ȡԼ�����ʽ
			OpFloat getLb() const; // ��ȡԼ������
			OpFloat getUb() const; // ��ȡԼ������
			OpLinConI* getImpl() const; // ��ȡimpl
		public:
			OpBool operator==(const OpLinCon& con) const;
			OpBool operator!=(const OpLinCon& con) const;
		public:
			OpLinCon(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpLinCon(OpLinConI* impl); // ��impl����
			OpLinCon(OpEnv env); // ��env����
			OpLinCon(OpEnv env, OpFloat lb, const Expression::OpLinExpr& expr, OpFloat ub); // ��env���첢ָ�����ֲ���
			OpLinCon(OpEnv env, OpFloat lb, const Expression::OpLinExpr& expr, OpFloat ub, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpLinCon();
		};

		// OpQuadCon��OPUA�Ķ���Լ����
		// ��ʽ��lb <= expr <= ub
		class OpQuadCon
			: public OpBase
		{
		public:
			void setName(OpStr name); // ����Լ������
			void setExpr(const Expression::OpQuadExpr& expr); // ����Լ�����ʽ
			void setLb(OpFloat lb); // ����Լ������
			void setUb(OpFloat ub); // ����Լ������
			OpStr getName() const; // ��ȡԼ������
			const Expression::OpQuadExpr& getExpr() const; // ��ȡԼ�����ʽ
			OpFloat getLb() const; // ��ȡԼ������
			OpFloat getUb() const; // ��ȡԼ������
			OpQuadConI* getImpl() const; // ��ȡimpl
		public:
			OpBool operator==(const OpQuadCon& con) const;
			OpBool operator!=(const OpQuadCon& con) const;
		public:
			OpQuadCon(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpQuadCon(OpQuadConI* impl); // ��impl����
			OpQuadCon(OpEnv env); // ��env����
			OpQuadCon(OpEnv env, OpFloat lb, const Expression::OpQuadExpr& expr, OpFloat ub); // ��env���첢ָ�����ֲ���
			OpQuadCon(OpEnv env, OpFloat lb, const Expression::OpQuadExpr& expr, OpFloat ub, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpQuadCon();
		};

		// OpConicCon��OPUA��׶Լ����
		// ��ʽ��X\in C
		class OpConicCon
			: public OpBase
		{
		public:
			void setSense(OpConicSense sense); // ����(�ı�)׶���ϵ�����
			void setName(OpStr name); // ����Լ������
			void addVar(Variable::OpVar var); // ��׶���ϱ����������һ������(����)
			void addVar(Variable::OpVarArr vars); // ��׶���ϱ����������һ�����(����)
			void removeVar(Variable::OpVar var);  // ��׶���ϱ��������Ƴ�һ������
			void removeVar(Variable::OpVarArr vars);  // ��׶���ϱ��������Ƴ�һ�����
			void removeVar(OpULInt n = 0); // ��׶���ϱ��������Ƴ�ĩβ�Ķ������
			OpConicSense getSense() const; // ��ȡ׶���ϵ�����
			const Expression::OpNLExpr& getConicExpr() const; // ��ȡ��ʾ׶���ϱ�����ı��ʽ
			OpStr getName() const; // ��ȡԼ������
			OpConicConI* getImpl() const; // ��ȡimpl
		public:
			OpBool operator==(const OpConicCon& con) const;
			OpBool operator!=(const OpConicCon& con) const;
		public:
			OpConicCon(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpConicCon(OpConicConI* impl); // ��impl����
			OpConicCon(OpEnv env); // ��env����
			OpConicCon(OpEnv env, OpConicSense sense); // ��env���첢ָ�����ֲ���
			OpConicCon(OpEnv env, OpConicSense sense, const Expression::OpNLExpr& expr); // ��env���첢ָ�����ֲ���
			OpConicCon(OpEnv env, OpConicSense sense, const Expression::OpNLExpr& expr, OpStr name); // ��env���첢ָ�����ֲ���
			OpConicCon(OpEnv env, OpConicSense sense, Variable::OpVarArr vars); // ��env���첢ָ�����ֲ���
			OpConicCon(OpEnv env, OpConicSense sense, Variable::OpVarArr vars, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpConicCon();
		};

		// OpPSDCon��OPUA�İ붨Լ����
		// ��ʽ��lb <= expr <= ub
		class OpPSDCon
			: public OpBase
		{
		public:
			void setName(OpStr name); // ����Լ������
			void setExpr(const Expression::OpPSDExpr& expr); // ����Լ�����ʽ
			void setLb(OpFloat lb); // ����Լ������
			void setUb(OpFloat ub); // ����Լ������
			OpStr getName() const; // ��ȡԼ������
			const Expression::OpPSDExpr& getExpr() const; // ��ȡԼ�����ʽ
			OpFloat getLb() const; // ��ȡԼ������
			OpFloat getUb() const; // ��ȡԼ������
			OpPSDConI* getImpl() const; // ��ȡimpl
		public:
			OpBool operator==(const OpPSDCon& con) const;
			OpBool operator!=(const OpPSDCon& con) const;
		public:
			OpPSDCon(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpPSDCon(OpPSDConI* impl); // ��impl����
			OpPSDCon(OpEnv env); // ��env����
			OpPSDCon(OpEnv env, OpFloat lb, const Expression::OpPSDExpr& expr, OpFloat ub); // ��env���첢ָ�����ֲ���
			OpPSDCon(OpEnv env, OpFloat lb, const Expression::OpPSDExpr& expr, OpFloat ub, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpPSDCon();
		};

		// OpSOSCon��OPUA��SOSԼ����
		// SOS(special ordered sets, ����˳��)
		// ��ʽ��X\in S
		class OpSOSCon
			: public OpBase
		{
		public:
			void setSense(OpConSense sense); // ����(�ı�)Լ���ķ���
			void setName(OpStr name); // ����Լ������
			void addVar(Variable::OpVar var, OpFloat weight = 1.0); // ��SOS�����һ������(�ظ������׷��Ȩ��)
			void addVar(Variable::OpVarArr vars, Container::OpFloatArr weights); // ��SOS�����һ�����(�ظ������׷��Ȩ��)
			void removeVar(Variable::OpVar var);  // ��SOS���Ƴ�һ������
			void removeVar(Variable::OpVarArr vars);  // ��SOS���Ƴ�һ�����
			OpConSense getSense() const; // ��ȡԼ���ķ���
			OpStr getName() const; // ��ȡԼ������		
			const Expression::OpLinExpr& getSOSExpr() const; // ��ȡ��ʾSOS���ϵı��ʽ(����*Ȩ��)	
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
