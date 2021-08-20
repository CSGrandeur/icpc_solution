// difficulty: 4
// Longest Common Substring II
// SAM 入门：理解 SAM 的 len 与 link
// 求多个字符串最长公共子串。
// 1. 对第一个字符串构造 SAM
// 2. 其他字符串在SAM上匹配时，到达每个位置都有一个在该位置的匹配长度，用 nodemax 保存当前字符串在这个位置的匹配长度
// 3. 对于每个节点 cur，其 link 连接的节点 pre 其实也满足匹配到 cur 的匹配长度，用 nodemax[cur] 更新 nodemax[pre] = max(nodemax[pre], min(smn[pre].len, nodemax[cur]));
// 4. 用当前字符串的 nodemax，更新所有字符串在每个节点匹配长度最小值 ans
// 5. 在所有节点的 ans 中取最大值
// 其中，处理 nodemax[pre] 需要一个自后向前的迭代，可以基于节点 len 做一次基数排序，倒序处理。
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<algorithm>
#include<unordered_map>
const int maxn = 5e5 + 10;
struct SAM
{
    struct SamNode
    {
        int len, link;
        std::unordered_map<int, int> nex;
        void Init(int l_ = 0){len = l_; link = -1; nex.clear();}
        SamNode(){Init();}
        SamNode(int l_){Init(l_);}
    };
    std::vector<SamNode> smn;
    int stp, slast;
    SAM(){Init();}
    SAM(int maxn_){smn.resize(maxn_);}
    void Init(){stp = slast = 0; AddNode();}
    void AddNode(int l_ = 0);
    void Add(int c);
    int Nex(int cur, int c) {return smn[cur].nex.count(c) ? smn[cur].nex[c] : 0;}
    SamNode &operator[](int cur) {return smn[cur];}
    template<typename GSTR>
    void Build(GSTR buf[], int len = -1);
    // RadixSort for node topo
    std::vector<int> rcnt, rp;
    void RadixSort();
};
void SAM::AddNode(int l_)
{
    if(stp >= smn.size()) smn.push_back(SamNode(l_));
    else smn[stp].Init(l_);
    stp ++;
}
void SAM::Add(int c)
{
    int cur = stp, p = slast;
    AddNode(smn[slast].len + 1);
    slast = cur;
    for(; p != -1 && !smn[p].nex.count(c); p = smn[p].link)
        smn[p].nex[c] = cur;
    if(p == -1) smn[cur].link = 0;
    else
    {
        int q = smn[p].nex[c];
        if(smn[p].len + 1 == smn[q].len) smn[cur].link = q;
        else
        {
            int clone = stp;
            AddNode();
            smn[clone].len = smn[p].len + 1;
            smn[clone].nex = smn[q].nex;
            smn[clone].link = smn[q].link;
            for(; p != -1 && smn[p].nex[c] == q; p = smn[p].link)
                smn[p].nex[c] = clone;
            smn[q].link = smn[cur].link = clone;
        }
    }
}
template<typename GSTR>
void SAM::Build(GSTR buf[], int len)
{
    Init();
    if(len == -1) len = strlen(buf);
    for(int i = 0; i < len; i ++) Add(buf[i]);
}
void SAM::RadixSort()
{
    if(rcnt.size() < stp) rcnt.resize(stp), rp.resize(stp);
    std::fill(rcnt.data(), rcnt.data() + stp, 0);
    for(int i = 0; i < stp; i ++) rcnt[smn[i].len]++;
    for(int i = 1; i < stp; i ++) rcnt[i] += rcnt[i-1];
    for(int i = 0; i < stp; i ++) rp[-- rcnt[smn[i].len]] = i;
}
char buf[maxn];
int ans[maxn], nodemax[maxn], totalans;
int main()
{
    SAM sam(maxn);
    int n, k;
    while(scanf("%s", buf) != EOF)
    {
        sam.Build(buf);
        sam.RadixSort();
        memset(ans, 0x3f, sizeof(ans));
        int totalans = 0;
        while(scanf("%s", buf) != EOF)
        {
            memset(nodemax, 0, sizeof(int) * sam.stp);
            for(int i = 0, cur = 0, nowlen = 0; buf[i]; i ++)
            {
                while(cur != -1 && !sam.Nex(cur, buf[i]))
                    cur = sam[cur].link, nowlen = cur != -1 ? sam[cur].len : 0;
                if(cur != -1) 
                    cur = sam.Nex(cur, buf[i]), nodemax[cur] = std::max(nodemax[cur], ++ nowlen);
                else cur = nowlen = 0;
            }
            for(int i = sam.stp - 1; i >= 0; i --)
            {
                int cur = sam.rp[i], pre = sam[cur].link;
                if(pre != -1) nodemax[pre] = std::max(nodemax[pre], std::min(sam.smn[pre].len, nodemax[cur]));
                ans[cur] = std::min(ans[cur], nodemax[cur]);
            }
        }
        for(int i = 0; i < sam.stp; i ++)
            if(ans[i] != 0x3f3f3f3f) totalans = std::max(totalans, ans[i]);
        printf("%d\n", totalans);
    }
    return 0;
}