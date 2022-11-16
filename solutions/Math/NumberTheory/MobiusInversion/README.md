## 莫反题目套路

1. $LCM(a,b)$可转为$\frac{ab}{gcd(a,b)}$.
1. $\sum \sum gcd(a,b)$ 可转为条件求和 $\sum\limits_{d} \sum\sum [gcd(a,b)==d]$
1. 条件求和$\sum\limits^{n}\sum\limits^{m} [gcd(a,b)==d]$可通过换元变为$\sum\limits^{\lfloor \frac{n}{d} \rfloor}\sum\limits^{\lfloor \frac{m}{d} \rfloor} [gcd(a^{'},b^{'})==1]$  
    
    类似的另一种形式$\sum\limits_{i=1}^{n}\sum\limits_{j=1}^{m}[d|gcd(i,j)] \Leftrightarrow \sum\limits_{i=1}^{\lfloor \frac{n}{d} \rfloor}\sum\limits_{j=1}^{\lfloor \frac{m}{d} \rfloor}1$，即用枚举$xd,jd$替代枚举$i,j$消除$[d|gcd(i,j)]$.

1. 可转为莫比乌斯函数表示 $[gcd(a,b)==1] \Leftrightarrow \sum\limits_{d|gcd(a,b)}\mu(d)$
1. 求和顺序交换以消除枚举$d|gcd(i,j)$
    $$
    \sum\limits_{i=1}\limits^{n}
    \sum\limits_{j=1}\limits^{m} ij
    \sum\limits_{d|gcd(i,j)}\mu(d)
    \Leftrightarrow
    \sum\limits_{d=1}\limits^{\min(n,m)}\mu(d) 
    \sum\limits_{d|i}\limits^{n}
    \sum\limits_{d|j}\limits^{m} ij 
    $$
1. 枚举约数改为枚举全部，观察等价关系：
    $$
    \sum\limits_{i=1}^{n}\sum\limits_{j=1}^{m}\sum_{x|i}\sum_{y|j}
    \Leftrightarrow
    \sum_{x=1}^{n}\sum_{y=1}^{m} \lfloor \frac{n}{x} \rfloor \lfloor \frac{m}{y}\rfloor
    $$
    由所有$i,j$的所有约数和变为枚举所有约数对其倍数的总贡献.

1. $\sum\limits_{d|i}^{n} i$ 可以换元消除约束条件 $\sum\limits_{i^{'}}^{\lfloor \frac{n}{d}\rfloor}i^{'}$
1. 求和提前，式中加“`[]`”条件： $\sum\limits_{x|i}\sum\limits_{y|j}\sum\limits_{p|gcd(x,y)}\mu(p)=\sum\limits_{p=1}^{min(i,j)}\sum\limits_{x|i}\sum\limits_{y|j}[p|gcd(x,y)]\mu(p)$，相当于把内层`for`循环提到外层，内层则增加了`if`.
1. $\sum\sum [gcd(i,j)=p]$ 这类问题适合莫反，设$f(x)=\sum\sum[gcd(i,j)=x]$，则$g(x)=\sum\limits_{x|d}f(d)$，从而莫反得到$f(x)=\sum\limits_{x|d}\mu(\lfloor \frac{d}{x} \rfloor)g(d)$
1. $\sum\limits_{i=1}^{n}\sum\limits_{j=1}^{m}\sum\limits_{x|d}[gcd(i,j)=d]=\lfloor \frac{n}{d} \rfloor \lfloor \frac{m}{d} \rfloor$，即最大公约数为$x$的所有倍数的个数.