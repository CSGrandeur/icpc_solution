// difficulty: 2
// Array Cloning Technique
// 用最多的数置换其它数。
// 这题最没意义的地方在于统计最多的数的时候，`map`能过，`unorderd_map`会超时。
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<vector>
#include<map>
const int maxn = 1e5 + 10;

int n, a;

int main()
{
    int t, sum;
    for(scanf("%d", &t); t --; )
    {
        std::map<int, int> mp;
        scanf("%d", &n);
        sum = 0;
        for(int i = 0; i < n; i ++)
        {
            scanf("%d", &a);
            mp[a] ++;
            sum = std::max(sum, mp[a]);
        }
        int cnt = 0, lft = n - sum;
        while(lft > 0)
        {
            int tmp = std::min(sum, lft);
            cnt += tmp + 1;
            lft -= tmp;
            sum += tmp;
        }
        printf("%d\n", cnt);
    }
    return 0;
}