# Task 3 — Frame-Stewart Tower of Hanoi (4 Pegs)

---

## Problem Statement

Given **8 disks** and **4 pegs**, transfer all disks from peg 1 to another peg subject to:
- Only one disk may be moved at a time.
- A larger disk may never be placed on top of a smaller one.

The task asks:
1. Use **Divide and Conquer** to transfer the disks.
2. Determine whether Dynamic Programming can solve the puzzle in **33 moves**; if not, design an algorithm that does.
3. Design a **Dynamic Programming** algorithm that generalises to any number of disks with 4 pegs.

---

## Answer to the Key Question

> **Can the standard Divide & Conquer (always split off 2 disks) solve it in 33 moves?**

No. The naïve approach of always keeping `k = 2` disks on the side gives:

```
T(n) = 2·T(n−2) + 3      →      T(8) = 45 moves
```

45 ≠ 33, so a smarter split is needed.

> **Can it be solved in 33 moves?**

Yes — using the **Frame-Stewart algorithm** with the optimal `k` per `n`:

```
T(8) = 33 moves   ✓
```

---

## Algorithms

### 1. Naïve Divide & Conquer (`k = 2` fixed)

Recursion always breaks off 2 disks, uses 3-peg Tower-of-Hanoi for the pair, and recurses.

```cpp
int pegs(int n, int s, int m1, int m2, int e) {
    if (n == 0) return 0;
    if (n == 1) { cout << s << ' ' << e << '\n'; return 1; }
    int cnt = 0;
    cnt += pegs(n-2, s, m1, e, m2);
    cout << s << ' ' << m1 << '\n';
    cout << s << ' ' << e  << '\n';
    cout << m1 << ' ' << e << '\n';
    cnt += 3;
    cnt += pegs(n-2, m2, m1, s, e);
    return cnt;
}
```

**Time complexity:** Θ(2^(n/2)) — produces **45 moves** for n = 8.

---

### 2. Optimal Divide & Conquer (Frame-Stewart)

The key insight: replace the fixed `k = 2` with an **optimal `kₙ`** computed per problem size.

#### Recurrence

```
T(n) = 2·T(n − kₙ) + 2^kₙ − 1
```

where `2^kₙ − 1 = Th(kₙ)` is the standard Tower-of-Hanoi cost for `kₙ` disks.

#### Optimal kₙ formula

Derived by inverting the triangular-number pattern observed in the brute-forced table:

```
kₙ = floor( (√(8n+1) − 1) / 2 )
```

| n  | kₙ | T(n) |
|----|----|------|
| 1  | —  | 1    |
| 2  | 1  | 3    |
| 4  | 2  | 9    |
| 8  | 3  | **33** |
| 16 | 5  | 161  |
| 32 | 6  | 1281 |

#### Optimal code

```cpp
int hanoi(int n, int s, int m, int e) {
    if (n == 0) return 0;
    int cnt = 0;
    cnt += hanoi(n-1, s, e, m);
    cout << s << ' ' << e << '\n'; cnt++;
    cnt += hanoi(n-1, m, s, e);
    return cnt;
}

int pegs(int n, int s, int m1, int m2, int e) {
    if (n == 0) return 0;
    if (n == 1) { cout << s << ' ' << e << '\n'; return 1; }
    int k = (sqrt(8*n + 1) - 1) / 2;   // optimal split
    int cnt = 0;
    cnt += pegs(n-k, s, m1, e, m2);
    cnt += hanoi(k-1, s, e, m1);
    cout << s << ' ' << e << '\n'; cnt++;
    cnt += hanoi(k-1, m1, s, e);
    cnt += pegs(n-k, m2, m1, s, e);
    return cnt;
}
```

**Time complexity:** O(4^√n)

---

### 3. Dynamic Programming

#### 3a. Memoised Divide & Conquer

Stores the sequence of moves for each sub-problem size using a canonical peg labelling (1–4). On a cache hit the stored sequence is remapped to the actual peg names.

```cpp
const int N = 100;
vector<vector<int>> dp[N], dph[N];
// See full implementation in report for hanoi() and pegs() with memoization.
```

**Time complexity:** O(4^√n) — same asymptotic bound, lower constant for repeated calls.

#### 3b. Counting Moves Only (iterative DP)

For any `n`, compute the minimum number of moves in O(n) time and O(n) space:

```cpp
const int N = 901;
long long dp[N];

int main() {
    dp[1] = 1;
    for (int i = 2; i < N; i++) {
        int k = (sqrtl(8*i + 1) - 1) / 2;
        dp[i] = 2 * dp[i-k] + (1LL << k) - 1;
    }
}
```

**Time complexity:** Θ(n) | **Space complexity:** Θ(n)

Sample output:
```
1:   1
2:   3
4:   9
8:   33
16:  161
32:  1281
64:  18433
128: 720897
```

---

## Closed-Form for T(n)

```
T(n) = (kₙ + a − 1) · 2^kₙ + 1

where:
  kₙ = floor( (√(8n+1) − 1) / 2 )
  a  = n − Σᵢ₌₁^kₙ i   (remaining disks after the triangular prefix)
```

Asymptotically: **T(n) ≈ 4^√n**

---

## Complexity Summary

| Algorithm                        | Moves for n=8 | Time Complexity |
|----------------------------------|---------------|-----------------|
| Naïve D&C (k=2 fixed)           | 45            | Θ(2^(n/2))      |
| Optimal D&C (Frame-Stewart)     | **33**        | O(4^√n)         |
| DP — memoised sequence          | **33**        | O(4^√n)         |
| DP — count only (iterative)     | **33**        | Θ(n)            |

---

