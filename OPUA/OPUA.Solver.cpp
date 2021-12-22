#include "OPUA.Solver.h"
#include<algorithm>

using namespace OPUA;

void OPUA::Solver::OpConfig::removeStrSpace(OpStr& key) const
{
	// ×ó²à
	auto wsEndsAt = std::find_if(key.begin(), key.end(), [](char c) {return c < 0 || !isspace(c); });
	key.erase(key.begin(), wsEndsAt);
	// ÓÒ²à
	auto wsStartsAt = std::find_if(key.rbegin(), key.rend(), [](char c) {return c < 0 || !isspace(c); });
	key.erase(wsStartsAt.base(), key.end());
}

void Solver::OpConfig::regCfg(OpStr key, OpBool val)
{
	removeStrSpace(key);
	bcfg_[key] = val; 
}

void Solver::OpConfig::regCfg(OpStr key, OpInt val)
{
	removeStrSpace(key);
	icfg_[key] = val; 
}

void Solver::OpConfig::regCfg(OpStr key, OpLInt val)
{
	removeStrSpace(key);
	lcfg_[key] = val;
}

void Solver::OpConfig::regCfg(OpStr key, OpFloat val)
{ 
	removeStrSpace(key);
	fcfg_[key] = val;
}

void Solver::OpConfig::regCfg(OpStr key, OpChar val)
{
	removeStrSpace(key);
	ccfg_[key] = val;
}

void Solver::OpConfig::regCfg(OpStr key, OpStr val)
{ 
	removeStrSpace(key);
	scfg_[key] = val;
}

void Solver::OpConfig::clrCfg(OpStr key, OpBool flag)
{ 
	bcfg_.erase(key);
}

void Solver::OpConfig::clrCfg(OpStr key, OpInt flag)
{ 
	icfg_.erase(key);
}

void Solver::OpConfig::clrCfg(OpStr key, OpLInt flag)
{
	lcfg_.erase(key);
}

void Solver::OpConfig::clrCfg(OpStr key, OpFloat flag)
{ 
	fcfg_.erase(key);
}

void Solver::OpConfig::clrCfg(OpStr key, OpChar flag)
{
	ccfg_.erase(key);
}

void Solver::OpConfig::clrCfg(OpStr key, OpStr flag)
{ 
	scfg_.erase(key);
}

void Solver::OpConfig::clrAll()
{ 
	bcfg_.clear();
	icfg_.clear();
	fcfg_.clear();
	scfg_.clear(); }

OpBool Solver::OpConfig::getCfg(OpStr key, OpBool flag) const
{ 
	removeStrSpace(key);
	return bcfg_.at(key);
}

OpInt Solver::OpConfig::getCfg(OpStr key, OpInt flag) const
{ 
	removeStrSpace(key);
	return icfg_.at(key);
}

OpInt Solver::OpConfig::getCfg(OpStr key, OpLInt flag) const
{
	removeStrSpace(key);
	return lcfg_.at(key);
}

OpFloat Solver::OpConfig::getCfg(OpStr key, OpFloat flag) const
{
	removeStrSpace(key);
	return fcfg_.at(key);
}

OpInt Solver::OpConfig::getCfg(OpStr key, OpChar flag) const
{
	removeStrSpace(key);
	return ccfg_.at(key);
}

OpStr Solver::OpConfig::getCfg(OpStr key, OpStr flag) const
{ 
	removeStrSpace(key);
	return scfg_.at(key);
}

