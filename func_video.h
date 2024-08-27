#ifndef FUNC_VIDEO_H
#define FUNC_VIDEO_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QPushButton>
#include <QSlider>

class func_video : public QWidget
{
    Q_OBJECT

public:
    explicit func_video(QWidget *parent = nullptr);

private slots:
    void play();
    void pause();
    void stop();
    void forward();
    void rewind();
    void openFile();
    void updateDuration(qint64 duration); // 更新进度条范围的槽函数
    void updatePosition(qint64 position); // 更新进度条位置的槽函数
    void seek(int position);              // 跳转到指定位置的槽函数
    void setVolume(int volume);
    void scanUsbDrive(const QString &drivePath);

private:
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QPushButton *forwardButton;
    QPushButton *rewindButton;
    QPushButton *openFileButton;
    QPushButton *scanUsbButton;
    QSlider *volumeSlider;  // 音量滑块
    QSlider *progressSlider; // 进度条
};

#endif // FUNC_VIDEO_H
