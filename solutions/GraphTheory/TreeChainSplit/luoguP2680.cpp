// difficulty: 5
// [NOIP2015 提高组] 运输计划
// 树链剖分 + 线段树
// 这道题用二分的话可能没有这么难，但是在做树链剖分练习，所以评为5星难度
// 求最长的路径，对路径上每个边，取 `max(路径长度减去该边, 不包含这条边的最长路径长度)`
// 求路径长度用一个求和线段树。
// 求“不包含这条边的最长路径长度” 用一个求最大值线段树维护，详见 `void UpdateEdgeMax(...)`
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

    // 将边绑定至子节点
    std::vector<int> e2v, v2e, vw;
    void EdgeBindNode();
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
    dfn[now] = dcnt;
    rnk[dcnt] = now;
    dcnt ++;
    if(son[now] == -1) return;
    TreeChainDFS2(son[now], t);
    for(int i = fst[now]; i != -1; i = nex[i])
        if(v[i] != son[now] && v[i] != fa[now])
            TreeChainDFS2(v[i], v[i]);

}
void FSGraph::EdgeBindNode()
{
    // 将边权绑定至对应的子节点，`e2v`为边（按输入顺序）向点的映射
    int vnum = v.size() + 10;
    e2v.resize(vnum);
    v2e.resize(vnum);
    vw.resize(vnum);
    for(int i = 0; i < w.size(); i += 2)
    {
        int son = fa[u[i]] == v[i] ? u[i] : v[i];
        e2v[i >> 1] = son;  // 第 i>>1 条边对应 son 这个节点
        v2e[son] = i >> 1;
        vw[dfn[son]] = w[i];
    }
}

struct SegTreeSum
{
    std::vector<int> sum;
    int segN, len;
    void Init(int n)
    {
        segN = n + 10;
        len = segN << 4 | 1;
        sum.resize(len);
        std::fill(sum.begin(), sum.end(), 0);
    }
    int BuildTree(int now, int left, int right, std::vector<int> &arr)
    {
        if(left == right - 1)
            return sum[now] = arr[left];
        int mid = left + right >> 1;
        if(mid > left) sum[now] += BuildTree(now << 1, left, mid, arr);
        if(right > mid) sum[now] += BuildTree(now << 1 | 1, mid, right, arr);
        return sum[now];
    }
    int Query(int now, int left, int right, int l, int r)
    {
        if(left >= l && right <= r) return sum[now];
        int mid = left + right >> 1;
        int ret = 0;
        if(l < mid) ret += Query(now << 1, left, mid, l, r);
        if(r > mid) ret += Query(now << 1 | 1, mid, right, l, r);
        return ret;
    }
};

