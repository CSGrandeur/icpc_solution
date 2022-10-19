// difficulty: 3
// Mathematical Circus
// k为奇数时，成对的奇偶数即可，两偶数相乘为4的倍速
// k为偶数但不是4的倍数时，成对的奇偶数中的偶数是否为4的倍数交替出现，4的倍数则奇数在前，否则奇数在后
// k为4的倍数则无解
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
int main()
{
    int t, n, k;
    for(scanf("%d", &t); t --; )
    {
        scanf("%d%d", &n, &k);
        if(k % 4 == 0) printf("NO\n");
        else
        {
            printf("YES\n");
            if(k & 1)
            {
                for(int i = 1; i < n; i += 2)
                    printf("%d %d\n", i, i + 1);
            }
            else
            {
                for(int i = 1; i < n; i += 2)
                {
                    if((i + 1) % 4 == 0) printf("%d %d\n", i, i + 1);
                    else printf("%d %d\n", i + 1, i);
                }                    
            }
        }
    }
    return 0;
}