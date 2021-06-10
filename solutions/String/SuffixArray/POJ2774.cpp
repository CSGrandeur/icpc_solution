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
inline int DACMP(int *r, int a, int b, int l)
{return r[a] == r[b] && r[a + l] == r[b + l];}
void DA(int r[], int sa[], int n, int m)
{
    int i, j, p, *x = wa, *y = wb, *t;
    r[n ++] = 0;
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
int sa[maxn], rk[maxn], lcp[maxn];
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
        DA(r, sa, n, 128);
        CalLcp(r, sa, n);
        for(int i = 1; i <= n; i ++)
            if(lcp[i] > ans && 1LL * (sa[i - 1] - blen) * (sa[i] - blen) < 0)
                ans = lcp[i];
        printf("%d\n", ans);
    }
    return 0;
}