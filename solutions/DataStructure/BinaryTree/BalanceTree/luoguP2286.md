### `[HNOI2004]宠物收养场`

> difficulty: 2

可作为Splay的 Pre、Next、Delete 练习

```cpp

#include<cstdio>
#include<algorithm>
#include<vector>
#include<queue>

struct SplayNode
{
    int ch[2], fa, val, cnt, sz, lz;
    void Init() {ch[0] = ch[1] = fa = val = cnt = sz = lz = 0;}
    SplayNode(){Init();}
};
struct SplayTree
{
    // 注意预留 sn[0] 为空节点。 sn.clear(), sn.push_back(SplayNode());
    std::vector<SplayNode> sn;
    int rt, tot, kind;
    void Init(){rt = tot = 0; sn.clear(); sn.push_back(SplayNode());}
    SplayTree(){Init();}
    void Maintain(int x)
    {
        sn[x].sz = sn[sn[x].ch[0]].sz + sn[sn[x].ch[1]].sz + sn[x].cnt;
    }
    inline void PushDown(int x)
    {
        // Nothing
    }
    int Ws(int x) {return x == sn[sn[x].fa].ch[1];}
    void Clear(int x) {sn[x].Init();}
    void Rotate(int x)
    {
        int y = sn[x].fa, z = sn[y].fa;
        PushDown(y), PushDown(x);
        int chk = Ws(x);
        sn[y].ch[chk] = sn[x].ch[chk ^ 1];
        if(sn[y].ch[chk]) sn[sn[y].ch[chk]].fa = y;
        sn[x].ch[chk ^ 1] = y;
        sn[y].fa = x;
        sn[x].fa = z;
        if(z) sn[z].ch[y == sn[z].ch[1]] = x;
        Maintain(y);
        Maintain(x);
    }
    void Splay(int x, int target=0)
    {
        for(int y = sn[x].fa; (y = sn[x].fa) != target; Rotate(x))
            if(sn[y].fa != target) Rotate(Ws(x) == Ws(y) ? y : x);
        if(!target) rt = x;
    }
    int Add(int k, int fa=0)
    {
        tot ++;
        int nn = sn.size();
        sn.push_back(SplayNode());
        sn[nn].val = k;
        sn[nn].cnt ++;
        sn[nn].fa = fa;
        Maintain(nn);
        if(fa)
        {
            sn[fa].ch[sn[fa].val < k] = nn;
            Maintain(fa);
        }
        return nn;
    }
    void Insert(int k)
    {
        if(!rt)
        {
            rt = Add(k);
            return;
        }
        int cur = rt, f = 0;
        while(true)
        {
            if(sn[cur].val == k)
            {
                sn[cur].cnt ++;
                Maintain(cur);
                Maintain(f);
                Splay(cur);
                break;
            }
            f = cur;
            cur = sn[cur].ch[sn[cur].val < k];
            if(!cur)
            {
                int nn = Add(k, f);
                Splay(nn);
                break;
            }
        }
    }
    int KthNode(int k)
    {
        int now = rt;
        while(true)
        {
            PushDown(now);
            if(sn[now].ch[0] && k <= sn[sn[now].ch[0]].sz)
                now = sn[now].ch[0];
            else
            {
                k -= sn[now].cnt + sn[sn[now].ch[0]].sz;
                if(k <= 0)
                {
                    Splay(now);
                    return now;
                }
                now = sn[now].ch[1];
            }
        }
        return 0;
    }
    int Pre(int cur=0)
    {
        if(!cur) cur = sn[rt].ch[0];
        else cur = sn[cur].ch[0];
        if(!cur) return cur;
        while(sn[cur].ch[1]) cur = sn[cur].ch[1];
        Splay(cur);
        return cur;
    }
    int Nxt(int cur=0)
    {
        if(!cur) cur = sn[rt].ch[1];
        else cur = sn[cur].ch[1];
        if(!cur) return cur;
        while(sn[cur].ch[0]) cur = sn[cur].ch[0];
        Splay(cur);
        return cur;
    }
    int Rank(int k)
    {
        int ret = 0, cur = rt;
        while(true)
        {
            if(k < sn[cur].val)
                cur = sn[cur].ch[0];
            else
            {
                ret += sn[sn[cur].ch[0]].sz;
                if(k == sn[cur].val)
                {
                    Splay(cur);
                    return ret + 1;
                }
                ret += sn[cur].cnt;
                cur = sn[cur].ch[1];
            }
        }
        return 0;
    }
    void Del(int k)
    {
        Rank(k);
        if(sn[rt].cnt > 1)
        {
            sn[rt].cnt --;
            Maintain(rt);
            return;
        }
        if(!sn[rt].ch[0] && !sn[rt].ch[1])
        {
            Clear(rt);
            rt = 0;
            return;
        }
        for(int i = 0; i <= 1; i ++)
        {
            if(sn[rt].ch[i]) continue;
            int cur = rt;
            rt = sn[rt].ch[i ^ 1];
            sn[rt].fa = 0;
            Clear(cur);
            return;
        }
        int cur = rt, x = Pre();
        sn[sn[cur].ch[1]].fa = x;
        sn[x].ch[1] = sn[cur].ch[1];
        Clear(cur);
        Maintain(rt);
    }
};
SplayTree spt;
int main()
{
    int n, res, a, b;
    while(scanf("%d", &n) != EOF)
    {
        spt.Init();
        res = 0;
        for(int i = 0; i < n; i ++)
        {
            scanf("%d%d", &a, &b);
            if(spt.sn[spt.rt].sz == 0 || spt.kind == a)
            {
                spt.Insert(b);
                spt.kind = a;
                continue;
            }
            spt.Insert(b);
            int bnode = spt.rt, chs;
            if(spt.sn[bnode].cnt == 1)
            {
                int pre = spt.Pre(bnode), nxt = spt.Nxt(bnode);
                if(!pre) chs = nxt;
                if(!nxt) chs = pre;
                if(pre && nxt)
                    chs = abs(spt.sn[pre].val - b) <= abs(spt.sn[nxt].val - b) ? pre : nxt;
            }
            else
                chs = bnode;
            res = (1LL * res + abs(spt.sn[chs].val - b)) % 1000000;
            spt.Del(b);
            spt.Del(spt.sn[chs].val);
        }
        printf("%d\n", res);
    }
    return 0;
}
```

