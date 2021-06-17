// difficulty: 3
// Chip Factory
// 异或最大值升级版，对每个数二进制建Trie，枚举两数结合，在Trie上尽可能反向移动
// 特别地，枚举的两个数要先在 Trie 上删除

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<algorithm>
const int maxn = 1e3 + 10;
int a[maxn << 5 | 1], n;
struct Node
{
    int cnt;
    int nex[2];
    void Init() {nex[0] = nex[1] = -1; cnt = 0;}
};
Node trie[maxn << 5];
int tp;
void Insert(int x)
{
    int now = 0;
    for(int i = 30; i >= 0; i --)
    {
        int ith1 = x >> i & 1;
        if(trie[now].nex[ith1] == -1) 
        {
            trie[tp].Init();
            trie[now].nex[ith1] = tp ++;
        }
        trie[now].cnt ++;
        now = trie[now].nex[ith1];        
    }
    trie[now].cnt ++;
}
int Find(int x, int rev=0)
{
    int now = 0, tmpx = 0;
    for(int i = 30; i >= 0; i --)
    {
        int ith1 = (x >> i & 1) ^ rev;
        if(rev && (trie[now].nex[ith1] == -1 || !trie[trie[now].nex[ith1]].cnt)) ith1 ^= 1;
        if(trie[now].cnt == 0 || trie[now].nex[ith1] == -1) return -1;
        now = trie[now].nex[ith1];
        tmpx |= ith1 << i;     
    }
    return trie[now].cnt > 0 ? tmpx : -1;
}
void Delete(int x)
{
    int now = 0;
    // if(Find(x) == -1) return;
    for(int i = 30; i >= 0; i --)
    {
        int ith1 = x >> i & 1;
        trie[now].cnt --;
        now = trie[now].nex[ith1];
    }
    trie[now].cnt --;
}
int main()
{
    int t;
    for(scanf("%d", &t); t --; )
    {
        scanf("%d", &n);
        trie[0].Init();
        tp = 1;
        for(int i = 0; i < n; i ++)
            scanf("%d", &a[i]), Insert(a[i]);
        int ans = 0;
        for(int i = 0; i < n; i ++)
        {
            Delete(a[i]);
            for(int j = i + 1; j < n; j ++)
            {
                Delete(a[j]);
                ans = std::max(ans, Find(a[i] + a[j], 1) ^ (a[i] + a[j]));
                Insert(a[j]);
            }
            Insert(a[i]);
        }
        printf("%d\n", ans);
    }
    return 0;
}