# OPUA

## OPUA.Solver: Gurobi接口使用说明
---

### 时间：2022年7月10日
### by：Katyusha
---
* ## 0 前言
  介绍OPUA中对于求解器Gurobi接口的封装原理。

* ## 1 Gurobi原生接口的设计

* ## 2 OPUA如何对Gurobi接口进行封装

* ## 3 OPUA中Gurobi参数的设置
* ### 3.1 Termination参数
  OPUA.GRB.Termination设置Gurobi的终止条件参数。

  | 参数名称 | 参数类型 | 默认值 | 允许值 | 注释 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.GRB.Termination.BarIterLimit | int | 1000 | [0, MAXINT] | 退出条件：内点法已迭代次数 |
  | OPUA.GRB.Termination.BestBdStop | double | INF | [-INF, INF] | ? |
  | OPUA.GRB.Termination.BestObjStop | double | -INF | [-INF, INF] | 退出条件：目标函数最优估计值(比这个值小就可以退出) |
  | OPUA.GRB.Termination.Cutoff | double | INF | [-INF, INF] | ? |
  | OPUA.GRB.Termination.IterationLimit | double | INF | [0, INF] | 退出条件：单纯形法已迭代次数 |
  | OPUA.GRB.Termination.NodeLimit | double | INF | [0, INF] | 退出条件：已遍历的MIP节点个数 |
  | OPUA.GRB.Termination.SolutionLimit | int | INF | [1, MAXINT] | 退出条件：已找到的可行解个数 |
  | OPUA.GRB.Termination.TimeLimit | double | INF | [0, INF] | 退出条件：求解已耗时 |

* ### 3.2 Tolerances参数
  OPUA.GRB.Tolerances设置Gurobi的可行性&最优性收敛判定参数。
  | 参数名称 | 参数类型 | 默认值 | 允许值 | 注释 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.GRB.Tolerances.BarConvTol | double | 1e-8 | [0.0, 1.0] | 内点法收敛间隙 |
  | OPUA.GRB.Tolerances.BarQCPConvTol | double | 1e-6 | [0.0, 1.0] | 内点法收敛间隙(QCP问题) |
  | OPUA.GRB.Tolerances.FeasibilityTol | double | 1e-6 | [1e-9, 1e-2] | 约束条件可行性判定间隙 |
  | OPUA.GRB.Tolerances.IntFeasTol | double | 1e-5 | [1e-9, 1e-1] | 整数解可行性判定间隙 |
  | OPUA.GRB.Tolerances.MarkowitzTol | double | 0.0078125 | [1e-4, 0.999] | ? |
  | OPUA.GRB.Tolerances.MIPGap | double | 1e-4 | [0, INF] | MIP问题相对收敛间隙 |
  | OPUA.GRB.Tolerances.MIPGapAbs | double | 1e-10 | [0, INF] | MIP问题绝对收敛间隙 |
  | OPUA.GRB.Tolerances.OptimalityTol | double | 1e-6 | [1e-9, 1e-2] | 对偶可行性判定间隙(原问题最优性判定间隙) |
  | OPUA.GRB.Tolerances.PSDTol | double | 1e-6 | [0, INF] | ? |

* ### 3.3 Simplex参数
  OPUA.GRB.Simplex设置Gurobi的单纯形法参数。
  | 参数名称 | 参数类型 | 默认值 | 允许值 | 注释 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.GRB.Simplex.InfUnbdInfo | int | 0 | [0, 1] | ? |
  | OPUA.GRB.Simplex.NormAdjust | int | -1 | [-1, 3] | ? |
  | OPUA.GRB.Simplex.ObjScale | double | 0.0 | [-1, INF] | ? |
  | OPUA.GRB.Simplex.PerturbValue | double | 0.0002 | [0, INF] | ? |
  | OPUA.GRB.Simplex.Quad | int | -1 | [-1, 1] | ? |
  | OPUA.GRB.Simplex.ScaleFlag | int | -1 | [-1, 3] | ? |
  | OPUA.GRB.Simplex.Sifting | int | -1 | [-1, 2] | ? |
  | OPUA.GRB.Simplex.SiftMethod | int | -1 | [-1, 2] | ? |
  | OPUA.GRB.Simplex.SimplexPricing | int | -1 | [-1, 3] | ? |

* ### 3.4 Barrier参数
  OPUA.GRB.Barrier设置Gurobi的内点法参数。
  | 参数名称 | 参数类型 | 默认值 | 允许值 | 注释 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.GRB.Barrier.BarCorrectors | int | -1 | [-1, MAXINT] | ? |
  | OPUA.GRB.Barrier.BarHomogeneous | int | -1 | [-1, 1] | ? |
  | OPUA.GRB.Barrier.BarOrder | int | -1 | [-1, 1] | ? |
  | OPUA.GRB.Barrier.Crossover | int | -1 | [-1, 4] | ? |
  | OPUA.GRB.Barrier.CrossoverBasis | int | 0 | [0, 1] | ? |
  | OPUA.GRB.Barrier.QCPDual | int | 0 | [0, 1] | ? |

