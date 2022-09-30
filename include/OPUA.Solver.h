#pragma once

#include "OPUA.Model.h"

#ifndef OPUA_CPX_VERSION_128
#define OPUA_CPX_VERSION_128
#endif

#ifndef OPUA_GRB_VERSION_95
#define OPUA_GRB_VERSION_95
#endif

#ifndef OPUA_SCIP_VERSION_701
#define OPUA_SCIP_VERSION_701
#endif

#ifndef OPUA_MSK_VERSION_9320
#define OPUA_MSK_VERSION_9320
#endif

#ifndef OPUA_COPT_VERSION_501
#define OPUA_COPT_VERSION_501
#endif

#ifndef OPUA_IPOPT_VERSION_3147
#define OPUA_IPOPT_VERSION_3147
#endif

#ifndef OPUA_GLPK_VERSION_50
#define OPUA_GLPK_VERSION_50
#endif

#ifndef OPUA_CBC_VERSION_2108
#define OPUA_CBC_VERSION_2108
#endif

namespace OPUA
{
	namespace Solver
	{
		class OpConfig;
		class OpCPXCfgCvt;
		class OpGRBCfgCvt;
		class OpSCIPCfgCvt;
		class OpMSKCfgCvt;
		class OpCOPTCfgCvt;
		class OpIPOPTCfgCvt;
		class OpGLPKCfgCvt;
		class OpCBCCfgCvt;

		class OpSolState;
		class OpCPXSolI;
		class OpCPXSol;
		class OpGRBSolI;
		class OpGRBSol;
		class OpSCIPSolI;
		class OpSCIPSol;
		class OpMSKSolI;
		class OpMSKSol;
		class OpCOPTSolI;
		class OpCOPTSol;
		class OpIPOPTSolI;
		class OpIPOPTSol;
		class OpGLPKSolI;
		class OpGLPKSol;
		class OpCBCSolI;
		class OpCBCSol;
		class OpAdapSol;

		/*
			ע��[1] �̶�������ǰ�汾��������Լ��OpLinCon������Լ��OpQuadCon��Ŀ�꺯��OpObj��Ч��������Լ����Ч�����ģ���к��й̶�����ʱ�������������Լ��
			֧�̶ֹ��������ܵ�������ӿ����£�CPX / GRB / MSK / SCIP / COPT / GLPK / IPOPT / CBC
		*/

		// OPUA֧�ֵ����������
		enum class OpSolType
		{
			Unknown,
			CPX,
			GRB,
			SCIP,
			MSK,
			COPT,
			IPOPT,
			GLPK,
			CBC
		};

		// OPUA���״̬
		enum class OpSolStatus
		{
			Unknown,
			Feasible,
			Optimal,
			Infeasible,
			Unbounded,
			InfeasibleOrUnbounded
		};

		// ͨ��������
		// ֻ������������(���������͡������͡�������ַ����ַ���)
		// ��ע��/��ѯ��ֵʱ���Զ�ȥ������Ŀհ��ַ���
		class OpConfig
		{
		protected:
			std::unordered_map<OpStr, OpBool> bcfg_;
			std::unordered_map<OpStr, OpInt> icfg_;
			std::unordered_map<OpStr, OpLInt> lcfg_;
			std::unordered_map<OpStr, OpFloat> fcfg_;
			std::unordered_map<OpStr, OpChar> ccfg_;
			std::unordered_map<OpStr, OpStr> scfg_;

		protected:
			void removeStrSpace(OpStr& key) const; // �Ƴ���ֵ����հ��ַ�
		public:
			// ע������(���ô����򸲸Ǿ�����)
			void regCfg(OpStr key, OpBool val);
			void regCfg(OpStr key, OpInt val);
			void regCfg(OpStr key, OpLInt val);
			void regCfg(OpStr key, OpFloat val);
			void regCfg(OpStr key, OpChar val);
			void regCfg(OpStr key, OpStr val);
			// �������
			void clrCfg(OpStr key, OpBool flag);
			void clrCfg(OpStr key, OpInt flag);
			void clrCfg(OpStr key, OpLInt flag);
			void clrCfg(OpStr key, OpFloat flag);
			void clrCfg(OpStr key, OpChar flag);
			void clrCfg(OpStr key, OpStr flag);
			template<typename T> inline void clrCfg(OpStr key) { clrCfg(key, T()); }
			void clrAll();
			// ��ȡ����
			OpBool getCfg(OpStr key, OpBool flag) const;
			OpInt getCfg(OpStr key, OpInt flag) const;
			OpInt getCfg(OpStr key, OpLInt flag) const;
			OpFloat getCfg(OpStr key, OpFloat flag) const;
			OpInt getCfg(OpStr key, OpChar flag) const;
			OpStr getCfg(OpStr key, OpStr flag) const;
			template<typename T> inline auto getCfg(OpStr key) const { return getCfg(key, T()); }
		public:
			template<typename T>
			class OpCfgCIter
			{
			private:
				using KeyValPair = typename std::unordered_map<OpStr, T>::const_iterator;
				KeyValPair iter_;
				OpStr prefix_;

