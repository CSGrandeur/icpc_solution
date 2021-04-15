// difficulty: 2
// Fractions Again?!
// 枚举的方式很有意思，`x==y`时它们为`2k`，那么`x>y`就知道`y`的枚举范围在`(k,2k]`了

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
using namespace std;
int k;
int main()
{
    while(scanf("%d", &k) != EOF)
    {
        vector<int> resx, resy;
        for(int y = k + 1; y <= (k << 1); y ++)
        {
            int d = y * k;
            int n = y - k;
            if(n > 0 && d % n == 0)
                resx.push_back(d / n), resy.push_back(y);
        }
        printf("%d\n", (int)resx.size());
        for(int i = 0; i < resx.size(); i ++)
            printf("1/%d = 1/%d + 1/%d\n", k, resx[i], resy[i]);
    }
    return 0;
}