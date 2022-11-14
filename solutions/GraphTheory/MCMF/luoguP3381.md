### `【模板】最小费用最大流 题解`

> difficulty: 3

Primal-Dual 原始对偶算法，设置势能 h 数组把边权转化为非负，用Dijkstra增广

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
bool PdDij()
{
    priority_queue<pii, vector<pii>, greater<pii> > q;
    for(int i = 0; i <= n; i ++) dis[i] = inf;
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

void PdMCMF()
{   // Primal-Dual
    PdSPFA();
    while(PdDij())
    {
        int mf = inf;
        for(int i = 0; i <= n; i ++) h[i] += dis[i];
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
int main()
{
    int a, b, cp, wt;
    while(scanf("%d%d%d%d", &n, &m, &so, &te) != EOF)
    {
        memset(fst, -1, sizeof(fst));
        memset(vis, 0, sizeof(vis));
        maxflow = mincost = 0;
        tp = 0;
        for(int i = 0; i < m; i ++)
        {
            scanf("%d%d%d%d", &a, &b, &cp, &wt);
            DbEdge(a, b, cp, wt);
        }
        PdMCMF();
        printf("%d %d\n", maxflow, mincost);

    }
    return 0;
}
```

