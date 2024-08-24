#include "func_music.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDirIterator>
#include <QFileInfo>
#include <QMessageBox>

func_Music::func_Music(QWidget *parent)
    : QWidget(parent),
      mediaPlayer(new QMediaPlayer(this)),
      playlist(new QMediaPlaylist(this)),
      currentLyricIndex(0),
      currentPlayMode(Loop) // 初始化播放模式
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
    changePlayModeButton = new QPushButton("Play Mode: Loop", this); // 默认显示循环播放模式
    viewPlaylistButton = new QPushButton("View Playlist", this); // 查看播放列表按钮
    musicListWidget = new QListWidget(this);

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
    mainLayout->addWidget(changePlayModeButton); // 添加到布局
    mainLayout->addWidget(viewPlaylistButton);  // 添加查看播放列表按钮到布局
    mainLayout->addWidget(musicListWidget);

    setLayout(mainLayout);

    // 连接信号和槽
    connect(playPauseButton, &QPushButton::clicked, this, &func_Music::togglePlayPause);
    connect(prevButton, &QPushButton::clicked, this, &func_Music::playPreviousSong);
    connect(nextButton, &QPushButton::clicked, this, &func_Music::playNextSong);
    connect(downloadButton, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("https://www.baidu.com"));
    });
    connect(musicSelectionButton, &QPushButton::clicked, this, &func_Music::on_selectMusicButton_clicked);
    connect(changePlayModeButton, &QPushButton::clicked, this, &func_Music::changePlayMode); // 连接播放模式切换按钮的信号
    connect(viewPlaylistButton, &QPushButton::clicked, this, &func_Music::showPlaylist); // 连接查看播放列表按钮的信号

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &func_Music::updateSeekBar);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [=](qint64 duration) {
        seekBar->setMaximum(duration);
    });

    // 初始化播放列表为空
    playlist->setCurrentIndex(0);
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
    if (playlist->mediaCount() > 0) {  // 确保播放列表中有歌曲
        playlist->previous();  // 切换到上一首歌曲
        mediaPlayer->play();  // 播放新设置的歌曲
        updateSongInfo();
    }
}

void func_Music::playNextSong()
{
    if (playlist->mediaCount() > 0) {  // 确保播放列表中有歌曲
        playlist->next();  // 切换到下一首歌曲
        mediaPlayer->play();  // 播放新设置的歌曲
        updateSongInfo();
    }
}

void func_Music::updateSongInfo()
{
    // 更新歌曲信息
    int currentIndex = playlist->currentIndex();
    if (currentIndex >= 0 && currentIndex < musicListWidget->count()) {
        QString currentSong = musicListWidget->item(currentIndex)->text();
        songTitleLabel->setText(QFileInfo(currentSong).baseName());
    }
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

void func_Music::on_selectMusicButton_clicked()
{
    loadMusicFiles();
}

void func_Music::loadMusicFiles()
{
    QStringList filters;
    filters << "*.mp3";

    // 在这里指定你的U盘路径
    QString usbPath;
#ifdef Q_OS_WIN
    usbPath = "F:/";  // 在Windows上指定U盘路径，例如F盘
#elif defined(Q_OS_LINUX)
    usbPath = "/media/user/USB";  // 在Linux上指定U盘路径
#else
    usbPath = QDir::homePath();  // 默认路径
#endif

    QDirIterator it(usbPath, filters, QDir::Files, QDirIterator::Subdirectories);
    QStringList musicFiles;

    while (it.hasNext()) {
        it.next();
        QFileInfo fileInfo(it.filePath());
        musicFiles << fileInfo.absoluteFilePath();  // 使用绝对路径
    }

    if (musicFiles.isEmpty()) {
        QMessageBox::information(this, "No Music Files", "No MP3 files found on the USB drive.");
        return;
    }

    musicListWidget->clear();
    musicListWidget->addItems(musicFiles);

    playlist->clear();  // 清空播放列表
    foreach (const QString &musicFile, musicFiles) {
        QMediaContent mediaContent(QUrl::fromLocalFile(musicFile));
        playlist->addMedia(mediaContent);  // 将所有歌曲添加到播放列表
    }
    playlist->setCurrentIndex(0);  // 确保播放列表从第一首歌开始

    // 处理列表项点击事件
    connect(musicListWidget, &QListWidget::itemClicked, [this](QListWidgetItem *item) {
        QString filePath = item->text();
        QFileInfo fileInfo(filePath);
        if (fileInfo.exists() && fileInfo.isFile()) {
            QMediaContent mediaContent(QUrl::fromLocalFile(filePath));
            playlist->clear();
            playlist->addMedia(mediaContent);
            playlist->setCurrentIndex(0);
            mediaPlayer->play();
            playPauseButton->setText("Pause");
            updateSongInfo();
        }
    });
}

void func_Music::changePlayMode()
{
    // 切换播放模式
    switch (currentPlayMode) {
    case Loop:
        currentPlayMode = SingleLoop;
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    case SingleLoop:
        currentPlayMode = Random;
        playlist->setPlaybackMode(QMediaPlaylist::Random);
        break;
    case Random:
        currentPlayMode = Loop;
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    }
    // 更新按钮文本
    updatePlayModeButtonText();
}

void func_Music::updatePlayModeButtonText()
{
    switch (currentPlayMode) {
    case Loop:
        changePlayModeButton->setText("Play Mode: Loop");
        break;
    case SingleLoop:
        changePlayModeButton->setText("Play Mode: Single Loop");
        break;
    case Random:
        changePlayModeButton->setText("Play Mode: Random");
        break;
    }
}

void func_Music::showPlaylist()
{
    // 显示播放列表的逻辑
    if (musicListWidget->isVisible()) {
        musicListWidget->hide();
        viewPlaylistButton->setText("View Playlist");
    } else {
        musicListWidget->show();
        viewPlaylistButton->setText("Hide Playlist");
    }
}
