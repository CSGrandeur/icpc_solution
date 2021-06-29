# 经典动态规划

## 最长下降子序列（LDS）

`O(nlogn)`

```cpp
#include<functional>
#include<algorithm>
const int maxn = 1100000;
int dp[maxn], tp;
int LDS(int a[], int n)
{
    tp = 0;
    for(int i = 0; i < n; i ++)
    {
        int dpIth = 0;
        if(tp == -1 || dp[tp] > a[i])
            dpIth = ++ tp;
        else
            // 求上升子序列时去掉 `std::greater<int>()`
            dpIth = std::upper_bound(dp, dp + tp + 1, a[i], std::greater<int>()) - dp;
        dp[dpIth] = a[i];
    }
    return tp + 1;
}
```

## 插头DP

插头DP类型多样，重点在于正确分类讨论和决策方式。

典型类型可分为 多回路、单回路、可转为回路的路径、真简单路径等。

这里提供单回路、真简单路径 两个示例

### 变量定义与操作封装

```cpp
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
{  // 状态转移，可能有：算权值取max，算个数则累加
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
```

### 单回路计数

UVALive 2738

```cpp
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
    return dp[now].count(rescode) ? dp[now][rescode] : ANST(0);
}
```

### 简单路径算最优权值和

ZOJ 3213

```cpp
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
```

### 广义路径（染色问题）

UVA10572

