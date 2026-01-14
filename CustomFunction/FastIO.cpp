#include <cstring>

namespace FastIO {
	int fastio_digit[67], fastio_len = 0;

    template <typename T>
    inline T read() {
        T x = 0, w = 0;
        char ch = getchar();
        while (ch < '0' || ch > '9')
            w |= (ch == '-'), ch = getchar();
        while ('0' <= ch && ch <= '9')
            x = x * 10 + (ch ^ '0'), ch = getchar();
        return w ? -x : x;
    }

    template <typename T>
    inline void write(T x) {
        if (!x)
            return (void)putchar('0');
        while (x)
            fastio_digit[++fastio_len] = x % 10, x /= 10;
        while (fastio_len > 0)
            putchar(fastio_digit[fastio_len--] ^ '0');
    }

    template <typename T>
    inline void print(T x) {
        if (x >= 0)
            write<T>(x);
        else
            putchar('-'), write<T>(-x);
    }

    template <typename T>
    inline void print(T x, char en) { 
        print<T>(x); 
        putchar(en); 
    }
}; 
using namespace FastIO;
