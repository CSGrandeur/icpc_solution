// difficulty: 2
// Milking Grid
// 二维的循环节
// 方法1：每行、每列分别用KMP求循环节，取循环节最小公倍数
// 方法2：整行、整列当作整体，横纵做两次KMP求横纵循环节


// 方法1
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const int maxn = 1e4 + 10;
const int maxm = 110;
char sg[maxn][maxm];
int nex[maxn];
int r, c;
void BuildNext(char st[], int nex[])
{
    nex[0] = -1;
    for(int i = 0, j = -1; st[i]; )
    {
        if(j == -1 || st[i] == st[j])
            nex[++ i] = ++ j;
        else
            j = nex[j];
    }
}
int GCD(int a, int b)
{
    return b ? GCD(b, a % b) : a;
}
int LCM(int a, int b, int limit)
{
    if(a == limit)
        return a;
    long long tmp = (long long)a * b / GCD(a, b);
    return tmp > limit ? limit : (int)tmp;
}
int main()
{
    int t, ansc, ansr;
    while(scanf("%d%d", &r, &c) != EOF)
    {
        for(int i = 0; i < r; i ++)
            scanf("%s", sg[i]);
        ansc = 1;
        for(int i = 0; i < r; i ++)
        {
            BuildNext(sg[i], nex);
            ansc = LCM(ansc, c - nex[c], c);
        }
        ansr = 1;
        for(int i = 0; i < c; i ++)
        {
            for(int j = 0; j < r; j ++)
                sg[r][j] = sg[j][i];
            sg[r][r] = 0;
            BuildNext(sg[r], nex);
            ansr = LCM(ansr, r - nex[r], r);
        }
        printf("%d\n", ansr * ansc);
    }
    return 0;
}


// 方法2
/*
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const int maxn = 1e4 + 10;
const int maxm = 110;
char sg[maxn][maxm];
char sgT[maxm][maxn];
int nex[maxn];
int r, c;
void BuildNext(int nex[], bool flagT)
{
    nex[0] = -1;
    for(int i = 0, j = -1; i < (flagT ? c : r); )
    {
        if(j == -1 || (flagT ? !strcmp(sgT[i], sgT[j]) : !strcmp(sg[i], sg[j])))
            nex[++ i] = ++ j;
        else
            j = nex[j];
    }
}
int main()
{
    int t, ansc, ansr;
    while(scanf("%d%d", &r, &c) != EOF)
    {
        for(int i = 0; i < r; i ++)
        {
            scanf("%s", sg[i]);
            for(int j = 0; sg[i][j]; j ++)
                sgT[j][i] = sg[i][j];
        }
        BuildNext(nex, true);
        ansr = c - nex[c];
        BuildNext(nex, false);
        ansc = r - nex[r];
        printf("%d\n", ansr * ansc);
    }
    return 0;
}
*/