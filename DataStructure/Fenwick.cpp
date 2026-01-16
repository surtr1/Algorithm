#include <vector>
#include <algorithm>
#include <cmath>

// 使用模板，T 可以是 int 或 long long
template<typename T>
struct Fenwick {
    int n;
    std::vector<T> tr;

    // 构造函数：初始化 n 和数组
    Fenwick(int n) : n(n), tr(n + 1, 0) {}

    // 内联 lowbit，或者直接在循环中写
    int lowbit(int x) { return x & -x; }

    // 单点修改：a[x] += c
    void add(int x, T c) {
        for (int i = x; i <= n; i += lowbit(i)) tr[i] += c;
    }

    // 查询前缀和：sum[x]
    T sum(int x) {
        T res = 0;
        for (int i = x; i > 0; i -= lowbit(i)) res += tr[i];
        return res;
    }

    // 查询区间和：a[l...r]
    T query(int l, int r) {
        if (l > r) return 0;
        return sum(r) - sum(l - 1);
    }

    // 查找第 k 小元素的索引（假设元素值为 1~n 的频率）
    // 时间复杂度：O(log n)
    // 逻辑：找到最大的 idx，使得 sum(idx) < k，返回 idx + 1
    int kth(T k) {
        int idx = 0;
        // 计算 log2(n) 的最高位，例如 n=10, 1<<bit = 8
        // std::__lg(n) 是 GCC 内建函数，非 GCC 环境可用 log2(n) 或手动循环
        int max_pow2 = 1 << std::__lg(n); 

        for (int i = max_pow2; i > 0; i >>= 1) {
            // 尝试跳跃 i 步
            // 1. 索引不越界
            // 2. 当前覆盖范围的和 < k
            if (idx + i <= n && tr[idx + i] < k) {
                idx += i;       // 跳跃
                k -= tr[idx];   // 减去这部分的和
            }
        }
        return idx + 1;
    }
};

/*
#include <iostream>
// 包含上面的结构体...

int main() {
    Fenwick<int> bit(10); // 支持 1-10 的范围
    
    // 插入数据：{1, 2, 3, 5, 5}
    bit.add(1, 1);
    bit.add(2, 1);
    bit.add(3, 1);
    bit.add(5, 2); // 插入两个 5

    // 查询前缀和
    std::cout << "Sum(3): " << bit.sum(3) << std::endl; // 输出 3

    // 查找第 4 小的数 (应该是指向 5)
    // 数组逻辑顺序: 1, 2, 3, 5, 5
    std::cout << "4th element: " << bit.kth(4) << std::endl; // 输出 5
    
    // 查找第 5 小的数 (也是 5)
    std::cout << "5th element: " << bit.kth(5) << std::endl; // 输出 5

    return 0;
}


*/



template <typename T>
struct Fenwick {
    int n;
    std::vector<T> a;
    
    Fenwick(int n_ = 0) {
        init(n_);
    }
    
    void init(int n_) {
        n = n_;
        a.assign(n, T{});
    }
    
    void add(int x, const T &v) {
        for (int i = x + 1; i <= n; i += i & -i) {
            a[i - 1] = a[i - 1] + v;
        }
    }
    
    T sum(int x) {
        T ans{};
        for (int i = x; i > 0; i -= i & -i) {
            ans = ans + a[i - 1];
        }
        return ans;
    }
    
    T rangeSum(int l, int r) {
        return sum(r) - sum(l);
    }
    
    /**
     * 寻找最大的 x，使得: sum(x) <= k
     * O(logn)
     */
    int select(const T &k) {
        int x = 0;
        T cur{};
        for (int i = 1 << std::__lg(n); i; i /= 2) {
            if (x + i <= n && cur + a[x + i - 1] <= k) {
                x += i;
                cur = cur + a[x - 1];
            }
        }
        return x;
    }
};