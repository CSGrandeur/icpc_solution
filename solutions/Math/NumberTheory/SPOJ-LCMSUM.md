### LCM Sum

转换成最大公约数求和，从而变为枚举`n`的约数的问题.

$
\sum_{i}^{n} LCM(i,n) \\
=\sum_{i}^{n} \frac{i*n}{GCD(i,n)} \\
=n\sum_{d|n} \sum_{i}^{n} \frac{i}{d}[GCD(i,n)=d] \\
=n\sum_{d|n} \sum_{j=1}^{n/d}j[GCD(j,n/d)=1] \\
=n\sum_{d|n} \phi(\frac{n}{d})\frac{n}{2d}
$ 

$\sum_{j=1}^{n/d}j[GCD(j,n/d)=1]$ 在求 $n/d$ 内与 $n/d$ 互质的数的和.

对于“求n以内与n互质的数的和”的问题，利用结论： `gcd(n, m) == gcd(n, n - m)`，即与`n`互质的数成对出现，每一对两数之和为`n`. `elp[n] = el[n] * n / 2`，注意 `n==1` 时需特判.

```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<vector>
#include<cmath>
const int maxn = 1e6 + 10;
int t, n;
std::vector<int> el;
void EulerList(int mxn)
{
    el.resize(mxn + 10);
    el[1] = 1;
    for(int i = 2; i <= mxn; i ++)
        el[i] = i;
    for(int i = 2; i <= mxn; i ++)
    {
        if(el[i] != i) continue;
        for(int j = i; j <= mxn; j += i)
            el[j] = el[j] / i * (i - 1);
    }
}
inline long long Elp(int x){return 1LL * el[x] * x / 2 + (x == 1);}
int main()
{
    EulerList(maxn);
    for(scanf("%d", &t); t --; )
    {
        scanf("%d", &n);
        long long res = 0;
        for(int i = sqrt(n); i; i --)
        {
            if(n % i) continue;
            res += Elp(n / i);
            if(i * i != n) res += Elp(i);
        }
        printf("%lld\n", res * n);
    }
    return 0;
}

```