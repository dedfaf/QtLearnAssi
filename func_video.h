#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

class func_video : public QWidget  // 将 QMainWindow 改为 QWidget，因为您已经有主页了
{
    Q_OBJECT

public:
    func_video(QWidget *parent = nullptr);
    ~func_video();

private slots:
    void openFile();

private:
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QSlider *positionSlider;
};
