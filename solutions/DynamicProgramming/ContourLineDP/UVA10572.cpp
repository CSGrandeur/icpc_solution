// difficulty: 5
// Black & White
// 插头DP入门5：连通块（广义路径）
// 网格图染色方案数，部分网格已设定颜色，染色需保证黑与黑连通，白与白连通，没有任何2*2局部颜色相同。
// 1. 设定：`o` 为 0， `#` 为 1， 边界外颜色用特判设为 -1， -1 的连通块编码为 0
// 2. 轮廓线： 0~m 共 m+1 个，对于处理 j 时， j - 1 为左边格子， j 为左上格子， j + 1 为上方格子
// 3. 转移： 每个格子，如果数据没限定符号，则颜色 0 和颜色 1 都尝试一次
//  对于尝试的颜色：
//  1) 左、上 与尝试颜色一致时，判断左上是否也一致，避免2*2相同。能行就直接连通， Merge 左 与 上 的连通块编号
//  2) 左、上 其中一个与尝试颜色一致，与一致的颜色连通
//  3) 左、上 都与尝试颜色不同，则考虑开新的连通块，但当当前块是右下最后一个，且左上与尝试颜色一致时，死角不能开新块，否则会有不连通的相同色块。
// 4. 状态合法性：转移状态计算好之后，不立刻转移，而要判断转移后的状态是否合法，是否把上方的连通块封死：
//  前提：尝试颜色与上方颜色不同，否则不会封死上方连通块
//  1) 轮廓线上存在与上方颜色相同，但连通块编号不同的块，则封死上方块后，上方连通块与其他同色连通块无法再连通
//  2) 轮廓线没有与上方颜色相同的连通块，但当前块位置不在最后一列的后两格，这样后续为避免2*2同色，必然在右下方出现新的与上方颜色相同的块，产生不连通同色区域
// 如果转移状态存在不合法情况，则不进行状态转移
// 5. 答案计算：
// 最终块处理结束后的所有状态中，轮廓线不超过两个连通块的状态为合法状态，统计这些状态的答案总和
// 6. 答案构造：
// 过程中用哈希记录状态的转移，就可以倒推构造一个答案

// 代码实现上，可以纯位运算，也可以在每一次状态转移做编码解码把状态还原到数组里操作。编解码的方式可读性更强也更便于调试。

