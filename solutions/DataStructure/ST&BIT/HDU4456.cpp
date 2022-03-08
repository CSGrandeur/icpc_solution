// difficulty: 2
// Crowd
// 比较直观的解是二维树状数组或线段树
// 处理两个问题：1. 坐标旋转45度； 2. 数据范围10^4不能暴力开
// 只有80000操作，可以动态开结点建线段树，写成四象限的"线段树"
// 也可以离散化做二维树状数组

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const int maxn = 1e5 + 10;
const int maxm = maxn << 4;
const int minco = -1e4 - 210;
const int maxco = 1e4 + 210;
const int limitco = 1e4;
int dr[4][maxm], val[maxm];
int tp;
int n, m, p, x, y, z;
inline void cvtxy(int &x, int &y)
{
    int t = x + y - limitco;
    y = y - x;
    x = t;
}
void InitNode(int qd, int now)
{
    if(dr[qd][now] == -1)
    {
        val[tp] = 0;
        dr[qd][now] = tp ++;
    }
}
void Update(int now, int v, int left, int right, int up, int down, int x, int y)
{
    if(left == right || up == down)
        return;
    val[now] += v;
    if(left == right - 1 && up == down - 1)
        return;
    int lrmid = left + right >> 1;
    int udmid = up + down >> 1;
    if(x < lrmid && y < udmid) InitNode(0, now), Update(dr[0][now], v, left, lrmid, up, udmid, x, y);
    if(x < lrmid && y >= udmid) InitNode(1, now), Update(dr[1][now], v, left, lrmid, udmid, down, x, y);
    if(x >= lrmid && y < udmid) InitNode(2, now), Update(dr[2][now], v, lrmid, right, up, udmid, x, y);
    if(x >= lrmid && y >= udmid) InitNode(3, now), Update(dr[3][now], v, lrmid, right, udmid, down, x, y);
}
int Query(int now, int left, int right, int up, int down, int sl, int sr, int su, int sd)
{
    if(now == -1 || left == right || up == down)
        return 0;
    if(sl <= left && sr >= right && su <= up && sd >= down)
        return val[now];
    int lrmid = left + right >> 1;
    int udmid = up + down >> 1;
    int res = 0;
    if(sl < lrmid && su < udmid) res += Query(dr[0][now], left, lrmid, up, udmid, sl, sr, su, sd);
    if(sl < lrmid && sd >= udmid) res += Query(dr[1][now], left, lrmid, udmid, down, sl, sr, su, sd);
    if(sr >= lrmid && su < udmid) res += Query(dr[2][now], lrmid, right, up, udmid, sl, sr, su, sd);
    if(sr >= lrmid && sd >= udmid) res += Query(dr[3][now], lrmid, right, udmid, down, sl, sr, su, sd);
    return res;
}
int main()
{
    while(scanf("%d%d", &n, &m) != EOF && n)
    {
        memset(dr, -1, sizeof(dr));
        val[0] = 0;
        tp = 1;
        while(m --)
        {
            scanf("%d%d%d%d", &p, &x, &y, &z);
            cvtxy(x, y);
            if(p == 1)
                Update(0, z, minco, maxco, minco, maxco, x, y);
            else
                printf("%d\n", Query(0, minco, maxco, minco, maxco, x - z, x + z + 1, y - z, y + z + 1));
        }
    }
    return 0;
}