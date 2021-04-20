// difficulty: 2
// Dropping Balls 
// 题目可以有多种考虑方式。比较好想的就是最终结果是一个二进制数，第x个小球每一层都在决定一个二进制位。

#include<stdio.h>
int main()
{
    int t, d, I, res;
    for(scanf("%d%d%d", &t, &d, &I); t --; scanf("%d%d", &d, &I))
    {
        for(res = 1, I --; -- d; I >>= 1)
            res = res << 1 | I & 1;
        printf("%d\n", res);
    }
    return 0;
}