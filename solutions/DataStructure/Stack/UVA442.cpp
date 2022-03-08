// difficulty: 2
// Matrix Chain Multiplication
// 题目数据给的友好，矩阵乘必有括号，括号内必是两个矩阵相乘，可以嵌套。
// 这样就可以遇到字母即入栈，遇到")"就出栈两个矩阵，相乘结果再入栈。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<queue>
struct Node
{
    int value;
    bool vis;
    int left;
    int right;
};
Node tree[300];
int tp, nodeNum;
char buf[100];
bool printFlag;
int Insert(int now, int num, char dire[])
{
    if(!dire[0])
    {
        if(tree[now].vis)
            return -1;
        tree[now].vis = true;
        tree[now].value = num;
        return 0;
    }
    int &nodeDire = dire[0] == 'L' ? tree[now].left : tree[now].right;
    if(nodeDire == 0)
        nodeDire = tp ++;
    return Insert(nodeDire, num, dire + 1);
}
void Show(int now)
{
    std::queue<int> q;
    q.push(0);
    while(!q.empty())
    {
        now = q.front();
        q.pop();
        if(tree[now].vis)
            printf(printFlag ? " %d" : "%d", tree[now].value), printFlag = true;
        if(tree[now].left)
            q.push(tree[now].left);
        if(tree[now].right)
            q.push(tree[now].right);
    } 
    printf("\n");
}
int main()
{
    int num;
    char tmp[100];
    bool flag;
    while(scanf("%s", buf) != EOF)
    {
        memset(tree, 0, sizeof(tree));
        tp = 1;
        nodeNum = 0;
        flag = true;
        do
        {
            if(strstr(buf, ",)"))
                sscanf(buf, "(%d,)", &num), tmp[0] = 0;
            else
                sscanf(buf, "(%d,%s", &num, tmp), tmp[strlen(tmp) - 1] = 0;
            if(flag && Insert(0, num, tmp) == -1)
                flag = false;
            nodeNum ++;
            scanf("%s", buf);
        } while(strcmp(buf, "()"));
        if(flag && nodeNum == tp)
            printFlag = false, Show(0);
        else
            printf("not complete\n");
    }
    return 0;
}