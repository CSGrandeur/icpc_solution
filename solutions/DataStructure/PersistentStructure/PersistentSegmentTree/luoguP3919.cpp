// difficulty: 2
// 可持久化线段树 1（可持久化数组）
// 可持久化数据结构入门，单点更新与查询
// 尝试vector封装，这里有坑：
// 不能在对vector赋值时使用能修改vector尺寸的递归函数的返回值，
// 比如 lc[now] = Build(...) ，在Build()递归时修改了lc尺寸的话，无法完成正确赋值
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
const int maxn = 1e6 + 10;
int n, m;
int a[maxn];
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
    int _Build(int left, int right, int a[])
    {
        if(left >= right) return -1;
        int now = Add();
        if(left == right - 1)
        {
            val[now] = a[left];
        }
        else
        {
            int mid = left + right >> 1;
            int nlc = _Build(left, mid, a);
            lc[now] = nlc;
            int nrc = _Build(mid, right, a);
            rc[now] = nrc;
        }
        return now;
    }
    void Build(int left, int right, int a[])
    {
        rt.push_back(_Build(left, right, a));
    }
    int _Update(int now, int left, int right, int v, int loc)
    {
        int nex = Clone(now);
        if(left == right - 1) 
        {
            val[nex] = v;
        }
        else
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
    int Query(int now, int left, int right, int loc)
    {
        if(left == right - 1)
        {
            return val[now];
        }
        int mid = left + right >> 1;
        if(loc < mid) return Query(lc[now], left, mid, loc);
        return Query(rc[now], mid, right, loc);
    }
};
int main()
{
    PersistentSegTree pst;
    int ver, op, loc, vl;
    while(scanf("%d%d", &n, &m) != EOF)
    {
        for(int i = 0; i < n; i ++)
            scanf("%d", &a[i]);
        pst.Init();
        pst.Build(0, n, a);
        for(int i = 0; i < m; i ++)
        {
            scanf("%d%d%d", &ver, &op, &loc);
            if(op == 1) 
            {
                scanf("%d", &vl);
                pst.Update(pst.rt[ver], 0, n, vl, loc - 1);
            }
            else
            {
                printf("%d\n", pst.Query(pst.rt[ver], 0, n, loc - 1));
                pst.rt.push_back(pst.rt[ver]);
            }

        }
    }
    return 0;
}