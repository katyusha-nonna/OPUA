#pragma once

#include "OPUA.Model.h"

namespace OPUA
{
	namespace Solver
	{
		class OpConfig;
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
		class OpConfig
		{
		protected:
			std::unordered_map<OpStr, OpBool> bcfg_;
			std::unordered_map<OpStr, OpLInt> icfg_;
			std::unordered_map<OpStr, OpFloat> fcfg_;
			std::unordered_map<OpStr, OpStr> scfg_;
		public:
			// ע������(���ô����򸲸Ǿ�����)
			void RegisterConfig(OpStr key, OpBool val);
			void RegisterConfig(OpStr key, OpLInt val);
			void RegisterConfig(OpStr key, OpFloat val);
			void RegisterConfig(OpStr key, OpStr val);
			// �������
			void ClearConfig(OpStr key, OpBool flag);
			void ClearConfig(OpStr key, OpLInt flag);
			void ClearConfig(OpStr key, OpFloat flag);
			void ClearConfig(OpStr key, OpStr flag);
			template<typename T> void ClearConfig(OpStr key) { ClearConfig(key, T()); }
			void ClearAll();
			// ��ȡ����
			OpBool GetConfig(OpStr key, OpBool flag) const;
			OpLInt GetConfig(OpStr key, OpLInt flag) const;
			OpFloat GetConfig(OpStr key, OpFloat flag) const;
			OpStr GetConfig(OpStr key, OpStr flag) const;
			template<typename T> auto GetConfig(OpStr key) const { return GetConfig(key, T()); }
		};

		class OpGRBSol
			: public OpBase
		{
		public:
			void extract(Model::OpModel mdl); // ��ȡOPUAģ�ͣ��γ�GRBģ�Ͷ���
			void solve(); // ִ�����
			void set(OpStr key, OpBool val); // ��������(������)
			void set(OpStr key, OpLInt val); // ��������(������)
			void set(OpStr key, OpFloat val); // ��������(������)
			void set(OpStr key, OpStr val); // ��������(�ַ���)
			OpStr getStatue(); // ��ȡ���״̬
			OpFloat getValue(Variable::OpVar var); // ��ȡ�����Ľ�
			OpFloat getValue(const Expression::OpLinExpr& expr); // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			OpFloat getValue(const Expression::OpQuadExpr& expr); // ��ȡ���ʽ�Ľ�(�ٶȽ���)
			OpFloat getValue(Objective::OpObj obj); // ��ȡĿ�꺯����(�ٶȽ���)
			OpFloat getDual(Constraint::OpLinCon con); // ��ȡ��ż��
		public:
			OpGRBSol(); // Ĭ�Ϲ��캯��(Ĭ��Ϊ��)
			OpGRBSol(OpGRBSolI* impl); // ��impl����
			OpGRBSol(OpEnv env); // ��env����
			OpGRBSol(OpEnv env, OpStr name); // ��env���첢ָ�����ֲ���
			OpGRBSol(OpEnv env, Model::OpModel mdl); // ��env���첢ָ�����ֲ���
			OpGRBSol(OpEnv env, Model::OpModel mdl, OpStr name); // ��env���첢ָ�����ֲ���
		public:
			virtual ~OpGRBSol();
		};
	}
}