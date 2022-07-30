#ifdef OPUA_COMPILE_CPX
#include "OPUA.Solver.h"
#include "OPUA.Exception.h"
#include "OPUA.Common.h"
#include <ilcplex/ilocplex.h>

using namespace OPUA;
#endif

#ifdef OPUA_COMPILE_CPX
/* OPUA::Solver::OpCPXCfgCvt */

// CPX配置翻译器
class Solver::OpCPXCfgCvt
{
protected:
	std::unordered_map<OpStr, IloCplex::NumParam> npdict_;
	std::unordered_map<OpStr, IloCplex::BoolParam> bpdict_;
	std::unordered_map<OpStr, IloCplex::IntParam> ipdict_;
	std::unordered_map<OpStr, IloCplex::LongParam> lpdict_;
	std::unordered_map<OpStr, IloCplex::StringParam> spdict_;

public:
	void init();
	IloCplex::NumParam getNumParam(OpStr key);
	IloCplex::BoolParam getBoolParam(OpStr key);
	IloCplex::IntParam getIntParam(OpStr key);
	IloCplex::LongParam getLongParam(OpStr key);
	IloCplex::StringParam getStringParam(OpStr key);
	
public:
	OpCPXCfgCvt();
};

void Solver::OpCPXCfgCvt::init()
{
#ifdef OPUA_CPX_VERSION_128
	// CPX版本：12.8
	// 按字母顺序排列	
	// 加载其他参数
	ipdict_.emplace("OPUA.CPX.Advance", IloCplex::Param::Advance);
	// 加载Barrier参数
	ipdict_.emplace("OPUA.CPX.Barrier.Algorithm", IloCplex::Param::Barrier::Algorithm);
	ipdict_.emplace("OPUA.CPX.Barrier.ColNonzeros", IloCplex::Param::Barrier::ColNonzeros);
	npdict_.emplace("OPUA.CPX.Barrier.ConvergeTol", IloCplex::Param::Barrier::ConvergeTol);
	ipdict_.emplace("OPUA.CPX.Barrier.Crossover", IloCplex::Param::Barrier::Crossover);
	ipdict_.emplace("OPUA.CPX.Barrier.Display", IloCplex::Param::Barrier::Display);
	lpdict_.emplace("OPUA.CPX.Barrier.Limits.Corrections", IloCplex::Param::Barrier::Limits::Corrections);
	npdict_.emplace("OPUA.CPX.Barrier.Limits.Growth", IloCplex::Param::Barrier::Limits::Growth);
	lpdict_.emplace("OPUA.CPX.Barrier.Limits.Iteration", IloCplex::Param::Barrier::Limits::Iteration);
	npdict_.emplace("OPUA.CPX.Barrier.Limits.ObjRange", IloCplex::Param::Barrier::Limits::ObjRange);
	ipdict_.emplace("OPUA.CPX.Barrier.Ordering", IloCplex::Param::Barrier::Ordering);
	npdict_.emplace("OPUA.CPX.Barrier.QCPConvergeTol", IloCplex::Param::Barrier::QCPConvergeTol);
	ipdict_.emplace("OPUA.CPX.Barrier.StartAlg", IloCplex::Param::Barrier::StartAlg);
	// 加载其他参数
	ipdict_.emplace("OPUA.CPX.ClockType", IloCplex::Param::ClockType);
	// 加载Conflict参数
	ipdict_.emplace("OPUA.CPX.Conflict.Algorithm", IloCplex::Param::Conflict::Algorithm);
	ipdict_.emplace("OPUA.CPX.Conflict.Display", IloCplex::Param::Conflict::Display);
	// 加载其他参数
	spdict_.emplace("OPUA.CPX.CPUmask", IloCplex::Param::CPUmask);
	npdict_.emplace("OPUA.CPX.DetTimeLimit", IloCplex::Param::DetTimeLimit);
	// 加载DistMIP参数
	npdict_.emplace("OPUA.CPX.DistMIP.Rampup.DetTimeLimit", IloCplex::Param::DistMIP::Rampup::DetTimeLimit);
	ipdict_.emplace("OPUA.CPX.DistMIP.Rampup.Duration", IloCplex::Param::DistMIP::Rampup::Duration);
	npdict_.emplace("OPUA.CPX.DistMIP.Rampup.TimeLimit", IloCplex::Param::DistMIP::Rampup::TimeLimit);
	// 加载Emphasis参数
	bpdict_.emplace("OPUA.CPX.Emphasis.Memory", IloCplex::Param::Emphasis::Memory);
	ipdict_.emplace("OPUA.CPX.Emphasis.MIP", IloCplex::Param::Emphasis::MIP);
	bpdict_.emplace("OPUA.CPX.Emphasis.Numerical", IloCplex::Param::Emphasis::Numerical);
	// 加载Feasopt参数
	ipdict_.emplace("OPUA.CPX.Feasopt.Mode", IloCplex::Param::Feasopt::Mode);
	npdict_.emplace("OPUA.CPX.Feasopt.Tolerance", IloCplex::Param::Feasopt::Tolerance);
	// 加载MIP参数
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.BQP", IloCplex::Param::MIP::Cuts::BQP);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.Cliques", IloCplex::Param::MIP::Cuts::Cliques);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.Covers", IloCplex::Param::MIP::Cuts::Covers);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.Disjunctive", IloCplex::Param::MIP::Cuts::Disjunctive);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.FlowCovers", IloCplex::Param::MIP::Cuts::FlowCovers);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.Gomory", IloCplex::Param::MIP::Cuts::Gomory);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.GUBCovers", IloCplex::Param::MIP::Cuts::GUBCovers);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.Implied", IloCplex::Param::MIP::Cuts::Implied);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.LiftProj", IloCplex::Param::MIP::Cuts::LiftProj);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.LocalImplied", IloCplex::Param::MIP::Cuts::LocalImplied);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.MCFCut", IloCplex::Param::MIP::Cuts::MCFCut);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.MIRCut", IloCplex::Param::MIP::Cuts::MIRCut);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.PathCut", IloCplex::Param::MIP::Cuts::PathCut);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.RLT", IloCplex::Param::MIP::Cuts::RLT);
	ipdict_.emplace("OPUA.CPX.MIP.Cuts.ZeroHalfCut", IloCplex::Param::MIP::Cuts::ZeroHalfCut);
	ipdict_.emplace("OPUA.CPX.MIP.Display", IloCplex::Param::MIP::Display);
	lpdict_.emplace("OPUA.CPX.MIP.Interval", IloCplex::Param::MIP::Interval);
	ipdict_.emplace("OPUA.CPX.MIP.Limits.AggForCut", IloCplex::Param::MIP::Limits::AggForCut);
	ipdict_.emplace("OPUA.CPX.MIP.Limits.AuxRootThreads", IloCplex::Param::MIP::Limits::AuxRootThreads);
	lpdict_.emplace("OPUA.CPX.MIP.Limits.CutPasses", IloCplex::Param::MIP::Limits::CutPasses);
	npdict_.emplace("OPUA.CPX.MIP.Limits.CutsFactor", IloCplex::Param::MIP::Limits::CutsFactor);
	ipdict_.emplace("OPUA.CPX.MIP.Limits.EachCutLimit", IloCplex::Param::MIP::Limits::EachCutLimit);
	ipdict_.emplace("OPUA.CPX.MIP.Limits.GomoryCand", IloCplex::Param::MIP::Limits::GomoryCand);
	lpdict_.emplace("OPUA.CPX.MIP.Limits.GomoryPass", IloCplex::Param::MIP::Limits::GomoryPass);
	lpdict_.emplace("OPUA.CPX.MIP.Limits.Nodes", IloCplex::Param::MIP::Limits::Nodes);
	npdict_.emplace("OPUA.CPX.MIP.Limits.PolishTime", IloCplex::Param::MIP::Limits::PolishTime);
	ipdict_.emplace("OPUA.CPX.MIP.Limits.Populate", IloCplex::Param::MIP::Limits::Populate);
	npdict_.emplace("OPUA.CPX.MIP.Limits.ProbeDetTime", IloCplex::Param::MIP::Limits::ProbeDetTime);
	npdict_.emplace("OPUA.CPX.MIP.Limits.ProbeTime", IloCplex::Param::MIP::Limits::ProbeTime);
	lpdict_.emplace("OPUA.CPX.MIP.Limits.RepairTries", IloCplex::Param::MIP::Limits::RepairTries);
	lpdict_.emplace("OPUA.CPX.MIP.Limits.Solutions", IloCplex::Param::MIP::Limits::Solutions);
	ipdict_.emplace("OPUA.CPX.MIP.Limits.StrongCand", IloCplex::Param::MIP::Limits::StrongCand);
	lpdict_.emplace("OPUA.CPX.MIP.Limits.StrongIt", IloCplex::Param::MIP::Limits::StrongIt);
	npdict_.emplace("OPUA.CPX.MIP.Limits.TreeMemory", IloCplex::Param::MIP::Limits::TreeMemory);
	ipdict_.emplace("OPUA.CPX.MIP.OrderType", IloCplex::Param::MIP::OrderType);
	npdict_.emplace("OPUA.CPX.MIP.PolishAfter.AbsMIPGap", IloCplex::Param::MIP::PolishAfter::AbsMIPGap);
	npdict_.emplace("OPUA.CPX.MIP.PolishAfter.DetTime", IloCplex::Param::MIP::PolishAfter::DetTime);
	npdict_.emplace("OPUA.CPX.MIP.PolishAfter.MIPGap", IloCplex::Param::MIP::PolishAfter::MIPGap);
	lpdict_.emplace("OPUA.CPX.MIP.PolishAfter.Nodes", IloCplex::Param::MIP::PolishAfter::Nodes);
	lpdict_.emplace("OPUA.CPX.MIP.PolishAfter.Solutions", IloCplex::Param::MIP::PolishAfter::Solutions);
	npdict_.emplace("OPUA.CPX.MIP.PolishAfter.Time", IloCplex::Param::MIP::PolishAfter::Time);
	npdict_.emplace("OPUA.CPX.MIP.Pool.AbsGap", IloCplex::Param::MIP::Pool::AbsGap);
	ipdict_.emplace("OPUA.CPX.MIP.Pool.Capacity", IloCplex::Param::MIP::Pool::Capacity);
	ipdict_.emplace("OPUA.CPX.MIP.Pool.Intensity", IloCplex::Param::MIP::Pool::Intensity);
	npdict_.emplace("OPUA.CPX.MIP.Pool.RelGap", IloCplex::Param::MIP::Pool::RelGap);
	ipdict_.emplace("OPUA.CPX.MIP.Pool.Replace", IloCplex::Param::MIP::Pool::Replace);
	npdict_.emplace("OPUA.CPX.MIP.Strategy.Backtrack", IloCplex::Param::MIP::Strategy::Backtrack);
	lpdict_.emplace("OPUA.CPX.MIP.Strategy.BBInterval", IloCplex::Param::MIP::Strategy::BBInterval);
	ipdict_.emplace("OPUA.CPX.MIP.Strategy.Branch", IloCplex::Param::MIP::Strategy::Branch);
	ipdict_.emplace("OPUA.CPX.MIP.Strategy.Dive", IloCplex::Param::MIP::Strategy::Dive);
	ipdict_.emplace("OPUA.CPX.MIP.Strategy.File", IloCplex::Param::MIP::Strategy::File);
	ipdict_.emplace("OPUA.CPX.MIP.Strategy.FPHeur", IloCplex::Param::MIP::Strategy::FPHeur);
	npdict_.emplace("OPUA.CPX.MIP.Strategy.HeuristicEffort", IloCplex::Param::MIP::Strategy::HeuristicEffort);
	lpdict_.emplace("OPUA.CPX.MIP.Strategy.HeuristicFreq", IloCplex::Param::MIP::Strategy::HeuristicFreq);
	ipdict_.emplace("OPUA.CPX.MIP.Strategy.KappaStats", IloCplex::Param::MIP::Strategy::KappaStats);
	bpdict_.emplace("OPUA.CPX.MIP.Strategy.LBHeur", IloCplex::Param::MIP::Strategy::LBHeur);
	ipdict_.emplace("OPUA.CPX.MIP.Strategy.MIQCPStrat", IloCplex::Param::MIP::Strategy::MIQCPStrat);
	ipdict_.emplace("OPUA.CPX.MIP.Strategy.NodeSelect", IloCplex::Param::MIP::Strategy::NodeSelect);
	bpdict_.emplace("OPUA.CPX.MIP.Strategy.Order", IloCplex::Param::MIP::Strategy::Order);
	ipdict_.emplace("OPUA.CPX.MIP.Strategy.PresolveNode", IloCplex::Param::MIP::Strategy::PresolveNode);
	ipdict_.emplace("OPUA.CPX.MIP.Strategy.Probe", IloCplex::Param::MIP::Strategy::Probe);
	lpdict_.emplace("OPUA.CPX.MIP.Strategy.RINSHeur", IloCplex::Param::MIP::Strategy::RINSHeur);
	ipdict_.emplace("OPUA.CPX.MIP.Strategy.Search", IloCplex::Param::MIP::Strategy::Search);
	ipdict_.emplace("OPUA.CPX.MIP.Strategy.VariableSelect", IloCplex::Param::MIP::Strategy::VariableSelect);
	ipdict_.emplace("OPUA.CPX.MIP.SubMIP.StartAlg", IloCplex::Param::MIP::SubMIP::StartAlg);
	ipdict_.emplace("OPUA.CPX.MIP.SubMIP.SubAlg", IloCplex::Param::MIP::SubMIP::SubAlg);
	lpdict_.emplace("OPUA.CPX.MIP.SubMIP.NodeLimit", IloCplex::Param::MIP::SubMIP::NodeLimit);
	ipdict_.emplace("OPUA.CPX.MIP.SubMIP.Scale", IloCplex::Param::MIP::SubMIP::Scale);
	npdict_.emplace("OPUA.CPX.MIP.Tolerances.AbsMIPGap", IloCplex::Param::MIP::Tolerances::AbsMIPGap);
	npdict_.emplace("OPUA.CPX.MIP.Tolerances.Linearization", IloCplex::Param::MIP::Tolerances::Linearization);
	npdict_.emplace("OPUA.CPX.MIP.Tolerances.Integrality", IloCplex::Param::MIP::Tolerances::Integrality);
	npdict_.emplace("OPUA.CPX.MIP.Tolerances.LowerCutoff", IloCplex::Param::MIP::Tolerances::LowerCutoff);
	npdict_.emplace("OPUA.CPX.MIP.Tolerances.MIPGap", IloCplex::Param::MIP::Tolerances::MIPGap);
	npdict_.emplace("OPUA.CPX.MIP.Tolerances.ObjDifference", IloCplex::Param::MIP::Tolerances::ObjDifference);
	npdict_.emplace("OPUA.CPX.MIP.Tolerances.RelObjDifference", IloCplex::Param::MIP::Tolerances::RelObjDifference);
	npdict_.emplace("OPUA.CPX.MIP.Tolerances.UpperCutoff", IloCplex::Param::MIP::Tolerances::UpperCutoff);
	// 加载MultiObjective参数
	ipdict_.emplace("OPUA.CPX.MultiObjective.Display", IloCplex::Param::MultiObjective::Display);
	// 加载Network参数
	ipdict_.emplace("OPUA.CPX.Network.Display", IloCplex::Param::Network::Display);
	lpdict_.emplace("OPUA.CPX.Network.Iterations", IloCplex::Param::Network::Iterations);
	ipdict_.emplace("OPUA.CPX.Network.NetFind", IloCplex::Param::Network::NetFind);
	ipdict_.emplace("OPUA.CPX.Network.Pricing", IloCplex::Param::Network::Pricing);
	npdict_.emplace("OPUA.CPX.Network.Tolerances.Feasibility", IloCplex::Param::Network::Tolerances::Feasibility);
	npdict_.emplace("OPUA.CPX.Network.Tolerances.Optimality", IloCplex::Param::Network::Tolerances::Optimality);
	// 加载其他参数
	ipdict_.emplace("OPUA.CPX.NodeAlgorithm", IloCplex::Param::NodeAlgorithm);
	ipdict_.emplace("OPUA.CPX.OptimalityTarget", IloCplex::Param::OptimalityTarget);
	// 加载Output参数
	bpdict_.emplace("OPUA.CPX.Output.CloneLog", IloCplex::Param::Output::CloneLog);
	spdict_.emplace("OPUA.CPX.Output.IntSolFilePrefix", IloCplex::Param::Output::IntSolFilePrefix);
	bpdict_.emplace("OPUA.CPX.Output.MPSLong", IloCplex::Param::Output::MPSLong);
	ipdict_.emplace("OPUA.CPX.Output.WriteLevel", IloCplex::Param::Output::WriteLevel);
	// 加载其他参数
	ipdict_.emplace("OPUA.CPX.Parallel", IloCplex::Param::Parallel);
	bpdict_.emplace("OPUA.CPX.ParamDisplay", IloCplex::Param::ParamDisplay);
	// 加载Preprocessing参数
	ipdict_.emplace("OPUA.CPX.Preprocessing.Aggregator", IloCplex::Param::Preprocessing::Aggregator);
	ipdict_.emplace("OPUA.CPX.Preprocessing.BoundStrength", IloCplex::Param::Preprocessing::BoundStrength);
	ipdict_.emplace("OPUA.CPX.Preprocessing.CoeffReduce", IloCplex::Param::Preprocessing::CoeffReduce);
	ipdict_.emplace("OPUA.CPX.Preprocessing.Dependency", IloCplex::Param::Preprocessing::Dependency);
	ipdict_.emplace("OPUA.CPX.Preprocessing.Dual", IloCplex::Param::Preprocessing::Dual);
	lpdict_.emplace("OPUA.CPX.Preprocessing.Fill", IloCplex::Param::Preprocessing::Fill);
	ipdict_.emplace("OPUA.CPX.Preprocessing.Folding", IloCplex::Param::Preprocessing::Folding);
	ipdict_.emplace("OPUA.CPX.Preprocessing.Linear", IloCplex::Param::Preprocessing::Linear);
	ipdict_.emplace("OPUA.CPX.Preprocessing.NumPass", IloCplex::Param::Preprocessing::NumPass);
	bpdict_.emplace("OPUA.CPX.Preprocessing.Presolve", IloCplex::Param::Preprocessing::Presolve);
	ipdict_.emplace("OPUA.CPX.Preprocessing.QCPDuals", IloCplex::Param::Preprocessing::QCPDuals);
	bpdict_.emplace("OPUA.CPX.Preprocessing.QPMakePSD", IloCplex::Param::Preprocessing::QPMakePSD);
	ipdict_.emplace("OPUA.CPX.Preprocessing.QToLin", IloCplex::Param::Preprocessing::QToLin);
	ipdict_.emplace("OPUA.CPX.Preprocessing.Reduce", IloCplex::Param::Preprocessing::Reduce);
	ipdict_.emplace("OPUA.CPX.Preprocessing.Relax", IloCplex::Param::Preprocessing::Relax);
	ipdict_.emplace("OPUA.CPX.Preprocessing.RepeatPresolve", IloCplex::Param::Preprocessing::RepeatPresolve);
	ipdict_.emplace("OPUA.CPX.Preprocessing.Symmetry", IloCplex::Param::Preprocessing::Symmetry);
	// 加载其他参数
	ipdict_.emplace("OPUA.CPX.RandomSeed", IloCplex::Param::RandomSeed);
	// 加载Read参数
	spdict_.emplace("OPUA.CPX.Read.APIEncoding", IloCplex::Param::Read::APIEncoding);
	ipdict_.emplace("OPUA.CPX.Read.Constraints", IloCplex::Param::Read::Constraints);
	ipdict_.emplace("OPUA.CPX.Read.DataCheck", IloCplex::Param::Read::DataCheck);
	spdict_.emplace("OPUA.CPX.Read.FileEncoding", IloCplex::Param::Read::FileEncoding);
	lpdict_.emplace("OPUA.CPX.Read.Nonzeros", IloCplex::Param::Read::Nonzeros);
	lpdict_.emplace("OPUA.CPX.Read.QPNonzeros", IloCplex::Param::Read::QPNonzeros);
	ipdict_.emplace("OPUA.CPX.Read.Scale", IloCplex::Param::Read::Scale);
	ipdict_.emplace("OPUA.CPX.Read.Variables", IloCplex::Param::Read::Variables);
	lpdict_.emplace("OPUA.CPX.Read.WarningLimit", IloCplex::Param::Read::WarningLimit);
	// 加载其他参数
	bpdict_.emplace("OPUA.CPX.Record", IloCplex::Param::Record);
	ipdict_.emplace("OPUA.CPX.RootAlgorithm", IloCplex::Param::RootAlgorithm);
	// 加载Sifting参数
	ipdict_.emplace("OPUA.CPX.Sifting.Algorithm", IloCplex::Param::Sifting::Algorithm);
	bpdict_.emplace("OPUA.CPX.Sifting.Simplex", IloCplex::Param::Sifting::Simplex);
	ipdict_.emplace("OPUA.CPX.Sifting.Display", IloCplex::Param::Sifting::Display);
	lpdict_.emplace("OPUA.CPX.Sifting.Iterations", IloCplex::Param::Sifting::Iterations);
	// 加载Simplex参数
	ipdict_.emplace("OPUA.CPX.Simplex.Crash", IloCplex::Param::Simplex::Crash);
	ipdict_.emplace("OPUA.CPX.Simplex.Dgradient", IloCplex::Param::Simplex::DGradient);
	ipdict_.emplace("OPUA.CPX.Simplex.Display", IloCplex::Param::Simplex::Display);
	ipdict_.emplace("OPUA.CPX.Simplex.DynamicRows", IloCplex::Param::Simplex::DynamicRows);
	lpdict_.emplace("OPUA.CPX.Simplex.Limits.Iterations", IloCplex::Param::Simplex::Limits::Iterations);
	npdict_.emplace("OPUA.CPX.Simplex.Limits.LowerObj", IloCplex::Param::Simplex::Limits::LowerObj);
	ipdict_.emplace("OPUA.CPX.Simplex.Limits.Perturbation", IloCplex::Param::Simplex::Limits::Perturbation);
	ipdict_.emplace("OPUA.CPX.Simplex.Limits.Singularity", IloCplex::Param::Simplex::Limits::Singularity);
	npdict_.emplace("OPUA.CPX.Simplex.Limits.UpperObj", IloCplex::Param::Simplex::Limits::UpperObj);
	npdict_.emplace("OPUA.CPX.Simplex.Perturbation.Constant", IloCplex::Param::Simplex::Perturbation::Constant);
	bpdict_.emplace("OPUA.CPX.Simplex.Perturbation.Indicator", IloCplex::Param::Simplex::Perturbation::Indicator);
	ipdict_.emplace("OPUA.CPX.Simplex.PGradient", IloCplex::Param::Simplex::PGradient);
	ipdict_.emplace("OPUA.CPX.Simplex.Pricing", IloCplex::Param::Simplex::Pricing);
	ipdict_.emplace("OPUA.CPX.Simplex.Refactor", IloCplex::Param::Simplex::Refactor);
	npdict_.emplace("OPUA.CPX.Simplex.Tolerances.Feasibility", IloCplex::Param::Simplex::Tolerances::Feasibility);
	npdict_.emplace("OPUA.CPX.Simplex.Tolerances.Markowitz", IloCplex::Param::Simplex::Tolerances::Markowitz);
	npdict_.emplace("OPUA.CPX.Simplex.Tolerances.Optimality", IloCplex::Param::Simplex::Tolerances::Optimality);
	// 加载其他参数
	ipdict_.emplace("OPUA.CPX.SolutionType", IloCplex::Param::SolutionType);
	ipdict_.emplace("OPUA.CPX.Threads", IloCplex::Param::Threads);
	npdict_.emplace("OPUA.CPX.TimeLimit", IloCplex::Param::TimeLimit);
	// 加载Tune参数
	npdict_.emplace("OPUA.CPX.Tune.DetTimeLimit", IloCplex::Param::Tune::DetTimeLimit);
	ipdict_.emplace("OPUA.CPX.Tune.Display", IloCplex::Param::Tune::Display);
	ipdict_.emplace("OPUA.CPX.Tune.Measure", IloCplex::Param::Tune::Measure);
	ipdict_.emplace("OPUA.CPX.Tune.Repeat", IloCplex::Param::Tune::Repeat);
	npdict_.emplace("OPUA.CPX.Tune.TimeLimit", IloCplex::Param::Tune::TimeLimit);
	// 加载其他参数
	spdict_.emplace("OPUA.CPX.WorkDir", IloCplex::Param::WorkDir);
	npdict_.emplace("OPUA.CPX.WorkMem", IloCplex::Param::WorkMem);
