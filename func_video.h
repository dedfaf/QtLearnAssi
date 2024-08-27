#ifndef FUNC_VIDEO_H
#define FUNC_VIDEO_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QPushButton>
#include <QSlider>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

class func_video : public QWidget {
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
    void updateDuration(qint64 duration);
    void updatePosition(qint64 position);
    void seek(int position);
    void setVolume(int volume);
    void scanUsbDrive(const QString &drivePath);  // 新增槽函数声明

private:
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QPushButton *forwardButton;
    QPushButton *rewindButton;
    QPushButton *openFileButton;
    QPushButton *scanUsbButton;  // 新增按钮变量
    QSlider *positionSlider;
    QSlider *volumeSlider;
};

#endif // FUNC_VIDEO_H
