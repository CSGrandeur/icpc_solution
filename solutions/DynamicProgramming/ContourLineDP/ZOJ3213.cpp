// difficulty: 5
// Beautiful Meadow
// 插头DP入门4：真-简单路径（"简单路径"是路径类型，不是说题简单）
// 此题要求为简单路径求最优权值，不一定要走所有块，无法构造成回路问题。
// 定义一个概念：独立插头，即当前情况下，没有穿过轮廓线的插头数量。
// 单路径不可能在轮廓线之上存在超过 2 个独立插头。
// 将独立插头添加至状态中，其他状态为穿过轮廓线的插头编号，轮廓线上至多 m 种插头。
// 描述编号不会超过 4 个二进制位，插头DP的问题规模决定了 long long 存的下所有状态。
// 其余就针对生成插头、合并插头分类讨论即可。
// 本题初始为 dp[0][0] = 0 表示一开始没有分数。
// 答案为最后一个块计算后，有 2 个独立插头的、轮廓线上无插头 的状态下的结果。

// 编解码版代码：
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<vector>
#include<unordered_map>
const int maxn = 11;

typedef long long CODET;
typedef long long ANST;
int n, m;
typedef std::unordered_map<CODET, ANST> DPMAP;
const CODET PLIMIT = 2;         // 独立插头数量限制
const int PL = 4;               // 连通块编码 Plug Lenth，编号 + 颜色01标记
const int PLCL = 0;             // 编码中的 Color 标记位长度
const CODET PM = ((CODET)1 << PL) - 1;

struct CLNode
{
    // m 为列数，使用全局变量
    std::vector<int> cl, sr;
    int rc;
    CLNode(){Init();}
    void Init() {rc = 0; cl.resize(maxn); sr.resize(maxn);}
    int Cl(int j) {return j < 0 || !sr[j] ? -1 : cl[j];}
    int Sr(int j) {return j < 0 ? -1 : sr[j];}
    CLNode& SetRc(int _rc) {rc = _rc; return *this;}    // 设置统计位rc，比如多少个独立插头
    int GetLink(int j);                                 // 获取插头另一头，括号式成对插头时使用，Link State
    CODET Encode();
    CLNode& Decode(CODET k);                            // 兼容"只有插头编号"和"有编号与颜色两个域"的编码，设置正确的PLCL（颜色域位宽）即可
    CLNode& Recode();                                   // 给插头重编号，Recode
    CLNode& Merge(int ith, int jth);                    // 合并插头
    CLNode& Set(int ith, int _sr=-1, int _cl=-1);       // 无颜色域时忽略 _cl 参数即可

};
int CLNode::GetLink(int j)
{
    int di = sr[j] == 1 ? 1 : -1, cnt = 0;
    for(int i = j; i >= 0 && i <= m; i += di)
    {
        if(sr[i] == 1) cnt ++;
        else if(sr[i] == 2) cnt --;
        if(!cnt) return i;
    }
    return -1;
}
CODET CLNode::Encode()
{
    CODET res = rc;
    for(int i = m; i >= 0; i --)
        res = res << PL | (CODET)sr[i] << PLCL | cl[i];
    return res;
}
CLNode& CLNode::Decode(CODET k)
{
    for(int i = 0; i <= m; i ++, k >>= PL)
    {
        sr[i] = k & PM;
        cl[i] = sr[i] & (1 << PLCL) - 1;
        sr[i] >>= PLCL;
    }
    rc = k;
    return *this;
}
CLNode& CLNode::Recode()
{
    std::vector<int> rc(PM + 1, -1);
    int rctp = 1;
    for(int i = 0; i <= m; i ++)
    {
        if(!sr[i]) continue;
        if(rc[sr[i]] == -1) rc[sr[i]] = rctp ++;
        sr[i] = rc[sr[i]];
    }
    return *this;
}
CLNode& CLNode::Merge(int ith, int jth)
{
    int ithsr = sr[ith];
    for(int i = 0; i <= m; i ++)
        if(sr[i] == ithsr) sr[i] = sr[jth];
    return *this;
}
CLNode& CLNode::Set(int ith, int _sr, int _cl)
{
    if(_sr != -1) sr[ith] = _sr;
    if(_cl != -1) cl[ith] = _cl;
    return *this;
}
CLNode cn;
DPMAP dp[2];
int dg[maxn][maxn];

