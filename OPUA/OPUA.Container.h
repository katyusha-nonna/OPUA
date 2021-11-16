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
				auto iter = std::find(elements_.begin(), elements_.end(), e); // �����Զ�������Ҫ����==�����
				return (iter == elements_.end()) ?
					size_ : iter - elements_.begin(); // �����ڷ���size
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

		template<typename T>
		class OpArrCIter // ����������
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
		class OpArrIter // ��ͨ������
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

		// OpArray��OPUA��̬����ģ��
		// û����const��ref����ȥ����������T��Ҫ��const��ref
		template<typename T>
		class OpArray
		{
		protected:
			OpArrayI<T>* impl_;

		public:
			OpULInt getSize() const { return impl_->getSize(); } // ��ȡ��ǰ�����С
			void add(OpArray arr) { impl_->add(arr->getImpl()); } // ����Ԫ��
			template<class... ArgTy>
			void add(ArgTy&&... arg) { impl_->add(std::forward<ArgTy>(arg)...); } // ����Ԫ��
			template<class... ArgTy>
			void add(OpULInt n, ArgTy&&... arg) { impl_->add(n, arg); } // ����Ԫ��	
			OpULInt find(const T& e)  const { return impl_->find(e); } // ����Ԫ��
			void remove(const T& e) { impl_->remove(e); } // �Ƴ�Ԫ��
			void remove(OpULInt from, OpULInt to) { impl_->remove(from, to); } // �Ƴ�Ԫ��
			void clear() { impl_->clear(); } // �������(���ͷ��ڴ�)
			void release() { if (impl_) impl_->release(), impl_ = nullptr; } // �ͷ�����(�ֶ��ͷ�)
			void releaseElements() { impl_->releaseElements(); } // �ͷ�����������Ԫ��			
			OpArrayI<T>* getImpl() const { return impl_; } // ��ȡimpl
			OpEnv getEnv() const { return impl_ ? OpEnv(impl_->getEnv()), OpEnv(nullptr); } // ��ȡ��������
		public:
			OpArrCIter<T> getCBegin() const { return impl_->elements_.cbegin(); } // �õ�������ʼ������
			OpArrCIter<T> getCEnd() const { return impl_->elements_.cend(); }; // �õ�����β�������
			OpArrIter<T> getBegin() { return impl_->elements_.begin(); }; // �õ���ʼ������
			OpArrIter<T> getEnd() { return impl_->elements_.end(); }; // �õ�β�������
		public:
			OpBool operator==(const OpArray<T>& arr) { return impl_ == arr.getImpl(); } /*�ж��Ƿ�Ϊͬһ����*/
			OpBool operator!=(const OpArray<T>& arr) { return impl_ != arr.getImpl(); } /*�ж��Ƿ��ͬһ����*/
			T& operator[](OpULInt i) { return impl_->getElement(i); } // ����Ԫ��
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

		template<typename TK, typename TV>
		class OpDictCIter // ����������
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
		class OpDictIter // ��ͨ������
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

		// OpDict��OPUA���ֵ���
		template<typename TK, typename TV> 
		class OpDict
		{
		protected:
			OpDictI<TK, TV>* impl_;
		public:
			OpULInt getSize() const { return impl_->getSize(); } // ��ȡ�ֵ��С
			OpBool has(const TK& key) { return impl_->has(key); } // �Ƿ����Ԫ��
			template<class... ArgTy>
			void add(const TK& key, ArgTy&&... arg) { impl_->add(key, std::forward<ArgTy>(arg)...); } // ���Ԫ��
			void remove(const TK& key) { impl_->remove(key); } // �Ƴ�Ԫ��
			void clear() { impl_->clear(); } // ����ֵ�
			void release() { if (impl_) impl_->release(), impl_ = nullptr; } // �ͷ��ֵ�
			void releaseElements() { impl_->releaseElements(); } // �ͷ��ֵ���Ԫ��(��ҪԪ���Զ���release)
			OpDictI<TK, TV>* getImpl() const { return impl_; } // ��ȡimpl
			OpEnv getEnv() const { return impl_ ? OpEnv(impl_->getEnv()), OpEnv(nullptr); }
		public:
			OpDictCIter<TK, TV> getCBegin() const { return impl_->elements_.cbegin(); } // �õ�������ʼ������
			OpDictCIter<TK, TV> getCEnd() const { return impl_->elements_.cend(); }; // �õ�����β�������
			OpDictIter<TK, TV> getBegin() { return impl_->elements_.begin(); }; // �õ���ʼ������
			OpDictIter<TK, TV> getEnd() { return impl_->elements_.end(); }; // �õ�β�������
		public:
			OpBool operator==(const OpDict<TK, TV>& dict) { return impl_ == dict.getImpl(); } /*�ж��Ƿ�Ϊͬһ�ֵ�*/
			OpBool operator!=(const OpDict<TK, TV>& dict) { return impl_ != dict.getImpl(); } /*�ж��Ƿ��ͬһ�ֵ�*/
			TV& operator[](const TK& key) { return impl_->getElement(key); } // ����Ԫ��(�����������ԭ�ش���)
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
