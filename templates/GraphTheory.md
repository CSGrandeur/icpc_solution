# 图论

## 前向星建图

```cpp
const int maxn = 510;
const int maxm = 100010;
int fst[maxn], nex[maxm], u[maxm], v[maxm], w[maxm];
int tp, n, m;  // tp边数，n结点数, m边数
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
```

## 最小生成树

### Prim

```cpp
typedef std::pair<int, int> pii;
int Prim()
{
    int dis[maxn];
    bool vis[maxn];
    std::priority_queue<pii, std::vector<pii>, std::greater<pii> > q;
    memset(vis, 0, sizeof(vis));
    memset(dis, 0x3f, sizeof(dis));
    dis[v[0]] = 0;
    int res = 0;
    int nodeCnt = 0;
    q.push(pii(0, 0));
    while(!q.empty())
    {
        int nowEdge = q.top().second;
        int now = v[nowEdge];
        q.pop();
        if(vis[now]) 
            continue;
        res += dis[now];
        vis[now] = true;
        nodeCnt ++;
        if(nodeCnt == n)
            break;
        for(int i = fst[now]; i != -1; i = nex[i])
        {
            if(w[i] < dis[v[i]])
            {
                dis[v[i]] = w[i];
                q.push(pii(w[i], i));
            }
        }
    }
    return res;
}
```

### Kruskal

```cpp
int p[maxn];
int wOrder[maxm];
inline bool comp(const int &a, const int &b)
{return w[a] < w[b];}
void OrderW()
{
    for(int i = 0; i < tp; i ++)
        wOrder[i] = i;
    std::sort(wOrder, wOrder + tp, comp);
}
int fa(int i)
{return p[i] == i ? i : p[i] = fa(p[i]);}
int Kruskal()
{
    int res = 0, edgeNum = 0;
    for(int i = 1; i <= n; i ++)
        p[i] = i;
    OrderW();
    for(int i = 0; i < tp; i ++)
    {
        int len = w[i];
        if(fa(u[i]) != fa(v[i]))
        {
            res += len;
            p[fa(u[i])] = fa(v[i]);
            edgeNum ++;
            if(edgeNum == n - 1)
                break;
        }
    }
    return edgeNum == n - 1 ? res : 0x3f3f3f3f;
}
```

## 最短路

### Floyd-Warshall

```cpp
int g[maxn][maxn];
void Floyd(int n)
{// 结点范围[1, n]
    for(int k = 1; k <= n; k ++)
        for(int i = 1; i <= n; i ++)
            for(int j = 1; j <= n; j ++)
                g[i][j] = min(g[i][k] + g[k][j], g[i][j]);
}
```
### Dijkstra

```cpp
#include<queue>
#include<algorithm>
using namespace std;
typedef pair<int, int> pii;
const int INF = 0x3f3f3f3f;
int dis[maxn];
int Dijkstra(int start, int end)
{
    memset(dis, 0x3f, sizeof(dis));
    dis[start] = 0;
    priority_queue<pii, vector<pii>, greater<pii> > q;
    q.push(pii(0, start));
    while(!q.empty())
    {
        pii now = q.top();
        q.pop();
        if(now.first != dis[now.second]) continue;
        if(now.second == end) return now.first;
        for(int i = fst[now.second]; i != -1; i = nex[i])
        {
            if(dis[now.second] + w[i] < dis[v[i]])
            {
                dis[v[i]] = dis[now.second] + w[i];
                q.push(pii(dis[v[i]], v[i]));
            }
        }
    }
    return INF;
}
```

## 拓扑排序

邻接矩阵，基于入度

```cpp
int g[maxn][maxn];
int ind[maxn];
int res[maxn], rtp;
void TopoSort()
{
    int i, j, k;
    rtp = 0;
    for(i = 0; i < n; i ++) ind[i] = 0;
    for(i = 0; i < n; i ++) 
        for(j = 0; j < n; j ++)
            ind[j] += g[i][j];
    for(i = 0; i < n; i ++)
    {
        for(j = 0; j < n && ind[j]; j ++);
        res[rtp ++] = j;
        ind[j] = -1;
        for(k = 0; k < n; k ++)
            ind[k] -= g[j][k];
    }
}
```