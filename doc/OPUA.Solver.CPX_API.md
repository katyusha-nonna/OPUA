# OPUA

## OPUA.Solver: Cplex接口使用说明
---

### 时间：2022年7月10日
### by：Katyusha
---
* ## 0 前言
  介绍OPUA中对于求解器Cplex接口的封装原理。

* ## 1 Cplex原生接口的设计

* ## 2 OPUA如何对Cplex接口进行封装

* ## 3 OPUA中Cplex参数的设置
  完整内容详见[doc/OPUA.Solver.Param.xlsx](OPUA.Solver.Param.xlsx)。全部内容来自官方说明文档[^reference1]。
* ### 3.1 Barrier参数
  OPUA.CPX.Barrier设置Cplex的内点法参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.Barrier.Algorithm | IloCplex::Param::Barrier::Algorithm | OpInt | 0 | [0, 3] |
  | OPUA.CPX.Barrier.ColNonzeros | IloCplex::Param::Barrier::ColNonzeros | OpInt | 0 | [0, INF] |
  | OPUA.CPX.Barrier.ConvergeTol | IloCplex::Param::Barrier::ConvergeTol | OpFloat | 1E-8 | [1E-12, INF] |
  | OPUA.CPX.Barrier.Crossover | IloCplex::Param::Barrier::Crossover | OpInt | 0 | [-1, 2] |
  | OPUA.CPX.Barrier.Display | IloCplex::Param::Barrier::Display | OpInt | 1 | [0, 2] |
  | OPUA.CPX.Barrier.Limits.Corrections | IloCplex::Param::Barrier::Limits::Corrections | OpLInt | 0 | [-1, INF] |
  | OPUA.CPX.Barrier.Limits.Growth | IloCplex::Param::Barrier::Limits::Growth | OpFloat | 1E12 | [1, INF] |
  | OPUA.CPX.Barrier.Limits.Iteration | IloCplex::Param::Barrier::Limits::Iteration | OpLInt | 9223372036800000000 | [0, INF] |
  | OPUA.CPX.Barrier.Limits.ObjRange | IloCplex::Param::Barrier::Limits::ObjRange | OpFloat | 1E20 | [0, INF] |
  | OPUA.CPX.Barrier.Ordering | IloCplex::Param::Barrier::Ordering | OpInt | 0 | [0, 3] |
  | OPUA.CPX.Barrier.QCPConvergeTol | IloCplex::Param::Barrier::QCPConvergeTol | OpFloat | 1E-7 | [1E-12, INF] |
  | OPUA.CPX.Barrier.StartAlg | IloCplex::Param::Barrier::StartAlg | OpInt | 1 | [1, 4] |

* ### 3.2 Conflict参数
  OPUA.CPX.Conflict设置Cplex的约束冲突参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.Conflict.Algorithm | IloCplex::Param::Conflict::Algorithm | OpInt | ? | ? |
  | OPUA.CPX.Conflict.Display | IloCplex::Param::Conflict::Display | OpInt | 1 | [0, 2] |

* ### 3.3 DistMIP参数
  OPUA.CPX.DistMIP设置Cplex的分布式混合整数求解参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.DistMIP.Rampup.DetTimeLimit | IloCplex::Param::DistMIP::Rampup::DetTimeLimit | OpFloat | BIGREAL | [0, INF] |
  | OPUA.CPX.DistMIP.Rampup.Duration | IloCplex::Param::DistMIP::Rampup::Duration | OpInt | 0 | [-1, 2] |
  | OPUA.CPX.DistMIP.Rampup.TimeLimit | IloCplex::Param::DistMIP::Rampup::TimeLimit | OpFloat | BIGREAL | [0, INF] |

* ### 3.4 Emphasis参数
  OPUA.CPX.Emphasis设置Cplex的求解器倾向参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.Emphasis.Memory | IloCplex::Param::Emphasis::Memory | OpBool | FALSE | {TRUE, FALSE} |
  | OPUA.CPX.Emphasis.MIP | IloCplex::Param::Emphasis::MIP | OpInt | 0 | [0, 4] |
  | OPUA.CPX.Emphasis.Numerical | IloCplex::Param::Emphasis::Numerical | OpBool | FALSE | {TRUE, FALSE} |

