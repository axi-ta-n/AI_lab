#include <iostream>
#include <limits.h>
using namespace std;
char board[3][3] = {{'1', '2', '3'},
                    {'4', '5', '6'},
                    {'7', '8', '9'}};
void printBoard()
{
    cout << "Current Board: \n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}
bool checkWin(char player)
{
    for (int i = 0; i < 3; i++)
    {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player))
            return true;
    }
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player))
        return true;

    return false;
}
bool checkDraw()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] != 'X' && board[i][j] != 'O')
            {
                return false;
            }
        }
    }
    return true;
}
int minimax(bool isMaximizing)
{
    if (checkWin('O'))
        return 1;
    if (checkWin('X'))
        return -1;
    if (checkDraw())
        return 0;
    if (isMaximizing)
    {
        int bestScore = INT_MIN;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] != 'X' && board[i][j] != 'O')
                {
                    char backup = board[i][j];
                    board[i][j] = 'O';
                    int score = minimax(false);
                    board[i][j] = backup;
                    bestScore = max(score, bestScore);
                }
            }
        }
        return bestScore;
    }
    else
    {
        int bestScore = INT_MAX;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] != 'X' && board[i][j] != 'O')
                {
                    char backup = board[i][j];
                    board[i][j] = 'X';
                    int score = minimax(true);
                    board[i][j] = backup;
                    bestScore = min(score, bestScore);
                }
            }
        }
        return bestScore;
    }
}
void computerMove()
{
    int bestScore = INT_MIN;
    int bestMove[2] = {-1, -1};
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] != 'X' && board[i][j] != 'O')
            {
                char backup = board[i][j];
                board[i][j] = 'O';
                int score = minimax(false);
                board[i][j] = backup;
                if (score > bestScore)
                {
                    bestScore = score;
                    bestMove[0] = i;
                    bestMove[1] = j;
                }
            }
        }
    }
    board[bestMove[0]][bestMove[1]] = 'O';
}
void playerMove()
{
    int choice;
    cout << "Enter the position (1-9) where you want to place X: ";
    cin >> choice;
    int row = (choice - 1) / 3;
    int col = (choice - 1) % 3;
    if (board[row][col] != 'X' && board[row][col] != 'O')
    {
        board[row][col] = 'X';
    }
    else
    {
        cout << "Invalid move, try again.\n";
        playerMove();
    }
}
int main()
{
    printBoard();
    while (true)
    {
        playerMove();

        printBoard();
        if (checkWin('X'))
        {
            cout << "Player wins!\n";
            break;
        }
        if (checkDraw())
        {
            cout << "It's a draw!\n";
            break;
        }
        computerMove();
        printBoard();
        if (checkWin('O'))
        {
            cout << "Computer wins!\n";
            break;
        }
        if (checkDraw())
        {
            cout << "It's a draw!\n";
            break;
        }
    }
    return 0;
}