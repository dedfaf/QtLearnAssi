#include "func_map.h"
#include "ui_func_map.h"
#include <QDir>
#include <QtDebug>
#include <QSslSocket>

func_map::func_map(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::func_map)
{
    ui->setupUi(this);

//    ui->quickWidget_mainMap->setParent(this);
//    ui->quickWidget_mainMap->setResizeMode(QQuickWidget::SizeRootObjectToView);
//    ui->quickWidget_mainMap->setAttribute(Qt::WA_AlwaysStackOnTop);
//    ui->quickWidget_mainMap->setClearColor(Qt::transparent);



}

func_map::~func_map()
{
    delete ui;
}
