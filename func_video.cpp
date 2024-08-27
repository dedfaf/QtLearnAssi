#include "func_video.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

func_video::func_video(QWidget *parent) : QWidget(parent)
{
    player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    player->setVideoOutput(videoWidget);

    playButton = new QPushButton("Play", this);
    pauseButton = new QPushButton("Pause", this);
    stopButton = new QPushButton("Stop", this);
    forwardButton = new QPushButton("Forward", this);
    rewindButton = new QPushButton("Rewind", this);
    openFileButton = new QPushButton("Open File", this);
    scanUsbButton = new QPushButton("Scan USB", this);
    volumeSlider = new QSlider(Qt::Horizontal, this); // 保留音量滑块

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(videoWidget);

    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(openFileButton);
    controlsLayout->addWidget(scanUsbButton);
    controlsLayout->addWidget(playButton);
    controlsLayout->addWidget(pauseButton);
    controlsLayout->addWidget(stopButton);
    controlsLayout->addWidget(forwardButton);
    controlsLayout->addWidget(rewindButton);
    controlsLayout->addWidget(volumeSlider); // 只保留音量滑块

    mainLayout->addLayout(controlsLayout);
    setLayout(mainLayout);

    connect(playButton, &QPushButton::clicked, this, &func_video::play);
    connect(pauseButton, &QPushButton::clicked, this, &func_video::pause);
    connect(stopButton, &QPushButton::clicked, this, &func_video::stop);
    connect(forwardButton, &QPushButton::clicked, this, &func_video::forward);
    connect(rewindButton, &QPushButton::clicked, this, &func_video::rewind);
    connect(openFileButton, &QPushButton::clicked, this, &func_video::openFile);
    connect(scanUsbButton, &QPushButton::clicked, this, [this]() {
        QString usbPath = "F:/";  // 修改为您的U盘路径
        scanUsbDrive(usbPath);
    });
    connect(volumeSlider, &QSlider::valueChanged, this, &func_video::setVolume);

    connect(player, &QMediaPlayer::durationChanged, this, &func_video::updateDuration);
    connect(player, &QMediaPlayer::positionChanged, this, &func_video::updatePosition);

    player->setVolume(50);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
}

void func_video::play()
{
    player->play();
}

void func_video::pause()
{
    player->pause();
}

void func_video::stop()
{
    player->stop();
}

void func_video::forward()
{
    qint64 newPosition = player->position() + 5000;  // 快进5秒
    player->setPosition(newPosition);
}

void func_video::rewind()
{
    qint64 newPosition = player->position() - 5000;  // 倒退5秒
    player->setPosition(newPosition);
}

void func_video::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video File"), "", tr("Video Files (*.mp4 *.avi *.mkv)"));
    if (!fileName.isEmpty()) {
        player->setMedia(QUrl::fromLocalFile(fileName));
        play();  // 加载后自动开始播放
    }
}

void func_video::updateDuration(qint64 duration)
{
    // 进度条相关功能已移除，不再更新
}

void func_video::updatePosition(qint64 position)
{
    // 进度条相关功能已移除，不再更新
}

void func_video::seek(int position)
{
    // 进度条相关功能已移除，不再处理
}

void func_video::setVolume(int volume)
{
    player->setVolume(volume);
}

void func_video::scanUsbDrive(const QString &drivePath)
{
    QDir usbDir(drivePath);
    QStringList filters;
    filters << "*.mp4";  // 只扫描MP4文件
    QFileInfoList fileList = usbDir.entryInfoList(filters, QDir::Files);

    if (!fileList.isEmpty()) {
        // 停止当前播放
        player->stop();

        // 设置找到的第一个视频文件并播放
        player->setMedia(QUrl::fromLocalFile(fileList.first().absoluteFilePath()));
        play();

        // 如果需要将所有文件添加到某个播放列表，可以在这里实现
    } else {
        QMessageBox::information(this, tr("No Video Files"), tr("No MP4 files found on the USB drive."));
    }
}
