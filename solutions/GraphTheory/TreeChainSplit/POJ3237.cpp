// difficulty: 4
// Tree
// 树链剖分 + 线段树
// 拿这道题作树链剖分上的线段树入门了。
// 首先题目的解主要在区间取负，维护max和min，取负时候max和min都取负并交换即可。
// 构造树剖的DFS序用作线段树的编号，即 v 为节点编号时， dfn[v] 为线段树的坐标。
// 把每条边映射到对应的子节点，`(u, v)` 路径修改就是线段树上若干段的修改。
// 路径与线段树上段落的对应关系是一个模板：
/*
```
while(top[a] != top[b]) // 当a和b各自所在重链的顶端不同时
{
    if(dep[top[a]] < dep[top[b]])
        std::swap(a, b);    // 取重链顶端深度较深的为 `a`
    // 处理区间 `[dfn[top[a]], dfn[a] + 1)` 的线段树任务，我习惯写左闭右开区间故这里有 `+1`
    DoSegTreeOperator(dfn[top[a]], dfn[a] + 1); 
    a = fa[top[a]]; // 置 `a` 为其父节点
}
if(dfn[a] > dfn[b]) std::swap(a, b);    // 取dfs序较小的为`a`
// 如果前面循环结束后的`a`与`b`不是同一个点，则再做一次区间操作
// 注意较“浅”的层是祖先节点，也即DFS序（dfn）较小的是祖先节点。
// 祖先节点绑定的边不在路径上，故这里有`dfn[a] + 1`，左闭右开于是有`dfn[b] + 1`。
if(dfn[a] != dfn[b]) DoSegTreeOperator(dfn[a] + 1, dfn[b] + 1);
```
*/

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

struct SegTree
{
    std::vector<int> segmax, segmin, lzNeg;
    int len;
    void Init(int n)
    {
        len = n << 5;
        segmax.resize(len);
        segmin.resize(len);
        lzNeg.resize(len);
        std::fill(segmax.begin(), segmax.end(), 0);
        std::fill(segmin.begin(), segmin.end(), 0);
        std::fill(lzNeg.begin(), lzNeg.end(), 0);
    }
    void BuildTree(int now, int left, int right, int *arr=NULL)
    {
        if(left == right - 1) 
        {
            segmax[now] = segmin[now] = (arr == NULL ? 0 : arr[left]); 
            return;
        }
        int mid = left + right >> 1;
        if(mid > left) BuildTree(now << 1, left, mid, arr);
        if(right > mid) BuildTree(now << 1 | 1, mid, right, arr);
        PushUp(now);
    }
    void UpdateNowNode(int now, int left, int right, int ith, int x)
    {
        if(left == right - 1) {segmax[now] = segmin[now] = x; return;}
        PushDown(now, left, right);
        int mid = left + right >> 1;
        if(ith < mid) UpdateNowNode(now << 1, left, mid, ith, x);
        else UpdateNowNode(now << 1 | 1, mid, right, ith, x);
        PushUp(now);
    }
    void Neg(int now)
    {
        lzNeg[now] ^= 1;
        segmax[now] = -segmax[now];
        segmin[now] = -segmin[now];
        std::swap(segmax[now], segmin[now]);
    }
    void PushDown(int now, int left, int right)
    {
        if(left >= right || !lzNeg[now]) return;
        int mid = left + right >> 1;
        lzNeg[now] ^= 1;
        if(mid > left) Neg(now << 1);
        if(mid < right) Neg(now << 1 | 1);
    }
    void PushUp(int now)
    {
        segmax[now] = std::max(segmax[now << 1], segmax[now << 1 | 1]);
        segmin[now] = std::min(segmin[now << 1], segmin[now << 1 | 1]);
    }
    void NegRange(int now, int left, int right, int l, int r)
    {
        if(left >= l && right <= r)
        {
            Neg(now);
            return;
        }
        PushDown(now, left, right);
        int mid = left + right >> 1;
        if(l < mid) NegRange(now << 1, left, mid, l, r);
        if(r > mid) NegRange(now << 1 | 1, mid, right, l, r);
        PushUp(now);
    }
    int Query(int now, int left, int right, int l, int r)
    {
        if(left >= l && right <= r)
            return segmax[now];
        int mid = left + right >> 1;
        int ret = -0x3f3f3f3f;
        PushDown(now, left, right);
        if(l < mid) ret = std::max(ret, Query(now << 1, left, mid, l, r));
        if(r > mid) ret = std::max(ret, Query(now << 1 | 1, mid, right, l, r));
        return ret;
    }
};
const int maxn = 1e4 + 10;
int n, ar[maxn], iev[maxn];
FSGraph fg;
SegTree st;
char op[20];
int main()
{
    int t, u, v, w;
    for(scanf("%d", &t); t--; )
    {
        scanf("%d", &n);
        fg.Init(n);
        st.Init(n);
        for(int i = 1; i < n; i ++)
            scanf("%d%d%d", &u, &v, &w), fg.DbEdge(u, v, w, w);
        fg.TreeChainBuild(n >> 1);
        for(int i = 0; i < fg.w.size(); i += 2)
        {
            // 将边权绑定至对应的子节点，并存入dfn序对应的线段树编号
            int son = fg.fa[fg.u[i]] == fg.v[i] ? fg.u[i] : fg.v[i];
            iev[i >> 1] = son;  // 第 i>>1 条边对应 son 这个节点
            ar[fg.dfn[son]] = fg.w[i];
        }
        int segN = n + 5;
        st.BuildTree(1, 0, segN, ar);
        int a, b;
        while(scanf("%s", op) && op[0] != 'D')
        {
            scanf("%d%d", &a, &b);
            if(op[0] == 'C')
                st.UpdateNowNode(1, 0, segN, fg.dfn[iev[a - 1]], b);
            else if(op[0] == 'N')
            {
                while(fg.top[a] != fg.top[b])
                {
                    if(fg.dep[fg.top[a]] < fg.dep[fg.top[b]])
                        std::swap(a, b);
                    st.NegRange(1, 0, segN, fg.dfn[fg.top[a]], fg.dfn[a] + 1);
                    a = fg.fa[fg.top[a]];
                }
                if(fg.dfn[a] > fg.dfn[b]) std::swap(a, b);
                if(fg.dfn[a] != fg.dfn[b]) st.NegRange(1, 0, segN, fg.dfn[a] + 1, fg.dfn[b] + 1);
            }
            else
            {
                int ans = -0x3f3f3f3f;
                while(fg.top[a] != fg.top[b])
                {
                    if(fg.dep[fg.top[a]] < fg.dep[fg.top[b]])
                        std::swap(a, b);
                    ans = std::max(ans, st.Query(1, 0, segN, fg.dfn[fg.top[a]], fg.dfn[a] + 1));
                    a = fg.fa[fg.top[a]];
                }
                if(fg.dfn[a] > fg.dfn[b]) std::swap(a, b);
                if(fg.dfn[a] != fg.dfn[b]) ans = std::max(ans, st.Query(1, 0, segN, fg.dfn[a] + 1, fg.dfn[b] + 1));
                printf("%d\n", ans);
            }
        }
    }
    return 0;
}