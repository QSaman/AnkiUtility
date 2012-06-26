#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSize>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionCopyImages, SIGNAL(toggled(bool)), ui->textEdit, SLOT(copyImages(bool)));
    connect(ui->actionDeleteImages, SIGNAL(triggered()), ui->textEdit, SLOT(deleteImages()));
    ui->textEdit->populateEditToolBar(ui->editToolBar);
    ui->editToolBar->setIconSize(QSize(40, 40));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isCopyImagedEnabled()
{
    return ui->actionCopyImages->isChecked();
}
