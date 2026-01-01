using i64 = long long;
using i128 = __int128;

i64 ceilDiv(i64 n, i64 m) {
    if (n >= 0) {
        return (n + m - 1) / m;
    } else {
        return n / m;
    }
}

i64 floorDiv(i64 n, i64 m) {
    if (n >= 0) {
        return n / m;
    } else {
        return (n - m + 1) / m;
    }
}


template<class T>
void chmax(T &mx, T b) {
    if (mx < b) {
        mx = b;
    }
}

template<class T>
void chmin(T& mn, T b) {
    if (mn > b) {
        mn = b;
    }
}

i128 gcd(i128 a, i128 b) {
    return b ? gcd(b, a % b) : a;
}

i64 power(i64 a, i64 b, i64 p) {
    i64 res = 1;
	while(b){
		if(b&1)res=res*a%p;
		a = a*a%p;
		b>>=1;
	}
    return res;
}