				friend class OpConfig;
			private:
				OpBool filter(OpStr key) const { return key.find(prefix_) != OpStr::npos; }
			public:
				OpBool operator==(const OpCfgCIter<T>& iter) const { return iter_ == iter.iter_; }
				OpBool operator!=(const OpCfgCIter<T>& iter) const { return iter_ != iter.iter_; }
				OpCfgCIter<T>& operator--() { --iter_; return *this; }
				OpCfgCIter<T>& operator++() { ++iter_; return *this; }
				OpBool ok() const { return filter(iter_->first); }
				const OpStr& getKey() const { return iter_->first; }
				const T& getVal() const { return iter_->second; }
			protected:
				OpCfgCIter(KeyValPair iter, OpStr prefix)
					: iter_(iter), prefix_(prefix) {}
			};

			// ��������
			OpCfgCIter<OpBool> getCBegin(OpStr prefix, OpBool flag) const;
			OpCfgCIter<OpInt> getCBegin(OpStr prefix, OpInt flag) const;
			OpCfgCIter<OpLInt> getCBegin(OpStr prefix, OpLInt flag) const;
			OpCfgCIter<OpFloat> getCBegin(OpStr prefix, OpFloat flag) const;
			OpCfgCIter<OpChar> getCBegin(OpStr prefix, OpChar flag) const;
			OpCfgCIter<OpStr> getCBegin(OpStr prefix, OpStr flag) const;
			template<typename T> inline auto getCBegin(OpStr prefix) const { return getCBegin(prefix, T()); }

			OpCfgCIter<OpBool> getCEnd(OpStr prefix, OpBool flag) const;
			OpCfgCIter<OpInt> getCEnd(OpStr prefix, OpInt flag) const;
			OpCfgCIter<OpLInt> getCEnd(OpStr prefix, OpLInt flag) const;
			OpCfgCIter<OpFloat> getCEnd(OpStr prefix, OpFloat flag) const;
			OpCfgCIter<OpChar> getCEnd(OpStr prefix, OpChar flag) const;
			OpCfgCIter<OpStr> getCEnd(OpStr prefix, OpStr flag) const;
			template<typename T> inline auto getCEnd(OpStr prefix) const { return getCEnd(prefix, T()); }
		};

