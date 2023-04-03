#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QDirModel>
#include <QStandardItemModel>
#include <QTreeWidget>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "frozenTableView.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    frozenTableView *ftable = new frozenTableView(this);
    ui->layout->addWidget(ftable);
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(1000);
    model->setRowCount(100);
    ftable->setModel(model);
    ftable->setLastFrozenColumn(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}
