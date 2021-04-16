// difficulty: 1
// Bandwidth
// 输入格式稍有点麻烦，数据规模很小可以直接暴力
// 也可以练习一下剪枝方法，这里代码就直接暴力了

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<algorithm>

const int BUFLEN = 1e6 + 10;
char buf[110];
bool g[8][8];
int vis;
int ndList[10], ntp;
int res[10];
int ansList[10], ans;
void ReadGraph()
{
    memset(g, 0, sizeof(g));
    bool ndFlag = false;
    char fst;
    vis = ntp = 0;
    for(int i = 0; buf[i]; i ++)
    {
        if(buf[i] == ';') ndFlag = false;
        else if(buf[i] == ':') ndFlag = true;
        else if(isalpha(buf[i]))
        {
            int nd = buf[i] - 'A';
            if(!ndFlag) fst = nd;
            else g[fst][nd] = g[nd][fst] = true;
            if(~vis >> nd & 1) ndList[ntp ++] = nd, vis |= 1 << nd;
        }        
    }
    std::sort(ndList, ndList + ntp);
}
void DFS(int now)
{
    if(now == ntp)
    {
        int ret = 0;
        for(int i = 0; i < ntp; i ++)
            for(int j = i + 1; j < ntp; j ++)
                ret = std::max(ret, g[res[i]][res[j]] ? j - i : 0);
        if(ret < ans)
            memcpy(ansList, res, sizeof(res)), ans = ret;
        return ;
    }
    int ret = 0;
    for(int i = 0; i < ntp; i ++)
    {
        if(vis >> ndList[i] & 1)
            continue;
        res[now] = ndList[i];
        vis |= 1 << ndList[i];
        DFS(now + 1);
        vis ^= 1 << ndList[i];
    }
    return;
}
int main()
{
    while(fgets(buf, BUFLEN, stdin) && buf[0] != '#')
    {
        buf[strlen(buf) - 1] = 0;  // fgts会得到\n
        ReadGraph();
        vis = 0;
        ans = 0x3f3f3f3f;
        DFS(0);
        for(int i = 0; i < ntp; i ++)
            printf("%c ", ansList[i] + 'A');
        printf("-> %d\n", ans);
    }
    return 0;
}