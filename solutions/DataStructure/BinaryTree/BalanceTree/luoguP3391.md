### `【模板】文艺平衡树`

> difficulty: 3

Splay

将l左侧旋转至根，r右侧旋转至根的右子节点，则要旋转的区间就是根的右子节点的左子节点

类似线段树打lazy标记，注意旋转、查找时候都要`PushDown`

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
    int rt, tot;
    void Init(){rt = tot = 0; sn.clear(); sn.push_back(SplayNode());}
    SplayTree(){Init();}
    void Maintain(int x)
    {
        sn[x].sz = sn[sn[x].ch[0]].sz + sn[sn[x].ch[1]].sz + sn[x].cnt;
    }
    void PushDown(int x)
    {
        if(x && sn[x].lz)
        {
            sn[sn[x].ch[1]].lz ^= 1;
            sn[sn[x].ch[0]].lz ^= 1;
            std::swap(sn[x].ch[1], sn[x].ch[0]);
            sn[x].lz = 0;
        }
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
    int Build(int l, int r, int fa)
    {
        if(l > r - 1) {return 0;}
        int mid = l + r >> 1;
        int now = Add(mid, fa);
        Build(l, mid, now);
        Build(mid + 1, r, now);
        Maintain(now);
        return now;
    }
    void Reverse(int l, int r)
    {
        if(l >= r) return;
        int ln = KthNode(l - 1), rn = KthNode(r);
        Splay(ln, 0);
        Splay(rn, ln);
        int pos = sn[rt].ch[1];
        pos = sn[pos].ch[0];
        sn[pos].lz ^= 1;
    }
};
SplayTree spt;
int n, m, l, r;
void Print(int x, bool &blankFlag)
{
    spt.PushDown(x);
    if(spt.sn[x].ch[0]) Print(spt.sn[x].ch[0], blankFlag);
    if(spt.sn[x].val >= 1 && spt.sn[x].val <= n)
    {
        if(blankFlag) printf(" ");
        blankFlag = true;
        printf("%d", spt.sn[x].val);
    }
    if(spt.sn[x].ch[1]) Print(spt.sn[x].ch[1], blankFlag);
}
int main()
{
    while(scanf("%d%d", &n, &m) != EOF)
    {
        spt.Init();
        spt.rt = spt.Build(0, n + 2, 0);
        while(m --)
        {
            scanf("%d%d", &l, &r);
            // 额外插入了 `0` 和 `n + 1`，故原来的 `l` 是第 `l + 1` 个数
            // 左闭右开故 `r + 2`
            spt.Reverse(l + 1, r + 2);
        }
        bool blankFlag = false;
        Print(spt.rt, blankFlag);
        printf("\n");
    }
    return 0;
}
```

