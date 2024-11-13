#include "SudokuWindow.h"
#include "SudokuSolver.h"

SudokuWindow::SudokuWindow() {
    QGridLayout* GridLayout = new QGridLayout(this);

    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            QLineEdit* cell = new QLineEdit(this);
            cell->setMaxLength(1);
            QRegularExpression regex("[1-9]");
            QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, this);
            cell->setValidator(validator);
            cell->setAlignment(Qt::AlignCenter);
            cell->setFixedSize(40, 40);
            GridLayout->addWidget(cell, row, col);
            cells[row][col] = cell;
        }
    }

    QPushButton* LoadButton = new QPushButton("Load", this);
    GridLayout->addWidget(LoadButton, N, 0, 1, N);

    QPushButton* SolveButton = new QPushButton("Solve", this);
    GridLayout->addWidget(SolveButton, N + 1, 0, 1, N);

    connect(SolveButton, &QPushButton::clicked, this, &SudokuWindow::SolveSudoku);
    connect(LoadButton, &QPushButton::clicked, this, &SudokuWindow::ReadSudoku);
}

SudokuWindow::~SudokuWindow() {
}

void SudokuWindow::ReadSudoku() {
    QFile file("s0.txt"); //Add and read entire folder later
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to Open Sudoku File: " + file.errorString());

        return;
    }

    QTextStream in(&file);
    QString line;
    int row = 0;
    while (!in.atEnd() && row < N) {
        line = in.readLine();
        QStringList numbers = line.split(" ");
        numbers.removeAll("");

        if (numbers.length() != N) {
            QMessageBox::warning(this, "Invalid File", "The Sudoku File is Incorrect");

            return;
        }

        for (int col = 0; col < N; col++) {
            QLineEdit* cell = cells[row][col];
            QString number = numbers[col];
            cell->setText(number == "0" ? "" : number);
            grid[row][col] = number.toInt();

            if (number != "0") {
                QFont font = cell->font();
                font.setBold(true);
                cell->setFont(font);
                cell->setReadOnly(true);
            }
            else {
                cell->setReadOnly(false);
                cell->setStyleSheet("");
            }
        }

        row++;
    }

    file.close();
}

void SudokuWindow::SolveSudoku() {
    SudokuSolver solver(grid);
    if (solver.Solve(0, 0)) {
        int(*SolvedGrid)[N] = solver.GetGrid();
        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; col++) {
                if (cells[row][col]->text().toInt() == 0) {
                    cells[row][col]->setText(QString::number(SolvedGrid[row][col]));
                    cells[row][col]->setStyleSheet("QLineEdit { color: green; }");
                }
            }
        }
        QMessageBox::information(this, "Sudoku Solver", "Puzzle Solved!");
    }
    else {
        QMessageBox::warning(this, "Sudoku Solver", "Solution Not Found");
    }
}