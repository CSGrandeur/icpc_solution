// difficulty: 1
// Maximum Product
// 需要`long long`
// 三重for O(n^3)暴力不优雅，枚举起点，从该几点连乘更新ans，O(n^2)

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int a[20];
int main()
{
    int t = 0, n;
    long long ans;
    while(scanf("%d", &n) != EOF)
    {
        ans = 0;
        for(int i = 0; i < n; i ++)
        {
            scanf("%d", &a[i]);
            long long tmp = 1;
            for(int j = i; j >= 0; j --)
            {
                tmp *= a[j];
                ans = tmp > ans ? tmp : ans;
            }
        }
        printf("Case #%d: The maximum product is %lld.\n\n", ++ t, ans);
    }
    return 0;
}