// difficulty: 3
// Mondriaan's Dream
// 插头DP入门0：骨牌覆盖
// 利用插头DP的思想，但不是严格意义上的插头。
// 网上的博客五花八门，各有各的解释，dp思路不尽相同，却又都不具体解释，怎么看怎么懵。
// dp顺序为从上到下、从左到右，每次往右移动一格，一行到头就换行。由“这一格”now，计算“下一格”nex。
// dp[now][k] 表示now 计算后轮廓线覆盖状态为 k 的情况数，k 的第 j 个二进制位为 1 表示轮廓线第 j 块不为空，否则为空。
// 计算 nex 时，考虑nex格（横向第 j 个）上面（up=1<<j）、左边（left=1<<j-1）是否为空格的情况。
// 对于 now 的状态 k ：
// 1. 如果 now 的 up 为空，则必从nex格放一个朝上的块，状态由 dp[now][k] 转移。
// 2. 如果 now 的 up 不为空，left 为空，则可以什么都不放，让 nex 的 up 为空，状态由dp[now][k] 转移。
// 3. 如果 now 的 up 和 left 都不为空，则什么都做不了，让 nex 的 up 为空，状态由dp[now][k] 转移。
// 发现以上 3 种情况表达出的转移方程相同，都是 dp[nex][k ^ up] += dp[now][k];
// 4. 如果 now 的 up 不为空，left 为空，则可以在 nex 放一个朝左的块，这样 nex 的left不为空，dp[nex][k ^ left] += dp[now][k];
// 处理第一行时，无论怎样都不能放朝上的块，所以初始状态为dp[0][(1 << w) - 1] = 1; 即假设“不存在的前一行”全部填满，是初始的 1 种情况。
// 全部处理完时，输出最后一格状态为整个轮廓线都填满的情况数 dp[now][(1 << w) - 1]。

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int maxn = 12;
int h, w;
long long dp[2][1 << maxn | 1];
int main()
{
    while(scanf("%d%d", &h, &w) != EOF && (h || w))
    {
        memset(dp, 0, sizeof(dp));
        int now = 0, nex = 1;
        dp[0][(1 << w) - 1] = 1;
        for(int i = 0; i < h; i ++)
        {
            for(int j = 0; j < w; j ++)
            {
                memset(dp[nex], 0, sizeof(dp[nex]));
                int up = 1 << j, left = 1 << (j - 1);
                for(int k = 0; k < 1 << w; k ++)
                {
                    if(j && (k & up) && !(k & left))
                        dp[nex][k ^ left] += dp[now][k];
                    dp[nex][k ^ up] += dp[now][k];
                }
                now = now + 1 & 1, nex = now ^ 1;
            }
        }
        printf("%lld\n", dp[now][(1 << w) - 1]);
    }
    return 0;
}