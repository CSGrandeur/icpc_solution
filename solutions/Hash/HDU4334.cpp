/*
典型的 unordered_map 题，二分也能做。这里用hash。
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
long long s[5][202];
int t, n;
const int MAXH = 1000007;
int first[MAXH], next[MAXH];
long long v[MAXH];
int tp;
void AddHash(int hashKey, long long value)
{
    if(first[hashKey] == -1)
    {
        first[hashKey] = tp ++;
        v[first[hashKey]] = value;
        return;
    }
    int i;
    for(i = first[hashKey]; v[i] != value && next[i] != -1; i = next[i]);
    if(v[i] != value)
    {
        next[i] = tp ++;
        v[next[i]] = value;
    }
}
bool FindHash(int hashKey, long long value)
{
    if(first[hashKey] == -1)
        return false;
    int i;
    for(i = first[hashKey]; v[i] != value && next[i] != -1; i = next[i]);
    return v[i] == value;
}
bool solve()
{
    for(int i = 0; i < n; i ++)
        for(int j = 0; j < n; j ++)
        {
            long long value = s[0][i] + s[1][j];
            unsigned hashKey = (unsigned)(value * 97) % MAXH;
            AddHash(hashKey, value);
        }
    for(int i = 0; i < n; i ++)
        for(int j = 0; j < n; j ++)
            for(int k = 0; k < n; k ++)
            {
                long long value = -(s[2][i] + s[3][j] + s[4][k]);
                int hashKey = (unsigned)(value * 97) % MAXH;
                if(FindHash(hashKey, value))
                    return true;
            }
    return false;

}
int main()
{
    for(scanf("%d", &t); t --; )
    {
        memset(first, -1, sizeof(first));
        memset(next, -1, sizeof(next));
        tp = 0;
        scanf("%d", &n);
        for(int i = 0; i < 5; i ++)
            for(int j = 0; j < n; j ++)
                scanf("%lld", &s[i][j]);
        printf(solve() ? "Yes\n" : "No\n");
    }
    return 0;
}