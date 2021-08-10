// Moamen and XOR
// 从高位到低位，迭代前缀相等的情况数，和当前位左式大于右式的情况数，累加。
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int mod = 1e9 + 7;
int n, k, t;
int PowMod(int a, int n, int mod)
{
    int ret = 1;
    for(; n; n >>= 1, a = 1LL * a * a % mod)
        if(n & 1) ret = 1LL * ret * a % mod;
    return ret;
}
int main()
{
    for(scanf("%d", &t); t --; )
    {
        scanf("%d%d", &n, &k);
        int ans = 0, tmp = 1;
        int n_1 = PowMod(2, n - 1, mod);
        for(int i = k - 1; i >= 0; i --)
        {
            if(n & 1)
            {
                tmp = 1LL * tmp * (n_1 + 1) % mod;
            }
            else
            {
                ans = (ans + 1LL * PowMod(PowMod(2, i, mod), n, mod) * tmp % mod) % mod;
                tmp = 1LL * (n_1 + mod - 1) % mod * tmp % mod;
            }
        }
        printf("%d\n", (ans + tmp) % mod);
    }
    return 0;
}