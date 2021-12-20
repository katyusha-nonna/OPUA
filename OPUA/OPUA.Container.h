#pragma once
#include "OPUA.Environment.h"
#include <vector>
#include <unordered_map>
#include <memory>

namespace OPUA
{
	namespace Container
	{
		template<typename T> class OpArrayI;
		template<typename T> class OpArray;
		template<typename TK, typename TV> class OpDictI;
		template<typename TK, typename TV> class OpDict;

		typedef OpArray<OpBool> OpBoolArr;
		typedef OpArray<OpLInt> OpLIntArr;
		typedef OpArray<OpFloat> OpFloatArr;

		// OpArrayI：OpArray的Impl类
		template<typename T>
		class OpArrayI
			: public OpImplBase
		{
		protected:
			typedef typename std::vector<T> ValTab;
			OpULInt size_;
			ValTab elements_;

			friend class OpArray<T>;
		protected:
			OpULInt getSize() const { return size_; }
			void add(OpArrayI* arr) { elements_.insert(elements_.end(), arr->elements_.begin(), arr->elements_.end()), size_ = elements_.size(); }
			template<class... ArgTy>
			void add(ArgTy&&... arg) { elements_.emplace_back(std::forward<ArgTy>(arg)...), size_ = elements_.size(); }
			template<class... ArgTy>
			void add(OpULInt n, ArgTy&&... arg)
			{
				elements_.reserve(size_ + n);
				for (OpULInt i = 0; i < n; i++)
					elements_.emplace_back(std::forward<ArgTy>(arg)...);
				size_ = elements_.size();
			}
			OpULInt find(const T& e) const
			{
				auto iter = std::find(elements_.begin(), elements_.end(), e); // 对于自定义类需要重载==运算符
				return (iter == elements_.end()) ?
					size_ : iter - elements_.begin(); // 不存在返回size
			}
			OpBool idxValid(OpULInt i) const
			{
				return i < size_;
			}
			T& getElement(OpULInt i)
			{
				return elements_.at(i);
			}
			const T& getElement(OpULInt i) const
			{
				return elements_.at(i);
			}
			void remove(const T& e)
			{
				auto pos = find(e);
				if (pos < size_)
					elements_.erase(elements_.begin() + pos), size_ = elements_.size();
			}
			void remove(OpULInt from, OpULInt to)
			{
				if (from >= size_ || to >= size_ || from > to)
					return;
				elements_.erase(elements_.begin() + from, elements_.begin() + to), size_ = elements_.size();
			}
			void clear() { elements_.clear(), size_ = elements_.size(); }
			void releaseElements() // 非built-in类型才有，且需要定义release函数
			{
				for (auto& e : elements_)
					e.release();
			}
		protected:
			OpArrayI(OpEnvI* env)
				: OpImplBase('T', env), size_(0) {}
			template<typename... ArgTy>
			OpArrayI(OpEnvI* env, OpULInt n, ArgTy&&... arg)
				: OpImplBase('T', env), size_(n)
			{
				elements_.reserve(size_);
				for (OpULInt i = 0; i < size_; i++)
					elements_.emplace_back(std::forward<ArgTy>(arg)...);
			}
		public:
			virtual ~OpArrayI() {}
		};

