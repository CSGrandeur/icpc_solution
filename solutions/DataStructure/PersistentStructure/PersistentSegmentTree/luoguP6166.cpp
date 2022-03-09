// difficulty: 3
// [IOI2012] scrivener
// 可持久化数据结构入门，单点更新与查询
// 每次插入字符就新增版本，需要记录一个当前版本的字符长度以便知道新插入的位置。
// 这个题有点卡内存，初始化的时候可不开所有节点，只动态加点
// 需要开 O2

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<unordered_map>
#include<algorithm>
int n;

struct PersistentSegTree
{
    std::vector<int> rt, lc, rc, sz;
    std::unordered_map<int, char> val;
    void Init()
    {
        rt.clear(), lc.clear(), rc.clear(), val.clear(), sz.clear();
    }
    int Add(int v=0, int l=-1, int r=-1)
    {
        lc.push_back(l);
        rc.push_back(r);
        if(v) val[lc.size() - 1] = v;
        return lc.size() - 1;
    }
    int Clone(int now)
    {
        if(now == -1) return Add();
        return Add(val.count(now) ? val[now] : 0, lc[now], rc[now]);
    }
    void Build()
    {
        rt.push_back(Add());
        sz.push_back(0);
    }
    int _Update(int now, int left, int right, int v, int loc)
    {
        int nex = Clone(now);
        if(left == right - 1)
            val[nex] = v;
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
        sz.push_back(sz.back() + 1);
    }
    char Query(int now, int left, int right, int loc)
    {
        if(left == right - 1)
            return val[now];
        int mid = left + right >> 1;
        if(loc < mid) return Query(lc[now], left, mid, loc);
        return Query(rc[now], mid, right, loc);
    }
};
int main()
{
    PersistentSegTree pst;
    char op[3], letter[20];
    int l, r, opnum;
    while(scanf("%d", &n) != EOF)
    {
        pst.Init();
        pst.Build();
        for(int i = 0; i < n; i ++)
        {
            scanf("%s%s", op, letter);
            if(op[0] == 'T')
            {
                pst.Update(pst.rt.back(), 0, n, letter[0], pst.sz.back());
            }
            else if(op[0] == 'U')
            {
                sscanf(letter, "%d", &opnum);
                pst.rt.push_back(pst.rt[pst.rt.size() - opnum - 1]);
                pst.sz.push_back(pst.sz[pst.sz.size() - opnum - 1]);
            }
            else
            {
                sscanf(letter, "%d", &opnum);
                printf("%c\n", pst.Query(pst.rt.back(), 0, n, opnum));
            }
        }
    }
    return 0;
}