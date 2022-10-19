// difficulty: 1
// Chip Game
// 2堆nim游戏，稳态是两个方向差距为偶数
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
int main()
{
    int t, n, m;
    for(scanf("%d", &t); t --; )
    {
        scanf("%d%d", &n, &m);
        printf(n + m & 1 ? "Burenka\n" : "Tonya\n");
    }
    return 0;
}