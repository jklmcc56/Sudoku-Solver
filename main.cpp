#include "SudokuWindow.h"
#include "SudokuSolver.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    SudokuWindow window;
    window.setWindowTitle("Sudoku Solver");
    window.resize(400, 450);
    window.show();

    return app.exec();
}
