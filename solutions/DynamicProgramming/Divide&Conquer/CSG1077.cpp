// difficulty: 3
// 2019
// 求树上和为2019的倍数的路径个数
// 随便取一结点为根，递归处理子树到达当前结点的和模2019为0~2019的路径个数的统计信息
// 用子结点的这个统计信息，更新经过当前结点的模2019为0的路径个数，并更新当前结点的统计信息。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<queue>
const int maxn = 2e4 + 10;
const int maxm = maxn << 1 | 1;
const int mod = 2019;
int fst[maxn], nex[maxm], u[maxm], v[maxm], w[maxm];
int n, tp;
int cnt[maxn][mod + 1], tmpcnt[mod + 1];
bool vis[maxn];
int ans;
void AddEdge(int u_, int v_, int w_)
{
    u[tp] = u_;
    v[tp] = v_;
    w[tp] = w_;
    nex[tp] = fst[u_];
    fst[u_] = tp ++;
}

void DbEdge(int u_, int v_, int w_)
{
    AddEdge(u_, v_, w_);
    AddEdge(v_, u_, w_);
}
void DPS(int now)
{
	vis[now] = true;
	for(int i = fst[now]; i != -1; i = nex[i])
	{
		if(vis[v[i]]) continue;
		DPS(v[i]);
		for(int j = 0; j < mod; j ++)
			tmpcnt[j] = cnt[v[i]][((j - w[i]) % mod + mod) % mod];
		for(int j = 0; j < mod; j ++)
			ans += tmpcnt[j] * cnt[now][(mod - j) % mod];
		for(int j = 0; j < mod; j ++)
			cnt[now][j] += tmpcnt[j];
	}
}
int main()
{
	int tu, tv, tw;
	while(scanf("%d", &n) != EOF)
	{
		memset(vis, 0, sizeof(vis));
		memset(cnt, 0, sizeof(cnt[0]) * (n + 1));
		memset(fst, -1, sizeof(fst));
		tp = 0;
		ans = 0;
		for(int i = 1; i <= n; i ++)
			cnt[i][0] = 1;
		for(int i = 1; i < n; i ++)
		{
			scanf("%d%d%d", &tu, &tv, &tw);
			DbEdge(tu, tv, tw);
		}
		DPS(1);
		printf("%d\n", ans);
	}
	return 0;
}