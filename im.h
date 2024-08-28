#ifndef IM_H
#define IM_H

#include <QWidget>

namespace Ui {
class IM;
}

class IM : public QWidget
{
    Q_OBJECT

public:
    explicit IM(QWidget *parent = nullptr);
    ~IM();

private:
    Ui::IM *ui;
};

#endif // IM_H