// 编解码方式版代码

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
const int maxn = 11;
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
    if(!upsr) return true;          // 编码为 0 则当前在第一行，"上一行"没有东西，没约束
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
        // 左与上 与当前块拟涂色不同色，特判右下"墙角"情况不能开新块
        if(End(i, j) && lucl == color) return;
        cn.Set(j, PM >> PLCL, color).Recode();
    }
    if(!JudgeValid(i, j, color)) return;
    UD(nexmp, cn.Encode(), v, i, j, k);
}
void PrintAns(CODET validcode)
{
    CODET nowcode = validcode;
    for(int i = n - 1; i >= 0; i --)
    {
        for(int j = m - 1; j >= 0; j --)
        {
            dg[i][j] = cn.Decode(nowcode).Cl(j) ? '#' : 'o';
            nowcode = pre[i][j][nowcode];
        }
        nowcode = pre[i][m][nowcode];  // LineShift时候多记了一步
    }
    for(int i = 0; i < n; i ++)
        puts(dg[i]);
    printf("\n");
}
ANST CLDP()
{
    int now = 0, nex = 1;
    dp[0].clear(); dp[1].clear();
    dp[0][0] = 1;
    for(int i = 0; i < n; i ++)
    {
        LineShift(dp[now], dp[nex], i, m);
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
int main()
{
    int t;
    for(scanf("%d", &t); t --; )
    {
        for(int i = 0; i < n; i ++)
            for(int j = 0; j <= m; j ++)
                pre[i][j].clear();
        scanf("%d%d", &n, &m);
        for(int i = 0; i < n; i ++)
            scanf("%s", dg[i]);
        CLDP();
    }
    return 0;
}


// 纯位运算代码
/*
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_map>
const int maxn = 11;
typedef long long CODET;
typedef long long ANST;
typedef std::unordered_map<CODET, ANST> DPMAP;
std::unordered_map<CODET, CODET> pre[maxn][maxn];    // 记录上一步编码
const int PL = 5;               // 连通块编码 Plug Lenth，编号 + 颜色01标记
const int PLCL = 1;             // 编码中的 Color 标记位长度
const CODET PM = (1 << PL) - 1; // 插头编号掩码，Plug Mask
int n, m;
DPMAP dp[2];
char dg[maxn][maxn];
inline int GLEN(CODET k, int i, int len) {return k >> i & ((CODET)1 << len) - 1;}
inline int GS(CODET k, int i) {return i < 0 ? -1 : GLEN(k, i * PL, PL);}
inline int GSCL(CODET k, int i) {return i < 0 ? -1 : GLEN(k, i * PL, PLCL);}
inline int GSSR(CODET k, int i) {return i < 0 ? -1 : GLEN(k, i * PL + PLCL, PL - PLCL);}
inline CODET SLEN(CODET k, int i, int len, CODET sta) {return k & ~(((CODET)1 << len) - 1 << i) | sta << i;}
inline CODET SS(CODET k, int i, CODET sta) {return SLEN(k, i * PL, PL, sta);}
inline CODET SSCL(CODET k, int i, CODET cl) {return SLEN(k, i * PL, PLCL, cl);}
inline CODET SSSR(CODET k, int i, CODET sr) {return SLEN(k, i * PL + PLCL, PL - PLCL, sr);}
inline CODET RC(CODET k)
{  // 给插头重编号，Recode
    std::vector<int> rc(PM + 1, -1);
    CODET dc, rctp = 1;
    for(int i = 0; i <= m; i ++)
    {
        if(!(dc = GSSR(k, i))) continue;
        if(rc[dc] == -1) rc[dc] = rctp ++;
        k = SSSR(k, i, rc[dc]);
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
bool JudgeValid(CODET nk, int i, int j, int color)
{  // 判断当前轮廓线是否合法
    int upcl = GSCL(nk, j + 1), upsr = GSSR(nk, j + 1);
    if(color == upcl) return true;  // 如果新块颜色与 up 相同，说明 up 这个连通块还没堵死
    if(!upsr) return true;          // 编码为 0 则当前在第一行，"上一行"没有东西，没约束
    int cntS = 0, cntC = 0;
    for(int o = 0; o <= m; o ++)
    {
        if(o == j + 1) continue;
        cntS += GSSR(nk, o) == upsr;     // 轮廓线有与 up 编号相同（即相连通）的块
        cntC += GSCL(nk, o) == upcl;     // 轮廓线有与 up 颜色相同的块
    }
    if(cntS) return true;
    if(cntC) return false;              // 有与 up 编号不同但颜色相同的块，此时封死 up 则 up 连通块与同色其他块无法再连通，结果会有多个连通块
    return i >= n - 1 && j >= m - 2;    // 没有与 up 同色块，此时如果右下方还有足够空间，未来为避免 2*2 同色则一定会出现新的不连通的同色块
}
void DPTrans(int i, int j, CODET k, ANST v, int color, DPMAP &nexmp)
{
    int lt = GS(k, j - 1), lu = GS(k, j), up = GS(k, j + 1);
    int ltcl = GSCL(k, j - 1), lucl = GSSR(k, j) ? GSCL(k, j) : -1, upcl = GSSR(k, j + 1) ? GSCL(k, j + 1) : -1;
    CODET nk;
    if(ltcl == color && upcl == color)
    {
        // 左与上 与当前块拟涂色同色，且左上不同色，合并连通块
        if(lucl == color) return;
        nk = RC(MG(SS(k, j, lt), j - 1, j + 1));
    }
    else if(ltcl == color || upcl == color)
        // 左与上 其中一个与当前块拟涂色同色，合并同色
        nk = SS(k, j, ltcl == color ? lt : up);
    else
    {
        // 左与上 与当前块拟涂色不同色，特判右下"墙角"情况不能开新块
        if(End(i, j) && lucl == color) return;
        nk = RC(SSCL(SS(k, j, PM), j, color));
    }
    if(!JudgeValid(nk, i, j, color)) return;
    UD(nexmp, nk, v, i, j, k);
}
void PrintAns(CODET validcode)
{
    CODET nowcode = validcode;
    for(int i = n - 1; i >= 0; i --)
    {
        for(int j = m - 1; j >= 0; j --)
        {
            dg[i][j] = GSCL(nowcode, j) ? '#' : 'o';
            nowcode = pre[i][j][nowcode];
        }
        nowcode = pre[i][m][nowcode];  // LineShift时候多记了一步
    }
    for(int i = 0; i < n; i ++)
        puts(dg[i]);
    printf("\n");
}
ANST CLDP()
{
    int now = 0, nex = 1;
    dp[0].clear(); dp[1].clear();
    dp[0][0] = 1;
    for(int i = 0; i < n; i ++)
    {
        LineShift(dp[now], dp[nex], i, m);
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
            maxblk = std::max(maxblk, GSSR(it.first, i));   // 这里基于最大编号，使用之前要确保 RC 过，即重编号为1、2、3...
        if(maxblk <= 2) validcode = it.first, ans += it.second;
    }
    printf("%lld\n", ans);
    if(validcode != -1) PrintAns(validcode);
    return ans;
}
int main()
{
    int t;
    for(scanf("%d", &t); t --; )
    {
        for(int i = 0; i < n; i ++)
            for(int j = 0; j <= m; j ++)
                pre[i][j].clear();
        scanf("%d%d", &n, &m);
        for(int i = 0; i < n; i ++)
            scanf("%s", dg[i]);
        CLDP();
    }
    return 0;
}
*/