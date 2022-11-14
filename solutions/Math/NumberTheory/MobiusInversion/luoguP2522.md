### `[HAOI2011]Problem b`

> difficulty: 3

虽然是莫比乌斯反演入门题，但即使学过莫反，第一次面对这样的问题仍然很难入手。

1. `Ans(a, b)` 区间问题转换为容斥后的 `Ans((1,b),(1,d))−Ans((1,b),(1,c−1))−Ans((1,a−1),(1,d))+Ans((1,a−1),(1,c−1))`

2. 莫反转换为求 $\lfloor \frac{a}{n} \rfloor \lfloor \frac{b}{n} \rfloor$ 问题

```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<vector>
const int maxn = 5e4 + 10;
int t, a, b, c, d, k;
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
    for(int i = 2; i <= mxn; i ++) mup[i] = mup[i - 1] + mu[i];
}
int Calc(int a, int b)
{
    int ans = 0, miab = std::min(a, b);
    for(int i = 1, j; i <= miab; i = j + 1)
    {
        j = std::min(a / (a / i), b / (b / i));
        ans += (mup[j] - mup[i - 1]) * (a / i) * (b / i);
    }
    return ans;
}
int main()
{
    MuList(maxn);
    for(scanf("%d", &t); t --; )
    {
        scanf("%d%d%d%d%d", &a, &b, &c, &d, &k);
        printf("%d\n", 
            Calc(b / k, d / k) - Calc(b / k, (c - 1) / k) - 
            Calc((a - 1) / k, d / k) + Calc((a - 1) / k, (c - 1) / k)
        );
    }
    return 0;
}
```