inline void UD(DPMAP &mp, CODET k, const ANST &v)
{  // 执行状态转移
    if(!mp.count(k)) mp[k] = 0;
    mp[k] = std::max(mp[k], v);
}
inline void LineShift(DPMAP &nowmp, DPMAP &nexmp)
{
    CODET CM = (1L << (m + 1) * PL) - 1;    // 编码掩码
    CODET NM = PM << (m + 1) * PL;          // 插头个数/额外计数位 掩码。一些题用不到，不过保持此操作不妨碍结果
    for(auto it : nowmp)
        UD(nexmp, it.first << PL & CM | it.first & NM, it.second);
}
inline bool Blocked(int i, int j) {return i < 0 || i >= n || j < 0 || j >= m || !dg[i][j];}
inline bool End(int i, int j) {return i == n - 1 && j == m - 1;}
void DPTrans(int i, int j, CODET k, ANST v, DPMAP &nexmp)
{
    cn.Decode(k);
    int left = cn.Sr(j), up = cn.Sr(j + 1), pnum = cn.rc;
    ANST nw = v + dg[i][j];
    if(Blocked(i, j))
    {
        if(!left && !up)
            UD(nexmp, k, v);
    }
    else if(!left && !up)
    {  // 如果左和上都没插头
        UD(nexmp, cn.Decode(0).SetRc(2).Encode(), dg[i][j]);                                // 单清当前草坪，不再移动的情况
        UD(nexmp, k, v);                                                                    // 保持上一步插头情况
        if(!Blocked(i + 1, j) && !Blocked(i, j + 1))
            UD(nexmp, cn.Decode(k).Set(j, PM).Set(j + 1, PM).Recode().Encode(), nw);        // 增加双插头
        if(pnum == PLIMIT) return;
        if(!Blocked(i + 1, j))
            UD(nexmp, cn.Decode(k).Set(j, PM).SetRc(pnum + 1).Recode().Encode(), nw);       // 新增向下插头，则该位置增加独立插头
        if(!Blocked(i, j + 1))
            UD(nexmp, cn.Decode(k).Set(j + 1, PM).SetRc(pnum + 1).Recode().Encode(), nw);   // 新增向右插头，则该位置增加独立插头
    }
    else if(!left + !up == 1)
    {
        CODET maxul = std::max(up, left), minul = std::min(up, left);
        if(!Blocked(i, j + 1))
            UD(nexmp, cn.Set(j, minul).Set(j + 1, maxul).Encode(), nw);
        if(!Blocked(i + 1, j))
            UD(nexmp, cn.Set(j, maxul).Set(j + 1, minul).Encode(), nw);
        if(pnum == PLIMIT) return;
        UD(nexmp, cn.Set(j, 0).Set(j + 1, 0).SetRc(pnum + 1).Encode(), nw);  // 插头终止
    }
    else if(left != up)
        UD(nexmp, cn.Merge(j, j + 1).Set(j, 0).Set(j + 1, 0).Encode(), nw);
}
ANST CLDP()
{
    int now = 0, nex = 1;
    dp[0].clear(); dp[1].clear();
    dp[0][0] = 0;
    for(int i = 0; i < n; i ++)
    {
        LineShift(dp[now], dp[nex]);
        now ^= 1, nex ^= 1;
        dp[nex].clear();
        for(int j = 0; j < m; j ++)
        {
            for(auto it : dp[now])
                DPTrans(i, j, it.first, it.second, dp[nex]);
            now ^= 1, nex ^= 1, dp[nex].clear();
        }
    }
    CODET rescode = cn.Decode(0).SetRc(PLIMIT).Encode();
    return dp[now].count(rescode) ? dp[now][rescode] : 0;
}
int main()
{
    int t;
    for(scanf("%d", &t); t --; )
    {
        scanf("%d%d", &n, &m);
        for(int i = 0; i < n; i ++)
            for(int j = 0; j < m; j ++)
                scanf("%d", &dg[i][j]);
        printf("%lld\n", CLDP());
    }
    return 0;
}

