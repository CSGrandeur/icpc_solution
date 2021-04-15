// difficulty: 1
// Corporate Identity
// 和 POJ 2185 Milking Grid 一模一样

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char s[4100][210];
char ps[210];
char ans[210];
int main()
{
    int m;
    while(scanf("%d", &m) && m)
    {
        strcpy(ans, "~");
        for(int i = 0; i < m; i ++)
            scanf("%s", s[i]);
        int i, j, k;
        int s0Len = strlen(s[0]);
        for(i = strlen(s[0]); i > 0; i --)
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
            if(strcmp(ans, "~"))
                break;
        }
        printf("%s\n", !i ? "IDENTITY LOST" : ans);
    }
    return 0;
}