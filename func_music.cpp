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
#include <algorithm> // 用于 std::random_shuffle

func_Music::func_Music(QWidget *parent)
    : QWidget(parent),
      mediaPlayer(new QMediaPlayer(this)),
      playlist(new QMediaPlaylist(this)),
      currentLyricIndex(0),
      currentPlayMode(Loop)
{
    setupUi();
    connectSignals();
    mediaPlayer->setPlaylist(playlist);
    playlist->setCurrentIndex(0);
}

func_Music::~func_Music()
{
    delete mediaPlayer;
    delete playlist;
}

void func_Music::setupUi()
{
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
    viewPlaylistButton = new QPushButton("View Playlist", this);
    addToFavoritesButton = new QPushButton("Add to Favorites", this);
    viewFavoritesButton = new QPushButton("View Favorites", this);
    musicListWidget = new QListWidget(this);
    currentTimeLabel = new QLabel("00:00", this);
    totalTimeLabel = new QLabel("00:00", this);

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
    mainLayout->addWidget(viewPlaylistButton);
    mainLayout->addWidget(addToFavoritesButton);
    mainLayout->addWidget(viewFavoritesButton);
    mainLayout->addWidget(musicListWidget);

    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addWidget(currentTimeLabel);
    timeLayout->addStretch();
    timeLayout->addWidget(totalTimeLabel);

    mainLayout->addLayout(timeLayout);

    setLayout(mainLayout);
}

void func_Music::connectSignals()
{
    connect(playPauseButton, &QPushButton::clicked, this, &func_Music::togglePlayPause);
    connect(prevButton, &QPushButton::clicked, this, &func_Music::playPreviousSong);
    connect(nextButton, &QPushButton::clicked, this, &func_Music::playNextSong);
    connect(downloadButton, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("https://music.163.com/"));
    });
    connect(musicSelectionButton, &QPushButton::clicked, this, &func_Music::on_selectMusicButton_clicked);
    connect(changePlayModeButton, &QPushButton::clicked, this, &func_Music::changePlayMode);
    connect(viewPlaylistButton, &QPushButton::clicked, this, &func_Music::showPlaylist);
    connect(addToFavoritesButton, &QPushButton::clicked, this, &func_Music::addToFavorites);
    connect(viewFavoritesButton, &QPushButton::clicked, this, &func_Music::showFavorites);

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &func_Music::updateSeekBar);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [=](qint64 duration) {
        seekBar->setMaximum(duration);

        // 更新总时间标签
        totalTimeLabel->setText(formatTime(duration));
    });
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &func_Music::onMediaStatusChanged);

    connect(seekBar, &QSlider::sliderReleased, this, [=]() {
        onSeekBarValueChanged(seekBar->value());
    });
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

void func_Music::onSeekBarValueChanged(int value)
{
    if (mediaPlayer->state() == QMediaPlayer::PlayingState) {
        mediaPlayer->setPosition(value);  // 设置媒体播放器的当前位置
    }
}

void func_Music::updateSeekBar()
{
    qint64 currentPosition = mediaPlayer->position();
    seekBar->setValue(currentPosition);

    // 更新当前时间标签
    QTime currentTime((currentPosition / 3600000) % 60, (currentPosition / 60000) % 60, (currentPosition / 1000) % 60);
    currentTimeLabel->setText(currentTime.toString("mm:ss"));

    while (currentLyricIndex < lyrics.size() && lyrics[currentLyricIndex].first <= currentPosition) {
        lyricLabel->setText(lyrics[currentLyricIndex].second);
        ++currentLyricIndex;
    }
}

