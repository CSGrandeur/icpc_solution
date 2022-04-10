// difficulty: 2
// Array Balancing
// 类似01背包的基础DP
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<vector>
const int maxn = 55;
typedef long long LL;
int n;
int a[maxn], b[maxn];
LL dp[2][maxn];
int main()
{
    int t;
    for(scanf("%d", &t); t --; )
    {
        memset(dp, 0x3f, sizeof(dp));
        dp[0][0] = dp[0][1] = dp[1][0] = dp[1][1] = 0;
        scanf("%d", &n);
        for(int i = 0; i < n; i ++) scanf("%d", &a[i]);
        for(int i = 0; i < n; i ++) scanf("%d", &b[i]);
        for(int i = 1; i < n; i ++)
        {
            dp[0][i] = std::min(dp[0][i - 1] + abs(a[i - 1] - a[i]) + abs(b[i - 1] - b[i]), dp[1][i - 1] + abs(b[i - 1] - a[i]) + abs(a[i - 1] - b[i]));
            dp[1][i] = std::min(dp[0][i - 1] + abs(a[i - 1] - b[i]) + abs(b[i - 1] - a[i]), dp[1][i - 1] + abs(b[i - 1] - b[i]) + abs(a[i - 1] - a[i]));
        }
        printf("%lld\n", std::min(dp[0][n - 1], dp[1][n - 1]));        
    }
    return 0;
}