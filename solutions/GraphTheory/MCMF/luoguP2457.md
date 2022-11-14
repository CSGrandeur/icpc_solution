### `[SDOI2006]仓库管理员的烦恼`

> difficulty: 3

每个物品编号为 1~n，每个仓库编号为 n+1~2n，源点 0，汇点 2n+1

源点向每个物品建容量为 1，费用为 0 的边，每个仓库向汇点建相同的边

每个物品向每个仓库建容量为 1，费用为所有该物品移到该仓库的代价 的边

最小费用最大流

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<queue>
using namespace std;
typedef pair<int, int> pii;
const int maxn = 5e3 + 10;
const int maxm = 1e5 + 10;
const int inf = 0x3f3f3f3f;
int fst[maxn], nex[maxm], v[maxm], cap[maxm], w[maxm], tp; 
int dis[maxn], h[maxn], work[maxn], n, m, so, te, maxflow, mincost;
int pv[maxn], pe[maxn];
bool vis[maxn];
void AddEdge(int s, int e, int cp, int wt)
{
    v[tp] = e;
    cap[tp] = cp;
    w[tp] = wt;
    nex[tp] = fst[s];
    fst[s] = tp ++;
}
void DbEdge(int s, int e, int cp, int wt)
{
    AddEdge(s, e, cp, wt);
    AddEdge(e, s, 0, -wt);
}
void PdSPFA()
{
    queue<int> q;
    memset(h, 0x3f, sizeof(h));
    h[so] = 0, vis[so] = true;
    q.push(so);
    while(!q.empty())
    {
        int now = q.front();
        q.pop();
        vis[now] = false;
        for(int i = fst[now]; i != -1; i = nex[i])
        {
            if(h[now] + w[i] >= h[v[i]] || !cap[i]) continue;
            h[v[i]] = h[now] + w[i];
            if(!vis[v[i]]) q.push(v[i]), vis[v[i]] = true;
        }
    }
}
bool PdDij(int ndnum)
{
    priority_queue<pii, vector<pii>, greater<pii> > q;
    for(int i = 0; i <= ndnum; i ++) dis[i] = inf;
    memset(vis, 0, sizeof(vis));
    dis[so] = 0;
    q.push(pii(0, so));
    while(!q.empty())
    {
        int now = q.top().second;
        q.pop();
        if(vis[now]) continue;
        vis[now] = true;
        for(int i = fst[now]; i != -1; i = nex[i])
        {
            int nh = w[i] + h[now] - h[v[i]];
            if(dis[now] + nh >= dis[v[i]] || !cap[i]) continue;
            dis[v[i]] = dis[now] + nh;
            pv[v[i]] = now;
            pe[v[i]] = i;
            if(!vis[v[i]]) q.push(pii(dis[v[i]], v[i]));
        }
    }
    return dis[te] != inf;
}

void PdMCMF(int ndnum)
{   // Primal-Dual
    PdSPFA();
    while(PdDij(ndnum))
    {
        int mf = inf;
        for(int i = 0; i <= ndnum; i ++) h[i] += dis[i];
        for(int i = te; i != so; i = pv[i]) mf = min(mf, cap[pe[i]]);
        for(int i = te; i != so; i = pv[i])
        {
            cap[pe[i]] -= mf;
            cap[pe[i] ^ 1] += mf;
        }
        maxflow += mf;
        mincost += mf * h[te];
    }
}
int g[200][200], sum[200];
int main()
{
    int cp;
    scanf("%d", &n);
    {
        so = 0;
        te = n << 1 | 1;
        memset(fst, -1, sizeof(fst));
        memset(vis, 0, sizeof(vis));
        memset(g, 0, sizeof(g));
        memset(sum, 0, sizeof(sum));
        maxflow = mincost = 0;
        tp = 0;
        for(int i = 1; i <= n; i ++)
            for(int j = 1; j <= n; j ++)
                scanf("%d", &g[i][j]), sum[j] += g[i][j];
        for(int i = 1; i <= n; i ++)
        {
            DbEdge(so, i, 1, 0);
            DbEdge(n + i, te, 1, 0);
            for(int j = 1; j <= n; j ++)
                DbEdge(i, n + j, 1, sum[i] - g[j][i]);
        }
        PdMCMF(te);
        printf("%d\n", mincost);

    }
    return 0;
}
```

