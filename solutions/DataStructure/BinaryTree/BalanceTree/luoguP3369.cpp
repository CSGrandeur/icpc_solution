// difficulty: 2
// 【模板】普通平衡树
// 模板题

#include<cstdio>
#include<algorithm>
#include<vector>
#include<queue>

struct SplayNode
{
    int ch[2], fa, val, cnt, sz;
    void Init() {ch[0] = ch[1] = fa = val = cnt = sz = 0;}
    SplayNode(){Init();}
};
std::vector<SplayNode> sn;  // 注意预留 sn[0] 为空节点。 sn.clear(), sn.push_back(SplayNode());
struct SplayTree
{
    int rt, tot;
    void Maintain(int x)
    {
        sn[x].sz = sn[sn[x].ch[0]].sz + sn[sn[x].ch[1]].sz + sn[x].cnt;
    }
    int Sonlr(int x) {return x == sn[sn[x].fa].ch[1];}
    void Clear(int x) {sn[x].Init();}
    void Rotate(int x)
    {
        int y = sn[x].fa, z = sn[y].fa, chk = Sonlr(x);
        sn[y].ch[chk] = sn[x].ch[chk ^ 1];
        if(sn[x].ch[chk ^ 1]) sn[sn[x].ch[chk ^ 1]].fa = y;
        sn[x].ch[chk ^ 1] = y;
        sn[y].fa = x;
        sn[x].fa = z;
        if(z) sn[z].ch[y == sn[z].ch[1]] = x;
        Maintain(x);
        Maintain(y);
    }
    void Splay(int x)
    {
        for(int f = sn[x].fa; f = sn[x].fa, f; Rotate(x))
            if(sn[f].fa) Rotate(Sonlr(x) == Sonlr(f) ? f : x);
        rt = x;
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
    int Kth(int k)
    {
        int cur = rt;
        while(true)
        {
            if(sn[cur].ch[0] && k <= sn[sn[cur].ch[0]].sz)
                cur = sn[cur].ch[0];
            else
            {
                k -= sn[cur].cnt + sn[sn[cur].ch[0]].sz;
                if(k <= 0)
                {
                    Splay(cur);
                    return sn[cur].val;
                }
                cur = sn[cur].ch[1];
            }
        }
        return 0;
    }
    int RtPre()
    {
        int cur = sn[rt].ch[0];
        if(!cur) return cur;
        while(sn[cur].ch[1]) cur = sn[cur].ch[1];
        Splay(cur);
        return cur;
    }
    int RtNxt()
    {
        int cur = sn[rt].ch[1];
        if(!cur) return cur;
        while(sn[cur].ch[0]) cur = sn[cur].ch[0];
        Splay(cur);
        return cur;
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
        int cur = rt, x = RtPre();
        sn[sn[cur].ch[1]].fa = x;
        sn[x].ch[1] = sn[cur].ch[1];
        Clear(cur);
        Maintain(rt);
    }
};
SplayTree spt;
int main()
{
    int n, opt, x;
    while(scanf("%d", &n) != EOF)
    {
        sn.clear();
        sn.push_back(SplayNode());
        for(int i = 0; i < n; i ++)
        {
            scanf("%d%d", &opt, &x);
            if(opt == 1) spt.Insert(x);
            else if(opt == 2) spt.Del(x);
            else if(opt == 3) printf("%d\n", spt.Rank(x));
            else if(opt == 4) printf("%d\n", spt.Kth(x));
            else if(opt == 5)
                spt.Insert(x), printf("%d\n", sn[spt.RtPre()].val), spt.Del(x);
            else    
                spt.Insert(x), printf("%d\n", sn[spt.RtNxt()].val), spt.Del(x);
        }
    }
    return 0;
}