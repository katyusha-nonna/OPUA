#include "OPUA.Solver.h"
#include<algorithm>

using namespace OPUA;

void OPUA::Solver::OpConfig::removeStrSpace(OpStr& key) const
{
	// ���
	auto wsEndsAt = std::find_if(key.begin(), key.end(), [](char c) {return c < 0 || !isspace(c); });
	key.erase(key.begin(), wsEndsAt);
	// �Ҳ�
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

OpLInt Solver::OpConfig::getCfg(OpStr key, OpLInt flag) const
{
	removeStrSpace(key);
	return lcfg_.at(key);
}

OpFloat Solver::OpConfig::getCfg(OpStr key, OpFloat flag) const
{
	removeStrSpace(key);
	return fcfg_.at(key);
}

OpChar Solver::OpConfig::getCfg(OpStr key, OpChar flag) const
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

Solver::OpSolState::OpSolState()
{

}

Solver::OpSolState::~OpSolState()
{

}

void Solver::OpAdapSol::extract(Model::OpModel mdl)
{
	rsolver_->extract(mdl);
}

void Solver::OpAdapSol::solve()
{
	rsolver_->solve();
}

void Solver::OpAdapSol::solveFixed()
{
	rsolver_->solveFixed();
}

void Solver::OpAdapSol::setParam(const OpConfig& cfg)
{
	rsolver_->setParam(cfg);
}

Solver::OpSolType Solver::OpAdapSol::getSolType() const
{
	return stype_;
}

OpLInt Solver::OpAdapSol::getStatus() const
{
	return rsolver_->getStatus();
}

OpFloat Solver::OpAdapSol::getObjValue() const
{
	return rsolver_->getObjValue();
}

OpFloat Solver::OpAdapSol::getValue(Variable::OpVar var) const
{
	return rsolver_->getValue(var);
}

OpFloat Solver::OpAdapSol::getValue(const Expression::OpLinExpr& expr) const
{
	return rsolver_->getValue(expr);
}

OpFloat Solver::OpAdapSol::getValue(const Expression::OpQuadExpr& expr) const
{
	return rsolver_->getValue(expr);
}

OpFloat Solver::OpAdapSol::getValue(Objective::OpObj obj) const
{
	return rsolver_->getValue(obj);
}

OpFloat Solver::OpAdapSol::getDual(Constraint::OpLinCon con) const
{
	return rsolver_->getDual(con);
}

void Solver::OpAdapSol::write(OpStr path) const
{
	rsolver_->write(path);
}

void Solver::OpAdapSol::release()
{
	rsolver_->release0();
}

Solver::OpAdapSol& Solver::OpAdapSol::operator=(const OpAdapSol& solver)
{
	if (this != &solver && stype_ == solver.stype_)
	{
		rsolver_ = solver.rsolver_;
	}
	return *this;
}

Solver::OpAdapSol::OpAdapSol(const OpAdapSol& solver)
	: stype_(solver.stype_), 
	rsolver_(solver.rsolver_)
{

}

OPUA::Solver::OpAdapSol::OpAdapSol(OpAdapSol&& solver)
	: stype_(solver.stype_),
	rsolver_(solver.rsolver_)
{

}

Solver::OpAdapSol::OpAdapSol(OpSolType type, OpEnv env)
	: stype_(type), 
	rsolver_(nullptr)
{
	switch (stype_)
	{
#ifdef OPUA_COMPILE_CPX
	case OpSolType::CPX:
		rsolver_ = new OpCPXSol(env);
		break;
#endif
#ifdef OPUA_COMPILE_GRB
	case OpSolType::GRB:
		rsolver_ = new OpGRBSol(env);
		break;
#endif
#ifdef OPUA_COMPILE_SCIP
	case OpSolType::SCIP:
		rsolver_ = new OpSCIPSol(env);
		break;
#endif
#ifdef OPUA_COMPILE_MSK
	case OpSolType::MSK:
		rsolver_ = new OpMSKSol(env);
		break;
#endif
#ifdef OPUA_COMPILE_COPT
	case OpSolType::COPT:
		rsolver_ = new OpCOPTSol(env);
		break;
#endif
#ifdef OPUA_COMPILE_IPOPT
	case OpSolType::IPOPT:
		rsolver_ = new OpIPOPTSol(env);
		break;
#endif
#ifdef OPUA_COMPILE_GLPK
	case OpSolType::GLPK:
		rsolver_ = new OpGLPKSol(env);
		break;
#endif
#ifdef OPUA_COMPILE_CBC
	case OpSolType::CBC:
		rsolver_ = new OpCBCSol(env);
		break;
#endif
	default:
		break;
	}
}

Solver::OpAdapSol::OpAdapSol(OpSolType type, OpEnv env, Model::OpModel mdl)
	: stype_(type),
	rsolver_(nullptr)
{
	switch (stype_)
	{
#ifdef OPUA_COMPILE_CPX
	case OpSolType::CPX:
		rsolver_ = new OpCPXSol(env, mdl);
		break;
#endif
#ifdef OPUA_COMPILE_GRB
	case OpSolType::GRB:
		rsolver_ = new OpGRBSol(env, mdl);
		break;
#endif
#ifdef OPUA_COMPILE_SCIP
	case OpSolType::SCIP:
		rsolver_ = new OpSCIPSol(env, mdl);
		break;
#endif
#ifdef OPUA_COMPILE_MSK
	case OpSolType::MSK:
		rsolver_ = new OpMSKSol(env, mdl);
		break;
#endif
#ifdef OPUA_COMPILE_COPT
	case OpSolType::COPT:
		rsolver_ = new OpCOPTSol(env, mdl);
		break;
#endif
#ifdef OPUA_COMPILE_IPOPT
	case OpSolType::IPOPT:
		rsolver_ = new OpIPOPTSol(env, mdl);
		break;
#endif
#ifdef OPUA_COMPILE_GLPK
	case OpSolType::GLPK:
		rsolver_ = new OpGLPKSol(env, mdl);
		break;
#endif
#ifdef OPUA_COMPILE_CBC
	case OpSolType::CBC:
		rsolver_ = new OpCBCSol(env, mdl);
		break;
#endif
	default:
		break;
	}
}

Solver::OpAdapSol::~OpAdapSol()
{
	delete rsolver_;
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
	OpSolStatus lookupTable[16] = {
		OpSolStatus::Unknown, /*SCIP_STATUS_UNKNOWN = 0 */
		OpSolStatus::Unknown, /*SCIP_STATUS_USERINTERRUPT = 1*/
		OpSolStatus::Unknown, /*SCIP_STATUS_NODELIMIT  = 2*/
		OpSolStatus::Unknown, /*SCIP_STATUS_TOTALNODELIMIT  = 3*/
		OpSolStatus::Unknown, /*SCIP_STATUS_STALLNODELIMIT  = 4*/
		OpSolStatus::Unknown, /*SCIP_STATUS_TIMELIMIT  = 5*/
		OpSolStatus::Unknown, /*SCIP_STATUS_MEMLIMIT  = 6*/
		OpSolStatus::Unknown, /*SCIP_STATUS_GAPLIMIT  = 7*/
		OpSolStatus::Unknown, /*SCIP_STATUS_SOLLIMIT  = 8*/
		OpSolStatus::Unknown, /*SCIP_STATUS_BESTSOLLIMIT  = 9*/
		OpSolStatus::Unknown, /*SCIP_STATUS_RESTARTLIMIT  = 10*/
		OpSolStatus::Optimal, /*SCIP_STATUS_OPTIMAL  = 11*/
		OpSolStatus::Infeasible, /*SCIP_STATUS_INFEASIBLE  = 12*/
		OpSolStatus::Unbounded, /*SCIP_STATUS_UNBOUNDED  = 13*/
		OpSolStatus::InfeasibleOrUnbounded, /*SCIP_STATUS_INFORUNBD  = 14*/
		OpSolStatus::Unknown /*SCIP_STATUS_TERMINATE  = 15*/
	};
	return lookupTable[status];
}

Solver::OpSolStatus Solver::IntMSKStatus2OpStatus(OpLInt status)
{
	return OpSolStatus::Unknown;
}

Solver::OpSolStatus OPUA::Solver::IntCOPTStatus2OpStatus(OpLInt status)
{
	OpSolStatus lookupTable[11] = {
	OpSolStatus::Unknown, /*COPT_LPSTATUS_UNSTARTED = 0*/
	OpSolStatus::Optimal, /*COPT_LPSTATUS_OPTIMAL = 1*/
	OpSolStatus::Infeasible, /*COPT_LPSTATUS_INFEASIBLE  = 2*/
	OpSolStatus::Unbounded, /*COPT_LPSTATUS_UNBOUNDED  = 3*/
	OpSolStatus::InfeasibleOrUnbounded, /*COPT_MIPSTATUS_INF_OR_UNB  = 4*/
	OpSolStatus::Unknown, /*COPT_LPSTATUS_NUMERICAL  = 5*/
	OpSolStatus::Unknown, /*COPT_MIPSTATUS_NODELIMIT  = 6*/
	OpSolStatus::Unknown, /*COPT_LPSTATUS_IMPRECISE  = 7*/
	OpSolStatus::Unknown, /*COPT_LPSTATUS_TIMEOUT  = 8*/
	OpSolStatus::Unknown, /*COPT_LPSTATUS_UNFINISHED  = 9*/
	OpSolStatus::Unknown /*COPT_LPSTATUS_INTERRUPTED   = 10*/
	};
	return lookupTable[status];
}

Solver::OpSolStatus Solver::IntIPOPTStatus2OpStatus(OpLInt status)
{
	OpSolStatus lookupTable[15] = {
		OpSolStatus::Unknown, /*not_defined = 0*/
		OpSolStatus::Optimal, /*success = 1*/
		OpSolStatus::Unknown, /*maxiter_exceeded  = 2*/
		OpSolStatus::Unknown, /*stop_at_tiny_step  = 3*/
		OpSolStatus::Unknown, /*stop_at_acceptable_point  = 4*/
		OpSolStatus::Infeasible, /*local_infeasibility  = 5*/
		OpSolStatus::Unknown, /*user_requested_stop  = 6*/
		OpSolStatus::Feasible, /*feasible_point_found  = 7*/
		OpSolStatus::Unknown, /*diverging_iterates  = 8*/
		OpSolStatus::Unknown, /*restoration_failure  = 9*/
		OpSolStatus::Unknown, /*error_in_step_computation  = 10*/
		OpSolStatus::Unknown, /*invalid_number_detected  = 11*/
		OpSolStatus::Unknown, /* too_few_degrees_of_freedom  = 12*/
		OpSolStatus::Unknown, /*internal_error  = 13*/
		OpSolStatus::Unknown /*unknown  = 14*/
	};
	return lookupTable[status];
}

Solver::OpSolStatus Solver::IntGLPKStatus2OpStatus(OpLInt status)
{
	OpSolStatus lookupTable[7] = {
		OpSolStatus::Unknown, /*NULL = 0*/
		OpSolStatus::Unknown, /*GLP_UNDEF = 1*/
		OpSolStatus::Feasible, /*GLP_FEAS = 2*/
		OpSolStatus::Infeasible, /*GLP_INFEAS = 3*/
		OpSolStatus::Infeasible, /*GLP_NOFEAS = 4*/
		OpSolStatus::Optimal, /*GLP_OPT = 5*/
		OpSolStatus::Unbounded /*GLP_UNBND = 6*/
	};
	return lookupTable[status];
}

Solver::OpSolStatus Solver::IntCBCStatus2OpStatus(OpLInt status)
{
	OpSolStatus lookupTable[5] = {
		OpSolStatus::Unknown, /*Unknown = 0*/
		OpSolStatus::Feasible, /*Feasible = 1*/
		OpSolStatus::Optimal, /*Optimal = 2*/
		OpSolStatus::Infeasible, /*Infeasible = 3*/
		OpSolStatus::Unbounded /*Unbounded = 4*/
	};
	return lookupTable[status];
}

Solver::OpSolStatus Solver::IntStatus2OpStatus(OpSolType stype, OpLInt status)
{
	OpSolStatus result(OpSolStatus::Unknown);
	switch (stype)
	{
#ifdef OPUA_COMPILE_CPX
	case OpSolType::CPX:
		result = IntCPXStatus2OpStatus(status);
		break;
#endif
#ifdef OPUA_COMPILE_GRB
	case OpSolType::GRB:
		result = IntGRBStatus2OpStatus(status);
		break;
#endif
#ifdef OPUA_COMPILE_SCIP
	case OpSolType::SCIP:
		result = IntSCIPStatus2OpStatus(status);
		break;
#endif
#ifdef OPUA_COMPILE_MSK
	case OpSolType::MSK:
		result = IntMSKStatus2OpStatus(status);
		break;
#endif
#ifdef OPUA_COMPILE_COPT
	case OpSolType::COPT:
		result = IntCOPTStatus2OpStatus(status);
		break;
#endif
#ifdef OPUA_COMPILE_IPOPT
	case OpSolType::IPOPT:
		result = IntIPOPTStatus2OpStatus(status);
		break;
#endif
#ifdef OPUA_COMPILE_GLPK
	case OpSolType::GLPK:
		result = IntGLPKStatus2OpStatus(status);
		break;
#endif
#ifdef OPUA_COMPILE_CBC
	case OpSolType::CBC:
		result = IntCBCStatus2OpStatus(status);
		break;
#endif
	default:
		break;
	}
	return result;
}

Solver::OpSolType Solver::CharSolType2OpSolType(OpChar type)
{
	auto solverMode(Solver::OpSolType::Unknown);
	switch (type)
	{
	case 'B':
		solverMode = Solver::OpSolType::CBC;
		break;
	case 'C':
		solverMode = Solver::OpSolType::CPX;
		break;
	case 'G':
		solverMode = Solver::OpSolType::GRB;
		break;
	case 'I':
		solverMode = Solver::OpSolType::IPOPT;
		break;
	case 'K':
		solverMode = Solver::OpSolType::GLPK;
		break;
	case 'M':
		solverMode = Solver::OpSolType::MSK;
		break;
	case 'S':
		solverMode = Solver::OpSolType::SCIP;
		break;
	case 'T':
		solverMode = Solver::OpSolType::COPT;
		break;
	default:
		break;
	}
	return solverMode;
}
