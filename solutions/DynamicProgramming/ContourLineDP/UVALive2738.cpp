// difficulty: 4
// Postal Vans
// 插头DP入门3：路径而非回路
// 此题强行增加难度需要高精度整数。
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<vector>
const int hashmod = 1e4 + 3;
const int hashnum = hashmod;

struct BigInt
{
    typedef std::vector<long long> VCT;
    VCT x;
    const int unt = 1e8;
    void Init(){x.clear();}
    BigInt(){}
    BigInt(const long long &a){Add(a);}
    BigInt(const int &a){Add(a);}
    BigInt(const BigInt &b){x = b.x;}
    BigInt(VCT &v){x = v;}
    void Add(const long long &a)
    {
        long long cur = a;
        for(auto &it : x)
        {
            it += cur;
            cur = it / unt;
            it = it % unt;
        }
        while(cur) x.push_back(cur % unt), cur /= unt;
    }
    BigInt &operator+=(long long a){Add(a); return *this;}
    BigInt operator+(long long a){BigInt tmp(x); tmp += a; return tmp;}
    BigInt &operator=(long long a){x.clear(); Add(a); return *this;}
    void Add(const BigInt &b)
    {
        int i;
        long long cur = 0;
        const VCT &y = b.x;
        if(x.size() < y.size()) x.resize(y.size(), 0);
        for(i = 0; i < x.size(); i ++)
        {
            x[i] += cur + (i < y.size() ? y[i] : 0);
            cur = x[i] / unt;
            x[i] %= unt;
        }
        while(cur) x.push_back(cur % unt), cur /= unt;
    }
    BigInt &operator+=(const BigInt &b){Add(b); return *this;}
    BigInt operator+(const BigInt &b){BigInt tmp(x); tmp += b; return tmp;}
    BigInt &operator=(const BigInt &b){x = b.x; return *this;}
    operator long long(){return x[0];}
    void Print()
    {
        if(x.size() == 0) {printf("0"); return; }
        for(int i = x.size() - 1; i >= 0; i --)
            printf(i == x.size() - 1 ? "%lld" : "%08lld", x[i]);
    }
    void Print3c()
    {
        if(x.size() == 0) {printf("0"); return; }
        char *buf = new char[x.size() * 8 + 1], *p = buf;
        for(int i = 0, j = x.size() - 1; i < x.size(); i ++, j --)
            p += sprintf(p, i ? "%08lld" : "%lld", x[j]);
        for(int i = strlen(buf) - 1, j = 0; buf[j]; j ++, i --)
            printf(",%c" + (i % 3 != 2 || !j), buf[j]);
        delete []buf;
    }
};
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
    ~HashTable(){delete []ht; delete []rcd;}
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
    MPTP_V &operator[](MPTP_K x)
    {
        MPTP_V z = 5;
        return insert(x).second;
    }
};
typedef HashTable<int, BigInt> DPMAP;
const int maxn = 13;
int n, m, lasti, lastj;
DPMAP dp[2];
char buf[maxn];
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
inline void ADP(DPMAP &mp, int k, const BigInt &v)
{
    if(!mp.count(k)) mp[k] = 0;
    mp[k] += v;
}
inline void LineShift(DPMAP &nowmp, DPMAP &nexmp)
{
    for(int i = 0; i < nowmp.sz; i ++)
        nexmp[nowmp.ht[i].first << 2 & (1 << (m + 1 << 1)) - 1] = nowmp.ht[i].second;
}
inline bool Blocked(int i, int j) {return false;}
inline bool End(int i, int j) {return i == n - 1 && j == m - 1;}
BigInt CLDP()
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
    return dp[now].count(0) ? dp[now][0] : BigInt(0);
}
int main()
{
    m = 4;
    while(scanf("%s", buf) != EOF && buf[0] != '#')
    {
        sscanf(buf, "%d", &n);
        BigInt res = CLDP();
        printf("%d: ", n);
        (res + res).Print3c();
        printf("\n");
    }
    return 0;
}
