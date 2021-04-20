// difficulty: 2
// Bad Hair Day
// 经典栈模拟题，用栈维护“当前奶牛之前所有高于该奶牛的奶牛”
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int n;
int a[81000], atp, tmp;
long long ans;
int main()
{
    while(scanf("%d", &n) != EOF)
    {
        atp = ans = 0;
        for(int i = 0; i < n; i ++)
        {
            scanf("%d", &tmp);
            while(atp > 0 && a[atp - 1] <= tmp)
                atp --;
            ans += atp;
            a[atp ++] = tmp;
        }
        printf("%lld\n", ans);
    }
    return 0;
}