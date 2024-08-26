#ifndef FUNC_VIDEO_H
#define FUNC_VIDEO_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QMediaPlaylist>

class func_video : public QWidget
{
    Q_OBJECT

public:
    explicit func_video(QWidget *parent = nullptr);
    ~func_video();

private slots:
    void togglePlayPause();
    void playPreviousVideo();
    void playNextVideo();
    void updateVideoInfo();
    void updateSeekBar();
    void on_selectVideoButton_clicked();
    void loadVideoFiles();
    void changePlayMode();
    void showPlaylist();

private:
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QMediaPlaylist *playlist;
    QSlider *seekBar;
    QPushButton *playPauseButton;
    QPushButton *prevButton;
    QPushButton *nextButton;
    QPushButton *selectVideoButton;
    QPushButton *changePlayModeButton;
    QPushButton *viewPlaylistButton;
    QLabel *videoTitleLabel;
    QListWidget *videoListWidget;
};

#endif // FUNC_VIDEO_H
