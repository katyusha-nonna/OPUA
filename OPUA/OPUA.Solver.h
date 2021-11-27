#pragma once

#include "OPUA.Model.h"

namespace OPUA
{
	namespace Solver
	{
		class OpConfig;
		class OpCPXCfgCvt;
		class OpGRBCfgCvt;
		class OpSCIPCfgCvt;

		class OpCPXSolI;
		class OpCPXSol;
		class OpGRBSolI;
		class OpGRBSol;
		class OpSCIPSolI;
		class OpSCIPSol;
		class OpMSKSolI;
		class OpMSKSol;

		// ͨ��������
		// ֻ������������(������������������ַ���)
		// ��ע��/��ѯ��ֵʱ���Զ�ȥ������Ŀհ��ַ���
		class OpConfig
		{
		protected:
			std::unordered_map<OpStr, OpBool> bcfg_;
			std::unordered_map<OpStr, OpLInt> icfg_;
			std::unordered_map<OpStr, OpFloat> fcfg_;
			std::unordered_map<OpStr, OpStr> scfg_;

		protected:
			void removeStrSpace(OpStr& key) const; // �Ƴ���ֵ����հ��ַ�
		public:
			// ע������(���ô����򸲸Ǿ�����)
			void regCfg(OpStr key, OpBool val);
			void regCfg(OpStr key, OpLInt val);
			void regCfg(OpStr key, OpFloat val);
			void regCfg(OpStr key, OpStr val);
			// �������
			void clrCfg(OpStr key, OpBool flag);
			void clrCfg(OpStr key, OpLInt flag);
			void clrCfg(OpStr key, OpFloat flag);
			void clrCfg(OpStr key, OpStr flag);
			template<typename T> inline void clrCfg(OpStr key) { clrCfg(key, T()); }
			void clrAll();
			// ��ȡ����
			OpBool getCfg(OpStr key, OpBool flag) const;
			OpLInt getCfg(OpStr key, OpLInt flag) const;
			OpFloat getCfg(OpStr key, OpFloat flag) const;
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
			OpCfgCIter<OpLInt> getCBegin(OpStr prefix, OpLInt flag) const;
			OpCfgCIter<OpFloat> getCBegin(OpStr prefix, OpFloat flag) const;
			OpCfgCIter<OpStr> getCBegin(OpStr prefix, OpStr flag) const;
			template<typename T> inline auto getCBegin(OpStr prefix) const { return getCBegin(prefix, T()); }

			OpCfgCIter<OpBool> getCEnd(OpStr prefix, OpBool flag) const;
			OpCfgCIter<OpLInt> getCEnd(OpStr prefix, OpLInt flag) const;
			OpCfgCIter<OpFloat> getCEnd(OpStr prefix, OpFloat flag) const;
			OpCfgCIter<OpStr> getCEnd(OpStr prefix, OpStr flag) const;
			template<typename T> inline auto getCEnd(OpStr prefix) const { return getCEnd(prefix, T()); }
		};

