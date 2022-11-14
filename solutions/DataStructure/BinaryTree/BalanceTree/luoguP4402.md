### `[Cerc2007]robotic sort 机械排序`

> difficulty: 3

Splay可以多种姿势过，转来转去太灵活了。。

记录每个大小的物品在Splay中的节点编号，从小到大处理每个物品

把物品对应节点Splay到根，左子树size可得到改物品位置

然后就是文艺平衡树的旋转。使用模板要特别注意每个遍历操作是否都PushDown了

```cpp

#include<cstdio>
#include<algorithm>
#include<vector>

struct DataNode
{
    int val, ith,  pos;
    bool operator<(const DataNode &that)
    {
        if(val == that.val) return ith < that.ith;
        return val < that.val;
    }
};
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
        // if(fa)
        // {
        //     sn[fa].ch[sn[fa].val < k] = nn;
        //     Maintain(fa);
        // }
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
    int Build(int l, int r, int fa, std::vector<DataNode> &p)
    {
        if(l > r - 1) {return 0;}
        int mid = l + r >> 1;
        int now = Add(p[mid].val, fa);
        p[mid].pos = now;
        int lch = Build(l, mid, now, p);
        int rch = Build(mid + 1, r, now, p);
        sn[now].ch[0] = lch;
        sn[now].ch[1] = rch;
        Maintain(now);
        return now;
    }
    int Nxt(int cur=0)
    {
        PushDown(rt);
        PushDown(cur);
        if(!cur) cur = sn[rt].ch[1];
        else cur = sn[cur].ch[1];
        if(!cur) return cur;
        while(PushDown(cur), sn[cur].ch[0]) cur = sn[cur].ch[0];
        Splay(cur);
        return cur;
    }
    void ReverseByNode(int ln, int rn)
    {
        // Reverse `[ln + 1, rn - 1]`
        Splay(ln, 0);
        Splay(rn, ln);
        sn[sn[rn].ch[0]].lz ^= 1;
    }
};
SplayTree spt;
std::vector<DataNode> p;
int main()
{
    int n, res, a, b;
    while(scanf("%d", &n) != EOF)
    {
        spt.Init();
        p.resize(n + 2);
        for(int i = 1; i <= n; i ++)
            scanf("%d", &p[i].val), p[i].ith = i;
        p[0].val = -0x3f3f3f3f;
        p[n + 1].val = 0x3f3f3f3f;
        spt.rt = spt.Build(0, n + 2, 0, p);
        std::sort(p.begin(), p.end());
        for(int i = 1; i <= n; i ++)
        {
            spt.Splay(p[i].pos);
            int ith = spt.sn[spt.sn[spt.rt].ch[0]].sz;
            if(i > 1) printf(" ");
            printf("%d", ith);
            int rn = spt.Nxt();
            spt.ReverseByNode(p[i - 1].pos, rn);
        }
        printf("\n");
    }
    return 0;
}
```