* ### 3.5 Feasopt参数
  OPUA.CPX.Feasopt设置Cplex的可行松弛参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.Feasopt.Mode | IloCplex::Param::Feasopt::Mode | OpInt | 0 | [0, 5] |
  | OPUA.CPX.Feasopt.Tolerance | IloCplex::Param::Feasopt::Tolerance | OpFloat | 1E-6 | [0, INF] |

* ### 3.6 MIP参数
  OPUA.CPX.MIP设置Cplex的混合整数求解参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.MIP.Cuts.BQP | IloCplex::Param::MIP::Cuts::BQP | OpInt | 0 | [-1, 3] |
  | OPUA.CPX.MIP.Cuts.Cliques | IloCplex::Param::MIP::Cuts::Cliques | OpInt | 0 | [-1, 3] |
  | OPUA.CPX.MIP.Cuts.Covers | IloCplex::Param::MIP::Cuts::Covers | OpInt | 0 | [-1, 3] |
  | OPUA.CPX.MIP.Cuts.Disjunctive | IloCplex::Param::MIP::Cuts::Disjunctive | OpInt | 0 | [-1, 3] |
  | OPUA.CPX.MIP.Cuts.FlowCovers | IloCplex::Param::MIP::Cuts::FlowCovers | OpInt | 0 | [-1, 2] |
  | OPUA.CPX.MIP.Cuts.Gomory | IloCplex::Param::MIP::Cuts::Gomory | OpInt | 0 | [-1, 2] |
  | OPUA.CPX.MIP.Cuts.GUBCovers | IloCplex::Param::MIP::Cuts::GUBCovers | OpInt | 0 | [-1, 2] |
  | OPUA.CPX.MIP.Cuts.Implied | IloCplex::Param::MIP::Cuts::Implied | OpInt | 0 | [-1, 2] |
  | OPUA.CPX.MIP.Cuts.LiftProj | IloCplex::Param::MIP::Cuts::LiftProj | OpInt | 0 | [-1, 3] |
  | OPUA.CPX.MIP.Cuts.LocalImplied | IloCplex::Param::MIP::Cuts::LocalImplied | OpInt | 0 | [-1, 3] |
  | OPUA.CPX.MIP.Cuts.MCFCut | IloCplex::Param::MIP::Cuts::MCFCut | OpInt | 0 | [-1, 2] |
  | OPUA.CPX.MIP.Cuts.MIRCut | IloCplex::Param::MIP::Cuts::MIRCut | OpInt | 0 | [-1, 2] |
  | OPUA.CPX.MIP.Cuts.PathCut | IloCplex::Param::MIP::Cuts::PathCut | OpInt | 0 | [-1, 2] |
  | OPUA.CPX.MIP.Cuts.RLT | IloCplex::Param::MIP::Cuts::RLT | OpInt | 0 | [-1, 3] |
  | OPUA.CPX.MIP.Cuts.ZeroHalfCut | IloCplex::Param::MIP::Cuts::ZeroHalfCut | OpInt | 0 | [-1, 2] |
  | OPUA.CPX.MIP.Display | IloCplex::Param::MIP::Display | OpInt | 2 | [0, 5] |
  | OPUA.CPX.MIP.Interval | IloCplex::Param::MIP::Interval | OpLInt | 0 | [-INF, INF] |
  | OPUA.CPX.MIP.Limits.AggForCut | IloCplex::Param::MIP::Limits::AggForCut | OpInt | 3 | [-INF, INF] |
  | OPUA.CPX.MIP.Limits.AuxRootThreads | IloCplex::Param::MIP::Limits::AuxRootThreads | OpInt | ? | ? |
  | OPUA.CPX.MIP.Limits.CutPasses | IloCplex::Param::MIP::Limits::CutPasses | OpLInt | 0 | [-1, INF] |
  | OPUA.CPX.MIP.Limits.CutsFactor | IloCplex::Param::MIP::Limits::CutsFactor | OpFloat | -1E0 | [0, INF] |
  | OPUA.CPX.MIP.Limits.EachCutLimit | IloCplex::Param::MIP::Limits::EachCutLimit | OpInt | 2100000000 | [0, INF] |
  | OPUA.CPX.MIP.Limits.GomoryCand | IloCplex::Param::MIP::Limits::GomoryCand | OpInt | 200 | [0, INF] |
  | OPUA.CPX.MIP.Limits.GomoryPass | IloCplex::Param::MIP::Limits::GomoryPass | OpLInt | 0 | [0, INF] |
  | OPUA.CPX.MIP.Limits.Nodes | IloCplex::Param::MIP::Limits::Nodes | OpLInt | 9223372036800000000 | [0, INF] |
  | OPUA.CPX.MIP.Limits.PolishTime | IloCplex::Param::MIP::Limits::PolishTime | OpFloat | ? | ? |
  | OPUA.CPX.MIP.Limits.Populate | IloCplex::Param::MIP::Limits::Populate | OpInt | ? | ? |
  | OPUA.CPX.MIP.Limits.ProbeDetTime | IloCplex::Param::MIP::Limits::ProbeDetTime | OpFloat | 1E75 | [0, INF] |
  | OPUA.CPX.MIP.Limits.ProbeTime | IloCplex::Param::MIP::Limits::ProbeTime | OpFloat | 1E75 | [0, INF] |
  | OPUA.CPX.MIP.Limits.RepairTries | IloCplex::Param::MIP::Limits::RepairTries | OpLInt | 0 | [-1, INF] |
  | OPUA.CPX.MIP.Limits.Solutions | IloCplex::Param::MIP::Limits::Solutions | OpLInt | 9223372036800000000 | [0, INF] |
  | OPUA.CPX.MIP.Limits.StrongCand | IloCplex::Param::MIP::Limits::StrongCand | OpInt | 10 | [0, INF] |
  | OPUA.CPX.MIP.Limits.StrongIt | IloCplex::Param::MIP::Limits::StrongIt | OpLInt | 0 | [0, INF] |
  | OPUA.CPX.MIP.Limits.TreeMemory | IloCplex::Param::MIP::Limits::TreeMemory | OpFloat | 1E75 | [0, INF] |
  | OPUA.CPX.MIP.OrderType | IloCplex::Param::MIP::OrderType | OpInt | 0 | [0, 3] |
  | OPUA.CPX.MIP.PolishAfter.AbsMIPGap | IloCplex::Param::MIP::PolishAfter::AbsMIPGap | OpFloat | 0 | [0, INF] |
  | OPUA.CPX.MIP.PolishAfter.DetTime | IloCplex::Param::MIP::PolishAfter::DetTime | OpFloat | ? | ? |
  | OPUA.CPX.MIP.PolishAfter.MIPGap | IloCplex::Param::MIP::PolishAfter::MIPGap | OpFloat | 0 | [0, 1] |
  | OPUA.CPX.MIP.PolishAfter.Nodes | IloCplex::Param::MIP::PolishAfter::Nodes | OpLInt | 9223372036800000000 | [0, INF] |
  | OPUA.CPX.MIP.PolishAfter.Solutions | IloCplex::Param::MIP::PolishAfter::Solutions | OpLInt | 9223372036800000000 | [1, INF] |
  | OPUA.CPX.MIP.PolishAfter.Time | IloCplex::Param::MIP::PolishAfter::Time | OpFloat | 1E75 | [0, INF] |
  | OPUA.CPX.MIP.Pool.AbsGap | IloCplex::Param::MIP::Pool::AbsGap | OpFloat | 1E75 | [0, INF] |
  | OPUA.CPX.MIP.Pool.Capacity | IloCplex::Param::MIP::Pool::Capacity | OpInt | 2100000000 | [0, INF] |
  | OPUA.CPX.MIP.Pool.Intensity | IloCplex::Param::MIP::Pool::Intensity | OpInt | 0 | [0, 4] |
  | OPUA.CPX.MIP.Pool.RelGap | IloCplex::Param::MIP::Pool::RelGap | OpFloat | 1E75 | [0, INF] |
  | OPUA.CPX.MIP.Pool.Replace | IloCplex::Param::MIP::Pool::Replace | OpInt | 0 | [0, 2] |
  | OPUA.CPX.MIP.Strategy.Backtrack | IloCplex::Param::MIP::Strategy::Backtrack | OpFloat | 9.999E-1 | [0, 1] |
  | OPUA.CPX.MIP.Strategy.BBInterval | IloCplex::Param::MIP::Strategy::BBInterval | OpLInt | 7 | [0, INF] |
  | OPUA.CPX.MIP.Strategy.Branch | IloCplex::Param::MIP::Strategy::Branch | OpInt | 0 | [-1, 1] |
  | OPUA.CPX.MIP.Strategy.Dive | IloCplex::Param::MIP::Strategy::Dive | OpInt | 0 | [0, 3] |
  | OPUA.CPX.MIP.Strategy.File | IloCplex::Param::MIP::Strategy::File | OpInt | 1 | [0, 3] |
  | OPUA.CPX.MIP.Strategy.FPHeur | IloCplex::Param::MIP::Strategy::FPHeur | OpInt | ? | ? |
  | OPUA.CPX.MIP.Strategy.HeuristicEffort | IloCplex::Param::MIP::Strategy::HeuristicEffort | OpFloat | ? | ? |
  | OPUA.CPX.MIP.Strategy.HeuristicFreq | IloCplex::Param::MIP::Strategy::HeuristicFreq | OpLInt | 0 | [-1, INF] |
  | OPUA.CPX.MIP.Strategy.KappaStats | IloCplex::Param::MIP::Strategy::KappaStats | OpInt | 0 | [-1, 2] |
  | OPUA.CPX.MIP.Strategy.LBHeur | IloCplex::Param::MIP::Strategy::LBHeur | OpBool | FALSE | {TRUE, FALSE} |
  | OPUA.CPX.MIP.Strategy.MIQCPStrat | IloCplex::Param::MIP::Strategy::MIQCPStrat | OpInt | ? | ? |
  | OPUA.CPX.MIP.Strategy.NodeSelect | IloCplex::Param::MIP::Strategy::NodeSelect | OpInt | 1 | [0, 3] |
  | OPUA.CPX.MIP.Strategy.Order | IloCplex::Param::MIP::Strategy::Order | OpBool | TRUE | {TRUE, FALSE} |
  | OPUA.CPX.MIP.Strategy.PresolveNode | IloCplex::Param::MIP::Strategy::PresolveNode | OpInt | 0 | [-1, 3] |
  | OPUA.CPX.MIP.Strategy.Probe | IloCplex::Param::MIP::Strategy::Probe | OpInt | 0 | [-1, 3] |
  | OPUA.CPX.MIP.Strategy.RINSHeur | IloCplex::Param::MIP::Strategy::RINSHeur | OpLInt | 0 | [-1, INF] |
  | OPUA.CPX.MIP.Strategy.Search | IloCplex::Param::MIP::Strategy::Search | OpInt | 0 | [-1, 2] |
  | OPUA.CPX.MIP.Strategy.VariableSelect | IloCplex::Param::MIP::Strategy::VariableSelect | OpInt | 0 | [-1, 4] |
  | OPUA.CPX.MIP.SubMIP.StartAlg | IloCplex::Param::MIP::SubMIP::StartAlg | OpInt | 0 | [0, 6] |
  | OPUA.CPX.MIP.SubMIP.SubAlg | IloCplex::Param::MIP::SubMIP::SubAlg | OpInt | 0 | [0, 5] |
  | OPUA.CPX.MIP.SubMIP.NodeLimit | IloCplex::Param::MIP::SubMIP::NodeLimit | OpLInt | 500 | [0, INF] |
  | OPUA.CPX.MIP.SubMIP.Scale | IloCplex::Param::MIP::SubMIP::Scale | OpInt | 0 | [-1, 1] |
  | OPUA.CPX.MIP.Tolerances.AbsMIPGap | IloCplex::Param::MIP::Tolerances::AbsMIPGap | OpFloat | 1E-6 | [0, INF] |
  | OPUA.CPX.MIP.Tolerances.Linearization | IloCplex::Param::MIP::Tolerances::Linearization | OpFloat | 1E-3 | [0, INF] |
  | OPUA.CPX.MIP.Tolerances.Integrality | IloCplex::Param::MIP::Tolerances::Integrality | OpFloat | 1E-5 | [0, 0.5] |
  | OPUA.CPX.MIP.Tolerances.LowerCutoff | IloCplex::Param::MIP::Tolerances::LowerCutoff | OpFloat | -1E75 | [-INF, INF] |
  | OPUA.CPX.MIP.Tolerances.MIPGap | IloCplex::Param::MIP::Tolerances::MIPGap | OpFloat | 1E-4 | [0, 1] |
  | OPUA.CPX.MIP.Tolerances.ObjDifference | IloCplex::Param::MIP::Tolerances::ObjDifference | OpFloat | 0 | [-INF, INF] |
  | OPUA.CPX.MIP.Tolerances.RelObjDifference | IloCplex::Param::MIP::Tolerances::RelObjDifference | OpFloat | 0 | [0, 1] |
  | OPUA.CPX.MIP.Tolerances.UpperCutoff | IloCplex::Param::MIP::Tolerances::UpperCutoff | OpFloat | 1E75 | [-INF, INF] |

