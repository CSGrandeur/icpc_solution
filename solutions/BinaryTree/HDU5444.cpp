/*
二叉树基础题。  
题意为先给数据按顺序建一个常规的二叉树，然后给一系列提问，问的每个编号，输出从根节点走到这个编号的路径。  
路径用东（E）西（W）表示，其实就是左子树、右子树。

建树和查询可以复用代码，加个flag。
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
struct Node
{
    int left;
    int right;
    int value;
};
Node tree[1111];
int tp;
void GoTree(int v, int pnow, bool queryFlag=false)
{
    if(tree[pnow].value == -1)
    {
        if(!queryFlag)
            tree[pnow].value = v;
        return;
    }
    if(v > tree[pnow].value)
    {
        if(queryFlag)
            printf("W");
        else if(tree[pnow].right == -1)
            tree[pnow].right = tp ++;
        if(tree[pnow].right != -1)
            GoTree(v, tree[pnow].right, queryFlag);
    }
    else if(v < tree[pnow].value)
    {
        if(queryFlag)
            printf("E");
        else if(tree[pnow].left == -1)
            tree[pnow].left = tp ++;
        if(tree[pnow].left != -1)
            GoTree(v, tree[pnow].left, queryFlag);
    }
}
int main()
{
    int t, n, q, a;
    for(scanf("%d", &t); t --; )
    {
        scanf("%d", &n);
        tp = 1;
        memset(tree, -1, sizeof(tree));
        while(n --)
        {
            scanf("%d", &a);
            GoTree(a, 0);
        }
        scanf("%d", &q);
        while(q --)
        {
            scanf("%d", &a);
            GoTree(a, 0, true);
            puts("");
        }
    }
    return 0;
}