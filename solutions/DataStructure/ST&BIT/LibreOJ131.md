### `树状数组 3 ：区间修改，区间查询`

> difficulty: 3

先用线段树做个模板，树状数组改天写

```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
typedef long long LL;
const int maxn = 1e6 + 10;
int a[maxn];
LL sum[maxn * 4], lasy[maxn * 4];
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
void UpdateNode(int now, int ls, int rs, LL v) {
    int rangeN = rs - ls;
    sum[now] += rangeN * v;
    lasy[now] += v;
}
void PushDown(int now, int ls, int rs) {
    if(lasy[now] == 0 || ls >= rs) return;
    int mid = ls + rs >> 1;
    UpdateNode(now << 1, ls, mid, lasy[now]);
    UpdateNode(now << 1 | 1, mid, rs, lasy[now]);
    lasy[now] = 0;
}
void PushUp(int now) {
    sum[now] = sum[now << 1] + sum[now << 1 | 1];
}
void Update(int now, int ls, int rs, int x, int y, int v) {
    if(ls >= rs) return;
    if(x <= ls && y >= rs) UpdateNode(now, ls, rs, v);
    else {
        PushDown(now, ls, rs);
        int mid = ls + rs >> 1;
        if(x < mid) Update(now << 1, ls, mid, x, y, v);
        if(y > mid) Update(now << 1 | 1, mid, rs, x, y, v);
        PushUp(now);
    }
}
LL Query(int now, int ls, int rs, int x, int y) {
    if(ls >= rs) return 0;
    if(x <= ls && y >= rs) return sum[now];
    PushDown(now, ls, rs);
    int mid = ls + rs >> 1;
    LL ret = 0;
    if(x < mid) ret += Query(now << 1, ls, mid, x, y);
    if(y > mid) ret += Query(now << 1 | 1, mid, rs, x, y);
    return ret;
}
int main() {
    int op, x, y, v, n, q;
    while(scanf("%d%d", &n, &q) != EOF) {
        for(int i = 0; i < n; i ++) {
            scanf("%d", &a[i]);
        }
        memset(lasy, 0, sizeof(lasy));
        BuildSegTree(1, 0, n);
        while(q --) {
            scanf("%d%d%d", &op, &x, &y);
            if(op == 1) {
                scanf("%d", &v);
                Update(1, 0, n, x - 1, y, v);
            }
            else {
                printf("%lld\n", Query(1, 0, n, x - 1, y));
            }
        }
    }
    return 0;
}
```