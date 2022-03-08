// difficulty: 1
// Division
// 0~9拼成两个允许有前导0的五位数，相除得输入的数
// 暴力枚举`0~9`全排列超时了，其实枚举分子的五位数排列组合即可
// 分母由分子除以输入的数得到，加上判断。
// 输出有奇怪的空行要求时要看清是"每组后空行"还是"两组间空行"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int vis, n;
bool flag;
void DFS(int now, int a)
{
    if(now == 5)
    {
        if(a % n != 0)
            return;
        int b = a / n;
        int tmp = vis;
        for(int i = 0, j = b; i < 5; i ++, j /= 10)
        {
            if(tmp >> j % 10 & 1)
                return;
            tmp |= 1 << j % 10;
        }
        flag = true;
        printf("%05d / %05d = %d\n", a, b, n);
        return;
    }
    for(int i = 0; i < 10; i ++)
        if(~vis >> i & 1)
        {
            vis |= (1 << i);
            DFS(now + 1, a * 10 + i);
            vis ^= (1 << i);
        }
}
int main()
{
    bool startFlag = false;
    while(scanf("%d", &n) && n)
    {
        if(startFlag) printf("\n");
        startFlag = true;
        vis = flag = 0;
        DFS(0, 0);
        if(!flag)
            printf("There are no solutions for %d.\n", n);
    }
    return 0;
}