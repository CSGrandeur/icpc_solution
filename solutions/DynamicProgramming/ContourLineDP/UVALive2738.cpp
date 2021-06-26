// difficulty: 4
// Postal Vans
// 插头DP入门2：单回路个数，双向都计数
// 此题强行增加难度需要高精度整数。

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

struct BigInt
{
    typedef std::vector<long long> VCT;
    VCT x;
    const int unt = 1e8;
    void Init(){x.clear();}
    BigInt(){}
    BigInt(const long long &a){Set(a);}
    BigInt(const int &a){Set(a);}
    BigInt(const BigInt &b){x = b.x;}
    BigInt(VCT &v){x = v;}
	void Set(const long long &a)
	{
		long long cur = a;
		while(cur) x.push_back(cur % unt), cur /= unt;
	}
    BigInt &operator+=(long long a){Add(a); return *this;}
    BigInt operator+(long long a){BigInt tmp(x); tmp += a; return tmp;}
    BigInt &operator=(long long a){x.clear(); Set(a); return *this;}
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
typedef int CODET;
typedef BigInt ANST;
typedef HashTable<CODET, ANST> DPMAP;
const CODET PLIMIT = 2;         // 独立插头数量限制
const int PL = 2;               // 插头编号需要的二进制位数，Plug Length
const CODET PM = (1 << PL) - 1; // 插头编号掩码，Plug Mask
const int maxn = 11;
int n, m, lasti, lastj;
DPMAP dp[2];
inline int GS(CODET k, int i) {return k >> (i * PL) & PM;}
inline CODET SS(CODET k, int i, CODET sta) {return k = (k & ~(PM << (i * PL))) | (sta << (i * PL));}
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
    mp[k] += v;
}
inline void LineShift(DPMAP &nowmp, DPMAP &nexmp)
{
    CODET CM = (1L << (m + 1) * PL) - 1;    // 编码掩码
    CODET NM = PM << (m + 1) * PL;          // 插头个数掩码
    for(auto it : nowmp)
        nexmp[it.first << PL & CM | it.first & NM] = nowmp[it.first];
}
inline bool Blocked(int i, int j) {return i < 0 || i >= n || j < 0 || j >= m;}
inline bool End(int i, int j) {return i == n - 1 && j == m - 1;}
ANST CLDP()
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
            for(auto it : dp[now])
            {
                CODET k = it.first;
                int up = GS(k, j + 1), left = GS(k, j);
                if(Blocked(i, j))
                {
                    if(!left && !up)
                        UD(dp[nex], k, it.second);
                }
                else if(!left && !up)
                {
                    if(!Blocked(i, j + 1) && !Blocked(i + 1, j))
                        UD(dp[nex], SS(SS(k, j, 1), j + 1, 2), it.second);
                }
                else if(!left + !up == 1)
                {
                    CODET maxul = std::max(up, left), minul = std::min(up, left);
                    if(!Blocked(i, j + 1))
                        UD(dp[nex], SS(SS(k, j, minul), j + 1, maxul), it.second);
                    if(!Blocked(i + 1, j))
                        UD(dp[nex], SS(SS(k, j, maxul), j + 1, minul), it.second);
                }
                else if(left == up)
                {
                    if(left == 1)
                        UD(dp[nex], SS(SS(SS(k, j, 0), j + 1, 0), LS(k, j + 1), 1), it.second);
                    else
                        UD(dp[nex], SS(SS(SS(k, j, 0), j + 1, 0), LS(k, j), 2), it.second);
                }
                else if(left == 2 || End(i, j))
                    UD(dp[nex], SS(SS(k, j, 0), j + 1, 0), it.second);
            }
            now ^= 1, nex ^= 1;
            dp[nex].clear();
        }
    }
    CODET rescode = 0;
    // 此题双向算两个，结果翻倍
    return dp[now].count(rescode) ? dp[now][rescode] + dp[now][rescode] : ANST(0);
}
int main()
{
    m = 4;
    while(scanf("%d", &n) != EOF && n != '#')
    {
        printf("%d: ", n);
        
        CLDP().Print3c();
        printf("\n");
    }
    return 0;
}
