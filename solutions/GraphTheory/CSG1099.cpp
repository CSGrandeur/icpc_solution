// difficulty: 3
// Minimum Spanning Tree
// 此题重在思维，证明最优解只在 l 或 r 出现。
// 证：假设答案最小生成树的边集合为 E，其边权和为 sum(E.a) + k*sum(E.b)，其中 l <= k <= r
// 而sum(E.a)、sum(E.b)是定值，故 sum(E.a) + k*sum(E.b) 关于 k 单调，
// 如果 k 不为 l 或 r，则 l 与 r 必有一个能使 sum(E.a) + k*sum(E.b) 取更小值。
// 从而最终答案为取 l 和取 r 各做一次最小生成树，取较小值。

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
const int maxn = 1e5 + 10;
const int maxm = 2e5 + 10;
int n, m, l, r, k;
struct Edge
{
    int u, v, a, b;
    long long Elen(){return a + 1LL * b * k;}
    bool operator<(Edge e){return Elen() < e.Elen();}
};
Edge eg[maxm];
int p[maxn];
int fa(int x){return p[x] = x == p[x] ? x : fa(p[x]);}

long long Kruskal()
{
    long long res = 0;
    int edgeNum = 0;
    for(int i = 1; i <= n; i ++) p[i] = i;
    std::sort(eg, eg + m);
    for(int i = 0; i < m; i ++)
    {
        if(fa(eg[i].u) != fa(eg[i].v))
        {
            res += eg[i].Elen();
            p[fa(eg[i].u)] = fa(eg[i].v);
            if(++ edgeNum == n - 1) break;
        }
    }
    return res;
}
int main()
{
    while(scanf("%d%d%d%d", &n, &m, &l, &r) != EOF)
    {
        for(int i = 0; i < m; i ++)
            scanf("%d%d%d%d", &eg[i].u, &eg[i].v, &eg[i].a, &eg[i].b);
        k = l;
        long long ans = Kruskal();
        k = r; ans = std::min(ans, Kruskal());
        printf("%lld\n", ans);
    }
    return 0;
}