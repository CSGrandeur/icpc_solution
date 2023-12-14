### `KMP `

> difficulty: 2

练习`nex`的认知及匹配过程

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string>
#include<unordered_map>
using namespace std;
const int maxs = 1e6 + 10;
int n;
char ts[maxs], ms[maxs];
int nex[maxs];
int slen, tlen;

void BuildNex(char ts[], int nex[])
{
    nex[0] = -1;
    for(int i = 0, j = -1; ts[i]; ) {
        if(j == -1 || ts[i] == ts[j])
            nex[++i] = ++j;
        else
            j = nex[j];
    }
}
int KmpMatch(char ms[], char ts[])
{
    for(int i = 0, j = 0; i < slen;) {
        if(j == -1 || ms[i] == ts[j]) {
            if(j == tlen - 1) {
                printf("%d\n", i - tlen + 1 + 1);
                j = nex[j];
            }
            else
                i ++, j ++;
        }
        else
            j = nex[j];
    }
    return -2;
}
int main()
{
    while(scanf("%s%s", ms, ts) != EOF) {
        slen = strlen(ms);
        tlen = strlen(ts);
        BuildNex(ts,nex);
        KmpMatch(ms, ts);
        for(int i = 1; i <= tlen; i ++) {
            printf(" %d" + (i == 1), nex[i]);
        }
    }
    return 0;
}
```