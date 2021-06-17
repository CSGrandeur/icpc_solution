// difficulty: 1
// 2020 vs 2018
// 图像处理特征提取启蒙，做法可以很多。用上下、左右两个方向都不连续三个“o”当作角点，数角点个数可以区分两个数字。
// 也可以用搜索找洞的个数之类的，某年 Word Final 的打卡题套路。
#include<stdio.h>
#include<string.h>
int n, m;
char buf[55][55];
int Corner(int x, int y)
{
    if(buf[y][x] != 'o') return 0;
    return (buf[y - 1][x] == 'o') + (buf[y + 1][x] == 'o') < 2 && (buf[y][x + 1] == 'o') + (buf[y][x - 1] == 'o') < 2;
}
int main()
{
    while(scanf("%d%d", &n, &m) != EOF)
    {
        memset(buf, '.', sizeof(buf));
        for(int i = 1; i <= n; i ++)
            scanf("%s", buf[i] + 1);
        int cnt = 0;
        for(int i = 1; i <= n; i ++)
            for(int j = 1; j <= m; j ++)
                cnt += Corner(j, i);
        printf("%d\n", cnt == 16 ? 2018 : 2020);
    }
    return 0;
}