QString func_Music::formatTime(qint64 duration)
{
    int minutes = (duration / 60000);  // 将毫秒转换为分钟
    int seconds = (duration % 60000) / 1000;  // 取余数并转换为秒
    return QString::asprintf("%02d:%02d", minutes, seconds);
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

void func_Music::playNextSong()
{
    int currentIndex = playlist->currentIndex();
    int nextIndex = (currentIndex + 1) % playlist->mediaCount(); // 循环播放
    playlist->setCurrentIndex(nextIndex);
    mediaPlayer->play();
    updateSongInfo();
}

void func_Music::playPreviousSong()
{
    int currentIndex = playlist->currentIndex();
    int previousIndex = (currentIndex - 1 + playlist->mediaCount()) % playlist->mediaCount(); // 循环播放
    playlist->setCurrentIndex(previousIndex);
    mediaPlayer->play();
    updateSongInfo();
}



void func_Music::on_selectMusicButton_clicked()
{
    loadMusicFiles();

    QStringList musicFiles;
    for (int i = 0; i < musicListWidget->count(); ++i) {
        musicFiles << musicListWidget->item(i)->text();
    }

    if (!musicFiles.isEmpty()) {
        QMessageBox::information(this, "Scanned Songs", "Scanned Songs:\n" + musicFiles.join("\n"));
    } else {
        QMessageBox::information(this, "No Music Files", "No MP3 files found on the USB drive.");
    }
}

void func_Music::loadMusicFiles()
{
    QStringList filters;
    filters << "*.mp3";

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

    connect(musicListWidget, &QListWidget::itemClicked, [=](QListWidgetItem *item) {
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
        playlist->setPlaybackMode(QMediaPlaylist::Sequential);  // 在 Random 模式下，播放模式仍设为 Sequential
        // 不立即打乱播放列表，而是在当前歌曲播放完毕后再处理
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
    musicListWidget->clear();  // 清空当前列表
    QStringList playlistItems;
    for (int i = 0; i < playlist->mediaCount(); ++i) {
        QUrl mediaUrl = playlist->media(i).canonicalUrl();
        playlistItems << mediaUrl.toLocalFile();  // 将文件路径添加到列表中
    }

    musicListWidget->addItems(playlistItems);  // 将播放列表显示在列表控件中
}

void func_Music::addToFavorites()
{
    int currentIndex = playlist->currentIndex();
    if (currentIndex >= 0 && currentIndex < musicListWidget->count()) {
        QString currentSong = musicListWidget->item(currentIndex)->text();
        if (!favoriteSongs.contains(currentSong)) {
            favoriteSongs.append(currentSong);
            QMessageBox::information(this, "Added to Favorites", "Song added to favorites.");
        } else {
            QMessageBox::information(this, "Already in Favorites", "Song is already in your favorites.");
        }
    }
}

void func_Music::showFavorites()
{
    musicListWidget->clear();  // 清空当前列表
    if (favoriteSongs.isEmpty()) {
        musicListWidget->addItem("No favorite songs yet.");
    } else {
        musicListWidget->addItems(favoriteSongs);  // 显示收藏夹中的歌曲
    }
}

void func_Music::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        if (currentPlayMode == SingleLoop) {
            mediaPlayer->play();  // 在 SingleLoop 模式下，自动重播当前歌曲
        } else if (currentPlayMode == Random) {
            shufflePlaylist();  // 在 Random 模式下，重新打乱播放列表
            playNextSong();  // 播放下一首歌曲
            showPlaylist();  // 自动显示播放列表
        }
    }
    // 更新歌曲信息
    updateSongInfo();
}

void func_Music::updateSongInfo()
{
    int currentIndex = playlist->currentIndex();
    if (currentIndex >= 0 && currentIndex < musicListWidget->count()) {
        QString currentSong = musicListWidget->item(currentIndex)->text();
        songTitleLabel->setText(QFileInfo(currentSong).baseName()); // 显示歌曲标题（不包含扩展名）
        artistLabel->setText("Artist Unknown"); // 这里可以根据实际情况设置艺术家信息

        // 设置音乐封面图片，假设图片名称与音乐文件相同，路径为 musicImages 文件夹
        QFileInfo fileInfo(currentSong);
        QString imagePath = QString("musicImages/%1.jpg").arg(fileInfo.baseName());
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            musicImageView->setPixmap(pixmap);
        } else {
            musicImageView->clear(); // 如果没有找到图片，则清空显示
        }
    } else {
        songTitleLabel->setText("No Song Playing");
        artistLabel->setText("");
        musicImageView->clear();
    }
}

