#pragma once

#include "OPUA.Variable.h"
#include <unordered_map>

namespace OPUA
{
	namespace Expression
	{
		class OpExpr;
		class OpLinExpr;
		class OpQuadExpr;
		class OpNLExpr;
		
		typedef Container::OpArray<OpLinExpr> OpLEArr; // ���Ա��ʽ����
		typedef Container::OpArray<OpQuadExpr> OpQEArr; // ���α��ʽ����
		typedef Container::OpArray<OpNLExpr> OpNLEArr; // �����Ա��ʽ����

		std::ostream& operator<<(std::ostream& stream, const OpLinExpr& expr);
		OpLinExpr operator+(const OpLinExpr& expr1, const OpLinExpr& expr2);
		OpLinExpr operator+(const OpLinExpr& expr);
		OpLinExpr operator+(Variable::OpVar var1, Variable::OpVar var2);
		OpLinExpr operator+(Variable::OpVar var, OpFloat val);
		OpLinExpr operator+(OpFloat val, Variable::OpVar var);
		OpLinExpr operator-(const OpLinExpr& expr1, const OpLinExpr& expr2);
		OpLinExpr operator-(const OpLinExpr& expr);
		OpLinExpr operator-(Variable::OpVar var);
		OpLinExpr operator-(Variable::OpVar var1, Variable::OpVar var2);
		OpLinExpr operator-(Variable::OpVar var, OpFloat val);
		OpLinExpr operator-(OpFloat val, Variable::OpVar var);
		OpLinExpr operator*(OpFloat val, Variable::OpVar var);
		OpLinExpr operator*(Variable::OpVar var, OpFloat val);
		OpLinExpr operator*(const OpLinExpr& expr, OpFloat val);
		OpLinExpr operator*(OpFloat val, const OpLinExpr& expr);
		OpLinExpr operator/(Variable::OpVar var, OpFloat val);
		OpLinExpr operator/(const OpLinExpr& expr, OpFloat val);

		std::ostream& operator<<(std::ostream& stream, const OpQuadExpr& expr);
		OpQuadExpr operator+(const OpQuadExpr& expr1, const OpQuadExpr& expr2);
		OpQuadExpr operator-(const OpQuadExpr& expr1, const OpQuadExpr& expr2);
		OpQuadExpr operator+(const OpQuadExpr& expr);
		OpQuadExpr operator-(const OpQuadExpr& expr);
		OpQuadExpr operator*(const OpQuadExpr& expr, OpFloat val);
		OpQuadExpr operator*(OpFloat val, const OpQuadExpr& expr);
		OpQuadExpr operator*(Variable::OpVar var1, Variable::OpVar var2);
		OpQuadExpr operator*(Variable::OpVar var, const OpLinExpr& expr);
		OpQuadExpr operator*(const OpLinExpr& expr, Variable::OpVar var);
		OpQuadExpr operator*(const OpLinExpr& expr1, const OpLinExpr& expr2);
		OpQuadExpr operator/(const OpQuadExpr& expr, OpFloat val);

		// OPUA�����Ժ���
		enum class OpNLFunc
		{
			Unknow, // δ֪
			Sum, // ��� Sum(x1, x2, ..., a)
			Abs, // ����ֵ abs(x1)
			Max, // ȡ���ֵ max(x1, x2, ..., a)
			Min, // ȡ��Сֵ min(x1, x2, ..., a)
			Square, // ƽ��x1^^2
			Sqrt, // ƽ���� sqrt(x1)
			Pow, // �� x1^^a
			Exp1, // ָ�� e^^x1
			Exp2, // ָ�� a^^x1
			Log1, // ���� log (10) x1
			Log2, // ���� log (e) x1
			Log3, // ���� log (a) x1
			Sin, // ���� sin(x1)
			Cos, // ���� cos(x1)
			Tan, // ���� tan(x1)
		};

		OpStr NLFunc2Str(OpNLFunc func); // �������Բ�����������ת��Ϊ�ַ�

		// OpExpr��OPUA���ʽ����
		// ��Ҫʹ�������
		class OpExpr
		{
		protected:

		public:

		};

