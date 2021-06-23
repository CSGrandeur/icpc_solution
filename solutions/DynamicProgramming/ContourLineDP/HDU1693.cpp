// difficulty: 3
// Eat the Trees
// 插头DP入门题，对不可行结点，无插头直接状态转移，有插头则状态归0。
// 对可行结点，双有插头转移至双无插头，双无插头转移至双有插头，一有一无转移至同向、转向的一有一无。
// 其中转向的一有一无与前两种可合并为：`dp[nex][k] = dp[now][k ^ left ^ up];`，同向一有一无额外增加转移：`dp[nex][k] += dp[now][k];`
// 每行末尾转下一行首时，编号最大的轮廓（竖着的）去掉，增加竖着的编号最小的轮廓（必然没有从左到右的插头），所有其他轮廓编号+1：
// dp[nex][k << 1] = dp[now][k]; dp[nex][k << 1 | 1] = 0;
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int maxn = 12;
int n, m;
long long dp[2][1 << maxn | 1];
int main()
{
    int cs = 1, t, blk;
    for(scanf("%d", &t); cs <= t; cs ++)
    {
        scanf("%d%d", &n, &m);
        memset(dp, 0, sizeof(dp));
        int now = 0, nex = 1;
        dp[0][0] = 1;
        for(int i = 0; i < n; i ++)
        {
            for(int k = 0; k < 1 << m; k ++)
            {
                dp[nex][k << 1] = dp[now][k];
                dp[nex][k << 1 | 1] = 0;
            }
            now ^= 1, nex ^= 1;
            for(int j = 0; j < m; j ++)
            {
                scanf("%d", &blk);
                int left = 1 << j, up = left << 1;
                for(int k = 0; k < 1 << m + 1; k ++)
                {
                    if(blk)
                    {
                        dp[nex][k] = dp[now][k ^ left ^ up];
                        if(!(k & left) + !(k & up) == 1)
                            dp[nex][k] += dp[now][k];
                    }
                    else if(!(k & left) && !(k & up)) 
                        dp[nex][k] = dp[now][k];
                    else dp[nex][k] = 0;
                }
                now ^= 1, nex ^= 1;
            }
        }
        printf("Case %d: There are %lld ways to eat the trees.\n", cs, dp[now][0]);
    }
    return 0;
}