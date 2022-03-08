// difficulty: 4
// Rocket Mania
// 插头DP应用。
// 给出左端第 X 行有火柴，中间各种形状可旋转导线，求最多能点燃右端的行数。
// 把图旋转90度处理，初始化第 X 个为 1 号插头，插头最小表示时保持 1 号不变。
// 最终状态求插头编号为 1 的个数。
// 关键剪枝：1)去掉没有1插头的状态；2)把没点燃且在轮廓线上没有连通插头的孤立插头置0，因为这种插头不影响最终结果。
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<vector>
#include<unordered_map>
const int maxn = 11;
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

typedef long long CODET;
typedef int ANST;
int n, m, srow;
typedef HashTable<CODET, ANST> DPMAP;
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
    CODET Encode();
    CLNode& Decode(CODET k);                            // 兼容"只有插头编号"和"有编号与颜色两个域"的编码，设置正确的PLCL（颜色域位宽）即可
    CLNode& Recode();                                   // 给插头重编号，Recode
    CLNode& Merge(int ith, int jth);                    // 合并插头
    CLNode& Set(int ith, int _sr=-1, int _cl=-1);       // 无颜色域时忽略 _cl 参数即可
    void Optimize();
};
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
        // 剪枝没有点燃的且轮廓线上无相连插头的孤立插头
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
CLNode cn;
DPMAP dp[2];
char dg[maxn][maxn];
inline void UD(DPMAP &mp, CODET k, const ANST &v)
{
    mp[k] = v;
}
inline void LineShift(DPMAP &nowmp, DPMAP &nexmp)
{
    CODET CM = (1L << (m + 1) * PL) - 1;    // 编码掩码
    CODET NM = PM << (m + 1) * PL;          // 插头个数/额外计数位 掩码。一些题用不到，不过保持此操作不妨碍结果
    for(auto it : nowmp)
        UD(nexmp, it.first << PL & CM | it.first & NM, it.second);
}
inline bool Blocked(int i, int j) {return i < 0 || i >= n || j < 0 || j >= m;}
inline bool End(int i, int j) {return i == n - 1 && j == m - 1;}
bool JudgeValid(CODET k)
{
    cn.Decode(k);
    for(int i = 0; i <= m; i ++)
        if(cn.Sr(i) == 1) return true;
    return false;
}
void DPTrans(int i, int j, CODET k, ANST v, DPMAP &nexmp)
{
    if(!JudgeValid(k)) return;
    cn.Decode(k);
    int left = cn.Sr(j), up = cn.Sr(j + 1);
    switch(dg[j][i])
    {
        case '.':
            UD(nexmp, cn.Set(j, 0).Set(j + 1, 0).Encode(), v);
            break;
        case '-':
            UD(nexmp, cn.Set(j, 0).Set(j + 1, left ? left : PM).Encode(), v);
            UD(nexmp, cn.Decode(k).Set(j, up ? up : PM).Set(j + 1, 0).Encode(), v);
            break;
        case 'L':
            if(left && up)
            {
                if(left == 1) UD(nexmp, cn.Merge(j, j + 1).Set(j, 0).Set(j + 1, 0).Encode(), v);
                else UD(nexmp, cn.Decode(k).Merge(j + 1, j).Set(j, 0).Set(j + 1, 0).Encode(), v);
            }
            if(left) UD(nexmp, cn.Decode(k).Set(j, left).Set(j + 1, 0).Encode(), v);
            if(up) UD(nexmp, cn.Decode(k).Set(j, 0).Set(j + 1, up).Encode(), v);
            UD(nexmp, cn.Decode(k).Set(j, PM).Set(j + 1, PM).Encode(), v);
            break;
        case 'T':
            if(left && up)
            {
                if(left == 1)
                {
                    UD(nexmp, cn.Decode(k).Merge(j, j + 1).Set(j, left).Set(j + 1, 0).Encode(), v);
                    UD(nexmp, cn.Decode(k).Merge(j, j + 1).Set(j, 0).Set(j + 1, left).Encode(), v);
                }
                else
                {
                    UD(nexmp, cn.Decode(k).Merge(j + 1, j).Set(j, up).Set(j + 1, 0).Encode(), v);
                    UD(nexmp, cn.Decode(k).Merge(j + 1, j).Set(j, 0).Set(j + 1, up).Encode(), v);
                }
            }
            if(left) UD(nexmp, cn.Decode(k).Set(j, left).Set(j + 1, left).Encode(), v);
            UD(nexmp, cn.Decode(k).Set(j, up ? up : PM).Set(j + 1, up ? up : PM).Encode(), v);
            break;
        case '+': 
            if(!left && !up)
                UD(nexmp, cn.Set(j, PM).Set(j + 1, PM).Encode(), v);
            else if(!left && up)
                UD(nexmp, cn.Set(j, up).Set(j + 1, up).Encode(), v);
            else if(left && !up)
                UD(nexmp, cn.Set(j, left).Set(j + 1, left).Encode(), v);
            else if(left == 1)
                UD(nexmp, cn.Merge(j, j + 1).Set(j, left).Set(j + 1, left).Encode(), v);
            else
                UD(nexmp, cn.Merge(j + 1, j).Set(j, up).Set(j + 1, up).Encode(), v);
    }
}
ANST CLDP()
{
    int now = 0, nex = 1;
    dp[0].clear(); dp[1].clear();
    dp[0][cn.Decode(0).Set(srow, 1).Encode()] = 1;
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
    ANST ans = 0, cnt;
    for(auto it : dp[now])
    {
        cnt = 0;
        cn.Decode(it.first);
        for(int i = 0; i < m; i ++) cnt += cn.Sr(i) == 1;
        ans = std::max(ans, cnt);
    }
    return ans;
}
int main()
{
    n = 6, m = 9;   // 此题从左向右dp
    while(scanf("%d", &srow) != EOF)
    {
        srow --;
        for(int i = 0; i < m; i ++)
            scanf("%s", dg[i]);
        printf("%d\n", CLDP());
    }
    return 0;
}
