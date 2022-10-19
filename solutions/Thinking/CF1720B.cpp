// difficulty: 2
// Interesting Sum
// 最大次大、最小次小 总能划分开，所以直接得解。

#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
const int maxn = 1e5 + 10;
int a[maxn];
int main()
{
    int t, n;
    for(scanf("%d", &t); t --; )
    {
        scanf("%d", &n);
        
        for(int i = 0; i < n; i ++)
            scanf("%d", &a[i]);
        std::sort(a, a + n);
        printf("%d\n", a[n - 1] + a[n - 2] - a[0] - a[1]);
    }
    return 0;
}