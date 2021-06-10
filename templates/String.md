# 字符串

## KMP

```cpp
int nex[maxn];
void BuildNext(char ps[], int nex[])
{
    nex[0] = -1;
    for(int i = 0, j = -1; ps[i]; )
    {
        if(j == -1 || ps[i] == ps[j])
            nex[++i] = ++j;
        else
            j = nex[j];
    }
}
int KmpMatch(char ms[], char ps[])
{
    int tlen = strlen(ps);
    for(int i = 0, j = 0; ms[i];)
    {
        if(j == -1 || ms[i] == ps[j])
        {
            if(j == tlen - 1)
            {
                // 确认一次完整匹配，此处可支持不同题目要求的记录匹配位置、
                // 统计匹配次数 或返回第一次匹配位置等操作。
                // 这里返回匹配位置。其它不结束循环的工作则 j = nex[j];
                return i - tlen + 1;    // 从 i 往前数 tlen-1 个
            }
            else
                i ++, j ++;
        }
        else
            j = nex[j];
    }
    return -1;
}
```

### KMP求最小循环节

`aaaa`循环节为`a`，`ababa`最小循环节为`ab`，`abcd`最小循环节为本身

最小循环节长度为

```cpp
int sLen = strlen(st);
int minLoopLen = sLen - nex[sLen];
```

## 后缀数组

### 倍增（DA）

罗穗骞 模板

```cpp
const int maxn = 2e5 + 10;
int wa[maxn], wb[maxn], wv[maxn], ws[maxn];
int sa[maxn], rk[maxn]
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
void GetRank()
{
    for(int i = 1; i <= n; i ++) rk[sa[i]] = i;
}
```

### 相邻rank后缀最长公共前缀

`lcp[i]` 表示rank为 i 与 i-1 两个后缀的最长公共前缀长度

```cpp
int lcp[maxn];
void CalLcp(int r[], int sa[], int n)
{
    int i, j, k = 0;
    for(i = 1; i <= n; i ++) rk[sa[i]] = i;
    for(i = 0; i < n; lcp[rk[i ++]] = k)
        for(k -= !!k, j = sa[rk[i] - 1]; r[i + k] == r[j + k]; k ++);
}
```