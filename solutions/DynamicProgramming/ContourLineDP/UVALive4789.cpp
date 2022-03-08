// difficulty: 5
// Channel
// ICPC2010 World Final 的 E 题，官方题解评价为该场比赛最难一题。
// "This problem has a somewhat standard, but very complicated, dynamic programming solution, making it one of the hardest problems in the set."
// 题目基调是一个从左上到右下、有障碍的路径问题，但给了一个很特别的要求：
// 路径不可以和自己之前的部分存在八连通，更严谨地说，可以是每个格子不能与沿着路径两步之外的格子有八连通，因为毕竟拐弯什么的总会挨着的。
// 官方题解是确定一行的 C、哪些C是路径端点、端点之间的连通情况，以这三个信息推下一行。
// 暂时没想好官方的解法怎么做，这次用插头DP做的。
// 这题麻烦在单用路径模型似乎无法判断题目要求的禁止路径八连通，而用染色模型又无法保证单一路径。
// 路径+染色 两个信息做标记，状态转移时为避免八连通，做的判断十分烧脑，大体思路是：
// 对上插头转右插头、左插头发右插头之前，判断右上的两个格子是否有下插头、下插头是否与当前插头编号一样（避免环）、是否是C等。
// 对左插头转下插头、上插头发下插头之前，判断左边是否有下插头、右上格子等等。
// 对于无插头，要发一个向右向下的转角插头前，把前两种情况结合判断一下。
// 细节画一画能理清，条件应该还可以简化。
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<vector>
#include<unordered_map>
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
const int maxn = 22;
typedef long long CODET;
typedef long long ANST;
int n, m;
typedef HashTable<CODET, ANST> DPMAP;  // 可换HashTable模板加速
const int PL = 5;                               // 连通块编码 Plug Lenth，编号 + 颜色01标记
const int PLCL = 1;                             // 编码中的 Color 标记位长度，不存在该域则设为0
const CODET PM = ((CODET)1 << PL) - 1;
HashTable<CODET, CODET> pre[maxn][maxn];
struct CLNode
{
    // m 为列数，使用全局变量
    std::vector<int> cl, sr;
    int rc;
    CLNode(){Init();}
    void Init() {rc = 0; cl.resize(maxn); sr.resize(maxn);}
    int Cl(int j) {return j < 0 || j > m ? 0 : cl[j];}
    int Sr(int j) {return j < 0 ? 0 : (j > m ? 0 : sr[j]);}
    CLNode& SetRc(int _rc) {rc = _rc; return *this;}    // 设置统计位rc，比如多少个独立插头
    int GetLink(int j);                                 // 获取插头另一头，括号式成对插头时使用，Link State
    CODET Encode();
    CLNode& Decode(CODET k);                            // 兼容"只有插头编号"和"有编号与颜色两个域"的编码，设置正确的PLCL（颜色域位宽）即可
    CLNode& Recode();                                   // 给插头重编号，Recode
    CLNode& Merge(int ith, int jth);                    // 合并插头，所有 jth 改为 ith 的编号
    CLNode& Set(int ith, int _sr=-1, int _cl=-1);       // 无颜色域时忽略 _cl 参数即可
    void Optimize();
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
    Optimize();
    Recode();  // 最小表示默认recode
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
void CLNode::Optimize()
{
    std::vector<int> cnt(PM + 1, 0);
    for(int i = 0; i <= m; i ++)
        if(sr[i]) cnt[sr[i]] ++;
    for(int i = 0; i <= m; i ++)
        if(sr[i] != 1 && cnt[sr[i]] < 2)
            sr[i] = 0;
}
CLNode& CLNode::Recode()
{
    std::vector<int> rc(PM + 1, -1);
    int rctp = 2;
    for(int i = 0; i <= m; i ++)
    {
        if(!sr[i] || sr[i] == 1) continue;
        if(rc[sr[i]] == -1) rc[sr[i]] = rctp ++;
        sr[i] = rc[sr[i]];
    }
    return *this;
}
CLNode& CLNode::Merge(int ith, int jth)
{
    int jthsr = sr[jth];
    for(int i = 0; i <= m; i ++)
        if(sr[i] == jthsr) sr[i] = sr[ith];
    return *this;
}
CLNode& CLNode::Set(int ith, int _sr, int _cl)
{
    if(_sr != -1) sr[ith] = _sr;
    if(_cl != -1) cl[ith] = _cl;
    return *this;
}
CLNode cn, tn;
DPMAP dp[2];
char dg[maxn][maxn];

inline void UD(DPMAP &mp, CODET k, const ANST &v, int i, int j, CODET prek)
{
    if(!mp.count(k)) mp[k] = 0;
    if(v > mp[k]) 
        mp[k] = v, pre[i][j][k] = prek;
}
inline void LineShift(DPMAP &nowmp, DPMAP &nexmp, int i, int j)
{
    CODET CM = ((CODET)1 << (m + 1) * PL) - 1;    // 编码掩码
    CODET NM = PM << (m + 1) * PL;          // 插头个数/额外计数位 掩码。一些题用不到，不过保持此操作不妨碍结果
    for(auto it : nowmp)
        UD(nexmp, it.first << PL & CM | it.first & NM, it.second, i, j, it.first);
}
inline bool Blocked(int i, int j) {return i < 0 || i >= n || j < 0 || j >= m || dg[i][j] == '#';}
inline bool End(int i, int j) {return i == n - 1 && j == m - 1;}
bool JudgeValid(CODET k)
{
    cn.Decode(k);
    for(int i = 0; i <= m; i ++)
        if(cn.Sr(i) == 1) return true;
    return false;
}
bool JudgeRight(int i, int j, int lt, int up)
{
    return !Blocked(i, j + 1) &&
        (
            lt && !tn.Cl(j + 1) && (tn.Sr(j + 2) && tn.Sr(j + 2) != lt || !tn.Cl(j + 2) && tn.Sr(j + 3) && tn.Sr(j + 3) != lt || !tn.Cl(j + 2) && !tn.Cl(j + 3)) ||
            up && !tn.Cl(j + 2) && (tn.Sr(j + 3) && tn.Sr(j + 3) != up || !tn.Cl(j + 3))
        );
}
bool JudgeDown(int i, int j, int lt, int up)
{
    return !Blocked(i + 1, j) && !tn.Sr(j + 2) && !tn.Sr(j - 1) || End(i, j);
}
bool JudgeNewPlug(int i, int j, int lt, int up)
{
    return !Blocked(i + 1, j) && !Blocked(i, j + 1) && !tn.Cl(j) && !tn.Cl(j - 1) && !tn.Cl(j + 1) &&
        (tn.Sr(j + 2) || !tn.Cl(j + 2) && tn.Sr(j + 3) || !tn.Cl(j + 2) && !tn.Cl(j + 3));
}
void DPTrans(int i, int j, CODET k, ANST v, DPMAP &nexmp)
{
    if(!JudgeValid(k)) return;
    cn.Decode(k);
    tn.Decode(k);
    int lt = tn.Sr(j), up = tn.Sr(j + 1);
    if(Blocked(i, j))
    {
        if(!lt && !up)
            UD(nexmp, cn.Decode(k).Set(j, 0, 0).Set(j + 1, 0).Encode(), v, i, j, k);
    }
    else if(!lt && !up)
    {
        UD(nexmp, cn.Decode(k).Set(j, 0, 0).Set(j + 1, 0).Encode(), v, i, j, k);
        if(JudgeNewPlug(i, j, lt, up))
            UD(nexmp, cn.Decode(k).Set(j, PM, 1).Set(j + 1, PM).Encode(), v + 1, i, j, k);
    }
    else if(!lt + !up == 1)
    {
        CODET maxul = std::max(up, lt), minul = std::min(up, lt);
        if(JudgeRight(i, j, lt, up))
            UD(nexmp, cn.Decode(k).Set(j, minul, 1).Set(j + 1, maxul).Encode(), v + 1, i, j, k);
        if(JudgeDown(i, j, lt, up))
            UD(nexmp, cn.Decode(k).Set(j, maxul, 1).Set(j + 1, minul).Encode(), v + 1, i, j, k);
        
    }
    else if(lt != up)
    {
        if(lt == 1) UD(nexmp, cn.Merge(j, j + 1).Set(j, 0, 1).Set(j + 1, 0).Encode(), v + 1, i, j, k);
        else UD(nexmp, cn.Merge(j + 1, j).Set(j, 0, 1).Set(j + 1, 0).Encode(), v + 1, i, j, k);
    }
}
void PrintAns(CODET validcode)
{
    CODET nowcode = validcode;
    for(int i = n - 1; i >= 0; i --)
    {
        for(int j = m - 1; j >= 0; j --)
        {
            if(cn.Decode(nowcode).Cl(j)) dg[i][j] = 'C';
            nowcode = pre[i][j][nowcode];
        }
        nowcode = pre[i][m][nowcode];
    }
    for(int i = 0; i < n; i ++)
        puts(dg[i]);
    printf("\n");
}
ANST CLDP()
{
    int now = 0, nex = 1;
    dp[0].clear(); dp[1].clear();
    dp[0][cn.Decode(0).Set(0, 1).Encode()] = 0;
    for(int i = 0; i < n; i ++)
    {
        LineShift(dp[now], dp[nex], i, m);
        now ^= 1, nex ^= 1, dp[nex].clear();
        for(int j = 0; j < m; j ++)
        {
            for(auto it : dp[now])
                DPTrans(i, j, it.first, it.second, dp[nex]);
            now ^= 1, nex ^= 1, dp[nex].clear();
        }
    }
    ANST ans = 0;
    CODET validcode = 0;
    for(auto it : dp[now])
    {
        if(cn.Decode(it.first).Sr(m - 1) == 1)
            if(it.second > ans)
                ans = it.second, validcode = it.first;
    }
    PrintAns(validcode);
    return ans;
}
int main()
{
    int t = 1;
    while(scanf("%d%d", &n, &m) != EOF && n + m)
    {
        for(int i = 0; i < n; i ++)
        {
            scanf("%s", dg[i]);
            for(int j = 0; j <= m; j ++)
                pre[i][j].clear();
        }
        printf("Case %d:\n", t ++);
        CLDP();
    }
    return 0;
}