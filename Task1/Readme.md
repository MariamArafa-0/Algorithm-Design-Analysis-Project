#  Task 01 — Tromino Tiling

> **CSE245 — Design and Analysis of Algorithms | Spring 2026**  
> **Ain Shams University — Faculty of Engineering — CSES Program**

---

##  Problem Definition

Given a **2ⁿ × 2ⁿ** board (where n > 1) with **one missing square**, tile the
entire board with **right trominoes** (L-shaped tiles made of 3 adjacent squares)
using **only 3 colors**, such that no two trominoes sharing an edge have the
same color.

---

##  Assumptions & Constraints

- Board size is always a power of 2: `size = 2^n` where `n >= 1`
- Exactly **one** square is missing from the board
- The missing square can be at **any position** on the board
- Each tromino covers exactly **3 squares** in an L-shape
- **No overlapping** trominoes allowed
- **No tromino** may cover the missing square
- At most **3 colors** are used for coloring
- No two **adjacent** (edge-sharing) trominoes may share the same color

---

## ⚙️ Algorithms Investigated

---

### 🔹 Algorithm 1 — Divide and Conquer

**Core Idea:**  
Recursively divide the 2ⁿ × 2ⁿ board into four 2ⁿ⁻¹ × 2ⁿ⁻¹ quadrants.
One quadrant contains the missing square. Place a single tromino at the
center of the board covering one cell from each of the other three quadrants.
Treat those center cells as "missing" for their respective quadrants and recurse.

**Pseudocode:**
```
TILE(board, size, row, col, missing_row, missing_col):
    if size == 2:
        place tromino on the 3 non-missing cells
        return

    mid = size / 2
    find which quadrant contains the missing square
    place center tromino covering one cell from each other 3 quadrants
    treat those center cells as "missing" for their quadrant

    TILE(top-left quadrant,     mid, ...)
    TILE(top-right quadrant,    mid, ...)
    TILE(bottom-left quadrant,  mid, ...)
    TILE(bottom-right quadrant, mid, ...)

COLOR(tiles, adjacency):
    for each tile (in order):
        try colors 1, 2, 3
        assign first color not used by any neighbor
        if no color works: backtrack
```

**Implementation:** [`src/divide_and_conquer.cpp`](./src/divide_and_conquer.cpp)

**Complexity:**

| | Best Case | Average Case | Worst Case |
|-|-----------|--------------|------------|
| Tiling Time | O(4^n) | O(4^n) | O(4^n) |
| Coloring Time | O(T) | O(T × 3^T) | O(T × 3^T) |
| Space | O(4^n) | O(4^n) | O(4^n) |

> Where T = total number of trominoes = (4^n − 1) / 3

---

### 🔹 Algorithm 2 — Brute Force

**Core Idea:**  
Scan the board cell by cell (left to right, top to bottom). At each uncovered
cell, try all 4 possible L-tromino orientations. Place the first valid one and
recurse. If no orientation works, backtrack and try another orientation at the
previous cell. Continue until the board is fully tiled or all possibilities
are exhausted.

**The 4 L-Tromino Orientations:**
```
Type 0:    Type 1:    Type 2:    Type 3:
X .        X X        . X        X X
X X        X .        X X        . X
```

**Pseudocode:**
```
BRUTE_FORCE(board):
    find first empty cell (r, c)
    if no empty cell found: return SUCCESS

    for each orientation O in [0, 1, 2, 3]:
        if tromino O fits at (r, c):
            place tromino O at (r, c)
            if BRUTE_FORCE(board) == SUCCESS:
                return SUCCESS
            remove tromino O from (r, c)   // backtrack

    return FAILURE

COLOR(tiles, adjacency):
    same backtracking coloring as Divide & Conquer
```

**Implementation:** [`src/brute_force.cpp`](./src/brute_force.cpp)

**Complexity:**

| | Best Case | Average Case | Worst Case |
|-|-----------|--------------|------------|
| Tiling Time | O(4^n) | O(4^(4^n/3)) | O(4^(4^n/3)) |
| Coloring Time | O(T) | O(T × 3^T) | O(T × 3^T) |
| Space | O(4^n) | O(4^n) | O(4^n) |

>  Brute Force is only feasible for **n ≤ 2** due to exponential growth.

---

##  Comparative Evaluation

| Criterion | Divide & Conquer | Brute Force |
|-----------|:----------------:|:-----------:|
| **Time Complexity** | O(4^n) | O(4^(4^n/3)) |
| **Space Complexity** | O(4^n) | O(4^n) |
| **Implementation Complexity** | Medium | Low |
| **Scalability** |  Excellent |  Very Poor |
| **Optimality** |  Always finds solution |  Always finds solution |
| **Practical for n=1** |  Instant |  Instant |
| **Practical for n=2** |  Instant |  Fast |
| **Practical for n=3** |  Fast |  Slow |
| **Practical for n=4** |  Fast |  Infeasible |
| **Requires backtracking** |  No (tiling) |  Yes (tiling) |

---

##  Recommendation

> **Best Approach: Divide and Conquer**
>
> The Divide and Conquer algorithm is vastly superior for this problem.
> It solves the tiling in guaranteed **O(4^n)** time — directly proportional
> to the number of cells — with no backtracking required during the tiling phase.
> The Brute Force approach suffers from double-exponential complexity
> O(4^(4^n/3)), making it completely infeasible for any board larger than 4×4.
> The mathematical structure of the problem (power-of-2 board, one missing cell)
> is perfectly suited to the divide and conquer paradigm.

---

##  Sample Output

### Divide & Conquer — n=2 (4×4 board), missing at (0,0)

```
  +----+----+----+----+
  | XX | R 2| R 2| G 3|
  +----+----+----+----+
  | R 2| B 1| G 3| G 3|
  +----+----+----+----+
  | G 4| G 4| B 1| R 5|
  +----+----+----+----+
  | G 4| R 6| R 6| R 5|
  +----+----+----+----+
```

### Brute Force — n=2 (4×4 board), missing at (0,0)

```
  +----+----+----+----+
  | XX | R 1| G 2| G 2|
  +----+----+----+----+
  | R 1| R 1| B 3| G 2|
  +----+----+----+----+
  | G 4| B 5| B 3| B 3|
  +----+----+----+----+
  | G 4| G 4| B 5| R 6|
  +----+----+----+----+
```

---

##  References

- Levitin, A. (2012). *Introduction to The Design and Analysis of Algorithms* (3rd ed.). Pearson.
- Cormen, T. H., et al. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
- Golomb, S. W. (1994). *Polyominoes: Puzzles, Patterns, Problems, and Packings*. Princeton University Press.
