// difficulty: 1
// Burenka Plays with Fractions
// 分类讨论下
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
typedef long long LL;
LL GCD(LL a, LL b){return b ? GCD(b, a % b) : a;}
int main()
{
    int t;
    long long a, b, c, d;
    for(scanf("%d", &t); t --; )
    {
        scanf("%lld%lld%lld%lld", &a, &b, &c, &d);
        if(!a || !c) {printf(a || c ? "1\n" : "0\n"); continue;}
        int g = GCD(a, b);
        a /= g; b /= g;
        g = GCD(c, d);
        c /= g, d /= g;
        if(a == c && b == d) printf("0\n");
        else
        {
            a *= d; c *= b;
            if(a % c == 0 || c % a == 0) printf("1\n");
            else printf("2\n");
        }
        
    }
    return 0;
}