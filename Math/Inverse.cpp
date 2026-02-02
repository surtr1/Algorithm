//对于任意整数 a，模数 mod，如果 a 和 mod 互质（即 gcd(a,mod)=1），
//那么在模 mod 意义下，a一定存在模逆元
//即使 a > mod ，也可以通过求 a % mod 的余数，将问题简化为求解这个余数的逆元。
//故而 在下面所有的算法中，最好先把除数取个模再运算 

//费马小定理: p 是一个质数,且gcd(a,p)==1 时 
// a^(p-1) = 1(mod p)  ==>  a*a^(p-2) = 1(mod p)
//欧拉定理 a^(eular(p)) = 1(mod p)  eular(p) = p-1 与1到p与p互质的数 
// O(log(mod))
using ll = long long;
const int mod = 1e9 + 7;
ll power(ll a, ll b, int mod) {
	ll ans = 1;
	while(b) {
		if(b&1) ans = (ans * a) %mod;
		a = (a * a) %mod;
		b >>= 1;
	}
	return ans;
}

ll inverse(ll a) {
	return power(a, mod - 2,mod);
}


//扩展欧几里得 ax+by =gcd(a,b)一定存在 
//当我们要求 a关于 p 的逆元时，若逆元存在，则只需gcd(a,p)==1
// a*x = 1(mod p) ==> a*x + p*k = 1 = gcd (a,p)
//O(log(min(a,mod))) 
ll exgcd(ll a,ll b,ll &x,ll &y)//扩展欧几里得算法 
{
	if(b==0)
	{
		x=1,y=0;
		return a;
	}
	ll ret=exgcd(b,a%b,y,x);
	y-=a/b*x;
	return ret;
}
ll getInvEx(int a,int mod) { //求a在mod下的逆元，不存在逆元返回-1
	ll x,y;
	ll d=exgcd(a,mod,x,y);
	return d==1?(x%mod+mod)%mod:-1;
}


//线性求逆元
//O(n) 
// inv[1] = 1 (i==1)
// inc[i] = (p-p/i)*inv(p%i)%p
ll inv[mod+5];
void getInv(int n,int mod) {
	inv[1]=1;
	for(int i=2;i<=n;i++)
		inv[i]=(mod-mod/i)*inv[mod%i]%mod;
}

