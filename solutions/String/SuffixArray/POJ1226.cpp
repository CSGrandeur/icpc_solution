// difficulty: 4
// Substrings
// 后缀数组入门1：公共子串问题
// 此题为经典入门题，不过后缀数组门槛本不低，且要考虑一些坑点，有debug难度，故定 4星难度
// 和两个字符串的最长子串思路一样，所有字符串拼起来，中间由各不相同且不在字符串出现的符号填充
// r 数组是int型的，所以填充符可以大于 128 。
// 二分枚举公共子串长度 mid，通过打标记（`who`数组与`visRecord`数组），判断 height 数组中连续不小于 mid 的各段落是否涵盖所有 n 个字符串。

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
char buf[210];
int r[maxn], who[maxn];
int visRecord[210], vtp;
void SA::CalHeight()
{
    int i, j, k = 0;
    for(i = 1; i <= len; i ++) rk[sa[i]] = i;
    for(i = 0; i < len; height[rk[i ++]] = k)
        for(k -= !!k, j = sa[rk[i] - 1]; r[i + k] == r[j + k]; k ++);
}
inline bool Vis(int ith) {return visRecord[ith] == vtp ? true : (visRecord[ith] = vtp) && false;}
bool Judge(SA &sa, int n, int len, int mid)
{
    int cnt = 0;
    for(int i = 1; i <= len; i ++)
    {
        if(sa.height[i] >= mid)
        {
            cnt += !cnt && who[sa.sa[i - 1]] >= 0 && !Vis(who[sa.sa[i - 1]]);
            cnt += who[sa.sa[i]] >= 0 && !Vis(who[sa.sa[i]]);
            if(cnt >= n) return true;
        }
        else
            cnt = 0, vtp ++;
    }
    return false;
}
int main()
{
    // freopen("test.in", "rb", stdin);
    // freopen("test.out", "wb", stdout);
    SA sa(maxn);
    int t, n, len;
    for(scanf("%d", &t); t --; )
    {
        memset(visRecord, 0, sizeof(visRecord));
        vtp = 1;
        scanf("%d", &n);
        len = 0;
        for(int i = 0; i < n; i ++)
        {
            scanf("%s", buf);
            for(int j = 0; buf[j]; j ++, len ++)
                r[len] = buf[j], who[len] = i;
            r[len] = 129 + (i << 1);
            who[len ++] = -1;
            for(int j = strlen(buf) - 1; j >= 0; j --, len ++)
                r[len] = buf[j], who[len] = i;
            r[len] = 129 + (i << 1 | 1);
            who[len] = -1;
            if(i < n - 1) len ++;
        }
        sa.CalSA(r, len, 350);
        sa.CalHeight();
        int left = 0, right = len + 1, mid;
        while(left <= right)
        {
            mid = left + right >> 1;
            if(Judge(sa, n, len, mid)) left = mid + 1;
            else right = mid - 1;
        }
        printf("%d\n", n == 1 ? len >> 1 : right);
    }
    return 0;
}
