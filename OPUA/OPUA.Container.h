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
		template<typename T> class OpArrCIter;
		template<typename T> class OpArrIter;
		template<typename T> class OpArray;
		template<typename TK, typename TV> class OpDictI;
		template<typename TK, typename TV> class OpDictCIter;
		template<typename TK, typename TV> class OpDictIter;
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
			friend class OpArrCIter<T>;
			friend class OpArrIter<T>;
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
			T& getElement(OpULInt i)
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

		template<typename T>
		class OpArrCIter // 常量迭代器
		{
		private:
			using Val = typename OpArrayI<T>::ValTab::const_iterator;

			Val iter_;

			friend class OpArray<T>;
		public:
			OpBool operator==(const OpArrCIter<T>& iter) const { return iter_ == iter.iter_; }
			OpBool operator!=(const OpArrCIter<T>& iter) const { return iter_ != iter.iter_; }
			OpArrCIter<T>& operator--() { --iter_; return *this; }
			OpArrCIter<T>& operator++() { ++iter_; return *this; }
			const T& getVal() const { return *iter_; }
		protected:
			OpArrCIter(Val iter)
				: iter_(iter) {}
		};

		template<typename T>
		class OpArrIter // 普通迭代器
		{
		private:
			using Val = typename OpArrayI<T>::ValTab::iterator;

			Val iter_;

			friend class OpArray<T>;
		public:
			OpBool operator==(const OpArrIter<T>& iter) const { return iter_ == iter.iter_; }
			OpBool operator!=(const OpArrIter<T>& iter) const { return iter_ != iter.iter_; }
			OpArrIter<T>& operator--() { --iter_; return *this; }
			OpArrIter<T>& operator++() { ++iter_; return *this; }
			T& getVal() { return *iter_; }
		protected:
			OpArrIter(Val iter)
				: iter_(iter) {}
		};

		// OpArray：OPUA动态数组模板
		// 没有做const和ref类型去除，因此最好T不要带const和ref
		template<typename T>
		class OpArray
		{
		protected:
			OpArrayI<T>* impl_;

		public:
			OpULInt getSize() const { return impl_->getSize(); } // 获取当前数组大小
			void add(OpArray arr) { impl_->add(arr->getImpl()); } // 插入元素
			template<class... ArgTy>
			void add(ArgTy&&... arg) { impl_->add(std::forward<ArgTy>(arg)...); } // 加入元素
			template<class... ArgTy>
			void add(OpULInt n, ArgTy&&... arg) { impl_->add(n, arg); } // 加入元素	
			OpULInt find(const T& e)  const { return impl_->find(e); } // 查找元素
			void remove(const T& e) { impl_->remove(e); } // 移除元素
			void remove(OpULInt from, OpULInt to) { impl_->remove(from, to); } // 移除元素
			void clear() { impl_->clear(); } // 清空数组(不释放内存)
			void release() { if (impl_) impl_->release(), impl_ = nullptr; } // 释放数组(手动释放)
			void releaseElements() { impl_->releaseElements(); } // 释放数组中所有元素			
			OpArrayI<T>* getImpl() const { return impl_; } // 获取impl
			OpEnv getEnv() const { return impl_ ? OpEnv(impl_->getEnv()), OpEnv(nullptr); } // 获取环境变量
		public:
			OpArrCIter<T> getCBegin() const { return impl_->elements_.cbegin(); } // 得到常量初始迭代器
			OpArrCIter<T> getCEnd() const { return impl_->elements_.cend(); }; // 得到常量尾后迭代器
			OpArrIter<T> getBegin() { return impl_->elements_.begin(); }; // 得到初始迭代器
			OpArrIter<T> getEnd() { return impl_->elements_.end(); }; // 得到尾后迭代器
		public:
			OpBool operator==(const OpArray<T>& arr) { return impl_ == arr.getImpl(); } /*判断是否为同一数组*/
			OpBool operator!=(const OpArray<T>& arr) { return impl_ != arr.getImpl(); } /*判断是否非同一数组*/
			T& operator[](OpULInt i) { return impl_->getElement(i); } // 访问元素
		public:
			OpArray()
				: impl_(nullptr) {}
			OpArray(OpArrayI<T>* impl)
				: impl_(impl) {}
			OpArray(OpEnv env)
				: impl_(new OpArrayI<T>(env.getImpl())) {}
			template<class... ArgTy>
			OpArray(OpEnv env, OpULInt n, ArgTy&&... arg)
				: impl_(new OpArrayI<T>(env.getImpl(), n, std::forward<ArgTy>(arg)...)) {}
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
			friend class OpDictCIter<TK, TV>;
			friend class OpDictIter<TK, TV>;
		protected:
			OpULInt getSize() const { return size_; }
			TV& getElement(const TK& key) { return elements_[key]; }
			OpBool has(const TK& key) { return elements_.find(key) != elements_.end(); }
			template<class... ArgTy>
			void add(const TK& key, ArgTy&&... arg) { elements_.try_emplace(key, std::forward<ArgTy>(arg)...), size_ = elements_.size(); }
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

		template<typename TK, typename TV>
		class OpDictCIter // 常量迭代器
		{
		private:
			using CKeyValPair = typename OpDictI<TK, TV>::KeyValPairTab::const_iterator;

			CKeyValPair iter_;

			friend class OpDict<TK, TV>;
		public:
			OpBool operator==(const OpDictCIter<TK, TV>& iter) const { return iter_ == iter.iter_; }
			OpBool operator!=(const OpDictCIter<TK, TV>& iter) const { return iter_ != iter.iter_; }
			OpDictCIter<TK, TV>& operator--() { --iter_; return *this; }
			OpDictCIter<TK, TV>& operator++() { ++iter_; return *this; }
			const TV& getVal() const { return iter_->second; }
			const TK& getKey() const { return iter_->first; }
		protected:
			OpDictCIter(CKeyValPair iter)
				: iter_(iter) {}
		};

		template<typename TK, typename TV>
		class OpDictIter // 普通迭代器
		{
		private:
			using KeyValPair = typename OpDictI<TK, TV>::KeyValPairTab::iterator;

			KeyValPair iter_;

			friend class OpDict<TK, TV>;
		public:
			OpBool operator==(const OpDictIter<TK, TV>& iter) const { return iter_ == iter.iter_; }
			OpBool operator!=(const OpDictIter<TK, TV>& iter) const { return iter_ != iter.iter_; }
			OpDictIter<TK, TV>& operator--() { --iter_; return *this; }
			OpDictIter<TK, TV>& operator++() { ++iter_; return *this; }
			TV& getVal() { return iter_->second; }
			const TK& getKey() const { return iter_->first; }
		protected:
			OpDictIter(KeyValPair iter)
				: iter_(iter) {}
		};

		// OpDict：OPUA的字典类
		template<typename TK, typename TV> 
		class OpDict
		{
		protected:
			OpDictI<TK, TV>* impl_;
		public:
			OpULInt getSize() const { return impl_->getSize(); } // 获取字典大小
			OpBool has(const TK& key) { return impl_->has(key); } // 是否存在元素
			template<class... ArgTy>
			void add(const TK& key, ArgTy&&... arg) { impl_->add(key, std::forward<ArgTy>(arg)...); } // 添加元素
			void remove(const TK& key) { impl_->remove(key); } // 移除元素
			void clear() { impl_->clear(); } // 清空字典
			void release() { if (impl_) impl_->release(), impl_ = nullptr; } // 释放字典
			void releaseElements() { impl_->releaseElements(); } // 释放字典中元素(需要元素自定义release)
			OpDictI<TK, TV>* getImpl() const { return impl_; } // 获取impl
			OpEnv getEnv() const { return impl_ ? OpEnv(impl_->getEnv()), OpEnv(nullptr); }
		public:
			OpDictCIter<TK, TV> getCBegin() const { return impl_->elements_.cbegin(); } // 得到常量初始迭代器
			OpDictCIter<TK, TV> getCEnd() const { return impl_->elements_.cend(); }; // 得到常量尾后迭代器
			OpDictIter<TK, TV> getBegin() { return impl_->elements_.begin(); }; // 得到初始迭代器
			OpDictIter<TK, TV> getEnd() { return impl_->elements_.end(); }; // 得到尾后迭代器
		public:
			OpBool operator==(const OpDict<TK, TV>& dict) { return impl_ == dict.getImpl(); } /*判断是否为同一字典*/
			OpBool operator!=(const OpDict<TK, TV>& dict) { return impl_ != dict.getImpl(); } /*判断是否非同一字典*/
			TV& operator[](const TK& key) { return impl_->getElement(key); } // 访问元素(如果不存在则原地创建)
		public:
			OpDict()
				: impl_(nullptr) {}
			OpDict(OpDictI<TK, TV>* impl)
				: impl_(impl) {}
			OpDict(OpEnv env)
				: impl_(new OpDictI<TK, TV>(env.getImpl())) {}
		};
	}
}
