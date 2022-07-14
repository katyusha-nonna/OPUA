## OPUA

OPUA(Optimization Program Universal API)是一套优化求解器通用接口工具，旨在统一主流求解器的接口，使用户可以使用统一的接口进行优化模型的数学建模，以及根据需求选择多种求解器进行求解(类似于Yalmip)。OPUA亦可为优化算法框架的开发提供支持。

计划支持的求解器：
* IBM ILOG CPLEX(已支持)
* Gurobi(已支持)
* Mosek
* SCIP(已支持)
* COPT(已支持)
* IPOPT(已支持)

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
* **OPUA.Constraint**: 约束条件变量模块，提供OPUA的约束条件模型，包括：线性约束、二次约束、SOS约束、非线性约束、条件约束、逻辑约束、特殊约束等
* **OPUA.Objective**: 目标函数模块，提供OPUA的目标函数模型
* **OPUA.Model**: 优化问题模块，提供OPUA的优化问题模型
* **OPUA.Solver**: 求解器模块，提供OPUA对主流求解器的求解接口，包括：GRB、CPX、MSK、SCIP、COPT、IPOPT等

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
| Eigen | OPUA_COMPILE_EIGEN |  | _CRT_SECURE_NO_WARNINGS |

## 使用说明

OPUA除表达式以外的所有基本组件(决策变量、约束条件、目标函数、优化模型、求解器接口)均使用了Pimpl设计模式，并且由OPUA环境变量提供统一的内存管理。

在创建模型之前，我们首先要创建一个环境变量：

```cpp
OpEnv env(true, "OPUA_ENV_TEST");
```

然后，我们可以创建决策变量：

```cpp
// 连续变量
Variable::OpVar var1(env, Variable::OpVarType::Con, 0.0, 100.0);
Variable::OpVar var2(env, Variable::OpVarType::Con, 0.0, 100.0); 
// 0-1变量
Variable::OpVar var3(env, Variable::OpVarType::Bool, 0, 1);
// 整数变量
Variable::OpVar var4(env, Variable::OpVarType::Int, 0.0, 10);
```

接着，我们创建各类约束条件。需要提醒的是，OPUA重载了+、-、*、/运算符，允许表达式参与数学运算，并通过<=、>=、==运算符直接创建约束条件：

```cpp
// 线性表达式
auto linexpr1 = var1 + 2 * var2 + 3 * var3 + 0.5;
auto linexpr2 = linexpr1 / 2;
// 二次表达式
auto quadexpr1 = linexpr1 * linexpr1;
// 线性约束
auto lincon1 = linexpr1 >= 5;
auto lincon2 = 4 == linexpr2;
// 二次约束
auto quadcon1 = quadexpr1 <= linexpr1;
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

这里我们给出了一份“八皇后问题”的示例程序作为参考：

```cpp
#include <iostream>
#include "OPUA.h"

