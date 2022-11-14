### `Construct the Great Wall`

> difficulty: 5

UVA1501, HDU4113, UVALive5762 同一题

用网格线包围"o"，把"x"隔离在外，求最短包围框。

基于格子DP难度太大，可基于格点DP。

操作单位：格点，n行m列的图，有n+1行、m+1列的格点

当前格子：当前格点右下方的格子，格点 j 的格子也为 j，编号从 0 开始，最后一个格点 m 右下方没有格子（地图外）

Cl：j-1为当前格子左侧，j为左上，j+1为上方， 0 表示在包围圈外，1 表示在内，当前格子为当前格点右下方的格子

Sr：插头编号

转移逻辑：插头由格点发向另一个格点，被插头分隔的两个方块在包围圈内外标记相反，

根据当前格子是"o"、"x"、"."，以及左上方格子的内外标记，决定是否发当前要发的插头

增加圈标记。连接编号相同的左+上插头表示生成一个圈，至多只能生成一个圈。

这个代码UVA能过，HDU没过。网上搜到两个HDU AC 代码，生成数据对拍了下，两个AC代码应该是错的。参考数据：

```cpp
/*
Input:
6
3 4
ox.o
...x
....
3 4
oxoo
...x
....
3 4
ox.o
...x
xxxx
4 3
...
x..
xo.
o..
4 4
.xo.
oxo.
ox.o
...x
4 3
xo.
.x.
.o.
...

Output:
Case #1: 14
Case #2: 14
Case #3: 14
Case #4: 8
Case #5: 20
Case #6: 12
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<unordered_map>
typedef long long CODET;
typedef long long ANST;
const int maxn = 11;
typedef std::unordered_map<CODET, ANST> DPMAP;
const int PL = 5;               // 连通块编码 Plug Lenth，编号 + 颜色01标记
const int PLCL = 1;             // 编码中的 Color 标记位长度
const CODET PM = ((CODET)1 << PL) - 1;
int n, m;
DPMAP dp[2];
char dg[maxn][maxn];
struct CLNode
{
    std::vector<int> cl, sr;
    int rc;
    CLNode(){Init();}
    void Init() {cl.resize(maxn); sr.resize(maxn);}
    int Cl(int j) {return j < 0 || j >= m ? 0 : cl[j];} // m 已+1，故原图格子数为m-1
    int Sr(int j) {return sr[j];}
    CLNode& SetRc(int _rc) {rc = _rc; return *this;}    // 设置统计位rc，比如多少个独立插头
    CODET Encode()
    {
        Recode();  // 最小表示默认recode
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
    {
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
CLNode cn, tn;
inline void UD(DPMAP &mp, CODET k, const ANST &v)
{
    if(!mp.count(k)) mp[k] = 0x3f3f3f3f;
    mp[k] = std::min(mp[k], v);
    if(k == 0 && v == 14)
        int x = 1;
}
inline void LineShift(DPMAP &nowmp, DPMAP &nexmp, int i, int j)
{
    CODET CM = (1L << (m + 1) * PL) - 1; 
    CODET NM = PM << (m + 1) * PL;
    for(auto it : nowmp)
        UD(nexmp, it.first << PL & CM | it.first & NM, it.second);
}
inline bool Blocked(int i, int j) {return i < 0 || i >= n || j < 0 || j >= m;}
inline bool End(int i, int j) {return i == n - 1 && j == m - 1;}
inline bool JudgeInclude(char icon, int io)
{
    return icon != 'x' && icon != 'o' || icon == 'x' && io || icon == 'o' && !io;
}
void DPTrans(int i, int j, CODET k, ANST v, DPMAP &nexmp)
{
    tn.Decode(k);
    int lt = tn.Sr(j), up = tn.Sr(j + 1), luio = tn.Cl(j);
    char icon = dg[i][j];
    // if(i == 3 && j == 2)
    // {
    //     if(v == 15)
    //     int x = 1;
    // }
    if(tn.rc)
    {
        if(!lt && !up && JudgeInclude(icon, !luio))
            UD(nexmp, cn.Decode(k).Set(j, 0, luio).Set(j + 1, 0).Encode(), v);
    }
    else if(!lt && !up)
    {
        if(!Blocked(i + 1, j + 1) && JudgeInclude(icon, luio) && !tn.rc)
            UD(nexmp, cn.Decode(k).Set(j, PM, !luio).Set(j + 1, PM).Encode(), v + 2);
        if(JudgeInclude(icon, !luio))
            UD(nexmp, cn.Decode(k).Set(j, 0, luio).Set(j + 1, 0).Encode(), v);
    }
    else if(!lt + !up == 1)
    {
        int refCl = lt ? luio : !luio, mx = std::max(lt, up);
        if(!Blocked(i, j + 1) && JudgeInclude(icon, refCl))
            UD(nexmp, cn.Decode(k).Set(j, 0, !refCl).Set(j + 1, mx).Encode(), v + 1);
        if(!Blocked(i + 1, j) && JudgeInclude(icon, !refCl))
            UD(nexmp, cn.Decode(k).Set(j, mx, refCl).Set(j + 1, 0).Encode(), v + 1);
    }
    else if(lt && up)
    {
        if(JudgeInclude(icon, luio))
            UD(nexmp, cn.Decode(k).Merge(j, j + 1).Set(j, 0, !luio).Set(j + 1, 0).SetRc(lt == up ? 1 : 0).Encode(), v);
    }
}
ANST CLDP()
{
    int now = 0, nex = 1;
    dp[0].clear(); dp[1].clear();
    dp[0][0] = 0;
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
    CODET rescode = cn.Decode(0).SetRc(1).Encode();
    return dp[now].count(rescode) ? dp[now][rescode] : -1;
}

int main()
{
    int t, cs = 1;
    for(scanf("%d", &t); cs <= t; cs ++)
    {
        if(scanf("%d%d", &n, &m) == EOF) break;
        for(int i = 0; i < n; i ++)
            scanf("%s", dg[i]);
        memset(dg[n], 0, sizeof(dg[n]));
        n ++, m ++;
        printf("Case #%d: %lld\n", cs, CLDP());
    }
    return 0;
}
```

