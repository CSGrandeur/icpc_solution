### `[国家集训队]Crash的数字表格 / JZPTAB`

> difficulty: 4

$
\sum\limits_{i=1}\limits^{n}
\sum\limits_{j=1}\limits^{m} LCM(i,j) \\
= \sum\limits_{i=1}\limits^{n}
\sum\limits_{j=1}\limits^{m} \frac{ij}{GCD(i,j)} \\
= \sum\limits_{d=1}\limits^{\min(n,m)}\sum\limits_{i=1}\limits^{n}
\sum\limits_{j=1}\limits^{m} \frac{ij}{d}[GCD(i,j)=d] \\
= \sum\limits_{d=1}\limits^{\min(n,m)}\sum\limits_{i=1}\limits^{n}
\sum\limits_{j=1}\limits^{m} \frac{ij}{d}[GCD(\frac{i}{d},\frac{j}{d})=1]
$


用 $i^{'}=\frac{i}{d}$, $j^{'}=\frac{j}{d}$ 换元代入。为整洁起见换元后的$i^{'}$,$j^{'}$在下述公式中仍然用符号$i$,$j$.  **不失一般性假设$n\leq m$，将式中$min(n,m)$写作$n$，后续涉及$n,m$相关取较小的式子都按此默认**.

$
\sum\limits_{d=1}\limits^{n}\sum\limits_{i=1}\limits^{n}
\sum\limits_{j=1}\limits^{m} ijd[GCD(i,j)=1] \\
= \sum\limits_{d=1}\limits^{n} d
\sum\limits_{i=1}\limits^{\lfloor \frac{n}{d} \rfloor }
\sum\limits_{j=1}\limits^{\lfloor \frac{m}{d} \rfloor } ij[GCD(i,j)=1]
$

用一个常用套路：$\sum\limits_{d|n}\mu(d)=1$ 替换$[GCD(i,j)=1]$得

$
\sum\limits_{d=1}\limits^{n} d
\sum\limits_{i=1}\limits^{\lfloor \frac{n}{d} \rfloor }
\sum\limits_{j=1}\limits^{\lfloor \frac{m}{d} \rfloor } ij
\sum\limits_{t|GCD(i,j)}\mu(t)
$

调换求和顺序，将$\sum\limits_{t|GCD(i,j)}\mu(t)$提前，原则是“入内求交，出外求并”，可查阅数论交换求和相关知识.

$
\sum\limits_{d=1}\limits^{n} d
\sum\limits_{t=1}\limits^{\lfloor \frac{n}{d} \rfloor}\mu(t) 
\sum\limits_{t|i}\limits^{\lfloor \frac{n}{d} \rfloor }
\sum\limits_{t|j}\limits^{\lfloor \frac{m}{d} \rfloor } ij 
$


用 $i^{'}=\frac{i}{t}, j^{'}=\frac{j}{t}$ 替换$i,j$，为简洁替换后仍然保持$i,j$符号代表替换后的$i^{'}$和$j^{'}$.

$
\sum\limits_{d=1}\limits^{n} d
\sum\limits_{t=1}\limits^{\lfloor \frac{n}{d} \rfloor}t^2\mu(t) 
\sum\limits_{i}\limits^{\lfloor \frac{n}{td} \rfloor }
\sum\limits_{j}\limits^{\lfloor \frac{m}{td} \rfloor } ij 
$

现在拆解一下计算过程：

$
g(a,b)=\sum\limits_{i}\limits^{a}\sum\limits_{j}\limits^{b} ij  \\
s(a,b)=\sum\limits_{t=1}\limits^{a}t^2\mu(t) g(\lfloor \frac{a}{t}\rfloor, \lfloor \frac{b}{t}\rfloor) \\
Solve(n,m)=\sum\limits_{d=1}\limits^{n} d\cdot s(\lfloor \frac{n}{d}\rfloor, \lfloor \frac{m}{d}\rfloor)
$

其中：

- $g(a,b)$ 可以 `O(1)` 计算
- $s(a,b)$ 可以数论分块
- $Solve(n,m)$ 也可以数论分块