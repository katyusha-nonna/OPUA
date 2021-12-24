# OPUA

## OPUA.Algorithm: 线性化方法
---

### 时间：2021年12月24日
### by：Katyusha
---
* ## 0 前言
  介绍OPUA中对非线性函数的线性化算法的实现原理。

* ## 1 整数$\times$连续项
  对于整数乘连续项一般会被求解器报非凸从而无法求解，可以使用BigM法对这一项进行松弛。

  假设我们的非线性项如下：$w=x*y:x\in\{0,1\},y\in[y^{min},y^{max}]$

  则进行如下松弛：

  $$
  \begin{aligned}
    &r=y-w\qquad&(1)\\
    &x*y^{min}\leq y-r\leq x*y^{max}\qquad&(2)\\
    &(1-x)y^{min}\leq r\leq(1-x)y^{max}\qquad&(3)\\
    &x\in\{0,1\},y\in[y^{min},y^{max}],r\in[y^{min},y^{max}] \qquad&(4) 
  \end{aligned}
  $$

* ## 2 绝对值项
  设绝对值项为：$w=|x|:x\in[x^{min},x^{max}]$，重构如下：
  $$
  \begin{aligned}
    &w=|x|=x^{+}+x^{-}\\
    &x=x^{+}-x^{-}\\
    &0\leqslant x^{+}\leqslant uM\\
    &0\leqslant x^{-}\leqslant (1-u)M\\
    &u\in\{0,1\}, x^{+}\geqslant 0, x^{-}\geqslant 0
  \end{aligned}
  $$

* ## 3 连续$\times$连续项
  对于连续项相乘，如果是同一变量，则变成二次项，对于相异变量相乘，则需要处理。这里特指后者：

  假设我们的非线性项如下：$w=x*y:x\in[x^{min},x^{max}],y\in[y^{min},y^{max}]$。处理的思路为将其中一个连续变量进行二进制离散化：

  $$
  \begin{aligned}
    &x=x^{min}+\Delta x\sum_{k=1}^{N}u_{k}2^{k-1}\\
    &\Delta x=\frac{x^{max}-x^{min}}{2^{N}}\\
    &\forall k\in\{1,\dots,N\}:u_{k}\in\{0,1\}
  \end{aligned}
  $$

  其中N确定了分辨率，N越大，采样点越多，分辨率越小，对$x$的刻画越精细，但是代价是增加的0-1变量越多。

  之后的处理结合“整数$\times$连续项”的处理方式。

