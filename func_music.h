#ifndef FUNC_MUSIC_H
#define FUNC_MUSIC_H

#include <QWidget>

namespace Ui {
class func_Music;
}

class func_Music : public QWidget
{
    Q_OBJECT

public:
    explicit func_Music(QWidget *parent = nullptr);
    ~func_Music();

private:
    Ui::func_Music *ui;
};

#endif // FUNC_MUSIC_H
