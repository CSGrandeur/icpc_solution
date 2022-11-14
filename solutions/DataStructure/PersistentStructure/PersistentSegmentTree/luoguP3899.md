### `[湖南集训]更为厉害`

> difficulty: 4

可持久化线段树处理二维范围和

题意：a为p节点，找到 b 与a距离不超过k，且c同时在a和b的子树中的(a,b,c)情况数

两种情况。

情况1：b为a的祖先节点，则c为a的后代节点，情况总数为 min(depth[a], k) * (size[a] - 1)，

即 a 的祖先节点距离 k 内的个数为 b 的情况数，a的后代节点个数为 c 的情况数，乘起来。

情况2：b为a的后代节点，则c只能是b的后代节点，情况总数为 a 的所有距离不超过 k 的后代，每个后代的后代节点数之和

关键点：注意到一棵子树的所有节点在DFS访问中是连续的，把DFS访问序号vis[]当横坐标，把节点深度depth[]当纵坐标

则考察的就是横坐标 depth[a] + 1到depth[a] + k 之间，纵坐标 vis[a] + 1 到 vis[a] + size[a] - 1 之间，所有点权值之和

点的权值就是该节点后代节点个数，即size[x]-1

这就成了平面范围点权和，感觉用KD-Tree能做。这里用主席树来做。

```cpp

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<queue>
#include<algorithm>
typedef long long LL;
struct Tree
{
    std::vector<int> fst, nex, v;
    void Init(int n_=0)
    {
        fst.clear(); 
        if(n_) fst.resize(n_ + 1, -1), 
        nex.clear(), v.clear();
    }
    void AddEdge(int u_, int v_)
    {
        while(fst.size() <= u_) fst.push_back(-1);
        while(fst.size() <= v_) fst.push_back(-1);
        v.push_back(v_);
        nex.push_back(fst[u_]);
        fst[u_] = nex.size() - 1;
    }
    void DbEdge(int u_, int v_){AddEdge(u_, v_); AddEdge(v_, u_);}
};
struct PersistentSegTree
{
    std::vector<int> rt, lc, rc;
    std::vector<LL> sum;
    void Init()
    {
        rt.clear(), lc.clear(), rc.clear(), sum.clear();
    }
    int Add(LL s=0, int l=-1, int r=-1)
    {
        sum.push_back(s);
        lc.push_back(l);
        rc.push_back(r);
        return lc.size() - 1;
    }
    int Clone(int now)
    {
        if(now == -1) return Add();
        return Add(sum[now], lc[now], rc[now]);
    }
    void Build()
    {
        rt.push_back(Add());
    }
    int Update(int now, int left, int right, int v, int loc)
    {
        int nex = Clone(now);
        sum[nex] += v;
        if(left < right - 1)
        {
            int mid = left + right >> 1;
            if(loc < mid) {int nlc = Update(lc[nex], left, mid, v, loc); lc[nex] = nlc;}
            else {int nrc = Update(rc[nex], mid, right, v, loc); rc[nex] = nrc;}
        }
        return nex;
    }
    LL Query(int lv, int rv, int l, int r, int left, int right)
    {
        if(lv == -1 && rv == -1 || r <= left || l >= right) return 0;
        if(l <= left && r >= right) return (rv == -1 ? 0 : sum[rv]) - (lv == -1 ? 0 : sum[lv]);
        int mid = left + right >> 1;
        return Query(lv == -1 ? -1 : lc[lv], rv == -1 ? -1 : lc[rv], l, r, left, mid) + 
        Query(lv == -1 ? -1 : rc[lv], rv == -1 ? -1 : rc[rv], l, r, mid, right);
    }
};
std::vector<int> order, vis, depth;
std::vector<LL> sz;
PersistentSegTree pst;
Tree tr;
int n, q, vorder;
LL DFS(int cur, int depth_)
{
    // 处理DFS访问序、节点深度、子树大小
    if(order[cur] != -1) return 0;
    order[cur] = vorder ++;
    vis.push_back(cur);
    depth[cur] = depth_;
    for(int i = tr.fst[cur]; i != -1; i = tr.nex[i])
        sz[cur] += DFS(tr.v[i], depth_ + 1);
    return ++ sz[cur];
}
int main()
{
    int u, v, p, k;
    while(scanf("%d%d", &n, &q) != EOF)
    {
        tr.Init(n);
        for(int i = 1; i < n; i ++)
        {
            scanf("%d%d", &u, &v);
            tr.DbEdge(u, v);
        }
        order.clear(); order.resize(n + 1, -1);
        vis.clear();
        depth.clear(); depth.resize(n + 1, -1);
        sz.clear(); sz.resize(n + 1, 0);
        vorder = 1;
        DFS(1, 0);
        pst.Init();
        pst.Build();
        for(int i = 0; i < n; i ++)
            pst.rt.push_back(pst.Update(pst.rt.back(), 0, n, sz[vis[i]] - 1, depth[vis[i]]));
        while(q --)
        {
            scanf("%d%d", &p, &k);
            LL ans = std::min(depth[p], k) * (sz[p] - 1);
            ans += pst.Query(
                pst.rt[order[p]], pst.rt[order[p] + sz[p] - 1], 
                depth[p] + 1, std::min(depth[p] + k + 1, n), 
                0, n
            );
            printf("%lld\n", ans);
        }
    }
    return 0;
}
```

