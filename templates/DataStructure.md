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