// 拓扑排序入门题
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<queue>
int n, m;
int fst[110], v[10100], nex[10100], ind[110];
int tp;
void AddEdge(int s, int e)
{
    nex[tp] = fst[s];
    fst[s] = tp;
    v[tp] = e;
    ind[e] ++;
    tp ++;
}
void TopoSort()
{
    std::queue<int> q;
    bool flag = false;
    for(int i = 1; i <= n; i ++)
        if(!ind[i])
            q.push(i);
    while(!q.empty())
    {
        int now = q.front();
        q.pop();
        for(int i = fst[now]; i != -1; i = nex[i])
        {
            ind[v[i]] --;
            if(!ind[v[i]])
                q.push(v[i]);
        }
        printf(flag ? " %d" : "%d", now);
        flag = true;
    }
    printf("\n");
}
int main()
{
    int s, e;
    while(scanf("%d%d", &n, &m) && n)
    {
        memset(fst, -1, sizeof(fst));
        memset(ind, 0, sizeof(ind));
        tp = 0;
        for(int i = 0; i < m; i ++)
        {
            scanf("%d%d", &s, &e);
            AddEdge(s, e);
        }
        TopoSort();
    }
    return 0;
}