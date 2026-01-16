/**
 * Z 算法 (Z-Algorithm / Extended KMP)
 * @param s 输入字符串
 * @return z 数组
 * * 定义: z[i] 表示 s 和 s[i...] (从 i 开始的后缀) 的最长公共前缀长度。
 * 时间复杂度: O(n)
 */

#include <vector>
#include <iostream>
using namespace std;


vector<int> zFunction(string s) {
    int n = s.size();
    vector<int> z(n);
    z[0] = n;
    for (int i = 1, j = 1; i < n; i++)
    {
        z[i] = max(0, min(j + z[j] - i, z[i - j]));
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > j + z[j]) {
            j = i;
        }
    }
    return z;
}