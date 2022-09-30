#ifdef OPUA_COMPILE_COPT
#include "OPUA.Solver.h"
#include "OPUA.Exception.h"
#include <coptcpp_inc/coptcpp_pch.h>
#endif

#ifdef OPUA_COMPILE_COPT
/* OPUA::Solver::OpCOPTCfgCvt */

// COPT配置翻译器
class OPUA::Solver::OpCOPTCfgCvt
{
protected:
	std::unordered_map<OpStr, OpStr> dpdict_;
	std::unordered_map<OpStr, OpStr> ipdict_;
public:
	void init();
	OpStr getDoubleParam(OpStr key);
	OpStr getIntParam(OpStr key);
public:
	OpCOPTCfgCvt();
};

void OPUA::Solver::OpCOPTCfgCvt::init()
{
#ifdef OPUA_COPT_VERSION_501
	// 配置对应版本：5.0.1
	// 加载整数参数

	// 加载浮点参数

#endif
}

OPUA::OpStr OPUA::Solver::OpCOPTCfgCvt::getDoubleParam(OpStr key)
{
	return dpdict_.at(key);
}

OPUA::OpStr OPUA::Solver::OpCOPTCfgCvt::getIntParam(OpStr key)
{
	return ipdict_.at(key);
}

OPUA::Solver::OpCOPTCfgCvt::OpCOPTCfgCvt()
{
	init();
}

/* OPUA::Solver::OpCOPTSolI */

typedef CoptException COPTExc;
typedef Envr COPTEnv;
typedef Model COPTModel;
typedef Var COPTVar;
typedef PsdVar COPTPSDVar;
typedef SymMatrix COPTSymMat;
typedef VarArray COPTVarArr;
typedef PsdVar COPTPSDVar;
typedef Expr COPTLinExpr;
typedef QuadExpr COPTQuadExpr;
typedef PsdExpr COPTPSDExpr;
typedef Constraint COPTLinCon;
typedef QConstraint COPTQuadCon;
typedef Cone COPTConicCon;
typedef PsdConstraint COPTPSDCon;
typedef Sos COPTSOSCon;
typedef GenConstr COPTGenCon;
typedef PsdConstraint COPTPSDCon;

