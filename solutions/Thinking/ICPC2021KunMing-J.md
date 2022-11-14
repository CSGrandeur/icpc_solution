### `Parallel Sort`

> difficulty: 3

思维题，用笔画一画会清晰些

每个数朝着目标位置连一条边，很容易想到主要需要处理"环"

发现无论环长度奇数还是偶数，多边形平行角互换总能把环拆成两两成对，即不再有大于 2 的长度的环

因而最多执行 2 个 round

```cpp

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<algorithm>
const int maxn = 110000;
int n, p[maxn], rp[maxn];
std::vector<std::vector<int> > res;
bool vis[maxn];
int main()
{
    while(scanf("%d", &n) != EOF)
    {
        res.clear();
        for(int i = 1; i <= n; i ++)
            scanf("%d", &p[i]), rp[p[i]] = i;
        while(true)
        {
            std::vector<int> tmpRes;
            memset(vis, 0, sizeof(vis));
            for(int i = 1; i <= n; i ++)
            {
                if(p[i] == i || vis[i])
                    continue;
                for(int j = i, k = p[i]; ; j = rp[j], k = p[k])
                {
                    vis[j] = vis[k] = true;
                    tmpRes.push_back(j);
                    tmpRes.push_back(k);
                    if(j == k || rp[j] == k)
                        break;
                }
            }
            if(tmpRes.size() == 0)
                break;
            for(int i = 0; i < tmpRes.size(); i += 2)
            {
                std::swap(rp[p[tmpRes[i]]], rp[p[tmpRes[i + 1]]]);
                std::swap(p[tmpRes[i]], p[tmpRes[i + 1]]);
            }
            res.push_back(tmpRes);
        }
        printf("%d\n", (int)res.size());
        for(int i = 0; i < res.size(); i ++)
        {
            printf("%d", (int)(res[i].size() >> 1));
            for(int j = 0; j < res[i].size(); j += 2)
                printf(" %d %d", res[i][j], res[i][j + 1]);
            printf("\n");
        }
    }
    return 0;
}
```

