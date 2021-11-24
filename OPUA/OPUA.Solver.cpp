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

void Solver::OpConfig::regCfg(OpStr key, OpLInt val)
{
	removeStrSpace(key);
	icfg_[key] = val; 
}

void Solver::OpConfig::regCfg(OpStr key, OpFloat val)
{ 
	removeStrSpace(key);
	fcfg_[key] = val;
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

void Solver::OpConfig::clrCfg(OpStr key, OpLInt flag)
{ 
	icfg_.erase(key);
}

void Solver::OpConfig::clrCfg(OpStr key, OpFloat flag)
{ 
	fcfg_.erase(key);
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

OpLInt Solver::OpConfig::getCfg(OpStr key, OpLInt flag) const
{ 
	removeStrSpace(key);
	return icfg_.at(key);
}

OpFloat Solver::OpConfig::getCfg(OpStr key, OpFloat flag) const
{
	removeStrSpace(key);
	return fcfg_.at(key);
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

Solver::OpConfig::OpCfgCIter<OpLInt> Solver::OpConfig::getCBegin(OpStr prefix, OpLInt flag) const
{
	return OpCfgCIter<OpLInt>(icfg_.cbegin(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpFloat> Solver::OpConfig::getCBegin(OpStr prefix, OpFloat flag) const
{
	return OpCfgCIter<OpFloat>(fcfg_.cbegin(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpStr> Solver::OpConfig::getCBegin(OpStr prefix, OpStr flag) const
{
	return OpCfgCIter<OpStr>(scfg_.cbegin(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpBool> Solver::OpConfig::getCEnd(OpStr prefix, OpBool flag) const
{
	return OpCfgCIter<OpBool>(bcfg_.cend(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpLInt> Solver::OpConfig::getCEnd(OpStr prefix, OpLInt flag) const
{
	return OpCfgCIter<OpLInt>(icfg_.cend(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpFloat> Solver::OpConfig::getCEnd(OpStr prefix, OpFloat flag) const
{
	return OpCfgCIter<OpFloat>(fcfg_.cend(), prefix);
}

Solver::OpConfig::OpCfgCIter<OpStr> Solver::OpConfig::getCEnd(OpStr prefix, OpStr flag) const
{
	return OpCfgCIter<OpStr>(scfg_.cend(), prefix);
}
