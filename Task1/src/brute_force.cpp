/*
 * ============================================================
 *  CSE245 - Design and Analysis of Algorithms | Spring 2026
 *  Ain Shams University - CSES Program
 * ============================================================
 *  Task 01: Tromino Tiling
 *  Technique: Brute Force
 * ============================================================
 *
 *  Problem:
 *      Given a 2^n x 2^n board with one missing square,
 *      tile it with L-shaped right trominoes using only
 *      3 colors such that no two adjacent trominoes share
 *      the same color.
 *
 *  Approach:
 *      1. Scan the board left-to-right, top-to-bottom.
 *      2. Find the first uncovered cell.
 *      3. Try all 4 possible L-tromino orientations at that cell.
 *      4. For each valid placement, recursively fill the rest.
 *      5. Backtrack if no orientation leads to a solution.
 *      6. After full tiling, apply 3-coloring via backtracking.
 *
 *  Complexity:
 *      Time  : O(4^(4^n / 3)) — exponential due to brute search
 *      Space : O(4^n) for board + O(4^n / 3) recursion depth
 *
 *  Note:
 *      Brute Force is SIGNIFICANTLY slower than Divide & Conquer.
 *      Only practical for small n (n <= 2 recommended).
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <string>
#include <iomanip>
using namespace std;

// ─────────────────────────────────────────────
//  Global State
// ─────────────────────────────────────────────
int boardSize;
vector<vector<int>> board;
int tileID = 1;
map<int, vector<pair<int,int>>> tilesMap;
map<int, set<int>> adjacency;
map<int, int> colorMap;

// ─────────────────────────────────────────────
//  4 L-Tromino Orientations
//  Each orientation is a set of 3 relative (row,col) offsets
// ─────────────────────────────────────────────
//
//   Orientation 0     Orientation 1
//   [ ][ ]            [ ][ ]
//   [ ]               [ ][ ]  -- wait, only 3 cells
//
//   Actually the 4 L-tromino shapes (3 cells each):
//
//   Type 0:   Type 1:   Type 2:   Type 3:
//   X .        X X       . X       X X
//   X X        X .       X X       . X
//
// ─────────────────────────────────────────────
vector<vector<pair<int,int>>> ORIENTATIONS = {
    {{0,0},{1,0},{1,1}},  // Type 0: top-left L
    {{0,0},{0,1},{1,0}},  // Type 1: top-right L (flipped)
    {{0,0},{0,1},{1,1}},  // Type 2: top L
    {{0,1},{1,0},{1,1}}   // Type 3: bottom-right L
};

// ─────────────────────────────────────────────
//  Check if a tromino placement is valid
// ─────────────────────────────────────────────
bool canPlace(int r, int c, vector<pair<int,int>>& shape) {
    for (auto [dr, dc] : shape) {
        int nr = r + dr, nc = c + dc;
        if (nr < 0 || nr >= boardSize || nc < 0 || nc >= boardSize) return false;
        if (board[nr][nc] != 0) return false;  // already occupied
    }
    return true;
}

// ─────────────────────────────────────────────
//  Place / Remove a tromino
// ─────────────────────────────────────────────
void placeTile(int r, int c, vector<pair<int,int>>& shape, int id) {
    for (auto [dr, dc] : shape) {
        board[r + dr][c + dc] = id;
        tilesMap[id].push_back({r + dr, c + dc});
    }
}

void removeTile(int r, int c, vector<pair<int,int>>& shape, int id) {
    for (auto [dr, dc] : shape) {
        board[r + dr][c + dc] = 0;
    }
    tilesMap.erase(id);
}

// ─────────────────────────────────────────────
//  Find the first uncovered cell
// ─────────────────────────────────────────────
pair<int,int> findFirstEmpty() {
    for (int r = 0; r < boardSize; r++)
        for (int c = 0; c < boardSize; c++)
            if (board[r][c] == 0) return {r, c};
    return {-1, -1};
}

// ─────────────────────────────────────────────
//  Brute Force Recursive Tiling
// ─────────────────────────────────────────────
bool bruteForce() {
    auto [r, c] = findFirstEmpty();
    if (r == -1) return true;  // all cells covered

    for (auto& shape : ORIENTATIONS) {
        if (canPlace(r, c, shape)) {
            int cid = tileID++;
            placeTile(r, c, shape, cid);

            if (bruteForce()) return true;

            // Backtrack
            removeTile(r, c, shape, cid);
            tileID--;
        }
    }
    return false;
}

// ─────────────────────────────────────────────
//  Build Adjacency Graph
// ─────────────────────────────────────────────
void buildAdjacency() {
    for (auto& [tid, _] : tilesMap) adjacency[tid] = {};

    int dx[] = {0, 1};
    int dy[] = {1, 0};

    for (int r = 0; r < boardSize; r++) {
        for (int c = 0; c < boardSize; c++) {
            int tid = board[r][c];
            if (tid == -1) continue;
            for (int d = 0; d < 2; d++) {
                int nr = r + dx[d], nc = c + dy[d];
                if (nr < boardSize && nc < boardSize) {
                    int ntid = board[nr][nc];
                    if (ntid != -1 && ntid != tid) {
                        adjacency[tid].insert(ntid);
                        adjacency[ntid].insert(tid);
                    }
                }
            }
        }
    }
}

// ─────────────────────────────────────────────
//  3-Coloring via Backtracking
// ─────────────────────────────────────────────
vector<int> tileOrder;

bool backtrackColor(int idx) {
    if (idx == (int)tileOrder.size()) return true;
    int tid = tileOrder[idx];

    for (int color = 1; color <= 3; color++) {
        bool valid = true;
        for (int nb : adjacency[tid]) {
            if (colorMap.count(nb) && colorMap[nb] == color) {
                valid = false;
                break;
            }
        }
        if (valid) {
            colorMap[tid] = color;
            if (backtrackColor(idx + 1)) return true;
            colorMap.erase(tid);
        }
    }
    return false;
}

// ─────────────────────────────────────────────
//  Print the Board to Console
// ─────────────────────────────────────────────
void printBoard() {
    string colorNames[] = {"", "R", "G", "B"};
    cout << "\n Board Layout (R=Red, G=Green, B=Blue, X=Missing):\n\n";

    cout << "  +";
    for (int j = 0; j < boardSize; j++) cout << "----+";
    cout << "\n";

    for (int i = 0; i < boardSize; i++) {
        cout << "  |";
        for (int j = 0; j < boardSize; j++) {
            int tid = board[i][j];
            if (tid == -1) {
                cout << " XX |";
            } else {
                int color = colorMap.count(tid) ? colorMap[tid] : 0;
                cout << " " << colorNames[color] << setw(2) << tid << " |";
            }
        }
        cout << "\n  +";
        for (int j = 0; j < boardSize; j++) cout << "----+";
        cout << "\n";
    }
}

// ─────────────────────────────────────────────
//  Print Complexity Summary
// ─────────────────────────────────────────────
void printComplexity(int n) {
    int totalTiles = (int)((pow(4, n) - 1) / 3);
    cout << "\n ────────────────────────────────────────\n";
    cout << "  Complexity Analysis (Brute Force)\n";
    cout << " ────────────────────────────────────────\n";
    cout << "  Board Size      : " << boardSize << " x " << boardSize << "\n";
    cout << "  Total Trominoes : " << totalTiles << "\n";
    cout << "  Tiling Time     : O(4^(4^n/3)) — exponential\n";
    cout << "  Coloring Time   : O(T x 3^T) — backtracking\n";
    cout << "  Space           : O(4^n) board + O(T) stack\n";
    cout << "  WARNING         : Only feasible for n <= 2\n";
    cout << " ────────────────────────────────────────\n";
}

// ─────────────────────────────────────────────
//  Main
// ─────────────────────────────────────────────
int main() {
    cout << "========================================\n";
    cout << "     TROMINO TILING — Brute Force      \n";
    cout << "     CSE245 | Ain Shams University     \n";
    cout << "========================================\n\n";
    cout << " WARNING: Brute Force is only practical\n";
    cout << " for small boards (n <= 2). n=3+ may\n";
    cout << " take extremely long to compute.\n\n";

    int n;
    cout << " Enter n (board size = 2^n, n >= 1): ";
    cin >> n;

    if (n > 2) {
        cout << "\n [WARNING] n > 2 detected. Brute Force may run\n";
        cout << " for a very long time. Recommended: use n <= 2.\n";
        cout << " Continue anyway? (y/n): ";
        char ch; cin >> ch;
        if (ch != 'y' && ch != 'Y') return 0;
    }

    boardSize = (int)pow(2, n);
    int maxVal = boardSize - 1;

    int mr, mc;
    cout << " Enter missing row (0 to " << maxVal << "): ";
    cin >> mr;
    cout << " Enter missing col (0 to " << maxVal << "): ";
    cin >> mc;

    if (mr < 0 || mr > maxVal || mc < 0 || mc > maxVal) {
        cout << "\n [ERROR] Missing square out of bounds!\n";
        return 1;
    }

    // Initialize board
    board.assign(boardSize, vector<int>(boardSize, 0));
    board[mr][mc] = -1;

    cout << "\n [1/3] Brute force tiling in progress...\n";
    cout << "       (This may take a while for large n)\n";

    if (!bruteForce()) {
        cout << "\n [✘] Brute Force failed to tile the board.\n";
        return 1;
    }

    cout << " [2/3] Building adjacency graph...\n";
    buildAdjacency();

    cout << " [3/3] Applying 3-coloring via backtracking...\n";
    for (auto& [tid, _] : tilesMap) tileOrder.push_back(tid);

    if (backtrackColor(0)) {
        cout << "\n [✔] Successfully tiled and colored the board!\n";
        printBoard();
        printComplexity(n);
    } else {
        cout << "\n [✘] Could not find a valid 3-coloring.\n";
    }

    return 0;
}
