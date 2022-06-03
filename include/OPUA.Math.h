#pragma once
#include "OPUA.Container.h"
#include "OPUA.Constant.h"

#ifndef OPUA_COMPILE_EIGEN
#define OPUA_COMPILE_EIGEN
#endif

namespace OPUA
{
	namespace Math
	{
		OpFloat OpMax(OpFloat val1, OpFloat val2); // �ϴ�ֵ
		OpFloat OpMax(Container::OpFloatArr vals); // �ϴ�ֵ
		OpFloat OpMin(OpFloat val1, OpFloat val2); // ��Сֵ
		OpFloat OpMin(Container::OpFloatArr vals); // �ϴ�ֵ
		OpFloat OpSum(Container::OpFloatArr vals); // ���

		/*
			�㷨���һԪ���κ����ֶ����Ի����(������ʽ)
			��ѧ��ʽ��
				ԭ������		of(x) = a*x^2 + b*x + c
				��Ϻ�����	af(x) = min{k*x + b}
			����˵����
				xmin / OpFloat / - / [-inf, inf] / x������
				xmax / OpFloat / - / [-inf, inf] / x������
				a / OpFloat / - / [-inf, inf] / ���κ���������ϵ��
				b / OpFloat / - / [-inf, inf] / ���κ���һ����ϵ��
				c / OpFloat / - / [-inf, inf] / ���κ���������ϵ��
				segment / OpULInt / - / [1, inf] / ��Ϸֶ���
			�ر�˵����
				1. ƽ���ֶΣ����ø�����ʽ
				2. �з���ֵ�汾�ķ���ֵ���޷���ֵ�汾��������е�param�����Ǳ�ƽ�������������з�ʽΪ[ [k1, b1], [k2, b2], ... , [kn, bn] ]
				3. �޷���ֵ�İ汾�����param���������µ�����
		*/
		void OpMathQPWLF(Container::OpFloatArr param, OpFloat xmin, OpFloat xmax, OpFloat a, OpFloat b, OpFloat c, OpULInt segment);
		Container::OpFloatArr OpMathQPWLF(OpEnv env, OpFloat xmin, OpFloat xmax, OpFloat a, OpFloat b, OpFloat c, OpULInt segment);

#ifdef OPUA_COMPILE_EIGEN
		/*
			�㷨��𣺶�Ԫһ��͹�����ֶ����Ի����
			��ѧ��ʽ��
				ԭ������		of(X)
				��Ϻ�����	af(X) = max{AX + B}
			����˵����
				X / OpFloatArr / - / - / ��ϵ�X
				Y / OpFloatArr / - / - / ��ϵ�ֵof(X)
				numPoint / OpULInt / - / [1, inf] / ��ϵ�����
				lenPoint / OpULInt / - / [1, inf] / ��ϵ㳤��
				lenPoint / OpULInt / - / [1, inf] / ��ϵ�ֶ���
				segment / OpULInt / - / [1, inf] / ��Ϸֶ���
				iterMax / OpULInt / - / [1, inf] / PWL����������
				randInit / OpULInt / - / [1, inf] / PWL�Ƿ���������ʼ��(true�������ʼ�� / false��ʹ��K��ֵ�����ʼ��)(��ʱ��Ч)
			�ر�˵����
				1. ��ƽ������ļ���ο����ף�Magnani A, Boyd S P. Convex piecewise-linear fitting[J]. Optimization and Engineering, 2009, 10(1): 1-17.
				2. �з���ֵ�汾�ķ���ֵ���޷���ֵ�汾��������е�param�����Ǳ�ƽ�������������з�ʽΪ[ [C1], [C2], ... , [Cn] ]
				3. �޷���ֵ�İ汾�����param���������µ�����
		*/
		void  OpMathMCPWLF(Container::OpFloatArr param, Container::OpFloatArr X, Container::OpFloatArr Y,
			OpULInt numPoint, OpULInt lenPoint, OpULInt segment, OpULInt iterMax, OpBool randInit);
		Container::OpFloatArr OpMathMCPWLF(OpEnv env, Container::OpFloatArr X, Container::OpFloatArr Y,
			OpULInt numPoint, OpULInt lenPoint, OpULInt segment, OpULInt iterMax, OpBool randInit);
#endif
	}
}