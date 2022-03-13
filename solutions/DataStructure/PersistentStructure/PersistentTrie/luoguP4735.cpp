// difficulty: 4
// 最大异或和
// 做这道题的前提是已经会用Trie求普通的异或最大问题：数组插入Trie，对于特定数x，按二进制位从高到低在Trie上找相反方向。
// 维护异或前缀 pre[r] ^ pre[l - 1] 即可得到 [l, r] 的异或值
// [p, N], x 的异或值，可以表达为 pre[N] ^ x ^ pre[p-1]，如果先计算 y = pre[N] ^ x，则为找与 y 异或最大的 pre[p-1]
// 关于 [l, r] 的范围，对每个 pre[i] 建立版本为 i 的可持久化Trie
// 右端点r：在版本 r 上可以找 r 前缀的最优的 pre[p-1]
// 左端点l：维护子树方向上数字个数sz，把 p 限制在区间 [l,r] 范围内，
//    即在树上走的时候，要走 sz[r] > sz[l] 的结点，这表示这个方向有 l 之后插入的数。反之符合这个方向的数在 l 之前，不在范围内
// 更精确分析一下区间，针对题目给出的[l,r]，应该是在[l-1,r-1]范围找 pre[p - 1] 的这个“p-1”，也就是限制条件为 sz[r-1]>sz[l-2]
// 这里注意到有 l-2 的情况，需要设置个“哨兵”，在一开始先插入一个数字 0 ，对应的版本序号要注意别弄错。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<queue>
#include<algorithm>
struct PersistentTrie
{
    std::vector<int> rt, ch[2], sz;
    const int MBIT = 31;
    void Init()
    {
        rt.clear(), ch[0].clear(), ch[1].clear(), sz.clear();
        rt.push_back(Add());
    }
    int Add(int s=0, int l=-1, int r=-1)
    {
        sz.push_back(s);
        ch[0].push_back(l);
        ch[1].push_back(r);
        return ch[0].size() - 1;
    }
    int Clone(int now)
    {
        if(now == -1) return Add();
        return Add(sz[now], ch[0][now], ch[1][now]);
    }
    void Insert(int nv, int x)
    {
        int nex = Clone(nv);
        sz[nex] ++;
        rt.push_back(nex);
        for(int i = MBIT; i >= 0; i --)
        {
            int dir = x >> i & 1;
            int newNex = Clone(ch[dir][nex]);
            sz[newNex] ++;
            ch[dir][nex] = newNex;
            nex = ch[dir][nex];
        }
    }
    inline int Sz(int now){return now == -1 ? 0 : sz[now];}
    inline int Ch(int dir, int now){return now == -1 ? -1 : ch[dir][now];}
    int Query(int lv, int rv, int x)
    {
        int res = 0;
        for(int i = MBIT; i >= 0; i --)
        {
            int dir = x >> i & 1;
            if(Sz(Ch(!dir, rv)) > Sz(Ch(!dir, lv)))
                lv = Ch(!dir, lv), rv = Ch(!dir, rv), res = res << 1 | 1;
            else
                lv = Ch(dir, lv), rv = Ch(dir, rv), res <<= 1;
        }
        return res;
    }
};
PersistentTrie pt;
int n, m;
int main()
{
    int l, r, x, lastap;
    char op[5];
    while(scanf("%d%d", &n, &m) != EOF)
    {
        pt.Init();
        lastap = 0;
        pt.Insert(pt.rt.back(), 0); // 哨兵
        for(int i = 0; i < n; i ++)
            scanf("%d", &x), lastap ^= x, pt.Insert(pt.rt.back(), lastap);
        while(m --)
        {
            scanf("%s", op);
            if(op[0] == 'A')
            {
                scanf("%d", &x);
                lastap ^= x;
                pt.Insert(pt.rt.back(), lastap);
            }
            else
            {
                scanf("%d%d%d", &l, &r, &x);
                printf("%d\n", pt.Query(pt.rt[l - 1], pt.rt[r], lastap ^ x));
            }
        }
    }
    return 0;
}