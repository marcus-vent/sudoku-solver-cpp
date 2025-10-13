// Marcus Venter 55823130

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Sudoku grid size
const int SIZE = 9;

// Function prototypes
void printMenu();
void printGrid(int grid[SIZE][SIZE]);
bool loadGridFromFile(const string& filename, int grid[SIZE][SIZE]);
bool saveGridToFile(const string& filename, int grid[SIZE][SIZE]);
bool findEmptyCell(int grid[SIZE][SIZE], int& row, int& col);
bool isSafe(int grid[SIZE][SIZE], int row, int col, int num);
bool solveSudoku(int grid[SIZE][SIZE]);


// --- Main Program ---
int main() {
    // The 9x9 Sudoku grid
    int grid[SIZE][SIZE] = {0};
    bool isPuzzleLoaded = false;
    int choice;

    // Main program loop
    while (true)
    {
        printMenu();

        cin >> choice;

        // Handle user's menu choice
        switch (choice)
        {
            case 1:
            {
                string filename = "puzzle.txt";

                if (loadGridFromFile(filename, grid))
                {
                    cout << "\n[SUCCESS] Puzzle loaded successfully from '" << filename << "'.\n";
                    isPuzzleLoaded = true;
                }
                else
                {
                    cout << "\n[ERROR] Could not load puzzle from '" << filename << "'.\n";
                    cout << "Please ensure the file exists and is formatted correctly.\n";
                }

                break;
            }

            case 2:
            {
                if (!isPuzzleLoaded)
                {
                    cout << "\n[ERROR] No puzzle loaded. Please load a puzzle first (Option 1).\n";

                    break;
                }

                cout << "\nSolving puzzle...\n";

                if (solveSudoku(grid))
                {
                    cout << "[SUCCESS] Puzzle solved!\n";
                    printGrid(grid);
                }
                else
                {
                    cout << "[FAILURE] This puzzle has no solution.\n";
                }

                break;
            }

            case 3:
            {
                if (!isPuzzleLoaded)
                {
                    cout << "\n[ERROR] No puzzle loaded. Please load a puzzle first (Option 1).\n";

                    break;
                }

                cout << "\nCurrent Puzzle Grid:\n";

                printGrid(grid);

                break;
            }

            case 4:
            {
                 if (!isPuzzleLoaded)
                {
                    cout << "\n[ERROR] No puzzle loaded. Please load a puzzle first (Option 1).\n";

                    break;
                }

                string filename = "solution.txt";

                if(saveGridToFile(filename, grid))
                {
                    cout << "\n[SUCCESS] Current grid state saved to '" << filename << "'.\n";
                }
                else
                {
                    cout << "\n[ERROR] Could not save the grid.\n";
                }

                break;
            }

            case 5:

                cout << "\nExiting the Sudoku Solver. Goodbye!\n";

                return 0; // Exit program

            default:

                cout << "\n[ERROR] Invalid choice. Please enter a number between 1 and 5.\n";

                break;
        }

        cout << "\n";
    }

    return 0;
}

void printMenu() {
    cout << "==========================\n";
    cout << "    C++ Sudoku Solver\n";
    cout << "==========================\n";
    cout << "1. Load Puzzle from 'puzzle.txt'\n";
    cout << "2. Solve Puzzle\n";
    cout << "3. Display Current Grid\n";
    cout << "4. Save Current Grid to 'solution.txt'\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

// Prints the 9x9 Sudoku grid
void printGrid(int grid[SIZE][SIZE])
{
    cout << "+-------+-------+-------+\n";

    for (int row = 0; row < SIZE; ++row)
    {
        cout << "| ";

        for (int col = 0; col < SIZE; ++col)
        {
            if (grid[row][col] == 0)
            {
                 cout << ". ";
            }
            else
            {
                cout << grid[row][col] << " ";
            }
            if ((col + 1) % 3 == 0)
            {
                cout << "| ";
            }
        }
        cout << "\n";

        if ((row + 1) % 3 == 0)
        {
            cout << "+-------+-------+-------+\n";
        }
    }
}

// Loads puzzle from a file into grid
bool loadGridFromFile(const string& filename, int grid[SIZE][SIZE])
{
    ifstream file(filename);

    if (!file.is_open())
    {
        return false; // File open failed
    }

    for (int row = 0; row < SIZE; ++row)
    {
        for (int col = 0; col < SIZE; ++col)
        {
            if (!(file >> grid[row][col]))
            {
                file.close();

                return false; // Reading a number failed
            }
        }
    }

    file.close();
    return true;
}

// Saves the current grid to file
bool saveGridToFile(const string& filename, int grid[SIZE][SIZE])
{
    ofstream file(filename);

     if (!file.is_open())
    {
        return false; // File open for writing failed
    }

    for (int row = 0; row < SIZE; ++row)
    {
        for (int col = 0; col < SIZE; ++col)
        {
            file << grid[row][col] << (col == SIZE - 1 ? "" : " ");
        }

        file << "\n";
    }

    file.close();

    return true;
}


// Finds the next empty cell (value 0)
bool findEmptyCell(int grid[SIZE][SIZE], int& row, int& col)
{
    for (row = 0; row < SIZE; ++row)
    {
        for (col = 0; col < SIZE; ++col)
        {
            if (grid[row][col] == 0)
            {
                return true; // Found empty cell
            }
        }
    }
    return false; // No empty cells
}

// Checks if a number placement is valid.
bool isSafe(int grid[SIZE][SIZE], int row, int col, int num)
{
    // Check row
    for (int c = 0; c < SIZE; ++c)
        {
        if (grid[row][c] == num)
        {
            return false;
        }
    }

    // Check column
    for (int r = 0; r < SIZE; ++r)
    {
        if (grid[r][col] == num)
        {
            return false;
        }
    }

    // Check 3x3 sub-grid
    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            if (grid[r + startRow][c + startCol] == num)
            {
                return false;
            }
        }
    }

    return true; // Placement is safe
}

// Solves Sudoku using backtracking
bool solveSudoku(int grid[SIZE][SIZE])
{
    int row, col;

    // No empty cells means puzzle is solved
    if (!findEmptyCell(grid, row, col))
    {
        return true;
    }

    // Try numbers 1-9 for empty cell
    for (int num = 1; num <= 9; ++num)
    {
        // Check if placement is valid.
        if (isSafe(grid, row, col, num))
        {
            // Make move.
            grid[row][col] = num;

            // Recurse
            if (solveSudoku(grid))
            {
                return true; // Solution found
            }

            // Backtrack: undo the move
            grid[row][col] = 0;
        }
    }

    // No number worked, backtrack further
    return false;
}


