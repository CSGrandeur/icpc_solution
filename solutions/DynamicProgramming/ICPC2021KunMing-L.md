### `Simone and Graph Coloring`

> difficulty: 2

最长下降子序列。每个数字自身所处的最长下降子序列的位置可以构造为其颜色编号。

10^6数量级需要 nlogn 算法，刚好可以DP时构造答案。

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<functional>
#include<algorithm>
const int maxn = 1100000;
int a[maxn];
int dp[maxn], tp;
int c[maxn];
int main()
{
    int t, n;
    for(scanf("%d", &t); t --; )
    {
        scanf("%d", &n);
        for(int i = 0; i < n; i ++)
            scanf("%d", &a[i]);
        tp = -1;
        for(int i = 0; i < n; i ++)
        {
            int dpIth = 0;
            if(tp == -1 || dp[tp] > a[i])
                dpIth = ++ tp;
            else
                dpIth = std::upper_bound(dp, dp + tp + 1, a[i], std::greater<int>()) - dp;
            dp[dpIth] = a[i];
            c[i] = dpIth + 1;
        }
        printf("%d\n", tp + 1);
        for(int i = 0; i < n; i ++)
            printf(i ? " %d" : "%d", c[i]);
        printf("\n");
    }
    return 0;
}
```

