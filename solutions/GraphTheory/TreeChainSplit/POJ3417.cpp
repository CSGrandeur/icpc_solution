// difficulty: 3
// Network 
// 树链剖分 + 树上差分
// 这道题思维难度大一些。从圈的角度考虑，`(u, v)` 添加一条边，则`u, v` 与他们的最近公共祖先 `lca` 构成一个圈 `(u, lca, v)`。
// 题目计算的是破一个树边和一个增边，那么破这种圈，就是`(u,lca)`、`(v,lca)`两个路径上的任一条边，加`(u,v)`这条边。
// 而如果一个树边包含在两个圈里，则破这个树边加一个增边不足以实现题目要求了。
// 从而可知，利用树上差分将增边`(u,v)`的`(u, lca)`和`(v,lca)`两条链都加 `1` 的统计值。
// 对统计值 0 的树边，可与 `m` 个增边任意组合
// 统计值 1 的树边，只能与对应的 `(u,v)`组合
// 统计值大于 1 的树边，无法贡献

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
const int maxn = 1e5 + 10;
int n, m, cc[maxn], ans;
FSGraph fg;
void SumDfs(int now)
{
    for(int i = fg.fst[now]; i != -1; i = fg.nex[i])
    {
        if(fg.v[i] == fg.fa[now]) continue;
        SumDfs(fg.v[i]);
        cc[now] += cc[fg.v[i]];
    }
    if(cc[now] == 0 && now != (n >> 1)) ans += m;
    else if(cc[now] == 1) ans ++;
}
int main()
{
    int u, v;
    while(scanf("%d%d", &n, &m) != EOF)
    {
        memset(cc, 0, sizeof(cc));
        fg.Init(n);
        for(int i = 1; i < n; i ++)
        {
            scanf("%d%d", &u, &v);
            fg.DbEdge(u, v);
        }
        fg.TreeChainBuild(n >> 1);
        for(int i = 0; i < m; i ++)
        {
            scanf("%d%d", &u, &v);
            int lca = fg.LCA(u, v);
            cc[u] ++;
            cc[v] ++;
            cc[lca] -= 2;
        }
        ans = 0;
        SumDfs(n >> 1);
        printf("%d\n", ans);
    }
    return 0;
}