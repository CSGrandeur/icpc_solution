// 1363C	Game On Leaves
// 是个递推问题，叶节点赢，非叶节点拿掉一个，如果 x 变成叶节点则对方胜，故拿掉一个时要让 x 依然是非叶节点才是最优策略。
// 在此前提下， n 个节点拿掉一个叶节点，则变成 n-1 问题。
// x 非叶节点且当 n=3 时先手败，递推得 x 非叶节点且 n 为偶数时先手胜。
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int t, n, x, indx;
    for (scanf("%d", &t); t--;)
    {
        scanf("%d%d", &n, &x);
        indx = 0;
        for (int i = 0; i < n - 1; i++)
        {
            int u, v;
            scanf("%d%d", &u, &v);
            if (u == x || v == x)
            {
                indx++;
            }
        }
        if (indx == 1 || n == 1 || n % 2 == 0)
        {
            printf("Ayush\n");
        }
        else
        {
            printf("Ashish\n");
        }
    }
    return 0;
}