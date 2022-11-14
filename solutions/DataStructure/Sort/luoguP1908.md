### `逆序对`

> difficulty: 1

归并时候统计左半边比右半边每个元素大的个数

```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
const int maxn = 5e5 + 10;
int a[maxn], b[maxn];
int n;
long long MergeCount(int a[], int left, int right)
{
    if(left >= right - 1) return 0;
    int mid = left + right >> 1;
    long long ret = 0;
    ret += MergeCount(a, left, mid);
    ret += MergeCount(a, mid, right);
    int i, j, k;
    for(i = k = left, j = mid; i < mid && j < right;)
    {
        if(a[i] <= a[j]) b[k ++] = a[i ++];
        else ret += mid - i, b[k ++] = a[j ++];
    }
    while(i < mid) b[k ++] = a[i ++];
    while(j < right) b[k ++] = a[j ++];
    memcpy(a + left, b + left, sizeof(a[0]) * (right - left));
    return ret;
}
int main()
{
    while(scanf("%d", &n) != EOF)
    {
        for(int i = 0; i < n; i ++)
            scanf("%d", &a[i]);
        printf("%lld\n", MergeCount(a, 0, n));
    }
    return 0;
}
```

