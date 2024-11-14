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
            GridLayout->addWidget(cell, row * 2, col * 2);
            cells[row][col] = cell;
        }
    }

    for (int row = 1; row < 3; ++row) {
        QFrame* HLine = new QFrame(this);
        HLine->setFrameShape(QFrame::HLine);
        HLine->setFrameShadow(QFrame::Sunken);
        HLine->setStyleSheet("background-color: black;");
        HLine->setFixedHeight(3);

        GridLayout->addWidget(HLine, row * 6 - 1, 0, 1, N * 2 - 1);
    }

    for (int col = 1; col < 3; ++col) {
        QFrame* VLine = new QFrame(this);
        VLine->setFrameShape(QFrame::VLine);
        VLine->setFrameShadow(QFrame::Sunken);
        VLine->setStyleSheet("background-color: black;");
        VLine->setFixedWidth(3);

        GridLayout->addWidget(VLine, 0, col * 6 - 1, N * 2 - 1, 1);
    }

    QComboBox* FileSelector = new QComboBox(this);
    for (int i = 0; i < 12; i++) {
        FileSelector->addItem(QString("Sudoku %1").arg(i + 1));
    }
    GridLayout->addWidget(FileSelector, N * 2 + 1, 0, 1, N * 2 - 1);

    LoadButton = new QPushButton("Load", this);
    GridLayout->addWidget(LoadButton, N * 2 + 2, 0, 1, N * 2 - 1);

    CheckButton = new QPushButton("Check", this);
    GridLayout->addWidget(CheckButton, N * 2 + 3, 0, 1, N * 2 - 1);

    SolveButton = new QPushButton("Solve", this);
    GridLayout->addWidget(SolveButton, N * 2 + 4, 0, 1, N * 2 - 1);

    connect(LoadButton, &QPushButton::clicked, [=]() {
        LoadSudoku(FileSelector->currentIndex());
        });
    connect(CheckButton, &QPushButton::clicked, this, &SudokuWindow::CheckSudoku);
    connect(SolveButton, &QPushButton::clicked, this, &SudokuWindow::SolveSudoku);

    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            QLineEdit* cell = cells[row][col];
            connect(cell, &QLineEdit::textChanged, this, [=]() { ResetCell(cell); });
        }
    }

    LoadButton->setFocus();
    LoadButton->setEnabled(true);
    CheckButton->setEnabled(false);
    SolveButton->setEnabled(false);
}

void SudokuWindow::LoadSudoku(int FileIndex) {
    QStringList SudokuFiles = {
    "s0.txt", "s1.txt", "s2.txt", "s3.txt", "s4.txt", "s5.txt", 
    "s6.txt", "s7.txt", "s8.txt", "s9.txt", "s10.txt", "s11.txt" };
    QString SelectedSudoku = SudokuFiles[FileIndex];
    QFile file(SelectedSudoku);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to Open Sudoku File: " + file.errorString());

        return;
    }

    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            QLineEdit* cell = cells[row][col];
            QFont font = cell->font();
            font.setBold(false);
            cell->setFont(font);
            cell->setReadOnly(false);
            cell->clear();
            cell->setStyleSheet("");
        }
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

    LoadButton->setEnabled(true);
    CheckButton->setEnabled(true);
    SolveButton->setEnabled(true);

    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            cells[row][col]->clearFocus();
        }
    }

    file.close();
}

void SudokuWindow::CheckSudoku() {
    SudokuSolver solver(grid);
    if (!solver.Solve(0, 0)) {
        QMessageBox::warning(this, "Sudoku Solver", "Solution Not Found - Invalid Sudoku");
    }

    int(*SolvedGrid)[N] = solver.GetGrid();
    bool solved = true;
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            QLineEdit* cell = cells[row][col];
            int SolvedCell = SolvedGrid[row][col];
            if (cell->text().toInt() == 0) {
                solved = false;
            }
            else if (cell->text().toInt() != SolvedCell) {
                cell->setStyleSheet("QLineEdit { color: red; }");
                solved = false;
            }
            else if (cell->text().toInt() == SolvedCell && !cell->font().bold()) {
                cell->setStyleSheet("QLineEdit { color: green; }");
                cell->setReadOnly(true);
            }
        }
    }

    if (solved == true) {
        QMessageBox::information(this, "Sudoku Solver", "Puzzle Solved!");

        LoadButton->setEnabled(true);
        CheckButton->setEnabled(false);
        SolveButton->setEnabled(true);

        for (int row = 0; row < N; ++row) {
            for (int col = 0; col < N; ++col) {
                cells[row][col]->clearFocus();
            }
        }
    }
    else {
        QMessageBox::information(this, "Sudoku Solver", "Error Detected - Wrong Number or Empty Cell!");
    }
}

void SudokuWindow::SolveSudoku() {
    SudokuSolver solver(grid);
    if (!solver.Solve(0, 0)) {
        QMessageBox::warning(this, "Sudoku Solver", "Solution Not Found - Invalid Sudoku");

        return;
    }

    int(*SolvedGrid)[N] = solver.GetGrid();
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            QLineEdit* cell = cells[row][col];
            int SolvedCell = SolvedGrid[row][col];
            if (cell->text().toInt() != SolvedCell) {
                cell->setText(QString::number(SolvedCell));
                cell->setStyleSheet("QLineEdit { color: red; }");
                cell->setReadOnly(true);
            }
            else if (cell->text().toInt() == SolvedCell && !cell->font().bold()) {
                cell->setStyleSheet("QLineEdit { color: green; }");
                cell->setReadOnly(true);
            }
        }
    }
    QMessageBox::information(this, "Sudoku Solver", "Puzzle Solved!");

    LoadButton->setEnabled(true);
    CheckButton->setEnabled(false);
    SolveButton->setEnabled(false);

    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            cells[row][col]->clearFocus();
        }
    }
}

void SudokuWindow::ResetCell(QLineEdit* cell) {
    cell->setStyleSheet("");
}

SudokuWindow::~SudokuWindow() {}