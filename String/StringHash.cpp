/**
 * @brief 双模数字符串哈希类 (Double Rolling Hash)
 * * @details
 * 该类封装了双模数多项式滚动哈希算法，用于快速判断字符串相等性。
 * 1.
 * 使用两组不同的模数 (1e9+7, 1e9+9) 和基数 (131, 13331) 同时计算，
 * 随机质数列表：1111111121、1211111123、1311111119。
 * 只有两组哈希值都相等才判定字符串相等，冲突概率极低。
 * 2. 时间复杂度
 * - 预处理: O(N)
 * - 子串哈希查询 (substring): O(1)
 * - 虚拟单点修改 (modify): O(1)
 * 3.内存优化:
 * 使用静态成员 (static) 存储 base 的幂次表，所有 StringHash 实例共享一份数据，
 * 避免重复计算，且支持自动动态扩容。
 * * @note 
 * 字符串下标从 0 开始。
 */



#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct StringHash {
    // 定义两个模数和进制，减少冲突概率
    static constexpr long long MOD1 = 1111111121;
    static constexpr long long MOD2 = 1211111123;
    static constexpr long long BASE1 = 131;
    static constexpr long long BASE2 = 13331;

    // 静态数组存储幂次，所有 StringHash 实例共享，节省空间和时间
    // C++17 可用 inline static，为了兼容性这里在类外初始化
    static vector<long long> pow1;
    static vector<long long> pow2;

    vector<long long> h1, h2; // 前缀哈希数组
    string s;

    // 构造函数
    StringHash(string s_) : s(s_) {
        int n = s.size();
        h1.resize(n + 1, 0);
        h2.resize(n + 1, 0);

        // 动态扩展幂次数组，确保长度足够
        while (pow1.size() <= n) {
            pow1.push_back((pow1.back() * BASE1) % MOD1);
            pow2.push_back((pow2.back() * BASE2) % MOD2);
        }
        // 计算前缀哈希
        for (int i = 0; i < n; i++) {
            h1[i + 1] = (h1[i] * BASE1 + s[i]) % MOD1;
            h2[i + 1] = (h2[i] * BASE2 + s[i]) % MOD2;
        }
    }

    // 获取整串哈希
    pair<long long, long long> get() {
        return {h1.back(), h2.back()};
    }

    // 获取子串 s[l...r] 的哈希值 (0-based index)
    // 闭区间：l 和 r 都包含
    pair<long long, long long> substring(int l, int r) {
        if (l > r) swap(l, r); // 容错
        // 公式：Hash[r] - Hash[l-1] * P[len]
        long long ans1 = (h1[r + 1] - h1[l] * pow1[r - l + 1]) % MOD1;
        if (ans1 < 0) ans1 += MOD1;

        long long ans2 = (h2[r + 1] - h2[l] * pow2[r - l + 1]) % MOD2;
        if (ans2 < 0) ans2 += MOD2;

        return {ans1, ans2};
    }

    // 模拟修改：计算如果将 idx 位置的字符修改为 x，整串的哈希值会变成多少
    // 复杂度 O(1)，不会通过修改原串来重新计算
    pair<long long, long long> modify(int idx, char x) {
        int n = s.size();
        // 权值取决于该字符距离末尾的位置：n - 1 - idx
        int p_idx = n - 1 - idx; 
        // 新哈希 = 旧哈希 + (新字符 - 旧字符) * base^权值
        long long diff = x - s[idx];
        long long ans1 = (h1.back() + diff * pow1[p_idx]) % MOD1;
        if (ans1 < 0) ans1 += MOD1;
    
        long long ans2 = (h2.back() + diff * pow2[p_idx]) % MOD2;
        if (ans2 < 0) ans2 += MOD2;

        return {ans1, ans2};
    }
};

// 初始化静态成员 (在全局区域)
vector<long long> StringHash::pow1{1};
vector<long long> StringHash::pow2{1};






// ==========================================
// 用法示例
// ==========================================
int main() {
    string str = "hello_world";
    StringHash sh(str);

    // 1. 获取子串 "hello" (0~4)
    auto h_hello = sh.substring(0, 4);
    cout << "Hash of 'hello': " << h_hello.first << ", " << h_hello.second << endl;

    // 2. 比较两个子串是否相等
    StringHash sh2("hello_hello");
    // 比较 str 的 "hello" 和 sh2 的后一个 "hello"
    if (sh.substring(0, 4) == sh2.substring(6, 10)) {
        cout << "Substrings match!" << endl;
    }

    // 3. 模拟修改
    // 将 'w' (index 6) 修改为 'm' -> "hello_morld"
    auto new_hash = sh.modify(6, 'm');
    
    // 验证修改后的哈希是否等于真实的新串哈希
    StringHash sh_modified("hello_morld");
    if (new_hash == sh_modified.get()) {
        cout << "Modify simulation is correct!" << endl;
    }

    return 0;
}
