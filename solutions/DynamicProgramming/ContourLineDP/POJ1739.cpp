// difficulty: 4
// Tony's Tour
// 插头DP入门3：路径而非回路
// 构造为单回路问题：最后增加两行，一行只有首尾可通过，一行全部可通行，将路径转换为回路问题
// POJ 目前不支持 C++ 11，所以自定义哈希的for(auto...)改为直接访问内部的ht[]了。
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int hashmod = 1e3 + 3;
const int hashnum = hashmod;
template<typename MPTP_K, typename MPTP_V>
struct HashTable
{
    struct HashNode
    {
        MPTP_K first;
        MPTP_V second;
        int nex;
    };
    HashNode *ht;
    int *rcd;
    int sz;
    HashTable(){ht = new HashNode[hashnum]; rcd = new int[hashmod]; Init();}
    ~HashTable(){delete ht; delete rcd;}
    HashNode *begin(){return ht;}
    HashNode *end(){return ht + sz;}
    void Init(){sz = 0; memset(rcd, -1, sizeof(int) * hashmod);}
    void clear() {Init();}
    int size() {return sz;}
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
        ht[sz].nex = rcd[hs];
        ht[sz].first = x;
        ht[sz].second = v;
        rcd[hs] = sz;
        return sz ++;
    }
    HashNode &insert(MPTP_K x, MPTP_V v=0) 
    {
        int ith = Find(x);
        return ht[ith == -1 ? _Insert(x, v) : ith];
    }
    bool count(MPTP_K x) {return Find(x) != -1;}
    MPTP_V &operator[](MPTP_K x){return insert(x).second;}
};
typedef HashTable<int, long long> DPMAP;
const int maxn = 13;
int n, m, lasti, lastj;
DPMAP dp[2];
char buf[maxn][maxn];
inline int GS(int k, int i) {return k >> (i << 1) & 3;}
inline int SS(int k, int i, int sta) {return k = (k & ~(3 << (i << 1))) | (sta << (i << 1));}
inline int LS(int k, int i)
{
    int di = GS(k, i) == 1 ? 1 : -1, cnt = 0;
    for(int j = i; j >= 0 && j <= m; j += di)
    {
        if(GS(k, j) == 1) cnt ++;
        else if(GS(k, j) == 2) cnt --;
        if(!cnt) return j;
    }
    return -1;
}
inline void ADP(DPMAP &mp, int k, long long v)
{
    if(!mp.count(k)) mp[k] = 0;
    mp[k] += v;
}
inline void LineShift(DPMAP &nowmp, DPMAP &nexmp)
{
    for(int i = 0; i < nowmp.sz; i ++)
        nexmp[nowmp.ht[i].first << 2 & (1 << (m + 1 << 1)) - 1] = nowmp.ht[i].second;
}
inline bool Blocked(int i, int j) {return buf[i][j] == '#';}
inline bool End(int i, int j) {return i == n - 1 && j == m - 1;}
long long CLDP()
{
    int now = 0, nex = 1;
    dp[0].clear(); dp[1].clear();
    dp[0][0] = 1;
    for(int i = 0; i < n; i ++)
    {
        LineShift(dp[now], dp[nex]);
        now ^= 1, nex ^= 1;
        dp[nex].clear();
        for(int j = 0; j < m; j ++)
        {
            for(int it = 0; it < dp[now].sz; it ++)
            { 
                int k = dp[now].ht[it].first;
                int up = GS(k, j + 1), left = GS(k, j);
                if(LS(k, j) == -1 || LS(k, j + 1) == -1) continue;
                if(Blocked(i, j))
                {
                    if(!left && !up)
                        ADP(dp[nex], k, dp[now][k]);
                }
                else if(!left && !up) 
                    ADP(dp[nex], SS(SS(k, j, 1), j + 1, 2), dp[now][k]);
                else if(!left + !up == 1) 
                {
                    ADP(dp[nex], SS(SS(k, j, up), j + 1, left), dp[now][k]);
                    ADP(dp[nex], k, dp[now][k]);
                }
                else if(left == 1 && up == 1)
                    ADP(dp[nex], SS(SS(SS(k, j, 0), j + 1, 0), LS(k, j + 1), 1), dp[now][k]);
                else if(left == 1 && up == 2 && End(i, j))
                    ADP(dp[nex], SS(SS(k, j, 0), j + 1, 0), dp[now][k]);
                else if(left == 2 && up == 1)
                    ADP(dp[nex], SS(SS(k, j, 0), j + 1, 0), dp[now][k]);
                else if(left == 2 && up == 2)
                    ADP(dp[nex], SS(SS(SS(k, j, 0), j + 1, 0), LS(k, j), 2), dp[now][k]);
            }
            now ^= 1, nex ^= 1;
            dp[nex].clear();
        }
    }
    return dp[now].count(0) ? dp[now][0] : 0;
}
int main()
{
    while(scanf("%d%d", &n, &m) && (n || m))
    {
        for(int i = 0; i < n; i ++)
            scanf("%s", buf[i]);
        memset(buf[n], '#', sizeof(buf[n]));
        buf[n][0] = buf[n][m - 1] = '.';
        n ++;
        memset(buf[n ++], '.', sizeof(buf[n]));
        printf("%lld\n", CLDP());

    }
    return 0;
}