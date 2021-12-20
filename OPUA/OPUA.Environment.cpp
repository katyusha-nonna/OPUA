#include "OPUA.Environment.h"
#include <algorithm>
#include <unordered_map>
#include <iostream>

/* OPUA::OpEnvI */

class OPUA::OpEnvI
{
protected:
	using IdxTab = std::unordered_map<OpChar, OpLInt>;
	using CpTab = std::unordered_map<OpImplBase*, OpULInt>;

	IdxTab counts_; // 组件索引
	CpTab impls_; // 组件表(引用计数管理)

	OpStr name_; // 环境名称

	friend class OpEnv;
	friend class OpImplBase;
protected:
	void release(); // 释放所有模型组件
	void tryRelease(OpImplBase* impl); // 尝试释放一个模型组件
	OpLInt getNewIndex(OpChar cls); // 获取一个新的组件索引
	OpULInt getIndexNum(); // 获取组件总数
	OpULInt getIndexNum(OpChar cls); // 获取一类组件总数
	OpULInt addManagement(OpImplBase* impl); // 添加内存管理
	OpULInt removeManagement(OpImplBase* impl); // 删除内存管理
	OpULInt getMemoryUsage(); // 获取内存使用情况
public:
	OpEnvI(OpStr name);
};

void OPUA::OpEnvI::release()
{
	// 注意防止释放过程中，对impls_的改变造成迭代器失效
	// 使用impls_的拷贝来迭代
	while (impls_.size())
	{
		auto cy(impls_);
		for (const auto& impl : cy)
			tryRelease(impl.first);
	}
}

void OPUA::OpEnvI::tryRelease(OpImplBase* impl)
{
	if (!removeManagement(impl))
		delete impl;
}

OPUA::OpLInt OPUA::OpEnvI::getNewIndex(OpChar cls)
{
	return counts_[cls]++;
}

OPUA::OpULInt OPUA::OpEnvI::getIndexNum()
{
	return impls_.size();
}

OPUA::OpULInt OPUA::OpEnvI::getIndexNum(OpChar cls)
{
	OpULInt count(0);
	for (const auto& impl : impls_)
		if (impl.first->class_ == cls)
			count++;
	return count;
}

OPUA::OpULInt OPUA::OpEnvI::addManagement(OpImplBase* impl)
{
	return impls_[impl] += 1;
}

OPUA::OpULInt OPUA::OpEnvI::removeManagement(OpImplBase* impl)
{
	auto& refcount(impls_.at(impl));
	if (!(--refcount))
	{
		impls_.erase(impl);
		return 0;
	}
	return refcount;
}

OPUA::OpULInt OPUA::OpEnvI::getMemoryUsage()
{
	// TODO：暂时未实现
	return 0;
}

OPUA::OpEnvI::OpEnvI(OpStr name)
	: name_(name)
{

}

/* OPUA::OpEnv */

OPUA::OpULInt OPUA::OpEnv::addManagement(OpImplBase* impl)
{
	return impl_->addManagement(impl);
}

OPUA::OpULInt OPUA::OpEnv::removeManagement(OpImplBase* impl)
{
	return impl_->removeManagement(impl);
}

OPUA::OpULInt OPUA::OpEnv::getMemoryUsage()
{
	return impl_->getMemoryUsage();
}

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

OPUA::OpEnvI* OPUA::OpEnv::getImpl() const
{
	return impl_;
}

OPUA::OpBool OPUA::OpEnv::operator==(const OpEnv& env) const
{
	return impl_ == env.impl_;
}

OPUA::OpBool OPUA::OpEnv::operator!=(const OpEnv& env) const
{
	return impl_ != env.impl_;
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
	env_->tryRelease(this); // 向env申请资源释放
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
	// 释放资源
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
