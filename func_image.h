#ifndef FUNC_IMAGE_H
#define FUNC_IMAGE_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QToolBar>
#include <QPixmap>

namespace Ui {
class func_image;
}

class func_image : public QWidget
{
    Q_OBJECT

public:
    explicit func_image(QWidget *parent = nullptr);
    ~func_image();

private slots:
    void loadImages();           // 加载文件夹中的图像
    void showImage(QListWidgetItem *item);  // 显示选中的大图
    void showNextImage();        // 显示下一张图片
    void showPreviousImage();    // 显示上一张图片
    void zoomIn();               // 放大图片
    void zoomOut();              // 缩小图片
    void rotateLeft();           // 向左旋转图片
    void rotateRight();          // 向右旋转图片

private:
    Ui::func_image *ui;
    QListWidget *listWidget;     // 显示小图的列表
    QLabel *imageLabel;          // 显示大图
    QToolBar *toolBar;           // 工具条
    QPixmap originalPixmap;      // 当前显示的大图
    double scaleFactor;          // 缩放比例
    int rotationAngle;           // 旋转角度
};

#endif // FUNC_IMAGE_H
