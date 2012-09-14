#include "src/forms/src/mainwindow.h"
#include "ui_mainwindow.h"
#include <QSize>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionCopyImages, SIGNAL(toggled(bool)), ui->textEdit, SLOT(copyImages(bool)));
    connect(ui->actionDeleteImages, SIGNAL(triggered()), ui->textEdit, SLOT(deleteImages()));
    ui->textEdit->populateEditWidget(ui->editToolBar);
    ui->editToolBar->setIconSize(QSize(40, 40));
    ui->textEdit->populateEditWidget(ui->menu_Edit);
    ui->textEdit->populateFontWidget(ui->fontToolBar);
    ui->fontToolBar->setIconSize(QSize(30, 30));
    setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isCopyImagedEnabled()
{
    return ui->actionCopyImages->isChecked();
}
