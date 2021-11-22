#include "OPUA.Solver.h"
#include <Gurobi/include/gurobi_c++.h>

using namespace OPUA;

/* OPUA::Solver::OpGRBCfgCvt */

// GRB配置翻译器
class Solver::OpGRBCfgCvt
{
protected:
	std::unordered_map<OpStr, GRB_DoubleParam> dpdict_;
	std::unordered_map<OpStr, GRB_IntParam> ipdict_;
	std::unordered_map<OpStr, GRB_StringParam> spdict_;
public:
	void init();
	GRB_DoubleParam getDoubleParam(OpStr key);
	GRB_IntParam getIntParam(OpStr key);
	GRB_StringParam getStringParam(OpStr key);
public:
	OpGRBCfgCvt();
};

void Solver::OpGRBCfgCvt::init()
{
	// 加载OPUA.GRB.Termination(终止条件)
	ipdict_.emplace("OPUA.GRB.Termination.BarIterLimit", GRB_IntParam_BarIterLimit);
	dpdict_.emplace("OPUA.GRB.Termination.BestBdStop", GRB_DoubleParam_BestBdStop);
	dpdict_.emplace("OPUA.GRB.Termination.BestObjStop", GRB_DoubleParam_BestObjStop);
	dpdict_.emplace("OPUA.GRB.Termination.Cutoff", GRB_DoubleParam_Cutoff);
	dpdict_.emplace("OPUA.GRB.Termination.IterationLimit", GRB_DoubleParam_IterationLimit);
	dpdict_.emplace("OPUA.GRB.Termination.NodeLimit", GRB_DoubleParam_NodeLimit);
	ipdict_.emplace("OPUA.GRB.Termination.SolutionLimit", GRB_IntParam_SolutionLimit);
	dpdict_.emplace("OPUA.GRB.Termination.TimeLimit", GRB_DoubleParam_TimeLimit);
	// 加载OPUA.GRB.Tolerances(可行性 & 最优性收敛判定参数)
	dpdict_.emplace("OPUA.GRB.Tolerances.BarConvTol", GRB_DoubleParam_BarConvTol);
	dpdict_.emplace("OPUA.GRB.Tolerances.BarQCPConvTol", GRB_DoubleParam_BarQCPConvTol);
	dpdict_.emplace("OPUA.GRB.Tolerances.FeasibilityTol", GRB_DoubleParam_FeasibilityTol);
	dpdict_.emplace("OPUA.GRB.Tolerances.IntFeasTol", GRB_DoubleParam_IntFeasTol);
	dpdict_.emplace("OPUA.GRB.Tolerances.MarkowitzTol", GRB_DoubleParam_MarkowitzTol);
	dpdict_.emplace("OPUA.GRB.Tolerances.MIPGap", GRB_DoubleParam_MIPGap);
	dpdict_.emplace("OPUA.GRB.Tolerances.MIPGapAbs", GRB_DoubleParam_MIPGapAbs);
	dpdict_.emplace("OPUA.GRB.Tolerances.OptimalityTol", GRB_DoubleParam_OptimalityTol);
	dpdict_.emplace("OPUA.GRB.Tolerances.PSDTol", GRB_DoubleParam_PSDTol);
	// 加载OPUA.GRB.Simplex(单纯形法参数)
	ipdict_.emplace("OPUA.GRB.Simplex.InfUnbdInfo", GRB_IntParam_InfUnbdInfo);
	ipdict_.emplace("OPUA.GRB.Simplex.NormAdjust", GRB_IntParam_NormAdjust);
	dpdict_.emplace("OPUA.GRB.Simplex.ObjScale", GRB_DoubleParam_ObjScale);
	dpdict_.emplace("OPUA.GRB.Simplex.PerturbValue", GRB_DoubleParam_PerturbValue);
	ipdict_.emplace("OPUA.GRB.Simplex.Quad", GRB_IntParam_Quad);
	ipdict_.emplace("OPUA.GRB.Simplex.ScaleFlag", GRB_IntParam_ScaleFlag);
	ipdict_.emplace("OPUA.GRB.Simplex.Sifting", GRB_IntParam_Sifting);
	ipdict_.emplace("OPUA.GRB.Simplex.SiftMethod", GRB_IntParam_SiftMethod);
	ipdict_.emplace("OPUA.GRB.Simplex.SimplexPricing", GRB_IntParam_SimplexPricing);
	// 加载OPUA.GRB.Barrier(内点法参数)
	ipdict_.emplace("OPUA.GRB.Barrier.BarCorrectors", GRB_IntParam_BarCorrectors);
	ipdict_.emplace("OPUA.GRB.Barrier.BarHomogeneous", GRB_IntParam_BarHomogeneous);
	ipdict_.emplace("OPUA.GRB.Barrier.BarOrder", GRB_IntParam_BarOrder);
	ipdict_.emplace("OPUA.GRB.Barrier.Crossover", GRB_IntParam_Crossover);
	ipdict_.emplace("OPUA.GRB.Barrier.CrossoverBasis", GRB_IntParam_CrossoverBasis);
	ipdict_.emplace("OPUA.GRB.Barrier.QCPDual", GRB_IntParam_QCPDual);
	// 加载OPUA.GRB.MIP(混合整数求解参数)
	ipdict_.emplace("OPUA.GRB.MIP.BranchDir", GRB_IntParam_BranchDir);
	ipdict_.emplace("OPUA.GRB.MIP.ConcurrentJobs", GRB_IntParam_ConcurrentJobs);
	ipdict_.emplace("OPUA.GRB.MIP.ConcurrentMIP", GRB_IntParam_ConcurrentMIP);
	ipdict_.emplace("OPUA.GRB.MIP.DegenMoves", GRB_IntParam_DegenMoves);
	ipdict_.emplace("OPUA.GRB.MIP.Disconnected", GRB_IntParam_Disconnected);
	ipdict_.emplace("OPUA.GRB.MIP.DistributedMIPJobs", GRB_IntParam_DistributedMIPJobs);
	dpdict_.emplace("OPUA.GRB.MIP.Heuristics", GRB_DoubleParam_Heuristics);
	dpdict_.emplace("OPUA.GRB.MIP.ImproveStartGap", GRB_DoubleParam_ImproveStartGap);
	dpdict_.emplace("OPUA.GRB.MIP.ImproveStartNodes", GRB_DoubleParam_ImproveStartNodes);
	dpdict_.emplace("OPUA.GRB.MIP.ImproveStartTime", GRB_DoubleParam_ImproveStartTime);
	ipdict_.emplace("OPUA.GRB.MIP.LazyConstraints", GRB_IntParam_LazyConstraints);
	ipdict_.emplace("OPUA.GRB.MIP.MinRelNodes", GRB_IntParam_MinRelNodes);
	ipdict_.emplace("OPUA.GRB.MIP.MIPFocus", GRB_IntParam_MIPFocus);
	ipdict_.emplace("OPUA.GRB.MIP.MIQCPMethod", GRB_IntParam_MIQCPMethod);
	spdict_.emplace("OPUA.GRB.MIP.NodefileDir", GRB_StringParam_NodefileDir);
	dpdict_.emplace("OPUA.GRB.MIP.NodefileStart", GRB_DoubleParam_NodefileStart);
	ipdict_.emplace("OPUA.GRB.MIP.NodeMethod", GRB_IntParam_NodeMethod);
	ipdict_.emplace("OPUA.GRB.MIP.NonConvex", GRB_IntParam_NonConvex);
	dpdict_.emplace("OPUA.GRB.MIP.NoRelHeurTime", GRB_DoubleParam_NoRelHeurTime);
	dpdict_.emplace("OPUA.GRB.MIP.NoRelHeurWork", GRB_DoubleParam_NoRelHeurWork);
	ipdict_.emplace("OPUA.GRB.MIP.PartitionPlace", GRB_IntParam_PartitionPlace);
	ipdict_.emplace("OPUA.GRB.MIP.PumpPassese", GRB_IntParam_PumpPasses);
	ipdict_.emplace("OPUA.GRB.MIP.RINS", GRB_IntParam_RINS);
	spdict_.emplace("OPUA.GRB.MIP.SolFiles", GRB_StringParam_SolFiles);
	ipdict_.emplace("OPUA.GRB.MIP.SolutionNumber", GRB_IntParam_SolutionNumber);
	ipdict_.emplace("OPUA.GRB.MIP.StartNodeLimit", GRB_IntParam_StartNodeLimit);
	ipdict_.emplace("OPUA.GRB.MIP.StartNumber", GRB_IntParam_StartNumber);
	ipdict_.emplace("OPUA.GRB.MIP.SubMIPNodes", GRB_IntParam_SubMIPNodes);
	ipdict_.emplace("OPUA.GRB.MIP.Symmetry", GRB_IntParam_Symmetry);
	ipdict_.emplace("OPUA.GRB.MIP.VarBranch", GRB_IntParam_VarBranch);
	ipdict_.emplace("OPUA.GRB.MIP.ZeroObjNodes", GRB_IntParam_ZeroObjNodes);
}

