// difficulty: 3
// Substrings
// 非常妙的递推：
// 由w==i-1的答案，加上两次出现距离大于 i 的数的个数，减去 i-1 长的后缀里不相同个数
// dp[i] = dp[i - 1] + sumDis[i] - sumPostfix;
// sumPostfix 可用 n-(i-1) 位置的数是否是最后出现递推，用 last[a[i]] 记录 a[i] 上次出现位置，
// sumDis[i] 是距离为 i 的数的个数的后缀和，这个后缀和就是距离大于 i 的数的个数

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef long long LL;
const int maxn = 1e6 + 10;
int n, q, w;
int a[maxn];
LL dp[maxn];
// 上一次位置，两次出现距离等于i的个数（后递推为后缀和），i-1长度后缀不相同个数
int last[maxn], sumDis[maxn], sumPostfix;
int main()
{
    while(scanf("%d", &n) && n)
    {
        memset(last, -1, sizeof(last));
        memset(sumDis, 0, sizeof(sumDis));
        dp[1] = n;
        sumPostfix = 0;
        for(int i = 0; i < n; i ++)
        {
            scanf("%d", &a[i]);
            sumDis[i - last[a[i]]] ++;
            last[a[i]] = i;
        }
        for(int i = n - 1; i; i --)
            sumDis[i] += sumDis[i + 1];
        for(int i = 2; i <= n; i ++)
        {
            sumPostfix += last[a[n - i + 1]] == n - i + 1;
            dp[i] = dp[i - 1] + sumDis[i] - sumPostfix;
        }
        for(scanf("%d", &q); q --; )
        {
            scanf("%d", &w);
            printf("%lld\n", dp[w]);
        }
    }
    return 0;
}