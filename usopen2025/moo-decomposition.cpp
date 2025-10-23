#pragma GCC optimize("O3")
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;

const int INF = 1000000000;
const long long MOD = 1000000007;

long long modpow(long long x, long long b, long long m = MOD) {
    x %= m;
    long long ans = 1;
    while(b) {
        if(b&1) ans = (ans*x)%m;
        x = (x*x)%m;
        b >>= 1;
    }

    return ans;
}

int main() {
    std::cin.tie(0)->sync_with_stdio(0);
    int k, n; long long l;
    std::cin >> k >> n >> l;

    std::string s;
    std::cin >> s;

    //precompute factorials for choose
    std::vector<long long> facts(n+1, 1);
    for(int i = 2; i <= n; ++i) facts[i] = (facts[i-1]*i)%MOD;

    std::vector<long long> invfact(n+1, 1);
    for(int i = 0; i <= n; ++i) invfact[i] = modpow(facts[i], MOD-2);

    auto choose = [&](long long t, long long e) {
        return (facts[t]*(invfact[e]*invfact[t-e]%MOD))%MOD;
    };

    //start from back, process forwards to find value of one copy
    long long res1 = 1, cntos = 0;
    for(int i = n-1; ~i; --i) {
        if(s[i] == 'O') ++cntos;
        else {
            res1 = (res1*choose(cntos, k))%MOD;
            cntos -= k;
        }
    }

    //apply modpow
    std::cout << modpow(res1, l)%MOD << "\n";

    return 0;
}
