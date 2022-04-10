// difficulty: 1
// GCD vs LCM
// 脑筋急转弯，构造和固定的、前两数GCD等于后两数LCM的四个数。a、c、d都为1时，b可以为任何数。
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<vector>
int n;
int main()
{
    int t;
    for(scanf("%d", &t); t --; )
    {
        scanf("%d", &n);
        printf("%d %d %d %d\n", 1, n - 3, 1, 1);
        
    }
    return 0;
}