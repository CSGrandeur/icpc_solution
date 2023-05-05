### `树状数组 1 ：单点修改，区间查询`

> difficulty: 2

树状数组入门第一题


```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
typedef long long LL;
const int maxn = 1e6 + 10;
int op, n, q;
LL a[maxn];

int Lowbit(int x) {return x & -x;}
void Update(int x, int v) {
    for(; x < maxn; x += Lowbit(x))
        a[x] += v;
}
LL Query(int x) {
    LL res = 0;
    for(; x; x -= Lowbit(x))
        res += a[x];
    return res;
}
int main() {
    int x, y;
    while(scanf("%d%d", &n, &q) != EOF) {
        memset(a, 0, sizeof(a));
        for(int i = 1; i <= n; i ++) {
            scanf("%d", &x);
            Update(i, x);
        }
        while(q --) {
            scanf("%d%d%d", &op, &x, &y);
            if(op == 1) {
                Update(x, y);
            }
            else {
                printf("%lld\n", Query(y) - Query(x - 1));
            }
        }
    }
    return 0;
}
```

线段树版本

```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
typedef long long LL;
const int maxn = 1e6 + 10;
int a[maxn];
LL sum[maxn * 4];
LL BuildSegTree(int now, int ls, int rs) {
    // now: 当前结点； [ls, rs) 左闭右开区间端点
    if(ls >= rs) return 0;
    if(ls == rs - 1) {return sum[now] = a[ls];}
    sum[now] = 0;
    int mid = ls + rs >> 1;
    sum[now] += BuildSegTree(now << 1, ls, mid);
    sum[now] += BuildSegTree(now << 1 | 1, mid, rs);
    return sum[now];
}
void Update(int now, int ls, int rs, int x, int v) {
    if(ls >= rs) return;
    if(ls == rs - 1) {sum[now] += v; return;}
    int mid = ls + rs >> 1;
    if(x < mid) Update(now << 1, ls, mid, x, v);
    else Update(now << 1 | 1, mid, rs, x, v);
    sum[now] += v;
}
LL Query(int now, int ls, int rs, int x, int y) {
    if(ls >= rs) return 0;
    if(x <= ls && y >= rs) return sum[now];
    int mid = ls + rs >> 1;
    LL ret = 0;
    if(x < mid) ret += Query(now << 1, ls, mid, x, y);
    if(y > mid) ret += Query(now << 1 | 1, mid, rs, x, y);
    return ret;
}
int main() {
    int op, x, y, n, q;
    while(scanf("%d%d", &n, &q) != EOF) {
        for(int i = 0; i < n; i ++) {
            scanf("%d", &a[i]);
        }
        BuildSegTree(1, 0, n);
        while(q --) {
            scanf("%d%d%d", &op, &x, &y);
            if(op == 1) {
                Update(1, 0, n, x - 1, y);
            }
            else {
                printf("%lld\n", Query(1, 0, n, x - 1, y));
            }
        }
    }
    return 0;
}
```