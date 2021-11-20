#include "OPUA.Model.h"
#include <iostream>

using namespace OPUA;

/* OPUA::Model::OpModelI */

class Model::OpModelI
	: public OpImplBase
{
protected:
	Variable::OpVarIdxDict mvars_; // 优化模型决策变量集
	Container::OpDict<OpLInt, OpULInt> mvrc_; // 决策变量引用计数
	Constraint::OpLCIdxDict mlcs_; // 优化模型线性约束集
	Constraint::OpQCIdxDict mqcs_; // 优化模型二次约束集
	Constraint::OpSOSIdxDict mscs_; // 优化模型SOS约束集
	Constraint::OpNLCIdxDict mnlcs_; // 优化模型非线性约束集
	Objective::OpObj mobj0_; // 优化模型目标函数(单目标优化)
	Objective::OpObjIdxDict mobjs_; // 优化模型目标函数集(多目标优化)
	OpStr mname_; // 优化模型名称

	friend class OpModel;
	friend class OpMdlCIter;
private:
	void init(); // 初始化
	void clear(); // 清空模型
	void addVar(Variable::OpVar var); // 添加变量
	void removeVar(Variable::OpVar var); // 移除变量(引用计数归零才真正删除)
	void addVarsFromLE(const Expression::OpLinExpr& expr); // 通过表达式添加变量(用于添加约束条件/目标函数时检查变量添加)
	void addVarsFromQE(const Expression::OpQuadExpr& expr); // 通过表达式添加变量(用于添加约束条件/目标函数时检查变量添加)
	void removeVarsFromLE(const Expression::OpLinExpr& expr); // 通过表达式移除变量
	void removeVarsFromQE(const Expression::OpQuadExpr& expr); // 通过表达式移除变量
protected:
	void addLinCon(Constraint::OpLinCon con); // 添加线性约束
	void addLinCons(Constraint::OpLCArr cons); // 添加线性约束集
	void addQuadCon(Constraint::OpQuadCon con); // 添加二次约束
	void addQuadCons(Constraint::OpQCArr cons); // 添加二次约束集
	void addSOS(Constraint::OpSOSCon con); // 添加SOS约束
	void addSOSs(Constraint::OpSOSArr cons); // 添加SOS约束集
	void addNLCon(Constraint::OpNLCon con); // 添加非线性约束
	void addNLCons(Constraint::OpNLCArr cons); // 添加非线性约束集
	void setObj(Objective::OpObj obj); // 设置单目标优化目标函数
	Objective::OpObj getObj(); // 获取单目标优化目标函数
	void addMultiObj(Objective::OpObj obj); // 添加多目标优化目标函数
	void addMultiObj(Objective::OpObjArr objs); // 添加多目标优化目标函数
	
	void removeLinCon(Constraint::OpLinCon con); // 移除线性约束
	void removeLinCons(Constraint::OpLCArr cons); // 移除线性约束集
	void removeQuadCon(Constraint::OpQuadCon con); // 移除二次约束
	void removeQuadCons(Constraint::OpQCArr cons); // 移除二次约束集
	void removeSOS(Constraint::OpSOSCon con); // 移除SOS约束
	void removeSOSs(Constraint::OpSOSArr cons); // 移除SOS约束集
	void removeNLCon(Constraint::OpNLCon con); // 移除非线性约束
	void removeNLCons(Constraint::OpNLCArr cons); // 移除非线性约束集
	void removeMultiObj(Objective::OpObj obj); // 移除多目标优化目标函数
	void removeMultiObj(Objective::OpObjArr objs); // 移除多目标优化目标函数

	void setName(OpStr name); // 设置名称
	OpStr getName() const; // 获取名称
	void write(OpStr path) const; // 输出模型
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
	mvrc_ = Container::OpDict<OpLInt, OpULInt>(localEnv);
	mlcs_ = Constraint::OpLCIdxDict(localEnv);
	mqcs_ = Constraint::OpQCIdxDict(localEnv);
	mscs_ = Constraint::OpSOSIdxDict(localEnv);
	mnlcs_ = Constraint::OpNLCIdxDict(localEnv);
	mobjs_ = Objective::OpObjIdxDict(localEnv);
}

void Model::OpModelI::clear()
{
	mvars_.release();
	mvrc_.release();
	mlcs_.release();
	mqcs_.release();
	mscs_.release();
	mnlcs_.release();
	mobjs_.release();
}

void Model::OpModelI::addVar(Variable::OpVar var)
{
	auto idx(var.getIndex());
	mvars_.add(idx, var);
	mvrc_[idx]++;
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

void Model::OpModelI::addLinCon(Constraint::OpLinCon con)
{
	auto idx(con.getIndex());
	if (!mlcs_.has(idx))
	{
		addVarsFromLE(con.getExpr(true));
		if (!con.isStandard())
			addVarsFromLE(con.getExpr(false));
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
		addVarsFromQE(con.getExpr(true));
		if (!con.isStandard())
			addVarsFromQE(con.getExpr(false));
		mqcs_.add(idx, con);
		con.lock();
	}
}

void Model::OpModelI::addQuadCons(Constraint::OpQCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		addQuadCon(cons[i]);
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
		for (auto iter = con.getCBegin(); iter != con.getCEnd(); ++iter)
			addVar(iter.getVal());
		mnlcs_.add(idx, con);
		con.lock();
	}
}

void Model::OpModelI::addNLCons(Constraint::OpNLCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		addNLCon(cons[i]);
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

Objective::OpObj Model::OpModelI::getObj()
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
		removeVarsFromLE(con.getExpr(true));
		if (!con.isStandard())
			removeVarsFromLE(con.getExpr(false));
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
		removeVarsFromQE(con.getExpr(true));
		if (!con.isStandard())
			removeVarsFromQE(con.getExpr(false));
	}
}

