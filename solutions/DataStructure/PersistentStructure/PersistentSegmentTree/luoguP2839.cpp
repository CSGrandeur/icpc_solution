// difficulty: 4
// [国家集训队]middle
// 涉及多个知识点：
// - 区间中位数：二分枚举 mid，大于mid为1，小于等于为-1，区间之和是否小于零进行二分
// - 区间最大前/后缀：区间是一段滑动窗口，找最大中位数窗口，结合区间中位数二分法，就是找 mid 处理为 1、-1 之后窗口和最大的区间
//      这个区间用线段树维护 区间和、区间最大前缀、区间最大后缀 得到，维护方式见代码
// - 多次在线查询，用可持久化线段树
// 对数值离散化，记录每个数值出现过的位置，初始化主席树为全 -1 ，从小到大的每个数对应每个版本，也对应 mid 为该数时，1、-1 线段树的状态
// 对每次给出的 a,b,c,d，二分枚举中位数（在离散化后的序号）mid，[b,c]肯定包含在滑窗中求其和，处理mid版本的线段树的[a,b)的最大后缀和、(c,d]最大前缀和
// 三段数之和为1、-1之和的最大窗口区间，看是否小于零更新二分的left与right
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<unordered_map>
#include<algorithm>
int n, m;

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
struct PersistentSegTree
{
    std::vector<int> rt, lc, rc, sum, lm, rm;
    void Init()
    {
        rt.clear(), lc.clear(), rc.clear(), sum.clear(), lm.clear(), rm.clear();
    }
    int Add(int s=0, int _lm=0, int _rm=0, int l=-1, int r=-1)
    {
        sum.push_back(s);
        lm.push_back(_lm);
        rm.push_back(_rm);
        lc.push_back(l);
        rc.push_back(r);
        return lc.size() - 1;
    }
    int Clone(int now, bool realClone)
    {
        if(now == -1) return Add();
        return realClone ? Add(sum[now], lm[now], rm[now], lc[now], rc[now]) : now;
    }
    void Build()
    {
        rt.push_back(Add());
    }
    void Maintain(int now)
    {
        if(lc[now] == -1 && rc[now] == -1)
            sum[now] = lm[now] = rm[now] = 0;
        else if(lc[now] == -1)
            sum[now] = sum[rc[now]], lm[now] = lm[rc[now]], rm[now] = rm[rc[now]];
        else if(rc[now] == -1)
            sum[now] = sum[lc[now]], lm[now] = lm[lc[now]], rm[now] = rm[lc[now]];
        else
        {
            sum[now] = sum[lc[now]] + sum[rc[now]];
            lm[now] = std::max(lm[lc[now]], sum[lc[now]] + lm[rc[now]]);
            rm[now] = std::max(rm[rc[now]], sum[rc[now]] + rm[lc[now]]);
        }
    }
    int Update(int now, int left, int right, int v, int loc, bool realClone=true)
    {
        int nex = Clone(now, realClone);
        if(left == right - 1)
        {
            sum[nex] = v;
            lm[nex] = rm[nex] = v < 0 ? 0 : v;
        }
        else if(left < right - 1)
        {
            int mid = left + right >> 1;
            if(loc < mid) {int nlc = Update(lc[nex], left, mid, v, loc); lc[nex] = nlc;}
            else {int nrc = Update(rc[nex], mid, right, v, loc); rc[nex] = nrc;}
            Maintain(nex);
        }
        return nex;
    }
    int QuerySum(int now, int l, int r, int left, int right)
    {
        if(now == -1 || r <= left || l >= right) return 0;
        if(l <= left && r >= right) return sum[now];
        int mid = left + right >> 1;
        return QuerySum(lc[now], l, r, left, mid) + QuerySum(rc[now], l, r, mid, right);
    }
    int QueryLm(int now, int l, int r, int left, int right)
    {
        if(now == -1 || r <= left || l >= right) return 0;
        if(l <= left && r >= right) return lm[now];
        int mid = left + right >> 1;
        return std::max(QueryLm(lc[now], l, r, left, mid), QuerySum(lc[now], l, r, left, mid) + QueryLm(rc[now], l, r, mid, right));
    }
    int QueryRm(int now, int l, int r, int left, int right)
    {
        if(now == -1 || r <= left || l >= right) return 0;
        if(l <= left && r >= right) return rm[now];
        int mid = left + right >> 1;
        return std::max(QueryRm(rc[now], l, r, mid, right), QuerySum(rc[now], l, r, mid, right) + QueryRm(lc[now], l, r, left, mid));
    }
};
int main()
{
    PersistentSegTree pst;
    Discretization<int> dt;
    std::vector<int> a;
    int x, tmp, aq[4];
    while(scanf("%d", &n) != EOF)
    {
        x = 0;
        for(int i = 0; i < n; i ++)
            scanf("%d", &tmp), a.push_back(tmp);
        dt.Make(a);
        // 处理离散化后每个数出现的位置列表
        std::vector<std::vector<int> > locList(n, std::vector<int>());
        for(int i = 0; i < n; i ++)
            locList[dt.Loc(a[i])].push_back(i);
        pst.Init();
        pst.Build();
        // 初始化线段树，对应 mid 小于所有数的统计
        for(int i = 0; i < n; i ++)
            pst.Update(pst.rt.back(), 0, n, 1, i, false);
        // 对 mid 从小到大超过每个数的情况建立新的版本，nlogn 时空复杂度
        for(int i = 0; i < dt.size(); i ++)
        {
            pst.rt.push_back(pst.Update(pst.rt.back(), 0, n, -1, locList[i][0]));
            for(int j = 1; j < locList[i].size(); j ++)
                pst.Update(pst.rt.back(), 0, n, -1, locList[i][j], false);
        }
        scanf("%d", &m);
        for(int i = 0; i < m; i ++)
        {
            for(int j = 0; j < 4; j ++)
                scanf("%d", &aq[j]), aq[j] = (aq[j] + x) % n;
            std::sort(aq, aq + 4);
            int left = 1, right = dt.size();
            while(left < right)
            {
                int mid = left + right >> 1;
                if(
                    pst.QuerySum(pst.rt[mid], aq[1], aq[2] + 1, 0, n) +
                    pst.QueryRm(pst.rt[mid], aq[0], aq[1], 0, n) +
                    pst.QueryLm(pst.rt[mid], aq[2] + 1, aq[3] + 1, 0, n) >= 0
                ) left = mid + 1;
                else right = mid;
            }
            x = dt[left - 1];
            printf("%d\n", x);
        }
    }
    return 0;
}