# 数据结构

## 树状数组

```cpp
// 前缀最大值为例
int n, di[maxn];
inline int lowbit(const int &x)
{
    return x&(-x);
}
void Update(int x, int v)
{
    while(x <= n){
        di[x] += max(di[x], v);
        x += lowbit(x);
    }
}
int Search(int x)
{
    int res = 0;
    while(x > 0)
    {
        res = max(res, di[x]);
        x -= lowbit(x);
    }
    return res;
}
```

## 可持久化线段树

### 常规定义

```cpp
const int maxN = 1e6 + 10;      // 数组大小
const int maxM = maxN << 5;     // 线段树结点占用，基本上是 maxN * log2(max value)
const int maxV = 1e9 + 10;      // 离散数值的最大值
int rt[maxN], lc[maxM], rc[maxM];   // root、左“指针”、右“指针”
int tp;                         // 全局自增新建结点“指针”
```

### 区间数值和

```cpp
void Update(int v, int left, int right, int &now, int last)
{
    // 沿着last版本的树，更新now版本的一条链
    now = tp ++;
    lc[now] = lc[last];
    rc[now] = rc[last];
    val[now] = val[last] + v;   // now版本下[left, right) 范围内所有数的和
    if(left >= right - 1) return;
    int mid = left + right >> 1;
    if(v < mid) Update(v, left, mid, lc[now], lc[last]);
    else Update(v, mid, right, rc[now], rc[last]);
}
LL Query(int st, int ed, int left, int right, int tl, int tr)
{
    // 求编号区间[l,r)的数值在[st, ed)范围的数的和
    // 通过传参，tl对应1~l-1区间的版本，tr对应1~r区间的版本，在树上求前缀差
    if(st <= left && ed >= right) return val[tr] - val[tl];
    int mid = left + right >> 1;
    LL res = 0;
    if(st < mid) res += Query(st, ed, left, mid, lc[tl], lc[tr]);
    if(ed > mid) res += Query(st, ed, mid, right, rc[tl], rc[tr]);
    return res;
}
```
