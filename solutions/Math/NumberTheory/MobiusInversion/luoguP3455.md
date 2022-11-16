### `[POI2007]ZAP-Queries`

> difficulty: 2

这道题非常适合入门，只用了最基础的套路，推导所用套路见 [莫反题目套路](./README.md).

为了习惯的符号，这里$a,b$用$n,m$表示，$x,y$用$i,j$表示，且不失一般性假设$n\leq m$ 省略 $\min(n,m)$这类符号.

$$
\begin{align}
Ans & = \sum\limits_{i=1}^{n}\sum\limits_{j=1}^{m}[gcd(i,j)=d] \\
& = \sum\limits_{i=1}^{\lfloor \frac{n}{d} \rfloor}\sum\limits_{j=1}^{\lfloor \frac{m}{d} \rfloor}[gcd(i,j)=1] \\
& = \sum\limits_{i=1}^{\lfloor \frac{n}{d} \rfloor}\sum\limits_{j=1}^{\lfloor \frac{m}{d} \rfloor}\sum\limits_{t|gcd(i,j)}\mu(t) \\
& = \sum\limits_{t=1}^{\lfloor \frac{n}{d}\rfloor} \mu(t)\sum\limits_{t|i}^{\lfloor \frac{n}{d} \rfloor}\sum\limits_{t|j}^{\lfloor \frac{m}{d} \rfloor}1 \\
& = \sum\limits_{t=1}^{\lfloor \frac{n}{d}\rfloor} \mu(t)\lfloor \frac{n}{d} \rfloor \lfloor \frac{m}{d} \rfloor
\end{align}
$$

从而预处理$\mu$函数前缀和以及数论分块解决。

```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<vector>
#include<cmath>
int t, n, m, d;
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
int main() {
    MuList(5e4 + 10);
    for(scanf("%d", &t); t --; ){
        scanf("%d%d%d", &n, &m, &d);
        if(n > m) std::swap(n, m);
        n /= d, m /= d;
        long long ans = 0;
        for(int i = 1, j; i <= n; i = j + 1) {
            j = std::min(n / (n / i), m / (m / i));
            ans += 1LL * (n / i) * (m / i) * (mup[j] - mup[i - 1]);
        }
        printf("%lld\n", ans);
    }
    return 0;
}

```

