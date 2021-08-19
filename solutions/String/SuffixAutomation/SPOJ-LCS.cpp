// difficulty: 2
// Longest Common Substring 
// SAM 入门：理解 SAM 的 len 与 link
// 求两个字符串最长公共子串。第一个字符串构造 SAM ，第二个字符串在第一个字符串遍历，失配时沿 link 走。
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
    void AddNode(int l_ = 0)
    {
        if(stp >= smn.size()) smn.push_back(SamNode(l_));
        else smn[stp].Init(l_);
        stp ++;
    }
    void Add(int c)
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
    void Build(char buf[])
    {
        Init();
        for(int i = 0; buf[i]; i ++)
            Add(buf[i]);
    }
};
char buf[maxn];
int main()
{
    SAM sam(maxn);
    int n, k;
    while(scanf("%s", buf) != EOF)
    {
        sam.Build(buf);
        scanf("%s", buf);
        int ans = 0, nowlen = 0;
        for(int i = 0, cur = 0; buf[i]; i ++)
        {
            if(sam.smn[cur].nex.count(buf[i]))
            {
                nowlen ++;
                ans = std::max(ans, nowlen);
                cur = sam.smn[cur].nex[buf[i]];
            }
            else
            {
                while(cur != -1 && !sam.smn[cur].nex.count(buf[i]))
                    cur = sam.smn[cur].link;
                if(cur == -1) nowlen = cur = 0;
                else 
                {
                    nowlen = sam.smn[cur].len + 1;
                    cur = sam.smn[cur].nex[buf[i]];
                    ans = std::max(ans, nowlen);
                }
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}