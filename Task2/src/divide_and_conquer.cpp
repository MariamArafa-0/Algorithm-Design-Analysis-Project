// ============================================================
//  Knight's Tour – Greedy (Warnsdorff's Heuristic)
//  Finds a CLOSED tour if one exists within max_retries attempts.
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cstdlib>
#include <ctime>

// All 8 knight moves (dx, dy)
const int DX[] = { 1,  1,  2,  2, -1, -1, -2, -2 };
const int DY[] = { 2, -2,  1, -1,  2, -2,  1, -1 };

// Count onward moves from (x, y) on the current board
int degree(int x, int y, int n, const std::vector<std::vector<int>>& board)
{
    int count = 0;
    for (int k = 0; k < 8; ++k) {
        int nx = x + DX[k], ny = y + DY[k];
        if (nx >= 0 && nx < n && ny >= 0 && ny < n && board[ny][nx] == -1)
            ++count;
    }
    return count;
}

void solveKnightTour()
{
    int n;
    std::cout << "Enter board size: ";
    std::cin >> n;

    if (n < 3) {
        std::cout << "Size too small for a tour.\n";
        return;
    }

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    const int MAX_RETRIES = 5000;
    int totalAttempts = 0;

    for (int attempt = 1; attempt <= MAX_RETRIES; ++attempt) {
        totalAttempts = attempt;

        std::vector<std::vector<int>> board(n, std::vector<int>(n, -1));
        int cx = 0, cy = 0;
        board[cy][cx] = 1;

        bool stuck = false;
        for (int step = 2; step <= n * n; ++step) {
            // Collect valid next moves with their Warnsdorff degree
            std::vector<std::tuple<int,int,int>> options; // (degree, nx, ny)
            for (int k = 0; k < 8; ++k) {
                int nx = cx + DX[k], ny = cy + DY[k];
                if (nx >= 0 && nx < n && ny >= 0 && ny < n && board[ny][nx] == -1) {
                    int deg = degree(nx, ny, n, board);
                    options.emplace_back(deg, nx, ny);
                }
            }

            if (options.empty()) { stuck = true; break; }

            // Shuffle to break ties randomly, then pick minimum degree
            std::random_shuffle(options.begin(), options.end());
            auto best = *std::min_element(options.begin(), options.end(),
                [](const auto& a, const auto& b){ return std::get<0>(a) < std::get<0>(b); });

            cx = std::get<1>(best);
            cy = std::get<2>(best);
            board[cy][cx] = step;
        }

        if (stuck) continue;

        // Check if the last cell is one knight-move away from (0,0)
        bool isClosed = false;
        for (int k = 0; k < 8; ++k)
            if (cx + DX[k] == 0 && cy + DY[k] == 0) { isClosed = true; break; }

        if (isClosed) {
            std::cout << "\nSuccess! Closed tour found.\n";
            std::cout << "Attempts taken: " << totalAttempts << "\n";
            std::cout << "Total moves in the tour: " << n * n << "\n";
            std::cout << std::string(30, '-') << "\n";
            for (int row = 0; row < n; ++row) {
                for (int col = 0; col < n; ++col)
                    std::cout << (board[row][col] < 10 ? "  " : " ") << board[row][col];
                std::cout << "\n";
            }
            return;
        }
    }

    std::cout << "Could not find a closed tour after " << MAX_RETRIES << " attempts.\n";
}

int main()
{
    solveKnightTour();
    return 0;
}
