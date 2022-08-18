// difficulty: 2
// 【模板】最近公共祖先（LCA）
// 适合做树链剖分入门的习题，先完成树链剖分
// 如果两点不在同一条链上，就将深度较大的点移动至所在重链链端的父节点 `a = fa[top[a]]`
// 直到两点到达同一条链上，输出深度较小的点

#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>
struct FSGraph
{
    std::vector<int> fst, nex, u, v, w;
    void Init(int n_=-1)
    {
        nex.clear();
        u.clear(); v.clear(); w.clear();
        if(n_ >= 0) fst.resize(n_ + 10), std::fill(fst.begin(), fst.end(), -1);
    }
    void AddEdge(int u_, int v_, int w_=1)
    {
        nex.push_back(fst[u_]);
        u.push_back(u_);
        v.push_back(v_);
        w.push_back(w_);
        fst[u_] = nex.size() - 1;
    }
    void DbEdge(int u_, int v_, int w_=1, int w2_=1)
    {
        AddEdge(u_, v_, w_);
        AddEdge(v_, u_, w2_);
    }
    std::vector<int> son, siz, dep, fa, top, dfn, rnk;
    int rt, dcnt;
    void TreeChainBuild(int rt_);
    void TreeChainDFS1(int now);
    void TreeChainDFS2(int now, int t);
    int LCA(int a, int b);
};
void FSGraph::TreeChainBuild(int rt_)
{
    rt = rt_;
    dcnt = 0;
    int n_ = fst.size() + 10;
    son.resize(n_);
    siz.resize(n_);
    dep.resize(n_);
    fa.resize(n_);
    top.resize(n_);
    dfn.resize(n_);
    rnk.resize(n_);
    fa[rt] = -1;
    TreeChainDFS1(rt);
    TreeChainDFS2(rt, rt);
}
void FSGraph::TreeChainDFS1(int now)
{
    son[now] = -1;
    siz[now] = 1;
    for(int i = fst[now]; i != -1; i = nex[i])
    {
        if(v[i] == fa[now]) continue;
        fa[v[i]] = now;
        dep[v[i]] = dep[now] + 1;
        TreeChainDFS1(v[i]);
        siz[now] += siz[v[i]];
        if(son[now] == -1 || siz[v[i]] > siz[son[now]])
            son[now] = v[i];
    }
}
void FSGraph::TreeChainDFS2(int now, int t)
{
    top[now] = t;
    dcnt ++;
    dfn[now] = dcnt;
    rnk[dcnt] = now;
    if(son[now] == -1) return;
    TreeChainDFS2(son[now], t);
    for(int i = fst[now]; i != -1; i = nex[i])
        if(v[i] != son[now] && v[i] != fa[now])
            TreeChainDFS2(v[i], v[i]);

}
int FSGraph::LCA(int a, int b)
{
    while(top[a] != top[b])
    {
        if(dep[top[a]] >= dep[top[b]]) a = fa[top[a]];
        else b = fa[top[b]];
    }
    return dep[a] < dep[b] ? a : b;
}
int n, m, s;
FSGraph fg;
int main()
{
    int a, b;
    while(scanf("%d%d%d", &n, &m, &s) != EOF)
    {
        fg.Init(n);
        for(int i = 1; i < n; i ++)
        {
            scanf("%d%d", &a, &b);
            fg.DbEdge(a, b);
        }
        fg.TreeChainBuild(s);

        for(int i = 0; i < m; i ++)
        {
            scanf("%d%d", &a, &b);
            printf("%d\n", fg.LCA(a, b));
        }
    }
    return 0;
}
