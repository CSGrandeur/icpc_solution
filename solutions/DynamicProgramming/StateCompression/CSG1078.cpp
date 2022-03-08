// difficulty: 5
// Parity of Tuples (Easy)
// 每个 vi 能给总结果带来的贡献相互独立，可以分别计算
// 对一个特定 vi ，计算对结果的贡献，可以用状态压缩 DP 刷表。
// 以下考虑对特定 vi 的 a[1] ~ a[m] 计算贡献：
// 一个状态码 mask 的第 i 位为 1 表示 a[i] 的二进制位 1 的个数位奇数，反之偶数，这种刷表是假设状态，不符合a[]真实状态的 dp[i][j] 会是 0，故而能够得到正确的结果。
// dp[i][j] 表示考察到第 i 个二进制位、状态码为 j 对最终结果的贡献。
// amask 为所有 a[] 的第 i 个二进制位为 1 的状态压缩，即 (amask >> t) & 1 为 1 表示 a[t] 的第 i 个二进制位为1。
// 状态转移为
// x 第 i 位取 0 时， dp[i][j] = dp[i - 1][j];
// x 第 i 位取 1 时， dp[i][j ^ mask] = dp[i - 1][j] * 3^(2^(i+1))
// 特别地，这里 "3^(2^(i+1))" 是因为x拆分处理，乘3的幂也拆分处理。
// count(x)*3^x 被拆分为：
// count(x) * 3^(x0*2^0 + x1*2^1 +  ...)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<list>
const int maxn = 1e4 + 10;
const int mod = 1e9 + 7;
int n, m, k;
int a[11], dp[35][1 << 11 | 1];
int b3[35];
void ModAdd(int &x, int y){ (x += y) %= mod;}
int Calc()
{
    for(int i = 0; i <= k; i ++) memset(dp[i], 0, sizeof(int) * (1 << m | 1));
    dp[0][0] = 1;
    for(int i = 0; i < k; i ++)
    {
        int amask = 0;
        for(int j = 0; j < m; j ++)
            amask |= (a[j] >> i & 1) << j;
        for(int j = (1 << m) - 1; j >= 0; j --)
        {
            ModAdd(dp[i + 1][j], dp[i][j]);
            ModAdd(dp[i + 1][j ^ amask], (long long)dp[i][j] * b3[i] % mod);
        }
    }
    return dp[k][(1 << m) - 1];
}
int main()
{
    b3[0] = 3;
    for(int i = 1; i < 32; i ++) b3[i] = (long long)b3[i - 1] * b3[i - 1] % mod;
    while(scanf("%d%d%d", &n, &m, &k) != EOF)
    {
        int ans = 0;
        for(int i = 0; i < n; i ++)
        {
            for(int j = 0; j < m; j ++)
                scanf("%d", &a[j]);
            ModAdd(ans, Calc());
        }
        printf("%d\n", ans);
    }
    return 0;
}