* ### 3.5 MIP参数
  OPUA.GRB.MIP设置Gurobi的混合整数求解参数。
  | 参数名称 | 参数类型 | 默认值 | 允许值 | 注释 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.GRB.MIP.BranchDir | int | 0 | [-1, 1] | 分支定界方向选择(0-自动 / -1-优先遍历下分支 / 1-优先遍历上分支) |
  | OPUA.GRB.MIP.ConcurrentJobs | int | 0 | [0, MAXINT] | 并行求解器任务数 |
  | OPUA.GRB.MIP.ConcurrentMIP | int | 1 | [0, MAXINT] | 并行工作的MIP求解器数 |
  | OPUA.GRB.MIP.ConcurrentSettings | string | "" |  | 并行环境配置参数 |
  | OPUA.GRB.MIP.DegenMoves | int | -1 | [-1, MAXINT] | ? |
  | OPUA.GRB.MIP.Disconnected | int | -1 | [-1, 2] | ? |
  | OPUA.GRB.MIP.DistributedMIPJobs | int | 0 | [0, MAXINT] | ? |
  | OPUA.GRB.MIP.Heuristics | double | 0.05 | [0, 1] | 使用启发式方法探索MIP可行解的耗时程度(占总求解时间的比例) |
  | OPUA.GRB.MIP.ImproveStartGap | double | 0.0 | [0.0, INF] | MIP求解过程中开始切换寻优策略的相对收敛间隙起点(RelGap小于该值时，切换寻优策略) |
  | OPUA.GRB.MIP.ImproveStartNodes | double | INF | [0.0, INF] | 效果同ImproveStartGap，条件改为已搜索的MIP节点数量 |
  | OPUA.GRB.MIP.ImproveStartTime | double | INF | [0.0, INF] | 效果同ImproveStartGap，条件改为已耗时 |
  | OPUA.GRB.MIP.LazyConstraints | int | 0 | [0, 1] | 启用Lazy约束时必须开启 |
  | OPUA.GRB.MIP.MinRelNodes | int | -1| [-1, MAXINT] | ? |
  | OPUA.GRB.MIP.MIPFocus | int | 0 | [0, 3] | 影响寻优策略的配置，数值越大越倾向于搜寻最优解，数值越小越倾向于搜寻可行解 |
  | OPUA.GRB.MIP.MIQCPMethod | int | -1 | [-1, 1] | MIQCP求解模式(-1-自动选择 / 0-在每个节点上求解QCP松弛 / 1-使用线性化的外逼近法) |
  | OPUA.GRB.MIP.NodefileDir | string | "" |  | ? |
  | OPUA.GRB.MIP.NodefileStart | double | INF | [0, INF] | ? |
  | OPUA.GRB.MIP.NodeMethod | int | -1 | [-1, 2] | MIP节点上求解松弛问题的算法选择模式(-1-自动 / 0-原始单纯形 / 1-对偶单纯形 / 2-内点法) |
  | OPUA.GRB.MIP.NonConvex | int | -1 | [-1, 2] | 如果MIP中有双线性项等非线性部分，需要打开此开关，否则报错 |
  | OPUA.GRB.MIP.NoRelHeurTime | double | 0 | [0, INF] | ? |
  | OPUA.GRB.MIP.NoRelHeurWork | double | 0 | [0, INF] | ? |
  | OPUA.GRB.MIP.PartitionPlace | int | 15 | [0, 31] | 位开关：详见GRB用户手册 |
  | OPUA.GRB.MIP.PumpPassese | int | -1 | [-1, MAXINT] | ? |
  | OPUA.GRB.MIP.RINS | int | -1 | [-1, MAXINT] | RINS探测频率(-1-自动 / other-探测频率上限) |
  | OPUA.GRB.MIP.SolFiles | string | "" |  | 当前解的输出文件路径 |
  | OPUA.GRB.MIP.SolutionNumber | int | 0 | [0, MAXINT] | 当MIP存在多解时，被选中的MIP解的序号 |
  | OPUA.GRB.MIP.StartNodeLimit | int | -1| [-3, MAXINT] | ? |
  | OPUA.GRB.MIP.StartNumber | int | 0 | [-1, MAXINT] | 当存在多个MIP起点时，被选中的MIP起点的序号 |
  | OPUA.GRB.MIP.SubMIPNodes | int | 500 | [-1, MAXINT] | ? |
  | OPUA.GRB.MIP.Symmetry | int | -1 | [-1, 2] | ? |
  | OPUA.GRB.MIP.VarBranch | int | -1 | [-1, 3] | 分支定界时选择变量的策略，详见GRB用户手册 |
  | OPUA.GRB.MIP.ZeroObjNodes  | int | -1 | [-1, MAXINT] | ? |
* ## 参考文献
  [^reference1]: XXX

  


