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

		// OpArrayI��OpArray��Impl��
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
				auto iter = std::find(elements_.begin(), elements_.end(), e); // �����Զ�������Ҫ����==�����
				return (iter == elements_.end()) ?
					size_ : iter - elements_.begin(); // �����ڷ���size
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
			void releaseElements() // ��built-in���Ͳ��У�����Ҫ����release����
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

		// OpArray��OPUA��̬����ģ��
		// û����const��ref����ȥ����������T��Ҫ��const��ref
		template<typename T>
		class OpArray
			: public OpBase
		{
		public:
			OpULInt getSize() const { return static_cast<OpArrayI<T>*>(impl_)->getSize(); } // ��ȡ��ǰ�����С
			T& getVal(OpULInt i) { return static_cast<OpArrayI<T>*>(impl_) ->getElement(); }
			const T& getVal(OpULInt i) const { return static_cast<OpArrayI<T>*>(impl_)->getElement(); }
			void add(OpArray arr) { static_cast<OpArrayI<T>*>(impl_)->add(arr.getImpl()); } // ����Ԫ��
			template<class... ArgTy>
			void add(ArgTy&&... arg) { static_cast<OpArrayI<T>*>(impl_)->add(std::forward<ArgTy>(arg)...); } // ����Ԫ��
			template<class... ArgTy>
			void add(OpULInt n, ArgTy&&... arg) { static_cast<OpArrayI<T>*>(impl_)->add(n, arg); } // ����Ԫ��	
			OpULInt find(const T& e)  const { return static_cast<OpArrayI<T>*>(impl_)->find(e); } // ����Ԫ��
			OpBool idxValid(OpULInt i) const { return static_cast<OpArrayI<T>*>(impl_)->idxValid(i); } // �ж������Ƿ���Ч
			void remove(const T& e) { static_cast<OpArrayI<T>*>(impl_)->remove(e); } // �Ƴ�Ԫ��
			void remove(OpULInt from, OpULInt to) { static_cast<OpArrayI<T>*>(impl_)->remove(from, to); } // �Ƴ�Ԫ��
			void clear() { static_cast<OpArrayI<T>*>(impl_)->clear(); } // �������(���ͷ��ڴ�)
			void releaseElements() { static_cast<OpArrayI<T>*>(impl_)->releaseElements(); } // �ͷ�����������Ԫ��			
			OpArrayI<T>* getImpl() const { return static_cast<OpArrayI<T>*>(impl_); } // ��ȡimpl
		public:
			class OpArrCIter // ����������
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

			class OpArrIter // ��ͨ������
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
			OpArrCIter getCBegin() const { return static_cast<OpArrayI<T>*>(impl_)->elements_.cbegin(); } // �õ�������ʼ������
			OpArrCIter getCEnd() const { return static_cast<OpArrayI<T>*>(impl_)->elements_.cend(); }; // �õ�����β�������
			OpArrIter getBegin() { return static_cast<OpArrayI<T>*>(impl_)->elements_.begin(); }; // �õ���ʼ������
			OpArrIter getEnd() { return static_cast<OpArrayI<T>*>(impl_)->elements_.end(); }; // �õ�β�������
		public:
			OpBool operator==(const OpArray<T>& arr) { return impl_ == arr.getImpl(); } /*�ж��Ƿ�Ϊͬһ����*/
			OpBool operator!=(const OpArray<T>& arr) { return impl_ != arr.getImpl(); } /*�ж��Ƿ��ͬһ����*/
			T& operator[](OpULInt i) { return static_cast<OpArrayI<T>*>(impl_)->getElement(i); } // ����Ԫ��
			const T& operator[](OpULInt i) const { return static_cast<OpArrayI<T>*>(impl_)->getElement(i); } // ����Ԫ��
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

		// OpDictI��OpDict��Impl��
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
			void releaseElements() // ��built-in���Ͳ��У�����Ҫ����release����
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

		// OpDict��OPUA���ֵ���
		template<typename TK, typename TV> 
		class OpDict
			: public OpBase
		{
		public:
			OpULInt getSize() const { return static_cast<OpDictI<TK, TV>*>(impl_)->getSize(); } // ��ȡ�ֵ��С
			TV& getVal(const TK& key) { return static_cast<OpDictI<TK, TV>*>(impl_)->getElement2(key); } // ����Ԫ��(�����Ѵ���)
			const TV& getVal(const TK& key) const { return static_cast<OpDictI<TK, TV>*>(impl_)->getElement2(key); } // ����Ԫ��(�����Ѵ���)
			OpBool has(const TK& key) const{ return static_cast<OpDictI<TK, TV>*>(impl_)->has(key); } // �Ƿ����Ԫ��
			template<class... ArgTy>
			void add(const TK& key, ArgTy&&... arg) { static_cast<OpDictI<TK, TV>*>(impl_)->add(key, std::forward<ArgTy>(arg)...); } // ���Ԫ��(��������򸲸�)
			template<class... ArgTy>
			void tryAdd(const TK& key, ArgTy&&... arg) { static_cast<OpDictI<TK, TV>*>(impl_)->tryAdd(key, std::forward<ArgTy>(arg)...); } // �������Ԫ��(������ڲ��Ḳ��)
			void remove(const TK& key) { static_cast<OpDictI<TK, TV>*>(impl_)->remove(key); } // �Ƴ�Ԫ��
			void clear() { static_cast<OpDictI<TK, TV>*>(impl_)->clear(); } // ����ֵ�
			void releaseElements() { static_cast<OpDictI<TK, TV>*>(impl_)->releaseElements(); } // �ͷ��ֵ���Ԫ��(��ҪԪ���Զ���release)
			OpDictI<TK, TV>* getImpl() const { return static_cast<OpDictI<TK, TV>*>(impl_); } // ��ȡimpl
		public:
			class OpDictCIter // ����������
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

			class OpDictIter // ��ͨ������
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
			OpDictCIter getCBegin() const { return static_cast<OpDictI<TK, TV>*>(impl_)->elements_.cbegin(); } // �õ�������ʼ������
			OpDictCIter getCEnd() const { return static_cast<OpDictI<TK, TV>*>(impl_)->elements_.cend(); }; // �õ�����β�������
			OpDictIter getBegin() { return static_cast<OpDictI<TK, TV>*>(impl_)->elements_.begin(); }; // �õ���ʼ������
			OpDictIter getEnd() { return static_cast<OpDictI<TK, TV>*>(impl_)->elements_.end(); }; // �õ�β�������
		public:
			OpBool operator==(const OpDict<TK, TV>& dict) { return impl_ == dict.getImpl(); } /*�ж��Ƿ�Ϊͬһ�ֵ�*/
			OpBool operator!=(const OpDict<TK, TV>& dict) { return impl_ != dict.getImpl(); } /*�ж��Ƿ��ͬһ�ֵ�*/
			TV& operator[](const TK& key) { return static_cast<OpDictI<TK, TV>*>(impl_)->getElement1(key); } // ����Ԫ��(�����������ԭ�ش���)
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
