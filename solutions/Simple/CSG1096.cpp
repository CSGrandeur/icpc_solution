// difficulty: 2
// 奇矩阵
// 判断矩阵任意两行对位差的绝对值之和为奇数。
// 这与每个元素的奇偶性有关，一行的任意元素奇偶性改变，都会改变与其它任意行差绝对值之和的奇偶性。
// 那么根据抽屉原理，超过 2 行就一定存在奇偶性一致的两行。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
const int maxn = 1e3 + 10;
int n, m;
int a[maxn][maxn];
bool Judge()
{
    if(n > 2) return false;
    if(n == 1) return true;
    bool validate = 0;
    for(int i = 0; i < m; i ++)
        validate ^= (a[0][i] ^ a[1][i]) & 1;
    return validate;
}
int main()
{
    while(scanf("%d%d", &n, &m) != EOF)
    {
        for(int i = 0; i < n; i ++)
            for(int j = 0; j < m; j ++)
                scanf("%d", &a[i][j]);
        printf(Judge() ? "Yes\n" : "No\n");
    }
    return 0;
}