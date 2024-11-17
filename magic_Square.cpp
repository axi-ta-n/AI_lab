#include <iostream>
#include <vector>
using namespace std;

vector<int> board(10, 2); // Board initialized to 2 (blank)
int magic_square[10] = {0, 8, 3, 4, 1, 5, 9, 6, 7, 2}; // Magic square values
int turn = 1; // Keeps track of the current turn
int player_symbol, comp_symbol;

// Function to make a strategic move (Make2)
int Make2() {
    if (board[5] == 2) // Prefer center
        return 5;
    // Choose a corner (2, 4, 6, 8)
    vector<int> corners = {2, 4, 6, 8};
    for (int corner : corners) {
        if (board[corner] == 2)
            return corner;
    }
    return 0; // No move found
}

// Function to make a move
void Go(int n) {
    board[n] = (turn % 2 != 0) ? player_symbol : comp_symbol; // Player on odd turns, computer on even
    turn++;
}

// Function to check if a winning move is possible for a given symbol
int Posswin(int p) {
    for (int i = 1; i <= 9; i++) {
        if (board[i] == p) {
            for (int j = i + 1; j <= 9; j++) {
                if (board[j] == p) {
                    int third_pos = 15 - (magic_square[i] + magic_square[j]);
                    for (int k = 1; k <= 9; k++) {
                        if (magic_square[k] == third_pos && board[k] == 2) {
                            return k; // Return the winning move
                        }
                    }
                }
            }
        }
    }
    return 0; // No winning move
}

// Function to print the current state of the board
void printBoard() {
    for (int i = 1; i <= 9; i++) {
        if (board[i] == 2)
            cout << "- ";
        else if (board[i] == 3)
            cout << "X ";
        else
            cout << "O ";
        if (i % 3 == 0)
            cout << endl;
    }
}

// Computer's move logic
void compmoves() {
    if (turn == 1) {
        Go(1); // First move to top-left corner
    } else if (turn == 2) {
        if (board[5] == 2)
            Go(5); // Second move prefers center
        else
            Go(1);
    } else if (turn == 3) {
        if (board[9] == 2)
            Go(9); // Prefer bottom-right corner
        else
            Go(3);
    } else {
        int winMove = Posswin(comp_symbol);
        if (winMove != 0) {
            Go(winMove); // Winning move for computer
        } else {
            int blockMove = Posswin(player_symbol);
            if (blockMove != 0) {
                Go(blockMove); // Block player's winning move
            } else {
                Go(Make2()); // Strategic move
            }
        }
    }
}

// Check if a player has won
bool checking_for_winner(int symbol) {
    vector<vector<int>> lines = {
        {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, // Rows
        {1, 4, 7}, {2, 5, 8}, {3, 6, 9}, // Columns
        {1, 5, 9}, {3, 5, 7}             // Diagonals
    };

    for (auto &line : lines) {
        if (board[line[0]] == symbol && board[line[1]] == symbol && board[line[2]] == symbol) {
            return true; // Winning line found
        }
    }
    return false;
}

// Check if the board is full
bool filled_board() {
    for (int i = 1; i <= 9; i++) {
        if (board[i] == 2) // Blank space found
            return false;
    }
    return true;
}

// Main game logic
void PlayGame() {
    char choice;
    do {
        cout << "Choose X or O: ";
        cin >> choice;
    } while (choice != 'X' && choice != 'x' && choice != 'O' && choice != 'o');
    player_symbol = (choice == 'O' || choice == 'o') ? 5 : 3; // X = 3, O = 5
    comp_symbol = (player_symbol == 3) ? 5 : 3;

    while (true) {
        printBoard();
        if (turn % 2 != 0) {
            int move;
            cout << "Your turn (enter a number 1-9): ";
            cin >> move;
            if (move < 1 || move > 9 || board[move] != 2) {
                cout << "Invalid move, please try again." << endl;
                continue;
            }
            Go(move);
        } else {
            cout << "Computer's turn..." << endl;
            compmoves();
        }

        // Check for a winner
        if (checking_for_winner(player_symbol)) {
            printBoard();
            cout << "Congratulations! You won!" << endl;
            break;
        } else if (checking_for_winner(comp_symbol)) {
            printBoard();
            cout << "Computer won! Better luck next time!" << endl;
            break;
        }

        // Check for a draw
        if (filled_board()) {
            printBoard();
            cout << "It's a draw!" << endl;
            break;
        }
    }
}

// Main function
int main() {
    PlayGame();
    return 0;
}
