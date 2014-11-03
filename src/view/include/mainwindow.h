#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

// Forward declarations
class Controller;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    MainWindow(Controller * controller);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    Controller * controller_;
};

#endif // MAINWINDOW_H
