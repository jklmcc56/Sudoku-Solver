#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QValidator>
#include <QPushButton>
#include <QMessageBox>
#include <Qfile>
#include <QTimer>

class SudokuWindow : public QWidget {
public:
    SudokuWindow();
    ~SudokuWindow();

private:
    QLineEdit* cells[9][9];
    int grid[9][9];

    void ReadSudoku();
    void SolveSudoku();
};