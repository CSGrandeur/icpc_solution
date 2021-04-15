// difficulty: 1
// Blue Jeans
// 这并不是个KMP的题，用KMP约等于暴力
// 放在KMP专题里，就暂时不讨论其它算法了，懒得写KMP，随便strstr吧

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char s[11][100];
char ps[100];
char ans[100];
int main()
{
    int t, m;
    for(scanf("%d", &t); t --; )
    {
        strcpy(ans, "Z");
        scanf("%d", &m);
        for(int i = 0; i < m; i ++)
            scanf("%s", s[i]);
        int i, j, k;
        int s0Len = strlen(s[0]);
        for(i = strlen(s[0]); i >= 3; i --)
        {
            for(j = 0; j <= s0Len - i; j ++)
            {
                strncpy(ps, s[0] + j, i);
                ps[i] = 0;
                for(k = 1; k < m && strstr(s[k], ps); k ++);
                if(k != m)
                    continue;
                if(strcmp(ans, ps) > 0)
                    strcpy(ans, ps);
            }
            if(strcmp(ans, "Z"))
                break;
        }
        printf("%s\n", i < 3 ? "no significant commonalities" : ans);
    }
    return 0;
}