Solver::OpConfig::OpCfgCIter<OpBool> Solver::OpConfig::getCBegin(OpStr prefix, OpBool flag) const
{
	return OpCfgCIter<OpBool>(bcfg_.cbegin(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpInt> Solver::OpConfig::getCBegin(OpStr prefix, OpInt flag) const
{
	return OpCfgCIter<OpInt>(icfg_.cbegin(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpLInt> Solver::OpConfig::getCBegin(OpStr prefix, OpLInt flag) const
{
	return OpCfgCIter<OpLInt>(lcfg_.cbegin(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpFloat> Solver::OpConfig::getCBegin(OpStr prefix, OpFloat flag) const
{
	return OpCfgCIter<OpFloat>(fcfg_.cbegin(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpChar> Solver::OpConfig::getCBegin(OpStr prefix, OpChar flag) const
{
	return OpCfgCIter<OpChar>(ccfg_.cbegin(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpStr> Solver::OpConfig::getCBegin(OpStr prefix, OpStr flag) const
{
	return OpCfgCIter<OpStr>(scfg_.cbegin(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpBool> Solver::OpConfig::getCEnd(OpStr prefix, OpBool flag) const
{
	return OpCfgCIter<OpBool>(bcfg_.cend(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpInt> Solver::OpConfig::getCEnd(OpStr prefix, OpInt flag) const
{
	return OpCfgCIter<OpInt>(icfg_.cend(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpLInt> Solver::OpConfig::getCEnd(OpStr prefix, OpLInt flag) const
{
	return OpCfgCIter<OpLInt>(lcfg_.cend(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpFloat> Solver::OpConfig::getCEnd(OpStr prefix, OpFloat flag) const
{
	return OpCfgCIter<OpFloat>(fcfg_.cend(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpChar> Solver::OpConfig::getCEnd(OpStr prefix, OpChar flag) const
{
	return OpCfgCIter<OpChar>(ccfg_.cend(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpStr> Solver::OpConfig::getCEnd(OpStr prefix, OpStr flag) const
{
	return OpCfgCIter<OpStr>(scfg_.cend(), prefix);
}

void Solver::OpAdapSol::extract(Model::OpModel mdl)
{
	switch (stype_)
	{
	case OpSolType::CPX:
		cpxs_.extract(mdl);
		break;
	case OpSolType::GRB:
		grbs_.extract(mdl);
		break;
	case OpSolType::SCIP:
		scips_.extract(mdl);
		break;
	default:
		break;
	}
}

void Solver::OpAdapSol::solve()
{
	switch (stype_)
	{
	case OpSolType::CPX:
		cpxs_.solve();
		break;
	case OpSolType::GRB:
		grbs_.solve();
		break;
	case OpSolType::SCIP:
		scips_.solve();
		break;
	default:
		break;
	}
}

void Solver::OpAdapSol::setParam(const OpConfig& cfg)
{
	switch (stype_)
	{
	case OpSolType::CPX:
		cpxs_.setParam(cfg);
		break;
	case OpSolType::GRB:
		grbs_.setParam(cfg);
		break;
	case OpSolType::SCIP:
		scips_.setParam(cfg);
		break;
	default:
		break;
	}
}

OpLInt Solver::OpAdapSol::getStatus() const
{
	OpLInt tmp(-1);
	switch (stype_)
	{
	case OpSolType::CPX:
		tmp = cpxs_.getStatus();
		break;
	case OpSolType::GRB:
		tmp = grbs_.getStatus();
		break;
	case OpSolType::SCIP:
		tmp = scips_.getStatus();
		break;
	default:
		break;
	}
	return tmp;
}

OpFloat Solver::OpAdapSol::getObjValue() const
{
	OpFloat tmp(NAN);
	switch (stype_)
	{
	case OpSolType::CPX:
		tmp = cpxs_.getObjValue();
		break;
	case OpSolType::GRB:
		tmp = grbs_.getObjValue();
		break;
	case OpSolType::SCIP:
		tmp = scips_.getObjValue();
		break;
	default:
		break;
	}
	return tmp;
}

OpFloat Solver::OpAdapSol::getValue(Variable::OpVar var) const
{
	OpFloat tmp(NAN);
	switch (stype_)
	{
	case OpSolType::CPX:
		tmp = cpxs_.getValue(var);
		break;
	case OpSolType::GRB:
		tmp = grbs_.getValue(var);
		break;
	case OpSolType::SCIP:
		tmp = scips_.getValue(var);
		break;
	default:
		break;
	}
	return tmp;
}

OpFloat Solver::OpAdapSol::getValue(const Expression::OpLinExpr& expr) const
{
	OpFloat tmp(NAN);
	switch (stype_)
	{
	case OpSolType::CPX:
		tmp = cpxs_.getValue(expr);
		break;
	case OpSolType::GRB:
		tmp = grbs_.getValue(expr);
		break;
	case OpSolType::SCIP:
		tmp = scips_.getValue(expr);
		break;
	default:
		break;
	}
	return tmp;
}

OpFloat Solver::OpAdapSol::getValue(const Expression::OpQuadExpr& expr) const
{
	OpFloat tmp(NAN);
	switch (stype_)
	{
	case OpSolType::CPX:
		tmp = cpxs_.getValue(expr);
		break;
	case OpSolType::GRB:
		tmp = grbs_.getValue(expr);
		break;
	case OpSolType::SCIP:
		tmp = scips_.getValue(expr);
		break;
	default:
		break;
	}
	return tmp;
}

OpFloat Solver::OpAdapSol::getValue(Objective::OpObj obj) const
{
	OpFloat tmp(NAN);
	switch (stype_)
	{
	case OpSolType::CPX:
		tmp = cpxs_.getValue(obj);
		break;
	case OpSolType::GRB:
		tmp = grbs_.getValue(obj);
		break;
	case OpSolType::SCIP:
		tmp = scips_.getValue(obj);
		break;
	default:
		break;
	}
	return tmp;
}

OpFloat Solver::OpAdapSol::getDual(Constraint::OpLinCon con) const
{
	OpFloat tmp(NAN);
	switch (stype_)
	{
	case OpSolType::CPX:
		tmp = cpxs_.getDual(con);
		break;
	case OpSolType::GRB:
		tmp = grbs_.getDual(con);
		break;
	case OpSolType::SCIP:
		tmp = scips_.getDual(con);
		break;
	default:
		break;
	}
	return tmp;
}

void Solver::OpAdapSol::write(OpStr path) const
{
	switch (stype_)
	{
	case OpSolType::CPX:
		cpxs_.write(path);
		break;
	case OpSolType::GRB:
		grbs_.write(path);
		break;
	case OpSolType::SCIP:
		scips_.write(path);
		break;
	default:
		break;
	}
}

void Solver::OpAdapSol::release()
{
	switch (stype_)
	{
	case OpSolType::CPX:
		cpxs_.release();
		break;
	case OpSolType::GRB:
		grbs_.release();
		break;
	case OpSolType::SCIP:
		scips_.release();
		break;
	default:
		break;
	}
}

Solver::OpAdapSol& Solver::OpAdapSol::operator=(const OpAdapSol& solver)
{
	if (this != &solver && stype_ == solver.stype_)
	{
		switch (stype_)
		{
		case OpSolType::CPX:
			cpxs_ = solver.cpxs_;
			break;
		case OpSolType::GRB:
			grbs_ = solver.grbs_;
			break;
		case OpSolType::SCIP:
			scips_ = solver.scips_;
			break;
		default:
			break;
		}
	}
	return *this;
}

Solver::OpAdapSol::OpAdapSol(const OpAdapSol& solver)
	: stype_(solver.stype_)
{
	switch (stype_)
	{
	case OpSolType::CPX:
		cpxs_ = solver.cpxs_;
		break;
	case OpSolType::GRB:
		grbs_ = solver.grbs_;
		break;
	case OpSolType::SCIP:
		scips_ = solver.scips_;
		break;
	default:
		break;
	}
}

Solver::OpAdapSol::OpAdapSol(OpSolType type, OpEnv env)
	: stype_(type)
{
	switch (stype_)
	{
	case OpSolType::CPX:
		cpxs_ = OpCPXSol(env);
		break;
	case OpSolType::GRB:
		grbs_ = OpGRBSol(env);
		break;
	case OpSolType::SCIP:
		scips_ = OpSCIPSol(env);
		break;
	default:
		break;
	}
}

Solver::OpAdapSol::OpAdapSol(OpSolType type, OpEnv env, Model::OpModel mdl)
	: stype_(type)
{
	switch (stype_)
	{
	case OpSolType::CPX:
		cpxs_ = OpCPXSol(env, mdl);
		break;
	case OpSolType::GRB:
		grbs_ = OpGRBSol(env, mdl);
		break;
	case OpSolType::SCIP:
		scips_ = OpSCIPSol(env, mdl);
		break;
	default:
		break;
	}
}

Solver::OpSolStatus Solver::IntCPXStatus2OpStatus(OpLInt status)
{
	OpSolStatus lookupTable[7] = {
		OpSolStatus::Unknown, /*IloAlgorithm::Status::Unknown = 0*/
		OpSolStatus::Feasible, /*IloAlgorithm::Status::Feasible = 1*/
		OpSolStatus::Optimal, /*IloAlgorithm::Status::Optimal = 2*/
		OpSolStatus::Infeasible, /*IloAlgorithm::Status::Infeasible = 3*/
		OpSolStatus::Unbounded, /*IloAlgorithm::Status::Unbounded = 4*/
		OpSolStatus::InfeasibleOrUnbounded, /*IloAlgorithm::Status::InfeasibleOrUnbounded = 5*/
		OpSolStatus::Unknown /*IloAlgorithm::Status::Error = 6*/
	};
	return lookupTable[status];
}

Solver::OpSolStatus Solver::IntGRBStatus2OpStatus(OpLInt status)
{
	OpSolStatus lookupTable[17] = {
		OpSolStatus::Unknown, /*NULL*/
		OpSolStatus::Unknown, /*GRB_LOADED = 1*/
		OpSolStatus::Optimal, /*GRB_OPTIMAL  = 2*/
		OpSolStatus::Infeasible, /*GRB_INFEASIBLE  = 3*/
		OpSolStatus::InfeasibleOrUnbounded, /*GRB_INF_OR_UNBD  = 4*/
		OpSolStatus::Unbounded, /*GRB_UNBOUNDED  = 5*/
		OpSolStatus::Unknown, /*GRB_CUTOFF  = 6*/
		OpSolStatus::Unknown, /*GRB_ITERATION_LIMIT  = 7*/
		OpSolStatus::Unknown, /*GRB_NODE_LIMIT  = 8*/
		OpSolStatus::Unknown, /*GRB_TIME_LIMIT  = 9*/
		OpSolStatus::Unknown, /*GRB_SOLUTION_LIMIT  = 10*/
		OpSolStatus::Unknown, /*GRB_INTERRUPTED  = 11*/
		OpSolStatus::Unknown, /*GRB_NUMERIC  = 12*/
		OpSolStatus::Unknown, /*GRB_SUBOPTIMAL  = 13*/
		OpSolStatus::Unknown, /*GRB_INPROGRESS  = 14*/
		OpSolStatus::Unknown, /*GRB_USER_OBJ_LIMIT  = 15*/
		OpSolStatus::Unknown /*GRB_WORK_LIMIT  = 16*/
	};
	return lookupTable[status];
}

Solver::OpSolStatus Solver::IntSCIPStatus2OpStatus(OpLInt status)
{
	return OpSolStatus::Unknown;
}

Solver::OpSolStatus Solver::IntMSKStatus2OpStatus(OpLInt status)
{
	return OpSolStatus::Unknown;
}

Solver::OpSolStatus Solver::IntStatus2OpStatus(OpSolType stype, OpLInt status)
{
	OpSolStatus result(OpSolStatus::Unknown);
	switch (stype)
	{
	case OpSolType::CPX:
		result = IntCPXStatus2OpStatus(status);
		break;
	case OpSolType::GRB:
		result = IntGRBStatus2OpStatus(status);
		break;
	case OpSolType::SCIP:
		result = IntSCIPStatus2OpStatus(status);
		break;
	case OpSolType::MSK:
		result = IntMSKStatus2OpStatus(status);
		break;
	default:
		break;
	}
	return result;
}
