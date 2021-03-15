// Broken Keyboard (a.k.a. Beiju Text) 
// 字符串处理，可以用链表

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
char list[110000];
int nex[110000];
int tp;
const int head = 0;
int rear, p;
inline bool judgec(char c){return isalpha(c) || c == '[' || c == ']' || c == '_';}
void Insert(char c)
{
    if(c == '[')
        p = head;
    else if(c == ']')
        p = rear;
    else
    {
        list[tp] = c, nex[tp] = nex[p], nex[p] = tp, p = tp, tp ++;
        if(nex[p] == 0)
            rear = p;
    }
}
int main()
{
    char c;
    while((c = getchar()) != EOF)
    {
        if(!judgec(c))
            continue;
        rear = p = head;
        tp = 1;
        nex[head] = 0;
        Insert(c);
        while(judgec(c = getchar()))
            Insert(c);
        for(int i = nex[head]; i != 0; i = nex[i])
            putchar(list[i]);
        puts("");
    }
    return 0;
}



/*
// 暴力拼贴
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<queue>
char buf[110000];
char bufCode[110000];
char homeBuf[110000];
char endBuf[110000];
int main()
{
    while(scanf("%s", buf) != EOF)
    {
        homeBuf[0] = endBuf[0] = 0;
        int bufLen = strlen(buf);
        for(int i = 0; buf[i] ; i ++)
        {
            if(buf[i] == '[')
                bufCode[i] = '[', buf[i] = 0;
            else if(buf[i] == ']')
                bufCode[i] = ']', buf[i] = 0;
        }
        for(int i = 0; i < bufLen; i ++)
            if(!buf[i] && bufCode[i] == ']')
                strcat(endBuf, buf + i + 1);
        for(int i = bufLen - 1; i >= 0; i --)
            if(!buf[i] && bufCode[i] == '[')
                strcat(homeBuf, buf + i + 1);
        if(buf[0])
            strcat(homeBuf, buf);
        strcat(homeBuf, endBuf);
        puts(homeBuf);
    }
    return 0;
}
*/