int main
{
  using namespace OPUA::Container;
  using namespace OPUA::Variable;
  using namespace OPUA::Expression;
  using namespace OPUA::Constraint;
  using namespace OPUA::Objective;
  using namespace OPUA::Model;
  using namespace OPUA::Solver;
  // 创建OPUA环境
  OpEnv env(true, "OPUA_ENV_TEST");
  // 创建OPUA模型对象
  OpModel mdl(env, "N-Queens");
  // 创建变量
  constexpr OpULInt N = 8;
  std::vector<std::vector<OpVar> > x(N);
  for (OpULInt i = 0; i < N; i++)
    x[i] = std::vector<OpVar>(N);
  for (OpULInt i = 0; i < N; i++)
    for (OpULInt j = 0; j < N; j++)
      x[i][j] = OpVar(env, OpVarType::Bool, 0, 1, "x_" + std::to_string(i) + "_" + std::to_string(j));
  // 创建并添加约束条件
  // 约束1：列之和不能超过1
  for (OpULInt j = 0; j < N; j++)
  {
    OpLinExpr colSum(0.0);
    for (OpULInt i = 0; i < N; i++)
      colSum += x[i][j];
    mdl.add(colSum <= 1);
  }
  // 约束2：行之和不能超过1
  for (OpULInt i = 0; i < N; i++)
  {
    OpLinExpr rowSum(0.0);
    for (OpULInt j = 0; j < N; j++)
      rowSum += x[i][j];
    mdl.add(rowSum <= 1);
  }
  // 约束3：对角线之和不能超过1
  for (OpULInt j = 0; j < N; j++)
  {
    OpLinExpr diagSum(0.0);
    for (OpULInt i = 0; i < N - j; i++)
      diagSum += x[i][j + i];
    mdl.add(diagSum <= 1);
  }
  for (OpULInt i = 0; i < N; i++)
  {
    OpLinExpr diagSum(0.0);
    for (OpULInt j = 0; j < N - i; j++)
      diagSum += x[i + j][j];
    mdl.add(diagSum <= 1);
  }
  for (OpULInt j = 0; j < N; j++)
  {
    OpLinExpr diagSum(0.0);
    for (OpULInt i = 0; i < N - j; i++)
      diagSum += x[i][N - j - i - 1];
    mdl.add(diagSum <= 1);
  }
  for (OpULInt i = 0; i < N; i++)
  {
    OpLinExpr diagSum(0.0);
    for (OpULInt j = 0; j < N - i; j++)
      diagSum += x[i + j][N - j - 1];
    mdl.add(diagSum <= 1);
  }
  // 创建目标函数	
  {
    OpLinExpr obj(0.0);
    for (size_t i = 0; i < N; i++)
      for (size_t j = 0; j < N; j++)
        obj += x[i][j];
    mdl.setObj(OpMaximize(env, obj));
  }
  // 输出模型
	mdl.write("");
  // 创建求解器对象并抽取模型
  OpGRBSol solver(env, mdl);
  // 创建配置器并配置求解参数
  OpConfig config;
  config.regCfg("OPUA.GRB.Termination.TimeLimit", OpFloat(60));
  config.regCfg("OPUA.GRB.Tolerances.MIPGap", OpFloat(1e-5));
  solver.setParam(config);
  // 求解模型
  solver.solve();
  // 输出结果
  std::cout << "Status:\t" << solver.getStatus() << std::endl;
  std::cout << "Obj:\t" << solver.getObjValue() << std::endl;
  std::cout << "x: " << std::endl;
  for (OpULInt i = 0; i < N; i++)
  {
    for (OpULInt j = 0; j < N; j++)
      std::cout << solver.getValue(x[i][j]) << '\t';
    std::cout << std::endl;
  }
  // 释放内存
  env.release();
  system("pause");
  return 0;
} 
```

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

这里我们给出了一份两阶段鲁棒问题的示例程序作为参考[^reference1]：

```cpp
#include <iostream>
#include "OPUA.h"

