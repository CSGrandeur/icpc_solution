// difficulty: 1
// 2020
// C语言基础
#include<stdio.h>
#include<string.h>
int n;
char buf[110000];
int main()
{
    while(scanf("%d", &n) != EOF)
    {
        scanf("%s", buf);
        char *b = buf;
        int cnt = 0;
        for(; (b = strstr(b, "2020")) != NULL; b += 4)
            cnt ++;
        printf("%d\n", cnt);
    }
    return 0;
}