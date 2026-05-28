// ============================================================
//  Tower of Hanoi (4 Pegs) – DP + Divide & Conquer
//  Memoizes subproblem move sequences so repeated sub-tours
//  are looked up in O(1) instead of recomputed.
// ============================================================

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

const int MAXN = 100;
vector<vector<int>> dp[MAXN];   // memoized 4-peg sequences
vector<vector<int>> dph[MAXN];  // memoized 3-peg sequences

// 3-peg Hanoi with memoization (canonical pegs 1,2,3 stored; remapped on return)
vector<vector<int>> hanoi(int n, int s, int m, int e) {
    if (n == 0) return {};
    if (!dph[n].empty()) {
        vector<vector<int>> v = dph[n];
        for (auto &p : v)
            for (auto &i : p) {
                if      (i == 1) i = s;
                else if (i == 2) i = m;
                else             i = e;
            }
        return v;
    }
    vector<vector<int>> v  = hanoi(n - 1, s, e, m);
    v.push_back({s, e});
    vector<vector<int>> v2 = hanoi(n - 1, m, s, e);
    v.insert(v.end(), v2.begin(), v2.end());

    // Store canonically (pegs 1,2,3)
    for (auto p : v) {
        for (auto &i : p) {
            if      (i == s) i = 1;
            else if (i == m) i = 2;
            else             i = 3;
        }
        dph[n].push_back(p);
    }
    return v;
}

// 4-peg Frame-Stewart with memoization
vector<vector<int>> pegs(int n, int s, int m1, int m2, int e) {
    if (n == 0) return {};
    if (n == 1) return {{s, e}};

    if (!dp[n].empty()) {
        vector<vector<int>> v = dp[n];
        for (auto &p : v)
            for (auto &i : p) {
                if      (i == 1) i = s;
                else if (i == 2) i = m1;
                else if (i == 3) i = m2;
                else             i = e;
            }
        return v;
    }

    int k = (int)((sqrt(8.0 * n + 1) - 1) / 2);

    vector<vector<int>> v  = pegs(n - k, s, m1, e, m2);
    vector<vector<int>> v2 = hanoi(k - 1, s, e, m1);
    v.insert(v.end(), v2.begin(), v2.end());
    v.push_back({s, e});
    v2 = hanoi(k - 1, m1, s, e);
    v.insert(v.end(), v2.begin(), v2.end());
    v2 = pegs(n - k, m2, m1, s, e);
    v.insert(v.end(), v2.begin(), v2.end());

    // Store canonically (pegs 1,2,3,4)
    for (auto p : v) {
        for (auto &i : p) {
            if      (i == s)  i = 1;
            else if (i == m1) i = 2;
            else if (i == m2) i = 3;
            else              i = 4;
        }
        dp[n].push_back(p);
    }
    return v;
}

int main() {
    int n;
    cout << "n = ";
    cin >> n;
    auto moves = pegs(n, 1, 2, 3, 4);
    for (int i = 0; i < (int)moves.size(); i++)
        cout << "Move #" << i + 1 << ": " << moves[i][0] << ' ' << moves[i][1] << '\n';
    return 0;
}
