## 数论常用结论

### 两数之积的约数个数

$d(ij)$ 表示 $i\cdot j$ 的约数个数， $d(ij)=\sum\limits_{x|i}\sum\limits_{y|j}[gcd(x,y)=1]$

考虑映射关系：

假设$i,j$都含有质因数 $p^{a}, p^{b}$.

当$i\cdot j$有约数$k$包含 $p^l, l \leq a$ 时，全都取$i$中的$p$，此时$x$包含$p^l$，$x,y$互质.

当$k$包含$p^q, q > a$ 时，$y$ 包含$p^{q-a}$，但让$x$不包含$p$来对应$x$包含$p^a$的情况.

即 $k$ 包含 $p^{0} \sim p^{a+b}$ 的所有情况，一一对应着 $x$包含$p^0 \sim p^a$、$y包含$p^{0}$ 以及$x$包含$p^{0}$、$y$包含$p^{1} \sim p^{b}$ 的所有情况，这样就得到了每个约数与互质的$x,y$的一一对应关系.

### 求和顺序交换

消除难处理的符号. 原则是 **“入内求交，出外求并”**，即内测和式转移外侧，求和范围与原来外侧范围取并集，反之取交集

$$
\sum\limits_{i=1}\limits^{n}
\sum\limits_{j=1}\limits^{m} ij
\sum\limits_{d|GCD(i,j)}\mu(d)
\Leftrightarrow
\sum\limits_{d=1}\limits^{\min(n,m)}\mu(d) 
\sum\limits_{d|i}\limits^{n}
\sum\limits_{d|j}\limits^{m} ij 
$$

这个例子中，`d`的和式范围取并（$i,j$的公约数 变为了 [1, min(n,m) ]$），$i,j$的和式范围取交，比如$i$范围由$[1,n]$变为了$[1,n]$内能被$d$整除的那些数.

