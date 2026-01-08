#include <vector>
#include <cassert>
#include <array>
using i64 = long long;

i64 exgcd(i64 a, i64 b, i64 &x, i64 &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    i64 g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}

// ax + b = 0 (mod m) -> ax = -b (mod m)
std::pair<i64, i64> sol(i64 a, i64 b, i64 m) {
    assert(m > 0);
    b *= -1; // 移项：变为 ax = -b (mod m)，即 ax ≡ -b (mod m)
    i64 x, y;
    i64 g = exgcd(a, m, x, y); // 求解 ax + my = g
    // 处理 g 为负数的情况（虽然通常gcd为正，但有的exgcd实现可能会返回负值）
    if (g < 0) {
        g *= -1;
        x *= -1;
        y *= -1;
    }
    // 裴蜀定理判定：如果 -b 不能被 gcd(a, m) 整除，则无解
    if (b % g != 0) {
        return {-1, -1};
    }
    // 此时得到了 ax_0 + my_0 = g 的解 x_0 (即代码中的 x)
    // 我们需要的是 ax ≡ -b (mod m) 的解
    // 等式两边同乘 (-b/g)，得到特解
    x = x * (b / g) % (m / g);
    
    // 调整解为非负最小整数解
    if (x < 0) {
        x += m / g;
    }
    // 返回 {最小非负解, 新的模数(周期)}
    // 通解公式为：X = x + k * (m / g)
    return {x, m / g};
}

std::array<i64, 3> exgcd(i64 a, i64 b) {
    if (!b) {
        return {a, 1, 0}; // 返回 {gcd, x, y}
    }
    // 结构化绑定 (C++17)
    auto [g, x, y] = exgcd(b, a % b); 
    // 直接套用公式：
    // x_new = y_old
    // y_new = x_old - (a/b) * y_old
    return {g, y, x - a / b * y};
}
