#ifndef FUNC_VIDEO_H
#define FUNC_VIDEO_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QPushButton>
#include <QSlider>
#include <QStringList>

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
    void updateDuration(qint64 duration);
    void updatePosition(qint64 position);
    void seek(int position);
    void setVolume(int volume);
    void scanUsbDrive(const QString &drivePath);
    void playPrevious();  // 播放前一个视频
    void playNext();      // 播放下一个视频

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
    QPushButton *previousButton;  // 新增：前一个视频按钮
    QPushButton *nextButton;      // 新增：下一个视频按钮
    QSlider *volumeSlider;
    QSlider *progressSlider;

    QStringList videoFiles;  // 视频文件列表
    int currentIndex;        // 当前播放的视频索引
};

#endif // FUNC_VIDEO_H
