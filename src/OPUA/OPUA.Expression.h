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
		class OpDenseMat;
		class OpSpraseMat;
		class OpPSDExpr;
		class OpNLExpr;
		
		typedef OpSpraseMat OpPSDRealMat; // PSDʵϵ������(ʹ��ϡ�����洢)
		typedef Container::OpArray<OpLinExpr> OpLEArr; // ���Ա��ʽ����
		typedef Container::OpArray<OpQuadExpr> OpQEArr; // ���α��ʽ����
		typedef Container::OpArray<OpPSDExpr> OpPSDEArr; // �붨���ʽ����
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

		std::ostream& operator<<(std::ostream& stream, const OpDenseMat& mat);
		std::ostream& operator<<(std::ostream& stream, const OpSpraseMat& mat);
		std::ostream& operator<<(std::ostream& stream, const OpPSDExpr& expr);
		OpPSDExpr operator+(const OpPSDExpr& expr1, const OpPSDExpr& expr2);
		OpPSDExpr operator-(const OpPSDExpr& expr1, const OpPSDExpr& expr2);
		OpPSDExpr operator+(const OpPSDExpr& expr);
		OpPSDExpr operator-(const OpPSDExpr& expr);
		OpPSDExpr operator*(const OpPSDExpr& expr, OpFloat val);
		OpPSDExpr operator*(OpFloat val, const OpPSDExpr& expr);
		OpPSDExpr operator*(Variable::OpPSDVar var, const OpPSDRealMat& mat);
		OpPSDExpr operator*(const OpPSDRealMat& mat, Variable::OpPSDVar var);
		OpPSDExpr operator/(const OpPSDExpr& expr, OpFloat val);

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
			friend class OpPSDExpr;
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

		// OpDenseMat�����ܾ���
		class OpDenseMat
		{
		protected:
			using DMValTab = std::vector<OpFloat>;

			OpULInt dim_; // ����ά��
			DMValTab data_; // ����Ԫ��

			friend class OpPSDExpr;
			friend std::ostream& operator<<(std::ostream& stream, const OpDenseMat& mat);
		private:
			OpBool isLegal(OpULInt row, OpULInt col) const;
			OpULInt coord2Idx(OpULInt row, OpULInt col) const;
		public:
			OpULInt getDim() const; // ��ȡά��
			OpFloat getVal(OpULInt row, OpULInt col) const; // ��ȡ��Ӧ���е�ֵ(�Ƿ�����������NaN)
			OpFloat& getValRef(OpULInt row, OpULInt col); // ��ȡ��Ӧ���е�ֵ������(�Ƿ��������׳��쳣)
			OpFloat trace() const; // ��(�Խ���֮��)
			OpBool isZeros(OpFloat zero) const; // �ж��Ƿ�Ϊ����(����Ԫ�ؾ���ֵ��С��zero)
			void setDim(OpULInt dim); // ����ά��(ԭ����ʧЧ)
			void setVal(OpULInt row, OpULInt col, OpFloat val); // д���Ӧ���е�ֵ(�Ƿ��������׳��쳣)
			void setVal(OpULInt dim, Container::OpFloatArr vals); // д�����(һάģʽ)
			void toZeros(); // תΪ����(ԭ����ʧЧ)
			void toZeros(OpULInt dim); // תΪָ��ά�ȵ�����(ԭ����ʧЧ)
			void toEyes(); // תΪ��λ�Խ���(ԭ����ʧЧ)
			void toEyes(OpULInt dim); // תΪָ��ά�ȵĵ�λ�Խ���(ԭ����ʧЧ)
			void toOnes(); // תΪ��λ��(ԭ����ʧЧ)
			void toOnes(OpULInt dim); // תΪָ��ά�ȵĵ�λ��(ԭ����ʧЧ)
			void piecewiseProd(OpFloat val); // ��λ����-��
			void piecewiseDiv(OpFloat val); // ��λ����-��
			void piecewiseInv(); // ��λ����-ȡ��
		public:
			OpDenseMat& operator+=(const OpDenseMat& mat);
			OpDenseMat& operator-=(const OpDenseMat& mat);
			OpDenseMat& operator*=(OpFloat val);
			OpDenseMat& operator/=(OpFloat val);
		public:
			OpDenseMat();
			OpDenseMat(OpULInt dim);
			OpDenseMat(OpULInt dim, Container::OpFloatArr vals);
		};

		// OpSpraseMat��ϡ�����(��ѹ��ģʽ)
		class OpSpraseMat
		{
		protected:
			using SMIdxTab = std::vector<OpULInt>;
			using SMValTab = std::vector<OpFloat>;

			OpULInt dim_; // ����ά��
			SMIdxTab col_; // ����ϡ��Ԫ���к�
			SMIdxTab off_; // ����ϡ��Ԫ����ƫ��
			SMValTab data_; // ����ϡ��Ԫ��

			friend class OpPSDExpr;
			friend std::ostream& operator<<(std::ostream& stream, const OpSpraseMat& mat);
		private:
			OpBool isLegal(OpULInt row, OpULInt col) const;
			OpULInt coord2Idx(OpULInt row, OpULInt col) const;
			OpULInt insert(OpULInt row, OpULInt col, OpFloat val);
		public:
			OpULInt getDim() const; // ��ȡά��
			OpULInt getNZ() const; // ��ȡ����Ԫ������
			OpFloat getVal(OpULInt row, OpULInt col) const; // ��ȡ��Ӧ���е�ֵ(�����ڽ�����0���Ƿ�����������NaN)
			OpFloat& getValRef(OpULInt row, OpULInt col); // ��ȡ��Ӧ���е�ֵ������(�����ڽ�����Ԫ�أ��Ƿ��������׳��쳣)
			OpFloat trace() const; // ��(�Խ���֮��)
			OpBool isZeros(OpFloat zero) const; // �ж��Ƿ�Ϊ����(����Ԫ�ؾ���ֵ��С��zero)
			void setDim(OpULInt dim); // ����ά��(ԭ����ʧЧ)
			void setVal(OpULInt row, OpULInt col, OpFloat val); // д���Ӧ���е�ֵ(�����ڽ�����Ԫ�أ��Ƿ��������׳��쳣)
			void setVal(OpULInt dim, Container::OpULIntArr rows, Container::OpULIntArr cols, Container::OpFloatArr vals); // д�����(һάģʽ)
			void clear(); // ��վ���(ԭ����ʧЧ)
			void toZeros(); // תΪ����(ԭ����ʧЧ)
			void toZeros(OpULInt dim); // תΪָ��ά�ȵ�����(ԭ����ʧЧ)
			void toEyes(); // תΪ��λ�Խ���(ԭ����ʧЧ)
			void toEyes(OpULInt dim); // תΪָ��ά�ȵĵ�λ�Խ���(ԭ����ʧЧ)
			void toOnes(); // תΪ��λ��(ԭ����ʧЧ)
			void toOnes(OpULInt dim); // תΪָ��ά�ȵĵ�λ��(ԭ����ʧЧ)
			void piecewiseProd(OpFloat val); // ��λ����-��
			void piecewiseDiv(OpFloat val); // ��λ����-��
			void piecewiseInv(); // ��λ����-ȡ��
		public:
			OpSpraseMat& operator+=(const OpSpraseMat& mat);
			OpSpraseMat& operator-=(const OpSpraseMat& mat);
			OpSpraseMat& operator*=(OpFloat val);
			OpSpraseMat& operator/=(OpFloat val);
		public:
			class OpSMIter // ϡ��������Ԫ�ص�����
			{
			private:
				using SMIdxTerm = SMIdxTab::const_iterator;
				using SMValTerm = SMValTab::const_iterator;

				SMIdxTerm citer_;
				SMValTerm viter_;

				friend class OpSpraseMat;
			public:
				OpBool operator==(const OpSMIter& iter) const; // �е�����
				OpBool operator!=(const OpSMIter& iter) const; // �з�����
				OpSMIter& operator--(); // ǰ׺�Լ�����
				OpSMIter& operator++(); // ǰ׺�Լ�����
				OpULInt getCol() const; // ��ȡ��ǰԪ���к�
				OpFloat getVal() const; // ��ȡ��ǰԪ��ֵ
			protected:
				OpSMIter(SMIdxTerm col, SMValTerm val);
			};

			OpSMIter getNZBegin(OpULInt row) const; // �õ�һ�еķ���Ԫ�س�ʼ������(�Ƿ��������׳��쳣)
			OpSMIter getNZEnd(OpULInt row) const; // �õ�һ�еķ���Ԫ��β�������(�Ƿ��������׳��쳣)
		public:
			OpSpraseMat();
			OpSpraseMat(OpULInt dim);
			OpSpraseMat(OpULInt dim, Container::OpULIntArr rows, Container::OpULIntArr cols, Container::OpFloatArr vals);
		};

		// OpPSDExpr��OPUA�붨���ʽ \sum(A*X)+b^{T}*x+c, X>=0
		class OpPSDExpr
			: public OpExpr
		{
		public:
		protected:
			using PSDTermTab = std::unordered_map<Variable::OpPSDVarI*, OpPSDRealMat>;

			OpLinExpr linexpr_; // ������
			PSDTermTab psdterm_; // �붨��

			friend std::ostream& operator<<(std::ostream& stream, const OpPSDExpr& expr);
			friend OpPSDExpr operator+(const OpPSDExpr& expr1, const OpPSDExpr& expr2);
			friend OpPSDExpr operator-(const OpPSDExpr& expr1, const OpPSDExpr& expr2);
			friend OpPSDExpr operator+(const OpPSDExpr& expr);
			friend OpPSDExpr operator-(const OpPSDExpr& expr);
			friend OpPSDExpr operator*(const OpPSDExpr& expr, OpFloat val);
			friend OpPSDExpr operator*(OpFloat val, const OpPSDExpr& expr);
			friend OpPSDExpr operator*(Variable::OpPSDVar var, const OpPSDRealMat& mat);
			friend OpPSDExpr operator*(const OpPSDRealMat& mat, Variable::OpPSDVar var);
			friend OpPSDExpr operator/(const OpPSDExpr& expr, OpFloat val);
		public:
			OpULInt getSize() const; // ��ȡ��������붨����Ŀ
			OpEnv getEnv() const; // ��ȡ��������(Ĭ�Ϸ����������е�һ�������Ļ�������������ǰ붨���е�һ�������Ļ�������������Ϊ�ջ�������)
			OpFloat getCoeff(Variable::OpVar var) const; // ��ȡ������������ϵ��
			const OpPSDRealMat& getCoeff(Variable::OpPSDVar var) const; // ��ȡPSD������PSD��ϵ��
			OpLinExpr getLinTerm() const; // ����������
			OpPSDExpr getPSDTerm() const; // ��ð붨���
			OpFloat getConstant() const; // ��ȡ����
			void setLinTerm(OpFloat val); // ����PSD���ʽ������
			void setLinTerm(Variable::OpVar var, OpFloat coeff); // ����PSD���ʽ������
			void setPSDTerm(Variable::OpPSDVar var, const OpPSDRealMat& coeff); // ����PSD���ʽPSD��
			void addLinTerm(OpFloat val); // ��PSD���ʽ��׷��������
			void addLinTerm(Variable::OpVar var, OpFloat coeff); // ��PSD���ʽ��׷��������
			void addPSDTerm(Variable::OpPSDVar var, const OpPSDRealMat& coeff); // ��PSD���ʽ��׷��PSD��
			void subPSDTerm(Variable::OpPSDVar var, const OpPSDRealMat& coeff); // ��PSD���ʽ��׷��PSD��
			void removeLinTerm(Variable::OpVar var); // ��PSD���ʽ���Ƴ�������
			void removePSDTerm(Variable::OpPSDVar var); // ��PSD���ʽ���Ƴ�PSD��
			void clear(); // ���PSD���ʽ
			void simplify(OpFloat zero = Constant::AbsEpsilon); // ���ʽ����
			void piecewiseProd(OpFloat val); // ��λ����-��(�൱��expr *= val)
			void piecewiseDiv(OpFloat val); // ��λ����-��(�൱��expr /= val)
			void piecewiseInv(); // ��λ����-ȡ��(�൱��expr = -expr)
		public:
			OpPSDExpr& operator+=(const OpPSDExpr& expr);
			OpPSDExpr& operator-=(const OpPSDExpr& expr);
			OpPSDExpr& operator*=(OpFloat val);
			OpPSDExpr& operator/=(OpFloat val);
		public:
			class OpPSDEIter // PSD������
			{
			private:
				using PSDTerm = PSDTermTab::const_iterator;

				PSDTerm iter_;

				friend class OpPSDExpr;
			public:
				OpBool operator==(const OpPSDEIter& iter) const; // �е�����
				OpBool operator!=(const OpPSDEIter& iter) const; // �з�����
				OpPSDEIter& operator--(); // ǰ׺�Լ�����
				OpPSDEIter& operator++(); // ǰ׺�Լ�����
				Variable::OpPSDVar getVar() const; // ��ȡ��ǰPSD��ı���
				const OpPSDRealMat& getCoeff() const; // ��ȡ��ǰPSD���ϵ��
			protected:
				OpPSDEIter(PSDTerm iter);
			};

			OpLinExpr::OpLEIter getLBegin() const; // �õ��������ʼ������
			OpLinExpr::OpLEIter getLEnd() const; // �õ�������β�������
			OpPSDEIter getPSDBegin() const; // �õ�PSD���ʼ������
			OpPSDEIter getPSDEnd() const; // �õ�PSD��β�������
		public:
			OpPSDExpr(OpFloat constant = 0.0);
			OpPSDExpr(Variable::OpVar var, OpFloat coeff = 1.0);
			OpPSDExpr(const OpLinExpr& expr);
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
			void addVar(Variable::OpVarArr vars); // ���������׷��һ�����
			void removeVar(Variable::OpVar var);  // �ӱ��������Ƴ�һ������
			void removeVar(Variable::OpVarArr vars);  // �ӱ��������Ƴ�һ�����
			void removeVar(OpULInt n = 0); // �ӱ��������Ƴ�ĩβ�Ķ������
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
