#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// 前向声明 func_Music 类
class func_Music;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_pushButton_music_clicked();
    void on_pushButton_video_clicked();
    void on_pushButton_image_clicked();
    void on_pushButton_weather_clicked();
    void on_pushButton_map_clicked();
    void on_pushButton_im_clicked();
    void on_pushButton_browser_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