class OPUA::Solver::OpCOPTSolI
	: public OpImplBase
{
protected:
	// COPT组件模型

	COPTEnv tenv_; // COPT环境变量
	COPTModel tmdl_; // COPT模型对象
protected:
	// OPUA-COPT映射信息

	std::unordered_map<OpLInt, COPTVar> vardict_; // COPT变量表
	std::unordered_map<OpLInt, COPTPSDVar> pvdict_; // COPTPSD变量表
	std::unordered_map<OpLInt, COPTLinCon> lcdict_; // COPT线性约束表
	std::unordered_map<OpLInt, COPTQuadCon> qcdict_; // COPT二次约束表
	std::unordered_map<OpLInt, COPTConicCon> cocdict_; // COPT锥约束表
	std::unordered_map<OpLInt, COPTPSDCon> pcdict_; // COPT半定约束表
	std::unordered_map<OpLInt, COPTSOSCon> scdict_; // COPTSOS约束表
	std::unordered_map<OpLInt, COPTGenCon> nlcdict_; // COPT指示器约束表
	OpCOPTCfgCvt cfgcvt_; // 配置翻译器

	friend class OPUA::Solver::OpCOPTSol;
private:
	COPTVar addCOPTVar(OPUA::Variable::OpVar var); // 从OPUA变量创建一个COPT变量
	COPTPSDVar addCOPTPSDVar(OPUA::Variable::OpPSDVar var); // 从OPUA变量创建一个COPT变量
	char senseConvert1(OPUA::Constraint::OpConSense sense); // 将OPUA约束类型转换为COPT约束类型
	int senseConvert2(OPUA::Constraint::OpConSense sense); // 将OPUA约束类型转换为COPT约束类型
	int senseConvert3(OPUA::Constraint::OpConicSense sense); // 将OPUA约束类型转换为COPT约束类型
	COPTLinExpr addCOPTLE(const OPUA::Expression::OpLinExpr & expr); // 由OPUA线性表达式创建一个COPT线性表达式
	COPTQuadExpr addCOPTQE(const OPUA::Expression::OpQuadExpr& expr); // 由OPUA二次表达式创建一个COPT二次表达式
	COPTSymMat addCOPTSymMat(const OPUA::Expression::OpPSDRealMat& mat); // 由OPUA稠密实半定矩阵创建一个COPT实对称阵
	COPTPSDExpr addCOPTPSDE(const OPUA::Expression::OpPSDExpr& expr); // 由OPUA半定表达式创建一个COPT半定表达式
	COPTLinCon addCOPTRange(Constraint::OpLinCon con);  // 从OPUA线性约束创建一个COPT线性约束
	COPTLinCon addCOPTLC(OPUA::Constraint::OpLinCon con); // 从OPUA线性约束创建一个COPT线性约束
	COPTLinCon addCOPTLC(OPUA::Constraint::OpLinCon con, OpBool lb); // 从OPUA线性约束创建一个COPT线性约束
	COPTQuadCon addCOPTQC(OPUA::Constraint::OpQuadCon con); // 从OPUA二次约束创建一个COPT二次约束
	COPTQuadCon addCOPTQC(OPUA::Constraint::OpQuadCon con, OpBool lb); // 从OPUA二次约束创建一个COPT二次约束
	COPTConicCon addCOPTCone(OPUA::Constraint::OpConicCon con); // 从OPUA锥约束创建一个COPT锥约束
	COPTPSDCon addCOPTPSDC(OPUA::Constraint::OpPSDCon con); // 从OPUA半定约束创建一个COPT半定约束
	COPTSOSCon addCOPTSOS(OPUA::Constraint::OpSOSCon con); // 从OPUASOS约束创建一个COPTSOS约束
	COPTGenCon addCOPTGenIndicator(OPUA::Constraint::OpCdtCon con); // 从OPUA条件约束创建一个COPTGenCon指示器约束
	COPTGenCon addCOPTGenIndicator(OPUA::Constraint::OpCdtCon con, OpBool lb); // 从OPUA条件约束创建一个COPTGenCon指示器约束
protected:
	void init(); // 初始化
	void clear(); // 清除所有组件与映射信息
	void extract(OPUA::Model::OpModel mdl); // 抽取OPUA模型
	void solve(); // 求解模型
	void solveFixed(); // 固定整数变量解并求解模型
	void setParam(const OPUA::Solver::OpConfig& cfg); // 设置配置
	OPUA::OpLInt getStatus() const; // 获取求解状态
	OPUA::OpFloat getObjValue() const; // 获取目标函数解
	OPUA::OpFloat getValue(OPUA::Variable::OpVar var) const; // 获取变量的解
	OPUA::OpFloat getValue(const OPUA::Expression::OpLinExpr& expr) const; // 获取表达式的解(速度较慢)
	OPUA::OpFloat getValue(const OPUA::Expression::OpQuadExpr& expr) const; // 获取表达式的解(速度较慢)
	OPUA::OpFloat getValue(OPUA::Objective::OpObj obj) const; // 获取目标函数解(速度较慢)
	OPUA::OpFloat getDual(OPUA::Constraint::OpLinCon con) const; // 获取对偶解
	void write(OPUA::OpStr path) const; // 将模型写入文件
	virtual OPUA::OpULInt getMemoryUsage() const; // 获取内存占用
protected:
	OpCOPTSolI(OPUA::OpEnvI* env);
	OpCOPTSolI(OPUA::OpEnvI* env, OPUA::Model::OpModel mdl);
public:
	virtual ~OpCOPTSolI();
};