* ### 3.7 MultiObjective参数
  OPUA.CPX.MultiObjective设置Cplex的多目标优化参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.MultiObjective.Display | IloCplex::Param::MultiObjective::Display | OpInt | ? | ? |

* ### 3.8 Network参数
  OPUA.CPX.Network设置Cplex的网络优化参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.Network.Display | IloCplex::Param::Network::Display | OpInt | ? | ? |
  | OPUA.CPX.Network.Iterations | IloCplex::Param::Network::Iterations | OpInt | 9223372036800000000 | [0, INF] |
  | OPUA.CPX.Network.NetFind | IloCplex::Param::Network::NetFind | OpInt | 2 | [1, 3] |
  | OPUA.CPX.Network.Pricing | IloCplex::Param::Network::Pricing | OpInt | 0 | [0, 3] |
  | OPUA.CPX.Network.Tolerances.Feasibility | IloCplex::Param::Network::Tolerances::Feasibility | OpFloat | 1E-6 | [1E-11, 1E-1] |
  | OPUA.CPX.Network.Tolerances.Optimality | IloCplex::Param::Network::Tolerances::Optimality | OpFloat | 1E-6 | [1E-11, 1E-1] |

* ### 3.9 Output参数
  OPUA.CPX.Output设置Cplex的显示与输出参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.Output.CloneLog | IloCplex::Param::Output::CloneLog | OpBool | ? | ? |
  | OPUA.CPX.Output.IntSolFilePrefix | IloCplex::Param::Output::IntSolFilePrefix | OpStr | "" | 文件名的前缀的有效字符串 |
  | OPUA.CPX.Output.MPSLong | IloCplex::Param::Output::MPSLong | OpBool | TRUE | {TRUE, FALSE} |
  | OPUA.CPX.Output.WriteLevel | IloCplex::Param::Output::WriteLevel | OpInt | 0 | [0, 4] |

