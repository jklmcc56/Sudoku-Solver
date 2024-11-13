#include <iostream>

#define N 9

using namespace std;

class SudokuSolver {
public:
		SudokuSolver(int UnsolvedGrid[N][N]);

		bool Solve(int row, int col);
		bool CheckRow(int num, int row);
		bool CheckColumn(int num, int col);
		bool CheckSquare(int num, int row, int col);

		int(*GetGrid())[N];

		void PrintGrid();

		~SudokuSolver();

private:
	int(*grid)[N];
};