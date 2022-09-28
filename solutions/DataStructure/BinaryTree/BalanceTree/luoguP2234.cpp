// difficulty: 2
// [HNOI2002]营业额统计
// 可作为Splay的 Pre、Next 练习

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
};
SplayTree spt;
int main()
{
    int n, res, a;
    while(scanf("%d", &n) != EOF)
    {
        spt.Init();
        res = 0;
        for(int i = 0; i < n; i ++)
        {
            scanf("%d", &a);
            spt.Insert(a);
            if(!i) res += a;
            else 
            {
                int anode = spt.rt, pre = spt.Pre(anode), nxt = spt.Nxt(anode), tmp = 0x3f3f3f3f;
                if(spt.sn[anode].cnt > 1) continue;
                if(pre) tmp = std::min(abs(spt.sn[pre].val - a), tmp);
                if(nxt) tmp = std::min(abs(spt.sn[nxt].val - a), tmp);
                res += tmp;
            }
        }
        printf("%d\n", res);
    }
    return 0;
}