struct SegTreeMax
{
    std::vector<int> mx, lz;
    int segN, len;
    void Init(int n)
    {
        segN = n + 10;
        len = segN << 4 | 1;
        mx.resize(len);
        lz.resize(len);
        std::fill(mx.begin(), mx.end(), -0x3f3f3f3f);
        std::fill(lz.begin(), lz.end(), 0);
    }
    void UpdateRange(int now, int left, int right, int l, int r, int x)
    {
        if(l >= r) return;
        PushDown(now, left, right);
        if(left >= l && right <= r)
        {
            UpdateLzMx(now, x);
            return;
        }
        int mid = left + right >> 1;
        if(l < mid) UpdateRange(now << 1, left, mid, l, r, x);
        if(r > mid) UpdateRange(now << 1 | 1, mid, right, l, r, x);
        PushUp(now);
    }
    void UpdateLzMx(int now, int lznum)
    {
        lz[now] = std::max(lz[now], lznum);
        mx[now] = std::max(mx[now], lz[now]);
    }
    void PushDown(int now, int left, int right)
    {
        if(left >= right || !lz[now]) return;
        int mid = left + right >> 1;
        if(mid > left) UpdateLzMx(now << 1, lz[now]);
        if(mid < right) UpdateLzMx(now << 1 | 1, lz[now]);
        lz[now] = 0;
    }
    void PushUp(int now)
    {
        mx[now] = std::max(mx[now << 1], mx[now << 1 | 1]);
    }
    int Query(int now, int left, int right, int l, int r)
    {
        if(left >= l && right <= r) return mx[now];
        int mid = left + right >> 1;
        int ret = -0x3f3f3f3f;
        PushDown(now, left, right);
        if(l < mid) ret = std::max(ret, Query(now << 1, left, mid, l, r));
        if(r > mid) ret = std::max(ret, Query(now << 1 | 1, mid, right, l, r));
        return ret;
    }
};
struct Chain
{
    int l;
    int r;
    Chain();
    Chain(int l_, int r_):l(l_),r(r_){}
    bool operator<(const Chain &b)
    {
        return l < b.l;
    }
};
const int maxn = 1e4 + 10;
int n, m, ar[maxn], iev[maxn];
FSGraph fg;
SegTreeSum sts;
SegTreeMax stmx;
std::vector<Chain> chl;
char op[20];
int PathLen(int a, int b)
{
    int ret = 0;
    while(fg.top[a] != fg.top[b])
    {
        if(fg.dep[fg.top[a]] < fg.dep[fg.top[b]])
            std::swap(a, b);
        ret += sts.Query(1, 0, sts.segN, fg.dfn[fg.top[a]], fg.dfn[a] + 1);
        a = fg.fa[fg.top[a]];
    }
    if(fg.dfn[a] > fg.dfn[b]) std::swap(a, b);
    if(fg.dfn[a] != fg.dfn[b]) ret += sts.Query(1, 0, sts.segN, fg.dfn[a] + 1, fg.dfn[b] + 1);
    return ret;
}
void UpdateEdgeMax(int a, int b, int plen)
{
    // 线段树定义：边（已绑定节点）编号区间的、与该边无交集的最大路径长度
    // 比如 `[l, r)` 区间，表示dfn编号在 `[l, r)` 的这些边都无交集的最大路径`(u, v)`的长度
    // 更新方法：对于一个路径，会有多条不重叠的链，对链排序，在上述线段树中以该路径长度更新这些不重叠链的补集区间
    // 原因：对于只包含`(a,b)`路径里面的边的其它路径（即`(a,b)`的子集），去掉`(a,b)`任何一条边，子集里的路径也仍然不会比`(a,b)`长
    // 所以当在最长路径 pmx 去掉任何一条边 k 的时候，新的最长链只会在 `{pmx - k}` 和 非pmx子集的其它路径中产生。
    
    chl.clear(); // 链区间
    while(fg.top[a] != fg.top[b])
    {
        if(fg.dep[fg.top[a]] < fg.dep[fg.top[b]])
            std::swap(a, b);
        chl.push_back(Chain(fg.dfn[fg.top[a]], fg.dfn[a] + 1));
        a = fg.fa[fg.top[a]];
    }
    if(fg.dfn[a] > fg.dfn[b]) std::swap(a, b);
    if(fg.dfn[a] != fg.dfn[b])
        chl.push_back(Chain(fg.dfn[a] + 1, fg.dfn[b] + 1));
    if(chl.empty()) return;
    std::sort(chl.begin(), chl.end());
    stmx.UpdateRange(1, 0, stmx.segN, 1, chl[0].l, plen);  // dfn[root]为0，root没有对应的边
    stmx.UpdateRange(1, 0, stmx.segN, chl.back().r, n, plen);
    for(int i = 1; i < chl.size(); i ++)
        stmx.UpdateRange(1, 0, stmx.segN, chl[i - 1].r, chl[i].l, plen);
}

int EachEdge(int a, int b, int plen)
{
    int ret = 0x3f3f3f3f;
    while(a != b)
    {
        if(fg.dep[a] < fg.dep[b]) std::swap(a, b);
        ret = std::min(ret, std::max(
            plen - fg.vw[fg.dfn[a]], 
            stmx.Query(1, 0, stmx.segN, fg.dfn[a], fg.dfn[a] + 1)
            ));
        a = fg.fa[a];
    }
    return ret == 0x3f3f3f3f ? 0 : ret;
}
int main()
{
    int t, u, v, w;
    int umx, vmx, plenmx;
    while(scanf("%d%d", &n, &m) != EOF)
    {
        fg.Init(n);
        sts.Init(n);
        stmx.Init(n);
        for(int i = 1; i < n; i ++)
            scanf("%d%d%d", &u, &v, &w), fg.DbEdge(u, v, w, w);
        fg.TreeChainBuild(n >> 1);
        fg.EdgeBindNode();
        sts.BuildTree(1, 0, sts.segN, fg.vw);
        plenmx = -1;
        for(int i = 0; i < m; i ++)
        {
            scanf("%d%d", &u, &v);
            int plen = PathLen(u, v);
            UpdateEdgeMax(u, v, plen);
            if(plen > plenmx)
            {
                umx = u;
                vmx = v;
                plenmx = plen;
            }
        }
        int res = EachEdge(umx, vmx, plenmx);
        printf("%d\n", res);        
    }
    return 0;
}