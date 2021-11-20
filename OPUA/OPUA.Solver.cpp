#include "OPUA.Solver.h"

using namespace OPUA;

void Solver::OpConfig::RegisterConfig(OpStr key, OpBool val)
{
	bcfg_[key] = val; 
}

void Solver::OpConfig::RegisterConfig(OpStr key, OpLInt val)
{
	icfg_[key] = val; 
}

void Solver::OpConfig::RegisterConfig(OpStr key, OpFloat val)
{ 
	fcfg_[key] = val;
}

void Solver::OpConfig::RegisterConfig(OpStr key, OpStr val)
{ 
	scfg_[key] = val;
}

void Solver::OpConfig::ClearConfig(OpStr key, OpBool flag)
{ 
	bcfg_.erase(key);
}

void Solver::OpConfig::ClearConfig(OpStr key, OpLInt flag)
{ 
	icfg_.erase(key);
}

void Solver::OpConfig::ClearConfig(OpStr key, OpFloat flag)
{ 
	fcfg_.erase(key);
}

void Solver::OpConfig::ClearConfig(OpStr key, OpStr flag)
{ 
	scfg_.erase(key);
}

void Solver::OpConfig::ClearAll()
{ 
	bcfg_.clear();
	icfg_.clear();
	fcfg_.clear();
	scfg_.clear(); }

OpBool Solver::OpConfig::GetConfig(OpStr key, OpBool flag) const
{ 
	return bcfg_.at(key);
}

OpLInt Solver::OpConfig::GetConfig(OpStr key, OpLInt flag) const
{ 
	return icfg_.at(key);
}

OpFloat Solver::OpConfig::GetConfig(OpStr key, OpFloat flag) const
{
	return fcfg_.at(key);
}

OpStr Solver::OpConfig::GetConfig(OpStr key, OpStr flag) const
{ 
	return scfg_.at(key);
}
