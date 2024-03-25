### `LIS`

> difficulty: 1

经典最长上升子序列

```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
const int maxn = 1e5 + 10;
int n, a[maxn], b[maxn];
int LIS(int a[], int n) {
int tlen = 0;   // 尾元素个数，指向尾元素数组最后一个元素的下一个位置
for(int i = 0; i < n; i ++) {
    int ith = std::upper_bound(b, b + tlen, a[i]) - b;  // 找到不小于a[i]的第一个数
    b[ith] = a[i];              // 用a[i]替换找到的位置，没找到则ith就是末尾位置
    if(ith == tlen) tlen ++;    // 没有不小于a[i]的数，a[i]放入尾部，长度增加
}
    return tlen;
}
int main() {
    while(scanf("%d", &n) != EOF) {
        for(int i = 0; i < n; i ++) {
            scanf("%d", &a[i]);
        }
        printf("%d\n", LIS(a, n));
    }
    return 0;
}
```