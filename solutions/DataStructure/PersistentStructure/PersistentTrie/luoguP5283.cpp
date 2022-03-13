// difficulty: 4
// [十二省联考 2019] 异或粽子
// 做luoguP4735之后已经有经验处理 Trie、区间异或问题
// 先转为前缀异或 pre，此题求所有 pre[i] ^ pre[j] 的(i,j)对中前 k 大
// 建可持久化Trie，首先对每个版本，求以最右pre[r]为右端点与前缀的最大的异或值插入优先级队列（大根堆）
// 同时插入对应的 r、第k大（第1 大）、版本编号
// 不断出队，对出队的 r、第k大、版本编号，求 r、第k+1大、版本编号，插入队列，直到出队 k 次
// 求一个数在Trie中第 k 大异或值：01反向边子树有不少于k个插入的数则走反向，否则走正向，且k减去反向子树数字个数

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<queue>
#include<algorithm>
#include<utility>
typedef long long LL;
struct Node
{
    LL v;
    int kth;
    int ver;
    LL raq;
    Node(){}
    Node(LL v_, int kth_, int ver_, LL raq_):v(v_),kth(kth_),ver(ver_), raq(raq_){}
    bool operator<(const Node &b)const{
        return v < b.v;
    }
};
struct PersistentTrie
{
    std::vector<int> rt, ch[2], sz;
    const int MBIT = 33;
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
    void Insert(int nv, LL x)
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
    LL Query(int nv, LL x, int k)
    {
        LL res = 0;
        if(Sz(nv) < k) return -1;
        for(int i = MBIT; i >= 0; i --)
        {
            int dir = x >> i & 1;
            if(Sz(Ch(!dir, nv)) >= k)
                nv = Ch(!dir, nv), res = res << 1 | 1;
            else
                k -= Sz(Ch(!dir, nv)), nv = Ch(dir, nv), res <<= 1;
        }
        return res;
    }
};
PersistentTrie pt;
int n, k;
int main()
{
    LL lastaq, x, ans;
    while(scanf("%d%d", &n, &k) != EOF)
    {
        pt.Init();
        pt.Insert(pt.rt.back(), lastaq = 0);
        std::priority_queue<Node> zz;
        for(int i = 0; i < n; i ++)
        {
            scanf("%lld", &x);
            lastaq ^= x;
            zz.push(Node(pt.Query(pt.rt.back(), lastaq, 1), 1, pt.rt.size() - 1, lastaq));
            pt.Insert(pt.rt.back(), lastaq);
        }
        ans = 0;
        while(k --)
        {
            Node now = zz.top();
            zz.pop();
            ans += now.v;
            LL nexv = pt.Query(pt.rt[now.ver], now.raq, now.kth + 1);
            if(nexv != -1) zz.push(Node(nexv, now.kth + 1, now.ver, now.raq));
        }
        printf("%lld\n", ans);
    }
    return 0;
}