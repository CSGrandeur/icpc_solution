### `[国家集训队]Crash的数字表格 / JZPTAB`

> difficulty: 4

$$
\sum\limits_{i=1}\limits^{n}
\sum\limits_{j=1}\limits^{m} LCM(i,j) \\
= \sum\limits_{i=1}\limits^{n}
\sum\limits_{j=1}\limits^{m} \frac{ij}{GCD(i,j)} \\
= \sum\limits_{d=1}\limits^{\min(n,m)}\sum\limits_{i=1}\limits^{n}
\sum\limits_{j=1}\limits^{m} \frac{ij}{d}[GCD(i,j)=d] \\
= \sum\limits_{d=1}\limits^{\min(n,m)}\sum\limits_{i=1}\limits^{n}
\sum\limits_{j=1}\limits^{m} \frac{ij}{d}[GCD(\frac{i}{d},\frac{j}{d})=1]
$$


用 $i^{'}=\frac{i}{d}$, $j^{'}=\frac{j}{d}$ 换元代入。为整洁起见换元后的$i^{'}$,$j^{'}$在下述公式中仍然用符号$i$,$j$.  **不失一般性假设$n\leq m$，将式中$min(n,m)$写作$n$，后续涉及$n,m$相关取较小的式子都按此默认**.

$$
\sum\limits_{d=1}\limits^{n}\sum\limits_{i=1}\limits^{n}
\sum\limits_{j=1}\limits^{m} ijd[GCD(i,j)=1] \\
= \sum\limits_{d=1}\limits^{n} d
\sum\limits_{i=1}\limits^{\lfloor \frac{n}{d} \rfloor }
\sum\limits_{j=1}\limits^{\lfloor \frac{m}{d} \rfloor } ij[GCD(i,j)=1]
$$

用一个常用套路：$\sum\limits_{d|n}\mu(d)=1$ 替换$[GCD(i,j)=1]$得

$$
\sum\limits_{d=1}\limits^{n} d
\sum\limits_{i=1}\limits^{\lfloor \frac{n}{d} \rfloor }
\sum\limits_{j=1}\limits^{\lfloor \frac{m}{d} \rfloor } ij
\sum\limits_{t|GCD(i,j)}\mu(t)
$$

调换求和顺序，将$\sum\limits_{t|GCD(i,j)}\mu(t)$提前，原则是“入内求交，出外求并”，可查阅数论交换求和相关知识.

$$
\sum\limits_{d=1}\limits^{n} d
\sum\limits_{t=1}\limits^{\lfloor \frac{n}{d} \rfloor}\mu(t) 
\sum\limits_{t|i}\limits^{\lfloor \frac{n}{d} \rfloor }
\sum\limits_{t|j}\limits^{\lfloor \frac{m}{d} \rfloor } ij 
$$


用 $i^{'}=\frac{i}{t}, j^{'}=\frac{j}{t}$ 替换$i,j$，为简洁替换后仍然保持$i,j$符号代表替换后的$i^{'}$和$j^{'}$.

$$
\sum\limits_{d=1}\limits^{n} d
\sum\limits_{t=1}\limits^{\lfloor \frac{n}{d} \rfloor}t^2\mu(t) 
\sum\limits_{i}\limits^{\lfloor \frac{n}{td} \rfloor }
\sum\limits_{j}\limits^{\lfloor \frac{m}{td} \rfloor } ij 
$$

现在拆解一下计算过程：

$$
g(a,b)=\sum\limits_{i}\limits^{a}\sum\limits_{j}\limits^{b} ij  \\
s(a,b)=\sum\limits_{t=1}\limits^{a}t^2\mu(t) g(\lfloor \frac{a}{t}\rfloor, \lfloor \frac{b}{t}\rfloor) \\
Solve(n,m)=\sum\limits_{d=1}\limits^{n} d\cdot s(\lfloor \frac{n}{d}\rfloor, \lfloor \frac{m}{d}\rfloor)
$$

其中：

- $g(a,b)$ 可以 `O(1)` 计算
- $s(a,b)$ 可以数论分块
- $Solve(n,m)$ 也可以数论分块


```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<vector>
const int mod = 20101009;
int n, m;
std::vector<int> mu, prm, mup;
void MuList(int mxn) {
    mu.resize(mxn + 10); mup.resize(mxn + 10);
    std::fill(mu.begin(), mu.end(), -2);
    mu[1] = mup[1] = 1;
    prm.clear();
    for (int i = 2; i <= mxn; i ++) {
        if (mu[i] == -2) prm.push_back(i), mu[i] = -1;
        for (int j = 0; j < prm.size() && i * prm[j] <= mxn; j ++) {
            if(i % prm[j] == 0) {
                mu[i * prm[j]] = 0;
                break;
            }
            mu[i * prm[j]] = -mu[i];
        }
    }
    for(int i = 2; i <= mxn; i ++) mup[i] = (mup[i - 1] + 1LL * mu[i] * i * i % mod) % mod;
}
inline int G(int a, int b){return (1LL * (b + 1) * b / 2 % mod) * (1LL * (a + 1) * a / 2 % mod) % mod;}
int S(int a, int b){
    int ret = 0;
    for(int i = 1, j; i <= a; i = j + 1){
        j = std::min(a / (a / i), b / (b / i));
        ret = (ret + 1LL * (mup[j] - mup[i - 1]) % mod * G(a / i, b / i) % mod) % mod;
    }
    return ret;
}
int Solve(int n, int m)
{
    int ret = 0;
    if(n > m) std::swap(n, m);
    for(int i = 1, j; i <= n; i = j + 1){
        j = std::min(n / (n / i), m / (m / i));
        ret = (ret + 1LL * (i + j) * (j - i + 1) / 2 % mod * S(n / i, m / i) % mod) % mod;
    }
    return (ret + mod) % mod;
}
int main(){
    MuList(1e7 + 1);
    while(scanf("%d%d", &n, &m) != EOF)
        printf("%d\n", Solve(n, m));
    return 0;
}
```