// 位运算版代码
/*
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<vector>
const int hashmod = 1e4 + 3;

template<typename MPTP_K, typename MPTP_V>
struct HashTable
{
    struct HashNode
    {
        MPTP_K first;
        MPTP_V second;
        int nex;
    };
    std::vector<HashNode> ht;
    std::vector<int> rcd;
    HashTable(){rcd.resize(hashmod); Init();}
    ~HashTable(){}
    typename std::vector<HashNode>::iterator begin(){return ht.begin();}
    typename std::vector<HashNode>::iterator end(){return ht.end();}
    void Init(){ht.clear(); std::fill(rcd.begin(), rcd.end(), -1);}
    void clear() {Init();}
    int size() {return ht.size();}
    int GetHash(MPTP_K x) {return (x ^ hashmod) % hashmod;}
    int Find(MPTP_K x)
    {
        for(int i = rcd[GetHash(x)]; i != -1; i = ht[i].nex)
            if(ht[i].first == x) return i;
        return -1;
    }
    int _Insert(MPTP_K x, MPTP_V v=0)
    {
        int hs = GetHash(x);
        HashNode tmp;
        tmp.nex = rcd[hs];
        tmp.first = x;
        tmp.second = v;
        ht.push_back(tmp);
        return rcd[hs] = ht.size() - 1;
    }
    HashNode &insert(MPTP_K x, MPTP_V v=0)
    {
        int ith = Find(x);
        return ht[ith == -1 ? _Insert(x, v) : ith];
    }
    bool count(MPTP_K x) {return Find(x) != -1;}
    MPTP_V &operator[](MPTP_K x){return insert(x).second;}
};
typedef long long CODET;
typedef long long ANST;
typedef HashTable<CODET, ANST> DPMAP;
const CODET PLIMIT = 2;         // 独立插头数量限制
const int PL = 4;               // 插头编号需要的二进制位数，Plug Length
const CODET PM = (1 << PL) - 1; // 插头编号掩码，Plug Mask
const int maxn = 11;
int n, m, lasti, lastj;
DPMAP dp[2];
int dg[maxn][maxn];
inline int GS(CODET k, int i) {return k >> (i * PL) & PM;}
inline CODET SS(CODET k, int i, CODET sta) {return k = (k & ~(PM << (i * PL))) | (sta << (i * PL));}
inline CODET RC(CODET k)
{  // 给插头重编号，Recode
    std::vector<int> rc(PM + 1, -1);
    CODET dc, rctp = 1;
    for(int i = 0; i <= m; i ++)
    {
        if(!(dc = GS(k, i))) continue;
        if(rc[dc] == -1) rc[dc] = rctp ++;
        k = SS(k, i, rc[dc]);
    }
    return k;
}
inline CODET MG(CODET k, int i, int j)
{  // 合并两个插头编号，Merge
    CODET jk = GS(k, j), ik = GS(k, i);
    for(int o = 0; o <= m; o ++)
        if(GS(k, o) == jk) k = SS(k, o, ik);
    return k;
}
inline int LS(CODET k, int i)
{  // 获取插头另一头，括号式成对插头时使用，Link State
    int di = GS(k, i) == 1 ? 1 : -1, cnt = 0;
    for(int j = i; j >= 0 && j <= m; j += di)
    {
        if(GS(k, j) == 1) cnt ++;
        else if(GS(k, j) == 2) cnt --;
        if(!cnt) return j;
    }
    return -1;
}
inline void UD(DPMAP &mp, CODET k, const ANST &v)
{
    if(!mp.count(k)) mp[k] = 0;
    mp[k] = std::max(mp[k], v);
}
inline void LineShift(DPMAP &nowmp, DPMAP &nexmp)
{
    CODET CM = (1L << (m + 1) * PL) - 1;    // 编码掩码
    CODET NM = PM << (m + 1) * PL;          // 插头个数掩码
    for(auto it : nowmp)
        nexmp[it.first << PL & CM | it.first & NM] = nowmp[it.first];
}
inline bool Blocked(int i, int j) {return i < 0 || i >= n || j < 0 || j >= m || !dg[i][j];}
inline bool End(int i, int j) {return i == n - 1 && j == m - 1;}
ANST CLDP()
{
    int now = 0, nex = 1;
    dp[0].clear(); dp[1].clear();
    dp[0][0] = 0;
    for(int i = 0; i < n; i ++)
    {
        LineShift(dp[now], dp[nex]);
        now ^= 1, nex ^= 1;
        dp[nex].clear();
        for(int j = 0; j < m; j ++)
        {
            for(auto it : dp[now])
            {
                CODET k = it.first;
                // Print2(k);
                int up = GS(k, j + 1), left = GS(k, j), pnum = GS(k, m + 1);
                ANST nw = it.second + dg[i][j];
                if(Blocked(i, j))
                {
                    if(!left && !up)
                        UD(dp[nex], k, it.second);
                }
                else if(!left && !up)
                {
                    UD(dp[nex], SS(0, m + 1, 2), dg[i][j]);            // 单清当前草坪，不再移动的情况
                    UD(dp[nex], k, it.second);                         // 保持上一步插头情况
                    if(!Blocked(i + 1, j) && !Blocked(i, j + 1))
                        UD(dp[nex], RC(SS(SS(k, j, PM), j + 1, PM)), nw);  // 增加双插头
                    if(pnum == PLIMIT) continue;
                    if(!Blocked(i + 1, j))
                        UD(dp[nex], RC(SS(SS(k, j, PM), m + 1, pnum + 1)), nw);  // 增加单插头
                    if(!Blocked(i, j + 1))
                        UD(dp[nex], RC(SS(SS(k, j + 1, PM), m + 1, pnum + 1)), nw);  // 增加单插头
                }
                else if(!left + !up == 1)
                {
                    CODET maxul = std::max(up, left), minul = std::min(up, left);
                    if(!Blocked(i, j + 1))
                        UD(dp[nex], SS(SS(k, j, minul), j + 1, maxul), nw);
                    if(!Blocked(i + 1, j))
                        UD(dp[nex], SS(SS(k, j, maxul), j + 1, minul), nw);
                    // UD(dp[nex], k, nw);
                    if(pnum == PLIMIT) continue;
                    UD(dp[nex], SS(SS(SS(k, j, 0), j + 1, 0), m + 1, pnum + 1), nw);  // 插头终止
                }
                else if(left != up)
                    UD(dp[nex], SS(SS(MG(k, j, j + 1), j, 0), j + 1, 0), nw);
            }
            now ^= 1, nex ^= 1;
            dp[nex].clear();
        }
    }
    CODET rescode = PLIMIT << (m + 1) * PL;
    return dp[now].count(rescode) ? dp[now][rescode] : 0;
}
int main()
{
    int t;
    for(scanf("%d", &t); t --; )
    {
        scanf("%d%d", &n, &m);
        for(int i = 0; i < n; i ++)
            for(int j = 0; j < m; j ++)
                scanf("%d", &dg[i][j]);
        printf("%lld\n", CLDP());
    }
    return 0;
}
*/