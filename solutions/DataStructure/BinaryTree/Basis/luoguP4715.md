### `淘汰赛`

> difficulty: 1

有点线段树的味道


```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
const int maxn = 1e5 + 10;
int n, n2;
int a[maxn];
int Win(int x, int y){return a[x] > a[y] ? x : y;}
int Loss(int x, int y){return a[x] < a[y] ? x : y;}
int GetSecond(int l, int r) {
    if(l == r - 1) return l;
    int lsec = GetSecond(l, l + r >> 1);
    int rsec = GetSecond(l + r >> 1, r);
    return l == 0 && r == n2 ? Loss(lsec, rsec) : Win(lsec, rsec);
}
int main() {
    while(scanf("%d", &n) != EOF) {
        n2 = 1 << n;
        for(int i = 0; i < n2; i ++) {
            scanf("%d", &a[i]);
        }
        printf("%d\n", GetSecond(0, n2) + 1);
    }
    return 0;
}
```