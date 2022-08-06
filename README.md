## OPUA

OPUA(Optimization Program Universal API)是一套优化求解器通用接口工具，旨在统一主流求解器的接口，使用户可以使用统一的接口进行优化模型的数学建模，以及根据需求选择多种求解器进行求解(类似于Yalmip)。OPUA亦可为优化算法框架的开发提供支持。

计划支持的求解器：
* IBM ILOG CPLEX(已支持)
* Gurobi(已支持)
* Mosek(已支持)
* SCIP(已支持)
* COPT(已支持)
* IPOPT(已支持)
* GLPK(已支持)
* CBC(已支持)

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
* **OPUA.Variable**: 决策变量模块，提供OPUA的决策变量模型，包括0-1变量、离散变量、连续变量、半定变量等
* **OPUA.Expression**: 表达式模块，提供OPUA的表达式模型，包括线性表达式、二次表达式和非线性表达式等
* **OPUA.Constraint**: 约束条件变量模块，提供OPUA的约束条件模型，包括：线性约束、二次约束、锥约束、半定约束、SOS约束、非线性约束、条件约束、逻辑约束、特殊约束等
* **OPUA.Objective**: 目标函数模块，提供OPUA的目标函数模型
* **OPUA.Model**: 优化问题模块，提供OPUA的优化问题模型
* **OPUA.Solver**: 求解器模块，提供OPUA对主流求解器的求解接口，包括：GRB、CPX、MSK、SCIP、COPT、IPOPT、GLPK、CBC等

除此之外包含以下进阶模块：

* **OPUA.Math**: 数学算法模块，提供各种数值算法用于辅助建模。目前已支持：函数线性分段拟合(一元二次、多元凸函数)
* **OPUA.Algorithm**: 优化算法模块，提供各种经典的优化模型及算法框架。目前已支持C&CG(用于两阶段鲁棒模型的求解)、BD(用于两阶段多场景随机规划模型的求解)、ATC(用于分层模型的分布式求解)。还提供约束线性化、KKT条件自动推导、对偶问题自动推导等通用算法。

以下文件提供基本的类型和常数定义：

* **OPUA.Type**: OPUA的数据类型定义，如整型、浮点等
* **OPUA.Constant**: OPUA的常数定义，如pi等

以下文件提供内存管理、文件I/O等基础功能：
* **OPUA.Common**: OPUA的通用工具，如计时器、控制台&文件打印等
* **OPUA.Exception**: OPUA的异常处理工具

OPUA提供了编译开关控制优化求解器或第三方库的编译，在编译时，将需要支持的求解器或第三方库对应的编译开关的宏及其他需要伴随添加的宏加入预处理器定义即可：

| 求解器或第三方库 | 编译开关宏名称 | 依赖库 | 其他需要伴随添加的宏名称 |
| :----: | :----: | :----: | :----: |
| IBM ILOG CPLEX | OPUA_COMPILE_CPX |  | IL_STD |
| Gurobi | OPUA_COMPILE_GRB |  |  |
| Mosek | OPUA_COMPILE_MSK |  |  |
| SCIP | OPUA_COMPILE_SCIP |  |  |
| COPT | OPUA_COMPILE_COPT |  |  |
| IPOPT | OPUA_COMPILE_IPOPT | CppAD |  |
| GLPK | OPUA_COMPILE_GLPK |  |  |
| CBC | OPUA_COMPILE_CBC |  |  |
| Eigen | OPUA_COMPILE_EIGEN |  | _CRT_SECURE_NO_WARNINGS |

**警告！**：Mosek求解器不可与其他求解器一同编译，Mosek需要在IDE中设置“属性->配置属性->C/C++->代码生成->运行库”为“多线程(/MT)”，其余求解器需设置为“多线程DLL(/MD)”。

## 使用说明

OPUA除表达式以外的所有基本组件(决策变量、约束条件、目标函数、优化模型、求解器接口)均使用了Pimpl设计模式，并且由OPUA环境变量提供统一的内存管理。

在创建模型之前，我们首先要创建一个环境变量：

```cpp
OpEnv env(true, "OPUA_ENV_TEST");
```

然后，我们可以创建决策变量：

```cpp
// 连续变量
OpVar var1(env, OpVarType::Con, 0.0, 100.0);
OpVar var2(env, OpVarType::Con, 0.0, 100.0); 
// 0-1变量
OpVar var3(env, OpVarType::Bool, 0, 1);
// 整数变量
OpVar var4(env, OpVarType::Int, 0.0, 10);
// 半定变量
OpPSDVar var5(env, 3);
```

接着，我们创建各类约束条件。需要提醒的是，OPUA重载了+、-、*、/运算符，允许表达式参与数学运算，并通过<=、>=、==运算符直接创建约束条件：