		// OpArray：OPUA动态数组模板
		// 没有做const和ref类型去除，因此最好T不要带const和ref
		template<typename T>
		class OpArray
			: public OpBase
		{
		public:
			OpULInt getSize() const { return static_cast<OpArrayI<T>*>(impl_)->getSize(); } // 获取当前数组大小
			T& getVal(OpULInt i) { return static_cast<OpArrayI<T>*>(impl_) ->getElement(); }
			const T& getVal(OpULInt i) const { return static_cast<OpArrayI<T>*>(impl_)->getElement(); }
			void add(OpArray arr) { static_cast<OpArrayI<T>*>(impl_)->add(arr.getImpl()); } // 插入元素
			template<class... ArgTy>
			void add(ArgTy&&... arg) { static_cast<OpArrayI<T>*>(impl_)->add(std::forward<ArgTy>(arg)...); } // 加入元素
			template<class... ArgTy>
			void add(OpULInt n, ArgTy&&... arg) { static_cast<OpArrayI<T>*>(impl_)->add(n, arg); } // 加入元素	
			OpULInt find(const T& e)  const { return static_cast<OpArrayI<T>*>(impl_)->find(e); } // 查找元素
			OpBool idxValid(OpULInt i) const { return static_cast<OpArrayI<T>*>(impl_)->idxValid(i); } // 判断索引是否有效
			void remove(const T& e) { static_cast<OpArrayI<T>*>(impl_)->remove(e); } // 移除元素
			void remove(OpULInt from, OpULInt to) { static_cast<OpArrayI<T>*>(impl_)->remove(from, to); } // 移除元素
			void clear() { static_cast<OpArrayI<T>*>(impl_)->clear(); } // 清空数组(不释放内存)
			void releaseElements() { static_cast<OpArrayI<T>*>(impl_)->releaseElements(); } // 释放数组中所有元素			
			OpArrayI<T>* getImpl() const { return static_cast<OpArrayI<T>*>(impl_); } // 获取impl
		public:
			class OpArrCIter // 常量迭代器
			{
			private:
				using Val = typename OpArrayI<T>::ValTab::const_iterator;

				Val iter_;

				friend class OpArray<T>;
			public:
				OpBool operator==(const OpArrCIter& iter) const { return iter_ == iter.iter_; }
				OpBool operator!=(const OpArrCIter& iter) const { return iter_ != iter.iter_; }
				OpArrCIter& operator--() { --iter_; return *this; }
				OpArrCIter& operator++() { ++iter_; return *this; }
				const T& getVal() const { return *iter_; }
			protected:
				OpArrCIter(Val iter)
					: iter_(iter) {}
			};

			class OpArrIter // 普通迭代器
			{
			private:
				using Val = typename OpArrayI<T>::ValTab::iterator;

				Val iter_;

				friend class OpArray<T>;
			public:
				OpBool operator==(const OpArrIter& iter) const { return iter_ == iter.iter_; }
				OpBool operator!=(const OpArrIter& iter) const { return iter_ != iter.iter_; }
				OpArrIter& operator--() { --iter_; return *this; }
				OpArrIter& operator++() { ++iter_; return *this; }
				T& getVal() { return *iter_; }
			protected:
				OpArrIter(Val iter)
					: iter_(iter) {}
			};
		public:
			OpArrCIter getCBegin() const { return static_cast<OpArrayI<T>*>(impl_)->elements_.cbegin(); } // 得到常量初始迭代器
			OpArrCIter getCEnd() const { return static_cast<OpArrayI<T>*>(impl_)->elements_.cend(); }; // 得到常量尾后迭代器
			OpArrIter getBegin() { return static_cast<OpArrayI<T>*>(impl_)->elements_.begin(); }; // 得到初始迭代器
			OpArrIter getEnd() { return static_cast<OpArrayI<T>*>(impl_)->elements_.end(); }; // 得到尾后迭代器
		public:
			OpBool operator==(const OpArray<T>& arr) { return impl_ == arr.getImpl(); } /*判断是否为同一数组*/
			OpBool operator!=(const OpArray<T>& arr) { return impl_ != arr.getImpl(); } /*判断是否非同一数组*/
			T& operator[](OpULInt i) { return static_cast<OpArrayI<T>*>(impl_)->getElement(i); } // 访问元素
			const T& operator[](OpULInt i) const { return static_cast<OpArrayI<T>*>(impl_)->getElement(i); } // 访问元素
		public:
			OpArray()
			{

			}
			OpArray(OpArrayI<T>* impl)
			{
				impl_ = impl;
			}
			OpArray(OpEnv env)
			{
				impl_ = new OpArrayI<T>(env.getImpl());
			}
			template<class... ArgTy>
			OpArray(OpEnv env, OpULInt n, ArgTy&&... arg)
			{
				impl_ = new OpArrayI<T>(env.getImpl(), n, std::forward<ArgTy>(arg)...);
			}
			~OpArray()
			{

			}
		};

		// OpDictI：OpDict的Impl类
		template<typename TK, typename TV> 
		class OpDictI
			: public OpImplBase
		{
		protected:
			typedef typename std::unordered_map<TK, TV> KeyValPairTab;

			OpULInt size_;
			KeyValPairTab elements_;

			friend class OpDict<TK, TV>;
		protected:
			OpULInt getSize() const { return size_; }
			TV& getElement1(const TK& key) { return elements_[key]; }
			TV& getElement2(const TK& key) { return elements_.at(key); }
			const TV& getElement2(const TK& key) const { return elements_.at(key); }
			OpBool has(const TK& key) const { return elements_.find(key) != elements_.end(); }
			template<class... ArgTy>
			void add(const TK& key, ArgTy&&... arg) { elements_.emplace(key, std::forward<ArgTy>(arg)...), size_ = elements_.size(); }
			template<class... ArgTy>
			void tryAdd(const TK& key, ArgTy&&... arg) { elements_.try_emplace(key, std::forward<ArgTy>(arg)...), size_ = elements_.size(); }
			void remove(const TK& key) { elements_.erase(key), size_ = elements_.size(); }
			void clear() { elements_.clear(), size_ = elements_.size(); }
			void releaseElements() // 非built-in类型才有，且需要定义release函数
			{
				for (auto& e : elements_)
					e.second.release();
			}
		protected:
			OpDictI(OpEnvI* env)
				: OpImplBase('T', env), size_(0) {}
		public:
			virtual ~OpDictI() {}
		};

