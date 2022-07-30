#pragma once

// OPUA����ͷ�ļ�

/*
*  
	���ܣ�
			OPUA(Optimization Program Universal API)��һ���Ż������ͨ�ýӿڹ��ߣ�ּ��ͳһ����������Ľӿڣ�
			ʹ�û�����ʹ��ͳһ�Ľӿڽ����Ż�ģ�͵���ѧ��ģ���Լ���������ѡ�����������������(������Yalmip)��
			OPUA���Ϊ�Ż��㷨��ܵĿ����ṩ֧�֡�

			�ƻ�֧�ֵ��������
				1. IBM CPLEX(��֧��)
				2. Gurobi(��֧��)
				3. Mosek(��֧��)
				4. SCIP(��֧��)
				5. COPT(��֧��)
				6. IPOPT(��֧��)
																																					By: Katyusha
*																																					2021/11/07@XJTU
*/



/*
	OPUA���Զ���ͨ�ù���ģ��
		�ṩͨ�õ�I/O���ߡ���ʱ���ߵ�
*/
#include "OPUA.Common.h"

/*
	OPUA���Զ��峣��ģ��
		�ṩ��Բ����Pi���������Infinity�ȳ����Ķ���
*/
#include "OPUA.Constant.h"

/*
	OPUA���Զ�����������
		�ṩOPUA���ȫ����������
*/
#include "OPUA.Type.h"

/*
	OPUA�Ľ�ģ����ģ��
		�ṩOPUA��Ļ�������(����ģ������Ĺ������)
*/
#include "OPUA.Environment.h"

/*
	OPUA������ģ��
		�ṩOPUA��������
*/
#include "OPUA.Container.h"

/*
	OPUA�ľ��߱���ģ��
		�ṩOPUA���߱�������
*/
#include "OPUA.Variable.h"

/*
	OPUA��Լ������ģ��
		�ṩOPUAԼ����������
*/
#include "OPUA.Constraint.h"

/*
	OPUA��Ŀ�꺯��ģ��
		�ṩOPUAĿ�꺯������
*/
#include "OPUA.Objective.h"

/*
	OPUA���Ż�ģ��ģ��
		�ṩOPUA�Ż�ģ�Ͷ���
*/
#include "OPUA.Model.h"

/*
	OPUA�������ģ��
		�ṩOPUA�������������Ľӿ�
*/
#include "OPUA.Solver.h"

/*
	OPUA����ѧģ��
		�ṩOPUA�е���ѧ�㷨
*/
#include "OPUA.Math.h"

/*
	OPUA���㷨ģ��
		�ṩOPUA�е��Ż��㷨
*/
#include "OPUA.Algorithm.h"
