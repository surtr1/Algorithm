/**
 * 离散化 0 - based
 * 预处理 O(n * log n)
 * 查询 O(log n)
 */

#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

template <typename T>
struct Compress {
    vector<T> alls;
    
    Compress() {}
    Compress(vector<T> in) : alls(in) {
        init();
    }

    // 添加元素
    void add(T x) {
        alls.emplace_back(x);
    }

    // 添加多个元素
    template <typename... Args>
    void add(T x, Args... args) {
        add(x), add(args...);
    }

    // 初始化：排序并去重
    void init() {
        sort(alls.begin(), alls.end());
        alls.erase(unique(alls.begin(), alls.end()), alls.end());
    }

    // 获取离散化后的下标 (0-based)
    // 输入原始值 val，返回排名 (0, 1, 2...)
    int operator[](T x) {
        // 使用 lower_bound 查找第一个 >= x 的位置
        return lower_bound(alls.begin(), alls.end(), x) - alls.begin();
    }

    // 反向查询：根据下标获取原始值
    // 输入下标 idx，返回原始值
    T Get(int idx) {
        assert(idx >= 0 && idx < alls.size());
        return alls[idx];
    }

    // 获取去重后的元素个数 (线段树需要开的大小)
    int size() {
        return alls.size();
    }

    // 调试输出
    friend auto &operator<<(ostream &o, const Compress &j) {
        o << "{";
        for (size_t i = 0; i < j.alls.size(); ++i) {
            o << i << ":" << j.alls[i] << (i == j.alls.size() - 1 ? "" : ", ");
        }
        return o << "}";
    }
};











int main() {
    // 假设这是题目给出的原始数据，数值很大，甚至可能有负数
    vector<int> raw_data = {10000, -5, 20, 10000, -5, 9999999};

    cout << "--- 1. 准备数据 ---" << endl;
    
    // 实例化离散化工具
    Compress<int> c;
    // 将所有出现过的数值加入
    for (int x : raw_data) {
        c.add(x);
    }
    // 也可以手动添加额外的边界点
    c.add(0); 
    // 执行初始化 (排序+去重)
    c.init();

    cout << "离散化完成，有效点数: " << c.size() << endl; // 应该有 5 个点: -5, 0, 20, 10000, 9999999
    
    cout << "\n--- 2. 查看映射关系 (Raw -> Index) ---" << endl;
    // 我们可以开一个紧凑的数组了，大小只需要 c.size()
    vector<int> count(c.size(), 0);

    for (int x : raw_data) {
        // 获取 x 对应的离散化下标
        int idx = c[x]; 
        
        cout << "原始值: " << x << " \t-> 映射下标: " << idx << endl;
        
        // 在紧凑数组上进行操作 (例如统计出现次数)
        count[idx]++;
    }

    cout << "\n--- 3. 验证统计结果 (Index -> Raw) ---" << endl;
    for (int i = 0; i < c.size(); i++) {
        if (count[i] > 0) {
            // 通过 c.Get(i) 找回原始值
            cout << "下标 " << i << " (代表原始值 " << c.Get(i) << ") 出现了 " << count[i] << " 次" << endl;
        }
    }

    cout << "\n--- 4. 查找不存在的值 ---" << endl;
    // 如果查找一个没加入过的值，lower_bound 会返回它“应该在”的位置
    // 比如查找 10 (它在 0 和 20 之间)
    int idx_10 = c[10];
    cout << "查询 10 的映射下标: " << idx_10 << endl; 
    cout << "该下标对应的实际值是: " << c.Get(idx_10) << endl; 
    // 结果应该是指向 20 的下标，因为 20 是第一个 >= 10 的数

    return 0;
}