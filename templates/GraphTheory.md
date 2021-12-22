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
int key[maxn];
bool vis[maxn];
int Prim()
{
    std::priority_queue<pii, std::vector<pii>, std::greater<pii> > q;
    memset(vis, 0, sizeof(vis));
    memset(key, 0x3f, sizeof(key));
    key[v[0]] = 0;
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
        res += key[now];
        vis[now] = true;
        nodeCnt ++;
        if(nodeCnt == n)
            break;
        for(int i = fst[now]; i != -1; i = nex[i])
        {
            if(w[i] < key[v[i]])
            {
                key[v[i]] = w[i];
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


## 矩阵树定理

一个图的生成树个数是其基尔霍夫矩阵任意主余子式的值。

基尔霍夫矩阵：
- $D_{ii}$: Degree(i)
- $D_{ij}$: i 到 j 的边数取负数

对于 n 阶矩阵，去掉任意的第 R 行 R 列，求剩下的 n-1 阶行列式的绝对值即最小生成树个数。

根据题目特点，可利用特殊数据推出行列式结果，数据范围允许时可按照行列式性质计算行列式。

## 网络流

### 最大流 Dinic

邻接矩阵版

```cpp
const int maxn = 2e2 + 10;  // 数据规模
const int inf = 0x3f3f3f3f; // 表示无穷大
int cap[maxn][maxn];    // 容量图，本代码没有记录可行流，直接减少容量边来代替表示流量的增加
int ly[maxn];           // 记录分层，即从源点起按广搜距离的分层，ly[i] 表示 i 到 源点的距离
int work[maxn];         // 辅助坐标，在计算增广线路时，每个结点已经遍历到的相邻结点编号，比如 work[i] = 5，表示 i 结点已经遍历到了 5 号结点，下次DFS到 i 的时候就从5开始。
int n, m, so, te;       // n结点数，m边数，so源点source，te汇点terminal
// 如果题目网络为浮点型，相应的DiDFS、Dinic返回值以及 cap 等相关变量也都改成double即可

bool DiBFS()
{
    // 广搜BFS做分层图，顺便返回是否能够从源点到达汇点。
    queue<int> q;
    memset(ly, -1, sizeof(ly));
    q.push(so);
    ly[so] = 0;
    while(!q.empty())
    {
        int now = q.front();
        q.pop();
        for(int nex = 1; nex <= te; nex ++)
        {
            if(ly[nex] > 0 ||!cap[now][nex]) continue;
            // 还有剩余容量，就计入辅助图，并同时利用BFS记录到源点的距离
            ly[nex] = ly[now] + 1;
            if(nex == te) return true;
            q.push(nex);
        }
    }
    // 没有到达汇点，说明已经达到最大流
    return false;
}
int DiDFS(int cur, int inc)
{
    // cur表示当前搜索结点，inc表示到cur为止，这条增广路径能增加的流量，即 min{路径上每一段可增加的流量}
    int tinc;
    if(cur == te)   // 递归终点：到达汇点
        return inc;
    // 这里 &i 用了引用，即枚举相邻结点时，work 辅助坐标跟着发生变化。
    for(int &i = work[cur]; i <= te; i ++)
    {
        // cur 到 i 有剩余容量，且在分层网络中 i 比 cur 距离源点远一部，且一直递归到终点的可增加流量不为零
        if(cap[cur][i] && ly[i] == ly[cur] + 1 && (tinc = DiDFS(i, min(inc, cap[cur][i]))))
        {
            // 对这条增广路径更新流量（本代码直接修改容量）
            cap[cur][i] -= tinc;
            cap[i][cur] += tinc;
            return tinc;
        }
    }
    return 0;
}

int Dinic()
{
    int ret = 0, tinc;
    while(DiBFS())  // 生成辅助网络（即保留所有不饱和反向边，也即能够增加正向流量的边）
    {
        // 清空辅助坐标，即DFS时候每个结点遍历相邻结点编号都从 0 开始
        memset(work, 0, sizeof(work));
        // 对于本次辅助网络，while找到所有的增广线路
        while(tinc = DiDFS(so, inf))
            ret += tinc;    // 最大流加上找到的增广流量
    }
    return ret;
}
```

前向星版

```cpp
// 建图 略...
const int maxn = 2e2 + 10;
const int maxm = 1e4 + 10;
const int inf = 0x3f3f3f3f;
int fst[maxn], nex[maxm], v[maxm], cap[maxm], tp; 
int ly[maxn], work[maxn], n, m, so, te;
bool DiBFS()
{
    queue<int> q;
    memset(ly, -1, sizeof(ly));
    q.push(so);
    ly[so] = 0;
    while(!q.empty())
    {
        int now = q.front();
        q.pop();
        for(int i = fst[now]; i != -1; i = nex[i])
        {
            if(ly[v[i]] >= 0 || !cap[i]) continue;
            ly[v[i]] = ly[now] + 1;
            if(v[i] == te) return true;
            q.push(v[i]);
        }
    }
    return false;
}
int DiDFS(int cur, int inc)
{
    int tinc;
    if(cur == te)
        return inc;
    for(int &i = work[cur]; i != -1; i = nex[i])
    {
        if(cap[i] && ly[v[i]] == ly[cur] + 1 && (tinc = DiDFS(v[i], min(inc, cap[i]))))
        {
            cap[i] -= tinc;
            cap[i ^ 1] += tinc;
            return tinc;
        }
    }
    return 0;
}

int Dinic()
{
    int ret = 0, tinc;
    while(DiBFS())
    {
        for(int i = 0; i <= n; i ++)
            work[i] = fst[i];
        while(tinc = DiDFS(so, inf))
            ret += tinc;
    }
    return ret;
}
```

### 最小费用最大流（MCMF）

#### Dinic版

```cpp
int fst[maxn], nex[maxm], v[maxm], cap[maxm], w[maxm], tp; 
int ly[maxn], work[maxn], n, m, so, te, maxflow, mincost;
bool inq[maxn];
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
bool DiSPFA()
{
    queue<int> q;
    memset(ly, 0x3f, sizeof(ly));
    q.push(so);
    ly[so] = 0;
    while(!q.empty())
    {
        int now = q.front();
        q.pop();
        inq[now] = false;
        for(int i = fst[now]; i != -1; i = nex[i])
        {
            if(ly[now] + w[i] >= ly[v[i]] || !cap[i]) continue;
            ly[v[i]] = ly[now] + w[i];
            if(!inq[v[i]]) q.push(v[i]), inq[v[i]] = true;
        }
    }
    return ly[te] != inf;
}
int DiDFS(int cur, int inc)
{
    int tinc;
    if(cur == te) return inc;
    inq[cur] = true;
    for(int &i = work[cur]; i != -1; i = nex[i])
    {
        if(inq[v[i]] || !cap[i] || ly[v[i]] != ly[cur] + w[i]) continue;
        if(!(tinc = DiDFS(v[i], min(inc, cap[i])))) continue;
        cap[i] -= tinc;
        cap[i ^ 1] += tinc;
        mincost += tinc * w[i];
        inq[cur] = false;
        return tinc;
    }
    inq[cur] = false;
    return 0;
}

void DinicMCMF()
{
    int tinc;
    while(DiSPFA())
    {
        memcpy(work, fst, sizeof(fst));
        while(tinc = DiDFS(so, inf))
            maxflow += tinc;
    }
}
```

#### Primal-Dual版

理论效率更高

```cpp
typedef pair<int, int> pii;
const int maxn = 5e3 + 10;
const int maxm = 1e5 + 10;
const int inf = 0x3f3f3f3f;
int fst[maxn], nex[maxm], v[maxm], cap[maxm], w[maxm], tp; 
int dis[maxn], h[maxn], work[maxn], n, m, so, te, maxflow, mincost;
int pv[maxn], pe[maxn];
bool vis[maxn];
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
```


### 二分图最大匹配

匈牙利算法

```cpp

bool g[maxn][maxn], chk[maxn];
int xM[maxn], yM[maxn];
int xN, yN, e;
bool SearchPath(int u)
{
    for(int v = 1; v <= yN; ++ v)
    {
        if(g[u][v] && !chk[v])
        {
            chk[v] = true;
            if(yM[v] == -1 || SearchPath(yM[v]))
            {
                yM[v] = u, xM[u] = v;
                return true;
            }    
        }
    }
    return false;
}
int Hungarian()
{
    memset(xM, -1, sizeof(xM));
    memset(yM, -1, sizeof(yM));
    int ret = 0;
    for(int u = 1; u <= xN; u ++)
    {
        if(xM[u] == -1)
        {
            memset(chk, false, sizeof(chk));
            ret += SearchPath(u);
        }
    }
    return ret;
}
```