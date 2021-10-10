// difficulty: 2
// 中国剩余定理基础题，这里测一下模板。
#include<stdio.h>
#include<stdlib.h>
typedef long long LL;
LL ExGCD(LL a, LL b, LL &x, LL &y)
{
    if(!b)
    {
        x = 1, y = 0;
        return a;
    }
    LL d = ExGCD(b, a % b, x, y), t = x;
    x = y, y = t - a / b * x;
    return d; 
}
int ExGcdInv(int a, int mod)
{
    LL x, y;
    ExGCD(a, mod, x, y);
    return (x % mod + mod) % mod;
}
LL CRT(int n, LL a[], LL b[]) 
{
    // x ≡ a[i] (mod b[i])
    LL bm = 1, ans = 0;
    for (int i = 0; i < n; i++) bm *= b[i];
    for (int i = 0; i < n; i++) 
    {
        LL mi = bm / b[i];
        ans = (ans + a[i] * mi * ExGcdInv(mi, b[i])) % bm;
    }
    return ans == 0 ? bm : ans;
}
LL a[5], b[5];
int main()
{
    int p, e, i, d, t = 0;
    b[0] = 23, b[1] = 28, b[2] = 33;
    while(scanf("%d%d%d%d", &p, &e, &i, &d) && p != -1)
    {
        a[0] = p % b[0], a[1] = e % b[1], a[2] = i % b[2];
        LL ans = CRT(3, a, b), bm = b[0] * b[1] * b[2];
        printf("Case %d: the next triple peak occurs in %lld days.\n", ++ t, (ans - 1 - d + bm) % bm + 1);
    }
    return 0;
}