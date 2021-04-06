# 字符串

## KMP
```cpp
int next[1000];
void BuildNext(char ts[])
{
    next[0] = -1;
    for(int i = 0, j = -1; ts[i]; )
    {
        if(j == -1 || ts[i] == ts[j])
            next[++i] = ++j;
        else
            j = next[j];
    }
}
int KmpMatch(char ms[], char ts[])
{
    int tlen = strlen(ts);
    for(int i = 0, j = 0; ms[i];)
    {
        if(j == -1 || ms[i] == ts[j])
        {
            if(j == tlen - 1)
            {
                // 确认一次完整匹配，此处可支持不同题目要求的记录匹配位置、
                // 统计匹配次数 或返回第一次匹配位置等操作。
                // 这里返回匹配位置。其它不结束循环的工作则 j = next[j];
                return i - tlen + 1;    // 从 i 往前数 tlen-1 个
            }
            else
                i ++, j ++;
        }
        else
            j = next[j];
    }
    return -1;
}
```