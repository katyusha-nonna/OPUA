#include "OPUA.Model.h"
#include <iostream>

using namespace OPUA;

/* OPUA::Model::OpModelI */

class Model::OpModelI
	: public OpImplBase
{
protected:
	Variable::OpVarIdxDict mvars_; // 优化模型决策变量集
	Variable::OpPSDVIdxDict mpvs_; // 优化模型PSD决策变量集
	Container::OpDict<OpLInt, OpULInt> mvrc_; // 决策变量引用计数
	Container::OpDict<OpLInt, OpULInt> mpvrc_; // PSD决策变量引用计数
	Constraint::OpLCIdxDict mlcs_; // 优化模型线性约束集
	Constraint::OpQCIdxDict mqcs_; // 优化模型二次约束集
	Constraint::OpConicIdxDict mcocs_; // 优化模型锥约束集
	Constraint::OpPSDCIdxDict mpcs_; // 优化模型半定约束集
	Constraint::OpSOSIdxDict mscs_; // 优化模型SOS约束集
	Constraint::OpNLCIdxDict mnlcs_; // 优化模型非线性约束集
	Constraint::OpCCIdxDict mccs_; // 优化模型条件约束集
	Objective::OpObj mobj0_; // 优化模型目标函数(单目标优化)
	Objective::OpObjIdxDict mobjs_; // 优化模型目标函数集(多目标优化)
	OpStr mname_; // 优化模型名称

	friend class OpModel;
	friend class OpMdlCIter;
private:
	void init(); // 初始化(仅允许构造函数使用)
	void clear(); // 清空模型(仅允许析构函数使用)
	void addVar(Variable::OpVar var); // 添加变量
	void addPSDVar(Variable::OpPSDVar var); // 添加PSD变量
	void removeVar(Variable::OpVar var); // 移除变量(引用计数归零才真正删除)
	void removePSDVar(Variable::OpPSDVar var); // 移除PSD变量(引用计数归零才真正删除)
	void addVarsFromLE(const Expression::OpLinExpr& expr); // 通过表达式添加变量(用于添加约束条件/目标函数时检查变量添加)
	void addVarsFromQE(const Expression::OpQuadExpr& expr); // 通过表达式添加变量(用于添加约束条件/目标函数时检查变量添加)
	void addVarsFromPSDE(const Expression::OpPSDExpr& expr); // 通过表达式添加变量(用于添加约束条件/目标函数时检查变量添加)
	void addVarsFromNLE(const Expression::OpNLExpr& expr); // 通过表达式添加变量(用于添加约束条件/目标函数时检查变量添加)
	void removeVarsFromLE(const Expression::OpLinExpr& expr); // 通过表达式移除变量
	void removeVarsFromQE(const Expression::OpQuadExpr& expr); // 通过表达式移除变量
	void removeVarsFromPSDE(const Expression::OpPSDExpr& expr); // 通过表达式移除变量
	void removeVarsFromNLE(const Expression::OpNLExpr& expr); // 通过表达式移除变量
protected:
	void addLinCon(Constraint::OpLinCon con); // 添加线性约束
	void addLinCons(Constraint::OpLCArr cons); // 添加线性约束集
	void addQuadCon(Constraint::OpQuadCon con); // 添加二次约束
	void addQuadCons(Constraint::OpQCArr cons); // 添加二次约束集
	void addConicCon(Constraint::OpConicCon con); // 添加锥约束
	void addConicCons(Constraint::OpConicArr cons); // 添加锥约束集
	void addPSDCon(Constraint::OpPSDCon con); // 添加半定约束
	void addPSDCons(Constraint::OpPSDCArr cons); // 添加半定约束集
	void addSOS(Constraint::OpSOSCon con); // 添加SOS约束
	void addSOSs(Constraint::OpSOSArr cons); // 添加SOS约束集
	void addNLCon(Constraint::OpNLCon con); // 添加非线性约束
	void addNLCons(Constraint::OpNLCArr cons); // 添加非线性约束集
	void addCdtCon(Constraint::OpCdtCon con); // 添加条件约束
	void addCdtCons(Constraint::OpCCArr cons); // 添加条件约束集
	void setObj(Objective::OpObj obj); // 设置单目标优化目标函数
	Objective::OpObj getObj() const; // 获取单目标优化目标函数
	void addMultiObj(Objective::OpObj obj); // 添加多目标优化目标函数
	void addMultiObj(Objective::OpObjArr objs); // 添加多目标优化目标函数
	
	void removeLinCon(Constraint::OpLinCon con); // 移除线性约束
	void removeLinCons(Constraint::OpLCArr cons); // 移除线性约束集
	void removeQuadCon(Constraint::OpQuadCon con); // 移除二次约束
	void removeQuadCons(Constraint::OpQCArr cons); // 移除二次约束集
	void removeConicCon(Constraint::OpConicCon con); // 移除锥约束
	void removeConicCons(Constraint::OpConicArr cons); // 移除锥约束集
	void removePSDCon(Constraint::OpPSDCon con); // 移除半定约束
	void removePSDCons(Constraint::OpPSDCArr cons); // 移除半定约束集
	void removeSOS(Constraint::OpSOSCon con); // 移除SOS约束
	void removeSOSs(Constraint::OpSOSArr cons); // 移除SOS约束集
	void removeNLCon(Constraint::OpNLCon con); // 移除非线性约束
	void removeNLCons(Constraint::OpNLCArr cons); // 移除非线性约束集
	void removeCdtCon(Constraint::OpCdtCon con); // 添加条件约束
	void removeCdtCons(Constraint::OpCCArr cons); // 添加条件约束集
	void removeMultiObj(Objective::OpObj obj); // 移除多目标优化目标函数
	void removeMultiObj(Objective::OpObjArr objs); // 移除多目标优化目标函数

	OpULInt getSize(Variable::OpVar flag) const; // 获取变量数目
	OpULInt getSize(Variable::OpPSDVar flag) const; // 获取PSD变量数目
	OpULInt getSize(Constraint::OpLinCon flag) const; // 获取线性约束数目
	OpULInt getSize(Constraint::OpQuadCon flag) const; // 获取二次约束数目
	OpULInt getSize(Constraint::OpConicCon flag) const; // 获取锥约束数目
	OpULInt getSize(Constraint::OpPSDCon flag) const; // 获取半定约束数目
	OpULInt getSize(Constraint::OpSOSCon flag) const; // 获取SOS约束数目
	OpULInt getSize(Constraint::OpNLCon flag) const; // 获取非线性约束数目
	OpULInt getSize(Constraint::OpCdtCon flag) const; // 获取条件约束数目
	OpULInt getSize(Objective::OpObj flag) const; // 获取目标函数数目

	void setName(OpStr name); // 设置名称
	OpStr getName() const; // 获取名称
	void write(OpStr path) const; // 输出模型
	void preRelease(); // 释放前准备
	virtual OpULInt getMemoryUsage() const; // 获取内存占用
protected:
	OpModelI(OpEnvI* env);
	OpModelI(OpEnvI* env, OpStr name);
public:
	virtual ~OpModelI();
};

