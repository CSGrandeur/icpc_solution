// difficulty: 4
// Distinct Substrings
// 这题正解是扩展KMP
// 这里用后缀自动机（SAM）水过：n个数建SAM，后1~m分别"虚假地"插入SAM，即只插入cur结点，但所有涉及修改其它结点的操作都跳过
// 此时 len[cur] - len[link[cur]] 就是新加数字的新增不重复后缀个数
#include<stdio.h>
#include<string.h>
#include<vector>
#include<unordered_map>
const int maxn = 4e6 + 10;
const int mod = 1e9 + 7;
struct SamNode
{
    int len, link;
    std::unordered_map<int, int> nex;
    void Init(int l_ = 0){len = l_; link = -1; nex.clear();}
};
SamNode smn[maxn];
int stp, slast;
int buf[maxn];
int wa[maxn], od[maxn], cnt[maxn];
int n, m;
void SamInit()
{
    stp = slast = 0;
    smn[stp ++].Init();
}
int SamAdd(int c, bool lastFlag = true)
{
    int cur = stp ++, p = slast;
    smn[cur].Init(smn[slast].len + 1);
    if(lastFlag) slast = cur;
    cnt[cur] = 1;
    for(; p != -1 && !smn[p].nex.count(c); p = smn[p].link)
        if(lastFlag) smn[p].nex[c] = cur;
    if(p == -1) {if(lastFlag) smn[cur].link = 0;}
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
                if(lastFlag) smn[p].nex[c] = clone;
            if(lastFlag) smn[q].link = clone;
            smn[cur].link = clone;
        }
    }
    return smn[cur].link == -1 ? smn[cur].len : smn[cur].len - smn[smn[cur].link].len;
}
int main()
{
    while(scanf("%d%d", &n, &m) != EOF)
    {
        SamInit();
        for(int i = 0; i < n; i ++)
        {
            scanf("%d", &buf[i]);
            SamAdd(buf[i]);
        }
        int t3 = 3, ans = 0;
        for(int i = 1; i <= m; i ++, t3 = 1LL * t3 * 3 % mod)
        {
            int tmp = SamAdd(i, false);
            ans ^= 1LL * tmp * t3 % mod;
        }
        printf("%d\n", ans);
    }
    return 0;
}