#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

class Cell {
private:
    bool isRevealed;
    bool isFlagged;

public:
    bool isMine;
    int numAdjacentMines;
    Cell() : isMine(false), isRevealed(false), isFlagged(false), numAdjacentMines(0) {}

    bool isCellMine() const {
        return isMine;
    }

    bool isCellRevealed() const {
        return isRevealed;
    }

    bool isCellFlagged() const {
        return isFlagged;
    }

    void setAdjacentMines(int count) {
        numAdjacentMines = count;
    }

    void revealCell() {
        isRevealed = true;
    }

    void flagCell() {
        isFlagged = true;
    }

    void unflagCell() {
        isFlagged = false;
    }

    friend ostream& operator<<(ostream& os, const Cell& cell) {
        if (cell.isRevealed) {
            if (cell.isMine) {
                os << "*";
            }
            else {
                os << cell.numAdjacentMines;
            }
        }
        else if (cell.isFlagged) {
            os << "F";
        }
        else {
            os << "-";
        }
        return os;
    }

    friend istream& operator>>(istream& is, Cell& cell) {
        char value;
        is >> value;
        cell.isMine = (value == '*');
        return is;
    }

    ~Cell() {}
};

class Board {
private:
    int rows;
    int columns;
    int numMines;
    int cellsToReveal;
    vector<vector<Cell>> grid;

public:
    Board(int rows, int columns, int numMines) : rows(rows), columns(columns), numMines(numMines), cellsToReveal(rows* columns - numMines) {
        grid.resize(rows, vector<Cell>(columns));
        placeMines();
        calculateAdjacentMines();
    }

    void play() {
        while (true) {
            displayBoard();

            int row, col;
            string action;
            cout << "Enter row, column, and action (R for reveal, F for flag/unflag): ";
            cin >> row >> col >> action;

            if (action == "R" || action == "r") {
                if (isValidMove(row, col)) {
                    if (grid[row][col].isCellMine()) {
                        cout << endl << "Game Over! You hit a mine." << endl;
                        revealBoard();
                        break;
                    }
                    else {
                        revealCell(row, col);
                        if (isGameWon()) {
                            cout << endl << "Congratulations! You won the game." << endl;
                            revealBoard();
                            break;
                        }
                    }
                }
            }
            else if (action == "F" || action == "f") {
                if (isValidCell(row, col)) {
                    if (grid[row][col].isCellRevealed()) {
                        cout << "Cannot flag a revealed cell." << endl;
                    }
                    else {
                        if (grid[row][col].isCellFlagged()) {
                            grid[row][col].unflagCell();
                            cout << "Cell unflagged." << endl;
                        }
                        else {
                            grid[row][col].flagCell();
                            cout << "Cell flagged." << endl;
                        }
                    }
                }
            }
            else {
                cout << "Invalid action. Try again." << endl;
            }
        }
    }

    ~Board() {}

private:
    void placeMines() {
        srand(time(nullptr));
        int minesPlaced = 0;
        while (minesPlaced < numMines) {
            int row = rand() % rows;
            int col = rand() % columns;
            if (!grid[row][col].isCellMine()) {
                grid[row][col] = Cell();
                grid[row][col].isMine = true;
                minesPlaced++;
            }
        }
    }
    void calculateAdjacentMines() {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                if (!grid[row][col].isCellMine()) {
                    int count = 0;
                    for (int i = -1; i <= 1; i++) {
                        for (int j = -1; j <= 1; j++) {
                            int newRow = row + i;
                            int newCol = col + j;
                            if (isValidCell(newRow, newCol) && grid[newRow][newCol].isCellMine()) {
                                count++;
                            }
                        }
                    }
                    grid[row][col].setAdjacentMines(count);
                }
            }
        }
    }

    bool isValidCell(int row, int col) const {
        return row >= 0 && row < rows&& col >= 0 && col < columns;
    }

    bool isValidMove(int row, int col) const {
        if (!isValidCell(row, col)) {
            cout << "Invalid move. Try again." << endl;
            return false;
        }
        else if (grid[row][col].isCellRevealed()) {
            cout << "Cell is already revealed." << endl;
            return false;
        }
        else if (grid[row][col].isCellFlagged()) {
            cout << "Cell is flagged. Unflag it to reveal." << endl;
            return false;
        }
        return true;
    }

    void revealCell(int row, int col) {
        if (isValidCell(row, col) && !grid[row][col].isCellRevealed() && !grid[row][col].isCellFlagged()) {
            grid[row][col].revealCell();
            cellsToReveal--;

            if (grid[row][col].numAdjacentMines == 0) {
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        int newRow = row + i;
                        int newCol = col + j;
                        if (isValidCell(newRow, newCol) && !grid[newRow][newCol].isCellRevealed()) {
                            revealCell(newRow, newCol);
                        }
                    }
                }
            }
        }
    }

    bool isGameWon() const {
        return cellsToReveal == 0;
    }

    void revealBoard() {
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                const int* revealCell();
            }
        }
        displayBoard();
    }

    void displayBoard() const {
        cout << endl << "    ";
        for (int col = 0; col < columns; col++) {
            cout << col << " ";
        }
        cout << endl;

        for (int row = 0; row < rows; row++) {
            if (row < 10) cout << ' ' << row << "  ";
            else cout << row << "  ";
            for (int col = 0; col < columns; col++) {
                cout << grid[row][col] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

class MinesweeperGame {
public:
    MinesweeperGame(int rows, int columns, int numMines) {
        Board board(rows, columns, numMines);
        board.play();
    }
};

int main() {
    int rows, columns, numMines;
    cout << "Enter number of rows (max = 10), columns (max = 10), and mines: ";
    cin >> rows >> columns >> numMines;
    if ((numMines > 0 && numMines <= rows * columns) && (rows > 0 && rows < 11) && (columns > 0 && columns < 11)) MinesweeperGame game(rows, columns, numMines);
    else if (!(rows > 0 && rows < 11) || !(columns > 0 && columns < 11)) {
        cout << "Invalid input. Number of rows and columns can't be less than 1 and greater than 10!" << endl << endl;
        main();
    }
    else if (!(numMines > 0) || (numMines > ((rows--) * (columns--)))) {
        cout << "Invalid input. Number of mines can't be less than 1 and greater than multiplication of rows and columns!" << endl << endl;
        main();
    }

    return 0;
}