void Model::OpModelI::init()
{
	OpEnv localEnv(env_);
	mvars_= Variable::OpVarIdxDict(localEnv);
	localEnv.addManagement(mvars_.getImpl());
	mpvs_ = Variable::OpPSDVIdxDict(localEnv);
	localEnv.addManagement(mpvs_.getImpl());
	mvrc_ = Container::OpDict<OpLInt, OpULInt>(localEnv);
	localEnv.addManagement(mvrc_.getImpl());
	mpvrc_ = Container::OpDict<OpLInt, OpULInt>(localEnv);
	localEnv.addManagement(mpvrc_.getImpl());
	mlcs_ = Constraint::OpLCIdxDict(localEnv);
	localEnv.addManagement(mlcs_.getImpl());
	mqcs_ = Constraint::OpQCIdxDict(localEnv);
	localEnv.addManagement(mqcs_.getImpl());
	mcocs_ = Constraint::OpConicIdxDict(localEnv);
	localEnv.addManagement(mcocs_.getImpl());
	mpcs_ = Constraint::OpPSDCIdxDict(localEnv);
	localEnv.addManagement(mpcs_.getImpl());
	mscs_ = Constraint::OpSOSIdxDict(localEnv);
	localEnv.addManagement(mscs_.getImpl());
	mnlcs_ = Constraint::OpNLCIdxDict(localEnv);
	localEnv.addManagement(mnlcs_.getImpl());
	mccs_ = Constraint::OpCCIdxDict(localEnv);
	localEnv.addManagement(mccs_.getImpl());
	mobjs_ = Objective::OpObjIdxDict(localEnv);
	localEnv.addManagement(mobjs_.getImpl());
}

void Model::OpModelI::clear()
{
	mvars_.release();
	mpvs_.release();
	mvrc_.release();
	mpvrc_.release();
	mlcs_.release();
	mqcs_.release();
	mcocs_.release();
	mpcs_.release();
	mscs_.release();
	mnlcs_.release();
	mccs_.release();
	mobjs_.release();
}