* ## 4 概率不等式
  概率不等式一般出现在机会约束中：

  $$Pr(\xi^{T}x\leqslant b)\geqslant 1-\varepsilon\qquad(1)$$

  首先解释一下符号：$\xi$是一个服从多维正态分布的随机变量，即$\xi\sim N(\mu, \Sigma)$，其中$\mu$是均值向量，$\Sigma$是协方差矩阵；$1-\varepsilon$是一个较高的概率值，这里假设$0<\varepsilon<0.5$；$\Phi^{-1}$是标准正态分布的逆累积分布函数；$b$就是一个实数。

  首先，我们可以得到：

  $$E(\xi^{T}x)=E(\Sigma x_{i}\xi_{i})=\Sigma(x_{i}E(\xi_{i}))=\Sigma x_{i}\mu_{i}=\mu^{T}x $$

  和：

  $$
  \begin{aligned}
  Var(\xi^{T}x)&=E[(\xi^{T}x-\mu^{T}x)^{T}(\xi^{T}x-\mu^{T}x)]\\
  &\quad\\
  &=E[((\xi^{T}-\mu^{T})x)^{T}(\xi^{T}-\mu^{T})x]\\
  &\quad\\
  &=E[x^{T}(\xi-\mu)(\xi-\mu)^{T}x]=x^{T}\Sigma x
  \end{aligned}
  $$


  对于$x^{T}\Sigma x=0$的情况，此时$\xi^{T}x=\mu^{T}x$，则上面的公式自然成立。对于$x^{T}\Sigma x>0$，则有：

  $$
  \frac{(\xi-\mu)^{T}x}{\sqrt{x^{T}\Sigma x}}=\frac{\xi^{T}x-\mu^{T}x}{\sqrt{x^{T}\Sigma x}}=\frac{\xi^{T}x-E(\xi^{T}x)}{\sqrt{Var(\xi^{T}x)}}\sim N(0, 1)
  $$

  所以对Pr中的公式进行变换：

  $$
  Pr(\xi^{T}x\leqslant b)=Pr(\frac{(\xi-\mu)^{T}x}{\sqrt{x^{T}\Sigma x}}\leqslant\frac{-\mu^{T}x+b}{\sqrt{x^{T}\Sigma x}})=\Phi(\frac{-\mu^{T}x+b}{\sqrt{x^{T}\Sigma x}})
  $$

  并利用累积分布函数单调递增的特性，则：

  $$
  \begin{aligned}
    Pr(\xi^{T}x\leqslant b)\geqslant 1-\varepsilon&\Leftrightarrow\quad\Phi(\frac{-\mu^{T}x+b}{\sqrt{x^{T}\Sigma x}})\geqslant 1-\varepsilon\\
    &\quad\\
    &\Leftrightarrow\quad\frac{-\mu^{T}x+b}{\sqrt{x^{T}\Sigma x}}\geqslant\Phi^{-1}(1-\varepsilon)\\
    &\quad\\
    &\Leftrightarrow\quad\mu^{T}x+\Phi^{-1}(1-\varepsilon)\sqrt{x^{T}\Sigma x}\leqslant b\qquad(2)
  \end{aligned}
  $$

  因此概率约束被转化为了锥约束：下面介绍二次约束向锥约束的转换细节。

  为了方便后续计算，添加辅助变量$\eta$，因此(2)可以被(3)和(4)代替：

  $$
  \begin{aligned}
    &\sqrt{x^{T}\Sigma x}\leqslant\eta\qquad&(3)\\
    &\mu^{T}x+\Phi^{-1}(1-\varepsilon)\eta\leqslant b\qquad&(4)\\
  \end{aligned}
  $$

  而约束(3)是一个二次约束，二次约束可以被转化为二阶锥约束(通式见附录)：

  $$
  \sqrt{x^{T}\Sigma x}\leqslant\eta\Leftrightarrow\Vert\Sigma^{\frac{1}{2}}x\Vert_{2}\leqslant\eta\qquad(5)
  $$
  
  因此用(4)、(5)可以代替(1)。
  
  特殊的，如果$\xi$之间是相互独立的且$x$不是决策变量，那么该约束退化为一个线性约束。

