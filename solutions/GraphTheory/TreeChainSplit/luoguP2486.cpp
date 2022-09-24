// difficulty: 4
// [SDOI2011]染色
// 树链剖分 + 线段树
// 区间连续颜色个数也算经典问题了：
// 线段树维护区间不同的连续颜色个数、左端点颜色、右端点颜色，
// 区间合并时特判左区间右端点与右区间左端点
// 其它的就是树链剖分常规操作了
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
    std::vector<int> vw;
    void ValueBindNode(std::vector<int> &pr);
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
    fa[rt] = 0;
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
void FSGraph::ValueBindNode(std::vector<int> &pr)
{
    vw.resize(dcnt + 10);
    for(int i = 1; i < dfn.size(); i ++)
        vw[dfn[i]] = pr[i];
}
struct SegNode
{
    int lp, rp, l, r, x, lc, rc, lz;
    void Init(){x = lz = 0, lc = rc = -1, lp = rp = -1;}
    void Init(int l_, int r_){Init(); l = l_, r = r_;}
    SegNode(){Init();}
    SegNode(int l_, int r_){Init(l_, r_);}
};
std::vector<SegNode> sn;
struct SegTree
{
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
    void Build(int now, std::vector<int> &vw)
    {
        if(now == -2) now = this->rt;
        if(sn[now].l == sn[now].r - 1)
        {
            if(vw.size() > sn[now].l)
                sn[now].x = 1, sn[now].lc = sn[now].rc = vw[sn[now].l];
            return;
        }
        int mid = sn[now].l + sn[now].r >> 1;
        if(sn[now].l < mid)
        {
            CheckNodeP(sn[now].lp, sn[now].l, mid);
            Build(sn[now].lp, vw);
        }
        if(sn[now].r > mid)
        {
            CheckNodeP(sn[now].rp, mid, sn[now].r);
            Build(sn[now].rp, vw);
        }
        PushUp(now);
    }
    void SetNode(int now, int lz)
    {
        sn[now].x = 1;
        sn[now].lc = sn[now].rc = sn[now].lz = lz;
    }
    void PushDown(int now)
    {
        if(sn[now].l == sn[now].r - 1 || !sn[now].lz) return;
        if(sn[now].lp != -1)
            SetNode(sn[now].lp, sn[now].lz);
        if(sn[now].rp != -1)
            SetNode(sn[now].rp, sn[now].lz);
        sn[now].lz = 0;
    }
    void PushUp(int now)
    {
        sn[now].x = 0;
        if(sn[now].lp != -1) 
        {
            sn[now].x += sn[sn[now].lp].x;
            sn[now].lc = sn[sn[now].lp].lc;
        }
        if(sn[now].rp != -1)
        {
            sn[now].x += sn[sn[now].rp].x;
            sn[now].rc = sn[sn[now].rp].rc;
        }
        if(sn[now].lp != -1 && sn[now].rp != -1 && sn[sn[now].lp].rc == sn[sn[now].rp].lc)
            sn[now].x --;
        if(sn[now].x < 0) sn[now].x = 0;
    }
    void UpdateRange(int now, int l, int r, int k)
    {
        if(now == -2) now = this->rt;
        if(sn[now].l >= l && sn[now].r <= r)
        {
            SetNode(now, k);
            return;
        }
        PushDown(now);
        int mid = sn[now].l + sn[now].r >> 1;
        if(l < mid && sn[now].lp != -1) UpdateRange(sn[now].lp, l, r, k);
        if(r > mid && sn[now].rp != -1) UpdateRange(sn[now].rp, l, r, k);
        PushUp(now);
    }
    SegNode SegMerge(const SegNode &ls, const SegNode &rs)
    {
        if(ls.lc == -1) return rs;
        if(rs.lc == -1) return ls;
        SegNode s(-1, -1);
        s.x = rs.x + ls.x - (rs.lc == ls.rc);
        s.lc = ls.lc;
        s.rc = rs.rc;
        return s;
    }
    SegNode Query(int now, int l, int r)
    {
        if(now == -2) now = this->rt;
        if(sn[now].l >= l && sn[now].r <= r) return sn[now];
        PushDown(now);
        int mid = sn[now].l + sn[now].r >> 1;
        SegNode ls, rs;
        if(l < mid && sn[now].lp != -1) ls = Query(sn[now].lp, l, r);
        if(r > mid && sn[now].rp != -1) rs = Query(sn[now].rp, l, r);
        return SegMerge(ls, rs);
    }
};
const int maxb = 5;
int n, m;
std::vector<int> pr;
SegTree sts;
FSGraph fg;
void UpdatePath(int a, int b, int k)
{
    while(fg.top[a] != fg.top[b])
    {
        if(fg.dep[fg.top[a]] < fg.dep[fg.top[b]])
            std::swap(a, b);
        sts.UpdateRange(-2, fg.dfn[fg.top[a]], fg.dfn[a] + 1, k);
        a = fg.fa[fg.top[a]];
    }
    if(fg.dfn[a] > fg.dfn[b]) std::swap(a, b);
    sts.UpdateRange(-2, fg.dfn[a], fg.dfn[b] + 1, k);
}
int QueryPath(int a, int b)
{
    bool lflag = true;
    SegNode lseg, rseg;
    while(fg.top[a] != fg.top[b])
    {
        if(fg.dep[fg.top[a]] < fg.dep[fg.top[b]])
            std::swap(a, b), lflag ^= 1;
        SegNode &nowseg = lflag ? lseg : rseg;
        nowseg = sts.SegMerge(sts.Query(-2, fg.dfn[fg.top[a]], fg.dfn[a] + 1), nowseg);
        a = fg.fa[fg.top[a]];
    }
    if(fg.dfn[a] > fg.dfn[b]) std::swap(a, b);
    else lflag ^= 1;
    
    SegNode &nowseg = lflag ? lseg : rseg;
    nowseg = sts.SegMerge(sts.Query(-2, fg.dfn[a], fg.dfn[b] + 1), nowseg);
    return lseg.x + rseg.x - (lseg.lc == rseg.lc);
}
int main()
{
    char op[3];
    int u, v, k;
    while(scanf("%d%d", &n, &m) != EOF)
    {
        sn.clear();
        pr.resize(n + 10);
        for(int i = 1; i <= n; i ++)
            scanf("%d", &pr[i]);
        fg.Init(n);
        for(int i = 1; i < n; i ++)
            scanf("%d%d", &u, &v), fg.DbEdge(u, v);
        fg.TreeChainBuild(1);
        fg.ValueBindNode(pr);
        sts.Init(n);
        sts.Build(-2, fg.vw);
        while(m --)
        {
            scanf("%s", op);
            if(op[0] == 'C')
            {
                scanf("%d%d%d", &u, &v, &k);
                UpdatePath(u, v, k);
            }
            else
            {
                scanf("%d%d", &u, &v);
                printf("%d\n", QueryPath(u, v));
            }
        }
    }
    return 0;
}