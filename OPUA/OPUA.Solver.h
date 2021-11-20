#pragma once

#include "OPUA.Model.h"

namespace OPUA
{
	namespace Solver
	{
		class OpCPXSolI;
		class OpCPXSol;
		class OpGRBSolI;
		class OpGRBSol;
		class OpSCIPSolI;
		class OpSCIPSol;
		class OpMSKSolI;
		class OpMSKSol;

		class OpGRBSol
			: public OpBase
		{
		public:
			void extract();
			void solve();
			OpFloat getValue(Variable::OpVar var);
			OpFloat getValue(const Expression::OpLinExpr& expr);
			OpFloat getValue(const Expression::OpQuadExpr& expr);

		};
	}
}