GRB_DoubleParam Solver::OpGRBCfgCvt::getDoubleParam(OpStr key)
{
	return dpdict_.at(key);
}

GRB_IntParam Solver::OpGRBCfgCvt::getIntParam(OpStr key)
{
	return ipdict_.at(key);
}

GRB_StringParam Solver::OpGRBCfgCvt::getStringParam(OpStr key)
{
	return spdict_.at(key);
}

Solver::OpGRBCfgCvt::OpGRBCfgCvt()
{
	init();
}

/* OPUA::Solver::OpGRBSolI */

class Solver::OpGRBSolI
	: public OpImplBase
{
protected:
	// GRB组件模型

	GRBEnv* genv_; // GRB环境变量
	GRBModel* gmdl_; // GRB模型对象
protected:
	// OPUA-GRB映射信息

	std::unordered_map<OpLInt, GRBVar> vardict_; // GRB变量表
	std::unordered_map<OpLInt, GRBConstr> lcdict_; // GRB线性约束表
	std::unordered_map<OpLInt, GRBQConstr> qcdict_; // GRB二次约束表
	std::unordered_map<OpLInt, GRBSOS> scdict_; // GRBSOS约束表
	std::unordered_map<OpLInt, GRBGenConstr> nlcdict_; // GRB非线性约束表
	std::unordered_map<OpLInt, GRBLinExpr> modict_; // GRB多目标优化目标函数表
	OpGRBCfgCvt cfgcvt_; // 配置翻译器

	friend class OpGRBSol;
private:
	GRBVar addGRBVar(Variable::OpVar var); // 从OPUA变量创建一个GRB变量
	char senseConvert1(Constraint::OpConSense sense); // 将OPUA约束类型转换为GRB约束类型
	int senseConvert2(Constraint::OpConSense sense); // 将OPUA约束类型转换为GRB约束类型
	GRBLinExpr addGRBLE(const Expression::OpLinExpr& expr); // 由OPUA线性表达式创建一个GRB线性表达式
	GRBQuadExpr addGRBQE(const Expression::OpQuadExpr& expr); // 由OPUA二次表达式创建一个GRB二次表达式
	GRBConstr addGRBLC(Constraint::OpLinCon con); // 从OPUA线性约束创建一个GRB线性约束
	GRBQConstr addGRBQC(Constraint::OpQuadCon con); // 从OPUA二次约束创建一个GRB二次约束
	GRBSOS addGRBSOS(Constraint::OpSOSCon con); // 从OPUASOS约束创建一个GRBSOS约束
	GRBGenConstr addGRBGen(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Max约束
	GRBGenConstr addGRBGenMax(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Max约束
	GRBGenConstr addGRBGenMin(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Min约束
	GRBGenConstr addGRBGenAbs(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Abs约束
	GRBGenConstr addGRBGenExp(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Exp约束
	GRBGenConstr addGRBGenExpA(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性ExpA约束
	GRBGenConstr addGRBGenLog(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Log约束
	GRBGenConstr addGRBGenLn(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性LogA约束
	GRBGenConstr addGRBGenLogA(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性LogA约束
	GRBGenConstr addGRBGenSquare(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Pow约束
	GRBGenConstr addGRBGenSqrt(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Pow约束
	GRBGenConstr addGRBGenPow(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Pow约束
	GRBGenConstr addGRBGenSin(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Sin约束
	GRBGenConstr addGRBGenCos(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Cos约束
	GRBGenConstr addGRBGenTan(Constraint::OpNLCon con); // 从OPUA非线性约束创建一个GRB非线性Tan约束
protected:
	void init(); // 初始化
	void clear(); // 清除所有组件与映射信息
	void extract(Model::OpModel mdl); // 抽取OPUA模型
	void solve(); // 求解模型
	void setParam(const OpConfig& cfg); // 设置配置
	OpLInt getStatus(); // 获取求解状态
	OpFloat getValue(Variable::OpVar var); // 获取变量的解
	OpFloat getValue(const Expression::OpLinExpr& expr); // 获取表达式的解(速度较慢)
	OpFloat getValue(const Expression::OpQuadExpr& expr); // 获取表达式的解(速度较慢)
	OpFloat getValue(Objective::OpObj obj); // 获取目标函数解(速度较慢)
	OpFloat getDual(Constraint::OpLinCon con); // 获取对偶解
protected:
	OpGRBSolI(OpEnvI* env);
	OpGRBSolI(OpEnvI* env, Model::OpModel mdl);
public:
	virtual ~OpGRBSolI();
};

GRBVar Solver::OpGRBSolI::addGRBVar(Variable::OpVar var)
{
	char vtype(GRB_CONTINUOUS);
	switch (var.getType())
	{
	case Variable::OpVarType::Bool:
		vtype = GRB_BINARY;
		break;
	case Variable::OpVarType::Int:
		vtype = GRB_INTEGER;
		break;
	case Variable::OpVarType::Con:
		vtype = GRB_CONTINUOUS;
		break;
	case Variable::OpVarType::Sem:
		vtype = GRB_SEMICONT;
		break;
	default:
		break;
	}
	return gmdl_->addVar(var.getLb(), var.getUb(), 0.0, vtype, var.getName());
}

char Solver::OpGRBSolI::senseConvert1(Constraint::OpConSense sense)
{
	auto tmp(GRB_EQUAL);
	switch (sense)
	{
	case OPUA::Constraint::OpConSense::Equal:
		tmp = GRB_EQUAL;
		break;
	case OPUA::Constraint::OpConSense::LessEqual:
		tmp = GRB_LESS_EQUAL;
		break;
	case OPUA::Constraint::OpConSense::GreatEqual:
		tmp = GRB_GREATER_EQUAL;
		break;
	default:
		break;
	}
	return tmp;
}

int Solver::OpGRBSolI::senseConvert2(Constraint::OpConSense sense)
{
	auto tmp(GRB_SOS_TYPE1);
	switch (sense)
	{
	case OPUA::Constraint::OpConSense::SOS1:
		tmp = GRB_SOS_TYPE1;
		break;
	case OPUA::Constraint::OpConSense::SOS2:
		tmp = GRB_SOS_TYPE2;
		break;
	default:
		break;
	}
	return tmp;
}

GRBLinExpr Solver::OpGRBSolI::addGRBLE(const Expression::OpLinExpr& expr)
{
	GRBLinExpr tmp(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		tmp += iter.getCoeff() * vardict_.at(iter.getVar().getIndex());
	return tmp;
}

GRBQuadExpr Solver::OpGRBSolI::addGRBQE(const Expression::OpQuadExpr& expr)
{
	GRBQuadExpr tmp(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		tmp += iter.getCoeff() * vardict_.at(iter.getVar().getIndex());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		tmp += iter.getCoeff() * vardict_.at(iter.getVar1().getIndex()) * vardict_.at(iter.getVar2().getIndex());
	return tmp;
}

GRBConstr Solver::OpGRBSolI::addGRBLC(Constraint::OpLinCon con)
{
	GRBLinExpr lhs(addGRBLE(con.getExpr(true)));
	GRBLinExpr rhs(con.isStandard() ? con.getRHS() : addGRBLE(con.getExpr(false)));
	return gmdl_->addConstr(lhs, senseConvert1(con.getSense()), rhs, con.getName());
}

GRBQConstr Solver::OpGRBSolI::addGRBQC(Constraint::OpQuadCon con)
{
	GRBQuadExpr lhs(addGRBQE(con.getExpr(true)));
	GRBQuadExpr rhs(con.isStandard() ? con.getRHS() : addGRBQE(con.getExpr(false)));
	return gmdl_->addQConstr(lhs, senseConvert1(con.getSense()), rhs, con.getName());
}

GRBSOS Solver::OpGRBSolI::addGRBSOS(Constraint::OpSOSCon con)
{
	auto& expr(con.getSOSExpr());
	int len(expr.getSize());
	std::vector<GRBVar> vars;
	std::vector<double> weights;
	vars.reserve(len);
	weights.reserve(len);
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
	{
		weights.emplace_back(iter.getCoeff());
		vars.emplace_back(vardict_.at(iter.getVar().getIndex()));
	}
	return gmdl_->addSOS(vars.data(), weights.data(), len, senseConvert2(con.getSense()));
}

GRBGenConstr Solver::OpGRBSolI::addGRBGen(Constraint::OpNLCon con)
{
	GRBGenConstr tmp;
	switch (con.getFunction())
	{
	case Expression::OpNLFunc::Unknow: // 无法处理
		break;
	case Expression::OpNLFunc::Sum: // 无法处理
		break;
	case Expression::OpNLFunc::Abs:
		tmp = addGRBGenAbs(con);
		break;
	case Expression::OpNLFunc::Max:
		tmp = addGRBGenMax(con);
		break;
	case Expression::OpNLFunc::Min:
		tmp = addGRBGenMin(con);
		break;
	case Expression::OpNLFunc::Square:
		tmp = addGRBGenSquare(con);
		break;
	case Expression::OpNLFunc::Sqrt:
		tmp = addGRBGenSqrt(con);
		break;
	case Expression::OpNLFunc::Pow:
		tmp = addGRBGenPow(con);
		break;
	case Expression::OpNLFunc::Exp1:
		tmp = addGRBGenExp(con);
		break;
	case Expression::OpNLFunc::Exp2:
		tmp = addGRBGenExpA(con);
		break;
	case Expression::OpNLFunc::Log1:
		tmp = addGRBGenLog(con);
		break;
	case Expression::OpNLFunc::Log2:
		tmp = addGRBGenLn(con);
		break;
	case Expression::OpNLFunc::Log3:
		tmp = addGRBGenLogA(con);
		break;
	case Expression::OpNLFunc::Sin:
		tmp = addGRBGenSin(con);
		break;
	case Expression::OpNLFunc::Cos:
		tmp = addGRBGenCos(con);
		break;
	case Expression::OpNLFunc::Tan:
		tmp = addGRBGenTan(con);
		break;
	default:
		break;
	}
	return tmp;
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenMax(Constraint::OpNLCon con)
{
	auto& expr(con.getExpr());
	int len(expr.getSize());
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	std::vector<GRBVar> vars;
	vars.reserve(len);
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
		vars.emplace_back(vardict_.at(iter.getVar().getIndex()));
	return gmdl_->addGenConstrMax(var0, vars.data(), len, expr.getParam(), con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenMin(Constraint::OpNLCon con)
{
	auto& expr(con.getExpr());
	int len(expr.getSize());
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	std::vector<GRBVar> vars;
	vars.reserve(len);
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
		vars.emplace_back(vardict_.at(iter.getVar().getIndex()));
	return gmdl_->addGenConstrMin(var0, vars.data(), len, expr.getParam(), con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenAbs(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrAbs(var0, var1, con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenExp(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrExp(var1, var0, con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenExpA(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrExpA(var1, var0, con.getExpr().getParam(), con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenLog(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrLog(var1, var0, con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenLn(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrLogA(var1, var0, Constant::NL, con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenLogA(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrLogA(var1, var0, con.getExpr().getParam(), con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenSquare(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrPow(var1, var0, 2.0, con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenSqrt(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrPow(var1, var0, 0.5, con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenPow(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrPow(var1, var0, con.getExpr().getParam(), con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenSin(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrSin(var1, var0, con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenCos(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrCos(var1, var0, con.getName());
}

GRBGenConstr Solver::OpGRBSolI::addGRBGenTan(Constraint::OpNLCon con)
{
	GRBVar var0(vardict_.at(con.getVar().getIndex()));
	GRBVar var1(vardict_.at(con.getExpr().getNLBegin().getVar().getIndex()));
	return gmdl_->addGenConstrTan(var1, var0, con.getName());
}

void Solver::OpGRBSolI::init()
{
	genv_ = new GRBEnv();
	gmdl_ = new GRBModel(*genv_);
}

void Solver::OpGRBSolI::clear()
{
	vardict_.clear();
	lcdict_.clear();
	qcdict_.clear();
	scdict_.clear();
	nlcdict_.clear();
	modict_.clear();
	delete gmdl_;
	gmdl_ = nullptr;
	delete genv_;
	genv_ = nullptr;
}

void Solver::OpGRBSolI::extract(Model::OpModel mdl)
{
	// 首先清除原模型
	if (genv_)
	{
		clear();
		init();
	}
	// 加载现有模型
	for (auto iter = mdl.getVBegin(); iter != mdl.getVEnd(); ++iter)
		vardict_.emplace(iter.getKey(), addGRBVar(iter.getVal()));
	for (auto iter = mdl.getLCBegin(); iter != mdl.getLCEnd(); ++iter)
		lcdict_.emplace(iter.getKey(), addGRBLC(iter.getVal()));
	for (auto iter = mdl.getQCBegin(); iter != mdl.getQCEnd(); ++iter)
		qcdict_.emplace(iter.getKey(), addGRBQC(iter.getVal()));
	for (auto iter = mdl.getSOSBegin(); iter != mdl.getSOSEnd(); ++iter)
		scdict_.emplace(iter.getKey(), addGRBSOS(iter.getVal()));
	for (auto iter = mdl.getNLCBegin(); iter != mdl.getNLCEnd(); ++iter)
		nlcdict_.emplace(iter.getKey(), addGRBGen(iter.getVal()));
	gmdl_->setObjective(addGRBQE(mdl.getObj().getQuadExpr()) + addGRBLE(mdl.getObj().getLinExpr()));
}

void Solver::OpGRBSolI::solve()
{
	gmdl_->optimize();
}

void Solver::OpGRBSolI::setParam(const OpConfig& cfg)
{
	for (const auto& key : cfg.icfg_)
		gmdl_->set(cfgcvt_.getIntParam(key.first), key.second);
	for (const auto& key : cfg.fcfg_)
		gmdl_->set(cfgcvt_.getDoubleParam(key.first), key.second);
	for (const auto& key : cfg.scfg_)
		gmdl_->set(cfgcvt_.getStringParam(key.first), key.second);
}

OpLInt Solver::OpGRBSolI::getStatus()
{
	return gmdl_->get(GRB_IntAttr_Status);
}

OpFloat Solver::OpGRBSolI::getValue(Variable::OpVar var)
{
	return vardict_.at(var.getIndex()).get(GRB_DoubleAttr::GRB_DoubleAttr_X);
}

OpFloat Solver::OpGRBSolI::getValue(const Expression::OpLinExpr& expr)
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	return result;
}

OpFloat Solver::OpGRBSolI::getValue(const Expression::OpQuadExpr& expr)
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar1()) * getValue(iter.getVar2());
	return result;
}

OpFloat Solver::OpGRBSolI::getValue(Objective::OpObj obj)
{
	return getValue(obj.getLinExpr()) + getValue(obj.getQuadExpr());
}

OpFloat Solver::OpGRBSolI::getDual(Constraint::OpLinCon con)
{
	return lcdict_.at(con.getIndex()).get(GRB_DoubleAttr::GRB_DoubleAttr_Pi);
}

Solver::OpGRBSolI::OpGRBSolI(OpEnvI* env)
	: OpImplBase('S', env),
	genv_(nullptr), 
	gmdl_(nullptr)
{
	init();
}

Solver::OpGRBSolI::OpGRBSolI(OpEnvI* env, Model::OpModel mdl)
	: OpImplBase('S', env),
	genv_(nullptr),
	gmdl_(nullptr)
{
	init();
	extract(mdl);
}

Solver::OpGRBSolI::~OpGRBSolI()
{
	clear();
}

/* OPUA::Solver::OpGRBSol */


void Solver::OpGRBSol::extract(Model::OpModel mdl)
{
	static_cast<OpGRBSolI*>(impl_)->extract(mdl);
}

void Solver::OpGRBSol::solve()
{
	static_cast<OpGRBSolI*>(impl_)->solve();
}

void OPUA::Solver::OpGRBSol::setParam(const OpConfig& cfg)
{
	static_cast<OpGRBSolI*>(impl_)->setParam(cfg);
}

OpLInt OPUA::Solver::OpGRBSol::getStatus()
{
	return static_cast<OpGRBSolI*>(impl_) ->getStatus();
}

OpFloat Solver::OpGRBSol::getValue(Variable::OpVar var)
{
	return static_cast<OpGRBSolI*>(impl_) ->getValue(var);
}

OpFloat Solver::OpGRBSol::getValue(const Expression::OpLinExpr& expr)
{
	return static_cast<OpGRBSolI*>(impl_) ->getValue(expr);
}

OpFloat Solver::OpGRBSol::getValue(const Expression::OpQuadExpr& expr)
{
	return static_cast<OpGRBSolI*>(impl_) ->getValue(expr);
}

OpFloat OPUA::Solver::OpGRBSol::getValue(Objective::OpObj obj)
{
	return static_cast<OpGRBSolI*>(impl_) ->getValue(obj);
}

OpFloat Solver::OpGRBSol::getDual(Constraint::OpLinCon con)
{
	return static_cast<OpGRBSolI*>(impl_) ->getDual(con);
}

Solver::OpGRBSol::OpGRBSol()
{

}

Solver::OpGRBSol::OpGRBSol(OpGRBSolI* impl)
{
	impl_ = impl;
}

Solver::OpGRBSol::OpGRBSol(OpEnv env)
{
	impl_ = new OpGRBSolI(env.getImpl());
}

Solver::OpGRBSol::OpGRBSol(OpEnv env, Model::OpModel mdl)
{
	impl_ = new OpGRBSolI(env.getImpl(), mdl);
}

Solver::OpGRBSol::~OpGRBSol()
{

}