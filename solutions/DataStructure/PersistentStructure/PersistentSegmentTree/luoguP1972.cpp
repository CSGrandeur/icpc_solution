// difficulty: 4
// [SDOI2009]HH的项链
// 本身不是个可持久化线段树（PST）的题，可以离线用树状数组做，难度系数也不高。
// 但是个练习PST的好机会，由于题目卡常，难度系数大一些。
// 离线思路：按右端点 r 排序后处理，按每个数字坐标1~n，树状数组（或线段树）统计区间数字个数。
// 每个坐标 i 的位置的数读入，则坐标 i 设为 1（+1），该数字上一次出现的位置 j 坐标由 1 改为0（-1）
// 对[l,r]，r位置的数插入后，统计[l,r]的 1 的总数。因为每个数字插入时，上次插入的位置的统计值都减掉了，
// 每个数都只在当前不重复地为它所在的位置贡献了 1 的统计值。
// PST在线思路：就是按离线思路每次插入都生成新版本。
// 题目卡常，本代码有三个优化点：读入外挂、vector预分配内存、query的时候当前版本“右半部分”直接全返回不用递归。
// 此外预分配内存节点数要开大些，这里开 n * 30 最后两组数据会WA，开到 n * 40 过了。

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<unordered_map>
#include<algorithm>
int n, m;
inline int ReadNum()
{
    char ch = getchar(); 
    int sum = 0;
    while(!(ch >= '0' && ch <= '9')) ch = getchar();
    while(ch >= '0' && ch <= '9') 
    sum = sum * 10 + ch - '0', ch = getchar();
    return sum;
}
const int maxn = 1e6 + 10;
const int maxm = maxn * 40;
struct PersistentSegTree
{
    std::vector<int> val, rt, lc, rc;
    void Init()
    {
        rt.clear(), lc.clear(), rc.clear(), val.clear();
        rt.reserve(maxn);
        lc.reserve(maxm);
        rc.reserve(maxm);
        val.reserve(maxm);
    }
    int Add(int v=0, int l=-1, int r=-1)
    {
        val.push_back(v);
        lc.push_back(l);
        rc.push_back(r);
        return lc.size() - 1;
    }
    int Clone(int now)
    {
        if(now == -1) return Add();
        return Add(val[now], lc[now], rc[now]);
    }
    void Build()
    {
        rt.push_back(Add());
    }
    int Update(int now, int left, int right, int v, int loc)
    {
        int nex = Clone(now);
        val[nex] += v;
        if(left < right - 1)
        {
            int mid = left + right >> 1;
            if(loc < mid) {int nlc = Update(lc[nex], left, mid, v, loc); lc[nex] = nlc;}
            else {int nrc = Update(rc[nex], mid, right, v, loc); rc[nex] = nrc;}
        }
        return nex;
    }
    int Query(int now, int l, int left, int right)
    {
        if(now == -1) return 0;
        if(l <= left) return val[now];
        int mid = left + right >> 1, ret = 0;
        if(l < mid) ret += Query(lc[now], l, left, mid);
        ret += Query(rc[now], l, mid, right);
        return ret;
    }
};
int main()
{
    PersistentSegTree pst;
    int tmp, l, r;
    while(scanf("%d", &n) != EOF)
    {
        pst.Init();
        pst.Build();
        std::vector<int> last((int)1e6 + 10, -1);
        for(int i = 0; i < n; i ++)
        {
            tmp = ReadNum();
            int tmpnode = pst.rt.back();
            if(last[tmp] != -1) tmpnode = pst.Update(tmpnode, 0, n, -1, last[tmp]);
            pst.rt.push_back(pst.Update(tmpnode, 0, n, 1, i));
            last[tmp] = i;
        }
        scanf("%d", &m);
        for(int i = 0; i < m; i ++)
        {
            l = ReadNum();
            r = ReadNum();
            printf("%d\n", pst.Query(pst.rt[r], l - 1, 0, n));
        }
    }
    return 0;
}