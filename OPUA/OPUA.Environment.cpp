#include "OPUA.Environment.h"
#include <algorithm>
#include <unordered_map>
#include <set>
#include <iostream>

/* OPUA::OpEnvI */

class OPUA::OpEnvI
{
protected:
	using IdxTab = std::unordered_map<OpChar, OpLInt>;
	using CpTab = std::unordered_map<OpChar, std::set<OpImplBase*> >;
	using CpIter = std::pair<CpTab::mapped_type::iterator, bool>;

	IdxTab counts_; // �������
	CpTab impls_; // �����

	OpStr name_; // ��������

	friend class OpEnv;
	friend class OpImplBase;
protected:
	void release(); // �ͷ�����ģ�����
	void release(OpImplBase* impl); // �ͷ�һ��ģ�����
	OpLInt getNewIndex(OpChar cls); // ��ȡһ���µ��������
	OpULInt getIndexNum(); // ��ȡ�������
	OpULInt getIndexNum(OpChar cls); // ��ȡһ���������
	CpIter addManagement(OpImplBase* impl); // ����ڴ����
	CpIter removeManagement(OpImplBase* impl); // ɾ���ڴ����
public:
	OpEnvI(OpStr name);
};

void OPUA::OpEnvI::release()
{
	// ע���ֹ�ͷŹ����У���impls_�ĸı���ɵ�����ʧЧ
	// ʹ��impls_�Ŀ���������
	auto cy(impls_);
	for (auto& cls : cy)
		for (auto& impl : cls.second)
			release(impl);
	// ��������
	impls_.clear();
}

void OPUA::OpEnvI::release(OpImplBase* impl)
{
	if (removeManagement(impl).second) // ��ֹ����ɾ��
		delete impl;	
}

OPUA::OpLInt OPUA::OpEnvI::getNewIndex(OpChar cls)
{
	return counts_[cls]++;
}

OPUA::OpULInt OPUA::OpEnvI::getIndexNum()
{
	OpULInt num(0);
	for (auto& cls : impls_)
		num += cls.second.size();
	return num;
}

OPUA::OpULInt OPUA::OpEnvI::getIndexNum(OpChar cls)
{
	auto iter = impls_.find(cls);
	if (iter != impls_.end())
		return iter->second.size();
	else
		return 0;
}

OPUA::OpEnvI::CpIter OPUA::OpEnvI::addManagement(OpImplBase* impl)
{
	return impls_[impl->class_].emplace(impl);
}

OPUA::OpEnvI::CpIter OPUA::OpEnvI::removeManagement(OpImplBase* impl)
{
	auto& cls = impls_.at(impl->class_);
	auto iter = cls.find(impl);
	if (iter != cls.end())
		return CpIter(cls.erase(iter), true);
	else
		return CpIter(iter, false);
}

OPUA::OpEnvI::OpEnvI(OpStr name)
	: name_(name)
{

}

/* OPUA::OpEnv */

void OPUA::OpEnv::release()
{
	if (impl_)
	{
		impl_->release();
		delete impl_;
		impl_ = nullptr;
	}
}

void OPUA::OpEnv::init(OpStr name)
{
	if (impl_)
		release();
	impl_ = new OpEnvI(name);
}

OPUA::OpEnvI* OPUA::OpEnv::getImpl()
{
	return impl_;
}

OPUA::OpEnv::OpEnv()
	: impl_(nullptr)
{

}

OPUA::OpEnv::OpEnv(OpEnvI* impl)
	: impl_(impl)
{

}

OPUA::OpEnv::OpEnv(OpBool init, OpStr name)
	: impl_(init ? new OpEnvI(name) : nullptr)
{

}

/* OPUA::OpImplBase */

void OPUA::OpImplBase::lock()
{
	locked_ = true;
}

void OPUA::OpImplBase::unlock()
{
	locked_ = false;
}

OPUA::OpBool OPUA::OpImplBase::isLocked() const
{
	return locked_;
}

void OPUA::OpImplBase::release()
{
	env_->release(this); // ��env������Դ�ͷ�
}

OPUA::OpChar OPUA::OpImplBase::getClass() const
{
	return class_;
}

OPUA::OpLInt OPUA::OpImplBase::getIndex() const
{
	return idx_;
}

OPUA::OpEnvI* OPUA::OpImplBase::getEnv() const
{
	return env_;
}

OPUA::OpImplBase::~OpImplBase()
{
	// �ͷ���Դ
}

OPUA::OpImplBase::OpImplBase(OpChar cls, OpEnvI* env)
	: locked_(false), class_(cls), idx_(env->getNewIndex(cls)), env_(env)
{
	env_->addManagement(this);
}

/* OPUA::OpBase:: */

OPUA::OpBool OPUA::OpBase::isLocked() const
{
	return impl_->isLocked();
}

OPUA::OpLInt OPUA::OpBase::getIndex() const
{
	return impl_->getIndex();
}

OPUA::OpEnv OPUA::OpBase::getEnv() const
{
	return impl_ ? OpEnv(impl_->getEnv()) : OpEnv(nullptr);
}

void OPUA::OpBase::release()
{
	if (impl_)
	{
		impl_->release();
		impl_ = nullptr;
	}
}

void OPUA::OpBase::lock()
{
	impl_->lock();
}

void OPUA::OpBase::unlock()
{
	impl_->unlock();
}

OPUA::OpBase::OpBase()
	: impl_(nullptr)
{

}

OPUA::OpBase::~OpBase()
{

}
