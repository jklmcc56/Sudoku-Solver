#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QValidator>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <Qfile>
#include <QTimer>

class SudokuWindow : public QWidget {
public:
    SudokuWindow();
    ~SudokuWindow();

private:
    QLineEdit* cells[9][9];
    int grid[9][9];

    QPushButton* LoadButton;
    QPushButton* CheckButton;
    QPushButton* SolveButton;

    void LoadSudoku(int FileIndex);
    void CheckSudoku();
    void SolveSudoku();

    void ResetCell(QLineEdit* cell);
};