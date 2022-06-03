#pragma once
#include "OPUA.Container.h"
#include "OPUA.Constant.h"

#ifndef OPUA_COMPILE_EIGEN
#define OPUA_COMPILE_EIGEN
#endif

namespace OPUA
{
	namespace Math
	{
		OpFloat OpMax(OpFloat val1, OpFloat val2); // 较大值
		OpFloat OpMax(Container::OpFloatArr vals); // 较大值
		OpFloat OpMin(OpFloat val1, OpFloat val2); // 较小值
		OpFloat OpMin(Container::OpFloatArr vals); // 较大值
		OpFloat OpSum(Container::OpFloatArr vals); // 求和

		/*
			算法类别：一元二次函数分段线性化拟合(割线形式)
			数学形式：
				原函数：		of(x) = a*x^2 + b*x + c
				拟合函数：	af(x) = min{k*x + b}
			参数说明：
				xmin / OpFloat / - / [-inf, inf] / x的下限
				xmax / OpFloat / - / [-inf, inf] / x的上限
				a / OpFloat / - / [-inf, inf] / 二次函数二次项系数
				b / OpFloat / - / [-inf, inf] / 二次函数一次项系数
				c / OpFloat / - / [-inf, inf] / 二次函数常数项系数
				segment / OpULInt / - / [1, inf] / 拟合分段数
			特别说明：
				1. 平均分段，采用割线形式
				2. 有返回值版本的返回值和无返回值版本输入参数中的param，都是扁平化的向量，排列方式为[ [k1, b1], [k2, b2], ... , [kn, bn] ]
				3. 无返回值的版本将清空param，并填入新的数据
		*/
		void OpMathQPWLF(Container::OpFloatArr param, OpFloat xmin, OpFloat xmax, OpFloat a, OpFloat b, OpFloat c, OpULInt segment);
		Container::OpFloatArr OpMathQPWLF(OpEnv env, OpFloat xmin, OpFloat xmax, OpFloat a, OpFloat b, OpFloat c, OpULInt segment);

#ifdef OPUA_COMPILE_EIGEN
		/*
			算法类别：多元一般凸函数分段线性化拟合
			数学形式：
				原函数：		of(X)
				拟合函数：	af(X) = max{AX + B}
			参数说明：
				X / OpFloatArr / - / - / 拟合点X
				Y / OpFloatArr / - / - / 拟合点值of(X)
				numPoint / OpULInt / - / [1, inf] / 拟合点数量
				lenPoint / OpULInt / - / [1, inf] / 拟合点长度
				lenPoint / OpULInt / - / [1, inf] / 拟合点分段数
				segment / OpULInt / - / [1, inf] / 拟合分段数
				iterMax / OpULInt / - / [1, inf] / PWL最大迭代次数
				randInit / OpULInt / - / [1, inf] / PWL是否采用随机初始化(true则随机初始化 / false则使用K均值聚类初始化)(暂时无效)
			特别说明：
				1. 超平面参数的计算参考文献：Magnani A, Boyd S P. Convex piecewise-linear fitting[J]. Optimization and Engineering, 2009, 10(1): 1-17.
				2. 有返回值版本的返回值和无返回值版本输入参数中的param，都是扁平化的向量，排列方式为[ [C1], [C2], ... , [Cn] ]
				3. 无返回值的版本将清空param，并填入新的数据
		*/
		void  OpMathMCPWLF(Container::OpFloatArr param, Container::OpFloatArr X, Container::OpFloatArr Y,
			OpULInt numPoint, OpULInt lenPoint, OpULInt segment, OpULInt iterMax, OpBool randInit);
		Container::OpFloatArr OpMathMCPWLF(OpEnv env, Container::OpFloatArr X, Container::OpFloatArr Y,
			OpULInt numPoint, OpULInt lenPoint, OpULInt segment, OpULInt iterMax, OpBool randInit);
#endif
	}
}