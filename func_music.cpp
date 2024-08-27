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
#include <QListWidget>
#include <QTime>
#include <algorithm> // 添加这个头文件用于 std::random_shuffle

func_Music::func_Music(QWidget *parent)
    : QWidget(parent),
      mediaPlayer(new QMediaPlayer(this)),
      playlist(new QMediaPlaylist(this)),
      currentLyricIndex(0),
      currentPlayMode(Loop)  // 默认播放模式设置为 Loop
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
    changePlayModeButton = new QPushButton("Play Mode: Loop", this);
    viewPlaylistButton = new QPushButton("View Playlist", this); // 新增查看播放列表按钮
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
    mainLayout->addWidget(changePlayModeButton);
    mainLayout->addWidget(viewPlaylistButton);  // 添加查看播放列表按钮到布局
    mainLayout->addWidget(musicListWidget);

    setLayout(mainLayout);

    // 连接信号和槽
    connect(playPauseButton, &QPushButton::clicked, this, &func_Music::togglePlayPause);
    connect(prevButton, &QPushButton::clicked, this, &func_Music::playPreviousSong);
    connect(nextButton, &QPushButton::clicked, this, &func_Music::playNextSong);
    connect(downloadButton, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("https://music.163.com/"));
    });
    connect(musicSelectionButton, &QPushButton::clicked, this, &func_Music::on_selectMusicButton_clicked);
    connect(changePlayModeButton, &QPushButton::clicked, this, &func_Music::changePlayMode);
    connect(viewPlaylistButton, &QPushButton::clicked, this, &func_Music::showPlaylist);  // 连接查看播放列表按钮的信号槽

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
    if (playlist->mediaCount() > 0) {
        playlist->previous();
        mediaPlayer->play();
        updateSongInfo();
    }
}

void func_Music::playNextSong()
{
    if (playlist->mediaCount() > 0) {
        playlist->next();
        mediaPlayer->play();
        updateSongInfo();
    }
}

void func_Music::updateSongInfo()
{
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

    QString usbPath;
#ifdef Q_OS_WIN
    usbPath = "C:/Users/86135/Desktop/video";  // 在Windows上指定U盘路径，例如F盘
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

    connect(musicListWidget, &QListWidget::itemClicked, [this](QListWidgetItem *item) {
        int index = musicListWidget->row(item);
        playlist->setCurrentIndex(index);
        mediaPlayer->play();
        playPauseButton->setText("Pause");
        updateSongInfo();
    });
}

void func_Music::changePlayMode()
{
    switch (currentPlayMode) {
    case Loop:
        currentPlayMode = SingleLoop;
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    case SingleLoop:
        currentPlayMode = Random;
        playlist->setPlaybackMode(QMediaPlaylist::Random);
        shufflePlaylist();  // 当模式切换到 Random 时调用随机化函数
        break;
    case Random:
        currentPlayMode = Loop;
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    }
    updatePlayModeButtonText();
}

void func_Music::shufflePlaylist()
{
    QList<QMediaContent> mediaList;
    int mediaCount = playlist->mediaCount();

    for (int i = 0; i < mediaCount; ++i) {
        mediaList.append(playlist->media(i));
    }

    // 使用时间种子初始化随机数生成器
    qsrand(QTime::currentTime().msec());

    // 打乱列表顺序
    std::random_shuffle(mediaList.begin(), mediaList.end());

    playlist->clear();
    foreach (const QMediaContent &mediaContent, mediaList) {
        playlist->addMedia(mediaContent);
    }
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
    QStringList playlistItems;
    for (int i = 0; i < playlist->mediaCount(); ++i) {
        QUrl mediaUrl = playlist->media(i).canonicalUrl();
        playlistItems << mediaUrl.toLocalFile();  // 将文件路径添加到列表中
    }

    // 弹出消息框显示播放列表
    QMessageBox::information(this, "Playlist", "Current Playlist:\n" + playlistItems.join("\n"));
}