#endif
}

IloCplex::NumParam Solver::OpCPXCfgCvt::getNumParam(OpStr key)
{
	return npdict_.at(key);
}

IloCplex::BoolParam Solver::OpCPXCfgCvt::getBoolParam(OpStr key)
{
	return bpdict_.at(key);
}

IloCplex::IntParam Solver::OpCPXCfgCvt::getIntParam(OpStr key)
{
	return ipdict_.at(key);
}

IloCplex::LongParam Solver::OpCPXCfgCvt::getLongParam(OpStr key)
{
	return lpdict_.at(key);
}

IloCplex::StringParam Solver::OpCPXCfgCvt::getStringParam(OpStr key)
{
	return spdict_.at(key);
}

Solver::OpCPXCfgCvt::OpCPXCfgCvt()
{
	init();
}

/* OPUA::Solver::OpCPXSolI */

class Solver::OpCPXSolI
	: public OpImplBase
{
protected:
	// CPX组件模型

	IloEnv cenv_; // CPX环境变量
	IloModel cmdl_; // CPX模型对象
	IloCplex csol_; // CPX求解器对象
protected:
	// OPUA-CPX映射信息

	std::unordered_map<OpLInt, IloNumVar> vardict_; // CPX变量表
	std::unordered_map<OpLInt, IloRange> lcdict_; // CPX线性约束表
	std::unordered_map<OpLInt, IloRange> qcdict_; // CPX二次约束表
	std::unordered_map<OpLInt, IloConstraint> scdict_; // CPXSOS约束表
	std::unordered_map<OpLInt, IloConstraint> nlcdict_; // CPX非线性约束表
	std::unordered_map<OpLInt, IloObjective> objdict_; // CPX优化目标函数表
	OpCPXCfgCvt cfgcvt_; // 配置翻译器

	friend class OpCPXSol;
private:
	IloNumVar::Type typeConvert(Variable::OpVarType type); // 将OPUA变量类型转换为CPX变量类型
	IloNumVar addCPXVar(Variable::OpVar var); // 从OPUA变量创建一个CPX变量
	IloNumExpr addCPXLE(const Expression::OpLinExpr& expr); // 由OPUA线性表达式创建一个CPX线性表达式
	IloNumExpr addCPXQE(const Expression::OpQuadExpr& expr); // 由OPUA二次表达式创建一个CPX二次表达式
	IloRange addCPXLC(Constraint::OpLinCon con); // 从OPUA线性约束创建一个CPX约束
	IloRange addCPXQC(Constraint::OpQuadCon con); // 从OPUA二次约束创建一个CPX约束
	IloConstraint addCPXSOS(Constraint::OpSOSCon con); // 从OPUASOS约束创建一个CPX约束
	IloConstraint addCPXNLC(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXSum(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXAbs(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXMax(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXMin(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXSquare(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXSqrt(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXPower(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXExp(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXLog(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个CPX约束
	IloConstraint addCPXIfThen(Constraint::OpCdtCon con); // 从OPUA条件约束中创建一个CPX约束
	IloObjective addCPXObj(Objective::OpObj obj); // 从OPUA目标函数创建一个CPX目标函数
protected:
	void init(); // 初始化
	void clear(); // 清除所有组件与映射信息
	void extract(Model::OpModel mdl); // 抽取OPUA模型
	void solve(); // 求解模型
	void solveFixed(); // 固定整数变量解并执行求解
	void setParam(const OpConfig& cfg); // 设置配置
	OpLInt getStatus() const; // 获取求解状态
	OpFloat getObjValue() const; // 获取目标函数解
	OpFloat getValue(Variable::OpVar var) const; // 获取变量的解
	OpFloat getValue(const Expression::OpLinExpr& expr) const; // 获取表达式的解(速度较慢)
	OpFloat getValue(const Expression::OpQuadExpr& expr) const; // 获取表达式的解(速度较慢)
	OpFloat getValue(Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
	OpFloat getDual(Constraint::OpLinCon con) const; // 获取对偶解
	void write(OpStr path) const; // 将模型写入文件
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
protected:
	OpCPXSolI(OpEnvI* env);
	OpCPXSolI(OpEnvI* env, Model::OpModel mdl);
public:
	virtual ~OpCPXSolI();
};

IloNumVar::Type Solver::OpCPXSolI::typeConvert(Variable::OpVarType type)
{
	auto tmp(IloNumVar::Type::Float);
	switch (type)
	{
	case Variable::OpVarType::Bool:
		tmp = IloNumVar::Type::Bool;
		break;
	case Variable::OpVarType::Int:
		tmp = IloNumVar::Type::Int;
		break;
	case Variable::OpVarType::Con:
		tmp = IloNumVar::Type::Float;
		break;
	case Variable::OpVarType::Sem:
		throw OpExcBase("[Solver::OpCPXSolI::typeConvert]: Exception->can not handle Variable::OpVarType::Sem");
		break;
	default:
		throw OpExcBase("[Solver::OpCPXSolI::typeConvert]: Exception->can not handle other variable type");
		break;
	}
	return tmp;
}

IloNumVar Solver::OpCPXSolI::addCPXVar(Variable::OpVar var)
{
	IloNum lb(Constant::IsInfinity(var.getLb()) ? -IloInfinity : var.getLb());
	IloNum ub(Constant::IsInfinity(var.getUb()) ? IloInfinity : var.getUb());
	return IloNumVar(cenv_, lb, ub, typeConvert(var.getType()), var.getName().c_str());
}

IloNumExpr Solver::OpCPXSolI::addCPXLE(const Expression::OpLinExpr& expr)
{
	IloNumExpr tmp(cenv_, expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		tmp += iter.getCoeff() * vardict_.at(iter.getVar().getIndex());
	return tmp;
}

IloNumExpr Solver::OpCPXSolI::addCPXQE(const Expression::OpQuadExpr& expr)
{
	IloNumExpr tmp(cenv_, expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		tmp += iter.getCoeff() * vardict_.at(iter.getVar().getIndex());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		tmp += iter.getCoeff() * vardict_.at(iter.getVar1().getIndex()) * vardict_.at(iter.getVar2().getIndex());
	return tmp;
}

IloRange Solver::OpCPXSolI::addCPXLC(Constraint::OpLinCon con)
{
	IloRange tmp(nullptr);
	IloNum lb(Constant::IsInfinity(con.getLb()) ? -IloInfinity : con.getLb());
	IloNum ub(Constant::IsInfinity(con.getUb()) ? IloInfinity : con.getUb());
	IloNumExpr expr(addCPXLE(con.getExpr()));
	tmp = IloRange(cenv_, lb, expr, ub, con.getName().c_str());
	expr.end();
	return tmp;
}

IloRange Solver::OpCPXSolI::addCPXQC(Constraint::OpQuadCon con)
{
	IloRange tmp(nullptr);
	IloNum lb(Constant::IsInfinity(con.getLb()) ? -IloInfinity : con.getLb());
	IloNum ub(Constant::IsInfinity(con.getUb()) ? IloInfinity : con.getUb());
	IloNumExpr expr(addCPXQE(con.getExpr()));
	tmp = IloRange(cenv_, lb, expr, ub, con.getName().c_str());
	expr.end();
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXSOS(Constraint::OpSOSCon con)
{
	IloConstraint tmp(nullptr);
	auto& expr(con.getSOSExpr());
	IloNumVarArray vars(cenv_);
	IloNumArray weights(cenv_);
	OpAutoFree guard([&]() { vars.end(); weights.end(); });
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
	{
		weights.add(iter.getCoeff());
		vars.add(vardict_.at(iter.getVar().getIndex()));
	}
	switch (con.getSense())
	{
	case Constraint::OpConSense::SOS1:
		tmp = IloSOS1(cenv_, vars, weights, con.getName().c_str());
		break;
	case Constraint::OpConSense::SOS2:
		tmp = IloSOS2(cenv_, vars, weights, con.getName().c_str());
		break;
	default:
		throw OpExcBase("[Solver::OpCPXSolI::addCPXSOS]: Exception->can not handle other sos sense");
		break;
	}
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXNLC(Constraint::OpNLCon con)
{
	IloConstraint tmp(nullptr);
	switch (con.getFunction())
	{
	case Expression::OpNLFunc::Unknow: // 无法处理
		throw OpExcBase("[Solver::OpCPXSolI::addCPXNLC]: Exception->can not handle Expression::OpNLFunc::Unknow");
		break;
	case Expression::OpNLFunc::Sum:
		tmp = addCPXSum(con);
		break;
	case Expression::OpNLFunc::Abs:
		tmp = addCPXAbs(con);
		break;
	case Expression::OpNLFunc::Max:
		tmp = addCPXMax(con);
		break;
	case Expression::OpNLFunc::Min:
		tmp = addCPXMin(con);
		break;
	case Expression::OpNLFunc::Square:
		tmp = addCPXSquare(con);
		break;
	case Expression::OpNLFunc::Sqrt:
		tmp = addCPXSqrt(con);
		break;
	case Expression::OpNLFunc::Pow:
		tmp = addCPXPower(con);
		break;
	case Expression::OpNLFunc::Exp1:
		tmp = addCPXExp(con);
		break;
	case Expression::OpNLFunc::Exp2: // 无法处理
		throw OpExcBase("[Solver::OpCPXSolI::addCPXNLC]: Exception->can not handle Expression::OpNLFunc::Exp2");
		break;
	case Expression::OpNLFunc::Log1: // 无法处理
		throw OpExcBase("[Solver::OpCPXSolI::addCPXNLC]: Exception->can not handle Expression::OpNLFunc::Log1");
		break;
	case Expression::OpNLFunc::Log2:
		tmp = addCPXLog(con);
		break;
	case Expression::OpNLFunc::Log3: // 无法处理
		throw OpExcBase("[Solver::OpCPXSolI::addCPXNLC]: Exception->can not handle Expression::OpNLFunc::Log3");
		break;
	case Expression::OpNLFunc::Sin: // 无法处理
		throw OpExcBase("[Solver::OpCPXSolI::addCPXNLC]: Exception->can not handle Expression::OpNLFunc::Sin");
		break;
	case Expression::OpNLFunc::Cos: // 无法处理
		throw OpExcBase("[Solver::OpCPXSolI::addCPXNLC]: Exception->can not handle Expression::OpNLFunc::Cos");
		break;
	case Expression::OpNLFunc::Tan: // 无法处理
		throw OpExcBase("[Solver::OpCPXSolI::addCPXNLC]: Exception->can not handle Expression::OpNLFunc::Tan");
		break;
	default:
		throw OpExcBase("[Solver::OpCPXSolI::addCPXNLC]: Exception->can not handle other NL function");
		break;
	}
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXSum(Constraint::OpNLCon con)
{
	IloConstraint tmp(nullptr);
	IloNumExpr rhs(cenv_);
	auto& expr(con.getExpr());
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
		rhs += vardict_.at(iter.getVar().getIndex());
	rhs += expr.getParam();
	tmp = vardict_.at(con.getVar().getIndex()) == rhs;
	tmp.setName(con.getName().c_str());
	rhs.end();
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXAbs(Constraint::OpNLCon con)
{
	IloConstraint tmp(vardict_.at(con.getVar().getIndex()) == IloAbs(vardict_.at(con.getExpr().getVar(0).getIndex())));
	tmp.setName(con.getName().c_str());
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXMax(Constraint::OpNLCon con)
{
	IloConstraint tmp(nullptr);
	IloNumExprArray vars(cenv_);
	auto& expr(con.getExpr());
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
		vars.add(vardict_.at(iter.getVar().getIndex()));
	vars.add(IloNumExpr(cenv_, expr.getParam()));
	tmp = vardict_.at(con.getVar().getIndex()) == IloMax(vars);
	tmp.setName(con.getName().c_str());
	vars.endElements();
	vars.end();
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXMin(Constraint::OpNLCon con)
{
	IloConstraint tmp(nullptr);
	IloNumExprArray vars(cenv_);
	auto& expr(con.getExpr());
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
		vars.add(vardict_.at(iter.getVar().getIndex()));
	vars.add(IloNumExpr(cenv_, expr.getParam()));
	tmp = vardict_.at(con.getVar().getIndex()) == IloMin(vars);
	tmp.setName(con.getName().c_str());
	vars.endElements();
	vars.end();
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXSquare(Constraint::OpNLCon con)
{
	IloConstraint tmp(vardict_.at(con.getVar().getIndex()) == IloSquare(vardict_.at(con.getExpr().getVar(0).getIndex())));
	tmp.setName(con.getName().c_str());
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXSqrt(Constraint::OpNLCon con)
{
	IloConstraint tmp(vardict_.at(con.getVar().getIndex()) == IloPower(vardict_.at(con.getExpr().getVar(0).getIndex()), 0.5));
	tmp.setName(con.getName().c_str());
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXPower(Constraint::OpNLCon con)
{
	auto& expr(con.getExpr());
	IloConstraint tmp(vardict_.at(con.getVar().getIndex()) == IloPower(vardict_.at(expr.getVar(0).getIndex()), expr.getParam()));
	tmp.setName(con.getName().c_str());
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXExp(Constraint::OpNLCon con)
{
	IloConstraint tmp(vardict_.at(con.getVar().getIndex()) == IloExponent(vardict_.at(con.getExpr().getVar(0).getIndex())));
	tmp.setName(con.getName().c_str());
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXLog(Constraint::OpNLCon con)
{
	IloConstraint tmp(vardict_.at(con.getVar().getIndex()) == IloLog(vardict_.at(con.getExpr().getVar(0).getIndex())));
	tmp.setName(con.getName().c_str());
	return tmp;
}

IloConstraint Solver::OpCPXSolI::addCPXIfThen(Constraint::OpCdtCon con)
{
	IloIfThen tmp(nullptr);
	if (con.isIndicator())
		tmp = IloIfThen(cenv_, vardict_.at(con.getVar().getIndex()) == 1, addCPXLC(con.getCon(false)), con.getName().c_str());
	else
		tmp = IloIfThen(cenv_, addCPXLC(con.getCon(true)), addCPXLC(con.getCon(false)), con.getName().c_str());
	return tmp;
}

IloObjective Solver::OpCPXSolI::addCPXObj(Objective::OpObj obj)
{
	IloObjective tmp(nullptr);
	IloNumExpr lins(addCPXLE(obj.getLinExpr()));
	IloNumExpr quads(addCPXQE(obj.getQuadExpr()));
	OpAutoFree guard([&]() { lins.end(); quads.end(); });
	switch (obj.getSense())
	{
	case Objective::OpObjSense::Min:
		tmp = IloMinimize(cenv_, lins + quads);
		break;
	case Objective::OpObjSense::Max:
		tmp = IloMaximize(cenv_, lins + quads);
		break;
	default:
		throw OpExcBase("[Solver::OpCPXSolI::addCPXObj]: Exception->can not handle other objective function sense");
		break;
	}
	tmp.setName(obj.getName().c_str());
	return tmp;
}

void Solver::OpCPXSolI::init()
{
	cenv_ = IloEnv();
	cmdl_ = IloModel(cenv_);
}

void Solver::OpCPXSolI::clear()
{
	vardict_.clear();
	lcdict_.clear();
	qcdict_.clear();
	scdict_.clear();
	nlcdict_.clear();
	objdict_.clear();
	if (csol_.getImpl())
		csol_.end();
	if (cmdl_.getImpl())
		cmdl_.end();
	if (cenv_.getImpl())
		cenv_.end();
}

void Solver::OpCPXSolI::extract(Model::OpModel mdl)
{
	try
	{
		// 首先清除原模型
		clear();
		init();
		// 加载现有模型
		for (auto iter = mdl.getCBegin<Variable::OpVar>(); iter != mdl.getCEnd<Variable::OpVar>(); ++iter)
			vardict_.emplace(iter.getKey(), addCPXVar(iter.getVal()));
		for (auto iter = mdl.getCBegin<Variable::OpPSDVar>(); iter != mdl.getCEnd<Variable::OpPSDVar>(); ++iter)
			throw OpExcBase("[Solver::OpCPXSolI::extract]: Exception->can not handle Variable::OpPSDVar");
		for (auto iter = mdl.getCBegin<Constraint::OpLinCon>(); iter != mdl.getCEnd<Constraint::OpLinCon>(); ++iter)
			lcdict_.emplace(iter.getKey(), addCPXLC(iter.getVal()));
		for (auto iter = mdl.getCBegin<Constraint::OpQuadCon>(); iter != mdl.getCEnd<Constraint::OpQuadCon>(); ++iter)
			qcdict_.emplace(iter.getKey(), addCPXQC(iter.getVal()));
		for (auto iter = mdl.getCBegin<Constraint::OpConicCon>(); iter != mdl.getCEnd<Constraint::OpConicCon>(); ++iter)
			throw OpExcBase("[Solver::OpCPXSolI::extract]: Exception->can not handle Constraint::OpConicCon");
		for (auto iter = mdl.getCBegin<Constraint::OpPSDCon>(); iter != mdl.getCEnd<Constraint::OpPSDCon>(); ++iter)
			throw OpExcBase("[Solver::OpCPXSolI::extract]: Exception->can not handle Constraint::OpPSDCon");
		for (auto iter = mdl.getCBegin<Constraint::OpSOSCon>(); iter != mdl.getCEnd<Constraint::OpSOSCon>(); ++iter)
			scdict_.emplace(iter.getKey(), addCPXSOS(iter.getVal()));
		for (auto iter = mdl.getCBegin<Constraint::OpNLCon>(); iter != mdl.getCEnd<Constraint::OpNLCon>(); ++iter)
			nlcdict_.emplace(iter.getKey(), addCPXNLC(iter.getVal()));
		for (auto iter = mdl.getCBegin<Constraint::OpCdtCon>(); iter != mdl.getCEnd<Constraint::OpCdtCon>(); ++iter)
			nlcdict_.emplace(iter.getKey(), addCPXIfThen(iter.getVal()));
		objdict_.emplace(mdl.getObj().getIndex(), addCPXObj(mdl.getObj()));

		for (auto& lc : lcdict_)
			cmdl_.add(lc.second);
		for (auto& qc : qcdict_)
			cmdl_.add(qc.second);
		for (auto& sc : scdict_)
			cmdl_.add(sc.second);
		for (auto& nlc : nlcdict_)
			cmdl_.add(nlc.second);
		for (auto& o : objdict_)
			cmdl_.add(o.second);
	}
	catch (OpExcBase& e)
	{
		clear();
		throw e;
	}
	catch (IloException& e)
	{
		clear();
		throw e;
	}
	catch (std::exception& e)
	{
		clear();
		throw e;
	}
	catch (...)
	{
		clear();
		throw OpExcBase("[Solver::OpCPXSolI::extract]: Exception->unknow exception!");
	}
}

void Solver::OpCPXSolI::solve()
{
	if (!csol_.getImpl())
		csol_ = IloCplex(cmdl_);
	csol_.solve();
}

void Solver::OpCPXSolI::solveFixed()
{
	csol_.solveFixed();
}

void Solver::OpCPXSolI::setParam(const OpConfig& cfg)
{
	if (csol_.getImpl())
	{
		for (auto iter = cfg.getCBegin<OpBool>("OPUA.CPX"); iter != cfg.getCEnd<OpBool>("OPUA.CPX"); ++iter)
			if (iter.ok())
				csol_.setParam(cfgcvt_.getBoolParam(iter.getKey()), iter.getVal());
		for (auto iter = cfg.getCBegin<OpInt>("OPUA.CPX"); iter != cfg.getCEnd<OpInt>("OPUA.CPX"); ++iter)
			if (iter.ok())
				csol_.setParam(cfgcvt_.getIntParam(iter.getKey()), iter.getVal());
		for (auto iter = cfg.getCBegin<OpLInt>("OPUA.CPX"); iter != cfg.getCEnd<OpLInt>("OPUA.CPX"); ++iter)
			if (iter.ok())
				csol_.setParam(cfgcvt_.getLongParam(iter.getKey()), iter.getVal());
		for (auto iter = cfg.getCBegin<OpFloat>("OPUA.CPX"); iter != cfg.getCEnd<OpFloat>("OPUA.CPX"); ++iter)
			if (iter.ok())
				csol_.setParam(cfgcvt_.getNumParam(iter.getKey()), iter.getVal());
		for (auto iter = cfg.getCBegin<OpStr>("OPUA.CPX"); iter != cfg.getCEnd<OpStr>("OPUA.CPX"); ++iter)
			if (iter.ok())
				csol_.setParam(cfgcvt_.getStringParam(iter.getKey()), iter.getVal().c_str());
	}
}

OpLInt Solver::OpCPXSolI::getStatus() const
{
	return csol_.getStatus();
}

OpFloat Solver::OpCPXSolI::getObjValue() const
{
	return csol_.getObjValue();
}

OpFloat Solver::OpCPXSolI::getValue(Variable::OpVar var) const
{
	return csol_.getValue(vardict_.at(var.getIndex()));
}

OpFloat Solver::OpCPXSolI::getValue(const Expression::OpLinExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	return result;
}

OpFloat Solver::OpCPXSolI::getValue(const Expression::OpQuadExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar1()) * getValue(iter.getVar2());
	return result;
}

OpFloat Solver::OpCPXSolI::getValue(Objective::OpObj obj) const
{
	return getValue(obj.getLinExpr()) + getValue(obj.getQuadExpr());
}

OpFloat Solver::OpCPXSolI::getDual(Constraint::OpLinCon con) const
{
	return csol_.getDual(lcdict_.at(con.getIndex()));
}

void Solver::OpCPXSolI::write(OpStr path) const
{
	if (path.size())
	{
		csol_.exportModel(path.c_str());
	}
	else
	{	
		// 无法输出到控制台
	}
}

OpULInt Solver::OpCPXSolI::getMemoryUsage() const
{
	return sizeof(*this);
}

Solver::OpCPXSolI::OpCPXSolI(OpEnvI* env)
	: OpImplBase('S', env),
	cenv_(nullptr),
	cmdl_(nullptr), 
	csol_(nullptr)
{

}

Solver::OpCPXSolI::OpCPXSolI(OpEnvI* env, Model::OpModel mdl)
	: OpImplBase('S', env),
	cenv_(nullptr),
	cmdl_(nullptr),
	csol_(nullptr)
{
	extract(mdl);
}

Solver::OpCPXSolI::~OpCPXSolI()
{
	clear();
}

/* OPUA::Solver::OpCPXSol */

void Solver::OpCPXSol::extract(Model::OpModel mdl)
{
	static_cast<OpCPXSolI*>(impl_)->extract(mdl);
}

void Solver::OpCPXSol::solve()
{
	static_cast<OpCPXSolI*>(impl_)->solve();
}

void Solver::OpCPXSol::solveFixed()
{
	static_cast<OpCPXSolI*>(impl_)->solveFixed();
}

void Solver::OpCPXSol::setParam(const OpConfig& cfg)
{
	static_cast<OpCPXSolI*>(impl_)->setParam(cfg);
}

OpLInt Solver::OpCPXSol::getStatus() const
{
	return static_cast<OpCPXSolI*>(impl_)->getStatus();
}

OpFloat Solver::OpCPXSol::getObjValue() const
{
	return static_cast<OpCPXSolI*>(impl_)->getObjValue();
}

OpFloat Solver::OpCPXSol::getValue(Variable::OpVar var) const
{
	return static_cast<OpCPXSolI*>(impl_)->getValue(var);
}

OpFloat Solver::OpCPXSol::getValue(const Expression::OpLinExpr& expr) const
{
	return static_cast<OpCPXSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpCPXSol::getValue(const Expression::OpQuadExpr& expr) const
{
	return static_cast<OpCPXSolI*>(impl_)->getValue(expr);
}

OpFloat Solver::OpCPXSol::getValue(Objective::OpObj obj) const
{
	return static_cast<OpCPXSolI*>(impl_)->getValue(obj);
}

OpFloat Solver::OpCPXSol::getDual(Constraint::OpLinCon con) const
{
	return static_cast<OpCPXSolI*>(impl_)->getDual(con);
}

Solver::OpCPXSolI* Solver::OpCPXSol::getImpl() const
{
	return static_cast<OpCPXSolI*>(impl_);
}

void Solver::OpCPXSol::write(OpStr path) const
{
	static_cast<OpCPXSolI*>(impl_)->write(path);
}

void Solver::OpCPXSol::release0()
{
	static_cast<OpCPXSolI*>(impl_)->release();
}

OpBool Solver::OpCPXSol::operator==(const OpCPXSol& sol) const
{
	return impl_ == sol.impl_;
}

OpBool Solver::OpCPXSol::operator!=(const OpCPXSol& sol) const
{
	return impl_ != sol.impl_;
}

Solver::OpCPXSol::OpCPXSol()
{

}

Solver::OpCPXSol::OpCPXSol(OpCPXSolI* impl)
{
	impl_ = impl;
}

Solver::OpCPXSol::OpCPXSol(OpEnv env)
{
	impl_ = new OpCPXSolI(env.getImpl());
}

Solver::OpCPXSol::OpCPXSol(OpEnv env, Model::OpModel mdl)
{
	impl_ = new OpCPXSolI(env.getImpl(), mdl);
}

Solver::OpCPXSol::~OpCPXSol()
{

}
#endif
