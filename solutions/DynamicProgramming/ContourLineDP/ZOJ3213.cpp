// difficulty: 5
// Beautiful Meadow
// 插头DP入门4：真-简单路径（“简单路径”是路径类型，不是说题简单）
// 此题要求为简单路径求最优权值，不一定要走所有块，无法构造成回路问题。
// 定义一个概念：独立插头，即当前情况下，没有穿过轮廓线的插头数量。
// 单路径不可能在轮廓线之上存在超过 2 个独立插头。
// 将独立插头添加至状态中，其他状态为穿过轮廓线的插头编号，轮廓线上至多 m 种插头。
// 描述编号不会超过 4 个二进制位，插头DP的问题规模决定了 long long 存的下所有状态。
// 其余就针对生成插头、合并插头分类讨论即可。
// 本题初始为 dp[0][0] = 0 表示一开始没有分数。
// 答案为最后一个块计算后，有 2 个独立插头的、轮廓线上无插头 的状态下的结果。

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<vector>
const int hashmod = 1e4 + 3;
const int hashnum = 3e4 + 10;

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
void Print2(CODET k) { while(k) printf("%lld", k & 1), k >>= 1; printf(" ****\n");}
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
