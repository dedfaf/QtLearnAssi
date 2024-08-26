#include "func_image.h"
#include "ui_func_image.h"
#include <QDir>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QPixmap>
#include <QToolBar>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>

func_image::func_image(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::func_image),
    scaleFactor(1.0),
    rotationAngle(0)
{
    ui->setupUi(this);

    // 初始化列表和显示区域
     listWidget = new QListWidget(this);
     imageLabel = new QLabel(this);
     imageLabel->setAlignment(Qt::AlignCenter);
     imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     imageLabel->setScaledContents(true);  // 使图像自适应 QLabel 的大小

     // 初始化工具栏
     toolBar = new QToolBar(this);
     QAction *zoomInAction = toolBar->addAction("Zoom In");
     QAction *zoomOutAction = toolBar->addAction("Zoom Out");
     QAction *rotateLeftAction = toolBar->addAction("Rotate Left");
     QAction *rotateRightAction = toolBar->addAction("Rotate Right");
     QAction *previousAction = toolBar->addAction("Previous");
     QAction *nextAction = toolBar->addAction("Next");

     // 连接信号和槽
     connect(zoomInAction, &QAction::triggered, this, &func_image::zoomIn);
     connect(zoomOutAction, &QAction::triggered, this, &func_image::zoomOut);
     connect(rotateLeftAction, &QAction::triggered, this, &func_image::rotateLeft);
     connect(rotateRightAction, &QAction::triggered, this, &func_image::rotateRight);
     connect(previousAction, &QAction::triggered, this, &func_image::showPreviousImage);
     connect(nextAction, &QAction::triggered, this, &func_image::showNextImage);

     // 布局
     QVBoxLayout *mainLayout = new QVBoxLayout(this);
     QHBoxLayout *contentLayout = new QHBoxLayout();

     contentLayout->addWidget(listWidget, 1);
     contentLayout->addWidget(imageLabel, 3);

     mainLayout->addLayout(contentLayout);
     mainLayout->addWidget(toolBar);

     setLayout(mainLayout);

     // 加载图片
     loadImages();

     // 连接列表点击信号
     connect(listWidget, &QListWidget::itemClicked, this, &func_image::showImage);
}

func_image::~func_image()
{
    delete ui;
}

void func_image::loadImages()
{
    QDir directory("/home/user/桌面");  // 替换为实际的图像文件夹路径
    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.png", QDir::Files);
    foreach(QString filename, images) {
        QListWidgetItem *item = new QListWidgetItem(QIcon(directory.path() + "/" + filename), filename);
        item->setData(Qt::UserRole, directory.path() + "/" + filename);  // 保存图像路径
        listWidget->addItem(item);
    }
}

void func_image::showImage(QListWidgetItem *item)
{
    QString imagePath = item->data(Qt::UserRole).toString();
    originalPixmap.load(imagePath);

    scaleFactor = 1.0;
    rotationAngle = 0;

    imageLabel->setPixmap(originalPixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}

void func_image::showNextImage()
{
    int currentIndex = listWidget->currentRow();
    if (currentIndex < listWidget->count() - 1) {
        listWidget->setCurrentRow(currentIndex + 1);
        showImage(listWidget->currentItem());
    }
}

void func_image::showPreviousImage()
{
    int currentIndex = listWidget->currentRow();
    if (currentIndex > 0) {
        listWidget->setCurrentRow(currentIndex - 1);
        showImage(listWidget->currentItem());
    }
}

void func_image::zoomIn()
{
    scaleFactor *= 1.25;
    imageLabel->setPixmap(originalPixmap.scaled(originalPixmap.size() * scaleFactor, Qt::KeepAspectRatio));
}

void func_image::zoomOut()
{
    scaleFactor *= 0.8;
    imageLabel->setPixmap(originalPixmap.scaled(originalPixmap.size() * scaleFactor, Qt::KeepAspectRatio));
}

void func_image::rotateLeft()
{
    rotationAngle -= 90;
    QTransform transform;
    transform.rotate(rotationAngle);
    QPixmap rotatedPixmap = originalPixmap.transformed(transform, Qt::SmoothTransformation);
    imageLabel->setPixmap(rotatedPixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}

void func_image::rotateRight()
{
    rotationAngle += 90;
    QTransform transform;
    transform.rotate(rotationAngle);
    QPixmap rotatedPixmap = originalPixmap.transformed(transform, Qt::SmoothTransformation);
    imageLabel->setPixmap(rotatedPixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}
