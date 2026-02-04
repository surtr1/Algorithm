#include <vector>
#include <iostream>

using namespace std;
using LL = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    LL n, m;
    cin >> n >> m;
    vector<LL> p(m);
    for (int i = 0; i < m; i++)
        cin >> p[i];
    LL ans = 0;
    for (int i = 1; i < (1 << m); i++) {
        LL t = 1, cnt = 0;
        for (int j = 0; j < m; j++) {
            if (i >> j & 1) {
                cnt++;
                t *= p[j];
                if (t > n)
                {
                    t = -1;
                    break;
                }
            }
        }
        if (t != -1) {
            if (cnt & 1)
                ans += n / t;
            else
                ans -= n / t;
        }
    }
    cout << ans << "\n";
}