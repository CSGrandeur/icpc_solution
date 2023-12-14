### `动物园`

> difficulty: 3

求每个前缀 `i` 的不超过`i`一半的相等前后缀个数 `num[i]` 按 `(num[i] + 1)`乘起来的结果.

`nex[i]`迭代到`i`的一半后，剩下的迭代次数就是`num[i]`.

- 前一半迭代，可以特判循环节跳到接近一半的位置
- 后一半迭代次数，可以dp记录一下

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string>
#include<unordered_map>
using namespace std;
const int maxs = 1e6 + 10;
const int mod = 1e9 + 7;
int n;
char ts[maxs];
int nex[maxs], dp[maxs];

void BuildNex(char ts[], int nex[]) {
    nex[0] = -1;
    for(int i = 0, j = -1; ts[i]; ) {
        if(j == -1 || ts[i] == ts[j])
            nex[++i] = ++j;
        else
            j = nex[j];
    }
}
int main() {
    int n, ans;
    for(scanf("%d", &n); n --; ) {
        scanf("%s", ts);
        BuildNex(ts, nex);
        ans = 1;
        int len = strlen(ts);
        memset(dp,0, sizeof(dp));
        for(int i = 2; i <= len; i ++) {
            if(nex[i] > 0) {
                dp[i] = dp[nex[i]] + 1;
            }
            int j = i, cs = j - nex[j];
            if(cs < i / 2) j = i - (i / 2 / cs) * cs;
            for(; j > i / 2; j = nex[j]);
            if(j > 0) {
                ans = 1LL * ans * (dp[j] + 2) % mod;
            }            
        }
        printf("%d\n", ans);
    }
    return 0;
}
```
