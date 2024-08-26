#ifndef FUNC_MUSIC_H
#define FUNC_MUSIC_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QListWidget>
#include <QVector>
#include <QPair>

class func_Music : public QWidget
{
    Q_OBJECT

public:
    explicit func_Music(QWidget *parent = nullptr);
    ~func_Music();

private slots:
    void togglePlayPause();
    void playPreviousSong();
    void playNextSong();
    void on_selectMusicButton_clicked();
    void changePlayMode();
    void updatePlayModeButtonText();
    void updateSeekBar();
    void loadLyrics(const QString &lyricPath);
    void updateLyric();
    void loadMusicFiles();
    void showPlaylist();
    void shufflePlaylist();  // 声明 shufflePlaylist 函数
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status); // 新增槽函数
    void onSeekBarValueChanged(int value);

    void addToFavorites();  // 添加当前播放的歌曲到收藏夹
    void showFavorites();   // 显示收藏夹中的歌曲列表

private:
    QMediaPlayer *mediaPlayer;
    QMediaPlaylist *playlist;
    QLabel *musicImageView;
    QLabel *songTitleLabel;
    QLabel *artistLabel;
    QSlider *seekBar;
    QLabel *lyricLabel;
    QPushButton *playPauseButton;
    QPushButton *prevButton;
    QPushButton *nextButton;
    QPushButton *downloadButton;
    QPushButton *musicSelectionButton;
    QPushButton *changePlayModeButton;
    QPushButton *viewPlaylistButton;
    QPushButton *addToFavoritesButton; // 新增的按钮
    QPushButton *viewFavoritesButton;  // 新增的按钮
    QListWidget *musicListWidget;

    QList<QString> favoriteSongs; // 删除了原有的 favoritesList 变量
    QVector<QPair<qint64, QString>> lyrics; // 存储歌词
    int currentLyricIndex; // 当前歌词索引

    enum PlayMode {
        Loop,
        SingleLoop,
        Random
    };

    PlayMode currentPlayMode;

    void updateSongInfo();
    void handleRandomPlayback();  // 新函数处理随机播放
    void setupUi(); // 新增函数声明
    void connectSignals(); // 新增函数声明
};

#endif // FUNC_MUSIC_H
