// difficulty: 4
// Musical Theme
// 无交叉最长相同子串。此题定义相同为允许加上或减去同一个值。
// 突破点为先处理为差分串，第一个数丢掉。然后正常求SA。
// 二分枚举最长子串。由于要求不交叉，在判断枚举值时需要在连续满足mid的sa中找距离最远的两个，举例不小于mid则为满足。
// 由于做了差分，最终求得的值需要再 +1 得到子串长度。
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<algorithm>
const int maxn = 2e5 + 10;
struct SA
{
    std::vector<int> wa, wb, wv, ws, sa, rk, height, r;
    int len, m;
    SA(){}
    SA(int maxn_){_Init(maxn_);}
    void _Init(int maxn_, bool dc3flag=false);
    inline int _DACMP(std::vector<int> &r, int a, int b, int l)
    {return r[a] == r[b] && r[a + l] == r[b + l];}
    template<typename STR_IN>
    void _GetR(STR_IN buf[], bool dc3flag=false)
    {
        _Init(std::max(len, m) + 1, dc3flag);
        for(int i = 0; i < len; i ++)
            r[i] = buf[i];
        r[len] = 0;
    }
    template<typename STR_IN>
    void CalSA(STR_IN buf[], int len_, int m_=128)
    {
        len = len_, m = m_;
        _GetR(buf);
        DA(len + 1, m);
    }
    // Cal rank or height Only valid when after GetSA(...)
    void CalRank();
    void CalHeight();
    void DA(int len, int m);
};
void SA::_Init(int maxn_, bool dc3flag)
{
    if(wa.size() <= maxn_)
    {
        r.resize(maxn_ + 1);
        wa.resize(maxn_ + 1);
        wb.resize(maxn_ + 1);
        wv.resize(maxn_ + 1);
        ws.resize(maxn_ + 1);
        sa.resize(maxn_ + 1);
        rk.resize(maxn_ + 1);
        height.resize(maxn_ + 1);
    }
    if(dc3flag && r.size() <= 3 * maxn_)
    {
        r.resize((len + 1) * 3);
        sa.resize((len + 1) * 3);
    }
}
void SA::DA(int n, int m)
{
    int i, j, p;
    std::vector<int> &x = wa, &y = wb;
    for(i = 0; i < m; i ++) ws[i] = 0;
    for(i = 0; i < n; i ++) ws[x[i] = r[i]] ++;
    for(i = 1; i < m; i ++) ws[i] += ws[i - 1];
    for(i = n - 1; i >= 0; i --) sa[-- ws[x[i]]] = i;
    for(j = p = 1; p < n; j <<= 1, m = p)
    {
        for(p = 0, i = n - j; i < n; i ++) y[p ++] = i;
        for(i = 0; i < n; i ++) if(sa[i] >= j) y[p ++] = sa[i] - j;
        for(i = 0; i < n; i ++) wv[i] = x[y[i]];
        for(i = 0; i < m; i ++) ws[i] = 0;
        for(i = 0; i < n; i ++) ws[wv[i]] ++;
        for(i = 1; i < m; i ++) ws[i] += ws[i - 1];
        for(i = n - 1; i >= 0; i --) sa[-- ws[wv[i]]] = y[i];
        for(std::swap(x, y), p = 1, x[sa[0]] = 0, i = 1; i < n; i ++)
            x[sa[i]] = _DACMP(y, sa[i - 1], sa[i], j) ? p - 1 : p ++;
    }
}
void SA::CalHeight()
{
    int i, j, k = 0;
    for(i = 1; i <= len; i ++) rk[sa[i]] = i;
    for(i = 0; i < len; height[rk[i ++]] = k)
        for(k -= !!k, j = sa[rk[i] - 1]; r[i + k] == r[j + k]; k ++);
}
bool Judge(SA &sa, int n, int mid)
{
    int l = sa.sa[0], r = sa.sa[0];
    for(int i = 1; i <= n; i ++)
    {
        if(sa.height[i] < mid) l = r = sa.sa[i];
        else
        {
            l = l == -1 ? sa.sa[i] : std::min(l, sa.sa[i]);
            r = r == -1 ? sa.sa[i] : std::max(r, sa.sa[i]);
            if(r - l >= mid) return true;
        }
    }
    return false;
}
int r[maxn];
int main()
{
    SA sa(maxn);
    int n, k;
    while(scanf("%d", &n) != EOF && n)
    {
        for(int i = 0; i < n; i ++)
            scanf("%d", &r[i]);
        n --;
        for(int i = 0; i < n; i ++)
            r[i] = r[i + 1] - r[i] + 90;
        r[n] = 0;
        sa.CalSA(r, n, 200);
        sa.CalHeight();
        int ans = 0;
        int left = 0, right = n + 1, mid;
        while(left < right - 1)
        {
            mid = left + right >> 1;
            if(Judge(sa, n, mid)) left = mid;
            else right = mid;
        }
        printf("%d\n", left >= 4 ? left + 1 : 0);
    }
    return 0;
}