COPTVar OPUA::Solver::OpCOPTSolI::addCOPTVar(OPUA::Variable::OpVar var)
{
	double lb(OPUA::Constant::IsInfinity(var.getLb()) ? -COPT_INFINITY : var.getLb());
	double ub(OPUA::Constant::IsInfinity(var.getUb()) ? COPT_INFINITY : var.getUb());
	char vtype(COPT_CONTINUOUS);
	switch (var.getType())
	{
	case OPUA::Variable::OpVarType::Bool:
		vtype = COPT_BINARY;
		break;
	case OPUA::Variable::OpVarType::Int:
		vtype = COPT_INTEGER;
		break;
	case OPUA::Variable::OpVarType::Con:
		vtype = COPT_CONTINUOUS;
		break;
	case OPUA::Variable::OpVarType::Sem:
		throw OpExcBase("[Solver::OpCOPTSolI::addCOPTVar]: Exception->can not handle Variable::OpVarType::Sem");
		break;
	default:
		throw OpExcBase("[Solver::OpCOPTSolI::addCOPTVar]: Exception->can not handle other variable type");
		break;
	}
	return tmdl_.AddVar(lb, ub, 0.0, vtype, var.getName().c_str());
}

COPTPSDVar OPUA::Solver::OpCOPTSolI::addCOPTPSDVar(OPUA::Variable::OpPSDVar var)
{
	return tmdl_.AddPsdVar(var.getDim(), var.getName().c_str());
}

char OPUA::Solver::OpCOPTSolI::senseConvert1(OPUA::Constraint::OpConSense sense)
{
	auto tmp(COPT_EQUAL);
	switch (sense)
	{
	case OPUA::Constraint::OpConSense::Equal:
		tmp = COPT_EQUAL;
		break;
	case OPUA::Constraint::OpConSense::LessEqual:
		tmp = COPT_LESS_EQUAL;
		break;
	case OPUA::Constraint::OpConSense::GreatEqual:
		tmp = COPT_GREATER_EQUAL;
		break;
	default:
		throw OpExcBase("[Solver::OpCOPTSolI::senseConvert1]: Exception->can not handle other sense");
		break;
	}
	return tmp;
}

int OPUA::Solver::OpCOPTSolI::senseConvert2(OPUA::Constraint::OpConSense sense)
{
	auto tmp(COPT_SOS_TYPE1);
	switch (sense)
	{
	case OPUA::Constraint::OpConSense::SOS1:
		tmp = COPT_SOS_TYPE1;
		break;
	case OPUA::Constraint::OpConSense::SOS2:
		tmp = COPT_SOS_TYPE2;
		break;
	default:
		throw OpExcBase("[Solver::OpCOPTSolI::senseConvert1]: Exception->can not handle other sos sense");
		break;
	}
	return tmp;
}

int OPUA::Solver::OpCOPTSolI::senseConvert3(OPUA::Constraint::OpConicSense sense)
{
	auto tmp(COPT_CONE_QUAD);
	switch (sense)
	{
	case OPUA::Constraint::OpConicSense::Unknow: // 无法处理
		throw OpExcBase("[Solver::OpCOPTSolI::senseConvert3]: Exception->can not handle Constraint::OpConicSense::Unknow");
		break;
	case OPUA::Constraint::OpConicSense::SOC:
		tmp = COPT_CONE_QUAD;
		break;
	case OPUA::Constraint::OpConicSense::RSOC:
		tmp = COPT_CONE_RQUAD;
		break;
	case OPUA::Constraint::OpConicSense::PC: // 无法处理
		throw OpExcBase("[Solver::OpCOPTSolI::senseConvert3]: Exception->can not handle Constraint::OpConicSense::PC");
		break;
	case OPUA::Constraint::OpConicSense::DPC: // 无法处理
		throw OpExcBase("[Solver::OpCOPTSolI::senseConvert3]: Exception->can not handle Constraint::OpConicSense::DPC");
		break;
	case OPUA::Constraint::OpConicSense::EC: // 无法处理
		throw OpExcBase("[Solver::OpCOPTSolI::senseConvert3]: Exception->can not handle Constraint::OpConicSense::EC");
		break;
	case OPUA::Constraint::OpConicSense::DEC: // 无法处理
		throw OpExcBase("[Solver::OpCOPTSolI::senseConvert3]: Exception->can not handle Constraint::OpConicSense::DEC");
		break;
	default:
		break;
	}
	return tmp;
}

