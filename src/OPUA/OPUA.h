#pragma once

// OPUA的总头文件

/*
*  
	介绍：
			OPUA(Optimization Program Universal API)是一套优化求解器通用接口工具，旨在统一主流求解器的接口，
			使用户可以使用统一的接口进行优化模型的数学建模，以及根据需求选择多种求解器进行求解(类似于Yalmip)。
			OPUA亦可为优化算法框架的开发提供支持。

			计划支持的求解器：
				1. IBM CPLEX(已支持)
				2. Gurobi(已支持)
				3. Mosek(已支持)
				4. SCIP(已支持)
				5. COPT(已支持)
				6. IPOPT(已支持)
																																					By: Katyusha
*																																					2021/11/07@XJTU
*/



/*
	OPUA的自定义通用工具模块
		提供通用的I/O工具、计时工具等
*/
#include "OPUA.Common.h"

/*
	OPUA的自定义常数模块
		提供如圆周率Pi，无穷大数Infinity等常数的定义
*/
#include "OPUA.Constant.h"

/*
	OPUA的自定义类型声明
		提供OPUA库的全局类型声明
*/
#include "OPUA.Type.h"

/*
	OPUA的建模环境模块
		提供OPUA库的环境变量(所有模型组件的管理对象)
*/
#include "OPUA.Environment.h"

/*
	OPUA的容器模块
		提供OPUA容器对象
*/
#include "OPUA.Container.h"

/*
	OPUA的决策变量模块
		提供OPUA决策变量对象
*/
#include "OPUA.Variable.h"

/*
	OPUA的约束条件模块
		提供OPUA约束条件对象
*/
#include "OPUA.Constraint.h"

/*
	OPUA的目标函数模块
		提供OPUA目标函数对象
*/
#include "OPUA.Objective.h"

/*
	OPUA的优化模型模块
		提供OPUA优化模型对象
*/
#include "OPUA.Model.h"

/*
	OPUA的求解器模块
		提供OPUA与各主流求解器的接口
*/
#include "OPUA.Solver.h"

/*
	OPUA的数学模块
		提供OPUA中的数学算法
*/
#include "OPUA.Math.h"

/*
	OPUA的算法模块
		提供OPUA中的优化算法
*/
#include "OPUA.Algorithm.h"
