// difficulty: 2
// Binary Median
// 好多年不做题，果然一个 long long 都 debug好几遍
// binary 是个混淆项，当 long long 处理，map记录被拿掉的数字，拿左边或右边按情况模拟左右移，跳过被拿掉的即可。
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <algorithm>
#define LLI long long
LLI mv_x(LLI x, std::map<LLI, bool> &mp, int dir)
{
    x += dir;
    while (mp.count(x))
    {
        x += dir;
    }
    return x;
}
LLI binary2int(char b[])
{
    LLI ret = 0;
    for (int i = 0; b[i]; i++)
    {
        ret <<= 1;
        ret += b[i] - '0';
    }
    return ret;
}
void int2binary(LLI x, int m, char *b)
{
    for (int i = m - 1; i >= 0; i--)
    {
        b[m - i - 1] = ((x >> i) & 1) + '0';
    }
    b[m] = 0;
}
int main()
{
    int t, n, m;
    LLI a;
    char b[100];
    for (scanf("%d", &t); t--;)
    {
        std::map<LLI, bool> mp;
        scanf("%d%d", &n, &m);
        LLI total = 1LL << m;
        LLI x = (total - 1) >> 1;

        while (n--)
        {
            total--;
            scanf("%s", b);
            a = binary2int(b);
            mp[a] = true;
            if (a > x && total % 2 == 0)
            {
                x = mv_x(x, mp, -1);
            }
            else if (a < x && total % 2)
            {
                x = mv_x(x, mp, 1);
            }
            else if (a == x)
            {
                if (total % 2)
                {
                    x = mv_x(x, mp, 1);
                }
                else
                {
                    x = mv_x(x, mp, -1);
                }
            }
        }
        char res[100];
        int2binary(x, m, res);
        printf("%s\n", res);
    }
    return 0;
}