COPTLinExpr OPUA::Solver::OpCOPTSolI::addCOPTLE(const OPUA::Expression::OpLinExpr& expr)
{
	COPTLinExpr tmp(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
	{
		auto var(iter.getVar());
		if (!var.getFixed())
			tmp.AddTerm(vardict_.at(var.getIndex()), iter.getCoeff());
		else
			tmp.AddConstant(iter.getCoeff() * var.getFixedValue());	
	}
	return tmp;
}

COPTQuadExpr OPUA::Solver::OpCOPTSolI::addCOPTQE(const OPUA::Expression::OpQuadExpr& expr)
{
	COPTQuadExpr tmp(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
	{
		auto var(iter.getVar());
		if (!var.getFixed())
			tmp.AddTerm(vardict_.at(var.getIndex()), iter.getCoeff());
		else
			tmp.AddConstant(iter.getCoeff() * var.getFixedValue());
	}
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
	{
		auto var1(iter.getVar1()), var2(iter.getVar2());
		auto fix1(var1.getFixed()), fix2(var2.getFixed());
		if (!fix1 && !fix2)
			tmp.AddTerm(vardict_.at(var1.getIndex()), vardict_.at(var2.getIndex()), iter.getCoeff());
		else if (fix1 && !fix2)
			tmp.AddTerm(vardict_.at(var2.getIndex()), iter.getCoeff() * var1.getFixedValue());
		else if (!fix1 && fix2)
			tmp.AddTerm(vardict_.at(var1.getIndex()), iter.getCoeff() * var2.getFixedValue());
		else
			tmp.AddConstant(iter.getCoeff() * var1.getFixedValue() * var2.getFixedValue());
	}
	return tmp;
}

COPTSymMat OPUA::Solver::OpCOPTSolI::addCOPTSymMat(const OPUA::Expression::OpPSDRealMat& mat)
{
	int dim(mat.getDim());
	int len(dim * (dim + 1) / 2);
	int count(0);
	std::vector<int> rows(len);
	std::vector<int> cols(len);
	std::vector<double> vals(len);
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < i + 1; j++)
		{
			rows[count] = i;
			cols[count] = j;
			vals[count] = mat.getVal(i, j);
			count++;
		}
	}
	return tmdl_.AddSparseMat(dim, len, rows.data(), cols.data(), vals.data());
}

COPTPSDExpr OPUA::Solver::OpCOPTSolI::addCOPTPSDE(const OPUA::Expression::OpPSDExpr& expr)
{
	COPTPSDExpr tmp(addCOPTLE(expr.getLinTerm()));
	for (auto iter = expr.getPSDBegin(); iter != expr.getPSDEnd(); ++iter)
		tmp.AddTerm(pvdict_.at(iter.getVar().getIndex()), addCOPTSymMat(iter.getCoeff()));
	return tmp;
}

COPTLinCon OPUA::Solver::OpCOPTSolI::addCOPTRange(Constraint::OpLinCon con)
{
	double lb(OPUA::Constant::IsInfinity(con.getLb()) ? -COPT_INFINITY : con.getLb());
	double ub(OPUA::Constant::IsInfinity(con.getUb()) ? COPT_INFINITY : con.getUb());
	return tmdl_.AddConstr(addCOPTLE(con.getExpr()), lb, ub, con.getName().c_str());
}

COPTLinCon OPUA::Solver::OpCOPTSolI::addCOPTLC(OPUA::Constraint::OpLinCon con)
{
	return tmdl_.AddConstr(addCOPTLE(con.getExpr()), COPT_EQUAL, con.getUb(), con.getName().c_str());
}

COPTLinCon OPUA::Solver::OpCOPTSolI::addCOPTLC(OPUA::Constraint::OpLinCon con, OpBool lb)
{
	if (lb)
		return tmdl_.AddConstr(addCOPTLE(con.getExpr()), COPT_GREATER_EQUAL, con.getLb(), con.getName().c_str());
	else
		return tmdl_.AddConstr(addCOPTLE(con.getExpr()), COPT_LESS_EQUAL, con.getUb(), con.getName().c_str());
}

COPTQuadCon OPUA::Solver::OpCOPTSolI::addCOPTQC(OPUA::Constraint::OpQuadCon con)
{
	return tmdl_.AddQConstr(addCOPTQE(con.getExpr()), COPT_EQUAL, con.getUb(), con.getName().c_str());
}

