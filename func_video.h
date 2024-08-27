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
    void setVolume(int volume);
    void scanUsbDrive(const QString &drivePath);

    // 移除了进度条相关的槽函数声明
    // void updateDuration(qint64 duration);
    // void updatePosition(qint64 position);
    // void seek(int position);

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
    // QSlider *positionSlider;  // 移除了进度条
    QSlider *volumeSlider;

    void updateDuration(qint64 duration);
    void updatePosition(qint64 position);
    void seek(int position);
};

#endif // FUNC_VIDEO_H