* ## 5 二次函数
  对于最小化目标函数中的二次函数(这里仅指一元二次函数的特殊情况$f(x)=ax^{2}+bx+c$，多元凸函数的情况见下一节)，可以采用分段线性化(这里仅采用均匀分段)的处理方式，即用多条割线对二次曲线进行逼近：

  有两种方式：
  * [1] 直接添加割线：
    这种就是将刻画二次曲线的数条割线直接写进约束中：

    $$
    \begin{aligned}
      \min&\quad C\\
      st&\quad C\geqslant k_{s}x+b_{s}:\forall s\in\{1,\dots,M\}\\
      &\quad x^{min}\leqslant x\leqslant x^{max}
    \end{aligned}
    $$

    其中，$M$为分段数，$x_{s}$为采样点(采样点为$M+1$个)：

    $$
    \left\{
      \begin{aligned}
         &k_{s}=\frac{f(x_{s+1})-f(x_{s})}{x_{s+1}-x_{s}}\\
         &b_{s}=\frac{x_{s+1}f(x_{s})-x_{s}f(x_{s+1})}{x_{s+1}-x_{s}}
      \end{aligned}
    \right.
    $$

  * [2] 设置分段变量：
    这种是将原先分布在$[x^{min},x^{max}]$上的$x$分为多个连续变量$x_{s}$。

    $$
    \left\{
      \begin{aligned}
         &C_{0}=a(x^{min})^{2}+bx^{min}+c\\
         &K_{s}=\frac{f(p_{s+1})-f(p_{s})}{p_{s+1}-p_{s}}\\
         &p_{s}=(s-1)\frac{(x^{max}-x^{min})}{M}+x^{min}\\
         &x=\sum_{s=1}^{M}x_{s}
      \end{aligned}
    \right.
    $$

    因此目标函数变为：

    $$
    \begin{aligned}
      \min&\quad \sum_{s=1}^{M}K_{s}x_{s}+C_{0}\\
      st&\quad x=\sum_{s=1}^{M}x_{s}\\
      &\quad 0\leqslant x_{s}\leqslant \frac{x^{max}-x^{min}}{M}
    \end{aligned}
    $$

* ## 6 一般(局部)凸非线性函数
  按照对二次函数进行逼近的思路，可以用多个采样点的割平面对多元凸函数进行逼近。可分为两种情况进行讨论，一种是该非线性函数位于目标函数中，一种是该函数位于约束中。

  假设非线性函数$f(x),x\in\mathbb{R}^{n}$在$\underline{x}\leqslant x\leqslant\overline{x}$上具有局部凸性，我们可用一个分段线性化函数$\tilde{f}(x)$对$f(x)$进行估计：

  $$
  \tilde{f}(x)=\max_{1\leqslant i\leqslant m}\{c_{i}^{T}x+b_{i}\}
  $$

  如果我们有$k$个估计点$(x,y=f(x))$就可以通过求解以下min-max问题得到拟合参数：

  $$
  \min_{c_{j},b_{j},1\leqslant j\leqslant m}\sum_{i=1}^{k}(\max_{1\leqslant j\leqslant m}\{c_{j}^{T}x_{i}+b_{j}\}-y_{i})^{2}
  $$

  求解这个问题的思路和K-means的思路类似[^reference1]，使用一种启发式的方法：

  首先定义$P_{j}^{(l)}$:

  $$
  \bigcup_{j}P_{j}^{(l)}=\{1,...,k\}\\
  P_{i}^{(l)}\cap P_{j}^{(l)}=\emptyset:\forall i\neq j
  $$

  $P$将估计点分成了$m$份，每一次迭代的每一个$P_{j}^{(l)}$都代表了参数$(c_{j}^{(l)},b_{j}^{(l)})$能够使估计点$x_{i}$在$\{c_{j}^{(l)T}x_{i}+b_{j}^{(l)T}\}$中取到最大。

  算法流程：
  * [1] 初始化$P_{j}^{(0)}:\forall j\in\{1,...,m\}$，迭代次数上限$l_{max}$;
  * [2] 如果$l>l_{max}$或$P_{j}^{(l)}=P_{j}^{(l+1)}$：结束，否则转[3]；
  * [3] 对于每一个$P_{j}^{(l)}$，计算集合内所包含的估计点的最小二乘估计：
  
    $$
    \left[
      \begin{matrix}
        c_{j}^{(l+1)}\\
        b_{j}^{(l+1)}
      \end{matrix}
    \right]=
    \left[
      \begin{matrix}
        \sum_{i\in P_{j}^{(l)}} x_{i}x_{i}^{T}&\sum_{i\in P_{j}^{(l)}} x_{i}\\
        \sum_{i\in P_{j}^{(l)}} x_{i}^{T}&|P_{j}^{(l)}|I
      \end{matrix}
    \right]^{-1}
    \left[
      \begin{matrix}
        \sum_{i\in P_{j}^{(l)}} y_{i}x_{i}\\
        \sum_{i\in P_{j}^{(l)}} y_{i}
      \end{matrix}
    \right]
    $$

  * [4] 更新$P_{j}^{(0)}$：遍历所有的估计点$(x_{i},y_{i})$，如果满足如下条件，则将$i$加入$P_{j}^{(l)}$。结束后$l+=1$并转[2]：

    $$
    f^{(l)}(x_{i}):=\max_{1\leqslant s\leqslant m}\{c_{s}^{(l)T}x_{i}+b_{s}^{(l)}\}=c_{j}^{(l)T}x_{i}+b_{j}^{(l)}
    $$

  得到拟合参数后，可以使用拟合的函数代替原非线性函数，主要使用情况分为两种：
  * [1] 该函数位于目标函数中：问题转化为min-max问题，进而转化为min问题：

    $$
    \min_{x\in D}\max_{1\leqslant i\leqslant m}\{c_{i}^{T}x+b_{i}\}\leftrightarrow
    \begin{aligned}
      \min&\quad \alpha\\
      st&\quad \alpha\geqslant c_{i}^{T}x+b_{i}:\forall i\in\{1,...,m\}\\
      st&\quad x\in D\\
    \end{aligned}
    $$

    即上一节二次函数线性化的一种基于最小二乘拟合的推广。

  * [2] 该函数位于约束条件中：引入0-1辅助变量，将max进行递推松弛：

    对于$t=\max\{p,q\}$可以使用如下约束进行替换：

    $$
    t=\max\{p,q\}\leftrightarrow
    \left\{
      \begin{aligned}
         &p\leqslant t\leqslant p+bM\\
         &q\leqslant t\leqslant q+(1-b)M
      \end{aligned}
    \right.
    $$


* ## 7 带极大/极小值
  如果是约束条件中出现极大极小，如下[^reference2]：

  $$
  w=\max_{i\in I}{x_{i}}\\
  w=\min_{j\in J}{y_{j}}
  $$

  先解释$\max$的线性化方法，其思路是引入整数变量并使用BigM方法使得至少有一个变量同时满足$x_{i}\leqslant w,x_{i}\geqslant w$同时成立，即$x_{i}=w$成立：

  $$
  w=\max_{i\in I}{x_{i}}\Longleftrightarrow
  \left\{
    \begin{aligned}
      &\forall i\in I:x_{i}\leqslant w\\
      &\forall i\in I:x_{i}\geqslant w-M(1-u_{i})\\
      &\sum_{i\in I}u_{i}\geqslant 1\\
      &\forall i\in I:u_{i}\in\{0,1\}
    \end{aligned}
  \right.  
  $$

  同理可得$\min$的线性化方法：

  $$
  w=\min_{j\in J}{x_{j}}\Longleftrightarrow
  \left\{
    \begin{aligned}
      &\forall j\in J:x_{j}\geqslant w\\
      &\forall j\in J:x_{j}\leqslant w+M(1-u_{j})\\
      &\sum_{j\in J}u_{j}\geqslant 1\\
      &\forall j\in J:u_{j}\in\{0,1\}
    \end{aligned}
  \right.  
  $$

  Gurobi可直接添加这种广义约束，使用Gurobi的Max/Min函数即可。

* ## 8 逻辑约束
  以逻辑或为例，两条不等式约束$con1$和$con2$，其中至少一条成立[^reference2]:

  $$
  (\sum_{i}a_{1,i}x_{i}\leqslant b_{1})\lor(\sum_{j}a_{2,j}y_{j}\leqslant b_{2})
  $$

  其线性化如下：

  $$
  \begin{aligned}
    &\sum_{i}a_{1,i}x_{i}\leqslant b_{1} + M(1-y_{1})\\
    &\sum_{j}a_{2,j}y_{j}\leqslant b_{2} + M(1-y_{2})\\
    &y_{1}+y_{2}\geqslant 1\\
    &y_{1}, y_{2}\in\{0, 1\}
  \end{aligned}
  $$

  其原理就是通过为每条约束添加相应的0-1变量，利用BigM松弛控制约束是否生效。其中$y_{1}+y_{2}\geqslant 1$是为了符合逻辑或的运算结果，可以替换为$\sum_{k}y_{k}\geqslant 1$以推广到$k$个约束的逻辑或等效。

  同理，可以得到等式约束或混合约束的逻辑或等效

  $$
  (\sum_{i}a_{1,i}x_{i}\leqslant b_{1})\lor(\sum_{j}a_{2,j}y_{j}=b_{2})
  $$

  等效如下：

  $$
  \begin{aligned}
    &\sum_{i}a_{1,i}x_{i}\leqslant b_{1} + M(1-y_{1})\\
    &\sum_{j}a_{2,j}y_{j}\leqslant b_{2} + M(1-y_{2})\\
    &\sum_{j}a_{2,j}y_{j}\geqslant b_{2} - M(1-y_{2})\\
    &y_{1}+y_{2}\geqslant 1\\
    &y_{1}, y_{2}\in\{0, 1\}
  \end{aligned}
  $$

  原理就是为等式约束同时松弛上界和下界。


* ## 附录
* ### 1 二次约束转二阶锥约束
  一般的，一个二次约束表达式：

  $$
  x^{T}Ax+q^{T}x+c\leqslant 0
  $$

  可转化为如下的二阶锥约束：

  $$
  \Vert A^{\frac{1}{2}}x+\frac{1}{2}A^{-\frac{1}{2}}q\Vert_{2}\leqslant-\frac{1}{4}q^{T}A^{-1}q-c
  $$

* ## 参考文献
  [^reference1]: Magnani, A., Boyd, S.P. Convex piecewise-linear fitting. Optim Eng 10, 1–17 (2009). https://doi.org/10.1007/s11081-008-9045-3
  [^reference2]: 王源 优化模型线性化方法总结 https://zhuanlan.zhihu.com/p/361766549

  


