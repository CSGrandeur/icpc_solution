// difficulty: 5
// 抄板子
// 在掌握数论变换（NTT）的前提下，本题不算难。
// 但难就在于掌握NTT已经很难了，还不能通过预处理直接套模板，而要魔改模板。
// 题目给的 “n 是 p − 1 的约数”、“w^n mod p = 1”、“p是质数” 这三个条件，满足了以NTT解题的条件。
// 以下内容学完NTT后再阅读：
// f(w^i) = f0(w^2i) + w^i * f1(w^2i)
// f(w^(i+len/2)) = f0(w^2i) + w^(i+len/2) * f1(w^2i)
// 满足NTT执行条件，可 nlogn 完成 f(w^0) ~ f(w^(n-1)) 的计算。
// 本题一个难点在于 n = 3*2^k，而不是 2 的幂，有三种处理方式：
// 方式1（标程）：迭代过程按NTT两两计算，迭代终点分三份处理。
// 方式2：递归计算，len不整除2时必为3，余下的常数级直接计算。
// 方式3（本题解方法，最接近套模板）：
// 直接把系数按序号模3分3份，每份长度都是2的幂，分别套NTT；
// 最终结果为 f(w^i) = f0(w^3i) + w^i * f1(w^3i) + w^2i * f2(w&3i)。
// f0、f1、f2 长度都为 n / 3 而不是 n，但其实 i 的取值关于 n/3 循环（w^n % p == 1 关于n循环, w^(3 * i) % p 则关于 n/3 循环）
// 取 fx(w^(3 * i % (n / 3))) 即可。
// 这样实现的NTT还是有两个地方与模板不同要魔改：
// 第一处：`int wn = PowMod(rog, len / h, mod);` 
// 这里 rog 是 w^3，并不一定是原根，指数也不是 (mod - 1) / h 而是 len / h，因为我们要计算f(w^0) ~ f(w^(len - 1))
// 第二处：`y[k + (h >> 1)] = (u + 1LL * t * PowMod(wn, h >> 1, mod)) % mod;` 
// 这里不再是 `(u - t + mod) % mod`，因为并不是基于原根进行的计算，并不符合类似FFT的 w^(k+n)=-w(k)，要按递推公式把 w^(len/2) 代进去。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
const int maxn = 2e5 + 10;
int n, p, w;
int a[3][maxn];

int PowMod(int a, int n, int mod)
{
    int ret = 1;
    for(; n; n >>= 1, a = 1LL * a * a % mod)
        if(n & 1) ret = 1LL * ret * a % mod;
    return ret;
}
void BitRevChange(int y[], int len) 
{
    std::vector<int> rev(len, 0);
    for (int i = 0; i < len; i ++) 
    {
        rev[i] = rev[i >> 1] >> 1;
        if (i & 1) rev[i] |= len >> 1;
    }
    for (int i = 0; i < len; ++i) 
        if (i < rev[i]) std::swap(y[i], y[rev[i]]);
}
void NTT(int y[], int len, int rog, int mod)
{
    BitRevChange(y, len);
    for(int h = 2; h <= len; h <<= 1)
    {
        int wn = PowMod(rog, len / h, mod);
        for(int j = 0; j < len; j += h)
        {
            int w = 1, wn2len = PowMod(wn, h >> 1, mod);
            for(int k = j; k < j + (h >> 1); k ++)
            {
                int u = y[k], t = 1LL * w * y[k + (h >> 1)] % mod;
                y[k] = (u + t) % mod;
                y[k + (h >> 1)] = (u + 1LL * t * wn2len) % mod;
                w = 1LL * w * wn % mod;
            }
        }
    }
}
int main()
{
    while(scanf("%d%d%d", &n, &p, &w) != EOF)
    {
        for(int i = 0; i < n; i ++)
            scanf("%d", &a[i % 3][i / 3]);
        int m = n / 3;
        for(int i = 0; i < 3; i ++)
            NTT(a[i], m, 1LL * w * w % p * w % p, p);
        for(int i = 0, wi = 1; i < n; i ++, wi = 1LL * wi * w % p)
        {
            int res = 0, tmpw = 1;
            for(int j = 0; j < 3; j ++, tmpw = 1LL * tmpw * wi % p)
                res = (res + 1LL * a[j][i % m] * tmpw % p) % p;
            printf("%d%c", res, " \n"[i == n - 1]);
        }
    }
    return 0;
}