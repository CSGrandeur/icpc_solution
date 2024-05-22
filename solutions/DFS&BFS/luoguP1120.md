### `小木棍`

> difficulty: 4

1. 从最大木棍的长度开始枚举
2. 枚举能整除总和的
3. （重要）每次凑特定长度使用的第一根必定是当前最长的，否则不用再试
4. （重要）每次凑齐一根后，后续凑新的尝试失败了，则不用再试

```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
#include<functional>
int n, ans, mx, sum, st, tp, len[111], cnt[111], tcnt[111];
bool DFS(int last, int remain_now, int remain_total) {
    if(remain_total == ans) {
        return true;
    }
    for(int i = last; i < tp; i ++) {
        if(!cnt[i] || len[i] > remain_now) continue;
        int nex_remain = remain_now - len[i];
        cnt[i] --;
        if(DFS(nex_remain ? i : 0, nex_remain ? nex_remain : ans, remain_total - len[i])) {
            return true;
        }
        cnt[i] ++;
        if(remain_now == ans || remain_now == len[i]) {
            return false;
        }
    }
    return false;
}
int main() {
    while(scanf("%d", &n) != EOF && n) {
        sum = mx = 0; 
        memset(tcnt, 0, sizeof(tcnt));
        for(int i = 0; i < n; i ++) {
            scanf("%d", &st);
            tcnt[st] ++;
            mx = std::max(st, mx);
            sum += st;
        }
        tp = 0;
        for(int i = 50; i; i --) {
            if(tcnt[i]) {
                len[tp] = i;
                cnt[tp] = tcnt[i];
                tp ++;
            }
        }
        for(ans = mx; ans < sum; ans ++) {
            if(sum % ans) continue;
            if(DFS(0, ans, sum)) break;
        }
        printf("%d\n", ans);
    }
    return 0;
}
```

