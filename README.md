## OPUA

OPUA(Optimization Program Universal API)是一套优化求解器通用接口工具，旨在统一主流求解器的接口，使用户可以使用统一的接口进行优化模型的数学建模，以及根据需求选择多种求解器进行求解(类似于Yalmip)。

计划支持的求解器：
* IBM CPLEX
* Gurobi
* Mosek
* SCIP

<p align="right">
By: Katyusha
</p>
<p align="right">
2021/11/07@XJTU
</p>

## 模块介绍

OPUA由以下基本模块构成：

* **OPUA.Environment**: 环境变量模块，提供统一的内存管理
* **OPUA.Containter**: 容器模块，提供OPUA的基本容器，如动态数组和字典
* **OPUA.Variable**: 决策变量模块，提供OPUA的决策变量模型，包括0-1变量、离散变量、连续变量等
* **OPUA.Expression**: 表达式模块，提供OPUA的表达式模型，包括线性表达式、二次表达式和非线性表达式等
* **OPUA.Constraint**: 约束条件变量模块，提供OPUA的约束条件模型，包括：线性约束、二次约束、SOS约束、非线性约束、特殊约束等
* **OPUA.Objective**: 目标函数模块，提供OPUA的目标函数模型
* **OPUA.Model**: 优化问题模块，提供OPUA的优化问题模型
* **OPUA.Solver**: 求解器模块，提供OPUA对主流求解器的求解接口

除此之外包含以下进阶模块：

* **OPUA.Math**: (尚未实现) 数学算法模块
* **OPUA.Algorithm**: (尚未实现) 优化算法模块，计划支持BD\C&CG

以下文件提供基本的类型和常数定义：

* **OPUA.Type**: OPUA的数据类型定义，如整型、浮点等
* **OPUA.Math**: OPUA的常数定义，如pi等
