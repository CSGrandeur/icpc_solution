// Play on Words 
// 单个字母视作结点，则单词为连接首尾字母的边。
// 图连通 + 欧拉通路/回路判断
// 连通判断：用并查集
// 欧拉通路：首结点出度入度-入度==1，尾结点入度-出度==1，其余入度==出度
// 欧拉回路：所有结点入度==出度
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
char buf[1100];
int ind[129], ond[129];
int p[129];
int fa(int x) {return p[x] = x == p[x] ? x : fa(p[x]);}
bool Judge()
{
    int last = -1;
    for(int i = 0; i < 129; i ++)
    {
        if((ind[i] || ond[i]) && last != fa(i))
        {
            if(last == -1) last = fa(i);
            else return false;
        }    
    }
    int icnt = 0, ocnt = 0;
    for(int i = 0; i < 128; i ++)
    {
        icnt += ind[i] > ond[i] ? ind[i] - ond[i] : 0;
        ocnt += ind[i] < ond[i] ? ond[i] - ind[i] : 0;
    }
    return icnt == 1 && ocnt == 1 || icnt == 0 && ocnt == 0;
}
int main()
{
    int t, n;
    for(scanf("%d", &t); t --; )
    {
        memset(ind, 0, sizeof(ind));
        memset(ond, 0, sizeof(ond));
        for(int i = 0; i < 129; i ++)
            p[i] = i;
        scanf("%d", &n);
        for(int i = 0; i < n; i ++)
        {
            scanf("%s", buf);
            int s = buf[0], e = buf[strlen(buf) - 1];
            ind[s] ++;
            ond[e] ++;
            p[fa(s)] = fa(e);
        }
        printf(Judge() ? "Ordering is possible.\n" : "The door cannot be opened.\n");
    }
    return 0;
}