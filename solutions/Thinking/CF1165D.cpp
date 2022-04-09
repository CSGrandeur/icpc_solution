// difficulty: 3
// GCD Guess
// 第一次做交互题一脸懵逼。数学不够敏感的话也很难想
// 基于`gcd(x+a,x+b)`猜30次，可以推测可能和二进制有关。
// 对于第k个二进制位，如果把小于 $2^k$ 的尾数清空，让 $a=2^k, b=2^{k+1}+a$，`gcd(x+a,x+b)`就可以判断这个二进制位。
// 尾数在每一步都能更新到，设 res，则$a=2^{k}-res, b=2^{k+1}+a-res$
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<vector>

int main()
{
    int t;
    for(scanf("%d", &t); t --; )
    {
        unsigned res = 0, a = 1, b = 2, rp;
        for(int i = 0; i < 30; i ++, a <<= 1, b <<= 1)
        {
            printf("? %u %u\n", a - res, b + a - res);
            fflush(stdout);
            scanf("%u", &rp);
            res += rp == a ? 0 : a;
        }
        printf("! %u\n", res);
    }

    return 0;
}