// difficulty: 3
// 【模板】可持久化线段树 2
// 可持久化数据结构入门，静态线段树版本控制
// 数据离散化，建区间和线段树，初始化每个位置为0，表示每种数的个数
// 按顺序把每个数（离散化后的编号）以可持久化的方式插入树，即每次插入一条链，新增一个root
// 对于 l r k 的查询，即第 r 个root对应的版本减去第 l - 1 个root对应的版本，
// 即为 [l, r] 这个区间的数对应的区间个数统计线段树，此时相当于常规线段树上第k小问题。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<unordered_map>
#include<algorithm>
int n, m;

struct PersistentSegTree
{
    std::vector<int> rt, lc, rc, val;
    void Init()
    {
        rt.clear(), lc.clear(), rc.clear(), val.clear();
    }
    int Add(int v=0, int l=-1, int r=-1)
    {
        val.push_back(v);
        lc.push_back(l);
        rc.push_back(r);
        return val.size() - 1;
    }
    int Clone(int now)
    {
        return Add(val[now], lc[now], rc[now]);
    }
    int _Build(int left, int right)
    {
        if(left >= right) return -1;
        int now = Add();
        if(left < right - 1)
        {
            int mid = left + right >> 1;
            int nlc = _Build(left, mid);
            lc[now] = nlc;
            int nrc = _Build(mid, right);
            rc[now] = nrc;
        }
        return now;
    }
    void Build(int left, int right)
    {
        rt.push_back(_Build(left, right));
    }
    int _Update(int now, int left, int right, int v, int loc)
    {
        int nex = Clone(now);
        val[nex] += v;
        if(left < right - 1) 
        {
            int mid = left + right >> 1;
            if(loc < mid) {int nlc = _Update(lc[nex], left, mid, v, loc); lc[nex] = nlc;}
            else {int nrc = _Update(rc[nex], mid, right, v, loc); rc[nex] = nrc;}
        }
        return nex;
    }
    void Update(int now, int left, int right, int v, int loc)
    {
        rt.push_back(_Update(now, left, right, v, loc));
    }
    int Query(int lnow, int rnow, int left, int right, int kth)
    {
        if(left == right - 1) return left;
        int sum = val[lc[rnow]] - val[lc[lnow]];
        int mid = left + right >> 1;
        if(sum >= kth) return Query(lc[lnow], lc[rnow], left, mid, kth);
        else return Query(rc[lnow], rc[rnow], mid, right, kth - sum);
    }
};
template<typename TP_V>
struct Discretization
{
    std::vector<TP_V> b;
    std::unordered_map<TP_V, int> mp;
    void Init(){b.clear(); mp.clear();}
    Discretization(){Init();}
    Discretization(std::vector<TP_V> &a){Make(a);}
    void Make(std::vector<TP_V> &a)
    {
        Init();
        Add(a);
    }
    void Add(std::vector<TP_V> &a)
    {
        for(auto i : a) b.push_back(i);
        std::sort(b.begin(), b.end());
        b.erase(std::unique(b.begin(), b.end()), b.end());
        for(int i = 0; i < b.size(); i ++) mp[b[i]] = i;
    }
    unsigned size()
    {
        return b.size();
    }
    TP_V &operator[](int ith){return b[ith];}
    int Loc(TP_V x) {return mp.count(x) ? mp[x] : -1;}
};
int main()
{
    PersistentSegTree pst;
    Discretization<int> dt;
    int tmp, l, r, k;
    std::vector<int> a;
    while(scanf("%d%d", &n, &m) != EOF)
    {
        a.clear();
        for(int i = 0; i < n; i ++)
        {
            scanf("%d", &tmp);
            a.push_back(tmp);
        }
        dt.Make(a);
        pst.Init();
        pst.Build(0, dt.size());
        for(int i = 0; i < n; i ++)
            pst.Update(pst.rt.back(), 0, dt.size(), 1, dt.Loc(a[i]));
        for(int i = 0; i < m; i ++)
        {
            scanf("%d%d%d", &l, &r, &k);
            printf("%d\n", dt[pst.Query(pst.rt[l - 1], pst.rt[r], 0, dt.size(), k)]);
        }
    }
    return 0;
}