		// OpLinExpr��OPUA���Ա��ʽ
		class OpLinExpr
			: public OpExpr
		{
		protected:
			using LinTermTab = std::unordered_map<Variable::OpVarI*, OpFloat>;

			OpFloat constant_; // ������
			LinTermTab linterm_; // ������

			friend class OpQuadExpr;

			friend std::ostream& operator<<(std::ostream& stream, const OpLinExpr& expr);
			friend OpLinExpr operator+(const OpLinExpr& expr1, const OpLinExpr& expr2);
			friend OpLinExpr operator+(const OpLinExpr& expr);
			friend OpLinExpr operator+(Variable::OpVar var1, Variable::OpVar var2);
			friend OpLinExpr operator+(Variable::OpVar var, OpFloat val);
			friend OpLinExpr operator+(OpFloat val, Variable::OpVar var);
			friend OpLinExpr operator-(const OpLinExpr& expr1, const OpLinExpr& expr2);
			friend OpLinExpr operator-(const OpLinExpr& expr);
			friend OpLinExpr operator-(Variable::OpVar var);
			friend OpLinExpr operator-(Variable::OpVar var1, Variable::OpVar var2);
			friend OpLinExpr operator-(Variable::OpVar var, OpFloat val);
			friend OpLinExpr operator-(OpFloat val, Variable::OpVar var);
			friend OpLinExpr operator*(OpFloat val, Variable::OpVar var);
			friend OpLinExpr operator*(Variable::OpVar var, OpFloat val);
			friend OpLinExpr operator*(const OpLinExpr& expr, OpFloat val);
			friend OpLinExpr operator*(OpFloat val, const OpLinExpr& expr);
			friend OpLinExpr operator/(Variable::OpVar var, OpFloat val);
			friend OpLinExpr operator/(const OpLinExpr& expr, OpFloat val);
		public:
			OpULInt getSize() const; // ��ȡ��������Ŀ
			OpEnv getEnv() const; // ��ȡ��������(Ĭ�Ϸ����������е�һ�������Ļ�������������Ϊ�ջ�������)
			OpFloat getCoeff(Variable::OpVar var) const; // ��ȡ������ϵ��
			OpLinExpr getLinTerm() const; // ����������
			OpFloat getConstant() const; // ��ȡ����
			void setLinTerm(OpFloat val); // �������Ա��ʽ������
			void setLinTerm(Variable::OpVar var, OpFloat coeff); // �������Ա��ʽ������
			void addLinTerm(OpFloat val); // �����Ա��ʽ��׷��������
			void addLinTerm(Variable::OpVar var, OpFloat coeff); // �����Ա��ʽ��׷��������
			void removeVar(Variable::OpVar var); // �����Ա��ʽ���Ƴ�����
			void clear(); // ������Ա��ʽ
			void simplify(OpFloat zero = Constant::AbsEpsilon); // ���ʽ����
			void piecewiseProd(OpFloat val); // ��λ����-��(�൱��expr *= val)
			void piecewiseDiv(OpFloat val); // ��λ����-��(�൱��expr /= val)
			void piecewiseInv(); // ��λ����-ȡ��(�൱��expr = -expr)
		public:
			OpLinExpr& operator+=(const OpLinExpr& expr);
			OpLinExpr& operator-=(const OpLinExpr& expr);
			OpLinExpr& operator*=(OpFloat val);
			OpLinExpr& operator/=(OpFloat val);
		public:
			class OpLEIter // ���Ե�����
			{
			private:
				using LinTerm = LinTermTab::const_iterator;

				LinTerm iter_;

				friend class OpLinExpr;
			public:
				OpBool operator==(const OpLEIter& iter) const; // �е�����
				OpBool operator!=(const OpLEIter& iter) const; // �з�����
				OpLEIter& operator--(); // ǰ׺�Լ�����
				OpLEIter& operator++(); // ǰ׺�Լ�����
				Variable::OpVar getVar() const; // ��ȡ��ǰ������ı���
				OpFloat getCoeff() const; // ��ȡ��ǰ�������ϵ��
			protected:
				OpLEIter(LinTerm iter);
			};

			OpLEIter getLBegin() const; // �õ��������ʼ������
			OpLEIter getLEnd() const; // �õ�������β�������
		public:
			OpLinExpr(OpFloat constant = 0.0);
			OpLinExpr(Variable::OpVar var, OpFloat coeff = 1.0);
		};

