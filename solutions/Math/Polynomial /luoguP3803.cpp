// difficulty: 3
// 【模板】多项式乘法
// 该题 FFT 和 NTT 都可以做。这里练习NTT模板，使用原根为 3 的常用质数 998244353 作为模数。

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
const int maxn = 3e6 + 10;
const int mod = 998244353;
const int rog = 3;
int PowMod(int a, int n, int mod=mod)
{
    int ret = 1;
    for(; n; n >>= 1, a = 1LL * a * a % mod)
        if(n & 1) ret = 1LL * ret * a % mod;
    return ret;
}

void BitRevChange(int y[], int len) 
{
    // len should be 2^k
    std::vector<int> rev(len, 0);
    for (int i = 0; i < len; i ++) 
    {
        rev[i] = rev[i >> 1] >> 1;
        if (i & 1) rev[i] |= len >> 1;
    }
    for (int i = 0; i < len; ++i) 
        if (i < rev[i]) std::swap(y[i], y[rev[i]]);
    return;
}
void NTT(int y[], int len, int on=1)
{
    // on == 1: NTT; on == -1: INTT; len should be 2^k
    BitRevChange(y, len);
    for(int h = 2; h <= len; h <<= 1)
    {
        int wn = PowMod(rog, (mod - 1) / h);
        if(on == -1) wn = PowMod(wn, mod - 2);
        for(int j = 0; j < len; j += h)
        {
            int w = 1;
            for(int k = j; k < j + (h >> 1); k ++)
            {
                int u = y[k], t = 1LL * w * y[k + (h >> 1)] % mod;
                y[k] = (u + t) % mod;
                y[k + (h >> 1)] = (u - t + mod) % mod;
                w = 1LL * w * wn % mod;
            }
        }
    }
    if(on != -1) return;
    for(int i = 0, leninv = PowMod(len, mod - 2); i < len; i ++) 
        y[i] = 1LL * y[i] * leninv % mod;
}
int a[maxn], b[maxn];
int n, m;
int main()
{
    while(scanf("%d%d", &n, &m) != EOF)
    {
        for(int i = 0; i <= n; i ++) scanf("%d", &a[i]);
        for(int i = 0; i <= m; i ++) scanf("%d", &b[i]);
        int len = 1;
        for(; len <= n + m; len <<= 1);
        for(int i = n + 1; i < len; i ++) a[i] = 0;
        for(int i = m + 1; i < len; i ++) b[i] = 0;
        NTT(a, len); NTT(b, len);
        for(int i = 0; i < len; i ++) a[i] = 1LL * a[i] * b[i] % mod;
        NTT(a, len, -1);
        for(int i = 0; i <= n + m; i ++) printf("%d%c", a[i], " \n"[i == n + m]);
    }
    return 0;
}

