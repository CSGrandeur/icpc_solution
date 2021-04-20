# 字符串

## KMP

```cpp
int nex[maxn];
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
int KmpMatch(char ms[], char ps[])
{
    int tlen = strlen(ps);
    for(int i = 0, j = 0; ms[i];)
    {
        if(j == -1 || ms[i] == ps[j])
        {
            if(j == tlen - 1)
            {
                // 确认一次完整匹配，此处可支持不同题目要求的记录匹配位置、
                // 统计匹配次数 或返回第一次匹配位置等操作。
                // 这里返回匹配位置。其它不结束循环的工作则 j = nex[j];
                return i - tlen + 1;    // 从 i 往前数 tlen-1 个
            }
            else
                i ++, j ++;
        }
        else
            j = nex[j];
    }
    return -1;
}
```

### KMP求最小循环节

`aaaa`循环节为`a`，`ababa`最小循环节为`ab`，`abcd`最小循环节为本身

最小循环节长度为

```cpp
int sLen = strlen(st);
int minLoopLen = sLen - nex[sLen];
```