		// �ο���https://youngforest.github.io/2020/05/27/unordered-map-hash-pair-c/
		// һ�ֱ�Xor����Ч��Hasherд��
		template <typename T>
		inline void hash_combine(std::size_t& seed, const T& val)
		{
			seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		template <typename T>
		inline void hash_val(std::size_t& seed, const T& val)
		{
			hash_combine(seed, val);
		}
		template <typename T, typename... Types>
		inline void hash_val(std::size_t& seed, const T& val, const Types &... args)
		{
			hash_combine(seed, val);
			hash_val(seed, args...);
		}

		template <typename... Types>
		inline std::size_t hash_val(const Types &... args)
		{
			std::size_t seed = 0;
			hash_val(seed, args...);
			return seed;
		}

		// OpQuadExpr��OPUA���α��ʽ
		class OpQuadExpr
			: public OpExpr
		{
		protected:
			struct PairHasher
			{
				/*
				template<class T>
				size_t operator()(const std::pair<T, T>& val) const
				{
					std::hash<T> hasher;
					return hasher(val.first) ^ hasher(val.second);
				}
				*/			
				template<class T>
				size_t operator()(const std::pair<T, T>& val) const
				{
					return val.first < val.second ? hash_val(val.first, val.second) : hash_val(val.second, val.first);
				}			
			};

			struct PairEqualityComparer
			{
				template<class T>
				bool operator()(const std::pair<T, T>& x, const std::pair<T, T>& y) const
				{
					return (x.first == y.first && x.second == y.second) ||
						(x.first == y.second && x.second == y.first);
				}
			};
		protected:
			using QuadTermTab = std::unordered_map<
				std::pair<Variable::OpVarI*, Variable::OpVarI*>,
				double,
				PairHasher,
				PairEqualityComparer>;

			OpLinExpr linexpr_; // ������
			QuadTermTab quadterm_;  // ������

			friend std::ostream& operator<<(std::ostream& stream, const OpQuadExpr& expr);
			friend OpQuadExpr operator+(const OpQuadExpr& expr1, const OpQuadExpr& expr2);
			friend OpQuadExpr operator-(const OpQuadExpr& expr1, const OpQuadExpr& expr2);
			friend OpQuadExpr operator+(const OpQuadExpr& expr);
			friend OpQuadExpr operator-(const OpQuadExpr& expr);
			friend OpQuadExpr operator*(const OpQuadExpr& expr, OpFloat val);
			friend OpQuadExpr operator*(OpFloat val, const OpQuadExpr& expr);
			friend OpQuadExpr operator*(Variable::OpVar var1, Variable::OpVar var2);
			friend OpQuadExpr operator*(Variable::OpVar var, const OpLinExpr& expr);
			friend OpQuadExpr operator*(const OpLinExpr& expr, Variable::OpVar var);
			friend OpQuadExpr operator*(const OpLinExpr& expr1, const OpLinExpr& expr2);
			friend OpQuadExpr operator/(const OpQuadExpr& expr, OpFloat val);
		public:
			OpULInt getSize() const; // ��ȡ���������������Ŀ
			OpEnv getEnv() const; // ��ȡ��������(Ĭ�Ϸ����������е�һ�������Ļ�������������Ƕ��������Ϊ�ջ�������)
			OpFloat getCoeff(Variable::OpVar var) const; // ��ȡ������������ϵ��
			OpFloat getCoeff(Variable::OpVar var1, Variable::OpVar var2) const; // ��ȡ�����Ķ�����ϵ��
			OpLinExpr getLinTerm() const; // ����������
			OpQuadExpr getQuadTerm() const; // ��ö������
			OpFloat getConstant() const; // ��ȡ����
			void setLinTerm(OpFloat val); // ���ö��α��ʽ������
			void setLinTerm(Variable::OpVar var, OpFloat coeff); // ���ö��α��ʽ������
			void setQuadTerm(Variable::OpVar var1, Variable::OpVar var2, OpFloat coeff); // ���ö��α��ʽ������
			void addLinTerm(OpFloat val); // �ڶ��α��ʽ��׷��������
			void addLinTerm(Variable::OpVar var, OpFloat coeff); // �ڶ��α��ʽ��׷��������
			void addQuadTerm(Variable::OpVar var1, Variable::OpVar var2, OpFloat coeff); // �ڶ��α��ʽ��׷�Ӷ�����
			void removeVar(Variable::OpVar var); // �Ӷ��α��ʽ���Ƴ�����
			void removeLinTerm(Variable::OpVar var); // �Ӷ��α��ʽ���Ƴ�������
			void removeQuadTerm(Variable::OpVar var1, Variable::OpVar var2); // �Ӷ��α��ʽ���Ƴ�������
			void clear(); // ������α��ʽ
			void simplify(OpFloat zero = Constant::AbsEpsilon); // ���ʽ����
			void piecewiseProd(OpFloat val); // ��λ����-��(�൱��expr *= val)
			void piecewiseDiv(OpFloat val); // ��λ����-��(�൱��expr /= val)
			void piecewiseInv(); // ��λ����-ȡ��(�൱��expr = -expr)
		public:
			OpQuadExpr& operator+=(const OpQuadExpr& expr);
			OpQuadExpr& operator-=(const OpQuadExpr& expr);
			OpQuadExpr& operator*=(OpFloat val);
			OpQuadExpr& operator/=(OpFloat val);
		public:
			class OpQEIter // ���ε�����
			{
			private:
				using QuadTerm = QuadTermTab::const_iterator;

				QuadTerm iter_;

				friend class OpQuadExpr;
			public:
				OpBool operator==(const OpQEIter& iter) const; // �е�����
				OpBool operator!=(const OpQEIter& iter) const; // �з�����
				OpQEIter& operator--(); // ǰ׺�Լ�����
				OpQEIter& operator++(); // ǰ׺�Լ�����
				Variable::OpVar getVar1() const; // ��ȡ��ǰ������ı���1
				Variable::OpVar getVar2() const; // ��ȡ��ǰ������ı���2
				OpFloat getCoeff() const; // ��ȡ��ǰ�������ϵ��
			protected:
				OpQEIter(QuadTerm iter);
			};

			OpLinExpr::OpLEIter getLBegin() const; // �õ��������ʼ������
			OpLinExpr::OpLEIter getLEnd() const; // �õ�������β�������
			OpQEIter getQBegin() const; // �õ��������ʼ������
			OpQEIter getQEnd() const; // �õ�������β�������
		public:
			OpQuadExpr(OpFloat constant = 0.0);
			OpQuadExpr(Variable::OpVar var, OpFloat coeff = 1.0);
			OpQuadExpr(const OpLinExpr& expr);
		};

		class OpNLExpr
			: public OpExpr
		{
		protected:
			using NLTermTab = std::vector<Variable::OpVarI*>;

			OpNLFunc func_; // �����Ժ���
			OpFloat param_; // �����Ժ�������(��ʱֻ����������)
			NLTermTab nlterm_; // ��������(����������)
		public:
			OpULInt getSize() const; // ��ȡ����������Ŀ
			OpEnv getEnv() const; // ��ȡ��������(Ĭ�Ϸ��ط��������е�һ�������Ļ�������������Ϊ�ջ�������)
			OpNLFunc getFunction() const;  // ��ȡ�����Ժ���
			OpFloat getParam() const; // ��ȡ�����Ժ�������
			Variable::OpVar getVar(OpULInt order) const; // ��ȡ���Ϊorder�ı���
			void setFunction(OpNLFunc func); // ���÷����Ժ���		
			void setParam(OpFloat param); // ���÷����Ժ�������
			void addVar(Variable::OpVar var);  // ���������׷��һ������
			void removeVar(Variable::OpVar var);  // �ӱ��������Ƴ�һ������
			void clear(); // ��������Ա��ʽ
		public:
			class OpNLEIter
			{
			private:
				using NLTerm = NLTermTab::const_iterator;

				NLTerm iter_;

				friend class OpNLExpr;
			public:
				OpBool operator==(const OpNLEIter& iter) const; // �е�����
				OpBool operator!=(const OpNLEIter& iter) const; // �з�����
				OpNLEIter& operator--(); // ǰ׺�Լ�����
				OpNLEIter& operator++(); // ǰ׺�Լ�����
				Variable::OpVar getVar() const; // ��ȡ��ǰ����
			protected:
				OpNLEIter(NLTerm iter);
			};

			OpNLExpr::OpNLEIter getNLBegin() const; // �õ����������ʼ������
			OpNLExpr::OpNLEIter getNLEnd() const; // �õ���������β�������
		public:
			OpNLExpr(OpNLFunc func = OpNLFunc::Unknow);
			OpNLExpr(OpNLFunc func, Variable::OpVarArr vars);
			OpNLExpr(OpNLFunc func, OpFloat param, Variable::OpVarArr vars);
		};
	}
}