* ### 3.10 Preprocessing参数
  OPUA.CPX.Preprocessing设置Cplex的预处理参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.Preprocessing.Aggregator | IloCplex::Param::Preprocessing::Aggregator | OpInt | -1 | [-1, INF] |
  | OPUA.CPX.Preprocessing.BoundStrength | IloCplex::Param::Preprocessing::BoundStrength | OpInt | -1 | [-1, 1] |
  | OPUA.CPX.Preprocessing.CoeffReduce | IloCplex::Param::Preprocessing::CoeffReduce | OpInt | -1 | [-1, 3] |
  | OPUA.CPX.Preprocessing.Dependency | IloCplex::Param::Preprocessing::Dependency | OpInt | -1 | [-1, 3] |
  | OPUA.CPX.Preprocessing.Dual | IloCplex::Param::Preprocessing::Dual | OpInt | -1 | [-1, 1] |
  | OPUA.CPX.Preprocessing.Fill | IloCplex::Param::Preprocessing::Fill | OpLInt | 10 | [0, INF] |
  | OPUA.CPX.Preprocessing.Folding | IloCplex::Param::Preprocessing::Folding | OpInt | ? | ? |
  | OPUA.CPX.Preprocessing.Linear | IloCplex::Param::Preprocessing::Linear | OpInt | 1 | [0, 1] |
  | OPUA.CPX.Preprocessing.NumPass | IloCplex::Param::Preprocessing::NumPass | OpInt | -1 | [-1, INF] |
  | OPUA.CPX.Preprocessing.Presolve | IloCplex::Param::Preprocessing::Presolve | OpBool | TRUE | {TRUE, FALSE} |
  | OPUA.CPX.Preprocessing.QCPDuals | IloCplex::Param::Preprocessing::QCPDuals | OpInt | ? | ? |
  | OPUA.CPX.Preprocessing.QPMakePSD | IloCplex::Param::Preprocessing::QPMakePSD | OpBool | TRUE | {TRUE, FALSE} |
  | OPUA.CPX.Preprocessing.QToLin | IloCplex::Param::Preprocessing::QToLin | OpInt | ? | ? |
  | OPUA.CPX.Preprocessing.Reduce | IloCplex::Param::Preprocessing::Reduce | OpInt | 3 | [0, 3] |
  | OPUA.CPX.Preprocessing.Relax | IloCplex::Param::Preprocessing::Relax | OpInt | -1 | [-1, 1] |
  | OPUA.CPX.Preprocessing.RepeatPresolve | IloCplex::Param::Preprocessing::RepeatPresolve | OpInt | -1 | [-1, 3] |
  | OPUA.CPX.Preprocessing.Symmetry | IloCplex::Param::Preprocessing::Symmetry | OpInt | -1 | [-1, 5] |

