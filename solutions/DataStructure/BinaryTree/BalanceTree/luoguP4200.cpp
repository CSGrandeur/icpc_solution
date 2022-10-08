// difficulty: 4
// 千山鸟飞绝
// Splay
// 每个坐标建一棵树，维护树上最大威武值，lazy标记士气值和团结值。
// 因为自己不能鼓励自己，所以每次插入之前先用树上信息更新该鸟，再插入。
// 插入后splay到树根添加lazy值，此lazy值不影响插入的鸟自己。
// 要注意初始位置就可以有位置相同的鸟，所以没到调度阶段已经需要作如上处理。

#include<cstdio>
#include<algorithm>
#include<vector>
#include<string>
#include<map>
struct Bird
{
    int w, x, y, sq, tj;
    void Init(){w = x = y = sq = tj = 0;}
    void Update(int lzs, int lzt) {sq = std::max(sq, lzs); tj = std::max(tj, lzt);}
};
std::vector<Bird> bd;

struct SplayNode
{
    int ch[2], fa, val, cnt, sz, mx, lzs, lzt;
    void Init() {ch[0] = ch[1] = fa = val = cnt = sz = mx = lzs = lzt = 0;}
    SplayNode(){Init();}
};
std::vector<SplayNode> sn;

struct SplayTree
{
    int rt;
    void Init(){rt = 0;}
    SplayTree(){Init();}
    void Maintain(int x)
    {
        sn[x].sz = sn[sn[x].ch[0]].sz + sn[sn[x].ch[1]].sz + sn[x].cnt;
        sn[x].mx = std::max(std::max(sn[sn[x].ch[0]].mx, sn[sn[x].ch[1]].mx), bd[sn[x].val].w);
    }
    void UpdateLz(int x, int lzs, int lzt)
    {
        if(!x) return;
        sn[x].lzs = std::max(sn[x].lzs, lzs);
        sn[x].lzt = std::max(sn[x].lzt, lzt);
        bd[sn[x].val].Update(lzs, lzt);
    }
    void PushDown(int x)
    {
        if(x && sn[x].lzs)
        {
            UpdateLz(sn[x].ch[0], sn[x].lzs, sn[x].lzt);
            UpdateLz(sn[x].ch[1], sn[x].lzs, sn[x].lzt);
            sn[x].lzs = sn[x].lzt = 0;
        }
    }
    int Ws(int x) {return x == sn[sn[x].fa].ch[1];}
    void Clear(int x) {sn[x].Init();}
    void Rotate(int x)
    {
        int y = sn[x].fa, z = sn[y].fa;
        // PushDown(y), PushDown(x);
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
    int Add(int k, int fa=0)
    {
        int nn = sn.size();
        sn.push_back(SplayNode());
        sn[nn].val = k;
        sn[nn].cnt ++;
        sn[nn].fa = fa;
        sn[nn].mx = bd[k].w;
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
            PushDown(cur);
            if(sn[cur].val == k)
            {
                sn[cur].cnt ++;
                Maintain(cur);
                Maintain(f);
                Splay(cur);
                return;
            }
            f = cur;
            cur = sn[cur].ch[sn[cur].val < k];
            if(!cur)
            {
                int nn = Add(k, f);
                Splay(nn);
                return;
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
    int RtPre(int cur=0)
    {
        if(!cur) cur = sn[rt].ch[0];
        else cur = sn[cur].ch[0];
        if(!cur) return cur;
        while(sn[cur].ch[1]) {cur = sn[cur].ch[1];}
        Splay(cur);
        return cur;
    }
    void Del(int k)
    {
        Rank(k);
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
std::map<std::string, int> mp;
std::vector<SplayTree> spt;
char buf[110];
inline int Mp(int x, int y)
{
    sprintf(buf, "%d_%d", x, y);
    if(!mp.count(buf)) 
    {
        mp[buf] = spt.size();
        spt.push_back(SplayTree());
    }
    return mp[buf];
}
int main()
{
    int n, t, v, x, y;
    sn.reserve((int)4e5);
    while(scanf("%d", &n) != EOF)
    {
        sn.clear(); sn.push_back(SplayNode());
        spt.clear(); mp.clear(); bd.resize(n + 10);
        for(int i = 1; i <= n; i++)
        {
            bd[i].Init();
            scanf("%d%d%d", &bd[i].w, &bd[i].x, &bd[i].y);
            int jthspt = Mp(bd[i].x, bd[i].y);
            bd[i].Update(sn[spt[jthspt].rt].mx, sn[spt[jthspt].rt].sz);
            spt[jthspt].Insert(i);
            sn[spt[jthspt].rt].lzs = bd[i].w;
            sn[spt[jthspt].rt].lzt = sn[spt[jthspt].rt].sz - 1;
        }
        scanf("%d", &t);
        while(t --)
        {
            scanf("%d%d%d", &v, &x, &y);
            int ithspt = Mp(bd[v].x, bd[v].y), jthspt = Mp(x, y);
            spt[ithspt].Del(v);
            bd[v].Update(sn[spt[jthspt].rt].mx, sn[spt[jthspt].rt].sz);
            bd[v].x = x, bd[v].y = y;
            spt[jthspt].Insert(v);
            sn[spt[jthspt].rt].lzs = bd[v].w;
            sn[spt[jthspt].rt].lzt = sn[spt[jthspt].rt].sz - 1;
        }
        for(int i = 1; i <= n; i ++)
        {
            int ithspt = Mp(bd[i].x, bd[i].y);
            spt[ithspt].Rank(i);
            printf("%lld\n", 1LL * bd[i].sq * bd[i].tj);
        }
        
    }
    return 0;
}