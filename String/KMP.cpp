/**
 * 应用：
    1. 在字符串中查找子串；
    2. 最小周期：字符串长度-整个字符串的 border；
    3. 最小循环节：区别于周期，当字符串长度 n mod (n - nxt[n]) = 0 时，等于最小周期，
    否则为 n。
 */

#include <vector>
#include <iostream>
using namespace std;


auto KMP = [&](string s, string t) -> void {
    int n = s.size(), m = t.size();
    vector<int> nxt(m + 1);
    s = '-' + s;
    t = '-' + t;
    for (int i = 2, j = 0; i <= m; i++) {
        while (j && t[i] != t[j + 1])
            j = nxt[j];
        if (t[i] == t[j + 1])
            j++;
        nxt[i] = j;
    }
    for (int i = 1, j = 0; i <= n; i++) {
        while (j && s[i] != t[j + 1])
            j = nxt[j];
        if (s[i] == t[j + 1])
            j++;
        if (j == m) {
            cout << i - m + 1 << "\n"; // t 在 s 中出现的位置
            j = nxt[j];
        }
    }
};




/**
 * KMP 前缀函数 (Prefix Function / Next Array)
 * * @param s 模式串 (Pattern String)
 * @return std::vector<int> f 数组 (1-based indexing for length)
 * * 定义: f[len] 表示 s 的长度为 len 的前缀 s[0...len-1] 中，
 * 最长的、同时是真前缀和真后缀的子串长度。
 * (即: Longest Proper Prefix which is also a Suffix)
 */
std::vector<int> kmp(std::string s) {
    int n = s.size();
    // f[i] 存储长度为 i 的前缀的 border 长度
    // f[0] = 0, f[1] = 0 (单个字符没有真前后缀)
    std::vector<int> f(n + 1); 
    
    // i 从 1 开始，因为 s[0] 的 border 长度默认为 0
    // j 代表当前匹配到的最长前缀长度 (也是上一个状态的 border 值)
    for (int i = 1, j = 0; i < n; i++) {
        // 如果当前字符 s[i] 不匹配 s[j] (即下一个待匹配字符)，则回退
        // j = f[j] 利用了之前计算好的前缀信息，避免从头开始匹配
        while (j && s[i] != s[j]) {
            j = f[j];
        }
        // 如果字符匹配，当前的最长公共前后缀长度 +1
        j += (s[i] == s[j]);
        // 记录结果：长度为 i+1 的前缀，其 border 长度为 j
        f[i + 1] = j;
    }
    return f;
}

void solve_match() {
    std::string text = "ababaababac";
    std::string pattern = "aba";
    
    // 1. 获取模式串的 next 数组
    std::vector<int> next = kmp(pattern);
    
    int n = text.size();
    int m = pattern.size();
    
    // 2. 遍历文本串进行匹配
    // j 代表当前 pattern 已经匹配了的长度
    for (int i = 0, j = 0; i < n; i++) {
        // 如果文本串当前字符 text[i] 与模式串下一个字符 pattern[j] 不匹配
        while (j && text[i] != pattern[j]) {
            j = next[j]; // 利用 next 数组回跳
        }
        
        // 如果匹配，匹配长度 +1
        if (text[i] == pattern[j]) {
            j++;
        }
        // 如果 j 达到了模式串长度，说明完全匹配
        if (j == m) {
            std::cout << "Found pattern at index: " << (i - m + 1) << std::endl;
            // 继续寻找下一个匹配，回退到 next[j]
            j = next[j]; 
        }
    }
}


void solve_period() {
    std::string s = "abcabcabc"; // 长度 9，循环节 "abc"
    std::vector<int> f = kmp(s);
    int n = s.size();
    
    // 这里的 f[n] 代表整个字符串的最长公共前后缀长度
    // 在本例中，"abcabcabc" 的 border 是 "abcabc"，长度为 6
    int len_border = f[n]; 
    int len_period = n - len_border; // 9 - 6 = 3
    
    if (len_border > 0 && n % len_period == 0) {
        std::cout << "字符串是循环的！" << std::endl;
        std::cout << "最小循环节长度: " << len_period << std::endl; // 输出 3
        std::cout << "循环次数: " << n / len_period << std::endl;     // 输出 3
    } else {
        std::cout << "字符串不是循环的。" << std::endl;
    }
}
