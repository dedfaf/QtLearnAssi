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

       // 初始化工具栏
       toolBar = new QToolBar(this);
       QAction *zoomInAction = toolBar->addAction("Zoom In");
       QAction *zoomOutAction = toolBar->addAction("Zoom Out");
       QAction *rotateLeftAction = toolBar->addAction("Rotate Left");
       QAction *rotateRightAction = toolBar->addAction("Rotate Right");

       // 连接工具栏按钮的信号与槽
       connect(zoomInAction, &QAction::triggered, this, &func_image::zoomIn);
       connect(zoomOutAction, &QAction::triggered, this, &func_image::zoomOut);
       connect(rotateLeftAction, &QAction::triggered, this, &func_image::rotateLeft);
       connect(rotateRightAction, &QAction::triggered, this, &func_image::rotateRight);

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

       // 布局
       QVBoxLayout *mainLayout = new QVBoxLayout(this);
       QHBoxLayout *imageLayout = new QHBoxLayout();
       QHBoxLayout *bottomLayout = new QHBoxLayout();

       // 添加工具栏到主布局
       mainLayout->addWidget(toolBar);

       // 设置图片显示区布局
       imageLayout->addWidget(prevButton);
       imageLayout->addWidget(imageLabel);
       imageLayout->addWidget(nextButton);
       mainLayout->addLayout(imageLayout);

       // 缩略图列表在底部
       bottomLayout->addWidget(listWidget);
       mainLayout->addLayout(bottomLayout);

       // 添加状态栏
       mainLayout->addWidget(statusBar);

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
       QDir directory("/home/user/桌面/test");  // 替换为实际的图像文件夹路径
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
