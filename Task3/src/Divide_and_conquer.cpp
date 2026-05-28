// ============================================================
//  Tower of Hanoi (4 Pegs) – Divide & Conquer
//  Uses Frame-Stewart algorithm with k = floor((sqrt(8n+1)-1)/2)
//  Prints every move to stdout.
// ============================================================

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

int cnt = 0;

// Standard 3-peg Hanoi for k disks: s → e using m as auxiliary
void hanoi(int n, int s, int m, int e) {
    if (n == 0) return;
    hanoi(n - 1, s, e, m);
    cnt++;
    cout << "Move #" << cnt << ": " << s << ' ' << e << '\n';
    hanoi(n - 1, m, s, e);
}

// 4-peg Frame-Stewart: s → e using m1, m2 as auxiliaries
void pegs(int n, int s, int m1, int m2, int e) {
    if (n == 0) return;
    if (n == 1) {
        cnt++;
        cout << "Move #" << cnt << ": " << s << ' ' << e << '\n';
        return;
    }
    int k = (int)((sqrt(8.0 * n + 1) - 1) / 2);
    pegs(n - k, s, m1, e, m2);          // move top (n-k) disks to m2
    hanoi(k - 1, s, e, m1);             // move k-1 disks to m1 (3-peg)
    cnt++;
    cout << "Move #" << cnt << ": " << s << ' ' << e << '\n';  // largest disk
    hanoi(k - 1, m1, s, e);             // move k-1 disks from m1 to e
    pegs(n - k, m2, m1, s, e);          // move (n-k) disks from m2 to e
}

int main() {
    int n;
    cout << "n = ";
    cin >> n;
    pegs(n, 1, 2, 3, 4);
    return 0;
}
