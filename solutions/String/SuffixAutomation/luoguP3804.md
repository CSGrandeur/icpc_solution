### `【模板】后缀自动机 (SAM)`

> difficulty: 3

每个非clone结点计 1 ，按 len 长度排序，倒叙刷表（link）统计结点len的出现次数，更新答案

```cpp
#include<stdio.h>
#include<string.h>
#include<vector>
#include<unordered_map>
const int maxn = 2e6 + 10;
struct SamNode
{
    int len, link;
    std::unordered_map<int, int> nex;
    void Init(int l_ = 0){len = l_; link = -1; nex.clear();}
};
SamNode smn[maxn];
int stp, slast;
char buf[maxn];
int wa[maxn], od[maxn], cnt[maxn];
void SamInit()
{
    stp = slast = 0;
    smn[stp ++].Init();
}
void SamAdd(char c)
{
    int cur = stp ++, p = slast;
    smn[cur].Init(smn[slast].len + 1);
    slast = cur;
    cnt[cur] = 1;
    for(; p != -1 && !smn[p].nex.count(c); p = smn[p].link)
        smn[p].nex[c] = cur;
    if(p == -1) smn[cur].link = 0;
    else
    {
        int q = smn[p].nex[c];
        if(smn[p].len + 1 == smn[q].len) smn[cur].link = q;
        else
        {
            int clone = stp ++;
            cnt[clone] = 0;
            smn[clone].len = smn[p].len + 1;
            smn[clone].nex = smn[q].nex;
            smn[clone].link = smn[q].link;
            for(; p != -1 && smn[p].nex[c] == q; p = smn[p].link)
                smn[p].nex[c] = clone;
            smn[q].link = smn[cur].link = clone;
        }
    }
}
int main()
{
    long long ans;
    while(scanf("%s", buf) != EOF)
    {
        SamInit();
        for(int i = 0; buf[i]; i ++) SamAdd(buf[i]);
        memset(wa, 0, sizeof(int) * (stp + 1));
        int maxlen = 0;
        for(int i = 0; i < stp; i ++) wa[smn[i].len] ++;
        for(int i = 1; i <= stp; i ++) wa[i] += wa[i - 1];
        for(int i = 0; i < stp; i ++) od[wa[smn[i].len] --] = i;
        ans = 0;
        for(int i = stp; i >= 0; i --)
        {
            int now = od[i];
            if(smn[now].link != -1) cnt[smn[now].link] += cnt[now];
            if(cnt[now] > 1) ans = std::max(1LL * smn[now].len * cnt[now], ans);
        } 
        printf("%lld\n", ans);
    }
    return 0;
}
```

