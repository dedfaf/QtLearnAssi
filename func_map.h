#ifndef FUNC_MAP_H
#define FUNC_MAP_H

#include <QWidget>
#include <QSslSocket>

namespace Ui {
class func_map;
}

class func_map : public QWidget
{
    Q_OBJECT

public:
    explicit func_map(QWidget *parent = nullptr);
    ~func_map();

private:
    Ui::func_map *ui;
};

#endif // FUNC_MAP_H
