#include "SudokuSolver.h"

SudokuSolver::SudokuSolver(int UnsolvedGrid[N][N]) {
	grid = new int[N][N];
	for (int row = 0; row < N; row++) {
		for (int col = 0; col < N; col++) {
			grid[row][col] = UnsolvedGrid[row][col];
		}
	}
}

bool SudokuSolver::Solve(int row, int col) {
	if (row == N - 1 && col == N) {
		return true;
	}

	if (col == N) {
		row++;
		col = 0;
	}

	if (grid[row][col] != 0) {
		return Solve(row, col + 1);
	}

	for (int num = 1; num <= N; num++) {
		if (CheckRow(num, row) && CheckColumn(num, col) && CheckSquare(num, row, col)) {
			grid[row][col] = num;
			if (Solve(row, col + 1)) {
				return true;
			}
		}
		grid[row][col] = 0;
	}

	return false;
}

bool SudokuSolver::CheckRow(int num, int row) {
	for (int col = 0; col < N; col++) {
		if (grid[row][col] == num) {
			return false;
		}
	}

	return true;
}

bool SudokuSolver::CheckColumn(int num, int col) {
	for (int row = 0; row < N; row++) {
		if (grid[row][col] == num) {
			return false;
		}
	}

	return true;
}

bool SudokuSolver::CheckSquare(int num, int row, int col) {
	row -= row % 3;
	col -= col % 3;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (grid[i + row][j + col] == num) {
				return false;
			}
		}
	}

	return true;
}

int (*SudokuSolver::GetGrid())[N] {
	return grid; 
}

//Does not print
void SudokuSolver::PrintGrid() {
	for (int row = 0; row < N; row++) {
		for (int col = 0; col < N; col++) {
			cout << grid[row][col] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

SudokuSolver::~SudokuSolver(){
}
