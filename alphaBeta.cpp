#include <iostream>
#include <vector>

using namespace std;

int turn = 1;
int MAX_DEPTH = 4;
long long recursionCount = 0;

// Structure to hold the result of minimax
struct MinimaxResult {
    int value;
    vector<int> path;
};

// Function prototypes
void userMove(vector<vector<int>>& board, int turn);
bool checkWin(vector<vector<int>>& board, int player);
int rating(vector<vector<int>> board, int move, int player);
void displayBoard(vector<vector<int>> board);
MinimaxResult min_max_ab(vector<vector<int>> board, int player, int depth, int alpha, int beta);
void computerMove(vector<vector<int>>& board, int turn);

// Function to handle the user's move
void userMove(vector<vector<int>>& board, int turn) {
    int move;
    while (true) {
        cout << "Enter your move (1-9): ";
        cin >> move;
        int row = (move - 1) / 3;
        int col = (move - 1) % 3;

        if (move >= 1 && move <= 9 && board[row][col] == 2) {
            board[row][col] = (turn % 2 != 0) ? 3 : 5;
            break;
        }
        else {
            cout << "Invalid move. Try again!" << endl;
        }
    }
}

// Function to check if a player has won
bool checkWin(vector<vector<int>>& board, int player) {
    int product = player * player * player;
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] * board[i][1] * board[i][2]) == product) return true;
        if ((board[0][i] * board[1][i] * board[2][i]) == product) return true;
    }
    if ((board[0][0] * board[1][1] * board[2][2]) == product) return true;
    if ((board[0][2] * board[1][1] * board[2][0]) == product) return true;
    return false;
}

// Function to evaluate the board and rate moves
int rating(vector<vector<int>> board, int move, int player) {
    int opponent = (player == 3) ? 5 : 3;
    if (checkWin(board, player)) return 100; // Winning move
    if (checkWin(board, opponent)) return -100; // Opponent's winning move
    return 0; // Neutral score
}

// Function to display the board
void displayBoard(vector<vector<int>> board) {
    cout << "-------------------" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "| ";
        for (int j = 0; j < 3; j++) {
            char symbol = (board[i][j] == 2) ? ' ' : (board[i][j] == 3) ? 'X' : 'O';
            cout << " " << symbol << "  | ";
        }
        cout << endl;
        cout << "-------------------" << endl;
    }
}

// Alpha-beta pruning minimax function
MinimaxResult min_max_ab(vector<vector<int>> board, int player, int depth, int alpha, int beta) {
    recursionCount++;
    MinimaxResult result;
    result.value = 0;

    if (depth >= MAX_DEPTH || checkWin(board, 3) || checkWin(board, 5)) {
        result.value = rating(board, 0, player);
        return result;
    }

    vector<int> possibleMoves;
    for (int i = 1; i <= 9; i++) {
        int row = (i - 1) / 3, col = (i - 1) % 3;
        if (board[row][col] == 2) possibleMoves.push_back(i);
    }

    if (possibleMoves.empty()) {
        result.value = rating(board, 0, player);
        return result;
    }

    for (int move : possibleMoves) {
        vector<vector<int>> newBoard = board;
        int row = (move - 1) / 3, col = (move - 1) % 3;
        newBoard[row][col] = player;

        MinimaxResult childResult = min_max_ab(newBoard, (player == 3) ? 5 : 3, depth + 1, -beta, -alpha);
        int moveValue = -childResult.value;

        if (moveValue > alpha) {
            alpha = moveValue;
            result.path = childResult.path;
            result.path.insert(result.path.begin(), move);

            if (alpha >= beta) {
                result.value = alpha; // Prune
                return result;
            }
        }
    }

    result.value = alpha;
    return result;
}

// Function to handle the computer's move
void computerMove(vector<vector<int>>& board, int turn) {
    recursionCount = 0;
    int player = (turn % 2 == 0) ? 5 : 3;

    int alpha = -1000;
    int beta = 1000;

    MinimaxResult result = min_max_ab(board, player, 0, alpha, beta);

    if (!result.path.empty()) {
        int bestMove = result.path[0];
        int row = (bestMove - 1) / 3, col = (bestMove - 1) % 3;
        board[row][col] = player;
    }

    cout << "Total recursive calls: " << recursionCount << endl;
}

// Main function to start the game
int main() {
    vector<vector<int>> board(3, vector<int>(3, 2));
    while (true) {
        displayBoard(board);
        if (checkWin(board, 5)) {
            cout << "COMPUTER WON!" << endl;
            break;
        }
        if (checkWin(board, 3)) {
            cout << "USER WON!" << endl;
            break;
        }
        if (turn > 9) {
            cout << "IT'S A DRAW!" << endl;
            break;
        }
        userMove(board, turn);
        turn++;

        displayBoard(board);
        if (checkWin(board, 5)) {
            cout << "COMPUTER WON!" << endl;
            break;
        }
        if (checkWin(board, 3)) {
            cout << "USER WON!" << endl;
            break;
        }
        if (turn > 9) {
            cout << "IT'S A DRAW!" << endl;
            break;
        }
        computerMove(board, turn);
        turn++;
    }
    return 0;
}
