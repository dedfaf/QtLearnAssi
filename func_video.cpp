#include "func_video.h"
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
#include <algorithm> // For std::random_shuffle

func_video::func_video(QWidget *parent)
    : QWidget(parent),
      mediaPlayer(new QMediaPlayer(this)),
      videoWidget(new QVideoWidget(this)),
      playlist(new QMediaPlaylist(this))
{
    // Initialize UI components
    mediaPlayer->setVideoOutput(videoWidget);
    mediaPlayer->setPlaylist(playlist);

    videoTitleLabel = new QLabel("Video Title", this);
    seekBar = new QSlider(Qt::Horizontal, this);
    playPauseButton = new QPushButton("Play", this);
    prevButton = new QPushButton("Previous", this);
    nextButton = new QPushButton("Next", this);
    selectVideoButton = new QPushButton("Select Video", this);
    changePlayModeButton = new QPushButton("Play Mode: Loop", this);
    viewPlaylistButton = new QPushButton("View Playlist", this);
    videoListWidget = new QListWidget(this);

    // Set layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(videoWidget);
    mainLayout->addWidget(videoTitleLabel);
    mainLayout->addWidget(seekBar);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(prevButton);
    controlLayout->addWidget(playPauseButton);
    controlLayout->addWidget(nextButton);
    mainLayout->addLayout(controlLayout);

    mainLayout->addWidget(selectVideoButton);
    mainLayout->addWidget(changePlayModeButton);
    mainLayout->addWidget(viewPlaylistButton);
    mainLayout->addWidget(videoListWidget);

    setLayout(mainLayout);

    // Connect signals and slots
    connect(playPauseButton, &QPushButton::clicked, this, &func_video::togglePlayPause);
    connect(prevButton, &QPushButton::clicked, this, &func_video::playPreviousVideo);
    connect(nextButton, &QPushButton::clicked, this, &func_video::playNextVideo);
    connect(selectVideoButton, &QPushButton::clicked, this, &func_video::on_selectVideoButton_clicked);
    connect(changePlayModeButton, &QPushButton::clicked, this, &func_video::changePlayMode);
    connect(viewPlaylistButton, &QPushButton::clicked, this, &func_video::showPlaylist);

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &func_video::updateSeekBar);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [=](qint64 duration) {
        seekBar->setMaximum(duration);
    });

    playlist->setCurrentIndex(0);
}

func_video::~func_video()
{
    delete mediaPlayer;
    delete playlist;
}

void func_video::togglePlayPause()
{
    if (mediaPlayer->state() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
        playPauseButton->setText("Play");
    } else {
        mediaPlayer->play();
        playPauseButton->setText("Pause");
    }
}

void func_video::playPreviousVideo()
{
    if (playlist->mediaCount() > 0) {
        playlist->previous();
        mediaPlayer->play();
        updateVideoInfo();
    }
}

void func_video::playNextVideo()
{
    if (playlist->mediaCount() > 0) {
        playlist->next();
        mediaPlayer->play();
        updateVideoInfo();
    }
}

void func_video::updateVideoInfo()
{
    int currentIndex = playlist->currentIndex();
    if (currentIndex >= 0 && currentIndex < videoListWidget->count()) {
        QString currentVideo = videoListWidget->item(currentIndex)->text();
        videoTitleLabel->setText(QFileInfo(currentVideo).baseName());
    }
}

void func_video::updateSeekBar()
{
    qint64 currentPosition = mediaPlayer->position();
    seekBar->setValue(currentPosition);
}

void func_video::on_selectVideoButton_clicked()
{
    loadVideoFiles();
}

void func_video::loadVideoFiles()
{
    QStringList filters;
    filters << "*.mp4" << "*.avi" << "*.mkv"; // Video file formats

    QString usbPath;
#ifdef Q_OS_WIN
    usbPath = "F:/";  // For Windows, specify the USB drive path, e.g., F:/
#elif defined(Q_OS_LINUX)
    usbPath = "/media/user/USB";  // For Linux, specify the USB drive path
#else
    usbPath = QDir::homePath();  // Default path
#endif

    QDirIterator it(usbPath, filters, QDir::Files, QDirIterator::Subdirectories);
    QStringList videoFiles;

    while (it.hasNext()) {
        it.next();
        QFileInfo fileInfo(it.filePath());
        videoFiles << fileInfo.absoluteFilePath();  // Use absolute path
    }

    if (videoFiles.isEmpty()) {
        QMessageBox::information(this, "No Video Files", "No video files found on the USB drive.");
        return;
    }

    videoListWidget->clear();
    videoListWidget->addItems(videoFiles);

    playlist->clear();  // Clear the playlist
    foreach (const QString &videoFile, videoFiles) {
        QMediaContent mediaContent(QUrl::fromLocalFile(videoFile));
        playlist->addMedia(mediaContent);  // Add all videos to the playlist
    }
    playlist->setCurrentIndex(0);  // Ensure the playlist starts with the first video

    connect(videoListWidget, &QListWidget::itemClicked, [this](QListWidgetItem *item) {
        int index = videoListWidget->row(item);
        playlist->setCurrentIndex(index);
        mediaPlayer->play();
        playPauseButton->setText("Pause");
        updateVideoInfo();
    });
}

void func_video::changePlayMode()
{
    // Similar to music player play mode logic
    // Code here to change video play modes (e.g., Loop, Single Loop, Random)
}

void func_video::showPlaylist()
{
    QStringList playlistItems;
    for (int i = 0; i < playlist->mediaCount(); ++i) {
        QUrl mediaUrl = playlist->media(i).canonicalUrl();
        playlistItems << mediaUrl.toLocalFile();  // Add file paths to the list
    }

    // Show the current playlist in a message box
    QMessageBox::information(this, "Playlist", "Current Playlist:\n" + playlistItems.join("\n"));
}
