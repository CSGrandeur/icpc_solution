// difficulty: 3
// [JLOI2014]松鼠的新家
// 树链剖分 + 树上差分
// 某个子节点 `+1`，对应某个祖先节点 `-1`，通过DFS就能表达该子节点到这个祖先节点这条链的每个节点都`+1`
// 每条路径都是两点分别到其LCA的一条链，分别做个差分统计
// 端点统计值特殊处理一下

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
const int maxn = 3e5 + 10;
int n;
int a[maxn], df[maxn];
FSGraph fg;
void SumDfs(int now)
{
    for(int i = fg.fst[now]; i != -1; i = fg.nex[i])
    {
        if(fg.v[i] == fg.fa[now]) continue;
        SumDfs(fg.v[i]);
        df[now] += df[fg.v[i]];
    }
}
int main()
{
    int u, v;
    while(scanf("%d", &n) != EOF)
    {
        for(int i = 0; i < n; i ++)
            scanf("%d", &a[i]);
        memset(df, 0, sizeof(df));
        fg.Init(n);
        for(int i = 1; i < n; i ++)
        {
            scanf("%d%d", &u, &v);
            fg.DbEdge(u, v);
        }
        fg.TreeChainBuild(n >> 1);

        for(int i = 1; i < n; i ++)
        {
            int lca = fg.LCA(a[i - 1], a[i]);
            df[a[i - 1]] ++;
            df[a[i]] ++;
            df[lca] --;
            if(fg.fa[lca] >= 0)
                df[fg.fa[lca]] --;
        }
        SumDfs(n >> 1);
        for(int i = 1; i < n; i ++)
            df[a[i]] --;  // 去掉到达和出发的重复
        for(int i = 1; i <= n; i ++)
            printf("%d\n", df[i]);        
    }
    return 0;
}