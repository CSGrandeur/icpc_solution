### `YY的GCD`

> difficulty: 3

推导所用套路见 [莫反题目套路](./README.md)

求$Ans = \sum\limits_{i=1}^{n}\sum\limits_{j=1}^{m}[gcd(i,j)=prime]$

设 
$$
f(x)=\sum\limits_{i=1}^{n}\sum\limits_{j=1}^{m}[gcd(i,j)=x]
$$

$$
g(x)=\sum\limits_{x|d}f(d)
$$

莫比乌斯反演得

$$
f(x)=\sum\limits_{x|d}\mu(\lfloor \frac{d}{x}\rfloor)g(d)
$$

观察$g(x)\sum\limits_{x|d}f(d)=\sum\limits_{x|d}\sum\limits_{i=1}^{n}\sum\limits_{j=1}^{m}[gcd(i,j)=d]$发现表达的是$n, m$以内的所有$i,j$的公约数是$x$的倍数的情况之和，所以

$$
g(x)=\lfloor \frac{n}{x} \rfloor \lfloor \frac{m}{x} \rfloor
$$

从而

$$
f(x)=\sum\limits_{x|d}\mu(\lfloor \frac{d}{x}\rfloor)\lfloor \frac{n}{d} \rfloor \lfloor \frac{m}{d} \rfloor
$$

代入题目所求：

$$
\begin{align}
Ans & = \sum\limits_{x\in prime}f(x) \\
& = \sum\limits_{x\in prime}\sum\limits_{x|d}^{\min(n,m)}\mu(\lfloor \frac{d}{x}\rfloor)\lfloor \frac{n}{d} \rfloor \lfloor \frac{m}{d} \rfloor \\
& = \sum\limits_{d=1}^{\min(n,m)}\sum\limits_{x\in prime\wedge x|d}\mu(\lfloor \frac{d}{x}\rfloor)\lfloor \frac{n}{d} \rfloor \lfloor \frac{m}{d} \rfloor \\
& = \sum\limits_{d=1}^{\min(n,m)}\lfloor \frac{n}{d} \rfloor \lfloor \frac{m}{d} \rfloor\sum\limits_{x\in prime\wedge x|d}\mu(\lfloor \frac{d}{x}\rfloor)
\end{align}
$$

除了数论分块，要处理$\sum\limits_{x\in prime\wedge x|d}\mu(\lfloor \frac{d}{x}\rfloor)$的关于$d$的前缀和.

```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<vector>
#include<cmath>
int t, n, m;
std::vector<int> mu, prm, mup;
std::vector<long long> mupd;
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
    for(int i = 2; i <= mxn; i ++) mup[i] = mup[i - 1] + mu[i];
}
void MuPdList(int mxn)
{
    mupd.resize(mxn + 10);
    std::fill(mupd.begin(), mupd.end(), 0);
    for(auto p:prm){
        for(int i = 1; i * p <= mxn; i ++)
            mupd[i * p] += mu[i];
    }
    for(int i = 1; i <= mxn; i ++)
        mupd[i] += mupd[i - 1];
}
int main() {
    MuList(1e7 + 10);
    MuPdList(1e7 + 10);
    for(scanf("%d", &t); t --; ){
        scanf("%d%d", &n, &m);
        if(n > m) std::swap(n, m);
        long long ans = 0;
        for(int i = 1, j; i <= n; i = j + 1) {
            j = std::min(n / (n / i), m / (m / i));
            ans += 1LL * (n / i) * (m / i) * (mupd[j] - mupd[i - 1]);
        }
        printf("%lld\n", ans);
    }
    return 0;
}
```