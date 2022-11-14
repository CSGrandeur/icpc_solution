### `【模板】网络最大流`

> difficulty: 2

标准模板

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<queue>
using namespace std;
const int maxn = 2e2 + 10;
const int maxm = 1e4 + 10;
const int inf = 0x3f3f3f3f;
int fst[maxn], nex[maxm], v[maxm], tp; 
long long cap[maxm];
int ly[maxn], work[maxn], n, m, so, te;
void AddEdge(int s, int e, int cp)
{
    v[tp] = e;
    cap[tp] = cp;
    nex[tp] = fst[s];
    fst[s] = tp ++;
}
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
int DiDFS(int cur, long long inc)
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

long long Dinic()
{
    long long ret = 0, tinc;
    while(DiBFS())
    {
        for(int i = 0; i <= n; i ++)
            work[i] = fst[i];
        while(tinc = DiDFS(so, inf))
            ret += tinc;
    }
    return ret;
}
int main()
{
    int tu, tv, tw;
    while(scanf("%d%d%d%d", &n, &m, &so, &te) != EOF)
    {
        memset(fst, -1, sizeof(fst));
        tp = 0;
        for(int i = 0; i < m; i ++)
        {
            scanf("%d%d%d", &tu, &tv, &tw);
            AddEdge(tu, tv, tw);
            AddEdge(tv, tu, 0);
        }
        printf("%lld\n", Dinic());
    }
    return 0;
}
```

