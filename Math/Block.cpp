/*
sum k mod i  = sum k - k/i *i
= n*k - sum i* k/i
*/

#include <bits/stdc++.h>
#define int long long
using namespace std;
typedef long long ll;
const int mod = 998244353;

void solve() {
    int n, k;
    cin >> n >> k;
    int l = 1;
    int ans = 0;
    while (l <= n) {
        if ((k / l) == 0)
            break;
        int r = k / (k / l);
        r = min(r, n);
        int sum1 = (l + r) * (r - l + 1) / 2;
        ans += sum1 * (k / l);
        // cout<<l<<" "<<r<<" "<<sum1<<endl;

        l = r + 1;
    }
    ans = n * k - ans;
    cout << ans << "\n";
}

int32_t main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int _ = 1;
    // cin>>_;
    while (_--)
        solve();

    return 0;
}
