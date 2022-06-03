# OPUA

## OPUA.Algorithm: Benders分解
---

### 时间：2022年6月3日
### by：Katyusha
---

* ## 0 前言
  本文主要讲述Benders分解的经典形式，参考[^reference1]。

* ## 1 经典模型
  考虑如下的MILP问题，其中：$y\in R^{n_{1}},A\in R^{m_{1}\times n_{1}},b\in R^{m_{1}}, x\in R^{n_{2}},B\in R^{m_{2}\times n_{1}}, D\in R^{m_{2}\times n_{2}}, d\in R^{m_{2}}$：

  $$
    \begin{aligned}
      \min&\quad f^{T}y+c^{T}x\\
      st&\quad Ay=b\\
      &\quad By+Dx=d\\
      &\quad x\geqslant 0\\
      &\quad y\in N
    \end{aligned}\tag{1-1}
  $$

  该问题可以等价于：

  $$
  \min_{\bar{y}\in Y}\{f^{T}\bar{y}+\min_{x\geqslant 0}\{c^{T}x:Dx=d-B\bar{y}\}\}\tag{1-2}
  $$

  其中：$Y=\{y | Ay=b,y\in N\}$。对于(1-2)中的内层的最小化问题，由于是线性优化，因此根据强对偶理论，其对偶问题的最优解和原问题应该相等。其中，对偶问题为：
  
  $$
  \max_{\pi\in R^{m_{2}}}\{\pi^{T}(d-B\bar{y}):\pi^{T}D\leqslant c\}\tag{1-3}
  $$

  因此(1-2)等价于(1-4)：

  $$
  \min_{\bar{y}\in Y}\{f^{T}\bar{y}+\max_{\pi\in R^{m_{2}}}\{\pi^{T}(d-B\bar{y}):\pi^{T}D\leqslant c\}\}\tag{1-4}
  $$

  如果(1-3)的可行域$F=\{\pi | \pi^{T}D\leqslant c\}\neq\emptyset$，那么有两种情况：一、无界(unbounded)；二、可行(feasible)。对于前者，根据单纯形法中的推论，存在一个极方向$r_{q},q\in Q: r_{1}^{T}(d-B\bar{y})>0$。这种情况是要避免的，因为这意味着$\bar{y}$是不可行的。因此添加一条 **可行割** 来保证$\bar{y}$的可行性：

  $$
  r_{q}^{T}(d-B\bar{y})\leqslant 0: \forall q\in Q\tag{1-5}
  $$

  可行割将问题的解限制在第二种情况中，内层最小化问题的解在某一个极点处取到$\pi_{e},e\in E$。因此，对外层最小化问题添加所有的可行割后，内层最小化问题会在极点取到最优解。因此(1-2)可以被重写为：

  $$
    \begin{aligned}
      \min_{\bar{y}\in Y}&\quad f^{T}y+\max_{e\in E}\{\pi_{e}^{T}(d-B\bar{y})\}\\
      st&\quad r_{q}^{T}(d-B\bar{y})\leqslant 0: \forall q\in Q
    \end{aligned}\tag{1-6}
  $$

  也可以构造一个辅助的连续变量，将问题修改为如下形式即得到了BD松弛主问题(MP)：

  $$
    \begin{aligned}
      \min&\quad f^{T}y+\eta\\
      st&\quad Ay=b\\
      &\quad\eta\geqslant\pi_{e}^{T}(d-By): \forall e\in E\\
      &\quad 0\geqslant r_{q}^{T}(d-By): \forall q\in Q\\
      &\quad y\in N
    \end{aligned}\tag{1-7}
  $$

  其中：$\eta\geqslant\pi_{e}^{T}(d-By)$被称为 **最优割** 。

  经典的BD包含一个松弛主问题(简称“主问题”)和一个线性子问题(简称“子问题”)：主问题将$y$的估计值传递给子问题，子问题将割(最优割与可行割)反馈给主问题，直到间隙为0(或近似为0)。由于$\min\max$的形式相当于逐点取下确界，因此每次子问题的解都是原问题的上界。由于主问题的解$\bar{y}$不一定满足可行(有可能需要加可行割)，因此主问题的解为原问题的下界。每次当子问题是可行时，反馈给主问题最优割；当子问题是不可行时，反馈给主问题可行割。

* ## 参考文献
  [^reference1]:Rahmaniani R , Crainic T G , Gendreau M , et al. The Benders decomposition algorithm: A literature review[J]. European Journal of Operational Research, 2016, 259(3):801-817.


