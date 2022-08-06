#include "OPUA.Common.h"
#include "OPUA.Algorithm.h"
#include "OPUA.Math.h"
#include "OPUA.Exception.h"
#include <thread>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>
using namespace OPUA;

/* OPUA::Algorithm::OpRobustModelI */

class Algorithm::OpRobustModelI
	: public OpImplBase
{
protected:
	// ģ������
	OpStr mname_;
	// �Ƿ���ִ���Զ���ż�Ƶ�
	OpBool adual_;
protected:
	// ��һ�׶�Ŀ�꺯��
	Expression::OpLinExpr fso_;
	// ��һ�׶�Լ��������
	Constraint::OpLCArr fsc_;
	// ��һ�ڶ�����Լ��������(������⣬�����ڵ�һ�׶�Լ�������ڹ�ͨ��һ�׶���ڶ��׶�)
	Constraint::OpLCArr fssc_;
	Constraint::OpQCArr fssqc_;
	// ��һ�׶ξ��߱�����
	Variable::OpVarArr fsv_;
	// ��һ�׶�������߱�����(�����ڵ�һ�׶ξ��߱�������)
	Variable::OpVarArr fssv_;
	// �ڶ��׶�ԭ����Ŀ�꺯��
	Expression::OpLinExpr sspo_;
	// �ڶ��׶�ԭ����Լ����
	Constraint::OpLCArr sspc_;
	// �ڶ��׶�ԭ���������
	Variable::OpVarArr sspv_;
	// ��żĿ�꺯��
	Expression::OpQuadExpr ssdo_;
	// �ڶ��׶ζ�ż����Լ����
	Constraint::OpLCArr ssdc_;
	// �ڶ��׶ζ�ż���������
	Variable::OpVarArr ssdv_;
	// �ڶ��׶�ԭ�����ʶ��(���ڱ���true��ʾ�Ǹ���false��ʾ�����ƣ�����Լ��true��ʾ����ʽ��false��ʾ��ʽ)
	Container::OpDict<Variable::OpVarI*, OpBool> sspfv_;
	Container::OpDict<Constraint::OpLinConI*, OpBool> sspfc_;
	// �ڶ��׶ζ�ż�����ʶ��(���ڱ���true��ʾ�Ǹ���false��ʾ�����ƣ�����Լ��true��ʾ����ʽ��false��ʾ��ʽ)
	Container::OpDict<Variable::OpVarI*, OpBool> ssdfv_;
	Container::OpDict<Constraint::OpLinConI*, OpBool> ssdfc_;
	// �ڶ��׶�ԭʼ-��żӳ���(ԭʼԼ��->��ż��������żԼ��->ԭʼ����)
	Container::OpDict<Constraint::OpLinConI*, Variable::OpVarI*> sspdm_;
	// ��ȷ��������
	Variable::OpVarArr uv_;
	// ��ȷ����Լ��
	Constraint::OpLCArr uc_;
protected:
	// �������½�(����->{�Ͻ�, �½�})
	Container::OpDict<Variable::OpVarI*, std::pair<OpFloat, OpFloat> > vbd_;
	// ������ȷBound�ı�����
	std::set<Variable::OpVarI*> ebdvs_;
protected:
	// ��һ�׶ξ��߱����Ľ�
	Container::OpFloatArr fss_;
	// �ڶ��׶�ԭ������߱����Ľ�
	Container::OpFloatArr ssps_;
	// �ڶ��׶ζ�ż������߱����Ľ�
	Container::OpFloatArr ssds_;
	// ��ȷ�����Ľ�
	Container::OpFloatArr us_;
	// ��һ�׶�Ŀ�꺯���Ľ�
	OpFloat fsos_;
	// �ڶ��׶�Ŀ�꺯���Ľ�
	OpFloat ssos_;

	friend class Algorithm::OpRobustModel;
	friend class Algorithm::OpAlgoCCG;
protected:
	// ��ʼ��(�������캯��ʹ��)
	void init();
	// ���ģ��(��������������ʹ��)
	void clear();
	// ���׶��Զ�ִ��Լ���ı�׼��(��autoDual֮ǰִ��)
	void autoStd(RobustStageType type);
	// �Զ��Ƶ���ż
	void autoDual();
	// �Զ��Ƶ����������½�
	void autoBound();
	// ����ģ��(��ʼ���⼯��)
	void update();
	// ��ʼ��������
	void initSolution(RobustStageType type);
	// ��ӱ���&Լ��
	void add(Variable::OpVarArr vars, RobustStageType type, OpBool flag);
	void add(Variable::OpVar var, RobustStageType type, OpBool flag);
	void add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag);
	void add(Constraint::OpLinCon con, RobustStageType type, OpBool flag);
	void add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag);
	void add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag);
	// ����Ŀ�꺯��
	void setObj(const Expression::OpLinExpr& expr, RobustStageType type);
	void setObj(const Expression::OpQuadExpr& expr, RobustStageType type);
	// ���ý�
	void setValue(Variable::OpVar var, RobustStageType type, OpFloat val);
	// ���ñ߽�
	void setBound(Variable::OpVar var, RobustStageType type, OpFloat val, OpBool lb);
	void setBound(Variable::OpVar var, RobustStageType type, OpFloat lbVal, OpFloat ubVal);
	// ��ȡ��
	OpFloat getValue(Variable::OpVar var, RobustStageType type) const;
	// ��ȡĿ�꺯����
	OpFloat getObjValue(RobustStageType type) const;
	// ��ȡ����&Լ����Ŀ
	OpULInt getSize(RobustStageType type, OpBool isVar) const;
	// ��ȡ�߽�
	OpFloat getBound(Variable::OpVar var, RobustStageType type, OpBool lb) const;
	// ����ģ��
	void write(OpStr root);
	// �ͷ�ǰ׼��
	void preRelease();
	// ��ȡ�ڴ�ռ��
	virtual OpULInt getMemoryUsage() const;
protected:
	// ���캯��
	OpRobustModelI(OpEnvI* env);
	OpRobustModelI(OpEnvI* env, OpStr name);
public:
	// ��������
	virtual ~OpRobustModelI();
};

void Algorithm::OpRobustModelI::init()
{
	OpEnv localEnv(env_);
	// ��һ�׶�ģ��
	fso_ = Expression::OpLinExpr(0.0);
	fsc_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(fsc_.getImpl());
	fssc_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(fssc_.getImpl());
	fssqc_ = Constraint::OpQCArr(localEnv);
	localEnv.addManagement(fssqc_.getImpl());
	fsv_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(fsv_.getImpl());
	fssv_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(fssv_.getImpl());
	// �ڶ��׶�ģ��
	sspo_ = Expression::OpLinExpr(0.0);
	sspc_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(sspc_.getImpl());
	sspv_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(sspv_.getImpl());
	ssdo_ = Expression::OpQuadExpr(0.0);
	ssdc_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(ssdc_.getImpl());
	ssdv_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(ssdv_.getImpl());
	sspfv_ = Container::OpDict<Variable::OpVarI*, OpBool>(localEnv);
	localEnv.addManagement(sspfv_.getImpl());
	sspfc_ = Container::OpDict<Constraint::OpLinConI*, OpBool>(localEnv);
	localEnv.addManagement(sspfc_.getImpl());
	ssdfv_ = Container::OpDict<Variable::OpVarI*, OpBool>(localEnv);
	localEnv.addManagement(ssdfv_.getImpl());
	ssdfc_ = Container::OpDict<Constraint::OpLinConI*, OpBool>(localEnv);
	localEnv.addManagement(ssdfc_.getImpl());
	sspdm_ = Container::OpDict<Constraint::OpLinConI*, Variable::OpVarI*>(localEnv);
	localEnv.addManagement(sspdm_.getImpl());
	// ��ȷ����ģ��
	uv_ = Variable::OpVarArr(localEnv);
	localEnv.addManagement(uv_.getImpl());
	uc_ = Constraint::OpLCArr(localEnv);
	localEnv.addManagement(uc_.getImpl());
	// �����߽�
	vbd_ = Container::OpDict<Variable::OpVarI*, std::pair<OpFloat, OpFloat> >(localEnv);
	localEnv.addManagement(vbd_.getImpl());
	ebdvs_ = std::set<Variable::OpVarI*>();
	// ģ�ͽ�
	fss_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(fss_.getImpl());
	ssps_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(ssps_.getImpl());
	ssds_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(ssds_.getImpl());
	us_ = Container::OpFloatArr(localEnv);
	localEnv.addManagement(us_.getImpl());
	fsos_ = 0.0;
	ssos_ = 0.0;
	// ����
	adual_ = false;
}

void Algorithm::OpRobustModelI::clear()
{
	// ��һ�׶�ģ��
	fso_.clear();
	fsc_.release();
	fssc_.release();
	fssqc_.release();
	fsv_.release();
	fssv_.release();
	// �ڶ��׶�ģ��
	sspo_.clear();
	sspc_.release();
	sspv_.release();
	ssdo_.clear();
	if (adual_)
	{
		ssdc_.releaseElements();
		ssdv_.releaseElements();
	}
	ssdc_.release();
	ssdv_.release();
	sspfv_.release();
	sspfc_.release();
	ssdfv_.release();
	ssdfc_.release();
	sspdm_.release();
	// ��ȷ����ģ��
	uv_.release();
	uc_.release();
	// �����߽�
	vbd_.release();
	ebdvs_.clear();
	// ģ�ͽ�
	fss_.release();
	ssps_.release();
	ssds_.release();
	us_.release();
}