int main
{
  using namespace OPUA::Container;
  using namespace OPUA::Variable;
  using namespace OPUA::Expression;
  using namespace OPUA::Constraint;
  using namespace OPUA::Objective;
  using namespace OPUA::Model;
  using namespace OPUA::Solver;
  using namespace OPUA::Algorithm;

  // 创建环境变量
  OpEnv env(true, "OPUA_ENV_TEST");
  // 创建两阶段鲁棒模型
  OpRobustModel model(env);
  // 创建变量
  OpVarArr y(env);
  for (OpULInt i = 0; i < 3; i++)
    y.add(OpVar(env, OpVarType::Bool, 0, 1, "y_" + std::to_string(i)));
  OpVarArr z(env);
  for (OpULInt i = 0; i < 3; i++)
    z.add(OpVar(env, OpVarType::Con, 0, 800, "z_" + std::to_string(i)));
  OpVarMat x(env);
  for (OpULInt i = 0; i < 3; i++)
  {
    OpVarArr tmp(env);
    for (OpULInt j = 0; j < 3; j++)
      tmp.add(OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x_" + std::to_string(i) + "_" + std::to_string(j)));
    x.add(tmp);
  }
  OpVarArr s(env);
  for (OpULInt i = 0; i < 6; i++)
    s.add(OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s_" + std::to_string(i)));
  OpFloat alpha = 1e3;
  OpVarArr d(env);
  for (OpULInt i = 0; i < 3; i++)
    d.add(OpVar(env, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "d_" + std::to_string(i)));
  d[0].setLb(206), d[0].setUb(246);
  d[1].setLb(274), d[1].setUb(314);
  d[2].setLb(220), d[2].setUb(260);
  OpVarArr g(env);
  for (OpULInt i = 0; i < 3; i++)
    g.add(OpVar(env, OpVarType::Con, 0, 1, "g_" + std::to_string(i)));
  // 形成两阶段鲁棒模型
  // [1] 第一阶段
  // [1-1] 目标函数
  model.setObj(400 * y[0] + 414 * y[1] + 326 * y[2] + 18 * z[0] + 25 * z[1] + 20 * z[2], RobustStageType::FirstStage);
  // [1-2] 约束条件
  for (int i = 0; i < 3; i++)
    model.add(0 <= 800 * y[i] - z[i], RobustStageType::FirstStage, true);
  // [1-3] 决策变量
  model.add(y, RobustStageType::FirstStage, true);
  model.add(z, RobustStageType::FirstStage, true);
  // [2] 第二阶段原问题
  // [2-1] 目标函数
  model.setObj(22 * x[0][0] + 33 * x[0][1] + 24 * x[0][2]
    + 33 * x[1][0] + 23 * x[1][1] + 30 * x[1][2]
    + 20 * x[2][0] + 25 * x[2][1] + 27 * x[2][2]
    + alpha * (s[0] + s[1] + s[2] + s[3] + s[4] + s[5]), RobustStageType::SecondStagePrimal);
  // [2-2] 约束条件
  for (int i = 0; i < 3; i++)
    model.add(0 <= s[i] + z[i] - (x[i][0] + x[i][1] + x[i][2]), RobustStageType::SecondStagePrimal, true);
  for (int i = 0; i < 3; i++)
    model.add(0 <= s[i + 3] + x[0][i] + x[1][i] + x[2][i] - d[i], RobustStageType::SecondStagePrimal, true);
  // [2-3] 决策变量
  for (int i = 0; i < 3; i++)
    model.add(x[i], RobustStageType::SecondStagePrimal, true);
  model.add(s, RobustStageType::SecondStagePrimal, true);
  // [4] 不确定集
  // [4-1] 约束条件
  model.add(d[0] - 40 * g[0] - 206 == 0, RobustStageType::Uncertainty, false);
  model.add(d[1] - 40 * g[1] - 274 == 0, RobustStageType::Uncertainty, false);
  model.add(d[2] - 40 * g[2] - 220 == 0, RobustStageType::Uncertainty, false);
  model.add(1.8 >= g[0] + g[1] + g[2], RobustStageType::Uncertainty, true);
  model.add(1.2 >= g[0] + g[1], RobustStageType::Uncertainty, true);
  // [4-2] 决策变量
  model.add(d, RobustStageType::Uncertainty, true);
  model.add(g, RobustStageType::Uncertainty, true);
  // 自动推导对偶
  model.autoDual();
  model.update();
  // 设置初始解
  model.setValue(d[0].getImpl(), RobustStageType::Uncertainty, 206);
  model.setValue(d[1].getImpl(), RobustStageType::Uncertainty, 274);
  model.setValue(d[2].getImpl(), RobustStageType::Uncertainty, 220);
  model.setValue(g[0].getImpl(), RobustStageType::Uncertainty, 0);
  model.setValue(g[1].getImpl(), RobustStageType::Uncertainty, 0);
  model.setValue(g[2].getImpl(), RobustStageType::Uncertainty, 0);
  // 导出模型
  model.write("./Model/Test");
  // 创建CCG求解器&配置器
  OpAlgoCCG solver(model);
  auto config = DefaultCfg4CCG();
  auto flag = solver.solve(config);
  // 输出解
  if (flag)
  {
    auto printSol = [](auto& varArr, auto& model, auto stage) {
      for (OpULInt i = 0; i < varArr.getSize(); i++)
        std::cout << model.getValue(varArr[i], stage) << '\t';
    };
    std::cout << "y: ";
    printSol(y, model, RobustStageType::FirstStage);
    std::cout << std::endl;
    std::cout << "z: ";
    printSol(z, model, RobustStageType::FirstStage);
    std::cout << std::endl;
    std::cout << "x: ";
    for (OpULInt i = 0; i < x.getSize(); i++)
      printSol(x[i], model, RobustStageType::SecondStagePrimal);
    std::cout << std::endl;
    std::cout << "d: ";
    printSol(d, model, RobustStageType::Uncertainty);
    std::cout << std::endl;
    std::cout << "g: ";
    printSol(g, model, RobustStageType::Uncertainty);
    std::cout << std::endl;
    std::cout << "obj(auto): " << model.getObjValue(RobustStageType::Unknown) << std::endl;
  }
  model.release();
  env.release();
  system("pause");
  return 0;
}
```

#### 两阶段多场景随机规划(Benders分解算法求解)

这里我们给出了一份两阶段问题的示例程序作为参考[^reference2]：

```cpp
#include <iostream>
#include "OPUA.h"

int main
{
  using namespace OPUA::Variable;
  using namespace OPUA::Expression;
  using namespace OPUA::Constraint;
  using namespace OPUA::Objective;
  using namespace OPUA::Algorithm;

  OpEnv env(true, "OPUA_ENV_TEST");
  OpMSSPModel model(env);
  OpLInt subIdx(model.addSubProb());
  // 构建主问题
  auto y1 = OpVar(env, OpVarType::Bool, 0, 1, "y1");
  auto y2 = OpVar(env, OpVarType::Bool, 0, 1, "y2");
  auto y3 = OpVar(env, OpVarType::Bool, 0, 1, "y3");
  auto y4 = OpVar(env, OpVarType::Bool, 0, 1, "y4");
  auto y5 = OpVar(env, OpVarType::Bool, 0, 1, "y5");
  model.add(y1);
  model.add(y2);
  model.add(y3);
  model.add(y4);
  model.add(y5);
  model.setObj(7 * y1 + 7 * y2 + 7 * y3 + 7 * y4 + 7 * y5);
  // 构建子问题
  auto x1 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x1");
  auto x2 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x2");
  auto x3 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x3");
  auto x4 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x4");
  auto x5 = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "x5");
  auto s1p = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s1+");
  auto s2p = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s2+");
  auto s3p = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s3+");
  auto s1n = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s1-");
  auto s2n = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s2-");
  auto s3n = OpVar(env, OpVarType::Con, 0, Constant::Infinity, "s3-");
  OpFloat pentyFactor(1e5);
  model.add(x1, subIdx);
  model.add(x2, subIdx);
  model.add(x3, subIdx);
  model.add(x4, subIdx);
  model.add(x5, subIdx);
  model.add(s1p, subIdx);
  model.add(s2p, subIdx);
  model.add(s3p, subIdx);
  model.add(s1n, subIdx);
  model.add(s2n, subIdx);
  model.add(s3n, subIdx);
  model.setObj(x1 + x2 + x3 + x4 + x5 + pentyFactor * (s1p + s1n + s2p + s2n + s3p + s3n), subIdx);
  model.add(x1 + x4 + x5 + s1p - s1n == 8, subIdx);
  model.add(x2 + x5 + s2p - s2n == 3, subIdx);
  model.add(x3 + x4 + s3p - s3n == 5, subIdx);
  model.add(x1 <= 8 * y1, subIdx);
  model.add(x2 <= 3 * y2, subIdx);
  model.add(x3 <= 5 * y3, subIdx);
  model.add(x4 <= 5 * y4, subIdx);
  model.add(x5 <= 3 * y5, subIdx);
  // 创建BD求解器&配置器
  OpAlgoBD solver(model);
  auto config = DefaultCfg4BD();
  config.regCfg("OPUA.CPX.Preprocessing.Presolve", OpBool(false));
  auto flag = solver.solve(config);
  // 输出解
  if (flag)
  {
    std::cout << "Objm: " << model.getObjValue() << std::endl;
    std::cout << "Objs: " << model.getObjValue(subIdx) << std::endl;
    std::cout << "y1: " << model.getValue(y1) << std::endl;
    std::cout << "y2: " << model.getValue(y2) << std::endl;
    std::cout << "y3: " << model.getValue(y3) << std::endl;
    std::cout << "y4: " << model.getValue(y4) << std::endl;
    std::cout << "y5: " << model.getValue(y5) << std::endl;
    std::cout << "x1: " << model.getValue(x1, subIdx) << std::endl;
    std::cout << "x2: " << model.getValue(x2, subIdx) << std::endl;
    std::cout << "x3: " << model.getValue(x3, subIdx) << std::endl;
    std::cout << "x4: " << model.getValue(x4, subIdx) << std::endl;
    std::cout << "x5: " << model.getValue(x5, subIdx) << std::endl;
  }
  env.release();
}
```

* ## 参考文献
  [^reference1]: Zeng B, Zhao L. Solving two-stage robust optimization problems using a column-and-constraint generation method[J]. Operations Research Letters, 2013, 41(5): 457-461.
  [^reference2]: http://hacivat.ie.boun.edu.tr/~taskin/pdf/taskin_benders.pdf