COPTQuadCon OPUA::Solver::OpCOPTSolI::addCOPTQC(OPUA::Constraint::OpQuadCon con, OpBool lb)
{
	if (lb)
		return tmdl_.AddQConstr(addCOPTQE(con.getExpr()), COPT_GREATER_EQUAL, con.getLb(), con.getName().c_str());
	else
		return tmdl_.AddQConstr(addCOPTQE(con.getExpr()), COPT_LESS_EQUAL, con.getUb(), con.getName().c_str());
}

COPTConicCon OPUA::Solver::OpCOPTSolI::addCOPTCone(OPUA::Constraint::OpConicCon con)
{
	COPTVarArr cvars;
	cvars.Reserve(con.getConicExpr().getSize());
	for (auto iter = con.getConicExpr().getNLBegin(); iter != con.getConicExpr().getNLEnd(); ++iter)
		cvars.PushBack(vardict_.at(iter.getVar().getIndex()));
	return tmdl_.AddCone(cvars, senseConvert3(con.getSense()));
}

COPTPSDCon OPUA::Solver::OpCOPTSolI::addCOPTPSDC(OPUA::Constraint::OpPSDCon con)
{
	double lb(OPUA::Constant::IsInfinity(con.getLb()) ? -COPT_INFINITY : con.getLb());
	double ub(OPUA::Constant::IsInfinity(con.getUb()) ? COPT_INFINITY : con.getUb());
	return tmdl_.AddPsdConstr(addCOPTPSDE(con.getExpr()), lb, ub, con.getName().c_str());
}

COPTSOSCon OPUA::Solver::OpCOPTSolI::addCOPTSOS(OPUA::Constraint::OpSOSCon con)
{
	auto& expr(con.getSOSExpr());
	int len(expr.getSize());
	COPTVarArr vars;
	std::vector<double> weights;
	vars.Reserve(len);
	weights.reserve(len);
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
	{
		weights.emplace_back(iter.getCoeff());
		vars.PushBack(vardict_.at(iter.getVar().getIndex()));
	}
	return tmdl_.AddSos(vars, weights.data(), senseConvert2(con.getSense()));
}

COPTGenCon OPUA::Solver::OpCOPTSolI::addCOPTGenIndicator(OPUA::Constraint::OpCdtCon con)
{
	COPTVar ind(vardict_.at(con.getVar().getIndex()));
	auto con2(con.getCon(false));
	return tmdl_.AddGenConstrIndicator(ind, 1, addCOPTLE(con2.getExpr()), COPT_EQUAL, con2.getUb());
}

COPTGenCon OPUA::Solver::OpCOPTSolI::addCOPTGenIndicator(OPUA::Constraint::OpCdtCon con, OpBool lb)
{
	COPTVar ind(vardict_.at(con.getVar().getIndex()));
	auto con2(con.getCon(false));
	if (lb)
		return tmdl_.AddGenConstrIndicator(ind, 1, addCOPTLE(con2.getExpr()), COPT_GREATER_EQUAL, con2.getLb());
	else
		return tmdl_.AddGenConstrIndicator(ind, 1, addCOPTLE(con2.getExpr()), COPT_LESS_EQUAL, con2.getUb());
}

void OPUA::Solver::OpCOPTSolI::init()
{
	tenv_ = COPTEnv();
	tmdl_ = tenv_.CreateModel("");
}

void OPUA::Solver::OpCOPTSolI::clear()
{
	vardict_.clear();
	pvdict_.clear();
	lcdict_.clear();
	qcdict_.clear();
	cocdict_.clear();
	pcdict_.clear();
	scdict_.clear();
	nlcdict_.clear();
	tmdl_ = COPTModel(nullptr);
	tenv_ = COPTEnv(static_cast<Copt::IEnvr*>(nullptr));
}

