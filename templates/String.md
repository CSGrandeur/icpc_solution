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

## 扩展KMP

求模式串与主串每个后缀的最长公共前缀（LCP），nex保存模式串自身每个后缀LCP，ext保存模式串与主串每个后缀LCP。

```cpp
char ps[maxn], ms[maxn];
int nex[maxn], ext[maxn];
void ExtNext(char ps[], int n)
{
    nex[0] = n;
    int now = 0, p0 = 1;
    while(ps[now] == ps[now + 1] && now + 1 < n) now ++;
    nex[1] = now;
    for(int i = 2; i < n; i ++)
    {
        if(i + nex[i - p0] < nex[p0] + p0) nex[i] = nex[i - p0];
        else
        {
            now = std::max(0, nex[p0] + p0 - i);
            while(ps[now] == ps[i + now] && i + now < n) now ++;
            nex[i] = now;
            p0 = i;
        }
    }
}
void ExtKmp(char ms[], char ps[], int m, int n)
{
    GetNext(ps, n);
    int now = 0, p0 = 0;
    while(ms[now] == ps[now] && now < std::min(m, n)) now ++;
    ext[0] = now;
    for(int i = 1; i < m; i ++)
    {
        if(i + nex[i - p0] < ext[p0] + p0) ext[i] = nex[i - p0];
        else
        {
            now = ext[p0] + p0 - i;
            now = std::max(now, 0);
            while(ps[now] == ms[i + now] && now < n && now + i < m) now ++;
            ext[i] = now;
            p0 = i;
        }
    }
}
```

## 后缀数组

调用前需要将关键值数组末尾设为0，以n+1长度调用

```cpp
r[n] = 0;
DA(r, sa, n + 1, 128);
// DC3(r, sa, n + 1, 128);
```

`height[i]` 表示rank为 i 与 i-1 两个后缀的最长公共前缀长度 `lcp(i, i - 1)`

### 倍增（DA）

罗穗骞 DA 模板，O(nlogn)，常数相对较小


```cpp
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
void SA::_Init(int maxn_)
{
    {
        if(r.size() <= maxn_)
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
void SA::CalRank()
{
    // the same as the first step in CalHeight()
    for(int i = 1; i <= len; i ++) rk[sa[i]] = i;
}
void SA::CalHeight()
{
    int i, j, k = 0;
    for(i = 1; i <= len; i ++) rk[sa[i]] = i;
    for(i = 0; i < len; height[rk[i ++]] = k)
        for(k -= !!k, j = sa[rk[i] - 1]; r[i + k] == r[j + k]; k ++);
}
```

### DC3

罗穗骞 DC3 模板，O(n)，常数相对 DA 较大

```cpp
// Differences from DA version
struct SA
{
    // ...
    // DC3
    template<typename STR_IN>
    void CalSA(STR_IN buf[], int len_, int m_=128)
    {
        len = len_, m = m_;
        _GetR(buf, true);        
        DC3(r.data(), sa.data(), len + 1, m);
    }
    inline int F(int x, int tb) {return x / 3 + (x % 3 == 1 ? 0 : tb);}
    inline int G(int x, int tb) {return x < tb ? x * 3 + 1 : (x - tb) * 3 + 2;}
    inline int C0(int r[], int a, int b){return !memcmp(r + a, r + b, sizeof(r[0]) * 3);}
    inline int C12(int k, int r[], int a, int b)
    {
        if(k == 2) return r[a] < r[b] || r[a] == r[b] && C12(1, r, a + 1, b + 1);
        else return r[a] < r[b] || r[a] == r[b] && wv[a + 1] < wv[b + 1];
    }
    void DC3Sort(int r[], std::vector<int> &a, std::vector<int> &b, int n, int m);
    void DC3(int r[], int sa[], int n, int m);
}
void SA::DC3Sort(int r[], std::vector<int> &a, std::vector<int> &b, int n, int m)
{
    for(int i = 0; i < n; i ++) wv[i] = r[a[i]];
    for(int i = 0; i < m; i ++) ws[i] = 0;
    for(int i = 0; i < n; i ++) ws[wv[i]] ++;
    for(int i = 1; i < m; i ++) ws[i] += ws[i - 1];
    for(int i = n - 1; i >= 0; i --) b[-- ws[wv[i]]] = a[i];
}
void SA::DC3(int r[], int sa[], int n, int m)
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
```

### 得到sa后换算rank

```cpp
int rk[maxn];
void GetRank()
{
    for(int i = 1; i <= n; i ++) rk[sa[i]] = i;
}
```


## 后缀自动机

结点数要开字符串长度二倍

```cpp
const int maxn = 5e5 + 10;
struct SAM
{
    struct SamNode
    {
        int len, link;
        std::unordered_map<int, int> nex;
        void Init(int l_ = 0){len = l_; link = -1; nex.clear();}
        SamNode(){Init();}
        SamNode(int l_){Init(l_);}
    };
    std::vector<SamNode> smn;
    int stp, slast;
    SAM(){Init();}
    SAM(int maxn_){smn.resize(maxn_);}
    void Init(){stp = slast = 0; AddNode();}
    void AddNode(int l_ = 0)
    {
        if(stp >= smn.size()) smn.push_back(SamNode(l_));
        else smn[stp].Init(l_);
        stp ++;
    }
    void Add(int c)
    {
        int cur = stp, p = slast;
        AddNode(smn[slast].len + 1);
        slast = cur;
        for(; p != -1 && !smn[p].nex.count(c); p = smn[p].link)
            smn[p].nex[c] = cur;
        if(p == -1) smn[cur].link = 0;
        else
        {
            int q = smn[p].nex[c];
            if(smn[p].len + 1 == smn[q].len) smn[cur].link = q;
            else
            {
                int clone = stp;
                AddNode();
                smn[clone].len = smn[p].len + 1;
                smn[clone].nex = smn[q].nex;
                smn[clone].link = smn[q].link;
                for(; p != -1 && smn[p].nex[c] == q; p = smn[p].link)
                    smn[p].nex[c] = clone;
                smn[q].link = smn[cur].link = clone;
            }
        }
    }
    void Build(char buf[])
    {
        Init();
        for(int i = 0; buf[i]; i ++)
            Add(buf[i]);
    }
};
```
