## 最小生成树
### Prim
```cpp
typedef std::pair<int, int> pii;
const int MAXN = 510;
const int MAXM = 100010;
int first[MAXN], nex[MAXM], u[MAXM], v[MAXM], w[MAXM];
int tp, n;  // tp边数，n节点数
void AddEdge(int u_, int v_, int w_)
{
    u[tp] = u_;
    v[tp] = v_;
    w[tp] = w_;
    nex[tp] = first[u_];
    first[u_] = tp ++;
}
void DbEdge(int u_, int v_, int w_)
{
    AddEdge(u_, v_, w_);
    AddEdge(v_, u_, w_);
}
int Prim()
{
    int dis[MAXN];
    bool vis[MAXN];
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
        for(int i = first[now]; i != -1; i = nex[i])
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
int p[MAXN];
int wOrder[MAXM];
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