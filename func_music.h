#ifndef FUNC_MUSIC_H
#define FUNC_MUSIC_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QListWidget>

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
    void shufflePlaylist();  // 在头文件中声明 shufflePlaylist 函数

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
    QListWidget *musicListWidget;

    enum PlayMode {
        Loop,
        SingleLoop,
        Random
    };

    PlayMode currentPlayMode;

    QVector<QPair<qint64, QString>> lyrics;
    int currentLyricIndex;

    void updateSongInfo();
};

#endif // FUNC_MUSIC_H
