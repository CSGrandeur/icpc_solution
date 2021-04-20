// difficulty: 1
// Entropy
// 此题和HDU1053是同一题，备份
// 哈夫曼编码基础应用

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<queue>
using namespace std;
char buf[1100];
const int MAXCODE = 128;
int cnt[MAXCODE];
int GetHuffmanLength(char st[])
{
    int length = 0;
    priority_queue<int, vector<int>, greater<int> > q;
    memset(cnt, 0, sizeof(cnt));
    for(int i = 0; st[i]; i ++)
        cnt[st[i]] ++;
    for(int i = 0; i < MAXCODE; i ++)
        if(cnt[i]) q.push(cnt[i]);
    for(length = 0; q.size() > 1; )
    {
        int left = q.top(); q.pop();
        int right = q.top(); q.pop();
        length += left + right;
        q.push(left + right);
    }
    if(length == 0 && !q.empty())
        length = q.top();
    return length;
}
int main()
{
    int n, m, length;
    while(scanf("%s", buf) && strcmp(buf, "END"))
    {
        int ascLen = strlen(buf) * 8;
        int huffmanLen = GetHuffmanLength(buf);
        printf("%d %d %.1f\n", ascLen, huffmanLen, (double)ascLen / huffmanLen);
    }
    return 0;
}