* ### 3.11 Read参数
  OPUA.CPX.Read设置Cplex的文件读取参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.Read.APIEncoding | IloCplex::Param::Read::APIEncoding | OpStr | ? | ? |
  | OPUA.CPX.Read.Constraints | IloCplex::Param::Read::Constraints | OpInt | ? | ? |
  | OPUA.CPX.Read.DataCheck | IloCplex::Param::Read::DataCheck | OpInt | 0 | [0, 2] |
  | OPUA.CPX.Read.FileEncoding | IloCplex::Param::Read::FileEncoding | OpStr | ? | ? |
  | OPUA.CPX.Read.Nonzeros | IloCplex::Param::Read::Nonzeros | OpLInt | 250 000 | [0, INF] |
  | OPUA.CPX.Read.QPNonzeros | IloCplex::Param::Read::QPNonzeros | OpLInt | 5 000 | [0, INF] |
  | OPUA.CPX.Read.Scale | IloCplex::Param::Read::Scale | OpInt | ? | ? |
  | OPUA.CPX.Read.Variables | IloCplex::Param::Read::Variable | OpInt | 60 000 | [0, INF] |
  | OPUA.CPX.Read.WarningLimit | IloCplex::Param::Read::WarningLimit | OpLInt | ? | ? |

* ### 3.12 Sifting参数
  OPUA.CPX.Sifting设置Cplex的筛选法参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.Sifting.Algorithm | IloCplex::Param::Sifting::Algorithm | OpInt | 0 | [0, 4] |
  | OPUA.CPX.Sifting.Simplex | IloCplex::Param::Sifting::Simplex | OpBool | TRUE | {TRUE, FALSE} |
  | OPUA.CPX.Sifting.Display | IloCplex::Param::Sifting::Display | OpInt | 1 | [0, 2] |
  | OPUA.CPX.Sifting.Iterations | IloCplex::Param::Sifting::Iterations | OpLInt | 9223372036800000000 | [0, INF] |

