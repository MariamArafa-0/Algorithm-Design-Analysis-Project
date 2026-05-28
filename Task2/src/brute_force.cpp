// ============================================================
//  Knight's Tour – Brute Force (Backtracking)
//  Explores every possible path; guaranteed to find a solution
//  if one exists, but exponential in time for large boards.
// ============================================================

#include <iostream>
#include <vector>
#include <string>

class KnightTourSolver
{
public:
    // All 8 knight moves (dx, dy)
    const int DX[8] = { 1,  2, -1, -2,  1,  2, -1, -2 };
    const int DY[8] = { 2,  1,  2,  1, -2, -1, -2, -1 };

    long long totalIterations = 0;

    bool isSafeMove(int x, int y, int n, const std::vector<std::vector<int>>& board) const
    {
        return x >= 0 && x < n && y >= 0 && y < n && board[x][y] == -1;
    }

    // Recursive backtracking search
    bool findTourPath(int cx, int cy, int moveCount, int n,
                      std::vector<std::vector<int>>& board)
    {
        ++totalIterations;

        if (moveCount == n * n)   // All cells visited
            return true;

        for (int k = 0; k < 8; ++k) {
            int nx = cx + DX[k];
            int ny = cy + DY[k];

            if (isSafeMove(nx, ny, n, board)) {
                board[nx][ny] = moveCount;

                if (findTourPath(nx, ny, moveCount + 1, n, board))
                    return true;

                board[nx][ny] = -1;   // backtrack
            }
        }
        return false;
    }

    // Entry point: initialise board and start from (0,0)
    std::vector<std::vector<int>> startSolving(int n)
    {
        std::vector<std::vector<int>> board(n, std::vector<int>(n, -1));
        board[0][0] = 0;

        if (findTourPath(0, 0, 1, n, board))
            return board;

        return {};   // empty means no solution found
    }
};

int main()
{
    int n;
    std::cout << "Enter board size n: ";
    std::cin >> n;

    KnightTourSolver solver;
    auto solution = solver.startSolving(n);

    if (!solution.empty()) {
        for (const auto& row : solution) {
            for (int val : row)
                std::cout << (val < 10 ? "  " : " ") << val;
            std::cout << "\n";
        }
    } else {
        std::cout << "No solution found.\n";
    }

    std::cout << "Number of iterations: " << solver.totalIterations << "\n";
    return 0;
}