		// OpDict：OPUA的字典类
		template<typename TK, typename TV> 
		class OpDict
			: public OpBase
		{
		public:
			OpULInt getSize() const { return static_cast<OpDictI<TK, TV>*>(impl_)->getSize(); } // 获取字典大小
			TV& getVal(const TK& key) { return static_cast<OpDictI<TK, TV>*>(impl_)->getElement2(key); } // 访问元素(必须已存在)
			const TV& getVal(const TK& key) const { return static_cast<OpDictI<TK, TV>*>(impl_)->getElement2(key); } // 访问元素(必须已存在)
			OpBool has(const TK& key) const{ return static_cast<OpDictI<TK, TV>*>(impl_)->has(key); } // 是否存在元素
			template<class... ArgTy>
			void add(const TK& key, ArgTy&&... arg) { static_cast<OpDictI<TK, TV>*>(impl_)->add(key, std::forward<ArgTy>(arg)...); } // 添加元素(如果存在则覆盖)
			template<class... ArgTy>
			void tryAdd(const TK& key, ArgTy&&... arg) { static_cast<OpDictI<TK, TV>*>(impl_)->tryAdd(key, std::forward<ArgTy>(arg)...); } // 尝试添加元素(如果存在不会覆盖)
			void remove(const TK& key) { static_cast<OpDictI<TK, TV>*>(impl_)->remove(key); } // 移除元素
			void clear() { static_cast<OpDictI<TK, TV>*>(impl_)->clear(); } // 清空字典
			void releaseElements() { static_cast<OpDictI<TK, TV>*>(impl_)->releaseElements(); } // 释放字典中元素(需要元素自定义release)
			OpDictI<TK, TV>* getImpl() const { return static_cast<OpDictI<TK, TV>*>(impl_); } // 获取impl
		public:
			class OpDictCIter // 常量迭代器
			{
			private:
				using CKeyValPair = typename OpDictI<TK, TV>::KeyValPairTab::const_iterator;

				CKeyValPair iter_;

				friend class OpDict<TK, TV>;
			public:
				OpBool operator==(const OpDictCIter& iter) const { return iter_ == iter.iter_; }
				OpBool operator!=(const OpDictCIter& iter) const { return iter_ != iter.iter_; }
				OpDictCIter& operator--() { --iter_; return *this; }
				OpDictCIter& operator++() { ++iter_; return *this; }
				const TV& getVal() const { return iter_->second; }
				const TK& getKey() const { return iter_->first; }
			protected:
				OpDictCIter(CKeyValPair iter)
					: iter_(iter) {}
			};

			class OpDictIter // 普通迭代器
			{
			private:
				using KeyValPair = typename OpDictI<TK, TV>::KeyValPairTab::iterator;

				KeyValPair iter_;

				friend class OpDict<TK, TV>;
			public:
				OpBool operator==(const OpDictIter& iter) const { return iter_ == iter.iter_; }
				OpBool operator!=(const OpDictIter& iter) const { return iter_ != iter.iter_; }
				OpDictIter& operator--() { --iter_; return *this; }
				OpDictIter& operator++() { ++iter_; return *this; }
				TV& getVal() { return iter_->second; }
				const TK& getKey() const { return iter_->first; }
			protected:
				OpDictIter(KeyValPair iter)
					: iter_(iter) {}
			};
		public:
			OpDictCIter getCBegin() const { return static_cast<OpDictI<TK, TV>*>(impl_)->elements_.cbegin(); } // 得到常量初始迭代器
			OpDictCIter getCEnd() const { return static_cast<OpDictI<TK, TV>*>(impl_)->elements_.cend(); }; // 得到常量尾后迭代器
			OpDictIter getBegin() { return static_cast<OpDictI<TK, TV>*>(impl_)->elements_.begin(); }; // 得到初始迭代器
			OpDictIter getEnd() { return static_cast<OpDictI<TK, TV>*>(impl_)->elements_.end(); }; // 得到尾后迭代器
		public:
			OpBool operator==(const OpDict<TK, TV>& dict) { return impl_ == dict.getImpl(); } /*判断是否为同一字典*/
			OpBool operator!=(const OpDict<TK, TV>& dict) { return impl_ != dict.getImpl(); } /*判断是否非同一字典*/
			TV& operator[](const TK& key) { return static_cast<OpDictI<TK, TV>*>(impl_)->getElement1(key); } // 访问元素(如果不存在则原地创建)
		public:
			OpDict()
			{

			}
			OpDict(OpDictI<TK, TV>* impl)
			{
				impl_ = impl;
			}
			OpDict(OpEnv env)
			{
				impl_ = new OpDictI<TK, TV>(env.getImpl());
			}
			~OpDict()
			{

			}
		};
	}
}
