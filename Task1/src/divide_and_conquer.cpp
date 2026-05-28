/*
 * ============================================================
 *  CSE245 - Design and Analysis of Algorithms | Spring 2026
 *  Ain Shams University - CSES Program
 * ============================================================
 *  Task 01: Tromino Tiling
 *  Technique: Divide and Conquer + 3-Coloring (Backtracking)
 * ============================================================
 *
 *  Problem:
 *      Given a 2^n x 2^n board with one missing square,
 *      tile it with L-shaped right trominoes using only
 *      3 colors such that no two adjacent trominoes share
 *      the same color.
 *
 *  Approach:
 *      1. Recursively divide the board into 4 quadrants.
 *      2. Place a tromino at the center covering 3 quadrants
 *         that do NOT contain the missing square.
 *      3. Recurse into each quadrant treating the center
 *         tromino cell as the "missing" square for that quad.
 *      4. After tiling, apply backtracking graph coloring
 *         with 3 colors so no adjacent trominoes share a color.
 *
 *  Complexity:
 *      Time  : O(4^n) for tiling | O(T * 3^T) for coloring
 *              where T = number of trominoes = (4^n - 1) / 3
 *      Space : O(4^n) for the board + O(T) for recursion stack
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
vector<vector<int>> board;        // stores tile IDs (-1 = missing)
int tileID = 1;
map<int, vector<pair<int,int>>> tilesMap;    // tileID -> list of cells
map<int, set<int>> adjacency;               // tileID -> neighboring tileIDs
map<int, int> colorMap;                     // tileID -> color (1,2,3)

// ─────────────────────────────────────────────
//  Place a 2x2 tromino (base case)
// ─────────────────────────────────────────────
void placeTile(int sz, int r, int c, int mr, int mc) {
    int cid = tileID++;
    for (int i = r; i < r + sz; i++) {
        for (int j = c; j < c + sz; j++) {
            if (i == mr && j == mc) continue;
            board[i][j] = cid;
            tilesMap[cid].push_back({i, j});
        }
    }
}

// ─────────────────────────────────────────────
//  Divide and Conquer Recursive Tiling
// ─────────────────────────────────────────────
void tileRecursive(int sz, int r, int c, int mr, int mc) {
    if (sz == 2) {
        placeTile(sz, r, c, mr, mc);
        return;
    }

    int mid = sz / 2;

    // Center cells of each quadrant
    vector<pair<int,int>> centerCoords = {
        {r + mid - 1, c + mid - 1},   // Top-Left quad
        {r + mid - 1, c + mid},        // Top-Right quad
        {r + mid,     c + mid - 1},    // Bottom-Left quad
        {r + mid,     c + mid}         // Bottom-Right quad
    };

    // Determine which quadrant contains the missing square
    vector<bool> quadMissing = {
        (mr < r + mid  && mc < c + mid),   // Top-Left
        (mr < r + mid  && mc >= c + mid),  // Top-Right
        (mr >= r + mid && mc < c + mid),   // Bottom-Left
        (mr >= r + mid && mc >= c + mid)   // Bottom-Right
    };

    // Place center tromino in the 3 quads that don't have the missing square
    int cid = tileID++;
    for (int i = 0; i < 4; i++) {
        if (!quadMissing[i]) {
            auto [cr, cc] = centerCoords[i];
            board[cr][cc] = cid;
            tilesMap[cid].push_back({cr, cc});
        }
    }

    // Quadrant offsets: [row_offset, col_offset]
    vector<pair<int,int>> quadOffsets = {{0,0},{0,mid},{mid,0},{mid,mid}};

    // Recurse into each quadrant
    for (int i = 0; i < 4; i++) {
        auto [ro, co] = quadOffsets[i];
        if (quadMissing[i]) {
            tileRecursive(mid, r + ro, c + co, mr, mc);
        } else {
            auto [cr, cc] = centerCoords[i];
            tileRecursive(mid, r + ro, c + co, cr, cc);
        }
    }
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

    // Top border
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
                int color = colorMap[tid];
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
    cout << "\n ─────────────────────────────────────────\n";
    cout << "  Complexity Analysis (Divide & Conquer)\n";
    cout << " ─────────────────────────────────────────\n";
    cout << "  Board Size      : " << boardSize << " x " << boardSize << "\n";
    cout << "  Total Trominoes : " << totalTiles << "\n";
    cout << "  Tiling Time     : O(4^n) = O(" << (int)pow(4,n) << ")\n";
    cout << "  Coloring Time   : O(T x 3^T) — backtracking\n";
    cout << "  Space           : O(4^n) board + O(T) stack\n";
    cout << " ─────────────────────────────────────────\n";
}

// ─────────────────────────────────────────────
//  Main
// ─────────────────────────────────────────────
int main() {
    cout << "========================================\n";
    cout << "   TROMINO TILING — Divide & Conquer   \n";
    cout << "   CSE245 | Ain Shams University       \n";
    cout << "========================================\n\n";

    int n;
    cout << " Enter n (board size = 2^n, n >= 1): ";
    cin >> n;

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

    cout << "\n [1/3] Tiling the board using Divide & Conquer...\n";
    tileRecursive(boardSize, 0, 0, mr, mc);

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

