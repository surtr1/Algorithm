/** * 懒标记线段树（LazySegmentTree）通用模板
 * * 包含功能：
 * 1. 基础：区间修改、区间查询
 * 2. 进阶：线段树上二分 (findFirst/findLast)
 * 3. 特殊：Segment Tree Beats (half) 和 楼房重建逻辑 (maintainL/R)
 * * 注意：Info 和 Tag 结构体需要根据具体题目自定义。
 * 基于 0 - idx 左闭右开区间的懒标记线段树
 */

#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

// 定义无穷大，用于特殊逻辑中的默认值
const int inf = 1e9; 

template<class Info, class Tag>
struct LazySegmentTree {
    int n; // 数组大小
    std::vector<Info> info; // 存储线段树节点信息
    std::vector<Tag> tag;   // 存储懒标记

    LazySegmentTree() : n(0) {}
    
    // 构造函数：指定大小和默认值
    LazySegmentTree(int n_, Info v_ = Info()) {
        init(n_, v_);
    }
    
    // 构造函数：通过数组初始化
    template<class T>
    LazySegmentTree(std::vector<T> init_) {
        init(init_);
    }

    // 初始化函数：指定大小
    void init(int n_, Info v_ = Info()) {
        init(std::vector(n_, v_));
    }

    // 初始化函数：通过数组构建
    template<class T>
    void init(std::vector<T> init_) {
        n = init_.size();
        // 分配 4 倍空间 (4 << log2(n))，这是线段树的标准空间
        info.assign(4 << std::__lg(n), Info());
        tag.assign(4 << std::__lg(n), Tag());
        
        std::function<void(int, int, int)> build = [&](int p, int l, int r) {
            // 叶子节点
            if (r - l == 1) {
                info[p] = init_[l]; // 将数组值转为 Info
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);      // 递归左子树
            build(2 * p + 1, m, r);  // 递归右子树
            pull(p);                 // 上传信息 (Push Up)
        };
        build(1, 0, n); // 根节点从 1 开始，区间 [0, n)
    }

    // Pull (Push Up): 利用子节点信息更新父节点
    void pull(int p) {
        info[p] = info[2 * p] + info[2 * p + 1]; // 依赖重载的 operator+
    }

    // Apply: 将标记 v 应用到节点 p
    void apply(int p, const Tag &v) {
        info[p].apply(v); // 更新节点数据
        tag[p].apply(v);  // 更新节点上的懒标记
    }

    // Push (Push Down): 下传懒标记
    void push(int p) {
        apply(2 * p, tag[p]);     // 下传给左子节点
        apply(2 * p + 1, tag[p]); // 下传给右子节点
        tag[p] = Tag();           // 清空当前节点标记
    }

