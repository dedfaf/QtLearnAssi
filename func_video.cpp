#include "func_video.h"
#include <QFileDialog>

func_video::func_video(QWidget *parent)
    : QWidget(parent)
{
    // 初始化 QMediaPlayer 和 QVideoWidget
    mediaPlayer = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    mediaPlayer->setVideoOutput(videoWidget);

    // 初始化按钮
    playButton = new QPushButton("Play", this);
    pauseButton = new QPushButton("Pause", this);
    stopButton = new QPushButton("Stop", this);

    // 初始化滑块
    positionSlider = new QSlider(Qt::Horizontal, this);

    // 布局管理
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(videoWidget);
    layout->addWidget(playButton);
    layout->addWidget(pauseButton);
    layout->addWidget(stopButton);
    layout->addWidget(positionSlider);

    setLayout(layout);  // 使用 QWidget 的 setLayout 而不是 QMainWindow 的 setCentralWidget

    // 连接信号和槽
    connect(playButton, &QPushButton::clicked, mediaPlayer, &QMediaPlayer::play);
    connect(pauseButton, &QPushButton::clicked, mediaPlayer, &QMediaPlayer::pause);
    connect(stopButton, &QPushButton::clicked, mediaPlayer, &QMediaPlayer::stop);
    connect(positionSlider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, positionSlider, &QSlider::setValue);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, positionSlider, &QSlider::setMaximum);
}

func_video::~func_video()
{
}

void func_video::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Video File", "", "Video Files (*.mp4 *.avi *.mkv)");
    if (!fileName.isEmpty()) {
        mediaPlayer->setMedia(QUrl::fromLocalFile(fileName));
        mediaPlayer->play();
    }
}
