// difficulty: 4
// Chip Move
// 从 `0` 出发，第 `i` 步要走 `k+i-1` 的正倍数距离，求`1~n`每个位置的方法数
// 分析得知至多 $sqrt(2n)$ 步能到达，DP为第 j 步到达 i 的方法数
// 空间优化：滚动数组
// 效率优化：步幅为 `k` 到达 `j`，则所有 `j % k` 同余的位置都能直接转移，故滚动数组中统计该值
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<queue>
#include<map>
#include<algorithm>
const int mod = 998244353;
const int maxn = 2e5 + 10;
int n, k;
int dp[maxn], sum[maxn], ans[maxn];
int main()
{
    while(scanf("%d%d", &n, &k) != EOF)
    {
        memset(ans, 0, sizeof(ans));
        memset(sum, 0, sizeof(sum));
        dp[0] = 1;
        for(int total = k; total <= n; total += ++ k)
        {
            for(int j = total - k; j < total; j ++)
                sum[j % k] = dp[j];             // `k`的步幅出发点至少是`total`位置，`dp[j]`是`k-1`的步幅到达所有`j % k`方案总和
            for(int j = total; j <= n; j ++)
            {
                int tmp = dp[j];
                dp[j] = sum[j % k];             // `k-1` 的步幅能到达的所有 `j % k` 的位置出发，用 `k` 的步幅可一步到 `j`，以此状态转移
                (sum[j % k] += tmp) %= mod;     // `k-1` 的步幅到达 `j` 的方法数累加到统计中，用于计算`k`的步幅到达的后续位置
                (ans[j] += dp[j]) %= mod;       // 最后一步为`k`的步幅到达`j`的方法数更新到达`j`的总方法数
            }
        }
        for(int i = 1; i <= n; i ++)
            printf(" %d" + (i == 1), ans[i]);
        printf("\n");
    }
    return 0;
}