// difficulty: 2
// Fence Repair
// 为赫夫曼树而出的题，还是赫夫曼树求权重和（最短编码长度）
// 坑点1：与编码不同，如果只有一根木板而且不需要砍的话，返回0
// 坑点2：直观看数据范围的积不大于 int，却容易忽视最终结果很多数会重复加，依然需要 long long
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<queue>
using namespace std;
typedef long long LL;
const int maxn = 21000;
const int maxl = 51000;
int n, cnt[maxn];

LL GetHuffmanLength()
{
    LL length = 0;
    priority_queue<LL, vector<LL>, greater<LL> > q;
    for(int i = 0; i < n; i ++)
        q.push(cnt[i]);
    for(length = 0; q.size() > 1; )
    {
        int left = q.top(); q.pop();
        int right = q.top(); q.pop();
        length += left + right;
        q.push(left + right);
    }
    return length;
}
int main()
{
    while(scanf("%d", &n) != EOF)
    {
        for(int i = 0; i < n; i ++)
            scanf("%d", &cnt[i]);
        printf("%lld\n", GetHuffmanLength());
    }
    return 0;
}