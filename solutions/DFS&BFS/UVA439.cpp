// Knight Moves 
// 基础BFS，按棋盘规则搜索，更新步数
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<queue>
bool vis[10][10];
struct Node
{
    int x;
    int y;
    int step;
    Node(){}
    Node(int x_, int y_, int step_): x(x_), y(y_), step(step_){}
};
int dx[8] = {-1, -1, 1, 1, -2, -2, 2, 2};
int dy[8] = {-2, 2, -2, 2, -1, 1, -1, 1};
int sx, sy, ex, ey;
int BFS()
{
    std::queue<Node> q;
    q.push(Node(sx, sy, 0));
    memset(vis, 0, sizeof(vis));
    vis[sx][sy] = true;
    while(!q.empty())
    {
        Node now = q.front();
        q.pop();
        if(now.x == ex && now.y == ey)
            return now.step;
        for(int k = 0; k < 8; k ++)
        {
            int xnex = now.x + dx[k];
            int ynex = now.y + dy[k];
            if(xnex >= 0 && xnex < 8 && ynex >= 0 && ynex < 8 && !vis[xnex][ynex])
            {
                vis[xnex][ynex] = true;
                q.push(Node(xnex, ynex, now.step + 1));
            }
        }
    }
    return -1;
}
int main()
{
    char bufa[5], bufb[5];
    while(scanf("%s%s", bufa, bufb) != EOF)
    {
        sx = bufa[0] - 'a', sy = bufa[1] - '1';
        ex = bufb[0] - 'a', ey = bufb[1] - '1';
        printf("To get from %c%d to %c%d takes %d knight moves.\n", 
            sx + 'a', sy + 1, ex + 'a', ey + 1, BFS());
    }
    return 0;
}