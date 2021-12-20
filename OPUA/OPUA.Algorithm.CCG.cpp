#include "OPUA.Common.h"
#include "OPUA.Algorithm.h"
#include "OPUA.Math.h"
#include <thread>
#include <set>

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
protected:
	// ��ʼ��(�������캯��ʹ��)
	void init();
	// ���ģ��(��������������ʹ��)
	void clear();
	// �Զ��Ƶ���ż
	void autoDual();
	// �Զ��Ƶ����������½�
	void autoBound();
	// ��ӱ���&Լ��
	void add(Variable::OpVarArr vars, RobustStageType type, OpBool flag);
	void add(Variable::OpVar var, RobustStageType type, OpBool flag);
	void add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag);
	void add(Constraint::OpLinCon con, RobustStageType type, OpBool flag);
	void add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag);
	void add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag);
	// ����Ŀ�꺯��
	void setObj(Expression::OpLinExpr expr, RobustStageType type);
	void setObj(Expression::OpQuadExpr expr, RobustStageType type);
	// ���ý�
	void setValue(Variable::OpVar var, RobustStageType type, OpFloat val);
	// ���ñ߽�
	void setBound(Variable::OpVar var, RobustStageType type, OpFloat val, OpBool lb);
	void setBound(Variable::OpVar var, RobustStageType type, OpFloat lbVal, OpFloat ubVal);
	// ��ȡ��
	OpFloat getValue(Variable::OpVar var, RobustStageType type) const;
	// ��ȡ����&Լ����Ŀ
	OpULInt getSize(RobustStageType type, OpBool isVar) const;
	// ��ȡ�߽�
	OpFloat getBound(Variable::OpVar var, RobustStageType type, OpBool lb) const;
	// ����ģ��
	void write(OpStr root);
	// �ͷ�ǰ׼��
	void preRelease();
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

void Algorithm::OpRobustModelI::add(Variable::OpVarArr vars, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStage)
	{
		fsv_.add(vars);
	}
	else if (type == RobustStageType::FirstStageSpecial)
	{
		fssv_.add(vars);
	}
	else if (type == RobustStageType::SecondStagePrimal)
	{
		sspv_.add(vars);
		for (int i = 0; i < vars.getSize(); i++)
			sspfv_.add(vars[i].getImpl(), flag);
	}
	else if (type == RobustStageType::SecondStageDual)
	{
		ssdv_.add(vars);
		for (int i = 0; i < vars.getSize(); i++)
			ssdfv_.add(vars[i].getImpl(), flag);
	}
	else if (type == RobustStageType::Uncertainty)
	{
		uv_.add(vars);
	}
	else
		return;
}

void Algorithm::OpRobustModelI::add(Variable::OpVar var, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStage)
	{
		fsv_.add(var);
	}
	else if (type == RobustStageType::FirstStageSpecial)
	{
		fssv_.add(var);
	}
	else if (type == RobustStageType::SecondStagePrimal)
	{
		sspv_.add(var);
		sspfv_.add(var.getImpl(), flag);
	}
	else if (type == RobustStageType::SecondStageDual)
	{
		ssdv_.add(var);
		ssdfv_.add(var.getImpl(), flag);
	}
	else if (type == RobustStageType::Uncertainty)
	{
		uv_.add(var);
	}
	else
		return;
}

void Algorithm::OpRobustModelI::add(Constraint::OpLCArr cons, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStage)
	{
		fsc_.add(cons);
	}
	else if (type == RobustStageType::FirstStageSpecial)
	{
		fssc_.add(cons);
	}
	else if (type == RobustStageType::SecondStagePrimal)
	{
		sspc_.add(cons);
		for (int i = 0; i < cons.getSize(); i++)
			sspfc_.add(cons[i].getImpl(), flag);
	}
	else if (type == RobustStageType::SecondStageDual)
	{
		ssdc_.add(cons);
		for (int i = 0; i < cons.getSize(); i++)
			ssdfc_.add(cons[i].getImpl(), flag);
	}
	else if (type == RobustStageType::Uncertainty)
	{
		uc_.add(cons);
	}
	else
		return;
}

void Algorithm::OpRobustModelI::add(Constraint::OpLinCon con, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStage)
	{
		fsc_.add(con);
	}
	else if (type == RobustStageType::FirstStageSpecial)
	{
		fssc_.add(con);
	}
	else if (type == RobustStageType::SecondStagePrimal)
	{
		sspc_.add(con);
		sspfc_.add(con.getImpl(), flag);
	}
	else if (type == RobustStageType::SecondStageDual)
	{
		ssdc_.add(con);
		ssdfc_.add(con.getImpl(), flag);
	}
	else if (type == RobustStageType::Uncertainty)
	{
		uc_.add(con);
	}
	else
		return;
}

