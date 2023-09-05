#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "backend.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(Backend *backend, QWidget *parent = nullptr);
    ~MainWindow();
    void onMoveButtonClicked();

private slots:

private:
    Ui::MainWindow *ui;
    Backend *m_backend;
};
#endif // MAINWINDOW_H
