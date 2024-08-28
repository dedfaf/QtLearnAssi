#include "func_video.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

func_video::func_video(QWidget *parent) : QWidget(parent), currentIndex(-1)
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
    previousButton = new QPushButton("Previous", this); // 新增
    nextButton = new QPushButton("Next", this);         // 新增
    volumeSlider = new QSlider(Qt::Horizontal, this);
    progressSlider = new QSlider(Qt::Horizontal, this);

    progressSlider->setRange(0, 0);

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
    controlsLayout->addWidget(previousButton);  // 新增
    controlsLayout->addWidget(nextButton);      // 新增
    controlsLayout->addWidget(volumeSlider);

    mainLayout->addWidget(progressSlider);
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
    connect(previousButton, &QPushButton::clicked, this, &func_video::playPrevious);  // 新增
    connect(nextButton, &QPushButton::clicked, this, &func_video::playNext);          // 新增
    connect(volumeSlider, &QSlider::valueChanged, this, &func_video::setVolume);
    connect(progressSlider, &QSlider::sliderMoved, this, &func_video::seek);

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
    qint64 newPosition = player->position() + 5000;
    player->setPosition(newPosition);
}

void func_video::rewind()
{
    qint64 newPosition = player->position() - 5000;
    player->setPosition(newPosition);
}

void func_video::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video File"), "", tr("Video Files (*.mp4 *.avi *.mkv)"));
    if (!fileName.isEmpty()) {
        player->setMedia(QUrl::fromLocalFile(fileName));
        videoFiles.clear();  // 清除旧列表
        videoFiles.append(fileName);  // 添加新文件
        currentIndex = 0;  // 重置索引
        play();
    }
}

void func_video::updateDuration(qint64 duration)
{
    progressSlider->setRange(0, duration);
}

void func_video::updatePosition(qint64 position)
{
    if (!progressSlider->isSliderDown()) {
        progressSlider->setValue(position);
    }
}

void func_video::seek(int position)
{
    player->setPosition(position);
}

void func_video::setVolume(int volume)
{
    player->setVolume(volume);
}

void func_video::scanUsbDrive(const QString &drivePath)
{
    QDir usbDir(drivePath);
    QStringList filters;
    filters << "*.mp4";
    QFileInfoList fileList = usbDir.entryInfoList(filters, QDir::Files);

    if (!fileList.isEmpty()) {
        player->stop();
        videoFiles.clear();
        for (const QFileInfo &fileInfo : fileList) {
            videoFiles.append(fileInfo.absoluteFilePath());
        }
        currentIndex = 0;
        player->setMedia(QUrl::fromLocalFile(videoFiles.at(currentIndex)));
        play();
    } else {
        QMessageBox::information(this, tr("No Video Files"), tr("No MP4 files found on the USB drive."));
    }
}

void func_video::playPrevious()
{
    if (currentIndex > 0) {
        currentIndex--;
        player->setMedia(QUrl::fromLocalFile(videoFiles.at(currentIndex)));
        play();
    }
}

void func_video::playNext()
{
    if (currentIndex < videoFiles.size() - 1) {
        currentIndex++;
        player->setMedia(QUrl::fromLocalFile(videoFiles.at(currentIndex)));
        play();
    }
}