* ### 3.13 Simplex参数
  OPUA.CPX.Simplex设置Cplex的单纯形法参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.Simplex.Crash | IloCplex::Param::Simplex::Crash | OpInt | 1 | [-1, 1] |
  | OPUA.CPX.Simplex.Dgradient | IloCplex::Param::Simplex::Dgradient | OpInt | 0 | [0, 5] |
  | OPUA.CPX.Simplex.Display | IloCplex::Param::Simplex::Display | OpInt | 1 | [0, 2] |
  | OPUA.CPX.Simplex.DynamicRows | IloCplex::Param::Simplex::DynamicRows | OpInt | -1 | [-1, 1] |
  | OPUA.CPX.Simplex.Limits.Iterations | IloCplex::Param::Simplex::Limits::Iterations | OpLInt | 9223372036800000000 | [0, INF] |
  | OPUA.CPX.Simplex.Limits.LowerObj | IloCplex::Param::Simplex::Limits::LowerObj | OpFloat | -1E75 | [-INF, INF] |
  | OPUA.CPX.Simplex.Limits.Perturbation | IloCplex::Param::Simplex::Limits::Perturbation | OpInt | 0 | [0, INF] |
  | OPUA.CPX.Simplex.Limits.Singularity | IloCplex::Param::Simplex::Limits::Singularity | OpInt | 10 | [0, INF] |
  | OPUA.CPX.Simplex.Limits.UpperObj | IloCplex::Param::Simplex::Limits::UpperObj | OpFloat | 1E75 | [-INF, INF] |
  | OPUA.CPX.Simplex.Perturbation.Constant | IloCplex::Param::Simplex::Perturbation::Constant | OpFloat | 1E-6 | [1E-8, INF] |
  | OPUA.CPX.Simplex.Perturbation.Indicator | IloCplex::Param::Simplex::Perturbation::Indicator | OpBool | FALSE | {TRUE, FALSE} |
  | OPUA.CPX.Simplex.Pgradient | IloCplex::Param::Simplex::Pgradient | OpInt | 0 | [-1, 4] |
  | OPUA.CPX.Simplex.Pricing | IloCplex::Param::Simplex::Pricing | OpInt | 0 | [0, INF] |
  | OPUA.CPX.Simplex.Refactor | IloCplex::Param::Simplex::Refactor | OpInt | ? | ? |
  | OPUA.CPX.Simplex.Tolerances.Feasibility | IloCplex::Param::Simplex::Tolerances::Feasibility | OpFloat | 1E-6 | [1E-9, 1E-1] |
  | OPUA.CPX.Simplex.Tolerances.Markowitz | IloCplex::Param::Simplex::Tolerances::Markowitz | OpFloat | 1E-2 | [1E-4, 9.9999E-1] |
  | OPUA.CPX.Simplex.Tolerances.Optimality | IloCplex::Param::Simplex::Tolerances::Optimality | OpFloat | 1E-6 | [1E-9, 1E-1] |

