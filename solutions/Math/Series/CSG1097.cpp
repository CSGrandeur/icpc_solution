// difficulty: 2
// 矩形并
// 把 ab 分别小于 xy、大于 xy 的部分分开计算，即都小于的部分、一个大于的部分、两个都大于的部分
// 四个部分分别是 等差数列乘等差数列、等差数列乘定值、定值乘定值。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
const int mod = 1e9 + 7;
inline int SqSum(int a)
{
    return 1LL * (1 + a) * a / 2 % mod;
}
inline int AcSum(int a, int b)
{
    return 1LL * SqSum(a) * SqSum(b) % mod;
}
int main()
{
    int a, b, x1, x2, y1, y2;
    while(scanf("%d%d%d%d%d%d", &a, &b, &x1, &x2, &y1, &y2) != EOF)
    {
        int rtx = std::min(x2, a), rty = std::min(y2, b);
        int base = AcSum(a, b);
        base = (base + 1LL * a * b % mod * (y2 - y1) % mod * (x2 - x1) % mod) % mod;
        if(rtx > x1 && rty > y1)
        {
            base = (base - AcSum(rtx - x1, rty - y1) % mod + mod) % mod;
            if(b > y2)
                base = ((base - 1LL * SqSum(rtx - x1) * (y2 - y1) % mod * (b - y2)) % mod + mod) % mod;
            if(a > x2)
                base = ((base - 1LL * SqSum(rty - y1) * (x2 - x1) % mod * (a - x2)) % mod + mod) % mod;
            if(b > y2 && a > x2)
                base = ((base - 1LL * (y2 - y1) % mod * (x2 - x1) % mod * (b - y2) % mod * (a - x2) % mod) % mod + mod) % mod;
        }
        printf("%d\n", base);
    }
    return 0;
}