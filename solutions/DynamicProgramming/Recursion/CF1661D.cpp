// difficulty: 3
// Progressions Covering
// 如果用差分处理等差数列会发现没法实时判断满足要求。
// 最后发现可用贪心策略加阶段标记，线段树都不需要。
// 从最后一个位置开始，先满足它。然后逐个往前找不满足的位置，
// 对于一个新的未满足位置，会发现只有把它当作等差数列的第k个位置才是最优策略。
// 在此贪心基础上，会发现一个简单的递推关系：
// 假设位置 `i` 的高度为 `y`、加了 `x` 次等差数列（无论每次是`1~k`的哪个位置），
// 位置 `i-1` 加了 `p` 次等差数列，则位置 `i-1` 的当前高度为 `y-p-(x-p)=y-x`，也就是`i-1`的高度与`p`无关，只与`x,y`有关
// 但需要维护 `i-1` 位置的等差数列次数，以便继续向前递推。维护方法是每次在`i`位置加等差数列时，为`i-k`位置记录一个次数“台阶”`step[i-k]`，
// 那么由`i`位置次数`x`推`i-1`位置的`p`时，就是`p=x-step[i-1]`。
// 到前`k`个位置范围时，等差数列顶到头了，就看这`k`个位置最高需要多少次。
// 总体`O(n)`
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<vector>
const int maxn = 3e5 + 10;
typedef long long LL;
int n, k;
LL step[maxn], b[maxn];
int main()
{
    LL x, y, z, sum, mx;
    while(scanf("%d%d", &n, &k) != EOF)
    {
        y = -1;
        sum = mx = 0;
        memset(step, 0, sizeof(step));
        for(int i = 0; i < n; i ++)
            scanf("%lld", &b[i]);
        for(int i = n - 1; i >= k - 1; i --)
        {
            if(y == -1)
            {
                x = (b[i] + k - 1) / k;
                y = x * k;
                sum += x;
                step[i - k] += x;
            }
            else
            {
                y = y - x;
                x -= step[i];
                if(y < b[i])
                {
                    z = (b[i] - y + k - 1) / k;
                    step[i - k] += z;
                    x += z;
                    y += z * k;
                    sum += z;
                }
            }
        }
        for(int i = k - 2, j = k - 1; i >= 0; i --, j --)
        {
            y = y - x;
            x -= step[i];
            if(y < b[i])
                mx = std::max(mx, (b[i] - y + j - 1) / j);
        }
        printf("%lld\n", sum + mx);        
    }
    return 0;
}