// ============================================================
//  Tower of Hanoi (4 Pegs) – DP Move Counter
//  Computes the MINIMUM number of moves for any n disks
//  using the Frame-Stewart recurrence (no moves printed).
//
//  Recurrence:
//    dp[1] = 1
//    dp[n] = 2*dp[n-k] + 2^k - 1
//    where k = floor((sqrt(8n+1) - 1) / 2)
// ============================================================

#include <iostream>
#include <cmath>
using namespace std;

const int MAXN = 901;
long long dp[MAXN];

int main() {
    dp[1] = 1;
    for (int i = 2; i < MAXN; i++) {
        int k = (int)((sqrtl(8.0L * i + 1) - 1) / 2);
        dp[i] = 2 * dp[i - k] + (1LL << k) - 1;
    }

    // Interactive loop; enter -1 to quit
    int x = 0;
    while (x != -1) {
        if (x) cout << "--------------------\n";
        cout << "n = ";
        cin >> x;
        if (x <= 0) continue;
        cout << "Minimum moves: " << dp[x] << '\n';
    }
    return 0;
}
