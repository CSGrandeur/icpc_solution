// difficulty: 3
// String Commutativity
// 比划一下可以发现，si+sj==sj+si 等价于 si 与 sj 有相同的最小循环节，循环节是 KMP 的经典功能
// 记录相同循环节的字符串个数即可计算答案。
// 特别注意必须计算完整循环节，比如 abcabca，len - nex[len]计算的循环节是 3，但末尾存在不完整部分，需要判断是否完整循环，此时实际循环节是 len。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string>
#include<unordered_map>
using namespace std;
const int maxn = 1e5 + 10;
const int maxs = 1e6 + 10;
int n;
char ps[maxs];
int nex[maxs];
void BuildNext(char ps[], int nex[])
{
    nex[0] = -1;
    for(int i = 0, j = -1; ps[i]; )
    {
        if(j == -1 || ps[i] == ps[j])
            nex[++i] = ++j;
        else
            j = nex[j];
    }
}
int main()
{
    unordered_map<string, int> mp;
    while(scanf("%d", &n) != EOF)
    {
        mp.clear();
        long long ans = 0;
        for(int i = 0; i < n; i ++)
        {
            scanf("%s", ps);
            BuildNext(ps, nex);
            int len = strlen(ps);
            if(len % (len - nex[len]) == 0)
                ps[len - nex[len]] = 0;
            if(!mp.count(ps)) mp[ps] = 0;
            mp[ps] ++;
        }
        for(auto it : mp)
            ans += 1LL * it.second * (it.second - 1) >> 1;
        printf("%lld\n", ans);
    }

    return 0;
}