// difficulty: 4
// 字典序
// 求一个字典序最小的编号序列对矩阵列向量重排序，使得排序后的行向量字典序非减
// 构造方式：对于任意 C[i - 1][k] > C[i][k] 的 行 i 与列 k，都至少存在一个 C[i - 1][j] < C[i][j] 的 j 排在 k 前面
// 可以对每个 C[i - 1][k] > C[i][k] 的情况，增加第 k 列的入度
// 选最小的入度为 0 的列 ith 放入答案序列，对所有 C[i - 1][ith] < C[i][ith] 的 i，减少 C[i - 1][k] > C[i][k] 的 k 的度。
// 特别地，这样的 i 做标记，每个 i 只统计了一次入度，所以也只减少一次入度
// 每轮对更新度之后的最小入度为0的列放入答案序列，重复进行，直到无解或得到解
// 因为每个行 i 的入度减小操作只会执行一次，所以复杂度为 O(nm)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const int maxn = 2100;
int c[maxn][maxn];
int n, m;
int res[maxn];
int ind[maxn];
bool vis[maxn], visind[maxn];
int main()
{
	while(scanf("%d%d", &n, &m) != EOF)
	{
		memset(ind, 0, sizeof(ind));
		memset(vis, 0, sizeof(vis));
		memset(visind, 0, sizeof(visind));
		for(int i = 0; i < n; i ++)
			for(int j = 0; j < m; j ++)
			{
				scanf("%d", &c[i][j]);
				ind[j] += i && c[i - 1][j] > c[i][j];
			}
		int rtp = 0, ith;
		for(; rtp < m; rtp ++)
		{
			for(ith = 0; ith < m && (vis[ith] || ind[ith]); ith ++);
			if(ith == m) break;
			res[rtp] = ith;
			vis[ith] = true;
			for(int i = 1; i < n; i ++)
			{
				if(c[i - 1][ith] >= c[i][ith] || visind[i]) continue;
				visind[i] = true;
				for(int j = 0; j < m; j ++)
					ind[j] -= ind[j] && c[i - 1][j] > c[i][j];
			}
		}
		if(rtp < m) printf("-1");
		else
		{
			for(int i = 0; i < rtp; i ++)
				printf(" %d" + !i, res[i] + 1);
		}
		printf("\n");
	}
	return 0;
}