void OPUA::Solver::OpCOPTSolI::extract(OPUA::Model::OpModel mdl)
{
	try
	{
		// 首先清除原模型
		clear();
		init();
		// 加载现有模型
		for (auto iter = mdl.getCBegin<OPUA::Variable::OpVar>(); iter != mdl.getCEnd<OPUA::Variable::OpVar>(); ++iter)
			vardict_.emplace(iter.getKey(), addCOPTVar(iter.getVal()));
		for (auto iter = mdl.getCBegin<OPUA::Variable::OpPSDVar>(); iter != mdl.getCEnd<OPUA::Variable::OpPSDVar>(); ++iter)
			pvdict_.emplace(iter.getKey(), addCOPTPSDVar(iter.getVal()));
		for (auto iter = mdl.getCBegin<OPUA::Constraint::OpLinCon>(); iter != mdl.getCEnd<OPUA::Constraint::OpLinCon>(); ++iter)
		{
			auto& con(iter.getVal());
			if (OPUA::Constant::IsEqual(con.getLb(), con.getUb()))
				lcdict_.emplace(iter.getKey(), addCOPTLC(con));
			else
				lcdict_.emplace(iter.getKey(), addCOPTRange(con));
		}
		for (auto iter = mdl.getCBegin<OPUA::Constraint::OpQuadCon>(); iter != mdl.getCEnd<OPUA::Constraint::OpQuadCon>(); ++iter)
		{
			auto& con(iter.getVal());
			if (OPUA::Constant::IsEqual(con.getLb(), con.getUb()))
			{
				qcdict_.emplace(iter.getKey(), addCOPTQC(con));
			}
			else
			{
				if (!OPUA::Constant::IsInfinity(con.getLb()))
					qcdict_.emplace(iter.getKey(), addCOPTQC(con, true));
				if (!OPUA::Constant::IsInfinity(con.getUb()))
					qcdict_.emplace(iter.getKey(), addCOPTQC(con, false));
			}
		}
		for (auto iter = mdl.getCBegin<OPUA::Constraint::OpConicCon>(); iter != mdl.getCEnd<OPUA::Constraint::OpConicCon>(); ++iter)
			cocdict_.emplace(iter.getKey(), addCOPTCone(iter.getVal()));
		for (auto iter = mdl.getCBegin<OPUA::Constraint::OpPSDCon>(); iter != mdl.getCEnd<OPUA::Constraint::OpPSDCon>(); ++iter)
			pcdict_.emplace(iter.getKey(), addCOPTPSDC(iter.getVal()));
		for (auto iter = mdl.getCBegin<OPUA::Constraint::OpSOSCon>(); iter != mdl.getCEnd<OPUA::Constraint::OpSOSCon>(); ++iter)
			scdict_.emplace(iter.getKey(), addCOPTSOS(iter.getVal()));
		for (auto iter = mdl.getCBegin<OPUA::Constraint::OpNLCon>(); iter != mdl.getCEnd<OPUA::Constraint::OpNLCon>(); ++iter)
			throw OpExcBase("[Solver::OpCOPTSolI::extract]: Exception->can not handle Constraint::OpNLCon");
		for (auto iter = mdl.getCBegin<OPUA::Constraint::OpCdtCon>(); iter != mdl.getCEnd<OPUA::Constraint::OpCdtCon>(); ++iter)
		{
			auto& con(iter.getVal());
			if (con.isIndicator())
			{
				auto& con2(con.getCon(false));
				if (OPUA::Constant::IsEqual(con2.getLb(), con2.getUb()))
				{
					nlcdict_.emplace(iter.getKey(), addCOPTGenIndicator(con));
				}
				else
				{
					if (!OPUA::Constant::IsInfinity(con2.getLb()))
						nlcdict_.emplace(iter.getKey(), addCOPTGenIndicator(con, true));
					if (!OPUA::Constant::IsInfinity(con2.getUb()))
						nlcdict_.emplace(iter.getKey(), addCOPTGenIndicator(con, false));
				}
			}
			else
				throw OpExcBase("[Solver::OpCOPTSolI::extract]: Exception->can not handle if-else constraint");
		}
		switch (mdl.getObj().getSense())
		{
		case OPUA::Objective::OpObjSense::Min:
			tmdl_.SetQuadObjective(addCOPTQE(mdl.getObj().getQuadExpr() + mdl.getObj().getLinExpr()), COPT_MINIMIZE);
			break;
		case OPUA::Objective::OpObjSense::Max:
			tmdl_.SetQuadObjective(addCOPTQE(mdl.getObj().getQuadExpr() + mdl.getObj().getLinExpr()), COPT_MAXIMIZE);
			break;
		default:
			throw OpExcBase("[Solver::OpCOPTSolI::extract]: Exception->can not handle other objective function sense");
			break;
		}
	}
	catch (OpExcBase& e)
	{
		clear();
		throw e;
	}
	catch (COPTExc& e)
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
		throw OpExcBase("[Solver::OpCOPTSolI::extract]: Exception->unknow exception!");
	}
}

