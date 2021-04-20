// difficulty: 2
// Catch That Cow
// 经典队列模拟题，广度优先搜索
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const int maxn = 210000;
int n, k;
int q[maxn], qt[maxn], front, rear, tmp;
bool vis[maxn];

int main()
{    
    while(scanf("%d%d", &n, &k) != EOF)
    {
        front = rear = 0;
        q[++ rear] = n;
        qt[rear] = 0;
        int now, tm;
        memset(vis, 0, sizeof(vis));
        while(front != rear)
        {
            now = q[++ front % maxn];
            tm = qt[front % maxn];
            if(now == k)
                break;
            if(now << 1 < maxn && !vis[now << 1])
            {
                q[++ rear % maxn] = now << 1;
                qt[rear % maxn] = tm + 1;
                vis[now << 1] = true;
            }
            if(now > 0 && !vis[now - 1])
            {
                q[++ rear % maxn] = now - 1;
                qt[rear % maxn] = tm + 1;
                vis[now - 1] = true;
            }
            if(now + 1 < maxn && !vis[now + 1])
            {
                q[++ rear % maxn] = now + 1;
                qt[rear % maxn] = tm + 1;
                vis[now + 1] = true;
            }
        }
        printf("%d\n", tm);
    }
    return 0;
}