```cpp
typedef long long CODET;
typedef long long ANST;
typedef std::unordered_map<CODET, ANST> DPMAP;
std::unordered_map<CODET, CODET> pre[maxn][maxn];    // 记录上一步编码
const int PL = 5;               // 连通块编码 Plug Lenth，编号 + 颜色01标记
const int PLCL = 1;             // 编码中的 Color 标记位长度
const CODET PM = ((CODET)1 << PL) - 1;
int n, m;
DPMAP dp[2];
char dg[maxn][maxn];
struct CLNode
{
    // m 为列数，使用全局变量
    std::vector<int> cl, sr;
    int rc;
    CLNode(){Init();}
    void Init() {cl.resize(maxn); sr.resize(maxn);}
    int Cl(int j) {return j < 0 || !sr[j] ? -1 : cl[j];}
    int Sr(int j) {return j < 0 ? -1 : sr[j];}
    CODET Encode()
    {
        CODET res = rc;
        for(int i = m; i >= 0; i --)
            res = res << PL | sr[i] << PLCL | cl[i];
        return res;
    }
    CLNode &Decode(CODET k)
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
    CLNode &Roll()
    {  // 换行后调整轮廓线坐标对应
        for(int i = 1; i <= m; i ++)
            cl[i] = cl[i - 1], sr[i] = sr[i - 1];
        cl[0] = sr[0] = 0;
        return *this;
    }
    CLNode &Recode()
    {  // 给插头重编号，Recode
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
    CLNode &Merge(int ith, int jth)
    {  // 合并插头
        int ithsr = sr[ith];
        for(int i = 0; i <= m; i ++)
            if(sr[i] == ithsr) sr[i] = sr[jth];
        return *this;
    }
    CLNode &Set(int ith, int _sr=-1, int _cl=-1)
    {
        if(_sr != -1) sr[ith] = _sr;
        if(_cl != -1) cl[ith] = _cl;
        return *this;
    }
};
CLNode cn;
inline void UD(DPMAP &mp, CODET k, const ANST &v, int i, int j, CODET prek)
{  // 执行状态转移
    if(!mp.count(k)) mp[k] = 0;
    mp[k] += v;
    pre[i][j][k] = prek;
}
inline void LineShift(DPMAP &nowmp, DPMAP &nexmp, int i, int j)
{
    CODET CM = (1L << (m + 1) * PL) - 1;    // 编码掩码
    CODET NM = PM << (m + 1) * PL;          // 插头个数/额外计数位 掩码。一些题用不到，不过保持此操作不妨碍结果
    for(auto it : nowmp)
        UD(nexmp, it.first << PL & CM | it.first & NM, it.second, i, j, it.first);
}
inline bool End(int i, int j) {return i == n - 1 && j == m - 1;}
bool JudgeValid(int i, int j, int color)
{  // 判断当前轮廓线是否合法
    int upcl = cn.Cl(j + 1), upsr = cn.Sr(j + 1);
    if(color == upcl) return true;  // 如果新块颜色与 up 相同，说明 up 这个连通块还没堵死
    if(!upsr) return true;          // 编码为 0 则当前在第一行，“上一行”没有东西，没约束
    int cntS = 0, cntC = 0;
    for(int o = 0; o <= m; o ++)
    {
        if(o == j + 1) continue;
        cntS += cn.Sr(o) == upsr;     // 轮廓线有与 up 编号相同（即相连通）的块
        cntC += cn.Cl(o) == upcl;     // 轮廓线有与 up 颜色相同的块
    }
    if(cntS) return true;
    if(cntC) return false;              // 有与 up 编号不同但颜色相同的块，此时封死 up 则 up 连通块与同色其他块无法再连通，结果会有多个连通块
    return i >= n - 1 && j >= m - 2;    // 没有与 up 同色块，此时如果右下方还有足够空间，未来为避免 2*2 同色则一定会出现新的不连通的同色块
}
void DPTrans(int i, int j, CODET k, ANST v, int color, DPMAP &nexmp)
{
    cn.Decode(k);
    int ltcl = cn.Cl(j - 1), lucl = cn.Cl(j), upcl = cn.Cl(j + 1);
    int ltsr = cn.Sr(j - 1), lusr = cn.Sr(j), upsr = cn.Sr(j + 1);
    if(ltcl == color && upcl == color)
    {
        // 左与上 与当前块拟涂色同色，且左上不同色，合并连通块
        if(lucl == color) return;
        cn.Set(j, ltsr, color).Merge(j - 1, j + 1).Recode();
    }
    else if(ltcl == color || upcl == color)
        // 左与上 其中一个与当前块拟涂色同色，合并同色
        cn.Set(j, ltcl == color ? ltsr : upsr, color);
    else
    {
        // 左与上 与当前块拟涂色不同色，特判右下“墙角”情况不能开新块
        if(End(i, j) && lucl == color) return;
        cn.Set(j, PM >> PLCL, color).Recode();
    }
    if(!JudgeValid(i, j, color)) return;
    UD(nexmp, cn.Encode(), v, i, j, k);
}
ANST CLDP()
{
    int now = 0, nex = 1;
    dp[0].clear(); dp[1].clear();
    dp[0][0] = 1;
    for(int i = 0; i < n; i ++)
    {
        LineShift(dp[now], dp[nex], i, 0);
        now ^= 1, nex ^= 1, dp[nex].clear();
        for(int j = 0; j < m; j ++)
        {
            for(auto it : dp[now])
            {
                if(dg[i][j] != '#')
                    DPTrans(i, j, it.first, it.second, 0, dp[nex]);
                if(dg[i][j] != 'o')
                    DPTrans(i, j, it.first, it.second, 1, dp[nex]);
            }
            now ^= 1, nex ^= 1, dp[nex].clear();
        }
    }
    ANST ans = 0;
    CODET validcode = -1;
    for(auto it : dp[now])
    {
        // 右下最后一个处理完后，没有进行LineShift，有效块是 [0, m-1]，对应最后一行
        // 根据这一行可以判断现存多少个连通块，只有连通块不大于 2 的状态为合法状态
        int maxblk = 0;
        for(int i = 0; i < m; i ++)
            maxblk = std::max(maxblk, cn.Decode(it.first).Sr(i));   // 这里基于最大编号，使用之前要确保 RC 过，即重编号为1、2、3...
        if(maxblk <= 2) validcode = it.first, ans += it.second;
    }
    printf("%lld\n", ans);
    if(validcode != -1) PrintAns(validcode);
    return ans;
}
```