void OPUA::Solver::OpCOPTSolI::solve()
{
	tmdl_.Solve();
}

void OPUA::Solver::OpCOPTSolI::solveFixed()
{
	
}

void OPUA::Solver::OpCOPTSolI::setParam(const OPUA::Solver::OpConfig& cfg)
{
	for (auto iter = cfg.getCBegin<OPUA::OpInt>("OPUA.COPT"); iter != cfg.getCEnd<OPUA::OpInt>("OPUA.COPT"); ++iter)
		if (iter.ok())
			tmdl_.SetIntParam(cfgcvt_.getIntParam(iter.getKey()).c_str(), iter.getVal());
	for (auto iter = cfg.getCBegin<OPUA::OpFloat>("OPUA.COPT"); iter != cfg.getCEnd<OPUA::OpFloat>("OPUA.COPT"); ++iter)
		if (iter.ok())
			tmdl_.SetDblParam(cfgcvt_.getDoubleParam(iter.getKey()).c_str(), iter.getVal());
}

OPUA::OpLInt OPUA::Solver::OpCOPTSolI::getStatus() const
{
	OPUA::OpLInt status(0);
	auto tmpMdl(tmdl_);
	if (tmpMdl.GetIntAttr(COPT_INTATTR_ISMIP))
		status = tmpMdl.GetIntAttr(COPT_INTATTR_MIPSTATUS);
	else
		status = tmpMdl.GetIntAttr(COPT_INTATTR_LPSTATUS);
	return status;
}

OPUA::OpFloat OPUA::Solver::OpCOPTSolI::getObjValue() const
{
	OPUA::OpFloat objval(OPUA::Constant::NaN);
	auto tmpMdl(tmdl_);
	if (tmpMdl.GetIntAttr(COPT_INTATTR_ISMIP))
		objval = tmpMdl.GetDblAttr(COPT_DBLATTR_BESTOBJ);
	else
		objval = tmpMdl.GetDblAttr(COPT_DBLATTR_LPOBJVAL);
	return objval;
}

OPUA::OpFloat OPUA::Solver::OpCOPTSolI::getValue(OPUA::Variable::OpVar var) const
{
	auto coptvar(vardict_.at(var.getIndex()));
	return var.getFixed() ? var.getFixedValue() : coptvar.Get(COPT_DBLINFO_VALUE);
}

OPUA::OpFloat OPUA::Solver::OpCOPTSolI::getValue(const OPUA::Expression::OpLinExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	return result;
}

OPUA::OpFloat OPUA::Solver::OpCOPTSolI::getValue(const OPUA::Expression::OpQuadExpr& expr) const
{
	OpFloat result(expr.getConstant());
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
		result += iter.getCoeff() * getValue(iter.getVar1()) * getValue(iter.getVar2());
	return result;
}

OPUA::OpFloat OPUA::Solver::OpCOPTSolI::getValue(OPUA::Objective::OpObj obj) const
{
	return getValue(obj.getLinExpr()) + getValue(obj.getQuadExpr());
}

OPUA::OpFloat OPUA::Solver::OpCOPTSolI::getDual(OPUA::Constraint::OpLinCon con) const
{
	auto coptcon(lcdict_.at(con.getIndex()));
	return coptcon.Get(COPT_DBLINFO_DUAL);
}

void OPUA::Solver::OpCOPTSolI::write(OPUA::OpStr path) const
{
	if (path.size())
	{
		auto tmpMdl(tmdl_);
		if (tmpMdl.GetIntAttr(COPT_INTATTR_ISMIP))
			tmpMdl.WriteMps(path.c_str());
		else
			tmpMdl.WriteLp(path.c_str());
	}
	else
	{
		// 不支持输出到控制台
	}
}

