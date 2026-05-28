# Knight's Tour – Advanced Algorithms & Complexity (Task 2)

**Name:** Youssef Shehta Abdelfattah  
**ID:** 24P0191 | **Group:** 3 | **Section:** 5

---

## Problem Statement

Can a chess knight visit every cell of an N×N chessboard **exactly once**, ending on a square that is one knight's move away from the starting cell? This is known as a **Closed Knight's Tour**.

---

## Feasibility by Board Size

| Case | Board Size | Closed Tour Possible? |
|------|-----------|----------------------|
| Too small | N = 1, 2, 3 |  No – insufficient space for legal moves |
| Forbidden even | N = 4 |  No – geometry prevents a closed loop |
| Odd boards | N = 5, 7, 9, 11, … |  No – N² is odd; color parity makes a closed loop impossible |
| Even boards | N = 6, 8, 10, 12, … |  Yes – always possible (Schwenk's Theorem) |

> **Open (non-closed) tour** is possible for all N ≥ 5, both even and odd, since the color-parity constraint does not apply.

---

## Algorithms

### 1. Greedy – Warnsdorff's Heuristic (`knight_tour_greedy`)

At each step, move to the unvisited square that has the **fewest onward moves** (minimum degree). If stuck, restart from the origin with a new random tie-breaking order. Repeat up to `max_retries = 5000` times.

**Key idea:** prefer "harder-to-reach" squares early so corners and edges are not stranded later.

| Metric | Value |
|--------|-------|
| Time Complexity | O(M × N²) |
| Space Complexity | O(N²) |
| M | number of restart attempts |

### 2. Brute Force – Backtracking (`knight_tour_bruteforce`)

Recursively try all 8 knight moves from each cell. If a dead end is reached, **backtrack** and try a different path. Guaranteed to find a solution if one exists, but exponential in practice.

| Metric | Value |
|--------|-------|
| Time Complexity | O(8^(N²)) |
| Space Complexity | O(N²) |

### Comparison

| | Greedy | Brute Force |
|---|---|---|
| Time | O(M · N²)  | O(8^(N²))  |
| Space | O(N²) | O(N²) |
| Guaranteed solution | No (probabilistic) | Yes |
| Practical for N ≥ 8 |  Fast |  Infeasible |

---

## Repository Structure

```
.
├── knight_tour_greedy.cpp       # Greedy solution (C++)
├── knight_tour_bruteforce.cpp   # Brute-force backtracking (C++)
├── knight_tour_greedy.py        # Greedy solution (Python)
├── knight_tour_bruteforce.py    # Brute-force solution (Python)
└── README.md
```

---

## How to Build & Run (C++)

```bash
# Greedy
g++ -O2 -o greedy knight_tour_greedy.cpp
./greedy
# → Enter board size: 8

# Brute Force
g++ -O2 -o brute knight_tour_bruteforce.cpp
./brute
# → Enter board size n: 6
```

Requires a C++14-compatible compiler (GCC, Clang, MSVC).

---

## Sample Output

### Greedy – N = 8 (Closed Tour Found)
```
Success! Closed tour found.
Attempts taken: 49
Total moves in the tour: 64
------------------------------
  1 16 49 34  3 18 21 52
 36 33  2 17 50 53  4 19
 15 64 35 48 39 20 51 22
 32 37 40 57 54 47 42  5
 61 14 63 38 41 56 23 46
 28 31 60 55 58 43  6  9
 13 62 29 26 11  8 45 24
 30 27 12 59 44 25 10  7
```

### Greedy – Odd N (No Closed Tour)
```
Enter board size: 9
Could not find a closed tour after 5000 attempts.
```

---

## Key Takeaway

A **closed** knight's tour exists if and only if **N is even and N ≥ 6**. For all other board sizes either the geometry or the color-parity constraint makes it impossible. The greedy Warnsdorff approach solves valid boards in milliseconds, while brute-force backtracking becomes impractical beyond N = 6.