		/*
			OpCPXBSol�������Cplex�Ľӿ���
			������˵����
		*/
		class OpCPXSol
			: public OpBase
		{
		public:
			void extract(Model::OpModel mdl); // ��ȡOPUAģ�ͣ��γ�CPXģ�Ͷ���
			void solve(); // ִ�����
			void setParam(const OpConfig& cfg); // ��������
			OpLInt getStatus() const; // ��ȡ���״̬
			OpFloat getObjValue() const; // ��ȡĿ�꺯����
			OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
			OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
			OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
			OpCPXSolI* getImpl() const; // ��ȡImpl
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
			������˵����
			[1] OPUA.GRB.Termination(��ֹ����)
				OPUA.GRB.Termination.BarIterLimit / int / 1000 / [0, MAXINT] / �˳��������ڵ㷨�ѵ�������
				OPUA.GRB.Termination.BestBdStop / double / INF / [-INF, INF] / ?
				OPUA.GRB.Termination.BestObjStop / double / -INF / [-INF, INF] / �˳�������Ŀ�꺯�����Ź���ֵ(�����ֵС�Ϳ����˳�)
				OPUA.GRB.Termination.Cutoff / double / INF / [-INF, INF] / ?
				OPUA.GRB.Termination.IterationLimit / double / INF / [0, INF] / �˳������������η��ѵ�������
				OPUA.GRB.Termination.NodeLimit / double / INF / [0, INF] / �˳��������ѱ�����MIP�ڵ����
				OPUA.GRB.Termination.SolutionLimit / int / INF / [1, MAXINT] / �˳����������ҵ��Ŀ��н����
				OPUA.GRB.Termination.TimeLimit / double / INF / [0, INF] / �˳�����������Ѻ�ʱ

			[2] OPUA.GRB.Tolerances(������&�����������ж�����)
				OPUA.GRB.Tolerances.BarConvTol / double / 1e-8 / [0.0, 1.0] / �ڵ㷨������϶
				OPUA.GRB.Tolerances.BarQCPConvTol / double / 1e-6 / [0.0, 1.0] / �ڵ㷨������϶(QCP����)
				OPUA.GRB.Tolerances.FeasibilityTol / double / 1e-6 / [1e-9, 1e-2] / Լ�������������ж���϶
				OPUA.GRB.Tolerances.IntFeasTol / double / 1e-5 / [1e-9, 1e-1] / ������������ж���϶
				OPUA.GRB.Tolerances.MarkowitzTol / double / 0.0078125 / [1e-4, 0.999] / ?
				OPUA.GRB.Tolerances.MIPGap / double / 1e-4 / [0, INF] / MIP�������������϶
				OPUA.GRB.Tolerances.MIPGapAbs / double / 1e-10 / [0, INF] / MIP�������������϶
				OPUA.GRB.Tolerances.OptimalityTol / double / 1e-6 / [1e-9, 1e-2] / ��ż�������ж���϶(ԭ�����������ж���϶)
				OPUA.GRB.Tolerances.PSDTol / double / 1e-6 / [0, INF] / ?

			[3] OPUA.GRB.Simplex(�����η�����)
				OPUA.GRB.Simplex.InfUnbdInfo / int / 0 / [0, 1] / ?
				OPUA.GRB.Simplex.NormAdjust / int / -1 / [-1, 3] / ?
				OPUA.GRB.Simplex.ObjScale / double / 0.0 / [-1, INF] / ?
				OPUA.GRB.Simplex.PerturbValue / double / 0.0002 / [0, INF] / ?
				OPUA.GRB.Simplex.Quad / int / -1 / [-1, 1] / ?
				OPUA.GRB.Simplex.ScaleFlag / int / -1 / [-1, 3] / ?
				OPUA.GRB.Simplex.Sifting / int / -1 / [-1, 2] / ?
				OPUA.GRB.Simplex.SiftMethod / int / -1 / [-1, 2] / ?
				OPUA.GRB.Simplex.SimplexPricing / int / -1 / [-1, 3] / ?

			[4] OPUA.GRB.Barrier(�ڵ㷨����)
				OPUA.GRB.Barrier.BarCorrectors / int / -1 / [-1, MAXINT] / ?
				OPUA.GRB.Barrier.BarHomogeneous / int / -1 / [-1, 1] / ?
				OPUA.GRB.Barrier.BarOrder / int / -1 / [-1, 1] / ?
				OPUA.GRB.Barrier.Crossover / int / -1 / [-1, 4] / ?
				OPUA.GRB.Barrier.CrossoverBasis / int / 0 / [0, 1] / ?
				OPUA.GRB.Barrier.QCPDual / int / 0 / [0, 1] / ?

			[5] OPUA.GRB.MIP(�������������)
				OPUA.GRB.MIP.BranchDir / int / 0 / [-1, 1] / ��֧���緽��ѡ��(0-�Զ� / -1-���ȱ����·�֧ / 1-���ȱ����Ϸ�֧)
				OPUA.GRB.MIP.ConcurrentJobs / int / 0 / [0, MAXINT] / ���������������
				OPUA.GRB.MIP.ConcurrentMIP / int / 1 / [0, MAXINT] / ���й�����MIP�������
				OPUA.GRB.MIP.ConcurrentSettings / string / "" / ���л������ò���
				OPUA.GRB.MIP.DegenMoves / int / -1 / [-1, MAXINT] / ?
				OPUA.GRB.MIP.Disconnected / int / -1 / [-1, 2] / ?
				OPUA.GRB.MIP.DistributedMIPJobs / int / 0 / [0, MAXINT] / ?
				OPUA.GRB.MIP.Heuristics / double / 0.05 / [0, 1] / ʹ������ʽ����̽��MIP���н�ĺ�ʱ�̶�(ռ�����ʱ��ı���)
				OPUA.GRB.MIP.ImproveStartGap / double / 0.0 / [0.0, INF] / MIP�������п�ʼ�л�Ѱ�Ų��Ե����������϶���(RelGapС�ڸ�ֵʱ���л�Ѱ�Ų���)
				OPUA.GRB.MIP.ImproveStartNodes / double / INF / [0.0, INF] / Ч��ͬImproveStartGap��������Ϊ��������MIP�ڵ�����
				OPUA.GRB.MIP.ImproveStartTime / double / INF / [0.0, INF] / Ч��ͬImproveStartGap��������Ϊ�Ѻ�ʱ
				OPUA.GRB.MIP.LazyConstraints / int / 0 / [0, 1] / ����LazyԼ��ʱ���뿪��
				OPUA.GRB.MIP.MinRelNodes / int / -1/ [-1, MAXINT] / ?
				OPUA.GRB.MIP.MIPFocus / int / 0 / [0, 3] / Ӱ��Ѱ�Ų��Ե����ã���ֵԽ��Խ��������Ѱ���Ž⣬��ֵԽСԽ��������Ѱ���н�
				OPUA.GRB.MIP.MIQCPMethod / -1 / [-1, 1] / MIQCP���ģʽ(-1-�Զ�ѡ�� / 0-��ÿ���ڵ������QCP�ɳ� / 1-ʹ�����Ի�����ƽ���)
				OPUA.GRB.MIP.NodefileDir / string / "" / ?
				OPUA.GRB.MIP.NodefileStart / double / INF / [0, INF] / ?
				OPUA.GRB.MIP.NodeMethod / int / -1 / [-1, 2] / MIP�ڵ�������ɳ�������㷨ѡ��ģʽ(-1-�Զ� / 0-ԭʼ������ / 1-��ż������ / 2-�ڵ㷨)
				OPUA.GRB.MIP.NonConvex / int / -1 / [-1, 2] / ���MIP����˫������ȷ����Բ��֣���Ҫ�򿪴˿��أ����򱨴�
				OPUA.GRB.MIP.NoRelHeurTime / double / 0 / [0, INF] / ?
				OPUA.GRB.MIP.NoRelHeurWork / double / 0 / [0, INF] / ?
				OPUA.GRB.MIP.PartitionPlace / int / 15 / [0, 31] / λ���أ����GRB�û��ֲ�
				OPUA.GRB.MIP.PumpPassese / int / -1 / [-1, MAXINT] / ?
				OPUA.GRB.MIP.RINS / int / -1 / [-1, MAXINT] / RINS̽��Ƶ��(-1-�Զ� / other-̽��Ƶ������)
				OPUA.GRB.MIP.SolFiles / string / "" / ��ǰ�������ļ�·��
				OPUA.GRB.MIP.SolutionNumber / int / 0 / [0, MAXINT] / ��MIP���ڶ��ʱ����ѡ�е�MIP������
				OPUA.GRB.MIP.StartNodeLimit / int / -1/ [-3, MAXINT] / ?
				OPUA.GRB.MIP.StartNumber / int / 0 / [-1, MAXINT] / �����ڶ��MIP���ʱ����ѡ�е�MIP�������
				OPUA.GRB.MIP.SubMIPNodes / int / 500 / [-1, MAXINT] / ?
				OPUA.GRB.MIP.Symmetry / int / -1 / [-1, 2] / ?
				OPUA.GRB.MIP.VarBranch / int / -1 / [-1, 3] / ��֧����ʱѡ������Ĳ��ԣ����GRB�û��ֲ�
				OPUA.GRB.MIP.ZeroObjNodes  / int / -1 / [-1, MAXINT] / ?

		*/
		class OpGRBSol
			: public OpBase
		{
		public:
			void extract(Model::OpModel mdl); // ��ȡOPUAģ�ͣ��γ�GRBģ�Ͷ���
			void solve(); // ִ�����
			void setParam(const OpConfig& cfg); // ��������
			OpLInt getStatus() const; // ��ȡ���״̬
			OpFloat getObjValue() const; // ��ȡĿ�꺯����
			OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
			OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
			OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
			OpGRBSolI* getImpl() const; // ��ȡImpl
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
			OpSCIPBSol�������SCIP�Ľӿ���
			������˵����
		*/
		class OpSCIPSol
			: public OpBase
		{
		public:
			void extract(Model::OpModel mdl); // ��ȡOPUAģ�ͣ��γ�SCIPģ�Ͷ���
			void solve(); // ִ�����
			void setParam(const OpConfig& cfg); // ��������
			OpLInt getStatus() const; // ��ȡ���״̬
			OpFloat getObjValue() const; // ��ȡĿ�꺯����
			OpFloat getValue(Variable::OpVar var) const; // ��ȡ�����Ľ�
			OpFloat getValue(const Expression::OpLinExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			OpFloat getValue(const Expression::OpQuadExpr& expr) const; // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			OpFloat getValue(Objective::OpObj obj) const; // ��ȡĿ�꺯����(�ٶȽ���)
			OpFloat getDual(Constraint::OpLinCon con) const; // ��ȡ��ż��
			OpSCIPSolI* getImpl() const; // ��ȡImpl
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
	}
}