OPUA::OpULInt OPUA::Solver::OpCOPTSolI::getMemoryUsage() const
{
	return sizeof(*this);
}

OPUA::Solver::OpCOPTSolI::OpCOPTSolI(OPUA::OpEnvI* env)
	: OpImplBase('S', env),
	tenv_(static_cast<Copt::IEnvr*>(nullptr)),
	tmdl_(nullptr)
{

}

OPUA::Solver::OpCOPTSolI::OpCOPTSolI(OPUA::OpEnvI* env, OPUA::Model::OpModel mdl)
	: OpImplBase('S', env),
	tenv_(static_cast<Copt::IEnvr*>(nullptr)),
	tmdl_(nullptr)
{
	extract(mdl);
}

OPUA::Solver::OpCOPTSolI::~OpCOPTSolI()
{
	clear();
}

/* OPUA::Solver::OpCOPTSol */

void OPUA::Solver::OpCOPTSol::extract(OPUA::Model::OpModel mdl)
{
	static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->extract(mdl);
}

void OPUA::Solver::OpCOPTSol::solve()
{
	static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->solve();
}

void OPUA::Solver::OpCOPTSol::solveFixed()
{
	static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->solveFixed();
}

void OPUA::Solver::OpCOPTSol::setParam(const OPUA::Solver::OpConfig& cfg)
{
	static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->setParam(cfg);
}

OPUA::OpLInt OPUA::Solver::OpCOPTSol::getStatus() const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getStatus();
}

OPUA::OpFloat OPUA::Solver::OpCOPTSol::getObjValue() const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getObjValue();
}

OPUA::OpFloat OPUA::Solver::OpCOPTSol::getValue(OPUA::Variable::OpVar var) const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getValue(var);
}

OPUA::OpFloat OPUA::Solver::OpCOPTSol::getValue(const OPUA::Expression::OpLinExpr& expr) const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getValue(expr);
}

OPUA::OpFloat OPUA::Solver::OpCOPTSol::getValue(const OPUA::Expression::OpQuadExpr& expr) const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getValue(expr);
}

OPUA::OpFloat OPUA::Solver::OpCOPTSol::getValue(OPUA::Objective::OpObj obj) const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getValue(obj);
}

OPUA::OpFloat OPUA::Solver::OpCOPTSol::getDual(OPUA::Constraint::OpLinCon con) const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->getDual(con);
}

OPUA::Solver::OpCOPTSolI* OPUA::Solver::OpCOPTSol::getImpl() const
{
	return static_cast<OPUA::Solver::OpCOPTSolI*>(impl_);
}

void OPUA::Solver::OpCOPTSol::write(OPUA::OpStr path) const
{
	static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->write(path);
}

void OPUA::Solver::OpCOPTSol::release0()
{
	static_cast<OPUA::Solver::OpCOPTSolI*>(impl_)->release();
}

OPUA::OpBool OPUA::Solver::OpCOPTSol::operator==(const OPUA::Solver::OpCOPTSol& sol) const
{
	return impl_ == sol.impl_;
}

OPUA::OpBool OPUA::Solver::OpCOPTSol::operator!=(const OPUA::Solver::OpCOPTSol& sol) const
{
	return impl_ != sol.impl_;
}

OPUA::Solver::OpCOPTSol::OpCOPTSol()
{

}

OPUA::Solver::OpCOPTSol::OpCOPTSol(OPUA::Solver::OpCOPTSolI* impl)
{
	impl_ = impl;
}

OPUA::Solver::OpCOPTSol::OpCOPTSol(OPUA::OpEnv env)
{
	impl_ = new OPUA::Solver::OpCOPTSolI(env.getImpl());
}

OPUA::Solver::OpCOPTSol::OpCOPTSol(OPUA::OpEnv env, OPUA::Model::OpModel mdl)
{
	impl_ = new OPUA::Solver::OpCOPTSolI(env.getImpl(), mdl);
}

OPUA::Solver::OpCOPTSol::~OpCOPTSol()
{

}

#endif
