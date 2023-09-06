#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::MainWindow(Backend *backend, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_backend(backend)
{
    ui->setupUi(this);
    connect(ui->launchButton, &QPushButton::clicked, this, &MainWindow::onMoveButtonClicked);
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMoveButtonClicked() {
    QString sourcePath = "test/test";
    QString destPath = "test/test1";

    m_backend->moveAllFiles(sourcePath, destPath);
}
