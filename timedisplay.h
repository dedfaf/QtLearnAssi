#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H

//#include <QWidget>
#include <QLabel>

namespace Ui {
class timeDisplay;
}

class timeDisplay : public QLabel
{
    Q_OBJECT

public:
    explicit timeDisplay(QWidget *parent = nullptr);
    ~timeDisplay();

public slots:
    void onTimeOutput();

private:
    Ui::timeDisplay *ui;
};

#endif // TIMEDISPLAY_H
