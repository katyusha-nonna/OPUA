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
		
		typedef OpSpraseMat OpPSDRealMat; // PSD实系数矩阵(使用稀疏矩阵存储)
		typedef Container::OpArray<OpLinExpr> OpLEArr; // 线性表达式数组
		typedef Container::OpArray<OpQuadExpr> OpQEArr; // 二次表达式数组
		typedef Container::OpArray<OpPSDExpr> OpPSDEArr; // 半定表达式数组
		typedef Container::OpArray<OpNLExpr> OpNLEArr; // 非线性表达式数组

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

		// OPUA非线性函数
		enum class OpNLFunc
		{
			Unknow, // 未知
			Sum, // 求和 Sum(x1, x2, ..., a)
			Abs, // 绝对值 abs(x1)
			Max, // 取最大值 max(x1, x2, ..., a)
			Min, // 取最小值 min(x1, x2, ..., a)
			Square, // 平方x1^^2
			Sqrt, // 平方根 sqrt(x1)
			Pow, // 幂 x1^^a
			Exp1, // 指数 e^^x1
			Exp2, // 指数 a^^x1
			Log1, // 对数 log (10) x1
			Log2, // 对数 log (e) x1
			Log3, // 对数 log (a) x1
			Sin, // 正弦 sin(x1)
			Cos, // 余弦 cos(x1)
			Tan, // 正切 tan(x1)
		};

		OpStr NLFunc2Str(OpNLFunc func); // 将非线性操作函数类型转换为字符

		// OpExpr：OPUA表达式基类
		// 不要使用这个类
		class OpExpr
		{
		protected:

		public:

		};

		// OpLinExpr：OPUA线性表达式
		class OpLinExpr
			: public OpExpr
		{
		protected:
			using LinTermTab = std::unordered_map<Variable::OpVarI*, OpFloat>;

			OpFloat constant_; // 常数项
			LinTermTab linterm_; // 线性项

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
			OpULInt getSize() const; // 获取线性项数目
			OpEnv getEnv() const; // 获取环境变量(默认返回线性项中第一个变量的环境变量，否则为空环境变量)
			OpFloat getCoeff(Variable::OpVar var) const; // 获取变量的系数
			OpLinExpr getLinTerm() const; // 获得线性项部分
			OpFloat getConstant() const; // 获取常量
			void setLinTerm(OpFloat val); // 设置线性表达式常数项
			void setLinTerm(Variable::OpVar var, OpFloat coeff); // 设置线性表达式线性项
			void addLinTerm(OpFloat val); // 在线性表达式中追加线性项
			void addLinTerm(Variable::OpVar var, OpFloat coeff); // 在线性表达式中追加线性项
			void removeVar(Variable::OpVar var); // 从线性表达式中移除变量
			void clear(); // 清除线性表达式
			void simplify(OpFloat zero = Constant::AbsEpsilon); // 表达式化简
			void piecewiseProd(OpFloat val); // 逐位运算-乘(相当于expr *= val)
			void piecewiseDiv(OpFloat val); // 逐位运算-除(相当于expr /= val)
			void piecewiseInv(); // 逐位运算-取负(相当于expr = -expr)
		public:
			OpLinExpr& operator+=(const OpLinExpr& expr);
			OpLinExpr& operator-=(const OpLinExpr& expr);
			OpLinExpr& operator*=(OpFloat val);
			OpLinExpr& operator/=(OpFloat val);
		public:
			class OpLEIter // 线性迭代器
			{
			private:
				using LinTerm = LinTermTab::const_iterator;

				LinTerm iter_;

				friend class OpLinExpr;
			public:
				OpBool operator==(const OpLEIter& iter) const; // 判等运算
				OpBool operator!=(const OpLEIter& iter) const; // 判非运算
				OpLEIter& operator--(); // 前缀自减运算
				OpLEIter& operator++(); // 前缀自加运算
				Variable::OpVar getVar() const; // 获取当前线性项的变量
				OpFloat getCoeff() const; // 获取当前线性项的系数
			protected:
				OpLEIter(LinTerm iter);
			};

			OpLEIter getLBegin() const; // 得到线性项初始迭代器
			OpLEIter getLEnd() const; // 得到线性项尾后迭代器
		public:
			OpLinExpr(OpFloat constant = 0.0);
			OpLinExpr(Variable::OpVar var, OpFloat coeff = 1.0);
		};

		// 参考了https://youngforest.github.io/2020/05/27/unordered-map-hash-pair-c/
		// 一种比Xor更高效的Hasher写法
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

		// OpQuadExpr：OPUA二次表达式
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

			OpLinExpr linexpr_; // 线性项
			QuadTermTab quadterm_;  // 二次项

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
			OpULInt getSize() const; // 获取线性项与二次项数目
			OpEnv getEnv() const; // 获取环境变量(默认返回线性项中第一个变量的环境变量，其次是二次项，否则为空环境变量)
			OpFloat getCoeff(Variable::OpVar var) const; // 获取变量的线性项系数
			OpFloat getCoeff(Variable::OpVar var1, Variable::OpVar var2) const; // 获取变量的二次项系数
			OpLinExpr getLinTerm() const; // 获得线性项部分
			OpQuadExpr getQuadTerm() const; // 获得二次项部分
			OpFloat getConstant() const; // 获取常量
			void setLinTerm(OpFloat val); // 设置二次表达式常数项
			void setLinTerm(Variable::OpVar var, OpFloat coeff); // 设置二次表达式线性项
			void setQuadTerm(Variable::OpVar var1, Variable::OpVar var2, OpFloat coeff); // 设置二次表达式二次项
			void addLinTerm(OpFloat val); // 在二次表达式中追加线性项
			void addLinTerm(Variable::OpVar var, OpFloat coeff); // 在二次表达式中追加线性项
			void addQuadTerm(Variable::OpVar var1, Variable::OpVar var2, OpFloat coeff); // 在二次表达式中追加二次项
			void removeVar(Variable::OpVar var); // 从二次表达式中移除变量
			void removeLinTerm(Variable::OpVar var); // 从二次表达式中移除线性项
			void removeQuadTerm(Variable::OpVar var1, Variable::OpVar var2); // 从二次表达式中移除二次项
			void clear(); // 清除二次表达式
			void simplify(OpFloat zero = Constant::AbsEpsilon); // 表达式化简
			void piecewiseProd(OpFloat val); // 逐位运算-乘(相当于expr *= val)
			void piecewiseDiv(OpFloat val); // 逐位运算-除(相当于expr /= val)
			void piecewiseInv(); // 逐位运算-取负(相当于expr = -expr)
		public:
			OpQuadExpr& operator+=(const OpQuadExpr& expr);
			OpQuadExpr& operator-=(const OpQuadExpr& expr);
			OpQuadExpr& operator*=(OpFloat val);
			OpQuadExpr& operator/=(OpFloat val);
		public:
			class OpQEIter // 二次迭代器
			{
			private:
				using QuadTerm = QuadTermTab::const_iterator;

				QuadTerm iter_;

				friend class OpQuadExpr;
			public:
				OpBool operator==(const OpQEIter& iter) const; // 判等运算
				OpBool operator!=(const OpQEIter& iter) const; // 判非运算
				OpQEIter& operator--(); // 前缀自减运算
				OpQEIter& operator++(); // 前缀自加运算
				Variable::OpVar getVar1() const; // 获取当前二次项的变量1
				Variable::OpVar getVar2() const; // 获取当前二次项的变量2
				OpFloat getCoeff() const; // 获取当前二次项的系数
			protected:
				OpQEIter(QuadTerm iter);
			};

			OpLinExpr::OpLEIter getLBegin() const; // 得到线性项初始迭代器
			OpLinExpr::OpLEIter getLEnd() const; // 得到线性项尾后迭代器
			OpQEIter getQBegin() const; // 得到二次项初始迭代器
			OpQEIter getQEnd() const; // 得到二次项尾后迭代器
		public:
			OpQuadExpr(OpFloat constant = 0.0);
			OpQuadExpr(Variable::OpVar var, OpFloat coeff = 1.0);
			OpQuadExpr(const OpLinExpr& expr);
		};

		// OpDenseMat：稠密矩阵
		class OpDenseMat
		{
		protected:
			using DMValTab = std::vector<OpFloat>;

			OpULInt dim_; // 矩阵维度
			DMValTab data_; // 矩阵元素

			friend class OpPSDExpr;
			friend std::ostream& operator<<(std::ostream& stream, const OpDenseMat& mat);
		private:
			OpBool isLegal(OpULInt row, OpULInt col) const;
			OpULInt coord2Idx(OpULInt row, OpULInt col) const;
		public:
			OpULInt getDim() const; // 获取维度
			OpFloat getVal(OpULInt row, OpULInt col) const; // 获取对应行列的值(非法索引将返回NaN)
			OpFloat& getValRef(OpULInt row, OpULInt col); // 获取对应行列的值的引用(非法索引将抛出异常)
			OpFloat trace() const; // 求迹(对角线之和)
			OpBool isZeros(OpFloat zero) const; // 判断是否为零阵(所有元素绝对值均小于zero)
			void setDim(OpULInt dim); // 设置维度(原数据失效)
			void setVal(OpULInt row, OpULInt col, OpFloat val); // 写入对应行列的值(非法索引将抛出异常)
			void setVal(OpULInt dim, Container::OpFloatArr vals); // 写入矩阵(一维模式)
			void toZeros(); // 转为零阵(原数据失效)
			void toZeros(OpULInt dim); // 转为指定维度的零阵(原数据失效)
			void toEyes(); // 转为单位对角阵(原数据失效)
			void toEyes(OpULInt dim); // 转为指定维度的单位对角阵(原数据失效)
			void toOnes(); // 转为单位阵(原数据失效)
			void toOnes(OpULInt dim); // 转为指定维度的单位阵(原数据失效)
			void piecewiseProd(OpFloat val); // 逐位运算-乘
			void piecewiseDiv(OpFloat val); // 逐位运算-除
			void piecewiseInv(); // 逐位运算-取负
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

		// OpSpraseMat：稀疏矩阵(行压缩模式)
		class OpSpraseMat
		{
		protected:
			using SMIdxTab = std::vector<OpULInt>;
			using SMValTab = std::vector<OpFloat>;

			OpULInt dim_; // 矩阵维度
			SMIdxTab col_; // 矩阵稀疏元素列号
			SMIdxTab off_; // 矩阵稀疏元素行偏移
			SMValTab data_; // 矩阵稀疏元素

			friend class OpPSDExpr;
			friend std::ostream& operator<<(std::ostream& stream, const OpSpraseMat& mat);
		private:
			OpBool isLegal(OpULInt row, OpULInt col) const;
			OpULInt coord2Idx(OpULInt row, OpULInt col) const;
			OpULInt insert(OpULInt row, OpULInt col, OpFloat val);
		public:
			OpULInt getDim() const; // 获取维度
			OpULInt getNZ() const; // 获取非零元素数量
			OpFloat getVal(OpULInt row, OpULInt col) const; // 获取对应行列的值(不存在将返回0，非法索引将返回NaN)
			OpFloat& getValRef(OpULInt row, OpULInt col); // 获取对应行列的值的引用(不存在将插入元素，非法索引将抛出异常)
			OpFloat trace() const; // 求迹(对角线之和)
			OpBool isZeros(OpFloat zero) const; // 判断是否为零阵(所有元素绝对值均小于zero)
			void setDim(OpULInt dim); // 设置维度(原数据失效)
			void setVal(OpULInt row, OpULInt col, OpFloat val); // 写入对应行列的值(不存在将插入元素，非法索引将抛出异常)
			void setVal(OpULInt dim, Container::OpULIntArr rows, Container::OpULIntArr cols, Container::OpFloatArr vals); // 写入矩阵(一维模式)
			void clear(); // 清空矩阵(原数据失效)
			void toZeros(); // 转为零阵(原数据失效)
			void toZeros(OpULInt dim); // 转为指定维度的零阵(原数据失效)
			void toEyes(); // 转为单位对角阵(原数据失效)
			void toEyes(OpULInt dim); // 转为指定维度的单位对角阵(原数据失效)
			void toOnes(); // 转为单位阵(原数据失效)
			void toOnes(OpULInt dim); // 转为指定维度的单位阵(原数据失效)
			void piecewiseProd(OpFloat val); // 逐位运算-乘
			void piecewiseDiv(OpFloat val); // 逐位运算-除
			void piecewiseInv(); // 逐位运算-取负
		public:
			OpSpraseMat& operator+=(const OpSpraseMat& mat);
			OpSpraseMat& operator-=(const OpSpraseMat& mat);
			OpSpraseMat& operator*=(OpFloat val);
			OpSpraseMat& operator/=(OpFloat val);
		public:
			class OpSMIter // 稀疏矩阵非零元素迭代器
			{
			private:
				using SMIdxTerm = SMIdxTab::const_iterator;
				using SMValTerm = SMValTab::const_iterator;

				SMIdxTerm citer_;
				SMValTerm viter_;

				friend class OpSpraseMat;
			public:
				OpBool operator==(const OpSMIter& iter) const; // 判等运算
				OpBool operator!=(const OpSMIter& iter) const; // 判非运算
				OpSMIter& operator--(); // 前缀自减运算
				OpSMIter& operator++(); // 前缀自加运算
				OpULInt getCol() const; // 获取当前元素列号
				OpFloat getVal() const; // 获取当前元素值
			protected:
				OpSMIter(SMIdxTerm col, SMValTerm val);
			};

			OpSMIter getNZBegin(OpULInt row) const; // 得到一行的非零元素初始迭代器(非法索引将抛出异常)
			OpSMIter getNZEnd(OpULInt row) const; // 得到一行的非零元素尾后迭代器(非法索引将抛出异常)
		public:
			OpSpraseMat();
			OpSpraseMat(OpULInt dim);
			OpSpraseMat(OpULInt dim, Container::OpULIntArr rows, Container::OpULIntArr cols, Container::OpFloatArr vals);
		};

		// OpPSDExpr：OPUA半定表达式 \sum(A*X)+b^{T}*x+c, X>=0
		class OpPSDExpr
			: public OpExpr
		{
		public:
		protected:
			using PSDTermTab = std::unordered_map<Variable::OpPSDVarI*, OpPSDRealMat>;

			OpLinExpr linexpr_; // 线性项
			PSDTermTab psdterm_; // 半定项

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
			OpULInt getSize() const; // 获取线性项与半定项数目
			OpEnv getEnv() const; // 获取环境变量(默认返回线性项中第一个变量的环境变量，其次是半定项中第一个变量的环境变量，否则为空环境变量)
			OpFloat getCoeff(Variable::OpVar var) const; // 获取变量的线性项系数
			const OpPSDRealMat& getCoeff(Variable::OpPSDVar var) const; // 获取PSD变量的PSD项系数
			OpLinExpr getLinTerm() const; // 获得线性项部分
			OpPSDExpr getPSDTerm() const; // 获得半定项部分
			OpFloat getConstant() const; // 获取常量
			void setLinTerm(OpFloat val); // 设置PSD表达式常数项
			void setLinTerm(Variable::OpVar var, OpFloat coeff); // 设置PSD表达式线性项
			void setPSDTerm(Variable::OpPSDVar var, const OpPSDRealMat& coeff); // 设置PSD表达式PSD项
			void addLinTerm(OpFloat val); // 在PSD表达式中追加线性项
			void addLinTerm(Variable::OpVar var, OpFloat coeff); // 在PSD表达式中追加线性项
			void addPSDTerm(Variable::OpPSDVar var, const OpPSDRealMat& coeff); // 在PSD表达式中追加PSD项
			void subPSDTerm(Variable::OpPSDVar var, const OpPSDRealMat& coeff); // 在PSD表达式中追减PSD项
			void removeLinTerm(Variable::OpVar var); // 从PSD表达式中移除线性项
			void removePSDTerm(Variable::OpPSDVar var); // 从PSD表达式中移除PSD项
			void clear(); // 清除PSD表达式
			void simplify(OpFloat zero = Constant::AbsEpsilon); // 表达式化简
			void piecewiseProd(OpFloat val); // 逐位运算-乘(相当于expr *= val)
			void piecewiseDiv(OpFloat val); // 逐位运算-除(相当于expr /= val)
			void piecewiseInv(); // 逐位运算-取负(相当于expr = -expr)
		public:
			OpPSDExpr& operator+=(const OpPSDExpr& expr);
			OpPSDExpr& operator-=(const OpPSDExpr& expr);
			OpPSDExpr& operator*=(OpFloat val);
			OpPSDExpr& operator/=(OpFloat val);
		public:
			class OpPSDEIter // PSD迭代器
			{
			private:
				using PSDTerm = PSDTermTab::const_iterator;

				PSDTerm iter_;

				friend class OpPSDExpr;
			public:
				OpBool operator==(const OpPSDEIter& iter) const; // 判等运算
				OpBool operator!=(const OpPSDEIter& iter) const; // 判非运算
				OpPSDEIter& operator--(); // 前缀自减运算
				OpPSDEIter& operator++(); // 前缀自加运算
				Variable::OpPSDVar getVar() const; // 获取当前PSD项的变量
				const OpPSDRealMat& getCoeff() const; // 获取当前PSD项的系数
			protected:
				OpPSDEIter(PSDTerm iter);
			};

			OpLinExpr::OpLEIter getLBegin() const; // 得到线性项初始迭代器
			OpLinExpr::OpLEIter getLEnd() const; // 得到线性项尾后迭代器
			OpPSDEIter getPSDBegin() const; // 得到PSD项初始迭代器
			OpPSDEIter getPSDEnd() const; // 得到PSD项尾后迭代器
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

			OpNLFunc func_; // 非线性函数
			OpFloat param_; // 非线性函数参数(暂时只允许单个参数)
			NLTermTab nlterm_; // 非线性项(变量表，有序)
		public:
			OpULInt getSize() const; // 获取非线性项数目
			OpEnv getEnv() const; // 获取环境变量(默认返回非线性项中第一个变量的环境变量，否则为空环境变量)
			OpNLFunc getFunction() const;  // 获取非线性函数
			OpFloat getParam() const; // 获取非线性函数参数
			Variable::OpVar getVar(OpULInt order) const; // 获取序号为order的变量
			void setFunction(OpNLFunc func); // 设置非线性函数		
			void setParam(OpFloat param); // 设置非线性函数参数
			void addVar(Variable::OpVar var);  // 向变量表中追加一个变量
			void addVar(Variable::OpVarArr vars); // 向变量表中追加一组变量
			void removeVar(Variable::OpVar var);  // 从变量表中移除一个变量
			void removeVar(Variable::OpVarArr vars);  // 从变量表中移除一组变量
			void removeVar(OpULInt n = 0); // 从变量表中移除末尾的多个变量
			void clear(); // 清除非线性表达式
		public:
			class OpNLEIter
			{
			private:
				using NLTerm = NLTermTab::const_iterator;

				NLTerm iter_;

				friend class OpNLExpr;
			public:
				OpBool operator==(const OpNLEIter& iter) const; // 判等运算
				OpBool operator!=(const OpNLEIter& iter) const; // 判非运算
				OpNLEIter& operator--(); // 前缀自减运算
				OpNLEIter& operator++(); // 前缀自加运算
				Variable::OpVar getVar() const; // 获取当前变量
			protected:
				OpNLEIter(NLTerm iter);
			};

			OpNLExpr::OpNLEIter getNLBegin() const; // 得到非线性项初始迭代器
			OpNLExpr::OpNLEIter getNLEnd() const; // 得到非线性项尾后迭代器
		public:
			OpNLExpr(OpNLFunc func = OpNLFunc::Unknow);
			OpNLExpr(OpNLFunc func, Variable::OpVarArr vars);
			OpNLExpr(OpNLFunc func, OpFloat param, Variable::OpVarArr vars);
		};
	}
}
