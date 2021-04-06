// Oil Deposits
// 直接搜索找连通域，可以用并查集
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char buf[110][110];
int p[11111];
bool vis[11111];
int fa(int now)
{
    return p[now] = now == p[now] ? now : fa(p[now]);
}
int main()
{
    int n, m;
    int dx[10] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int dy[10] = {-1, -1, -1, 0, 0, 1, 1, 1};
    while(scanf("%d%d", &n, &m) && n != 0 && m != 0)
    {
        memset(buf, 0, sizeof(buf));
        for(int i = 1; i <= n; i ++)
            scanf("%s", buf[i] + 1);
        for(int i = (n + 1) * (m + 1); i >= 0; i --)
            p[i] = i;
        for(int i = 1; i <= n; i ++)
        {
            for(int j = 1; j <= m; j ++)
            {
                for(int k = 0; k < 8; k ++)
                {
                    int ti = i + dx[k];
                    int tj = j + dy[k];
                    if(buf[i][j] == '@' && buf[ti][tj] == '@')
                    {
                        p[fa((i - 1) * m + j - 1)] = fa((ti - 1) * m + tj - 1);
                    }
                }
            }
        }
        memset(vis, 0, sizeof(vis));
        int ans = 0;
        for(int i = n * m; i >= 0; i --)
        {
            if(buf[i / m + 1][i % m + 1] == '@' && !vis[fa(i)])
                vis[fa(i)] = true, ans ++;
        }
        printf("%d\n", ans);
    }
    return 0;
}