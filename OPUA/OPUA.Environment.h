#pragma once

#include "OPUA.Type.h"

#define implbase_cast(impl) (OpImplBase*)(impl)

namespace OPUA
{
	class OpEnvI;
	class OpEnv;
	class OpImplBase;

	// OpEnv��OPUA��������
	// ��������������ڴ����
	class OpEnv
	{
	protected:
		OpEnvI* impl_;
	public:
		OpULInt addManagement(OpImplBase* impl); // ����ڴ����(���������ü���)
		OpULInt removeManagement(OpImplBase* impl); // �Ƴ��ڴ����(���������ü��������ü������������Ƴ�����)
		OpULInt getMemoryUsage(); // ��ȡ�ڴ�ʹ�����
		void release(); // �ͷ�����ģ�����(����Ҫ�ֶ�ִ��)
		void init(OpStr name); // ��ʼ��(��ʼ��Ϊ�����Զ�ִ���ͷ�)
		OpEnvI* getImpl() const; // ��ȡImpl
	public:
		OpBool operator==(const OpEnv& env) const;
		OpBool operator!=(const OpEnv& env) const;
	public:
		OpEnv(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
		OpEnv(OpEnvI* impl); // ��Impl����
		OpEnv(OpBool init, OpStr name); // ��ʼ������
	};

	// OpImplBase������Impl�Ļ���
	// ԭ������˵����ʼ��ʱ��Impl��������ɳ�ʼ��������Env�ӹܣ��ͷ�ʱ��Impl����Env�ύ�ͷ�������Env�ͷ�����
	// �������Ķ��󲿷ֵ��޸Ĺ������ޣ���ϸ���ݲο������ģ����˵��
	class OpImplBase
	{
	protected:
		/*
		������:
			V / ����
			E / ���ʽ(���ʽ��ʱ�������й�)
			C / Լ��
			O / Ŀ�꺯��
			M / ģ��
			S / �����
			T / ����
			U / δ֪
		*/
		OpBool locked_; // �Ƿ�����
		OpChar class_; // ������
		OpLInt idx_; // �������

		OpEnvI* env_; // ��������

		friend class OpEnvI;
		friend class OpBase;
	protected:
		void lock(); // �������
		void unlock(); // �������
		OpBool isLocked() const; // �Ƿ�����
		void release(); // �ͷ��������(�ֶ��ͷ�)
		OpChar getClass() const; // ��ȡ������
		OpLInt getIndex() const; // ��ȡ�������
		OpEnvI* getEnv() const; // ��ȡ��������
	protected:
		OpImplBase(OpChar cls, OpEnvI* env);
	public:
		virtual ~OpImplBase(); // �ͷ��������(�����ͷ�)
	};

	// ����Op����Ļ���(���ʽ����)
	class OpBase
	{
	protected:
		OpImplBase* impl_;
	public:
		OpBool isLocked() const; // �Ƿ�����
		OpLInt getIndex() const; // ��ȡ����
		OpEnv getEnv() const; // ��ȡ��������
		void release(); // �ͷű���(�ֶ��ͷ�)
		void lock(); // �������
		void unlock(); // �������
	public:
		OpBase();
	public:
		virtual ~OpBase();
	};
}
