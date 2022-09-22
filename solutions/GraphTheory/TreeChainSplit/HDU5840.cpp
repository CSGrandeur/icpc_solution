// difficulty: 5
// This world need more Zhu
// 树链剖分 + 线段树
// 这题英文通篇语法错误
// 实测对`k==1`时用线段树处理，`k>1`时在路径上跳着走，就可以了。
// 但是信了网上各种博客说“分块”的邪，实现起来麻烦好多。
// 对较大的 k ，路径上跳着走。
// 对较小的 k ，按 dep 把节点分为 k 类，每类构造成线段树
// 网上的方式是对不同的 k 各做一个线段树，每个线段树上每类覆盖一段，用二分定位。
// 感觉网上这种方式太麻烦了，直接开成 `k * k` 个线段树了，用动态开点。
// 然后发现 k 阈值稍微取大就爆内存，不断调小就发现了开头说的， `k==1` 时用线段树就行了。
// 写都写了，阈值就取 5 吧，可以AC。
// 另一个暂时没搞明白的情况，交 `C++`可以过，交 `G++` 会WA。

#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<algorithm>
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
    dcnt = 1;
    int n_ = fst.size() + 10;
    son.resize(n_);
    siz.resize(n_);
    dep.resize(n_);
    fa.resize(n_);
    top.resize(n_);
    dfn.resize(n_);
    rnk.resize(n_);
    dep[rt] = 1;
    fa[rt] = rt;
    top[0] = 0;
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
    dfn[now] = dcnt;
    rnk[dcnt] = now;
    dcnt ++;
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
struct SegNode
{
    int x, lp, rp, l, r;
    void Init(){x = 0, lp = rp = -1;}
    void Init(int l_, int r_){Init(); l = l_, r = r_;}
    SegNode(){Init();}
    SegNode(int l_, int r_){Init(l_, r_);}
};
std::vector<SegNode> sn;
struct SegTree
{
    // 动态开点线段树
    int rt, segN;
    void Init(int n)
    {
        segN = n + 10;
        rt = sn.size();
        sn.push_back(SegNode(0, segN));
    }
    void CheckNodeP(int &tp, int l, int r)
    {
        if(tp != -1) return;
        tp = sn.size();
        sn.push_back(SegNode(l, r));
    }
    int UpdateNode(int now, int ith, int x)
    {
        if(now == -2) now = this->rt;
        if(sn[now].l == sn[now].r - 1) {return sn[now].x = x;}
        int mid = sn[now].l + sn[now].r >> 1;
        if(ith < mid)
        {
            CheckNodeP(sn[now].lp, sn[now].l, mid);
            sn[now].x = std::max(sn[now].x, UpdateNode(sn[now].lp, ith, x));
        }
        else
        {
            CheckNodeP(sn[now].rp, mid, sn[now].r);
            sn[now].x = std::max(sn[now].x, UpdateNode(sn[now].rp, ith, x));
        }
        return sn[now].x;
    }
    int Query(int now, int l, int r)
    {
        if(now == -2) now = this->rt;
        if(sn[now].l >= l && sn[now].r <= r) return sn[now].x;
        int mid = sn[now].l + sn[now].r >> 1;
        int ret = 0;
        if(l < mid && sn[now].lp != -1) ret = std::max(ret, Query(sn[now].lp, l, r));
        if(r > mid && sn[now].rp != -1) ret = std::max(ret, Query(sn[now].rp, l, r));
        return ret;
    }
};
const int maxb = 5;
int n, m;
std::vector<int> pr;
SegTree sts[maxb][maxb];
FSGraph fg;
inline void GenKSeg(int k, int kmod)
{
    if(sts[k][kmod].rt != -1) return;
    sts[k][kmod].Init(n);
    for(int j = 1; j <= n; j ++)
    {
        if(fg.dep[j] % k != kmod) continue;
        sts[k][kmod].UpdateNode(-2, fg.dfn[j], pr[j]);
    }
}
int QueryPath(int a, int b, int k)
{
    bool lflag = true;
    int lca = fg.LCA(a, b), len = fg.dep[a] + fg.dep[b] - fg.dep[lca] * 2 + 1;
    int ret = 0, lcnt = 0, rcnt = ((k - len % k - 1) % k + k) % k;
    int ldmod = ((fg.dep[a] - k + 1) % k + k) % k, rdmod = ((fg.dep[b] - len % k) % k + k) % k;
    while(fg.top[a] != fg.top[b])
    {
        if(fg.dep[fg.top[a]] < fg.dep[fg.top[b]])
            std::swap(a, b), lflag ^= 1;
        if(k < maxb)
        {
            int &dmod = lflag ? ldmod : rdmod;
            GenKSeg(k, dmod);
            ret = std::max(ret, sts[k][dmod].Query(-2, fg.dfn[fg.top[a]], fg.dfn[a] + 1));
        }
        else
        {
            int &cnt = lflag ? lcnt : rcnt;
            for(int i = fg.dfn[a] + 1 - (k - cnt % k); i >= fg.dfn[fg.top[a]]; i -= k)
                ret = std::max(ret, pr[fg.rnk[i]]);
            cnt += fg.dfn[a] - fg.dfn[fg.top[a]] + 1;
        }
        a = fg.fa[fg.top[a]];
    }
    if(fg.dfn[a] > fg.dfn[b]) std::swap(a, b);
    else lflag ^= 1;
    if(k < maxb)
    {
        int &dmod = lflag ? ldmod : rdmod;
        GenKSeg(k, dmod);
        ret = std::max(ret, sts[k][dmod].Query(-2, fg.dfn[a], fg.dfn[b] + 1));
    }
    else
    {
        int &cnt = lflag ? lcnt : rcnt;
        for(int i = fg.dfn[b] + 1 - (k - cnt % k); i >= fg.dfn[a]; i -= k)
            ret = std::max(ret, pr[fg.rnk[i]]);
    }
    return ret;
}
int main()
{
    int t, cs, u, v, k;
    for(cs = 1, scanf("%d", &t); cs <= t; cs ++)
    {
        sn.clear();
        memset(sts, -1, sizeof(sts));
        scanf("%d%d", &n, &m);
        pr.resize(n + 10);
        for(int i = 1; i <= n; i ++)
            scanf("%d", &pr[i]);
        fg.Init(n);
        for(int i = 1; i < n; i ++)
            scanf("%d%d", &u, &v), fg.DbEdge(u, v);
        fg.TreeChainBuild(1);
        printf("Case #%d:\n", cs);
        while(m --)
        {
            scanf("%d%d%d", &u, &v, &k);
            printf("%d\n", QueryPath(u, v, k));
        }
    }
    return 0;
}