    // 单点修改 (递归实现)
    // p: 当前节点索引, [l, r): 当前区间, x: 目标位置, v: 新值
    void modify(int p, int l, int r, int x, const Info &v) {
        if (r - l == 1) {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        push(p); // 修改前先下传标记
        if (x < m) {
            modify(2 * p, l, m, x, v);
        } else {
            modify(2 * p + 1, m, r, x, v);
        }
        pull(p); // 修改后更新父节点
    }

    // 单点修改 (对外接口)
    void modify(int p, const Info &v) {
        modify(1, 0, n, p, v);
    }

    // 区间查询
    // [x, y): 查询的目标区间
    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) {
            return Info(); // 无交集返回空 Info
        }
        if (l >= x && r <= y) {
            return info[p]; // 完全包含，直接返回当前节点信息
        }
        int m = (l + r) / 2;
        push(p); // 查询子节点前必须下传标记
        return rangeQuery(2 * p, l, m, x, y) + rangeQuery(2 * p + 1, m, r, x, y);
    }

    Info rangeQuery(int l, int r) {
        return rangeQuery(1, 0, n, l, r);
    }

    // 区间修改 (Range Apply)
    // [x, y): 修改的目标区间, v: 标记
    void rangeApply(int p, int l, int r, int x, int y, const Tag &v) {
        if (l >= y || r <= x) {
            return;
        }
        if (l >= x && r <= y) {
            apply(p, v); // 完全包含，打标记并更新，不再递归
            return;
        }
        int m = (l + r) / 2;
        push(p); // 递归前下传旧标记
        rangeApply(2 * p, l, m, x, y, v);
        rangeApply(2 * p + 1, m, r, x, y, v);
        pull(p); // 递归后更新当前节点
    }

    void rangeApply(int l, int r, const Tag &v) {
        return rangeApply(1, 0, n, l, r, v);
    }

    // ==========================================
    // 功能 I: Segment Tree Beats (吉司机线段树)
    // 警告：需要 Info 结构体包含 min, max, act 等特定成员变量
    // ==========================================

    // 区间操作（通常用于区间除法、取模等无法直接打标记的操作）
    // 这里的逻辑示例：区间除以 2 (向下取整)
    void half(int p, int l, int r) {
        if (info[p].act == 0) { // act 通常表示区间是否有非零值等剪枝条件
            return;
        }
        // 核心 Beats 逻辑：如果区间最小值和最大值的除法结果一致，
        // 说明整个区间可以转化为 "区间减法" 或 "区间赋值" 操作
        if ((info[p].min + 1) / 2 == (info[p].max + 1) / 2) {
            apply(p, {-(info[p].min + 1) / 2}); // 转化为减法 Tag
            return;
        }
        int m = (l + r) / 2;
        push(p);
        half(2 * p, l, m);
        half(2 * p + 1, m, r);
        pull(p);
    }

    void half() {
        half(1, 0, n);
    }
    
    // ==========================================
    // 功能 II: 线段树上二分 (Segment Tree Binary Search)
    // 时间复杂度 O(log N)
    // ==========================================

    // 查找区间 [x, y) 内第一个满足条件 pred 的位置
    template<class F>
    int findFirst(int p, int l, int r, int x, int y, F &&pred) {
        if (l >= y || r <= x) {
            return -1;
        }
        // 剪枝：如果当前节点的所有内容都不满足条件，直接返回
        if (l >= x && r <= y && !pred(info[p])) {
            return -1;
        }
        if (r - l == 1) {
            return l; // 找到叶子节点
        }
        int m = (l + r) / 2;
        push(p);
        int res = findFirst(2 * p, l, m, x, y, pred); // 先查左边
        if (res == -1) {
            res = findFirst(2 * p + 1, m, r, x, y, pred); // 左边没找到查右边
        }
        return res;
    }

    template<class F>
    int findFirst(int l, int r, F &&pred) {
        return findFirst(1, 0, n, l, r, pred);
    }

    // 查找区间 [x, y) 内最后一个满足条件 pred 的位置
    template<class F>
    int findLast(int p, int l, int r, int x, int y, F &&pred) {
        if (l >= y || r <= x) {
            return -1;
        }
        if (l >= x && r <= y && !pred(info[p])) {
            return -1;
        }
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) / 2;
        push(p);
        int res = findLast(2 * p + 1, m, r, x, y, pred); // 先查右边
        if (res == -1) {
            res = findLast(2 * p, l, m, x, y, pred); // 右边没找到查左边
        }
        return res;
    }

    template<class F>
    int findLast(int l, int r, F &&pred) {
        return findLast(1, 0, n, l, r, pred);
    }
    
    // ==========================================
    // 功能 III: 楼房重建 / 历史最大值类逻辑
    // 需要 Info 包含 maxlowl, maxlowr, difl 等特定成员
    // ==========================================

    // 维护左侧最大值相关信息
    void maintainL(int p, int l, int r, int pre) {
        if (info[p].difl > 0 && info[p].maxlowl < pre) {
            return;
        }
        if (r - l == 1) {
            // 叶子节点特殊处理逻辑
            info[p].max = info[p].maxlowl; 
            info[p].maxl = info[p].maxr = l;
            info[p].maxlowl = info[p].maxlowr = -inf;
            return;
        }
        int m = (l + r) / 2;
        push(p);
        maintainL(2 * p, l, m, pre);
        pre = std::max(pre, info[2 * p].max); // 更新前缀最大值
        maintainL(2 * p + 1, m, r, pre);
        pull(p);
    }
    void maintainL() {
        maintainL(1, 0, n, -1);
    }

    // 维护右侧最大值相关信息
    void maintainR(int p, int l, int r, int suf) {
        if (info[p].difr > 0 && info[p].maxlowr < suf) {
            return;
        }
        if (r - l == 1) {
            info[p].max = info[p].maxlowl;
            info[p].maxl = info[p].maxr = l;
            info[p].maxlowl = info[p].maxlowr = -inf;
            return;
        }
        int m = (l + r) / 2;
        push(p);
        maintainR(2 * p + 1, m, r, suf);
        suf = std::max(suf, info[2 * p + 1].max); // 更新后缀最大值
        maintainR(2 * p, l, m, suf);
        pull(p);
    }
    void maintainR() {
        maintainR(1, 0, n, -1);
    }
};

// ==========================================
// 以下是针对 "区间最大值" 的简单实现
// ==========================================

struct Tag {
    int x = 0;
    // apply: 标记合并逻辑 (Tag + Tag)
    void apply(const Tag &t) & {
        x = std::max(x, t.x);
    }
};

struct Info {
    int x = 0;
    // apply: 标记作用于节点逻辑 (Info + Tag)
    void apply(const Tag &t) & {
        x = std::max(x, t.x);
    }
};

// 重载 + 运算符: 节点合并逻辑 (Info + Info)
Info operator+(const Info &a, const Info &b) {
    return {std::max(a.x, b.x)};
}