		// ����ʵ��Solver����Ӧ�ӿ�OpAdapSol��״̬��
		// ���е�������ӿڶ�Ҫ�̳�����ಢʵ�ֻ����ķ��ʺ���
		class OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl) = 0;
			virtual void solve() = 0;
			virtual void solveFixed() = 0;
			virtual void setParam(const OpConfig& cfg) = 0;
			virtual OpLInt getStatus() const = 0;
			virtual OpFloat getObjValue() const = 0;
			virtual OpFloat getValue(Variable::OpVar var) const = 0;
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const = 0;
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const = 0;
			virtual OpFloat getValue(Objective::OpObj obj) const = 0;
			virtual OpFloat getDual(Constraint::OpLinCon con) const = 0;
			virtual void write(OpStr path) const = 0;
			virtual void release0() = 0; // ������release����
		public:
			OpSolState();
			virtual ~OpSolState();
		};

		/*
			OpCPXBSol�������Cplex�Ľӿ���
			������˵����doc/OPUA.Solver.CPX_API.md
		*/
		class OpCPXSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // ��ȡOPUAģ�ͣ��γ�CPXģ�Ͷ���
			virtual void solve(); // ִ�����
			virtual void solveFixed(); // �̶����������Ⲣִ�����
			virtual void setParam(const OpConfig& cfg); // ��������
			virtual OpLInt getStatus() const; // ��ȡ���״̬
			virtual OpFloat getObjValue() const; // ��ȡĿ�꺯����
			virtual OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
			OpCPXSolI* getImpl() const; // ��ȡImpl
			virtual void write(OpStr path) const; // ��ģ��д���ļ�
			virtual void release0(); // �ͷ��ڴ�(����ӿ��Ǹ�OpAdapSol�õģ��ֶ��ͷ������release())
		public:
			OpBool operator==(const OpCPXSol& sol) const;
			OpBool operator!=(const OpCPXSol& sol) const;
		public:
			OpCPXSol(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpCPXSol(OpCPXSolI* impl); // ��impl����
			OpCPXSol(OpEnv env); // ��env����
			OpCPXSol(OpEnv env, Model::OpModel mdl); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpCPXSol();
		};

		/*
			OpGRBSol�������Gurobi�Ľӿ���
			������˵����doc/OPUA.Solver.GRB_API.md
		*/
		class OpGRBSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // ��ȡOPUAģ�ͣ��γ�GRBģ�Ͷ���
			virtual void solve(); // ִ�����
			virtual void solveFixed(); // �̶����������Ⲣִ�����
			virtual void setParam(const OpConfig& cfg); // ��������
			virtual OpLInt getStatus() const; // ��ȡ���״̬
			virtual OpFloat getObjValue() const; // ��ȡĿ�꺯����
			virtual OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
			OpGRBSolI* getImpl() const; // ��ȡImpl
			virtual void write(OpStr path) const; // ��ģ��д���ļ�
			virtual void release0(); // �ͷ��ڴ�(����ӿ��Ǹ�OpAdapSol�õģ��ֶ��ͷ������release())
		public:
			OpBool operator==(const OpGRBSol& sol) const;
			OpBool operator!=(const OpGRBSol& sol) const;
		public:
			OpGRBSol(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpGRBSol(OpGRBSolI* impl); // ��impl����
			OpGRBSol(OpEnv env); // ��env����
			OpGRBSol(OpEnv env, Model::OpModel mdl); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpGRBSol();
		};

		/*
			OpSCIPSol�������SCIP�Ľӿ���
			������˵����doc/OPUA.Solver.SCIP_API.md
		*/
		class OpSCIPSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // ��ȡOPUAģ�ͣ��γ�SCIPģ�Ͷ���
			virtual void solve(); // ִ�����
			virtual void solveFixed(); // �̶����������Ⲣִ�����
			virtual void setParam(const OpConfig& cfg); // ��������
			virtual OpLInt getStatus() const; // ��ȡ���״̬
			virtual OpFloat getObjValue() const; // ��ȡĿ�꺯����(SCIP��Ŀ�꺯������Ϊ�����Һ��Գ�����)
			virtual OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
			OpSCIPSolI* getImpl() const; // ��ȡImpl
			virtual void write(OpStr path) const; // ��ģ��д���ļ�
			virtual void release0(); // �ͷ��ڴ�(����ӿ��Ǹ�OpAdapSol�õģ��ֶ��ͷ������release())
		public:
			OpBool operator==(const OpSCIPSol& sol) const;
			OpBool operator!=(const OpSCIPSol& sol) const;
		public:
			OpSCIPSol(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpSCIPSol(OpSCIPSolI* impl); // ��impl����
			OpSCIPSol(OpEnv env); // ��env����
			OpSCIPSol(OpEnv env, Model::OpModel mdl); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpSCIPSol();
		};

		/*
			OpMSKSol�������Mosek�Ľӿ���
			������˵����doc/OPUA.Solver.MSK_API.md
		*/
		class OpMSKSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // ��ȡOPUAģ�ͣ��γ�MSKģ�Ͷ���
			virtual void solve(); // ִ�����
			virtual void solveFixed(); // �̶����������Ⲣִ�����
			virtual void setParam(const OpConfig& cfg); // ��������
			virtual OpLInt getStatus() const; // ��ȡ���״̬
			virtual OpFloat getObjValue() const; // ��ȡĿ�꺯����
			virtual OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
			OpMSKSolI* getImpl() const; // ��ȡImpl
			virtual void write(OpStr path) const; // ��ģ��д���ļ�
			virtual void release0(); // �ͷ��ڴ�(����ӿ��Ǹ�OpAdapSol�õģ��ֶ��ͷ������release())
		public:
			OpBool operator==(const OpMSKSol& sol) const;
			OpBool operator!=(const OpMSKSol& sol) const;
		public:
			OpMSKSol(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpMSKSol(OpMSKSolI* impl); // ��impl����
			OpMSKSol(OpEnv env); // ��env����
			OpMSKSol(OpEnv env, Model::OpModel mdl); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpMSKSol();
		};

		/*
			OpCOPTSol�������COPT�Ľӿ���
			������˵����doc/OPUA.Solver.COPT_API.md
		*/
		class OpCOPTSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // ��ȡOPUAģ�ͣ��γ�COPTģ�Ͷ���
			virtual void solve(); // ִ�����
			virtual void solveFixed(); // �̶����������Ⲣִ�����
			virtual void setParam(const OpConfig& cfg); // ��������
			virtual OpLInt getStatus() const; // ��ȡ���״̬
			virtual OpFloat getObjValue() const; // ��ȡĿ�꺯����
			virtual OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
			OpCOPTSolI* getImpl() const; // ��ȡImpl
			virtual void write(OpStr path) const; // ��ģ��д���ļ�
			virtual void release0(); // �ͷ��ڴ�(����ӿ��Ǹ�OpAdapSol�õģ��ֶ��ͷ������release())
		public:
			OpBool operator==(const OpCOPTSol& sol) const;
			OpBool operator!=(const OpCOPTSol& sol) const;
		public:
			OpCOPTSol(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpCOPTSol(OpCOPTSolI* impl); // ��impl����
			OpCOPTSol(OpEnv env); // ��env����
			OpCOPTSol(OpEnv env, Model::OpModel mdl); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpCOPTSol();
		};

		/*
			OpIPOPTSol�������IPOPT�Ľӿ���
			������˵����doc/OPUA.Solver.IPOPT_API.md
		*/
		class OpIPOPTSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // ��ȡOPUAģ�ͣ��γ�IPOPTģ�Ͷ���
			virtual void solve(); // ִ�����
			virtual void solveFixed(); // �̶����������Ⲣִ�����
			virtual void setParam(const OpConfig& cfg); // ��������
			virtual OpLInt getStatus() const; // ��ȡ���״̬
			virtual OpFloat getObjValue() const; // ��ȡĿ�꺯����
			virtual OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
			OpIPOPTSolI* getImpl() const; // ��ȡImpl
			virtual void write(OpStr path) const; // ��ģ��д���ļ�
			virtual void release0(); // �ͷ��ڴ�(����ӿ��Ǹ�OpAdapSol�õģ��ֶ��ͷ������release())
		public:
			OpBool operator==(const OpIPOPTSol& sol) const;
			OpBool operator!=(const OpIPOPTSol& sol) const;
		public:
			OpIPOPTSol(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpIPOPTSol(OpIPOPTSolI* impl); // ��impl����
			OpIPOPTSol(OpEnv env); // ��env����
			OpIPOPTSol(OpEnv env, Model::OpModel mdl); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpIPOPTSol();
		};

		/*
			OpGLPKSol�������GLPK�Ľӿ���
			������˵����doc/OPUA.Solver.GLPK_API.md
		*/
		class OpGLPKSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // ��ȡOPUAģ�ͣ��γ�GLPKģ�Ͷ���
			virtual void solve(); // ִ�����
			virtual void solveFixed(); // �̶����������Ⲣִ�����
			virtual void setParam(const OpConfig& cfg); // ��������
			virtual OpLInt getStatus() const; // ��ȡ���״̬
			virtual OpFloat getObjValue() const; // ��ȡĿ�꺯����
			virtual OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
			OpGLPKSolI* getImpl() const; // ��ȡImpl
			virtual void write(OpStr path) const; // ��ģ��д���ļ�
			virtual void release0(); // �ͷ��ڴ�(����ӿ��Ǹ�OpAdapSol�õģ��ֶ��ͷ������release())
		public:
			OpBool operator==(const OpGLPKSol& sol) const;
			OpBool operator!=(const OpGLPKSol& sol) const;
		public:
			OpGLPKSol(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpGLPKSol(OpGLPKSolI* impl); // ��impl����
			OpGLPKSol(OpEnv env); // ��env����
			OpGLPKSol(OpEnv env, Model::OpModel mdl); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpGLPKSol();
		};

		/*
			OpCBCSol�������CBC�Ľӿ���
			������˵����doc/OPUA.Solver.CBC_API.md
		*/
		class OpCBCSol
			: public OpBase, public OpSolState
		{
		public:
			virtual void extract(Model::OpModel mdl); // ��ȡOPUAģ�ͣ��γ�CBCģ�Ͷ���
			virtual void solve(); // ִ�����
			virtual void solveFixed(); // �̶����������Ⲣִ�����
			virtual void setParam(const OpConfig& cfg); // ��������
			virtual OpLInt getStatus() const; // ��ȡ���״̬(ע��CBC�����״̬���뱻OPUA���¶���)
			virtual OpFloat getObjValue() const; // ��ȡĿ�꺯����
			virtual OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
			virtual OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			virtual OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
			virtual OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
			OpCBCSolI* getImpl() const; // ��ȡImpl
			virtual void write(OpStr path) const; // ��ģ��д���ļ�
			virtual void release0(); // �ͷ��ڴ�(����ӿ��Ǹ�OpAdapSol�õģ��ֶ��ͷ������release())
		public:
			OpBool operator==(const OpCBCSol& sol) const;
			OpBool operator!=(const OpCBCSol& sol) const;
		public:
			OpCBCSol(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpCBCSol(OpCBCSolI* impl); // ��impl����
			OpCBCSol(OpEnv env); // ��env����
			OpCBCSol(OpEnv env, Model::OpModel mdl); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpCBCSol();
		};

		/*
			OpAdapSol�����������Ӧ�ӿ���
		*/
		class OpAdapSol
		{
		protected:
			const OpSolType stype_;
			OpSolState* rsolver_;
		public:
			void extract(Model::OpModel mdl);
			void solve();
			void solveFixed();
			void setParam(const OpConfig& cfg);
			OpSolType getSolType() const;
			OpLInt getStatus() const;
			OpFloat getObjValue() const;
			OpFloat getValue(Variable::OpVar var) const;
			OpFloat getValue(const Expression::OpLinExpr& expr) const;
			OpFloat getValue(const Expression::OpQuadExpr& expr) const;
			OpFloat getValue(Objective::OpObj obj) const;
			OpFloat getDual(Constraint::OpLinCon con) const;
			void write(OpStr path) const;
			void release();
		public:
			OpAdapSol& operator=(const OpAdapSol& solver);
		public:
			OpAdapSol() = delete;
			OpAdapSol(const OpAdapSol& solver);
			OpAdapSol(OpAdapSol&& solver);
			OpAdapSol(OpSolType type, OpEnv env);
			OpAdapSol(OpSolType type, OpEnv env, Model::OpModel mdl);
			~OpAdapSol();
		};

		// ���״̬ת������(CPX)
		OpSolStatus IntCPXStatus2OpStatus(OpLInt status);
		// ���״̬ת������(GRB)
		OpSolStatus IntGRBStatus2OpStatus(OpLInt status);
		// ���״̬ת������(SCIP)
		OpSolStatus IntSCIPStatus2OpStatus(OpLInt status);
		// ���״̬ת������(MSK)
		OpSolStatus IntMSKStatus2OpStatus(OpLInt status);
		// ���״̬ת������(COPT)
		OpSolStatus IntCOPTStatus2OpStatus(OpLInt status);
		// ���״̬ת������(IPOPT)
		OpSolStatus IntIPOPTStatus2OpStatus(OpLInt status);
		// ���״̬ת������(GLPK)
		OpSolStatus IntGLPKStatus2OpStatus(OpLInt status);
		// ���״̬ת������(CBC)
		OpSolStatus IntCBCStatus2OpStatus(OpLInt status);
		// ���״̬ת������(ͨ��)
		OpSolStatus IntStatus2OpStatus(OpSolType stype, OpLInt status);

		/*
			�������д����ת������
			G - GRB
			C - CPX
			S - SCIP
			M - MSK
			T - COPT
			K - GLPK
			B - CBC
			I - IPOPT
		*/
		OpSolType CharSolType2OpSolType(OpChar type);
	}
}