void Model::OpModelI::removeQuadCons(Constraint::OpQCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		removeQuadCon(cons[i]);
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
		for (auto iter = con.getCBegin(); iter != con.getCEnd(); ++iter)
			removeVar(iter.getVal());
	}
}

void Model::OpModelI::removeNLCons(Constraint::OpNLCArr cons)
{
	for (OpULInt i = 0; i < cons.getSize(); i++)
		removeNLCon(cons[i]);
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
	auto& stream(std::cout);
	stream << "Name: " << mname_ << std::endl;
	stream << mobj0_ << std::endl;
	for (auto iter = mlcs_.getCBegin(); iter != mlcs_.getCEnd(); ++iter)
		stream << iter.getVal().getName() << ":\t" << iter.getVal() << std::endl;
	for (auto iter = mqcs_.getCBegin(); iter != mqcs_.getCEnd(); ++iter)
		stream << iter.getVal().getName() << ":\t" << iter.getVal() << std::endl;
	for (auto iter = mscs_.getCBegin(); iter != mscs_.getCEnd(); ++iter)
		stream << iter.getVal().getName() << ":\t" << iter.getVal() << std::endl;
	stream << std::endl << "Bounds: " << std::endl;
	for (auto iter = mvars_.getCBegin(); iter != mvars_.getCEnd(); ++iter)
	{
		auto& var(iter.getVal());
		stream << var.getName() << ":\t" << var.getLb() << " <= " << var.getUb() << std::endl;
	}

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

void Model::OpModel::add(Constraint::OpSOSCon con)
{
	static_cast<OpModelI*>(impl_)->addSOS(con);
}

void Model::OpModel::add(Constraint::OpSOSArr cons)
{
	static_cast<OpModelI*>(impl_)->addSOSs(cons);
}

void OPUA::Model::OpModel::add(Constraint::OpNLCon con)
{
	static_cast<OpModelI*>(impl_)->addNLCon(con);
}

void OPUA::Model::OpModel::add(Constraint::OpNLCArr cons)
{
	static_cast<OpModelI*>(impl_)->addNLCons(cons);
}

void Model::OpModel::setObj(Objective::OpObj obj)
{
	static_cast<OpModelI*>(impl_)->setObj(obj);
}

Objective::OpObj Model::OpModel::getObj()
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

void Model::OpModel::remove(Constraint::OpSOSCon con)
{
	static_cast<OpModelI*>(impl_)->removeSOS(con);
}

void Model::OpModel::remove(Constraint::OpSOSArr cons)
{
	static_cast<OpModelI*>(impl_)->removeSOSs(cons);
}

void OPUA::Model::OpModel::remove(Constraint::OpNLCon con)
{
	static_cast<OpModelI*>(impl_)->removeNLCon(con);
}

void OPUA::Model::OpModel::remove(Constraint::OpNLCArr cons)
{
	static_cast<OpModelI*>(impl_)->removeNLCons(cons);
}

void Model::OpModel::write(OpStr path) const
{
	static_cast<OpModelI*>(impl_)->write(path);
}

void OPUA::Model::OpModel::setName(OpStr name)
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

Variable::OpVarIdxDict::OpDictCIter Model::OpModel::getVBegin()
{
	return static_cast<OpModelI*>(impl_)->mvars_.getCBegin();
}

Variable::OpVarIdxDict::OpDictCIter Model::OpModel::getVEnd()
{
	return static_cast<OpModelI*>(impl_)->mvars_.getCEnd();
}

Constraint::OpLCIdxDict::OpDictCIter Model::OpModel::getLCBegin()
{
	return static_cast<OpModelI*>(impl_)->mlcs_.getCBegin();
}

Constraint::OpLCIdxDict::OpDictCIter Model::OpModel::getLCEnd()
{
	return static_cast<OpModelI*>(impl_)->mlcs_.getCEnd();
}

Constraint::OpQCIdxDict::OpDictCIter Model::OpModel::getQCBegin()
{
	return static_cast<OpModelI*>(impl_)->mqcs_.getCBegin();
}

Constraint::OpQCIdxDict::OpDictCIter Model::OpModel::getQCEnd()
{
	return static_cast<OpModelI*>(impl_)->mqcs_.getCEnd();
}

Constraint::OpSOSIdxDict::OpDictCIter Model::OpModel::getSOSBegin()
{
	return static_cast<OpModelI*>(impl_)->mscs_.getCBegin();
}

Constraint::OpSOSIdxDict::OpDictCIter Model::OpModel::getSOSEnd()
{
	return static_cast<OpModelI*>(impl_)->mscs_.getCEnd();
}

Constraint::OpNLCIdxDict::OpDictCIter Model::OpModel::getNLCBegin()
{
	return static_cast<OpModelI*>(impl_)->mnlcs_.getCBegin();
}

Constraint::OpNLCIdxDict::OpDictCIter Model::OpModel::getNLCEnd()
{
	return static_cast<OpModelI*>(impl_)->mnlcs_.getCEnd();
}

Objective::OpObjIdxDict::OpDictCIter Model::OpModel::getOBegin()
{
	return static_cast<OpModelI*>(impl_)->mobjs_.getCBegin();
}

Objective::OpObjIdxDict::OpDictCIter Model::OpModel::getOEnd()
{
	return static_cast<OpModelI*>(impl_)->mobjs_.getCEnd();
}

OpBool Model::OpModel::operator==(const OpModel& model)
{
	return impl_ == model.getImpl();
}

OpBool Model::OpModel::operator!=(const OpModel& model)
{
	return impl_ != model.getImpl();
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

OPUA::Model::OpModel::~OpModel()
{

}
