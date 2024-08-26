#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "func_music.h"
#include <QNetworkReply>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Default Stack
    ui->stackedWidget_func->setCurrentIndex(4);

    qInfo() << ui->stackedWidget_func->currentIndex();
    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::supportsSsl();
    qDebug() << QSslSocket::sslLibraryVersionString();
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
    ui->stackedWidget_func->setCurrentIndex(3);// Weather Stack
}

void MainWindow::on_pushButton_map_clicked()
{
    ui->stackedWidget_func->setCurrentIndex(4);// Map Stack
}

void MainWindow::on_pushButton_IM_clicked()
{
    ui->stackedWidget_func->setCurrentIndex(5);// IM Stack
}

void MainWindow::on_pushButton_browser_clicked()
{
    ui->stackedWidget_func->setCurrentIndex(6);// Browser Stack
}
