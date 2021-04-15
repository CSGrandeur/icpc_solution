// difficulty: 2
// The Donkey of Gui Zhou
// 模拟操作即可。
// 一开始没考虑窝在那里不动等，处理这个逻辑一片狼藉，代码极不优雅。。

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

const int maxn = 1100;
struct QNode
{
    int x, y, d, pace;
    QNode(){}
    QNode(int x_, int y_, int d_, int p_)
    {
        x = x_;
        y = y_;
        d = d_;
        pace = p_;
    }
};

int n;
int g[maxn][maxn], dunkeyLastPace;
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};
inline bool CanGo(int x, int y, bool dt)
{
    return x >= 0 && x < n && y >= 0 && y < n && (!dt && !g[y][x] || dt && g[y][x] >= 0);
}
inline bool JudgeGo(QNode now, QNode &nex, bool dt)
{
    bool goFlag = false;
    int turn = 0;
    for(int turn = 0; !goFlag && turn < 2; turn ++, goFlag = CanGo(nex.x, nex.y, dt))
    {
        nex = QNode(now.x + dx[now.d], now.y + dy[now.d], now.d, now.pace + 1);
        now.d = (now.d + (dt ? 3 : 1)) % 4;
    }
    return goFlag;
}
void Run(int r, int c, int d, bool dt=false)
{
    QNode now, nex;
    if(!dt)
        g[r][c] = 1;
    for(now = QNode(c, r, d, 1); ; now = nex)
    {
        if(dt && (g[now.y][now.x] == now.pace || g[now.y][now.x] == dunkeyLastPace && now.pace > dunkeyLastPace))
        {
            printf("%d %d\n", now.y, now.x);
            return;
        }
        if(!JudgeGo(now, nex, dt))
        {
            if(dt && now.pace < g[now.y][now.x])
            {
                printf("%d %d\n", now.y, now.x);
                return;
            }
            if(!dt)
            {
                dunkeyLastPace = now.pace;
                g[now.y][now.x] = now.pace;
            }
            break;
        }
        g[now.y][now.x] = now.pace * (dt ? -1 : 1);
    }
    if(dt)
        printf("-1\n");
}
int main()
{

    while(scanf("%d", &n) && n)
    {
        int r, c, d;
        memset(g, 0, sizeof(g));
        scanf("%d%d%d", &r, &c, &d);
        Run(r, c, d);
        scanf("%d%d%d", &r, &c, &d);
        Run(r, c, d, true);
    }
    return 0;
}
