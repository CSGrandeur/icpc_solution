// difficulty: 3
// Long Long Message
// 求两字符串最长公共子串。
// 把两个字符串拼接，但中间隔一个两个串都不存在的“字符”，建后缀数组，求相邻rank后缀最长公共前缀lcp
// 隔一个字符用来避免把二号串开头算到一号串末尾里的长度，如 abc, abca，加特殊字符拼接，abc#abca与abca公共前缀是3
// 但如果不加特殊字符，abcabca与abca公共前缀就是 4 了，但是这个公共前缀包含了二号字符串的开头，干扰了答案
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int maxn = 2e5 + 10;
int wa[maxn], wb[maxn], wv[maxn], ws[maxn];
int sa[maxn];
inline int DACMP(int *r, int a, int b, int l)
{return r[a] == r[b] && r[a + l] == r[b + l];}
void DA(int r[], int sa[], int n, int m)
{
    int i, j, p, *x = wa, *y = wb, *t;
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
        for(t = x, x = y, y = t, p = 1, x[sa[0]] = 0, i = 1; i < n; i ++)
            x[sa[i]] = DACMP(y, sa[i - 1], sa[i], j) ? p - 1 : p ++;
    }
}
int rk[maxn], lcp[maxn];
void CalLcp(int r[], int sa[], int n)
{
    int i, j, k = 0;
    for(i = 1; i <= n; i ++) rk[sa[i]] = i;
    for(i = 0; i < n; lcp[rk[i ++]] = k)
        for(k -= !!k, j = sa[rk[i] - 1]; r[i + k] == r[j + k]; k ++);
}
char buf[maxn];
int r[maxn], n;
int main()
{
    while(scanf("%s", buf) != EOF)
    {
        int ans = 0, blen = strlen(buf);
        buf[blen] = 1;
        scanf("%s", buf + blen + 1);
        for(n = 0; buf[n]; n ++)
            r[n] = buf[n];
        r[n] = 0;
        DA(r, sa, n + 1, 128);
        CalLcp(r, sa, n);
        for(int i = 1; i <= n; i ++)
            if(lcp[i] > ans && 1LL * (sa[i - 1] - blen) * (sa[i] - blen) < 0)
                ans = lcp[i];
        printf("%d\n", ans);
    }
    return 0;
}

/*
// DC3
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int maxn = 2e5 + 10;
int wa[maxn], wb[maxn], wv[maxn], ws[maxn];
inline int F(int x, int tb) {return x / 3 + (x % 3 == 1 ? 0 : tb);}
inline int G(int x, int tb) {return x < tb ? x * 3 + 1 : (x - tb) * 3 + 2;}
inline int C0(int r[], int a, int b){return !memcmp(r + a, r + b, sizeof(r[0]) * 3);}
inline int C12(int k, int r[], int a, int b)
{
    if(k == 2) return r[a] < r[b] || r[a] == r[b] && C12(1, r, a + 1, b + 1);
    else return r[a] < r[b] || r[a] == r[b] && wv[a + 1] < wv[b + 1];
}
void DC3Sort(int r[], int a[], int b[], int n, int m)
{
    for(int i = 0; i < n; i ++) wv[i] = r[a[i]];
    for(int i = 0; i < m; i ++) ws[i] = 0;
    for(int i = 0; i < n; i ++) ws[wv[i]] ++;
    for(int i = 1; i < m; i ++) ws[i] += ws[i - 1];
    for(int i = n - 1; i >= 0; i --) b[-- ws[wv[i]]] = a[i];
}
void DC3(int r[], int sa[], int n, int m)
{
    int i, j, *rn = r + n, *san = sa + n, ta = 0, tb = (n + 1) / 3, tbc = 0, p;
    r[n] = r[n + 1] = 0;
    for(i = 0; i < n; i ++) if(i % 3) wa[tbc ++] = i;
    DC3Sort(r + 2, wa, wb, tbc, m);
    DC3Sort(r + 1, wb, wa, tbc, m);
    DC3Sort(r, wa, wb, tbc, m);
    for(p = 1, rn[F(wb[0], tb)] = 0, i = 1; i < tbc; i ++)
        rn[F(wb[i], tb)] = C0(r, wb[i - 1], wb[i]) ? p - 1 : p ++;
    if(p < tbc) DC3(rn, san, tbc, p);
    else for(i = 0; i < tbc; i ++) san[rn[i]] = i;
    for(i = 0; i < tbc; i ++) if(san[i] < tb) wb[ta ++] = san[i] * 3;
    if(n % 3 == 1) wb[ta ++] = n - 1;
    DC3Sort(r, wb, wa, ta, m);
    for(i = 0; i < tbc; i ++) wv[wb[i] = G(san[i], tb)] = i;
    for(i = j = p = 0; i < ta && j < tbc; p ++)
        sa[p] = C12(wb[j] % 3, r, wa[i], wb[j]) ? wa[i ++] : wb[j ++];
    for(; i < ta; p ++) sa[p] = wa[i ++];
    for(; j < tbc; p ++) sa[p] = wb[j ++];

}
int sa[maxn * 3], rk[maxn], lcp[maxn];
void CalLcp(int r[], int sa[], int n)
{
    int i, j, k = 0;
    for(i = 1; i <= n; i ++) rk[sa[i]] = i;
    for(i = 0; i < n; lcp[rk[i ++]] = k)
        for(k -= !!k, j = sa[rk[i] - 1]; r[i + k] == r[j + k]; k ++);
}
char buf[maxn];
int r[maxn * 3], n;
int main()
{
    while(scanf("%s", buf) != EOF)
    {
        int ans = 0, blen = strlen(buf);
        buf[blen] = 1;
        scanf("%s", buf + blen + 1);
        for(n = 0; buf[n]; n ++)
            r[n] = buf[n];
        r[n] = 0;
        DC3(r, sa, n + 1, 128);
        CalLcp(r, sa, n);
        for(int i = 1; i <= n; i ++)
            if(lcp[i] > ans && 1LL * (sa[i - 1] - blen) * (sa[i] - blen) < 0)
                ans = lcp[i];
        printf("%d\n", ans);
    }
    return 0;
}
*/