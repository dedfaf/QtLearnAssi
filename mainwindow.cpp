#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "func_music.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Default Stack
    ui->stackedWidget_func->setCurrentIndex(-1);

    qInfo() << ui->stackedWidget_func->currentIndex();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_music_clicked()
{
    ui->stackedWidget_func->setCurrentIndex(0);  // Music Stack
}

void MainWindow::on_pushButton_video_clicked()
{
    ui->stackedWidget_func->setCurrentIndex(1);  // Video Stack
}

void MainWindow::on_pushButton_image_clicked()
{
    ui->stackedWidget_func->setCurrentIndex(2); // Image Stack
}

void MainWindow::on_pushButton_weather_clicked()
{
    // TODO: // Weather Stack
}

void MainWindow::on_pushButton_map_clicked()
{
    // TODO: // Map Stack
}

void MainWindow::on_pushButton_IM_clicked()
{
    // TODO: // IM Stack
}

void MainWindow::on_pushButton_browser_clicked()
{
    // TODO: // Browser Stack
}
