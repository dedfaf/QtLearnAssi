#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->stackedWidget_func->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_music_clicked()
{
    ui->stackedWidget_func->setCurrentIndex(0);
}

void MainWindow::on_pushButton_video_clicked()
{
    ui->stackedWidget_func->setCurrentIndex(1);
}

void MainWindow::on_pushButton_image_clicked()
{
//    ui->stackedWidget->
}

void MainWindow::on_pushButton_weather_clicked()
{
//    ui->stackedWidget->
}

void MainWindow::on_pushButton_map_clicked()
{
//    ui->stackedWidget->
}

void MainWindow::on_pushButton_im_clicked()
{
//    ui->stackedWidget->
}

void MainWindow::on_pushButton_browser_clicked()
{
//    ui->stackedWidget->
}
