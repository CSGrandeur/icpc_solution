// difficulty: 1
// Moves 
// 分类讨论一下
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
int main()
{
    int t, n;
    for(scanf("%d", &t); t --; )
    {
        scanf("%d", &n);
        if(n < 0) n = -n;
        printf("%d\n", n / 3 + (3 - n % 3) % 3 - (n % 3 == 1 && n != 1));
    }
    return 0;
}