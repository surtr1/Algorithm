#include<bits/stdc++.h>
// #define int long long
#define all1(x) (x).begin()+1,(x).end()
using namespace std;
typedef long long ll;

const int N = 5e5+10;
int n;
int p[N][30];
int pos[N][30];
void insert(int id,int x){
    for(int i=0;i<=29;i++){
        pos[id][i]=pos[id-1][i];
        p[id][i]=p[id-1][i];
    }
    int now = id;
    for(int i=29;i>=0;i--){
        if(!(x>>i&1))continue;
        if(!p[id][i]){
            pos[id][i]=now;
            p[id][i]=x;break;
        }
        if(pos[id][i]<now){
            swap(pos[id][i],now);
            swap(p[id][i],x);
        }
        x^=p[id][i];
    }
}
int query(int l,int r,int x){
    for(int i=29;i>=0;i--){
        if(pos[r][i]>=l&&(x^p[r][i])>x){
            x^=p[r][i];
        }
    }
    return x;
}

void solve(){
    cin>>n;
    for(int i=1;i<=n;i++){
        ll c;cin>>c;
        insert(i,c);
    }
    int q;cin>>q;
    while(q--){
        int l,r;cin>>l>>r;
        cout<<query(l,r,0)<<"\n";
    }
}
/*

*/
int32_t main(){
    ios::sync_with_stdio(0),cin.tie(0);
    int _=1;
    // cin>>_;
    for(int i=1;i<=_;i++){
        solve();
    }
    return 0;
}