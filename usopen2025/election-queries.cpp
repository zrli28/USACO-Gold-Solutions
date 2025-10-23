#pragma GCC optimize("O3")
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;

const int INF = 1000000000;

int main() {
    std::cin.tie(0)->sync_with_stdio(0);
    int n, q;
    scanf("%d %d", &n, &q);

    std::vector<int> a(n+1), votes(n+1, 0);
    for(int i = 1; i <= n; ++i) {
        scanf("%d", &a[i]);
        ++votes[a[i]];
    }

    //bash using heap
    std::vector<int> value(n+1, 0);
    std::vector<std::vector<int>> mins(n+1), maxs(n+1);

    int mx = 0;
    for(int i = 1; i <= n; ++i) {
        int c = votes[i];
        if(c > mx) mx = c;
        ++value[c];
        mins[c].push_back(i);
        maxs[c].push_back(i);
    }

    for(int i = 0; i <= n; ++i) {
        if(!mins[i].empty()) make_heap(mins[i].begin(), mins[i].end(), std::greater<>());
        if(!maxs[i].empty()) make_heap(maxs[i].begin(), maxs[i].end());
    }

    while(q--) {
        int idx, x;
        scanf("%d %d", &idx, &x);

        //do updates
        int u = a[idx];
        if(u != x) {
            int cu = votes[u];
            --value[cu];
            --votes[u];
            {
                int c = cu-1;
                ++value[c];
                mins[c].push_back(u);
                push_heap(mins[c].begin(), mins[c].end(), std::greater<>());
                maxs[c].push_back(u);
                push_heap(maxs[c].begin(), maxs[c].end());
            }

            int cx = votes[x];
            --value[cx];
            ++votes[x];
            {
                int c = cx+1;
                ++value[c];
                mins[c].push_back(x);
                push_heap(mins[c].begin(), mins[c].end(), std::greater<>());
                maxs[c].push_back(x);
                push_heap(maxs[c].begin(), maxs[c].end());
                if(c > mx) mx = c;
            }

            a[idx] = x;
            while(mx && !value[mx]) --mx;
        }

        int suffmn = INF, suffmx = 0, res = 0;
        //iterate through mx
        for(int j = mx; ~j; --j) {
            if(value[j] > 0) {
                auto &hmin = mins[j];
                //lazily adjust min heap
                while(!hmin.empty()) {
                    int top = hmin.front();
                    if(votes[top] == j) break;
                    pop_heap(hmin.begin(), hmin.end(), std::greater<>());
                    hmin.pop_back();
                }

                int mn = (hmin.empty() ? INT_MAX : hmin.front());

                //same with max heap
                auto &hmax = maxs[j];
                while(!hmax.empty()) {
                    int top = hmax.front();
                    if(votes[top] == j) break;
                    pop_heap(hmax.begin(), hmax.end());
                    hmax.pop_back();
                }

                int mxv = (hmax.empty() ? 0 : hmax.front());

                if(mn < suffmn) suffmn = mn;
                if(mxv > suffmx) suffmx = mxv;
            }

            //then iterate though second value
            int i = mx-j;
            if(i && i < mx && value[i]) {
                auto &hmaxi = maxs[i];
                while(!hmaxi.empty()) {
                    int top = hmaxi.front();
                    if(votes[top] == i) break;
                    pop_heap(hmaxi.begin(), hmaxi.end());
                    hmaxi.pop_back();
                }

                int hii = (hmaxi.empty() ? 0 : hmaxi.front());

                auto &hmini = mins[i];
                while(!hmini.empty()) {
                    int top = hmini.front();
                    if(votes[top] == i) break;
                    pop_heap(hmini.begin(), hmini.end(), std::greater<>());
                    hmini.pop_back();
                }

                int loi = (hmini.empty() ? INT_MAX : hmini.front());
                res = std::max(res, std::max(std::abs(hii-suffmn), std::abs(loi-suffmx)));
            }
        }

        printf("%d\n", res);
    }

    //overall complexity ~O(qn), lots of const factor optimizations

    return 0;
}