void Algorithm::OpRobustModelI::autoStd(RobustStageType type)
{
	auto stdCvt = [](auto& conArray) {
		for (OpULInt i = 0; i < conArray.getSize(); i++)
		{
			auto& con(conArray[i]);
			if (con.isLocked())
			{
				con.unlock();
				con.standardize();
				con.lock();
			}
			else
				con.standardize();
		}
	};
	switch (type)
	{
	case OPUA::Algorithm::RobustStageType::Unknown:
		break;
	case OPUA::Algorithm::RobustStageType::FirstStage:
		stdCvt(fsc_);
		break;
	case OPUA::Algorithm::RobustStageType::FirstStageSpecial:
		stdCvt(fssc_);
		break;
	case OPUA::Algorithm::RobustStageType::SecondStagePrimal:
		stdCvt(sspc_);
		break;
	case OPUA::Algorithm::RobustStageType::SecondStageDual:
		stdCvt(ssdc_);
		break;
	case OPUA::Algorithm::RobustStageType::Uncertainty:
		stdCvt(uc_);
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::autoDual()
{
	// ִ�иú�����Ҫ������Ϊ��������(���԰���������������ر��ʽ��ҪΪ����)
	// ���Եڶ��׶�ԭʼ������ж�ż�Ƶ�
	// �ֶ����ã�sspo_, sspc_, sspv_, sspfc_, sspfv_
	// �Զ����ɣ�ssdo_, ssdc_, ssdv_, ssdfc_, ssdfv_, sspdm_

	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;

	// ��������
	OpEnv localEnv(env_);
	std::thread::id tid = std::this_thread::get_id();
	std::cout << "[OPUA::Algorithm::OpRobustModel::autoDual](Thread: " << tid << ")MemoryUsage(start): " << localEnv.getMemoryUsage() << std::endl;
	// (��Ҫ�ú������)��ʱ����
	OpULInt numLambda(0), numMu(0), numNeq(0), numEq(0); // ͳ��ֵ
	std::vector<OpVar> lambda; // ����ʽԼ����ż������(����Ҫ�ͷ�)
	std::vector<OpVar> mu; // ��ʽԼ����ż������(����Ҫ�ͷ�)
	std::unordered_map<OpVarI*, std::set<OpULInt> > primalVarAtCon; // ���߱������ֵ�Լ����Χ��ѯ��
	std::unordered_map<OpVarI*, OpFloat> primalObjCoefMap; // Ŀ�꺯��ϵ����ѯ��
	std::unordered_map<OpLinConI*, OpLinExpr> primalRHS; // �Ҷ�������ѯ��
	// �ػ�����(�Զ��ͷ��ڴ�)
	OpAutoFree guard([&]() {
		std::cout << "[OPUA::Algorithm::OpRobustModel::autoDual](Thread: " << tid << ")MemoryUsage(end): " << localEnv.getMemoryUsage() << std::endl;
	});

	// ������ż���߱���&��ԭʼԼ���������ż���߱���
	for (auto it = sspfc_.getCBegin(); it != sspfc_.getCEnd(); ++it)
	{
		if (it.getVal())
		{
			auto dualVar = OpVar(localEnv, OpVarType::Con, 0, Constant::Infinity, "lambda_" + std::to_string(numLambda));
			lambda.emplace_back(dualVar);
			ssdfv_.add(dualVar.getImpl(), true);
			sspdm_.add(it.getKey(), dualVar.getImpl());
			++numLambda;
		}
		else
		{
			auto dualVar = OpVar(localEnv, OpVarType::Con, -Constant::Infinity, Constant::Infinity, "mu_" + std::to_string(numMu));
			mu.emplace_back(dualVar.getImpl());
			ssdfv_.add(dualVar.getImpl(), false);
			sspdm_.add(it.getKey(), dualVar.getImpl());
			++numMu;
		}
	}
	for (auto it = sspfv_.getCBegin(); it != sspfv_.getCEnd(); ++it)
	{
		if (it.getVal())
			++numNeq;
		else
			++numEq;
	}
	for (const auto& dv : lambda)
		ssdv_.add(dv);
	for (const auto& dv : mu)
		ssdv_.add(dv);
	// �γɾ��߱������ֵ�Լ����Χ��ѯ��
	for (OpULInt i = 0; i < sspc_.getSize(); i++)
		for (auto it = sspc_[i].getExpr().getLBegin(); it != sspc_[i].getExpr().getLEnd(); ++it)
			primalVarAtCon[it.getVar().getImpl()].emplace(i);
	// �γ�ԭʼ����Ŀ�꺯��ϵ����ѯ��
	for (auto it = sspo_.getLBegin(); it != sspo_.getLEnd(); ++it)
		primalObjCoefMap.emplace(it.getVar().getImpl(), it.getCoeff());
	// �γ�ԭʼ�����Ҷ�������ѯ��(����Ҷ���������ֻ��LB�������ܰ����ɷǵڶ��׶ξ��߱������ɵı��ʽ)
	for (OpULInt i = 0; i < sspc_.getSize(); i++)
	{
		const auto& primalCon = sspc_[i];
		auto& rhs = primalRHS.emplace(primalCon.getImpl(), 0.0).first->second;
		rhs.addLinTerm(primalCon.getLb());
		for (auto it = primalCon.getExpr().getLBegin(); it != primalCon.getExpr().getLEnd(); ++it)
		{
			auto var = it.getVar();
			if (!sspfv_.has(var.getImpl()))
				rhs.addLinTerm(var, -it.getCoeff());
		}
	}
	// ���ݵڶ��׶�ԭʼ������߱�����������Ƶ���żԼ������
	for (OpULInt i = 0; i < sspv_.getSize(); i++)
	{
		const auto& primalVar = sspv_[i];
		OpFloat dualRHS = primalObjCoefMap.find(primalVar.getImpl()) != primalObjCoefMap.end() ?
			primalObjCoefMap.at(primalVar.getImpl()) : 0.0;
		OpLinExpr dualExpr(0.0);
		// ����������Ż�������ͳ��ÿ���������ֵ�Լ���ķ�Χ�Լ��ٵ����Ĵ���
		if (primalVarAtCon.find(primalVar.getImpl()) != primalVarAtCon.end())
		{
			for (auto& j : primalVarAtCon.at(primalVar.getImpl()))
			{
				const auto& primalCon = sspc_[j];
				for (auto it = primalCon.getExpr().getLBegin(); it != primalCon.getExpr().getLEnd(); ++it)
				{
					if (it.getVar().getImpl() == primalVar.getImpl())
					{
						dualExpr.addLinTerm(sspdm_.getVal(primalCon.getImpl()), it.getCoeff());
						break;
					}
				}
			}
		}
		if (sspfv_.getVal(primalVar.getImpl()))
		{
			OpLinCon dualCon(localEnv, -Constant::Infinity, dualExpr, dualRHS);
			ssdfc_.add(dualCon.getImpl(), true);
			sspdm_.add(dualCon.getImpl(), primalVar.getImpl());
			ssdc_.add(dualCon);
		}
		else
		{
			OpLinCon dualCon(localEnv, dualRHS, dualExpr, dualRHS);
			ssdfc_.add(dualCon.getImpl(), false);
			sspdm_.add(dualCon.getImpl(), primalVar.getImpl());
			ssdc_.add(dualCon);
		}
	}
	// �����Ҷ������Ƶ���żĿ�꺯��(���ܲ���������)
	// �����������Զ����򣬱������ϵ��Ϊ0
	for (OpULInt i = 0; i < sspc_.getSize(); i++)
	{
		const auto& primalCon = sspc_[i];
		ssdo_ += primalRHS.at(primalCon.getImpl()) * OpVar(sspdm_.getVal(primalCon.getImpl()));
	}
	ssdo_.simplify();
}

void Algorithm::OpRobustModelI::autoBound()
{
	// �Զ��Ƶ�ԭʼ����/��ż���������½�
	// Ŀǰ��ԭ���ǽ���Լ����ʽ��Bound��¼����

	// ��������
	OpEnv localEnv(env_);
	std::thread::id tid = std::this_thread::get_id();
	std::cout << "[OPUA::Algorithm::OpRobustModel::autoBound](Thread: " << tid << ")MemoryUsage(start): " << localEnv.getMemoryUsage() << std::endl;
	// �ػ�����(�Զ��ͷ��ڴ�)
	OpAutoFree guard([&]() {
		std::cout << "[OPUA::Algorithm::OpRobustModel::autoBound](Thread: " << tid << ")MemoryUsage(end): " << localEnv.getMemoryUsage() << std::endl;
	});
	// ���ȸ��ݱ��������lb��ub����ʼ��
	auto init = [&](auto& varArray) {
		for (OpULInt i = 0; i < varArray.getSize(); i++)
		{
			auto& var = varArray[i];
			vbd_[var.getImpl()] = { var.getLb(), var.getUb() };
		}
	};
	init(fsv_);
	init(sspv_);
	init(ssdv_);
	init(uv_);
	// Ȼ��Ѳ�����е�Լ�����ҳ�boundԼ������¼
	auto findBound = [&](auto& conArray) {
		for (OpULInt i = 0; i < conArray.getSize(); i++)
		{
			const auto& con = conArray[i];
			if (con.getExpr().getSize() == 1)
			{
				auto var = con.getExpr().getLBegin().getVar();
				auto coef = con.getExpr().getLBegin().getCoeff();
				auto& bound = vbd_[var.getImpl()];
				if (coef > 0)
				{
					bound.first = Math::OpMax(bound.first, con.getLb() / coef);
					bound.second = Math::OpMin(bound.second, con.getUb() / coef);
				}
				else if (coef < 0)
				{
					bound.first = Math::OpMax(bound.first, con.getUb() / coef);
					bound.second = Math::OpMin(bound.second, con.getLb() / coef);
				}
			}
		}
	};
	findBound(fsc_);
	findBound(sspc_);
	findBound(ssdc_);
	findBound(uc_);
}

void Algorithm::OpRobustModelI::update()
{
	initSolution(RobustStageType::FirstStage);
	initSolution(RobustStageType::SecondStagePrimal);
	initSolution(RobustStageType::SecondStageDual);
	initSolution(RobustStageType::Uncertainty);
}

void Algorithm::OpRobustModelI::initSolution(RobustStageType type)
{
	switch (type)
	{
	case RobustStageType::FirstStage:
		fss_.setSize(fsv_.getSize());
		break;
	case RobustStageType::SecondStagePrimal:
		ssps_.setSize(sspv_.getSize());
		break;
	case RobustStageType::SecondStageDual:
		ssds_.setSize(ssdv_.getSize());
		break;
	case RobustStageType::Uncertainty:
		us_.setSize(uv_.getSize());
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::add(Variable::OpVarArr vars, RobustStageType type, OpBool flag)
{
	switch (type)
	{
	case RobustStageType::FirstStage:
		fsv_.add(vars);
		break;
	case RobustStageType::FirstStageSpecial:
		fssv_.add(vars);
		break;
	case RobustStageType::SecondStagePrimal:
		sspv_.add(vars);
		for (OpULInt i = 0; i < vars.getSize(); i++)
			sspfv_.add(vars[i].getImpl(), flag);
		break;
	case RobustStageType::SecondStageDual:
		ssdv_.add(vars);
		for (OpULInt i = 0; i < vars.getSize(); i++)
			ssdfv_.add(vars[i].getImpl(), flag);
		break;
	case RobustStageType::Uncertainty:
		uv_.add(vars);
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::add(Variable::OpVar var, RobustStageType type, OpBool flag)
{
	switch (type)
	{
	case RobustStageType::FirstStage:
		fsv_.add(var);
		break;
	case RobustStageType::FirstStageSpecial:
		fssv_.add(var);
		break;
	case RobustStageType::SecondStagePrimal:
		sspv_.add(var);
		sspfv_.add(var.getImpl(), flag);
		break;
	case RobustStageType::SecondStageDual:
		ssdv_.add(var);
		ssdfv_.add(var.getImpl(), flag);
		break;
	case RobustStageType::Uncertainty:
		uv_.add(var);
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag)
{
	switch (type)
	{
	case RobustStageType::FirstStage:
		fsc_.add(cons);
		break;
	case RobustStageType::FirstStageSpecial:
		fssc_.add(cons);
		break;
	case RobustStageType::SecondStagePrimal:
		sspc_.add(cons);
		for (OpULInt i = 0; i < cons.getSize(); i++)
			sspfc_.add(cons[i].getImpl(), flag);
		break;
	case RobustStageType::SecondStageDual:
		ssdc_.add(cons);
		for (OpULInt i = 0; i < cons.getSize(); i++)
			ssdfc_.add(cons[i].getImpl(), flag);
		break;
	case RobustStageType::Uncertainty:
		uc_.add(cons);
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::add(Constraint::OpLinCon con, RobustStageType type, OpBool flag)
{
	switch (type)
	{
	case RobustStageType::FirstStage:
		fsc_.add(con);
		break;
	case RobustStageType::FirstStageSpecial:
		fssc_.add(con);
		break;
	case RobustStageType::SecondStagePrimal:
		sspc_.add(con);
		sspfc_.add(con.getImpl(), flag);
		break;
	case RobustStageType::SecondStageDual:
		ssdc_.add(con);
		ssdfc_.add(con.getImpl(), flag);
		break;
	case RobustStageType::Uncertainty:
		uc_.add(con);
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStageSpecial)
		fssqc_.add(cons);
}

void Algorithm::OpRobustModelI::add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStageSpecial)
		fssqc_.add(con);
}

void Algorithm::OpRobustModelI::setObj(const Expression::OpLinExpr& expr, RobustStageType type)
{
	switch (type)
	{
	case RobustStageType::FirstStage:
		fso_ = expr;
		break;
	case RobustStageType::SecondStagePrimal:
		sspo_ = expr;
		break;
	case RobustStageType::SecondStageDual:
		ssdo_ = expr;
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::setObj(const Expression::OpQuadExpr& expr, RobustStageType type)
{
	if (type == RobustStageType::SecondStageDual)
		ssdo_ = expr;
}

void Algorithm::OpRobustModelI::setValue(Variable::OpVar var, RobustStageType type, OpFloat val)
{
	auto setVal = [](auto& varArray, auto& valArray, auto var, auto val) {
		auto index = varArray.find(var); // ����Ҳ�����Ӧ��var��find��������size
		if (valArray.idxValid(index))
			valArray[index] = val;
	};
	switch (type)
	{
	case RobustStageType::FirstStage:
		setVal(fsv_, fss_, var, val);
		break;
	case RobustStageType::SecondStagePrimal:
		setVal(sspv_, ssps_, var, val);
		break;
	case RobustStageType::SecondStageDual:
		setVal(ssdv_, ssds_, var, val);
		break;
	case RobustStageType::Uncertainty:
		setVal(uv_, us_, var, val);
		break;
	default:
		break;
	}
}

void Algorithm::OpRobustModelI::setBound(Variable::OpVar var, RobustStageType type, OpFloat val, OpBool lb)
{
	if (lb)
		vbd_[var.getImpl()].first = val;
	else
		vbd_[var.getImpl()].second = val;
}

void Algorithm::OpRobustModelI::setBound(Variable::OpVar var, RobustStageType type, OpFloat lbVal, OpFloat ubVal)
{
	setBound(var, type, lbVal, true);
	setBound(var, type, ubVal, false);
}

OpFloat Algorithm::OpRobustModelI::getValue(Variable::OpVar var, RobustStageType type) const
{
	OpFloat result(Constant::NaN);
	OpULInt index(Constant::MaxSize);
	switch (type)
	{
	case RobustStageType::FirstStage:
		index = fsv_.find(var);
		if (fss_.idxValid(index))
			result = fss_[index];
		break;
	case RobustStageType::SecondStagePrimal:
		index = sspv_.find(var);
		if (ssps_.idxValid(index))
			result = ssps_[index];
		break;
	case RobustStageType::SecondStageDual:
		index = ssdv_.find(var);
		if (ssds_.idxValid(index))
			result = ssds_[index];
		break;
	case RobustStageType::Uncertainty:
		index = uv_.find(var);
		if (us_.idxValid(index))
			result = us_[index];
		break;
	default:
		break;
	}
	return result;
}

OpFloat Algorithm::OpRobustModelI::getObjValue(RobustStageType type) const
{
	OpFloat result(Constant::NaN);
	switch (type)
	{
	case RobustStageType::Unknown:
		result = fsos_ + ssos_;
		break;
	case RobustStageType::FirstStage:
		result = fsos_;
		break;
	case RobustStageType::SecondStagePrimal:
		result = ssos_;
		break;
	case RobustStageType::SecondStageDual:
		result = ssos_;
		break;
	default:
		break;
	}
	return result;
}

OpULInt Algorithm::OpRobustModelI::getSize(RobustStageType type, OpBool isVar) const
{
	OpULInt result(0);
	switch (type)
	{
	case RobustStageType::FirstStage:
		result = isVar ? fsv_.getSize() : fsc_.getSize();
		break;
	case RobustStageType::FirstStageSpecial:
		result = isVar ? fssv_.getSize() : fssc_.getSize() + fssqc_.getSize();
		break;
	case RobustStageType::SecondStagePrimal:
		result = isVar ? sspv_.getSize() : sspc_.getSize();
		break;
	case RobustStageType::SecondStageDual:
		result = isVar ? ssdv_.getSize() : ssdc_.getSize();
		break;
	case RobustStageType::Uncertainty:
		result = isVar ? uv_.getSize() : uc_.getSize();
		break;
	default:
		break;
	}
	return result;
}

OpFloat Algorithm::OpRobustModelI::getBound(Variable::OpVar var, RobustStageType type, OpBool lb) const
{
	OpFloat bound(Constant::NaN);
	if (vbd_.has(var.getImpl()))
		bound = lb ? vbd_.getVal(var.getImpl()).first : vbd_.getVal(var.getImpl()).second;
	return bound;
}

void Algorithm::OpRobustModelI::write(OpStr root)
{
	using namespace Objective;
	using namespace Model;

	// ��������
	OpEnv localEnv(env_);
	std::thread::id tid = std::this_thread::get_id();
	std::cout << "[OPUA::Algorithm::OpRobustModel::write](Thread: " << tid << ")MemoryUsage(start): " << localEnv.getMemoryUsage() << std::endl;
	// ����ģ���ļ���Ҫ��MODEL�����CPLEX����
	OpModel model1(localEnv), model2(localEnv), model3(localEnv);
	model1.setName(mname_.c_str());
	model2.setName(mname_.c_str());
	model3.setName(mname_.c_str());
	OpObj obj1(OpMinimize(localEnv, fso_)),
		obj2(OpMinimize(localEnv, sspo_)),
		obj3(OpMaximize(localEnv, ssdo_));
	// �ػ�����(�Զ��ͷ��ڴ�)
	OpAutoFree guard([&]() {
		try
		{
			model1.release();
			model2.release();
			model3.release();
			obj1.release();
			obj2.release();
			obj3.release();
			std::cout << "[OPUA::Algorithm::OpRobustModel::write](Thread: " << tid << ")MemoryUsage(end): " << localEnv.getMemoryUsage() << std::endl;
		}
		catch (std::exception& ex)
		{
			std::cerr << ex.what() << std::endl;
		}
	});

	// ������һ�׶κ͵ڶ��׶�ģ��
	model1.setObj(obj1);
	model1.add(fsc_);
	model2.setObj(obj2);
	model2.add(sspc_);
	model3.setObj(obj3);
	model3.add(ssdc_);
	// ����ģ��
	std::string path1(root + "/" + mname_ + "_FS.lp"),
		path2(root + "/" + mname_ + "_SSP.lp"),
		path3(root + "/" + mname_ + "_SSD.lp");
	//model1.write(path1.c_str());
	//model2.write(path2.c_str());
	//model3.write(path3.c_str());
	model1.write("");
	model2.write("");
	model3.write("");
}

void Algorithm::OpRobustModelI::preRelease()
{
	OpEnv localEnv(env_);
	// ��һ�׶�ģ��
	localEnv.removeManagement(fsc_.getImpl());
	localEnv.removeManagement(fssc_.getImpl());
	localEnv.removeManagement(fssqc_.getImpl());
	localEnv.removeManagement(fsv_.getImpl());
	localEnv.removeManagement(fssv_.getImpl());
	// �ڶ��׶�ģ��
	localEnv.removeManagement(sspc_.getImpl());
	localEnv.removeManagement(sspv_.getImpl());
	localEnv.removeManagement(ssdc_.getImpl());
	localEnv.removeManagement(ssdv_.getImpl());
	localEnv.removeManagement(sspfv_.getImpl());
	localEnv.removeManagement(sspfc_.getImpl());
	localEnv.removeManagement(ssdfv_.getImpl());
	localEnv.removeManagement(ssdfc_.getImpl());
	localEnv.removeManagement(sspdm_.getImpl());
	// ��ȷ����ģ��
	localEnv.removeManagement(uv_.getImpl());
	localEnv.removeManagement(uc_.getImpl());
	// �����߽�
	localEnv.removeManagement(vbd_.getImpl());
	// ģ�ͽ�
	localEnv.removeManagement(fss_.getImpl());
	localEnv.removeManagement(ssps_.getImpl());
	localEnv.removeManagement(ssds_.getImpl());
	localEnv.removeManagement(us_.getImpl());
}

OpULInt Algorithm::OpRobustModelI::getMemoryUsage() const
{
	return sizeof(*this);
}

Algorithm::OpRobustModelI::OpRobustModelI(OpEnvI* env)
	: OpImplBase('M', env),
	mname_("RobustModel_" + std::to_string(idx_)),
	adual_(false)
{
	init();
}

Algorithm::OpRobustModelI::OpRobustModelI(OpEnvI* env, OpStr name)
	: OpImplBase('M', env),
	mname_(name),
	adual_(false)
{
	init();
}

Algorithm::OpRobustModelI::~OpRobustModelI()
{
	clear();
}

/* OPUA::Algorithm::OpRobustModel */

void Algorithm::OpRobustModel::autoStd(RobustStageType type)
{
	static_cast<OpRobustModelI*>(impl_)->autoStd(type);
}

void Algorithm::OpRobustModel::autoDual()
{
	static_cast<OpRobustModelI*>(impl_)->autoDual();
}

void Algorithm::OpRobustModel::autoBound()
{
	static_cast<OpRobustModelI*>(impl_)->autoBound();
}

void Algorithm::OpRobustModel::update()
{
	static_cast<OpRobustModelI*>(impl_)->update();
}

void Algorithm::OpRobustModel::add(Variable::OpVarArr vars, RobustStageType type, OpBool flag)
{
	static_cast<OpRobustModelI*>(impl_)->add(vars, type, flag);
}

void Algorithm::OpRobustModel::add(Variable::OpVar var, RobustStageType type, OpBool flag)
{
	static_cast<OpRobustModelI*>(impl_)->add(var, type, flag);
}

void Algorithm::OpRobustModel::add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag)
{
	static_cast<OpRobustModelI*>(impl_)->add(cons, type, flag);
}

void Algorithm::OpRobustModel::add(Constraint::OpLinCon con, RobustStageType type, OpBool flag)
{
	static_cast<OpRobustModelI*>(impl_)->add(con, type, flag);
}

void Algorithm::OpRobustModel::add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag)
{
	static_cast<OpRobustModelI*>(impl_)->add(cons, type, flag);
}

void Algorithm::OpRobustModel::add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag)
{
	static_cast<OpRobustModelI*>(impl_)->add(con, type, flag);
}

void OPUA::Algorithm::OpRobustModel::setObj(const Expression::OpLinExpr& expr, RobustStageType type)
{
	static_cast<OpRobustModelI*>(impl_)->setObj(expr, type);
}

void OPUA::Algorithm::OpRobustModel::setObj(const Expression::OpQuadExpr& expr, RobustStageType type)
{
	static_cast<OpRobustModelI*>(impl_)->setObj(expr, type);
}

void Algorithm::OpRobustModel::setValue(Variable::OpVar var, RobustStageType type, OpFloat val)
{
	static_cast<OpRobustModelI*>(impl_)->setValue(var, type, val);
}

void Algorithm::OpRobustModel::setBound(Variable::OpVar var, RobustStageType type, OpFloat val, OpBool lb)
{
	static_cast<OpRobustModelI*>(impl_)->setBound(var, type, val, lb);
}

void Algorithm::OpRobustModel::setBound(Variable::OpVar var, RobustStageType type, OpFloat lbVal, OpFloat ubVal)
{
	static_cast<OpRobustModelI*>(impl_)->setBound(var, type, lbVal, ubVal);
}

OpFloat Algorithm::OpRobustModel::getValue(Variable::OpVar var, RobustStageType type) const
{
	return static_cast<OpRobustModelI*>(impl_)->getValue(var, type);
}

OpFloat OPUA::Algorithm::OpRobustModel::getObjValue(RobustStageType type) const
{
	return static_cast<OpRobustModelI*>(impl_)->getObjValue(type);
}

OpULInt Algorithm::OpRobustModel::getSize(RobustStageType type, OpBool isVar) const
{
	return static_cast<OpRobustModelI*>(impl_)->getSize(type, isVar);
}

OpFloat Algorithm::OpRobustModel::getBound(Variable::OpVar var, RobustStageType type, OpBool lb) const
{
	return static_cast<OpRobustModelI*>(impl_)->getBound(var, type, lb);
}

void Algorithm::OpRobustModel::write(OpStr root)
{
	static_cast<OpRobustModelI*>(impl_)->write(root);
}

Algorithm::OpRobustModelI* Algorithm::OpRobustModel::getImpl() const
{
	return static_cast<OpRobustModelI*>(impl_);
}

void Algorithm::OpRobustModel::release()
{
	// ������д��release()��������ΪOpRobustModelI�а�����OPUA���
	// ������ִ��preRelease()������Щ��������ü�������֤OpRobustModelI������������ʱ���������ͷ���Щ���
	if (impl_)
	{
		static_cast<OpRobustModelI*>(impl_)->preRelease();
		static_cast<OpRobustModelI*>(impl_)->release();
		impl_ = nullptr;
	}
}

OpBool Algorithm::OpRobustModel::operator==(const OpRobustModel& model) const
{
	return impl_ == model.getImpl();
}

OpBool Algorithm::OpRobustModel::operator!=(const OpRobustModel& model) const
{
	return impl_ != model.getImpl();
}

Algorithm::OpRobustModel::OpRobustModel()
{

}

Algorithm::OpRobustModel::OpRobustModel(OpRobustModelI* impl)
{
	impl_ = impl;
}

Algorithm::OpRobustModel::OpRobustModel(OpEnv env)
{
	impl_ = new OpRobustModelI(env.getImpl());
}

Algorithm::OpRobustModel::OpRobustModel(OpEnv env, OpStr name)
{
	impl_ = new OpRobustModelI(env.getImpl(), name);
}

Algorithm::OpRobustModel::~OpRobustModel()
{

}

/* OPUA::Algorithm::OpAlgoCCG */

struct Algorithm::OpAlgoCCG::OpCCGIterInfo
{
	OpBool needSolveRobust; // �Ƿ���Ҫ���³������(False-��Ϊȷ��������������)
	Solver::OpSolType solverMode; // �����ģʽ
	Solver::OpSolStatus masterStatus; // ���������״̬
	Solver::OpSolStatus subStatus; // ���������״̬
	Solver::OpSolStatus deterministicStatus; // ȷ�����������״̬
	OpLInt initMode; // ��ʼ��ģʽѡ��
	OpLInt subProbSolveMode; // ���������ģʽѡ��(��ʱ�������ã��̶�ʹ��OA)
	OpLInt iter; // ��ǰ��������
	OpLInt iterMax; // ����������
	OpLInt iterNoImpr; // �޸��Ƶ��ۼƵ�������
	OpLInt iterNoImprLimt; // �޸��Ƶ�����ۼƵ�������
	OpFloat etaVal; // �����⸨��������(���������Ž�)
	OpFloat lb; // �����½�
	OpFloat ub; // �����Ͻ�
	OpFloat epsilon; //  ������ֵ
	OpFloat prevLb; // ��һ�ε����������½�
	OpFloat prevUb; // ��һ�ε����������Ͻ�
	OpFloat lbEpsilon; // �����½�������ֵ
	OpFloat ubEpsilon; // �����Ͻ�������ֵ

	Variable::OpVar eta; // �����⸨������(���������Ž�)(��Ҫ�ͷ�)
	Variable::OpVarArr masterAddVars; // �������ڵ����ж�̬���ɵı���(��Ҫ�ͷ�)
	Constraint::OpLCArr masterAddCons; // �������ڵ����ж�̬���ɵ�Լ��(��Ҫ�ͷ�)
	Objective::OpObj masterObj; // �����������Ŀ�꺯��(��Ҫ�ͷ�)
	Model::OpModel masterModel; // ����������ģ�Ͷ���(��Ҫ�ͷ�)
	Constraint::OpLCArr deterministicAddCons; // ȷ�������⶯̬���ɵ�Լ��(��Ҫ�ͷ�)
	Objective::OpObj deterministicObj; // ȷ���������Ŀ�꺯��(��Ҫ�ͷ�)
	Model::OpModel deterministicModel; // ȷ��������ģ�Ͷ���(��Ҫ�ͷ�)

	void init(const OpRobustModelI* mdl, const Solver::OpConfig& config); // ��ʼ��
	void clear(); // ����
	void printIterInfo(std::ostream& stream, const std::thread::id& tid); // ��ӡ������Ϣ
};

void Algorithm::OpAlgoCCG::OpCCGIterInfo::init(const OpRobustModelI* mdl, const Solver::OpConfig& config)
{
	OpEnv localEnv(mdl->env_);
	needSolveRobust = (mdl->uc_.getSize()) && (!config.getCfg<OpBool>("OPUA.Algorithm.CCG.GetDeterministicSolution"));
	masterStatus = Solver::OpSolStatus::Unknown;
	subStatus = Solver::OpSolStatus::Unknown;
	deterministicStatus = Solver::OpSolStatus::Unknown;
	initMode = config.getCfg<OpLInt>("OPUA.Algorithm.CCG.FirstStageInitMode");
	subProbSolveMode = config.getCfg<OpLInt>("OPUA.Algorithm.CCG.SubProbSolveMode"); // ����������ʱ��������
	iter = 0;
	iterMax = config.getCfg<OpLInt>("OPUA.Algorithm.CCG.IterMax");
	iterNoImpr = 0;
	iterNoImprLimt = std::max(2, config.getCfg<OpLInt>("OPUA.Algorithm.CCG.NoImprovementIterLimit"));
	etaVal = 0.0;
	lb = -Constant::Infinity;
	ub = Constant::Infinity;
	epsilon = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGGap");
	prevLb = -Constant::Infinity;
	prevUb = Constant::Infinity;
	lbEpsilon = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGLBGap");
	ubEpsilon = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGUBGap");

	eta = Variable::OpVar(localEnv, Variable::OpVarType::Con, 0, Constant::Infinity, "eta");
	masterAddVars = Variable::OpVarArr(localEnv);
	masterAddCons = Constraint::OpLCArr(localEnv);
	masterObj = Objective::OpMinimize(localEnv, mdl->fso_ + eta);
	masterModel = Model::OpModel(localEnv, "CCG_Master_Prob");
	deterministicAddCons = Constraint::OpLCArr(localEnv);
	deterministicObj = Objective::OpObj(localEnv, Objective::OpObjSense::Min);
	deterministicModel = Model::OpModel(localEnv, "CCG_Deterministic_Prob");
	solverMode = Solver::CharSolType2OpSolType(config.getCfg<OpChar>("OPUA.Algorithm.CCG.MIPSolverMode"));
}

void Algorithm::OpAlgoCCG::OpCCGIterInfo::clear()
{
	// �ͷ�������OPUA���
	deterministicModel.release();
	deterministicObj.release();
	deterministicAddCons.releaseElements(); // �ⲿ����CCG���������ı�������Ҫ�����ͷ�
	deterministicAddCons.release();
	masterModel.release();
	masterObj.release();
	masterAddCons.releaseElements(); // �ⲿ����CCG���������ı�������Ҫ�����ͷ�
	masterAddCons.release();
	masterAddVars.releaseElements(); // �ⲿ����CCG���������ı�������Ҫ�����ͷ�
	masterAddVars.release();
	eta.release();
}

void Algorithm::OpAlgoCCG::OpCCGIterInfo::printIterInfo(std::ostream& stream, const std::thread::id& tid)
{
	stream << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")ITER\tLB\tUB\tGAP" << std::endl;
	stream << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")"
		<< iter << '\t'
		<< lb << '\t'
		<< ub << '\t'
		<< ub - lb << std::endl;
}

struct Algorithm::OpAlgoCCG::OpOAIterInfo
{
	Solver::OpSolStatus oaSubStatus; // OA���������״̬
	Solver::OpSolStatus oaMasterStatus; // OA���������״̬
	OpLInt oaIter; // OA��������
	OpLInt oaIterMax; // OA������������
	OpLInt oaIterNoImpr; // OA�޸����ۼƵ�������
	OpLInt oaIterNoImprLimt; // OA�޸����ۼƵ�����������
	OpFloat oaGap; // OA���������϶
	OpFloat oaLbGap; // OA�½����������϶(�޸����о�)
	OpFloat oaUbGap; // OA�Ͻ����������϶(�޸����о�)
	OpFloat oaUb; // ��ǰOA�Ͻ�
	OpFloat oaLb; // ��ǰOA�½�
	OpFloat oaPrevUb; // ��һ�ε�����OA�Ͻ�
	OpFloat oaPrevLb; // ��һ�ε�����OA�½�
	OpFloat betaVal; // OA������Ŀ�꺯����������Ŀ�꺯����

	Expression::OpLinExpr oaSubBasicObjExpr; // OA���������Ŀ�꺯�����ʽ(����˫������Ĳ���)
	Constraint::OpLCArr oaSubAddCons; // OA�������еĶ�̬���ɵ�Լ��(��Ҫ�ͷ�)
	Objective::OpObj oaSubObj; // OA������Ŀ�꺯������(��Ҫ�ͷ�)
	Objective::OpObj oaSubTmpObj;  // OA��������ʱĿ�꺯������(������;����Ŀ�꺯��)(��Ҫ�ͷ�)
	Model::OpModel oaSubModel; // OA������ģ�Ͷ���(��Ҫ�ͷ�)
	Variable::OpVar beta; // OA������Ŀ�꺯�������������ֵ(��Ҫ�ͷ�)
	Constraint::OpLCArr oaMasterAddCons; // OA�������еĶ�̬���ɵ�Լ��(��Ҫ�ͷ�)
	Objective::OpObj oaMasterObj; // OA������Ŀ�꺯������(��Ҫ�ͷ�)
	Model::OpModel oaMasterModel; // OA������ģ�Ͷ���(��Ҫ�ͷ�)

	void init(const OpRobustModelI* mdl, const Solver::OpConfig& config); // ��ʼ��
	void clear(); // ����
	void printIterInfo(std::ostream& stream, const std::thread::id& tid); // ��ӡ������Ϣ
};


void Algorithm::OpAlgoCCG::OpOAIterInfo::init(const OpRobustModelI* mdl, const Solver::OpConfig& config)
{
	// TODO����OA��CCG���룬OAӵ���Լ�����������ǰ׺
	OpEnv localEnv(mdl->env_);
	oaSubStatus = Solver::OpSolStatus::Unknown;
	oaMasterStatus = Solver::OpSolStatus::Unknown;
	oaIter = 1;
	oaIterMax = 100;
	oaIterNoImpr = 0;
	oaIterNoImprLimt = std::max(2, config.getCfg<OpLInt>("OPUA.Algorithm.CCG.NoImprovementIterLimit"));
	oaGap = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGGap");
	oaLbGap = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGLBGap");
	oaUbGap = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGUBGap");
	oaUb = Constant::Infinity;
	oaLb = -Constant::Infinity;
	oaPrevUb = Constant::Infinity;
	oaPrevLb = -Constant::Infinity;
	betaVal = 0.0;

	oaSubBasicObjExpr = Expression::OpLinExpr(0.0);
	oaSubAddCons = Constraint::OpLCArr(localEnv);
	oaSubObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	oaSubTmpObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	oaSubModel = Model::OpModel(localEnv);
	beta = Variable::OpVar(localEnv);
	oaMasterAddCons = Constraint::OpLCArr(localEnv);
	oaMasterObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	oaMasterModel = Model::OpModel(localEnv);
}

void Algorithm::OpAlgoCCG::OpOAIterInfo::clear()
{
	oaSubModel.release();
	oaSubTmpObj.release();
	oaSubObj.release();
	oaSubAddCons.releaseElements();
	oaSubAddCons.release();
	oaSubBasicObjExpr.clear();

	oaMasterModel.release();
	oaMasterObj.release();
	oaMasterAddCons.releaseElements();
	oaMasterAddCons.release();
	beta.release();
}

void Algorithm::OpAlgoCCG::OpOAIterInfo::printIterInfo(std::ostream& stream, const std::thread::id& tid)
{
	stream << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")OA: ITER\tLB\tUB\tGAP" << std::endl;
	stream << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")OA: "
		<< oaIter << '\t'
		<< oaLb << '\t'
		<< oaUb << '\t'
		<< oaUb - oaLb << std::endl;
}

struct Algorithm::OpAlgoCCG::OpADIterInfo
{
	Solver::OpSolStatus adSubStatus; // AD���������״̬
	Solver::OpSolStatus adMasterStatus; // AD���������״̬
	OpLInt adIter; // AD��������
	OpLInt adIterMax; // AD������������
	OpLInt adIterNoImpr; // AD�޸����ۼƵ�������
	OpLInt adIterNoImprLimt; // AD�޸����ۼƵ�����������
	OpFloat adGap; // AD���������϶
	OpFloat adLbGap; // AD�½����������϶(�޸����о�)
	OpFloat adUbGap; // AD�Ͻ����������϶(�޸����о�)
	OpFloat adUb; // ��ǰAD�Ͻ�
	OpFloat adLb; // ��ǰAD�½�
	OpFloat adPrevUb; // ��һ�ε�����AD�Ͻ�
	OpFloat adPrevLb; // ��һ�ε�����AD�½�

	Objective::OpObj adSubObj; // AD������Ŀ�꺯������(��Ҫ�ͷ�)
	Objective::OpObj adSubTmpObj;  // AD��������ʱĿ�꺯������(������;����Ŀ�꺯��)(��Ҫ�ͷ�)
	Model::OpModel adSubModel; // AD������ģ�Ͷ���(��Ҫ�ͷ�)
	Objective::OpObj adMasterObj; // AD������Ŀ�꺯������(��Ҫ�ͷ�)
	Objective::OpObj adMasterTmpObj;  // AD��������ʱĿ�꺯������(������;����Ŀ�꺯��)(��Ҫ�ͷ�)
	Model::OpModel adMasterModel; // AD������ģ�Ͷ���(��Ҫ�ͷ�)

	void init(const OpRobustModelI* mdl, const Solver::OpConfig& config); // ��ʼ��
	void clear(); // ����
	void printIterInfo(std::ostream& stream, const std::thread::id& tid); // ��ӡ������Ϣ
};

void Algorithm::OpAlgoCCG::OpADIterInfo::init(const OpRobustModelI* mdl, const Solver::OpConfig& config)
{
	OpEnv localEnv(mdl->env_);
	adSubStatus = Solver::OpSolStatus::Unknown;
	adMasterStatus = Solver::OpSolStatus::Unknown;
	adIter = 1;
	adIterMax = 100;
	adIterNoImpr = 0;
	adIterNoImprLimt = std::max(2, config.getCfg<OpLInt>("OPUA.Algorithm.CCG.NoImprovementIterLimit"));
	adGap = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGGap");
	adLbGap = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGLBGap");
	adUbGap = config.getCfg<OpFloat>("OPUA.Algorithm.CCG.CCGUBGap");
	adUb = Constant::Infinity;
	adLb = -Constant::Infinity;
	adPrevUb = Constant::Infinity;
	adPrevLb = -Constant::Infinity;

	adSubObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	adSubTmpObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	adSubModel = Model::OpModel(localEnv);
	adMasterObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	adMasterTmpObj = Objective::OpObj(localEnv, Objective::OpObjSense::Max);
	adMasterModel = Model::OpModel(localEnv);
}

void Algorithm::OpAlgoCCG::OpADIterInfo::clear()
{
	adMasterModel.release();
	adMasterTmpObj.release();
	adMasterObj.release();
	adSubModel.release();
	adSubTmpObj.release();
	adSubObj.release();
}

void Algorithm::OpAlgoCCG::OpADIterInfo::printIterInfo(std::ostream& stream, const std::thread::id& tid)
{
	stream << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")AD: ITER\tLB\tUB\tGAP" << std::endl;
	stream << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")AD: "
		<< adIter << '\t'
		<< adLb << '\t'
		<< adUb << '\t'
		<< adUb - adLb << std::endl;
}

void Algorithm::OpAlgoCCG::extract(OpRobustModel model)
{
	mdl_ = model.getImpl();
}

OpBool Algorithm::OpAlgoCCG::solve(const Solver::OpConfig& config)
{
	using namespace Container;
	using namespace Variable;
	using namespace Expression;
	using namespace Constraint;
	using namespace Model;
	using namespace Solver;

	// ����ɹ��ı�־
	OpBool successful = false;
	// ��������
	OpEnv localEnv(mdl_->env_);
	std::thread::id tid = std::this_thread::get_id();
	std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")MemoryUsage(start): " << localEnv.getMemoryUsage() << std::endl;
	// ����������Ϣ
	OpCCGIterInfo info;
	info.init(mdl_, config);
	// �ػ�����
	OpAutoFree guard([&]() {
		try
		{
			info.clear();
			std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")MemoryUsage(end): " << localEnv.getMemoryUsage() << std::endl;
		}
		catch (std::exception& ex)
		{
			std::cerr << ex.what() << std::endl;
		}
	});

	// lambda������;����ʼ��
	auto init = [&]() {
		if (info.needSolveRobust)
		{
			switch (info.initMode)
			{
			case 0:
				mdl_->fss_.setSize(mdl_->fsv_.getSize());
				break;
			case 1:
				break;
			case 2:
				mdl_->fss_.setSize(mdl_->fsv_.getSize());
				break;
			default:
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->wrong init mode";
				throw OpExcBase(msg.str());
				break;
			}
			mdl_->ssds_.setSize(mdl_->ssdv_.getSize());
			mdl_->ssps_.setSize(mdl_->sspv_.getSize());
			//mdl_->us_.setSize(mdl_->uc_.getSize());
			info.iter++;
		}
		else
		{
			mdl_->fss_.setSize(mdl_->fsv_.getSize());
			mdl_->ssps_.setSize(mdl_->sspv_.getSize());
		}
	};
	// lambda������;�����ɻ���������
	auto createBasicMasterModel = [&]() {
		// ����������Ŀ�꺯��
		info.masterModel.setObj(info.masterObj);
		// ����������Լ������(�����Զ�����)
		info.masterModel.add(mdl_->fsc_);
	};
	// lambda������;���̶���ȷ���������һ�׶�+���׶�ԭ���⣬����ȷ���Խ�
	auto createDeterministicSol1 = [&]() {

	};
	// lambda������;���̶�һ�׶α����Ͳ�ȷ�����������׶�ԭ���⣬����ȷ���Խ�
	// ���棺���ɷ�������(��Ϊobj�ظ�������Ч)
	auto createDeterministicSol2 = [&]() {
		auto deterministicSolver = OpAdapSol(info.solverMode, localEnv);
		OpAutoFree localGuard([&]() {
			try
			{
				deterministicSolver.release();
			}
			catch (std::exception& ex)
			{
				std::cerr << ex.what() << std::endl;
			}
		});
		// ͳ��һ�׶α���/��ȷ������
		std::unordered_map<OpVarI*, OpFloat> firstVarSols;
		for (OpULInt i = 0; i < mdl_->fsv_.getSize(); i++)
			firstVarSols.emplace(mdl_->fsv_[i].getImpl(), mdl_->fss_[i]);
		for (OpULInt i = 0; i < mdl_->uv_.getSize(); i++)
			firstVarSols.emplace(mdl_->uv_[i].getImpl(), mdl_->us_[i]);
		// �����̶�һ�׶α���&��ȷ���������Ķ��׶�ԭ����Լ������
		for (OpULInt i = 0; i < mdl_->sspc_.getSize(); i++)
		{
			const auto& con = mdl_->sspc_[i];
			OpLinExpr expr(con.getExpr().getConstant());
			for (auto it = con.getExpr().getLBegin(); it != con.getExpr().getLEnd(); ++it)
			{
				auto iter = firstVarSols.find(it.getVar().getImpl());
				if (iter != firstVarSols.end())
					expr.addLinTerm(iter->second * it.getCoeff());
				else
					expr.addLinTerm(it.getVar(), it.getCoeff());
			}
			if (expr.getSize())
			{
				auto newCon = OpLinCon(localEnv, con.getLb(), expr, con.getUb());
				if (newCon.getImpl())
				{
					info.deterministicAddCons.add(newCon);
					info.deterministicModel.add(newCon);
				}
			}
		}
		// �γɹ̶�һ�׶α���&��ȷ���������Ķ��׶�Ŀ�꺯��
		{
			OpLinExpr newObjExpr(mdl_->sspo_.getConstant());
			for (auto it = mdl_->sspo_.getLBegin(); it != mdl_->sspo_.getLEnd(); ++it)
			{
				auto iter = firstVarSols.find(it.getVar().getImpl());
				if (iter != firstVarSols.end())
					newObjExpr.addLinTerm(iter->second * it.getCoeff());
				else
					newObjExpr.addLinTerm(it.getVar(), it.getCoeff());
			}
			info.deterministicObj.setLinExpr(newObjExpr);
			info.deterministicModel.setObj(info.deterministicObj);
		}
		// ���̶�һ�׶α���&��ȷ���������Ķ��׶�ģ��
		deterministicSolver.extract(info.deterministicModel);
		deterministicSolver.setParam(config);
		deterministicSolver.solve();
		info.deterministicStatus = IntStatus2OpStatus(info.solverMode, deterministicSolver.getStatus());
		// ȡֵ
		if (info.deterministicStatus == OpSolStatus::Optimal)
		{
			for (OpULInt i = 0; i < mdl_->sspv_.getSize(); i++)
				mdl_->ssps_[i] = deterministicSolver.getValue(mdl_->sspv_[i]);
		}
		else
		{
			std::stringstream msg;
			msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->resolve sub primal model failed";
			throw OpExcBase(msg.str());
		}
		std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")resolve sub primal model finished" << std::endl;
	};
	// lambda������;����������������������Ľ�
	auto updateMasterSpecialSolution = [&](OpLInt k) {
		if (!mdl_->fssv_.getSize())
			return;
		// ���������ģ��
		auto specialCons = OpLCArr(localEnv);
		auto specialQCons = OpQCArr(localEnv);
		auto specialModel = OpModel(localEnv);
		auto specialSolver = OpAdapSol(info.solverMode, localEnv);
		OpAutoFree localGuard([&]() {
			try
			{
				specialSolver.release();
				specialModel.release();
				specialQCons.releaseElements();
				specialQCons.release();
				specialCons.releaseElements();
				specialCons.release();
			}
			catch (std::exception& ex)
			{
				std::cerr << ex.what() << std::endl;
			}
		});
		// ͳ��һ�׶α�����
		std::unordered_map<OpVarI*, OpFloat> firstVarSols;
		for (OpULInt i = 0; i < mdl_->fsv_.getSize(); i++)
			firstVarSols.emplace(mdl_->fsv_[i].getImpl(), mdl_->fss_[i]);
		for (OpULInt i = 0; i < mdl_->fssv_.getSize(); i++)
			firstVarSols.erase(mdl_->fssv_[i].getImpl());
		// ����������һ�׶���������Լ��(�����Զ�����)
		for (OpULInt i = 0; i < mdl_->fssc_.getSize(); i++)
		{
			const auto& con = mdl_->fssc_[i];
			OpLinExpr expr(con.getExpr().getConstant());
			for (auto it = con.getExpr().getLBegin(); it != con.getExpr().getLEnd(); ++it)
			{
				auto iter = firstVarSols.find(it.getVar().getImpl());
				if (iter != firstVarSols.end())
					expr += iter->second * it.getCoeff();
				else
					expr += it.getVar() * it.getCoeff();
			}
			if (expr.getSize())
				specialCons.add(OpLinCon(localEnv, con.getLb(), expr, con.getUb()));
		}
		specialModel.add(specialCons);
		// ����������һ�׶��������Լ��(�����Զ�����)
		for (OpULInt i = 0; i < mdl_->fssqc_.getSize(); i++)
		{
			const auto& con = mdl_->fssqc_[i];
			OpQuadExpr expr(con.getExpr().getConstant());
			for (auto it = con.getExpr().getLBegin(); it != con.getExpr().getLEnd(); ++it)
			{
				auto iter = firstVarSols.find(it.getVar().getImpl());
				if (iter != firstVarSols.end())
					expr.addLinTerm(iter->second * it.getCoeff());
				else
					expr.addLinTerm(it.getVar(), it.getCoeff());
			}
			for (auto it = con.getExpr().getQBegin(); it != con.getExpr().getQEnd(); ++it)
			{
				auto var1 = it.getVar1(), var2 = it.getVar2();
				auto iter1 = firstVarSols.find(var1.getImpl()), iter2 = firstVarSols.find(var2.getImpl());
				if (iter1 == firstVarSols.end() && iter2 == firstVarSols.end())
					expr.addQuadTerm(var1, var2, it.getCoeff());
				else if (iter1 == firstVarSols.end() && iter2 != firstVarSols.end())
					expr.addLinTerm(var1, iter2->second * it.getCoeff());
				else if (iter1 != firstVarSols.end() && iter2 == firstVarSols.end())
					expr.addLinTerm(var2, iter1->second * it.getCoeff());
				else if (iter1 != firstVarSols.end() && iter2 != firstVarSols.end())
					expr.addLinTerm(iter1->second * iter2->second * it.getCoeff());
			}
			if (expr.getSize())
				specialQCons.add(OpQuadCon(localEnv, con.getLb(), expr, con.getUb()));
		}
		specialModel.add(specialQCons);
		// ���&ȡֵ
		specialSolver.extract(specialModel);
		specialSolver.setParam(config);
		specialSolver.solve();
		auto specialStatus = IntStatus2OpStatus(info.solverMode, specialSolver.getStatus());
		if (specialStatus == OpSolStatus::Optimal)
		{
			for (OpULInt i = 0; i < mdl_->fssv_.getSize(); i++)
				mdl_->fss_[mdl_->fsv_.find(mdl_->fssv_[i])] = specialSolver.getValue(mdl_->fssv_[i]);
		}
		else
		{
			std::stringstream msg;
			msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->re-solve special cons failed";
			throw OpExcBase(msg.str());
		}
		std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")re-solve special cons finished" << std::endl;
	};
	// lambda������;������������Ľ�
	auto updateMasterSolution = [&](OpLInt k) {
		if (k > 1)
		{
			auto masterSolver = OpAdapSol(info.solverMode, localEnv);
			OpAutoFree localGuard([&]() {
				try
				{
					masterSolver.release();
				}
				catch (std::exception& ex)
				{
					std::cerr << ex.what() << std::endl;
				}
			});

			masterSolver.extract(info.masterModel);
			masterSolver.setParam(config);
			masterSolver.solve();
			auto masterStatus = IntStatus2OpStatus(info.solverMode, masterSolver.getStatus());
			if (masterStatus == OpSolStatus::Optimal)
			{
				info.masterStatus = OpSolStatus::Optimal;
				info.prevLb = info.lb;
				info.lb = masterSolver.getValue(info.masterObj);
				info.etaVal = masterSolver.getValue(info.eta);
				for (OpULInt i = 0; i < mdl_->fsv_.getSize(); i++)
					mdl_->fss_[i] = masterSolver.getValue(mdl_->fsv_[i]);
				updateMasterSpecialSolution(k);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->solve master model failed";
				throw OpExcBase(msg.str());
			}
		}
		else
		{
			switch (info.initMode)
			{
			case 0:
				for (int i = 0; i < mdl_->fss_.getSize(); i++)
					mdl_->fss_[i] = 0; // ��0ֵ��Ϊ��ʼ��
				updateMasterSpecialSolution(k);
				break;
			case 1:
				break; // ʹ���Զ����
			case 2:
				createDeterministicSol1(); // ʹ��ȷ���Խ�
				updateMasterSpecialSolution(k);
				break;
			}
		}
	};
	// lambda������;������������Ľ�(OA����)
	auto updateSubSolution1 = [&](OpLInt k) {
		// ��������ʼ��OA�����Ļ�����Ϣ
		OpOAIterInfo oaInfo;
		oaInfo.init(mdl_, config);
		Solver::OpAdapSol oaSubSolver(info.solverMode, localEnv); // OA�����������(��Ҫ�ͷ�)
		Solver::OpAdapSol oaMasterSolver(info.solverMode, localEnv); // OA�����������(��Ҫ�ͷ�)
		// һЩͳ�Ʊ���
		std::unordered_map<OpVarI*, OpFloat> firstVarSols; // һ�׶α�����
		std::unordered_map<OpVarI*, OpFloat> unSetSol; // (˫��������)��ȷ���������� 
		std::unordered_map<OpVarI*, OpFloat> dualSetSol; // (˫��������)��ż������
		std::unordered_map<OpVarI*, OpVar> oaSubVars; // OA�����������
		std::unordered_map<OpVarI*, OpVar> oaMasterVars; // OA�����������
		std::vector<std::tuple<OpVarI*, OpVarI*, OpFloat> > bilinears; // ˫������({��ȷ������, ��ż����, ϵ��})
		std::vector<std::pair<OpVarI*, OpFloat> > linears; // ������({��ȷ������, ϵ��})
		// �Զ��ͷ��ڴ�
		OpAutoFree localGuard([&]() {
			try
			{
				oaSubSolver.release();
				oaMasterSolver.release();
				oaInfo.clear();
			}
			catch (std::exception& ex)
			{
				std::cerr << ex.what() << std::endl;
			}
		});

		// ͳ��һ�׶α�����
		for (OpULInt i = 0; i < mdl_->fsv_.getSize(); i++)
			firstVarSols.emplace(mdl_->fsv_[i].getImpl(), mdl_->fss_[i]);
		// һЩͨ�õĺ���
		auto createOAVars = [&](OpVarArr& varArr, std::unordered_map<OpVarI*, OpVar>& vars) { /* ��³��ģ���г�ȡOA��������(����ͳ��) */
			for (OpULInt i = 0; i < varArr.getSize(); i++)
				vars.emplace(varArr[i].getImpl(), varArr[i]);
		};
		auto createOACons = [&](OpLCArr& conArr, OpModel& model, OpLCArr& conSave) { /* ����OA�������������Լ�� */
			for (OpULInt i = 0; i < conArr.getSize(); i++)
			{
				const auto& con = conArr[i];
				OpLinExpr expr(con.getExpr().getConstant());
				for (auto it = con.getExpr().getLBegin(); it != con.getExpr().getLEnd(); ++it)
				{
					auto iter = firstVarSols.find(it.getVar().getImpl());
					if (iter != firstVarSols.end())
						expr.addLinTerm(iter->second * it.getCoeff());
					else
						expr.addLinTerm(it.getVar(), it.getCoeff());
				}
				if (expr.getSize())
				{
					auto newCon = OpLinCon(localEnv, con.getLb(), expr, con.getUb());
					if (newCon.getImpl())
					{
						conSave.add(newCon);
						model.add(newCon);
					}
				}			
			}
		};
		auto oaQuit = [&]() { /* �ж�OA�������˳����� */
			if (oaInfo.oaIter > 1 ?  /* �ǵ�һ�ε�������Ҫͳ�����½��ۼ��޸��ƵĴ��� */
				(std::abs(oaInfo.oaLb - oaInfo.oaPrevLb) / std::abs(oaInfo.oaPrevLb) <= oaInfo.oaLbGap /* �½��޸����о� */
					&& std::abs(oaInfo.oaUb - oaInfo.oaPrevUb) / std::abs(oaInfo.oaPrevUb) <= oaInfo.oaUbGap) /* �Ͻ��޸����о� */
				: false /* ��һ�ε������������½��Ƿ���������ƽ���ͳ�� */)
				oaInfo.oaIterNoImpr++;
			else
				oaInfo.oaIterNoImpr = 0;
			return (oaInfo.oaIter > oaInfo.oaIterMax
				|| oaInfo.oaIterNoImpr > oaInfo.oaIterNoImprLimt
				|| std::abs(oaInfo.oaUb - oaInfo.oaLb) / std::abs(oaInfo.oaUb) < oaInfo.oaGap);
		};
		// Ϊ�����ⴴ������(ֻ�ǳ�ȡ���γɼ��ϣ�����ͳ�ƣ��������´�������)
		createOAVars(mdl_->ssdv_, oaSubVars);
		// Ϊ���������Լ������(�����Զ�����)
		createOACons(mdl_->ssdc_, oaInfo.oaSubModel, oaInfo.oaSubAddCons);
		// Ϊ�����ⴴ������Ŀ�꺯��
		for (auto it = mdl_->ssdo_.getLBegin(); it != mdl_->ssdo_.getLEnd(); ++it)
		{
			auto iter1 = firstVarSols.find(it.getVar().getImpl());
			auto iter2 = oaSubVars.find(it.getVar().getImpl());
			if (iter1 != firstVarSols.end() && iter2 == oaSubVars.end())
				oaInfo.oaSubBasicObjExpr.addLinTerm(iter1->second * it.getCoeff());
			else if (iter1 == firstVarSols.end() && iter2 != oaSubVars.end())
				oaInfo.oaSubBasicObjExpr.addLinTerm(iter2->second, it.getCoeff());
			else
				linears.emplace_back(it.getVar().getImpl(), it.getCoeff());
		}
		for (auto it = mdl_->ssdo_.getQBegin(); it != mdl_->ssdo_.getQEnd(); ++it)
		{
			auto var1Ptr = it.getVar1().getImpl(), var2Ptr = it.getVar2().getImpl();
			auto iter1 = firstVarSols.find(var1Ptr), iter2 = firstVarSols.find(var2Ptr);
			if (iter1 != firstVarSols.end() && iter2 == firstVarSols.end())
				oaInfo.oaSubBasicObjExpr.addLinTerm(oaSubVars.at(var2Ptr), iter1->second * it.getCoeff()); /*������*/
			else if (iter1 == firstVarSols.end() && iter2 != firstVarSols.end())
				oaInfo.oaSubBasicObjExpr.addLinTerm(oaSubVars.at(var1Ptr), iter2->second * it.getCoeff()); /*������*/
			else /* ˫������*/
			{
				if (oaSubVars.find(var1Ptr) == oaSubVars.end())
					bilinears.emplace_back(var1Ptr, var2Ptr, it.getCoeff());
				else
					bilinears.emplace_back(var2Ptr, var1Ptr, it.getCoeff());
			}
		}
		// �γɲ�ȷ������ֵ
		//mdl_->autoBound();
		//for (auto& l : linears)
		//	unSetSol.try_emplace(l.first, mdl_->getBound(l.first, RobustStageType::Uncertainty, true)); // ��ȷ�����ĳ�ֵͳһʹ�ñ�������
		//for (auto& bl : bilinears)
		//	unSetSol.try_emplace(std::get<0>(bl), mdl_->getBound(std::get<0>(bl), RobustStageType::Uncertainty, true)); // ��ȷ�����ĳ�ֵͳһʹ�ñ�������
		for (auto& l : linears)
			unSetSol.try_emplace(l.first, mdl_->getValue(l.first, RobustStageType::Uncertainty));
		for (auto& bl : bilinears)
			unSetSol.try_emplace(std::get<0>(bl), mdl_->getValue(std::get<0>(bl), RobustStageType::Uncertainty));
		// Ϊ�����ⴴ������(ֻ�ǳ�ȡ���γɼ��ϣ�����ͳ�ƣ��������´�������)
		createOAVars(mdl_->ssdv_, oaMasterVars);
		createOAVars(mdl_->uv_, oaMasterVars);
		// Ϊ����������Լ��
		createOACons(mdl_->ssdc_, oaInfo.oaMasterModel, oaInfo.oaMasterAddCons);
		createOACons(mdl_->uc_, oaInfo.oaMasterModel, oaInfo.oaMasterAddCons);
		// Ϊ�����ⴴ��Ŀ�꺯��
		{
			OpLinExpr oaMasterObjExpr(oaInfo.beta);
			for (auto it = mdl_->ssdo_.getLBegin(); it != mdl_->ssdo_.getLEnd(); ++it)
			{
				auto iter = firstVarSols.find(it.getVar().getImpl());
				if (iter != firstVarSols.end())
					oaMasterObjExpr.addLinTerm(iter->second * it.getCoeff());
				else
					oaMasterObjExpr.addLinTerm(oaMasterVars.at(it.getVar().getImpl()), it.getCoeff());
			}
			for (auto it = mdl_->ssdo_.getQBegin(); it != mdl_->ssdo_.getQEnd(); ++it)
			{
				auto var1Ptr = it.getVar1().getImpl(), var2Ptr = it.getVar2().getImpl();
				auto iter1 = firstVarSols.find(var1Ptr), iter2 = firstVarSols.find(var2Ptr);
				if (iter1 != firstVarSols.end() && iter2 == firstVarSols.end())
					oaMasterObjExpr.addLinTerm(oaMasterVars.at(var2Ptr), iter1->second * it.getCoeff()); /*������*/
				else if (iter1 == firstVarSols.end() && iter2 != firstVarSols.end())
					oaMasterObjExpr.addLinTerm(oaMasterVars.at(var1Ptr), iter2->second * it.getCoeff()); /*������*/
				else
					; /*˫����������beta����*/
			}
			oaInfo.oaMasterObj.setLinExpr(oaMasterObjExpr);
			oaInfo.oaMasterModel.setObj(oaInfo.oaMasterObj);
		}

		// �������
		while (!oaQuit())
		{
			// ����������Ŀ�꺯��
			{
				OpLinExpr oaSubObjExpr(oaInfo.oaSubBasicObjExpr);
				for (auto& l : linears)
					oaSubObjExpr += l.second * unSetSol.at(l.first);
				for (auto& bl : bilinears)
					oaSubObjExpr += std::get<2>(bl) * unSetSol.at(std::get<0>(bl)) * oaSubVars.at(std::get<1>(bl));
				oaInfo.oaSubModel.setObj(oaInfo.oaSubTmpObj); // ����Ϊ�˸���Ŀ�꺯����������һ���յ���ʱĿ�꺯��
				oaInfo.oaSubObj.setLinExpr(oaSubObjExpr); // ��ʱԭĿ�꺯���Ѿ����������������������ʽ
				oaInfo.oaSubModel.setObj(oaInfo.oaSubObj); // ����ٽ�ԭĿ�꺯���������¼���ģ��
			}
			// ���������
			oaSubSolver.extract(oaInfo.oaSubModel);
			oaSubSolver.setParam(config);
			oaSubSolver.solve();
			oaInfo.oaSubStatus = IntStatus2OpStatus(info.solverMode, oaSubSolver.getStatus());
			// ������ȡֵ&�����½�
			if (oaInfo.oaSubStatus == OpSolStatus::Optimal)
			{
				// ȡ(˫��������)��ż������
				for (auto& bl : bilinears)
					dualSetSol[std::get<1>(bl)] = oaSubSolver.getValue(oaSubVars.at(std::get<1>(bl)));
				// ����Lb
				oaInfo.oaPrevLb = oaInfo.oaLb;
				oaInfo.oaLb = oaSubSolver.getValue(oaInfo.oaSubObj);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->solve OA sub model failed";
				throw OpExcBase(msg.str());
			}
			// ���ɲ�������������Ÿ�
			{
				OpLinExpr cutExpr(0.0);
				for (auto& bl : bilinears)
				{
					auto unPtr = std::get<0>(bl), dualPtr = std::get<1>(bl);
					auto coef = std::get<2>(bl);
					cutExpr.addLinTerm(-coef * dualSetSol.at(dualPtr) * unSetSol.at(unPtr));
					cutExpr.addLinTerm(oaMasterVars.at(unPtr), coef * dualSetSol.at(dualPtr));
					cutExpr.addLinTerm(oaMasterVars.at(dualPtr), coef * unSetSol.at(unPtr));
				}
				if (cutExpr.getSize())
				{
					auto newCut = oaInfo.beta <= cutExpr;
					if (newCut.getImpl())
					{
						oaInfo.oaMasterAddCons.add(newCut);
						oaInfo.oaMasterModel.add(newCut);
					}
				}
			}
			// ���������
			oaMasterSolver.extract(oaInfo.oaMasterModel);
			oaMasterSolver.setParam(config);
			oaMasterSolver.solve();
			oaInfo.oaMasterStatus = IntStatus2OpStatus(info.solverMode, oaMasterSolver.getStatus());
			// ������ȡֵ&�����Ͻ�
			if (oaInfo.oaMasterStatus == OpSolStatus::Optimal)
			{
				// ȡ��ȷ��������
				for (auto& l : linears)
					unSetSol[l.first] = oaMasterSolver.getValue(oaMasterVars.at(l.first));
				for (auto& bl : bilinears)
					unSetSol[std::get<0>(bl)] = oaMasterSolver.getValue(oaMasterVars.at(std::get<0>(bl)));
				// ����Ub
				oaInfo.oaPrevUb = oaInfo.oaUb;
				oaInfo.oaUb = oaMasterSolver.getValue(oaInfo.oaMasterObj);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->solve OA master model failed";
				throw OpExcBase(msg.str());
			}
			// ��ӡOA������Ϣ
			oaInfo.printIterInfo(std::cout, tid);
			// ����OA������Ϣ
			oaInfo.oaIter++;
		}

		// �����������(��ȷ������&��ż��)
		for (OpULInt i = 0; i < mdl_->ssdv_.getSize(); i++)
			mdl_->ssds_[i] = oaMasterSolver.getValue(mdl_->ssdv_[i]);
		for (OpULInt i = 0; i < mdl_->uv_.getSize(); i++)
			mdl_->us_[i] = oaMasterSolver.getValue(mdl_->uv_[i]);
		// ����CCG�Ͻ�
		info.prevUb = info.ub;
		if (k > 1)
		{
			auto actualObj = oaInfo.oaUb + mdl_->sspo_.getConstant();
			for (auto it = mdl_->sspo_.getLBegin(); it != mdl_->sspo_.getLEnd(); ++it)
			{
				auto iter = oaMasterVars.find(it.getVar().getImpl());
				if (iter != oaMasterVars.end())
					actualObj += it.getCoeff() * oaMasterSolver.getValue(iter->second);
			}
			info.ub = info.lb - info.etaVal + actualObj;
		}
		info.subStatus = OpSolStatus::Optimal;
	};
	// lambda������;������������Ľ�(�������)
	auto updateSubSolution2 = [&](OpLInt k) {
		// ��������ʼ��AD�����Ļ�����Ϣ
		OpADIterInfo adInfo;
		adInfo.init(mdl_, config);
		Solver::OpAdapSol adSubSolver(info.solverMode, localEnv); // AD�����������(��Ҫ�ͷ�)
		Solver::OpAdapSol adMasterSolver(info.solverMode, localEnv); // AD�����������(��Ҫ�ͷ�)
		// һЩͳ�Ʊ���
		std::unordered_map<OpVarI*, OpFloat> firstVarSols; // һ�׶α�����
		std::unordered_map<OpVarI*, OpFloat> unSetSol; // ��ȷ���������� 
		std::unordered_map<OpVarI*, OpFloat> dualSetSol; // ��ż������
		// �Զ��ͷ��ڴ�
		OpAutoFree localGuard([&]() {
			try
			{
				adSubSolver.release();
				adMasterSolver.release();
				adInfo.clear();
			}
			catch (std::exception& ex)
			{
				std::cerr << ex.what() << std::endl;
			}
		});

		// һЩͨ�õĺ���
		auto adQuit = [&]() { /* �ж�AD�������˳����� */
			if (adInfo.adIter > 1 ?  /* �ǵ�һ�ε�������Ҫͳ�����½��ۼ��޸��ƵĴ��� */
				(std::abs(adInfo.adLb - adInfo.adPrevLb) / std::abs(adInfo.adPrevLb) <= adInfo.adLbGap /* �½��޸����о� */
					&& std::abs(adInfo.adUb - adInfo.adPrevUb) / std::abs(adInfo.adPrevUb) <= adInfo.adUbGap) /* �Ͻ��޸����о� */
				: false /* ��һ�ε������������½��Ƿ���������ƽ���ͳ�� */)
				adInfo.adIterNoImpr++;
			else
				adInfo.adIterNoImpr = 0;
			return (adInfo.adIter > adInfo.adIterMax
				|| adInfo.adIterNoImpr > adInfo.adIterNoImprLimt
				|| std::abs(adInfo.adUb - adInfo.adLb) / std::abs(adInfo.adUb) < adInfo.adGap);
		};
		// ͳ��һ�׶α�����
		for (OpULInt i = 0; i < mdl_->fsv_.getSize(); i++)
			firstVarSols.emplace(mdl_->fsv_[i].getImpl(), mdl_->fss_[i]);
		// ͳ�Ʋ�ȷ������
		for (OpULInt i = 0; i < mdl_->uv_.getSize(); i++)
			unSetSol.emplace(mdl_->uv_[i].getImpl(), mdl_->us_[i]);
		// ��ʼ����ż��
		for (OpULInt i = 0; i < mdl_->ssdv_.getSize(); i++)
			dualSetSol.emplace(mdl_->ssdv_[i].getImpl(), 0);
		// ����������(������ֻ������ȷ����Լ��)
		adInfo.adMasterModel.add(mdl_->uc_);
		// ����������(������ֻ������żԼ��)
		adInfo.adSubModel.add(mdl_->ssdc_);

		// �������
		while (!adQuit())
		{
			// ����������Ŀ�꺯��
			{
				OpLinExpr adSubObjExpr(0);
				for (auto it = mdl_->ssdo_.getLBegin(); it != mdl_->ssdo_.getLEnd(); ++it)
						adSubObjExpr.addLinTerm(it.getVar(), it.getCoeff()); /* ������->ϵ��*��ż���� */
				for (auto it = mdl_->ssdo_.getQBegin(); it != mdl_->ssdo_.getQEnd(); ++it)
				{
					auto var1Ptr = it.getVar1().getImpl(), var2Ptr = it.getVar2().getImpl();
					auto iter1 = firstVarSols.find(var1Ptr), iter2 = firstVarSols.find(var2Ptr);
					if (iter1 != firstVarSols.end() && iter2 == firstVarSols.end())
						adSubObjExpr.addLinTerm(var2Ptr, iter1->second * it.getCoeff()); /* ������->ϵ��*һ�׶α���*��ż���� */
					else if (iter1 == firstVarSols.end() && iter2 != firstVarSols.end())
						adSubObjExpr.addLinTerm(var1Ptr, iter2->second * it.getCoeff()); /* ������->ϵ��*��ż����*һ�׶α��� */
					else
					{
						auto iter3 = unSetSol.find(var1Ptr), iter4 = unSetSol.find(var2Ptr);
						if (iter3 != unSetSol.end() && iter4 == unSetSol.end())
							adSubObjExpr.addLinTerm(var2Ptr, iter3->second * it.getCoeff()); /* ������->ϵ��*��ȷ��������*��ż���� */
						else if (iter3 == unSetSol.end() && iter4 != unSetSol.end())
							adSubObjExpr.addLinTerm(var1Ptr, iter4->second * it.getCoeff()); /* ������->ϵ��*��ż����*��ȷ�������� */
						else
							; /*����������*/
					}
				}
				adInfo.adSubModel.setObj(adInfo.adSubTmpObj); // ����Ϊ�˸���Ŀ�꺯����������һ���յ���ʱĿ�꺯��
				adInfo.adSubObj.setLinExpr(adSubObjExpr); // ��ʱԭĿ�꺯���Ѿ����������������������ʽ
				adInfo.adSubModel.setObj(adInfo.adSubObj); // ����ٽ�ԭĿ�꺯���������¼���ģ��
			}
			// ���������
			adSubSolver.extract(adInfo.adSubModel);
			adSubSolver.setParam(config);
			adSubSolver.solve();
			adInfo.adSubStatus = IntStatus2OpStatus(info.solverMode, adSubSolver.getStatus());
			// ������ȡֵ&�����½�
			if (adInfo.adSubStatus == OpSolStatus::Optimal)
			{
				// ���¶�ż������
				for (auto& lbd : dualSetSol)
					lbd.second = adSubSolver.getValue(lbd.first);
				// ����Lb
				adInfo.adPrevLb = adInfo.adLb;
				adInfo.adLb = adSubSolver.getValue(adInfo.adSubObj);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->solve AD sub model failed";
				throw OpExcBase(msg.str());
			}
			// ����������Ŀ�꺯��
			{
				OpLinExpr adMasterObjExpr(0);
				for (auto it = mdl_->ssdo_.getLBegin(); it != mdl_->ssdo_.getLEnd(); ++it)
					adMasterObjExpr.addLinTerm(dualSetSol.at(it.getVar().getImpl()) * it.getCoeff()); /* ������->ϵ��*��ż���� */
				for (auto it = mdl_->ssdo_.getQBegin(); it != mdl_->ssdo_.getQEnd(); ++it)
				{
					auto var1Ptr = it.getVar1().getImpl(), var2Ptr = it.getVar2().getImpl();
					auto iter1 = firstVarSols.find(var1Ptr), iter2 = firstVarSols.find(var2Ptr);
					if (iter1 != firstVarSols.end() && iter2 == firstVarSols.end())
						adMasterObjExpr.addLinTerm(iter1->second * dualSetSol.at(var2Ptr) * it.getCoeff()); /* ������->ϵ��*һ�׶α���*��ż���� */
					else if (iter1 == firstVarSols.end() && iter2 != firstVarSols.end())
						adMasterObjExpr.addLinTerm(dualSetSol.at(var1Ptr) * iter2->second * it.getCoeff()); /* ������->ϵ��*��ż����*һ�׶α��� */
					else
					{
						auto iter3 = dualSetSol.find(var1Ptr), iter4 = dualSetSol.find(var2Ptr);
						if (iter3 != dualSetSol.end() && iter4 == dualSetSol.end())
							adMasterObjExpr.addLinTerm(var2Ptr, iter3->second * it.getCoeff()); /* ������->ϵ��*��ż����*��ȷ�������� */
						else if (iter3 == dualSetSol.end() && iter4 != dualSetSol.end())
							adMasterObjExpr.addLinTerm(var1Ptr, iter4->second * it.getCoeff()); /* ������->ϵ��*��ȷ��������*��ż���� */
						else
							; /*����������*/
					}
				}
				adInfo.adMasterModel.setObj(adInfo.adMasterTmpObj); // ����Ϊ�˸���Ŀ�꺯����������һ���յ���ʱĿ�꺯��
				adInfo.adMasterObj.setLinExpr(adMasterObjExpr); // ��ʱԭĿ�꺯���Ѿ����������������������ʽ
				adInfo.adMasterModel.setObj(adInfo.adMasterObj); // ����ٽ�ԭĿ�꺯���������¼���ģ��
			}
			// ���������
			adMasterSolver.extract(adInfo.adMasterModel);
			adMasterSolver.setParam(config);
			adMasterSolver.solve();
			adInfo.adMasterStatus = IntStatus2OpStatus(info.solverMode, adMasterSolver.getStatus());
			// ������ȡֵ&�����Ͻ�
			if (adInfo.adMasterStatus == OpSolStatus::Optimal)
			{
				// ���²�ȷ������
				for (auto& un : unSetSol)
					un.second = adMasterSolver.getValue(un.first);
				// ����Ub
				adInfo.adPrevUb = adInfo.adUb;
				adInfo.adUb = adMasterSolver.getValue(adInfo.adMasterObj);
			}
			else
			{
				std::stringstream msg;
				msg << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception->solve AD master model failed";
				throw OpExcBase(msg.str());
			}
			// ��ӡAD������Ϣ
			adInfo.printIterInfo(std::cout, tid);
			// ����OA������Ϣ
			adInfo.adIter++;
		}
		// ����CCG�������(��ȷ������&��ż��)
		for (OpULInt i = 0; i < mdl_->ssdv_.getSize(); i++)
			mdl_->ssds_[i] = dualSetSol.at(mdl_->ssdv_[i].getImpl());
		for (OpULInt i = 0; i < mdl_->uv_.getSize(); i++)
			mdl_->us_[i] = unSetSol.at(mdl_->uv_[i].getImpl());
		// ����CCG�Ͻ�
		info.prevUb = info.ub;
		if (k > 1)
		{
			auto actualObj = adInfo.adUb + mdl_->sspo_.getConstant();
			for (auto it = mdl_->sspo_.getLBegin(); it != mdl_->sspo_.getLEnd(); ++it)
			{
				auto iter = unSetSol.find(it.getVar().getImpl());
				if (iter != unSetSol.end())
					actualObj += it.getCoeff() * iter->second;
			}
			info.ub = info.lb - info.etaVal + actualObj;
		}
		info.subStatus = OpSolStatus::Optimal;
	};
	// lambda������;����->�����������ɲ�����������
	auto createAndAddCuts = [&](OpLInt k) {
		// ���ȸ��Ʊ���������
		std::unordered_map<OpVarI*, OpVar> subVars;
		for (OpULInt i = 0; i < mdl_->sspv_.getSize(); i++)
		{
			const auto& var = mdl_->sspv_[i];
			subVars.emplace(var.getImpl(), OpVar(localEnv, var.getType(), var.getLb(), var.getUb(), var.getName() + "_" + std::to_string(k)));
		}
		for (const auto& v : subVars)
			info.masterAddVars.add(v.second);
		// ͳ�Ʋ�ȷ������
		std::unordered_map<OpVarI*, OpFloat> uncertaintyVarSols;
		for (OpULInt i = 0; i < mdl_->uv_.getSize(); i++)
			uncertaintyVarSols.emplace(mdl_->uv_[i].getImpl(), mdl_->us_[i]);
		// �γ��µ����Ÿ�
		if (info.subStatus == OpSolStatus::Optimal)
		{
			OpLinExpr newObj(mdl_->sspo_.getConstant());
			for (auto it = mdl_->sspo_.getLBegin(); it != mdl_->sspo_.getLEnd(); ++it)
			{
				auto iter = uncertaintyVarSols.find(it.getVar().getImpl());
				if (iter != uncertaintyVarSols.end())
					newObj.addLinTerm(iter->second * it.getCoeff());
				else
					newObj.addLinTerm(subVars.at(it.getVar().getImpl()), it.getCoeff());
			}
			auto newCut = info.eta >= newObj;
			newCut.setName("ccg_cut_" + std::to_string(k));
			//auto newCut = OpLinCon(localEnv, -Constant::Infinity, newObj - info.eta, 0, "ccg_cut_" + std::to_string(k)); // ��һ��д��
			if (newCut.getImpl())
			{
				info.masterAddCons.add(newCut);
				info.masterModel.add(newCut);
			}
		}
		// �γ��µĿ��и�(�ع�Լ��)
		for (OpULInt i = 0; i < mdl_->sspc_.getSize(); i++)
		{
			const auto& originalCon = mdl_->sspc_[i];
			OpLinExpr newConExpr(originalCon.getExpr().getConstant());
			for (auto it = originalCon.getExpr().getLBegin(); it != originalCon.getExpr().getLEnd(); ++it)
			{
				auto varPtr = it.getVar().getImpl();
				if (subVars.find(varPtr) != subVars.end())
					newConExpr.addLinTerm(subVars.at(varPtr), it.getCoeff()); // ���׶�ԭ�������
				else if (uncertaintyVarSols.find(varPtr) != uncertaintyVarSols.end())
					newConExpr.addLinTerm(it.getCoeff() * uncertaintyVarSols.at(varPtr)); // ��ȷ��������
				else
					newConExpr.addLinTerm(it.getVar(), it.getCoeff()); // һ�׶α���
			}
			if (newConExpr.getSize())
			{
				OpLinCon newCon;
				if (mdl_->sspfc_.getVal(originalCon.getImpl()))
					newCon = newConExpr >= originalCon.getLb();
				else
					newCon = newConExpr == originalCon.getLb();
				if (newCon.getImpl())
				{
					info.masterAddCons.add(newCon);
					info.masterModel.add(newCon);
				}
			}
		}
	};
	// lambda�����ж��Ƿ������˳�����
	auto quit = [&]() {
		if (info.iter > 1 ? (std::abs(info.lb - info.prevLb) / std::abs(info.prevLb) <= info.lbEpsilon && std::abs(info.ub - info.prevUb) / std::abs(info.prevUb) <= info.ubEpsilon) : false)
			info.iterNoImpr++;
		else
			info.iterNoImpr = 0;
		return ((info.ub - info.lb) / (std::abs(info.ub) > info.epsilon ? std::abs(info.ub) : 1.0) <= info.epsilon)
			|| (info.iter > info.iterMax)
			|| (info.iterNoImpr >= info.iterNoImprLimt - 1);
	};

	try
	{
		// ��ʱ��
		OpTimer watch(true);
		// ��ʼ��
		init();
		switch (info.subProbSolveMode)
		{
		case 0:
			std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")sub problem mode: OA" << std::endl;
			break;
		case 1:
			std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")sub problem mode: AD" << std::endl;
			break;
		default:
			break;
		}
		if (info.needSolveRobust)
		{
			// ��ʼ������������
			createBasicMasterModel();
			// �����
			while (!quit())
			{
				// �����������
				updateMasterSolution(info.iter);
				// �����������
				switch (info.subProbSolveMode)
				{
				case 0:
					updateSubSolution1(info.iter);
					break;
				case 1:
					updateSubSolution2(info.iter);
					break;
				default:
					break;
				}
				// ����������-���������Ÿ�
				createAndAddCuts(info.iter);
				// ��ӡCCG������Ϣ
				info.printIterInfo(std::cout, tid);
				// ����CCG��������
				info.iter++;
			}
			// �������ڶ��׶������Ի�ȡԭʼ������
			createDeterministicSol2();
			std::cout << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Time(s): " << watch.second() << std::endl;
		}
		else
		{
			createDeterministicSol1();
		}
		// ������
		mdl_->fsos_ = info.lb - info.etaVal;
		mdl_->ssos_ = info.etaVal;
		successful = true;
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "[OPUA::Algorithm::OpAlgoCCG::solve](Thread: " << tid << ")Exception: unknown" << std::endl;
	}
	return successful;
}

Algorithm::OpAlgoCCG::OpAlgoCCG()
	: mdl_(nullptr)
{

}

Algorithm::OpAlgoCCG::OpAlgoCCG(OpRobustModel model)
	: mdl_(model.getImpl())
{

}

Algorithm::OpAlgoCCG::~OpAlgoCCG()
{

}


void Algorithm::DefaultCfg4CCG(Solver::OpConfig& config)
{
	// ����������������(����Ĭ��ֵ)
	config.regCfg("OPUA.Algorithm.CCG.IterMax", OpLInt(1000));
	config.regCfg("OPUA.Algorithm.CCG.CCGGap", OpFloat(1e-5));
	config.regCfg("OPUA.Algorithm.CCG.CCGLBGap", OpFloat(1e-5));
	config.regCfg("OPUA.Algorithm.CCG.CCGUBGap", OpFloat(1e-5));
	config.regCfg("OPUA.Algorithm.CCG.LogOutput", OpBool(false));
	config.regCfg("OPUA.Algorithm.CCG.LogOutputPath", OpStr("Log.txt"));
	config.regCfg("OPUA.Algorithm.CCG.SubProbSolveMode", OpLInt(0));
	config.regCfg("OPUA.Algorithm.CCG.NoImprovementIterLimit", OpLInt(3));
	config.regCfg("OPUA.Algorithm.CCG.FirstStageInitMode", OpLInt(0)); // 0-0ֵ��ʼ�� / 1-������ֵ / 2-ȷ���Խ�
	config.regCfg("OPUA.Algorithm.CCG.GetDeterministicSolution", OpBool(false));
	config.regCfg("OPUA.Algorithm.CCG.MIPTimeLimit", OpFloat(3600));
	config.regCfg("OPUA.Algorithm.CCG.MIPGap", OpFloat(1e-5));
	config.regCfg("OPUA.Algorithm.CCG.MIPSolverMode", OpChar('G')); // MIP�����ѡ��G-GRB / C-CPX / S-SCIP / M-MSK
}

Solver::OpConfig Algorithm::DefaultCfg4CCG()
{
	Solver::OpConfig config;
	DefaultCfg4CCG(config);
	return config;
}