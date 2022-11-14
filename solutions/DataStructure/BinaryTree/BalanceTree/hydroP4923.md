### `[Lydsy1706月赛]K小值查询`

> difficulty: 4

大于`2*k`的数减k后树上的相对位置不变，`k~2*k`这些数会变

暴力地把`k~2*k`的数从树种删除，减去k后重新插入，而`2*k`以上的数打lazy标记.

小于`2*k`才暴力，于是每个数暴力操作的次数为`O(log k)`.

模板是“自动”splay的版本容易绕进去，基于旋转找左右端点后的DFS删插数据时，作记录，DFS结束后再统一插入

```cpp

#include<cstdio>
#include<algorithm>
#include<vector>
#include<string>
typedef long long LL;
struct SplayNode
{
    int ch[2], fa, cnt, sz;
    LL val, lz;
    void Init() {ch[0] = ch[1] = fa = val = cnt = sz = lz = 0;}
    SplayNode(){Init();}
};

std::vector<int> tbi, cct;
struct SplayTree
{
    int rt;
    std::vector<SplayNode> sn;
    void Init(){rt = 0;sn.clear(); sn.push_back(SplayNode());}
    SplayTree(){Init();}
    void Maintain(int x){sn[x].sz = sn[sn[x].ch[0]].sz + sn[sn[x].ch[1]].sz + sn[x].cnt;}
    inline void UpdateLz(int x, LL lz)
    {
        if(!x) return;
        sn[x].lz += lz;
        sn[x].val -= lz;
    }
    void PushDown(int x)
    {
        if(!x || !sn[x].lz) return;
        UpdateLz(sn[x].ch[0], sn[x].lz);
        UpdateLz(sn[x].ch[1], sn[x].lz);
        sn[x].lz = 0;
    }
    int Ws(int x) {return x == sn[sn[x].fa].ch[1];}
    void Clear(int x) {sn[x].Init();}
    void Rotate(int x)
    {
        int y = sn[x].fa, z = sn[y].fa;
        int chk = Ws(x);
        sn[y].ch[chk] = sn[x].ch[chk ^ 1];
        if(sn[y].ch[chk]) sn[sn[y].ch[chk]].fa = y;
        sn[x].ch[chk ^ 1] = y;
        sn[y].fa = x;
        sn[x].fa = z;
        if(z) sn[z].ch[y == sn[z].ch[1]] = x;
        Maintain(y), Maintain(x);
    }
    void Splay(int x, int target=0)
    {
        std::vector<int> path;
        for(int f = x; f; f = sn[f].fa) path.push_back(f);
        for(int i = path.size() - 1; i >= 0; i --) PushDown(path[i]);
        for(int y; (y = sn[x].fa) != target; Rotate(x))
            if(sn[y].fa != target) Rotate(Ws(x) == Ws(y) ? y : x);
        if(!target) rt = x;
    }
    int Add(LL k, int cnt=1, int fa=0)
    {
        int nn = sn.size();
        sn.push_back(SplayNode());
        sn[nn].val = k;
        sn[nn].cnt += cnt;
        sn[nn].fa = fa;
        Maintain(nn);
        if(fa)
        {
            sn[fa].ch[sn[fa].val < k] = nn;
            Maintain(fa);
        }
        return nn;
    }
    void Insert(LL k, int cnt=1)
    {
        if(!rt) {rt = Add(k, cnt); return;}
        int cur = rt, f = 0;
        while(true)
        {
            PushDown(cur);
            if(sn[cur].val == k)
            {
                sn[cur].cnt += cnt;
                Maintain(cur);
                Maintain(f);
                Splay(cur);
                return;
            }
            f = cur;
            cur = sn[cur].ch[sn[cur].val < k];
            if(!cur)
            {
                Splay(Add(k, cnt, f));
                return;
            }
        }
    }
    int UpperBound(int k)
    {
        int lastLager = 0, cur = rt;
        while(cur)
        {
            PushDown(cur);
            if(k < sn[cur].val)
            {
                lastLager = cur;
                if(!sn[cur].ch[0]) break;
                cur = sn[cur].ch[0];
            }                
            else
            {
                if(!sn[cur].ch[1]) break;
                cur = sn[cur].ch[1];
            }
        }
        if(sn[cur].val <= k) cur = lastLager;
        if(cur) Splay(cur);
        return cur;
    }
    int KthNode(int k)
    {
        int cur = rt;
        while(true)
        {
            PushDown(cur);
            if(sn[cur].ch[0] && k <= sn[sn[cur].ch[0]].sz)
                cur = sn[cur].ch[0];
            else
            {
                k -= sn[cur].cnt + sn[sn[cur].ch[0]].sz;
                if(k <= 0)
                {
                    Splay(cur);
                    return cur;
                }
                cur = sn[cur].ch[1];
            }
        }
        return 0;
    }
    int RtPre(int cur=0)
    {
        if(!cur) cur = sn[rt].ch[0];
        else cur = sn[cur].ch[0];
        if(!cur) return cur;
        while(sn[cur].ch[1]) {cur = sn[cur].ch[1];}
        Splay(cur);
        return cur;
    }
    void DFS(int cur, int k)
    {
        if(!cur) return;
        PushDown(cur);
        DFS(sn[cur].ch[0], k);
        DFS(sn[cur].ch[1], k);
        sn[sn[cur].fa].ch[Ws(cur)] = 0;
        Maintain(sn[cur].fa);
        tbi.push_back(sn[cur].val);
        cct.push_back(sn[cur].cnt);
    }
};
SplayTree spt;
int main()
{
    int n, m, a, op, k;
    spt.sn.reserve((int)1e5 + 100);
    while(scanf("%d%d", &n, &m) != EOF)
    {
        spt.Init();
        spt.Insert(-1e16);
        spt.Insert(1e16);
        int ith = 0;
        while(n --) scanf("%d", &a), spt.Insert(a);
        while(m --)
        {
            scanf("%d%d", &op, &k);
            if(op == 1) printf("%lld\n", spt.sn[spt.KthNode(k + 1)].val);
            else
            {
                spt.UpperBound(k);
                int l = spt.RtPre();
                int r = spt.UpperBound(k << 1);
                spt.Splay(l);
                spt.Splay(r, l);
                int cur = spt.sn[r].ch[0];
                spt.UpdateLz(r, k);
                spt.PushDown(r);
                tbi.clear(), cct.clear();
                spt.DFS(cur, k);
                for(int i = cur; i; i = spt.sn[i].fa)
                    spt.Maintain(i);
                for(int i = 0; i < tbi.size(); i ++)
                    spt.Insert(tbi[i], cct[i]);
            }
        }
    }
    return 0;
}
```