void Algorithm::OpRobustModelI::add(Constraint::OpQCArr cons, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStageSpecial)
	{
		fssqc_.add(cons);
	}
	else
		return;
}

void Algorithm::OpRobustModelI::add(Constraint::OpQuadCon con, RobustStageType type, OpBool flag)
{
	if (type == RobustStageType::FirstStageSpecial)
	{
		fssqc_.add(con);
	}
	else
		return;
}

void Algorithm::OpRobustModelI::setObj(Expression::OpLinExpr expr, RobustStageType type)
{
	if (type == RobustStageType::FirstStage)
	{
		fso_ = expr;
	}
	else if (type == RobustStageType::SecondStagePrimal)
	{
		sspo_ = expr;
	}
	else if (type == RobustStageType::SecondStageDual)
	{
		ssdo_ = expr;
	}
	else
		return;
}

void Algorithm::OpRobustModelI::setObj(Expression::OpQuadExpr expr, RobustStageType type)
{
	if (type == RobustStageType::SecondStageDual)
	{
		ssdo_ = expr;
	}
	else
		return;
}

void Algorithm::OpRobustModelI::setValue(Variable::OpVar var, RobustStageType type, OpFloat val)
{
	auto setVal = [](auto& varArray, auto& valArray, auto var, auto val) {
		auto index = varArray.find(var); // ����Ҳ�����Ӧ��var��find��������size
		if (valArray.idxValid(index))
			valArray[index] = val;
	};
	if (type == RobustStageType::FirstStage)
		setVal(fsv_, fss_, var, val);
	else if (type == RobustStageType::SecondStagePrimal)
		setVal(sspv_, ssps_, var, val);
	else if (type == RobustStageType::SecondStageDual)
		setVal(ssdv_, ssds_, var, val);
	else if (type == RobustStageType::Uncertainty)
		setVal(uv_, us_, var, val);
	else
		return;
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
	OpFloat result(0);
	if (type == RobustStageType::FirstStage)
	{
		auto index = fsv_.find(var);
		if (fsv_.idxValid(index))
			result = fss_[index];
	}
	else if (type == RobustStageType::SecondStagePrimal)
	{
		auto index = sspv_.find(var);
		if (sspv_.idxValid(index))
			result = ssps_[index];
	}
	else if (type == RobustStageType::SecondStageDual)
	{
		auto index = ssdv_.find(var);
		if (ssdv_.idxValid(index))
			result = ssds_[index];
	}
	else if (type == RobustStageType::Uncertainty)
	{
		auto index = uv_.find(var);
		if (uv_.idxValid(index))
			result = us_[index];
	}
	return result;
}

OpULInt Algorithm::OpRobustModelI::getSize(RobustStageType type, OpBool isVar) const
{
	OpULInt result(0);
	if (type == RobustStageType::FirstStage)
		result = isVar ? fsv_.getSize() : fsc_.getSize();
	else if (type == RobustStageType::FirstStageSpecial)
		result = isVar ? fssv_.getSize() : fssc_.getSize() + fssqc_.getSize();
	else if (type == RobustStageType::SecondStagePrimal)
		result = isVar ? sspv_.getSize() : sspc_.getSize();
	else if (type == RobustStageType::SecondStageDual)
		result = isVar ? ssdv_.getSize() : ssdc_.getSize();
	else if (type == RobustStageType::Uncertainty)
		result = isVar ? uv_.getSize() : uc_.getSize();
	return result;
}

OpFloat Algorithm::OpRobustModelI::getBound(Variable::OpVar var, RobustStageType type, OpBool lb) const
{
	OpFloat bound(lb ? -Constant::Infinity : Constant::Infinity);
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

void Algorithm::OpRobustModel::autoDual()
{
	static_cast<OpRobustModelI*>(impl_)->autoDual();
}

void Algorithm::OpRobustModel::autoBound()
{
	static_cast<OpRobustModelI*>(impl_)->autoBound();
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

void OPUA::Algorithm::OpRobustModel::setObj(Expression::OpLinExpr expr, RobustStageType type)
{
	static_cast<OpRobustModelI*>(impl_)->setObj(expr, type);
}

void OPUA::Algorithm::OpRobustModel::setObj(Expression::OpQuadExpr expr, RobustStageType type)
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
