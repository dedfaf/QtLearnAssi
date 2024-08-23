#ifndef FUNC_MUSIC_H
#define FUNC_MUSIC_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
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
    void updateSongInfo();
    void updateSeekBar();
    void loadLyrics(const QString &lyricPath);
    void updateLyric();
    void on_selectMusicButton_clicked();  // 新增槽函数
    void loadMusicFiles();  // 新增方法

private:
    QMediaPlayer *mediaPlayer;
    QMediaPlaylist *playlist;
    QSlider *seekBar;
    QLabel *musicImageView;
    QLabel *songTitleLabel;
    QLabel *artistLabel;
    QLabel *lyricLabel;
    QPushButton *playPauseButton;
    QPushButton *prevButton;
    QPushButton *nextButton;
    QPushButton *downloadButton;
    QPushButton *musicSelectionButton;
    QListWidget *musicListWidget;  // 新增列表部件
    QList<QPair<qint64, QString>> lyrics;
    int currentLyricIndex;
};

#endif // FUNC_MUSIC_H
