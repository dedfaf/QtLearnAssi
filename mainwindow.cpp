#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "func_music.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建 func_Music 实例并添加到 stackedWidget_func 中
    func_Music *musicWidget = new func_Music(this);
    ui->stackedWidget_func->addWidget(musicWidget);

    // 设置默认显示的页面（可根据需求设置）
    ui->stackedWidget_func->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_music_clicked()
{
    ui->stackedWidget_func->setCurrentIndex(0);  // 显示音乐页面
}

void MainWindow::on_pushButton_video_clicked()
{
    ui->stackedWidget_func->setCurrentIndex(1);  // 显示视频页面
}

void MainWindow::on_pushButton_image_clicked()
{
    // TODO: 实现图像页面切换
}

void MainWindow::on_pushButton_weather_clicked()
{
    // TODO: 实现天气页面切换
}

void MainWindow::on_pushButton_map_clicked()
{
    // TODO: 实现地图页面切换
}

void MainWindow::on_pushButton_im_clicked()
{
    // TODO: 实现即时消息页面切换
}

void MainWindow::on_pushButton_browser_clicked()
{
    // TODO: 实现浏览器页面切换
}
