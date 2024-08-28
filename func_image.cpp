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
#include <QPushButton>
#include <QStatusBar>
#include <QWheelEvent>

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
    imageLabel->setScaledContents(false);

// 为图片显示区添加隔离框
    QFrame *imageFrame = new QFrame(this);
    imageFrame->setFrameShape(QFrame::StyledPanel);  // 设置为面板样式
    imageFrame->setLineWidth(2);  // 设置线宽
    imageFrame->setContentsMargins(0, 0, 0, 0);  // 设置内容边距

// 设置样式表来确保边框可见
    imageFrame->setStyleSheet("QFrame { border: 2px solid black; padding: 5px; }");

// 使用布局管理 imageLabel 和 imageFrame
    QVBoxLayout *imageFrameLayout = new QVBoxLayout(imageFrame);
    imageFrameLayout->addWidget(imageLabel);
    imageFrame->setLayout(imageFrameLayout);

// 初始化工具栏按钮
    QPushButton *zoomInButton = new QPushButton("Zoom In", this);
    QPushButton *zoomOutButton = new QPushButton("Zoom Out", this);
    QPushButton *rotateLeftButton = new QPushButton("Rotate Left", this);
    QPushButton *rotateRightButton = new QPushButton("Rotate Right", this);

// 连接工具栏按钮的信号与槽
    connect(zoomInButton, &QPushButton::clicked, this, &func_image::zoomIn);
    connect(zoomOutButton, &QPushButton::clicked, this, &func_image::zoomOut);
    connect(rotateLeftButton, &QPushButton::clicked, this, &func_image::rotateLeft);
    connect(rotateRightButton, &QPushButton::clicked, this, &func_image::rotateRight);

// 创建用于工具栏按钮的水平布局，并均匀分布按钮
    QHBoxLayout *toolBarLayout = new QHBoxLayout();
    toolBarLayout->addWidget(zoomInButton);
    toolBarLayout->addWidget(zoomOutButton);
    toolBarLayout->addWidget(rotateLeftButton);
    toolBarLayout->addWidget(rotateRightButton);

// 左右图片切换按钮
    QPushButton *prevButton = new QPushButton("<", this);
    QPushButton *nextButton = new QPushButton(">", this);
    prevButton->setFixedSize(50, 50);
    nextButton->setFixedSize(50, 50);

    connect(prevButton, &QPushButton::clicked, this, &func_image::showPreviousImage);
    connect(nextButton, &QPushButton::clicked, this, &func_image::showNextImage);

// 状态栏用于显示当前文件名和图片序号
    QStatusBar *statusBar = new QStatusBar(this);
    statusBar->setSizeGripEnabled(false);  // 禁止调整窗口大小
    QLabel *statusLabel = new QLabel(this);
    statusBar->addWidget(statusLabel);

// 设置缩略图列表的固定大小
    listWidget->setFixedSize(120, 380);

// 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *imageLayout = new QHBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();  // 用于底部按钮的布局
    QHBoxLayout *statusLayout = new QHBoxLayout();  // 用于状态栏的布局
    QVBoxLayout *leftLayout = new QVBoxLayout();  // 左侧布局，包括缩略图列表和工具栏


// 将缩略图列表放在左侧
    leftLayout->addWidget(listWidget);

// 在主布局中添加左侧布局和图片显示区
    imageLayout->addLayout(leftLayout);
    imageLayout->addWidget(imageFrame);

// 将工具栏布局放在图片显示区上方
    mainLayout->addLayout(toolBarLayout); // 添加工具栏布局
    mainLayout->addLayout(imageLayout);

// 将左右图片切换按钮和状态栏放在图片显示区下面
    buttonLayout->addWidget(prevButton);
    buttonLayout->addStretch();  // 使按钮均匀分布
    buttonLayout->addWidget(nextButton);

// 添加状态栏到状态布局
    statusLayout->addWidget(statusBar);

// 设置主布局
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(statusLayout);

    setLayout(mainLayout);

// 加载图片
    loadImages();

// 连接列表点击信号
    connect(listWidget, &QListWidget::itemClicked, this, &func_image::showImage);

// 设置初始状态栏文本
    statusLabel->setText("当前文件名： 无");
}

func_image::~func_image()
{
   delete ui;
}

void func_image::loadImages()
{
   QDir directory("D:/Daily/#2024/8-28");  // 替换为实际的图像文件夹路径
   QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.png" << "*.gif" << "*.webp" << "*.bmp", QDir::Files);
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

   // 更新状态栏
   QLabel *statusLabel = findChild<QStatusBar *>()->findChild<QLabel *>();
   if (statusLabel) {
       int currentIndex = listWidget->currentRow() + 1;
       int totalImages = listWidget->count();
       statusLabel->setText(QString("当前文件名: %1    第%2/%3张").arg(imagePath).arg(currentIndex).arg(totalImages));
   }
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
// 获取当前显示图片的尺寸
    QSize currentSize = imageLabel->pixmap()->size();
    QSize newSize = currentSize * 1.25;

    // 限制最大放大倍数为原始图片的4倍
    if (newSize.width() <= originalPixmap.size().width() * 4 &&
        newSize.height() <= originalPixmap.size().height() * 4) {
        imageLabel->setPixmap(originalPixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        // 如果超出限制，则使用最大允许大小
        newSize = originalPixmap.size() * 4;
        imageLabel->setPixmap(originalPixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void func_image::zoomOut()
{
// 获取当前显示图片的尺寸
   QSize currentSize = imageLabel->pixmap()->size();
   QSize newSize = currentSize * 0.8;
   imageLabel->setPixmap(originalPixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
void func_image::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0) {
        // 向上滚动，放大图片
        zoomIn();
    } else {
        // 向下滚动，缩小图片
        zoomOut();
    }
}
