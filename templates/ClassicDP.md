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