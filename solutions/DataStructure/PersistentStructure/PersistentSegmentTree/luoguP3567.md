### `[POI2014]KUR-Couriers`

> difficulty: 3

可持久化数据结构入门，静态线段树版本控制

建区间个数统计线段树，初始化每个位置为0，表示每种数的个数

按顺序把每个数以可持久化的方式插入树，即每次插入一条链，新增一个root

对于 l r 的查询，即第 r 个root对应的版本减去第 l - 1 个root对应的版本即为 [l, r] 这个区间的数对应的区间个数统计线段树

找数量多于半数的区间。如果整个区间都不多于半数则直接返回。

这题讨论区有人提出卡可持久化线段树的内存。这个代码全vector动态结构，没有出现卡内存问题。

带上O2提交轻松过。不带O2的话卡第二组数据。

```cpp

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
    int Query(int lnow, int rnow, int left, int right, int halfnum)
    {
        if(val[rnow] - val[lnow] <= halfnum) return -1;
        if(left == right - 1) return left;
        int sum = val[lc[rnow]] - val[lc[lnow]];
        int mid = left + right >> 1;
        if(sum > halfnum) return Query(lc[lnow], lc[rnow], left, mid, halfnum);
        else return Query(rc[lnow], rc[rnow], mid, right, halfnum);
    }
};
int main()
{
    PersistentSegTree pst;
    int tmp, l, r;
    std::vector<int> a;
    while(scanf("%d%d", &n, &m) != EOF)
    {
        a.clear();
        pst.Init();
        pst.Build(0, n);
        for(int i = 0; i < n; i ++)
        {
            scanf("%d", &tmp);
            pst.Update(pst.rt.back(), 0, n, 1, tmp - 1);
        }
        for(int i = 0; i < m; i ++)
        {
            scanf("%d%d", &l, &r);
            printf("%d\n", pst.Query(pst.rt[l - 1], pst.rt[r], 0, n, r - l + 1 >> 1) + 1);
        }
    }
    return 0;
}
```