```cpp
// 线性表达式
auto linexpr1 = var1 + 2 * var2 + 3 * var3 + 0.5;
auto linexpr2 = linexpr1 / 2;
// 二次表达式
auto quadexpr1 = linexpr1 * linexpr1;
// 半定表达式
auto A1 = Expression::OpPSDRealMat(3);
A1.toEyes();
auto psdexpr1 = A1 * var5 + linexpr1;
// 线性约束
auto lincon1 = linexpr1 >= 5;
auto lincon2 = 4 == linexpr2;
// 二次约束
auto quadcon1 = quadexpr1 <= linexpr1;
// 锥约束
OpVarArr X(env);
X.add(var3);
X.add(var2);
X.add(var1);
auto conic1 = OpConicCon(env, OpConicSense::SOC, X);
// 半定约束
auto psdcon1 = psdexpr1 == 0;
// SOS约束
auto sos1con1 = OpSOSCon(env, OpConSense::SOS1, linexpr1);
// 非线性约束
auto nlcon1 = OpAbs(env, var4, var1);
// 条件约束
auto indcon1 = var4 >> lincon2;
```

接着，我们可以创建目标函数：

```cpp
// 线性目标函数
auto linobj1 = OpMinimize(env, linexpr2);  
// 二次目标函数 
auto quadobj1 = OpMinimize(env, quadexpr1);
```

最后，创建优化模型，将约束条件和目标函数加入模型中。需要提醒的是：**OPUA会自动将约束条件及目标函数中所包含的变量加入模型中，不需要手动处理。当约束条件或目标函数被加入模型后，约束条件或目标函数本身会被锁定(自动执行lock())，限制其修改；当约束条件或目标函数从模型中移除时，会被解锁(自动执行unlock())，以允许修改，这样设计的理由在于防止用户在将某个组件加入模型后擅自修改而引发混乱。**

```cpp
// 创建模型对象
OpModel mdl(env, "OPUA_MODEL_TEST");
// 加入约束条件
mdl.add(lincon1);
// 移除约束条件
mdl.remove(lincon1);
```

至此，模型已经构建完毕。接下来进入求解阶段。我们提供了多种商业求解器的接口，允许用户在这些求解器之间自由选择而不必更改模型本身，实现了求解器的自由切换。以Gurobi为例，我们需要依次进行以下步骤：创建一个求解器接口对象->抽取我们的OPUA模型->设置参数->求解模型->最后获取求解结果：

```cpp
// 创建求解器接口对象并抽取OPUA模型
Solver::OpGRBSol grb1(env, mdl);
// 创建通用配置器并填入配置
Solver::OpConfig cfg1;
cfg1.regCfg("OPUA.GRB.Termination.TimeLimit", OpFloat(60));
// 加载求解配置
grb1.setParam(cfg1);
// 调用求解器执行求解
grb1.solve();
// 求解完毕，取变量、表达式、目标函数的解
std::cout << var1 << '\t' << grb1.getValue(var1) << std::endl;
std::cout << var2 << '\t' << grb1.getValue(var2) << std::endl;
std::cout << var3 << '\t' << grb1.getValue(var3) << std::endl;
std::cout << "Obj:\t" << grb1.getValue(linobj1) << std::endl;
```

最后需要特别提醒的是，OPUA除表达式以外的所有组件都有两种释放内存的方式
* [1] 单组件手动释放
  
  ```cpp
  mdl.release();
  ```

* [2] 使用环境变量统一释放
  
  ```cpp
  env.release();
  ```

需要指出的是，环境变量不会在析构时自动执行release()，因此用户需要保证一旦环境变量不再使用，就要正确地执行release()。

## 示例程序
### 基础功能

这里我们给出了一份[“八皇后问题”的示例程序](./example/n_queen.cpp)作为参考。

运行结果如下：

```cpp
Status: 2
Obj:    8
x:
0       0       0       1       0       0       0       0
0       1       0       0       0       0       0       0
0       0       0       0       0       0       1       0
0       0       1       0       0       0       0       0
0       0       0       0       0       1       0       0
0       0       0       0       0       0       0       1
1       0       0       0       0       0       0       0
0       0       0       0       1       0       0       0
请按任意键继续. . .
```

### 进阶功能

#### 两阶段鲁棒优化(C&CG算法求解)

这里我们给出了一份[两阶段鲁棒问题的示例程序](./example/ccg.cpp)作为参考[^reference1]：

运行结果如下：

```cpp
y: 1    0       1
z: 252  0       520
x: 0    0       252     0       0       0       206     314     0
d: 206  314     252
g: 0    1       0.8
obj(auto): 33680
请按任意键继续. . .
```

#### 两阶段多场景随机规划(Benders分解算法求解)

这里我们给出了一份[两阶段问题的示例程序](./example/bd.cpp)作为参考[^reference2]。

运行结果如下：

```cpp
Objm: 14
Objs: 8
y1: 0
y2: 0
y3: 0
y4: 1
y5: 1
x1: 0
x2: 0
x3: 0
x4: 5
x5: 3
请按任意键继续. . .
```

* ## 参考文献
  [^reference1]: Zeng B, Zhao L. Solving two-stage robust optimization problems using a column-and-constraint generation method[J]. Operations Research Letters, 2013, 41(5): 457-461.
  [^reference2]: http://hacivat.ie.boun.edu.tr/~taskin/pdf/taskin_benders.pdf
