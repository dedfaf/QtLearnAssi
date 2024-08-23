#include "func_music.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

func_Music::func_Music(QWidget *parent)
    : QWidget(parent),
      mediaPlayer(new QMediaPlayer(this)),
      playlist(new QMediaPlaylist(this)),
      currentLyricIndex(0)
{
    // 初始化 UI 组件
    mediaPlayer->setPlaylist(playlist);

    // 创建UI元素
    musicImageView = new QLabel(this);
    songTitleLabel = new QLabel("Song Title", this);
    artistLabel = new QLabel("Artist", this);
    seekBar = new QSlider(Qt::Horizontal, this);
    lyricLabel = new QLabel("", this);
    playPauseButton = new QPushButton("Play", this);
    prevButton = new QPushButton("Previous", this);
    nextButton = new QPushButton("Next", this);
    downloadButton = new QPushButton("Download Song", this);
    musicSelectionButton = new QPushButton("Select Music", this);

    // 设置布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(musicImageView);
    mainLayout->addWidget(songTitleLabel);
    mainLayout->addWidget(artistLabel);
    mainLayout->addWidget(seekBar);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(prevButton);
    controlLayout->addWidget(playPauseButton);
    controlLayout->addWidget(nextButton);
    mainLayout->addLayout(controlLayout);

    mainLayout->addWidget(lyricLabel);
    mainLayout->addWidget(downloadButton);
    mainLayout->addWidget(musicSelectionButton);

    setLayout(mainLayout);

    // 连接信号和槽
    connect(playPauseButton, &QPushButton::clicked, this, &func_Music::togglePlayPause);
    connect(prevButton, &QPushButton::clicked, this, &func_Music::playPreviousSong);
    connect(nextButton, &QPushButton::clicked, this, &func_Music::playNextSong);
    connect(downloadButton, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("https://www.baidu.com"));
    });

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &func_Music::updateSeekBar);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [=](qint64 duration) {
        seekBar->setMaximum(duration);
    });

    // 初始化播放列表为空
    playlist->setCurrentIndex(0);

    // 不自动播放任何音乐
}

func_Music::~func_Music()
{
    delete mediaPlayer;
    delete playlist;
}

void func_Music::togglePlayPause()
{
    if (mediaPlayer->state() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
        playPauseButton->setText("Play");
    } else {
        mediaPlayer->play();
        playPauseButton->setText("Pause");
    }
}

void func_Music::playPreviousSong()
{
    playlist->previous();
    updateSongInfo();
}

void func_Music::playNextSong()
{
    playlist->next();
    updateSongInfo();
}

void func_Music::updateSongInfo()
{
    // 更新歌曲信息
    songTitleLabel->setText("Current Song Title");  // 可以根据实际情况更新
    artistLabel->setText("Current Artist");
}

void func_Music::updateSeekBar()
{
    qint64 currentPosition = mediaPlayer->position();
    seekBar->setValue(currentPosition);

    // 更新歌词
    while (currentLyricIndex < lyrics.size() && lyrics[currentLyricIndex].first <= currentPosition) {
        lyricLabel->setText(lyrics[currentLyricIndex].second);
        ++currentLyricIndex;
    }
}

void func_Music::loadLyrics(const QString &lyricPath)
{
    lyrics.clear();
    QFile file(lyricPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpression regex("\\[(\\d+):(\\d+).(\\d+)](.*)");
        QRegularExpressionMatch match = regex.match(line);
        if (match.hasMatch()) {
            int minutes = match.captured(1).toInt();
            int seconds = match.captured(2).toInt();
            int millis = match.captured(3).toInt();
            QString content = match.captured(4);
            qint64 startTime = minutes * 60000 + seconds * 1000 + millis;
            lyrics.append(qMakePair(startTime, content));
        }
    }

    lyricLabel->clear();
}

void func_Music::updateLyric()
{
    if (currentLyricIndex < lyrics.size()) {
        lyricLabel->setText(lyrics[currentLyricIndex].second);
    }
}
