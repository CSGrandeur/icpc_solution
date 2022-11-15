### `[SDOI2015]约数个数和`

> difficulty: 4

推荐这篇题解：https://www.luogu.com.cn/blog/peng-ym/solution-p3327

$d(ij)=\sum\limits_{x|i}\sum\limits_{y|j}[gcd(x,y)=1]$，证明见 [数论常用结论](https://github.com/CSGrandeur/icpc_solution/blob/master/solutions/Math/NumberTheory/README.md#两数之积的约数个数).


$$
\begin{align}
&\sum_{i=1}^{n}\sum_{j=1}^{m}d(ij) \\
& = \sum_{i=1}^{n}\sum_{j=1}^{m}\sum\limits_{x|i}\sum\limits_{y|j}[gcd(x,y)=1] \\
& = \sum_{i=1}^{n}\sum_{j=1}^{m}\sum\limits_{x|i}\sum\limits_{y|j}\sum\limits_{d|gcd(x,y)}\mu(d) \\
& = \sum\limits_{d}^{\min(n,m)}\mu(d)\sum_{i=1}^{n}\sum_{j=1}^{m}\sum\limits_{x|i}\sum\limits_{y|j}[d|gcd(x,y)] \\
& = \sum\limits_{d}^{\min(n,m)}\mu(d)\sum_{x=1}^{n}\sum_{y=1}^{m}\lfloor \frac{n}{x} \rfloor \lfloor \frac{m}{y} \rfloor[d|gcd(x,y)] \\
& = \sum\limits_{d}^{\min(n,m)}\mu(d)\sum_{x=1}^{\lfloor \frac{n}{d} \rfloor}\sum_{y=1}^{\lfloor \frac{m}{d} \rfloor}\lfloor \frac{n}{xd} \rfloor \lfloor \frac{m}{yd} \rfloor \\
& = \sum\limits_{d}^{\min(n,m)}\mu(d)(\sum_{x=1}^{\lfloor \frac{n}{d} \rfloor}\lfloor \frac{n}{xd} \rfloor)(\sum_{y=1}^{\lfloor \frac{m}{d} \rfloor}\lfloor \frac{m}{yd} \rfloor)
\end{align}
$$

先打表莫比乌斯函数前缀和，并对所有的$n$打表 $\sum\limits_{i=1}^{n}\lfloor \frac{n}{i}\rfloor$，这样对于任意固定的 $d$，可以`O(1)`获得 $\sum\limits_{x=1}^{\lfloor \frac{n}{d} \rfloor}\lfloor \frac{n}{xd} \rfloor$.

```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<vector>
typedef long long LL;
int n, m;
std::vector<int> mu, prm, mup;
std::vector<LL> g;
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
void GList(int mxn)
{
    // $\sum\limits_{i=1}^{k} \lfloor \frac{k}{i} \rfloor$ 打表
    g.resize(mxn + 10);
    for(int k = 1; k <= mxn; k ++) {
        g[k] = 0;
        for(int i = 1, j; i <= k; i = j + 1) {
            j = k / (k / i);
            g[k] += 1LL * (j - i + 1) * (k / i);
        }
    }
}
int main(){
    int t;
    MuList(5e4 + 10);
    GList(5e4 + 10);
    for(scanf("%d", &t); t --; ){
        scanf("%d%d", &n, &m);
        if(n > m) std::swap(n, m);
        LL ans = 0;
        for(int i = 1, j; i <= n; i = j + 1)
        {
            j = std::min(n / (n / i), m / (m / i));
            ans += 1LL * (mup[j] - mup[i - 1]) * g[n / i] * g[m / i];
        }
        printf("%lld\n", ans);
    }
    return 0;
}
```