* ### 3.14 Tune参数
  OPUA.CPX.Tune设置Cplex的调整参数参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.Tune.DetTimeLimit | IloCplex::Param::Tune::DetTimeLimit | OpFloat | 1E75 | [0, INF] |
  | OPUA.CPX.Tune.Display | IloCplex::Param::Tune::Display | OpInt | 1 | [0, 3] |
  | OPUA.CPX.Tune.Measure | IloCplex::Param::Tune::Measure | OpInt | 1 | [1, 2] |
  | OPUA.CPX.Tune.Repeat | IloCplex::Param::Tune::Repeat | OpInt | 1 | [0, INF] |
  | OPUA.CPX.Tune.TimeLimit | IloCplex::Param::Tune::TimeLimit | OpFloat | 1E75 | [0, INF] |

* ### 3.15 其他参数
  OPUA.CPX设置其他未归类的Cplex参数。

  | 参数名称 | 原参数名称 | 参数类型 | 默认值 | 允许值 |
  | :---- | :---- | :---- | :---- | :---- |
  | OPUA.CPX.Advance | IloCplex::Param::Advance | OpInt | 1 | [0, 2] |
  | OPUA.CPX.ClockType | IloCplex::Param::ClockType | OpInt | 2 | [0, 2] |
  | OPUA.CPX.CPUmask | IloCplex::Param::CPUmask | OpStr | "auto" | 由集合 {0-9, a-f, A-F} 中的数字或字符组成的字符串或"off"或"auto" |
  | OPUA.CPX.DetTimeLimit | IloCplex::Param::DetTimeLimit | OpFloat | ? | ? |
  | OPUA.CPX.NodeAlgorithm | IloCplex::Param::NodeAlgorithm | OpInt | 0 | [0, 5] |
  | OPUA.CPX.OptimalityTarget | IloCplex::Param::OptimalityTarget | OpInt | 0 | [0, 3] |
  | OPUA.CPX.Parallel | IloCplex::Param::Parallel | OpInt | 0 | [-1, 1] |
  | OPUA.CPX.ParamDisplay | IloCplex::Param::ParamDisplay | OpBool | TRUE | {TRUE, FALSE} |
  | OPUA.CPX.RandomSeed | IloCplex::Param::RandomSeed | OpInt | ? | ? |
  | OPUA.CPX.Record | IloCplex::Param::Record | OpBool | ? | ? |
  | OPUA.CPX.RootAlgorithm | IloCplex::Param::RootAlgorithm | OpInt | 0 | [0, 6] |
  | OPUA.CPX.SolutionType | IloCplex::Param::SolutionType | OpInt | 0 | [0, 2] |
  | OPUA.CPX.Threads | IloCplex::Param::Threads | OpInt | 0 | [0, INF] |
  | OPUA.CPX.TimeLimit | IloCplex::Param::TimeLimit | OpFloat | 1E+75 | [0, INF] |
  | OPUA.CPX.WorkDir | IloCplex::Param::WorkDir | OpStr | "." | 任何现有目录 |
  | OPUA.CPX.WorkMem | IloCplex::Param::WorkMem | OpFloat | 2048 | [0, INF] |


* ## 参考文献
  [^reference1]: IBM CPLEX-部分参数列表. https://www.ibm.com/docs/zh/icos/12.8.0.0?topic=cplex-topical-list-parameters

  


