// difficulty: 4
// Modulo Nine
// 多年没做刷表的题，突然遇到就完全没思路了。
// 0、3、6、9 可以组成满足要求的序列，0、9单个就能满足，3、6需要两个。
// dp[i][j][k] 表示 i 前缀满足要求情况下的最后两个使要求满足的数的位置为 j、k 的情况个数。
// 用 left[i] 表示以 i 为右端的区间，要求满足题目要求的左端点。
// 题目没要求的 i 的left[i]为0；有要求的 i，left[i]取最大的左端点。
// 用 dp[i][j][k] 刷 dp[i + 1] 的表，且只刷 j、k >= left[i + 1] 的情况
// 刷表使每一步都做了过滤，保证刷到 dp[i][j][k] 时，是 i 前缀都满足题目要求的解的个数。
// 刷表数组可复用，开二维 dp[j][k] 即可。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
const int maxn = 55;
const int mod = 1e9 + 7;
int n, m;
int left[maxn];
int dp[maxn][maxn];

int main()
{
	while(scanf("%d%d", &n, &m) != EOF)
	{
		memset(dp, 0, sizeof(dp));
		memset(left, 0, sizeof(left));
		dp[0][0] = 1;
		for(int i = 0, l, r; i < m; i ++)
		{
			scanf("%d%d", &l, &r);
			left[r] = std::max(l, left[r]);
		}
		for(int i = 1; i <= n; i ++)
		{
			for(int j = 0; j < i; j ++)
			{
				for(int k = 0; k <= j; k ++)
				{
					dp[i][i] = (dp[i][i] + 2LL * dp[j][k]) % mod;
					dp[i][j] = j < left[i] ? 0 : (dp[i][j] + 2LL * dp[j][k]) % mod;
					dp[j][k] = k < left[i] ? 0 : 6LL * dp[j][k] % mod;
				}
			}
		}
		int ans = 0;
		for(int i = left[n]; i <= n; i ++)
			for(int j = left[n]; j <= i; j ++)
				ans = (ans + dp[i][j]) % mod;
		printf("%d\n", ans);
	}
	return 0;
}