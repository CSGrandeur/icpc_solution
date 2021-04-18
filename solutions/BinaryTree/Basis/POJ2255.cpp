// difficulty: 2
// Tree Recovery
// 二叉树基于两种遍历序列构建，可以取巧递归输出，不必把树显式地建出来

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char preord[110], inord[110];
void MakePost(int inleft, int inright, int prefirst)
{
    if(inleft >= inright)
        return;
    int inroot = strchr(inord, preord[prefirst]) - inord;
    MakePost(inleft, inroot, prefirst + 1);
    MakePost(inroot + 1, inright, prefirst + inroot - inleft + 1);
    printf("%c", preord[prefirst]);
}
int main()
{
    while(scanf("%s%s", preord, inord) != EOF)
    {
        MakePost(0, strlen(inord), 0);
        printf("\n");
    }
    return 0;
}