void Model::OpModelI::addVar(Variable::OpVar var)
{
	auto idx(var.getIndex());
	mvars_.tryAdd(idx, var);
	mvrc_[idx]++;
	var.lock();
}

void Model::OpModelI::addPSDVar(Variable::OpPSDVar var)
{
	auto idx(var.getIndex());
	mpvs_.tryAdd(idx, var);
	mpvrc_[idx]++;
	var.lock();
}

void Model::OpModelI::removeVar(Variable::OpVar var)
{
	auto idx(var.getIndex());
	if (mvrc_.has(idx)) // 避免无符号整数越过0
	{
		if (!--mvrc_[idx])
		{
			var.unlock();
			mvars_.remove(idx);
			mvrc_.remove(idx);		
		}
	}
}

void Model::OpModelI::removePSDVar(Variable::OpPSDVar var)
{
	auto idx(var.getIndex());
	if (mpvrc_.has(idx)) // 避免无符号整数越过0
	{
		if (!--mpvrc_[idx])
		{
			var.unlock();
			mpvs_.remove(idx);
			mpvrc_.remove(idx);
		}
	}
}

void Model::OpModelI::addVarsFromLE(const Expression::OpLinExpr& expr)
{
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		addVar(iter.getVar());
}

void Model::OpModelI::addVarsFromQE(const Expression::OpQuadExpr& expr)
{
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		addVar(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
	{
		auto var1(iter.getVar1()), var2(iter.getVar2());
		addVar(var1);
		if (var1 != var2)
			addVar(var2);
	}
}

void Model::OpModelI::addVarsFromPSDE(const Expression::OpPSDExpr& expr)
{
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		addVar(iter.getVar());
	for (auto iter = expr.getPSDBegin(); iter != expr.getPSDEnd(); ++iter)
		addPSDVar(iter.getVar());
}

void Model::OpModelI::addVarsFromNLE(const Expression::OpNLExpr& expr)
{
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
		addVar(iter.getVar());
}

void Model::OpModelI::removeVarsFromLE(const Expression::OpLinExpr& expr)
{
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		removeVar(iter.getVar());
}

void Model::OpModelI::removeVarsFromQE(const Expression::OpQuadExpr& expr)
{
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		removeVar(iter.getVar());
	for (auto iter = expr.getQBegin(); iter != expr.getQEnd(); ++iter)
	{
		auto var1(iter.getVar1()), var2(iter.getVar2());
		removeVar(var1);
		if (var1 != var2)
			removeVar(var2);
	}
}

void Model::OpModelI::removeVarsFromPSDE(const Expression::OpPSDExpr& expr)
{
	for (auto iter = expr.getLBegin(); iter != expr.getLEnd(); ++iter)
		removeVar(iter.getVar());
	for (auto iter = expr.getPSDBegin(); iter != expr.getPSDEnd(); ++iter)
		removePSDVar(iter.getVar());
}

void Model::OpModelI::removeVarsFromNLE(const Expression::OpNLExpr& expr)
{
	for (auto iter = expr.getNLBegin(); iter != expr.getNLEnd(); ++iter)
		removeVar(iter.getVar());
}

void Model::OpModelI::addLinCon(Constraint::OpLinCon con)
{
	auto idx(con.getIndex());
	if (!mlcs_.has(idx))
	{
		addVarsFromLE(con.getExpr());
		mlcs_.add(idx, con);
		con.lock();
	}
}

void Model::OpModelI::addLinCons(Constraint::OpLCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		addLinCon(cons[i]);
}

void Model::OpModelI::addQuadCon(Constraint::OpQuadCon con)
{
	auto idx(con.getIndex());
	if (!mqcs_.has(idx))
	{
		addVarsFromQE(con.getExpr());
		mqcs_.add(idx, con);
		con.lock();
	}
}

void Model::OpModelI::addQuadCons(Constraint::OpQCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		addQuadCon(cons[i]);
}

void Model::OpModelI::addConicCon(Constraint::OpConicCon con)
{
	auto idx(con.getIndex());
	if (!mcocs_.has(idx))
	{
		addVarsFromNLE(con.getConicExpr());
		mcocs_.add(idx, con);
		con.lock();
	}
}

void Model::OpModelI::addConicCons(Constraint::OpConicArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		addConicCon(cons[i]);
}

void Model::OpModelI::addPSDCon(Constraint::OpPSDCon con)
{
	auto idx(con.getIndex());
	if (!mpcs_.has(idx))
	{
		addVarsFromPSDE(con.getExpr());
		mpcs_.add(idx, con);
		con.lock();
	}
}

void Model::OpModelI::addPSDCons(Constraint::OpPSDCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		addPSDCon(cons[i]);
}

void Model::OpModelI::addSOS(Constraint::OpSOSCon con)
{
	auto idx(con.getIndex());
	if (!mscs_.has(idx))
	{
		addVarsFromLE(con.getSOSExpr());
		mscs_.add(idx, con);
		con.lock();
	}
}

void Model::OpModelI::addSOSs(Constraint::OpSOSArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		addSOS(cons[i]);
}

void Model::OpModelI::addNLCon(Constraint::OpNLCon con)
{
	auto idx(con.getIndex());
	if (!mnlcs_.has(idx))
	{
		addVar(con.getVar());
		addVarsFromNLE(con.getExpr());
		mnlcs_.add(idx, con);
		con.lock();
	}
}

void Model::OpModelI::addNLCons(Constraint::OpNLCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		addNLCon(cons[i]);
}

void Model::OpModelI::addCdtCon(Constraint::OpCdtCon con)
{
	auto idx(con.getIndex());
	if (!mccs_.has(idx))
	{
		if (con.isIndicator())
		{
			addVar(con.getVar());
		}
		else
		{
			auto con1(con.getCon(true));
			addVarsFromLE(con1.getExpr());
		}
		auto con2(con.getCon(false));
		addVarsFromLE(con2.getExpr());
		mccs_.add(idx, con);
		con.lock();
	}
}

void Model::OpModelI::addCdtCons(Constraint::OpCCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		addCdtCon(cons[i]);
}

void Model::OpModelI::setObj(Objective::OpObj obj)
{
	if (mobj0_ != obj)
	{
		// 首先移除原目标函数的变量
		if (mobj0_.getImpl())
		{
			mobj0_.unlock();
			removeVarsFromLE(mobj0_.getLinExpr());
			removeVarsFromQE(mobj0_.getQuadExpr());
		}
		// 添加新的变量
		addVarsFromLE(obj.getLinExpr());
		addVarsFromQE(obj.getQuadExpr());
		mobj0_ = obj;
		obj.lock();
	}
}

Objective::OpObj Model::OpModelI::getObj() const
{
	return mobj0_;
}

void Model::OpModelI::addMultiObj(Objective::OpObj obj)
{
	// TODO 暂未实现，需要考虑多目标优化的方式()
}

void Model::OpModelI::addMultiObj(Objective::OpObjArr objs)
{
	for (OpULInt i = 0; i < objs.getSize(); i++)
		addMultiObj(objs[i]);
}

void Model::OpModelI::removeLinCon(Constraint::OpLinCon con)
{
	auto idx(con.getIndex());
	if (mlcs_.has(idx))
	{
		con.unlock();
		mlcs_.remove(idx);
		removeVarsFromLE(con.getExpr());
	}
}

void Model::OpModelI::removeLinCons(Constraint::OpLCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		removeLinCon(cons[i]);
}

void Model::OpModelI::removeQuadCon(Constraint::OpQuadCon con)
{
	auto idx(con.getIndex());
	if (mqcs_.has(idx))
	{
		con.unlock();
		mqcs_.remove(idx);
		removeVarsFromQE(con.getExpr());
	}
}

void Model::OpModelI::removeQuadCons(Constraint::OpQCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		removeQuadCon(cons[i]);
}

void Model::OpModelI::removeConicCon(Constraint::OpConicCon con)
{
	auto idx(con.getIndex());
	if (mcocs_.has(idx))
	{
		con.unlock();
		mcocs_.remove(idx);
		removeVarsFromNLE(con.getConicExpr());
	}
}

void Model::OpModelI::removeConicCons(Constraint::OpConicArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		removeConicCon(cons[i]);
}

void Model::OpModelI::removePSDCon(Constraint::OpPSDCon con)
{
	auto idx(con.getIndex());
	if (mpcs_.has(idx))
	{
		con.unlock();
		mpcs_.remove(idx);
		removeVarsFromPSDE(con.getExpr());
	}
}

void Model::OpModelI::removePSDCons(Constraint::OpPSDCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		removePSDCon(cons[i]);
}

void Model::OpModelI::removeSOS(Constraint::OpSOSCon con)
{
	auto idx(con.getIndex());
	if (mscs_.has(idx))
	{
		con.unlock();
		mscs_.remove(idx);
		removeVarsFromLE(con.getSOSExpr());
	}
}

void Model::OpModelI::removeSOSs(Constraint::OpSOSArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		removeSOS(cons[i]);
}

void Model::OpModelI::removeNLCon(Constraint::OpNLCon con)
{
	auto idx(con.getIndex());
	if (mnlcs_.has(idx))
	{
		con.unlock();
		mnlcs_.remove(idx);
		removeVarsFromNLE(con.getExpr());
		removeVar(con.getVar());
	}
}

void Model::OpModelI::removeNLCons(Constraint::OpNLCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		removeNLCon(cons[i]);
}

void Model::OpModelI::removeCdtCon(Constraint::OpCdtCon con)
{
	auto idx(con.getIndex());
	if (mccs_.has(idx))
	{
		con.unlock();
		mccs_.remove(idx);
		auto con2(con.getCon(false));
		removeVarsFromLE(con2.getExpr());
		if (con.isIndicator())
		{
			removeVar(con.getVar());
		}
		else
		{
			auto con1(con.getCon(true));
			removeVarsFromLE(con1.getExpr());
		}
	}
}

void Model::OpModelI::removeCdtCons(Constraint::OpCCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		removeCdtCon(cons[i]);
}

void Model::OpModelI::removeMultiObj(Objective::OpObj obj)
{
	// TODO 暂未实现，需要考虑多目标优化的方式()
}

void Model::OpModelI::removeMultiObj(Objective::OpObjArr objs)
{
	for (OpULInt i = 0; i < objs.getSize(); i++)
		removeMultiObj(objs[i]);
}

OpULInt Model::OpModelI::getSize(Variable::OpVar flag) const
{
	return mvars_.getSize();
}

OpULInt Model::OpModelI::getSize(Variable::OpPSDVar flag) const
{
	return mpvs_.getSize();
}

OpULInt Model::OpModelI::getSize(Constraint::OpLinCon flag) const
{
	return mlcs_.getSize();
}

OpULInt Model::OpModelI::getSize(Constraint::OpQuadCon flag) const
{
	return mqcs_.getSize();
}

OpULInt Model::OpModelI::getSize(Constraint::OpConicCon flag) const
{
	return mcocs_.getSize();
}

OpULInt Model::OpModelI::getSize(Constraint::OpPSDCon flag) const
{
	return mpcs_.getSize();
}

OpULInt Model::OpModelI::getSize(Constraint::OpSOSCon flag) const
{
	return mscs_.getSize();
}

OpULInt Model::OpModelI::getSize(Constraint::OpNLCon flag) const
{
	return mnlcs_.getSize();
}

OpULInt Model::OpModelI::getSize(Constraint::OpCdtCon flag) const
{
	return mccs_.getSize();
}

OpULInt Model::OpModelI::getSize(Objective::OpObj flag) const
{
	return mobjs_.getSize() + 1;
}

void Model::OpModelI::setName(OpStr name)
{
	mname_ = name;
}

OpStr Model::OpModelI::getName() const
{
	return mname_;
}

void Model::OpModelI::write(OpStr path) const
{
	if (path.size())
	{
		// 输出到文件
	}
	else
	{
		// 输出到控制台
		auto& stream(std::cout);
		stream << "Name: " << mname_ << std::endl;
		stream << mobj0_ << std::endl;
		for (auto iter = mlcs_.getCBegin(); iter != mlcs_.getCEnd(); ++iter)
			stream << iter.getVal().getName() << ":\t" << iter.getVal() << std::endl;
		for (auto iter = mqcs_.getCBegin(); iter != mqcs_.getCEnd(); ++iter)
			stream << iter.getVal().getName() << ":\t" << iter.getVal() << std::endl;
		for (auto iter = mcocs_.getCBegin(); iter != mcocs_.getCEnd(); ++iter)
			stream << iter.getVal().getName() << ":\t" << iter.getVal() << std::endl;
		for (auto iter = mpcs_.getCBegin(); iter != mpcs_.getCEnd(); ++iter)
			stream << iter.getVal().getName() << ":\t" << iter.getVal() << std::endl;
		for (auto iter = mnlcs_.getCBegin(); iter != mnlcs_.getCEnd(); ++iter)
			stream << iter.getVal().getName() << ":\t" << iter.getVal() << std::endl;
		for (auto iter = mscs_.getCBegin(); iter != mscs_.getCEnd(); ++iter)
			stream << iter.getVal().getName() << ":\t" << iter.getVal() << std::endl;
		for (auto iter = mccs_.getCBegin(); iter != mccs_.getCEnd(); ++iter)
			stream << iter.getVal().getName() << ":\t" << iter.getVal() << std::endl;
		stream << "Bounds: " << std::endl;
		for (auto iter = mvars_.getCBegin(); iter != mvars_.getCEnd(); ++iter)
		{
			auto& var(iter.getVal());
			stream << var.getName() << ":\t[";
			if (Constant::IsNInfinity(var.getLb()))
				stream << "-inf";
			else
				stream << var.getLb();
			stream << ", ";
			if (Constant::IsPInfinity(var.getUb()))
				stream << "inf";
			else
				stream << var.getUb();
			stream << "]" << std::endl;
		}
		stream << "PSDs: " << std::endl;
		for (auto iter = mpvs_.getCBegin(); iter != mpvs_.getCEnd(); ++iter)
		{
			auto& var(iter.getVal());
			stream << var.getName() << ":\t[" << var.getDim() << " * " << var.getDim() << "]" << std::endl;
		}
	}
}

void Model::OpModelI::preRelease()
{
	OpEnv localEnv(env_);
	localEnv.removeManagement(mvars_.getImpl());
	localEnv.removeManagement(mpvs_.getImpl());
	localEnv.removeManagement(mvrc_.getImpl());
	localEnv.removeManagement(mpvrc_.getImpl());
	localEnv.removeManagement(mlcs_.getImpl());
	localEnv.removeManagement(mqcs_.getImpl());
	localEnv.removeManagement(mcocs_.getImpl());
	localEnv.removeManagement(mpcs_.getImpl());
	localEnv.removeManagement(mscs_.getImpl());
	localEnv.removeManagement(mnlcs_.getImpl());
	localEnv.removeManagement(mccs_.getImpl());
	localEnv.removeManagement(mobjs_.getImpl());
}

OpULInt Model::OpModelI::getMemoryUsage() const
{
	return sizeof(*this);
}

Model::OpModelI::OpModelI(OpEnvI* env)
	: OpImplBase('M', env),
	mname_("Model_" + std::to_string(idx_))
{
	init();
}

Model::OpModelI::OpModelI(OpEnvI* env, OpStr name)
	: OpImplBase('M', env),
	mname_(name)
{
	init();
}

Model::OpModelI::~OpModelI()
{
	clear();
}

/* OPUA::Model::OpModel */

void Model::OpModel::add(Constraint::OpLinCon con)
{
	static_cast<OpModelI*>(impl_)->addLinCon(con);
}

void Model::OpModel::add(Constraint::OpLCArr cons)
{
	static_cast<OpModelI*>(impl_)->addLinCons(cons);
}

void Model::OpModel::add(Constraint::OpQuadCon con)
{
	static_cast<OpModelI*>(impl_)->addQuadCon(con);
}

void Model::OpModel::add(Constraint::OpQCArr cons)
{
	static_cast<OpModelI*>(impl_)->addQuadCons(cons);
}

void Model::OpModel::add(Constraint::OpConicCon con)
{
	static_cast<OpModelI*>(impl_)->addConicCon(con);
}

void Model::OpModel::add(Constraint::OpConicArr cons)
{
	static_cast<OpModelI*>(impl_)->addConicCons(cons);
}

void Model::OpModel::add(Constraint::OpPSDCon con)
{
	static_cast<OpModelI*>(impl_)->addPSDCon(con);
}

void Model::OpModel::add(Constraint::OpPSDCArr cons)
{
	static_cast<OpModelI*>(impl_)->addPSDCons(cons);
}

void Model::OpModel::add(Constraint::OpSOSCon con)
{
	static_cast<OpModelI*>(impl_)->addSOS(con);
}

void Model::OpModel::add(Constraint::OpSOSArr cons)
{
	static_cast<OpModelI*>(impl_)->addSOSs(cons);
}

void Model::OpModel::add(Constraint::OpNLCon con)
{
	static_cast<OpModelI*>(impl_)->addNLCon(con);
}

void Model::OpModel::add(Constraint::OpNLCArr cons)
{
	static_cast<OpModelI*>(impl_)->addNLCons(cons);
}

void Model::OpModel::add(Constraint::OpCdtCon con)
{
	static_cast<OpModelI*>(impl_)->addCdtCon(con);
}

void Model::OpModel::add(Constraint::OpCCArr cons)
{
	static_cast<OpModelI*>(impl_)->addCdtCons(cons);
}

void Model::OpModel::setObj(Objective::OpObj obj)
{
	static_cast<OpModelI*>(impl_)->setObj(obj);
}

Objective::OpObj Model::OpModel::getObj() const
{
	return static_cast<OpModelI*>(impl_)->getObj();
}

void Model::OpModel::remove(Constraint::OpLinCon con)
{
	static_cast<OpModelI*>(impl_)->removeLinCon(con);
}

void Model::OpModel::remove(Constraint::OpLCArr cons)
{
	static_cast<OpModelI*>(impl_)->removeLinCons(cons);
}

void Model::OpModel::remove(Constraint::OpQuadCon con)
{
	static_cast<OpModelI*>(impl_)->removeQuadCon(con);
}

void Model::OpModel::remove(Constraint::OpQCArr cons)
{
	static_cast<OpModelI*>(impl_)->removeQuadCons(cons);
}

void Model::OpModel::remove(Constraint::OpConicCon con)
{
	static_cast<OpModelI*>(impl_)->removeConicCon(con);
}

void Model::OpModel::remove(Constraint::OpConicArr cons)
{
	static_cast<OpModelI*>(impl_)->removeConicCons(cons);
}

void Model::OpModel::remove(Constraint::OpPSDCon con)
{
	static_cast<OpModelI*>(impl_)->removePSDCon(con);
}

void Model::OpModel::remove(Constraint::OpPSDCArr cons)
{
	static_cast<OpModelI*>(impl_)->removePSDCons(cons);
}

void Model::OpModel::remove(Constraint::OpSOSCon con)
{
	static_cast<OpModelI*>(impl_)->removeSOS(con);
}

void Model::OpModel::remove(Constraint::OpSOSArr cons)
{
	static_cast<OpModelI*>(impl_)->removeSOSs(cons);
}

void Model::OpModel::remove(Constraint::OpNLCon con)
{
	static_cast<OpModelI*>(impl_)->removeNLCon(con);
}

void Model::OpModel::remove(Constraint::OpNLCArr cons)
{
	static_cast<OpModelI*>(impl_)->removeNLCons(cons);
}

void Model::OpModel::remove(Constraint::OpCdtCon con)
{
	static_cast<OpModelI*>(impl_)->removeCdtCon(con);
}

void Model::OpModel::remove(Constraint::OpCCArr cons)
{
	static_cast<OpModelI*>(impl_)->removeCdtCons(cons);
}

void Model::OpModel::write(OpStr path) const
{
	static_cast<OpModelI*>(impl_)->write(path);
}

OpULInt Model::OpModel::getSize(Variable::OpVar flag) const
{
	return static_cast<OpModelI*>(impl_)->getSize(flag);
}

OpULInt Model::OpModel::getSize(Variable::OpPSDVar flag) const
{
	return static_cast<OpModelI*>(impl_)->getSize(flag);
}

OpULInt Model::OpModel::getSize(Constraint::OpLinCon flag) const
{
	return static_cast<OpModelI*>(impl_)->getSize(flag);
}

OpULInt Model::OpModel::getSize(Constraint::OpQuadCon flag) const
{
	return static_cast<OpModelI*>(impl_)->getSize(flag);
}

OpULInt Model::OpModel::getSize(Constraint::OpConicCon flag) const
{
	return static_cast<OpModelI*>(impl_)->getSize(flag);
}

OpULInt Model::OpModel::getSize(Constraint::OpPSDCon flag) const
{
	return static_cast<OpModelI*>(impl_)->getSize(flag);
}

OpULInt Model::OpModel::getSize(Constraint::OpSOSCon flag) const
{
	return static_cast<OpModelI*>(impl_)->getSize(flag);
}

OpULInt Model::OpModel::getSize(Constraint::OpNLCon flag) const
{
	return static_cast<OpModelI*>(impl_)->getSize(flag);
}

OpULInt Model::OpModel::getSize(Constraint::OpCdtCon flag) const
{
	return static_cast<OpModelI*>(impl_)->getSize(flag);
}

OpULInt Model::OpModel::getSize(Objective::OpObj flag) const
{
	return static_cast<OpModelI*>(impl_)->getSize(flag);
}

void Model::OpModel::setName(OpStr name)
{
	static_cast<OpModelI*>(impl_)->setName(name);
}

OpStr Model::OpModel::getName() const
{
	return static_cast<OpModelI*>(impl_)->getName();
}

Model::OpModelI* Model::OpModel::getImpl() const
{
	return static_cast<OpModelI*>(impl_);
}

void Model::OpModel::release()
{
	// 这里重写了release()方法，因为OpModelI中包含了OPUA组件
	// 必须先执行preRelease()减少这些组件的引用计数，保证OpModelI调用析构函数时，能正常释放这些组件
	if (impl_)
	{
		static_cast<OpModelI*>(impl_)->preRelease();
		static_cast<OpModelI*>(impl_)->release();
		impl_ = nullptr;
	}
}

Variable::OpVarIdxDict::OpDictCIter Model::OpModel::getCBegin(Variable::OpVar flag) const
{
	return static_cast<OpModelI*>(impl_)->mvars_.getCBegin();
}

Variable::OpVarIdxDict::OpDictCIter Model::OpModel::getCEnd(Variable::OpVar flag) const
{
	return static_cast<OpModelI*>(impl_)->mvars_.getCEnd();
}

Variable::OpPSDVIdxDict::OpDictCIter Model::OpModel::getCBegin(Variable::OpPSDVar flag) const
{
	return static_cast<OpModelI*>(impl_)->mpvs_.getCBegin();
}

Variable::OpPSDVIdxDict::OpDictCIter Model::OpModel::getCEnd(Variable::OpPSDVar flag) const
{
	return static_cast<OpModelI*>(impl_)->mpvs_.getCEnd();
}

Constraint::OpLCIdxDict::OpDictCIter Model::OpModel::getCBegin(Constraint::OpLinCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mlcs_.getCBegin();
}

Constraint::OpLCIdxDict::OpDictCIter Model::OpModel::getCEnd(Constraint::OpLinCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mlcs_.getCEnd();
}

Constraint::OpQCIdxDict::OpDictCIter Model::OpModel::getCBegin(Constraint::OpQuadCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mqcs_.getCBegin();
}

Constraint::OpQCIdxDict::OpDictCIter Model::OpModel::getCEnd(Constraint::OpQuadCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mqcs_.getCEnd();
}

Constraint::OpConicIdxDict::OpDictCIter Model::OpModel::getCBegin(Constraint::OpConicCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mcocs_.getCBegin();
}

Constraint::OpConicIdxDict::OpDictCIter Model::OpModel::getCEnd(Constraint::OpConicCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mcocs_.getCEnd();
}

Constraint::OpPSDCIdxDict::OpDictCIter Model::OpModel::getCBegin(Constraint::OpPSDCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mpcs_.getCBegin();
}

Constraint::OpPSDCIdxDict::OpDictCIter Model::OpModel::getCEnd(Constraint::OpPSDCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mpcs_.getCEnd();
}

Constraint::OpSOSIdxDict::OpDictCIter Model::OpModel::getCBegin(Constraint::OpSOSCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mscs_.getCBegin();
}

Constraint::OpSOSIdxDict::OpDictCIter Model::OpModel::getCEnd(Constraint::OpSOSCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mscs_.getCEnd();
}

Constraint::OpNLCIdxDict::OpDictCIter Model::OpModel::getCBegin(Constraint::OpNLCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mnlcs_.getCBegin();
}

Constraint::OpNLCIdxDict::OpDictCIter Model::OpModel::getCEnd(Constraint::OpNLCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mnlcs_.getCEnd();
}

Constraint::OpCCIdxDict::OpDictCIter Model::OpModel::getCBegin(Constraint::OpCdtCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mccs_.getCBegin();
}

Constraint::OpCCIdxDict::OpDictCIter Model::OpModel::getCEnd(Constraint::OpCdtCon flag) const
{
	return static_cast<OpModelI*>(impl_)->mccs_.getCEnd();
}

Objective::OpObjIdxDict::OpDictCIter Model::OpModel::getCBegin(Objective::OpObj flag) const
{
	return static_cast<OpModelI*>(impl_)->mobjs_.getCBegin();
}

Objective::OpObjIdxDict::OpDictCIter Model::OpModel::getCEnd(Objective::OpObj flag) const
{
	return static_cast<OpModelI*>(impl_)->mobjs_.getCEnd();
}

OpBool Model::OpModel::operator==(const OpModel& model) const
{
	return impl_ == model.impl_;
}

OpBool Model::OpModel::operator!=(const OpModel& model) const
{
	return impl_ != model.impl_;
}

Model::OpModel::OpModel()
{

}

Model::OpModel::OpModel(OpModelI* impl)
{
	impl_ = impl;
}

Model::OpModel::OpModel(OpEnv env)
{
	impl_ = new OpModelI(env.getImpl());
}

Model::OpModel::OpModel(OpEnv env, OpStr name)
{
	impl_ = new OpModelI(env.getImpl(), name);
}

Model::OpModel::~OpModel()
{

}
