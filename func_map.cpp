#include "func_map.h"
#include "ui_func_map.h"
#include <QDir>
#include <QtDebug>
#include <QSslSocket>
#include <QQuickItem>

func_map::func_map(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::func_map)
{
    ui->setupUi(this);

//    ui->quickWidget_mainMap->setResizeMode(QQuickWidget::SizeRootObjectToView);
//    ui->quickWidget_mainMap->setAttribute(Qt::WA_AlwaysStackOnTop);
//    ui->quickWidget_mainMap->setClearColor(Qt::transparent);

    QQuickItem *mapItem = ui->quickWidget_mainMap->rootObject();

    connect(ui->comboBox_mapTypeSelection, QOverload<int>::of(&QComboBox::currentIndexChanged), [mapItem](int indexSelected) {
        QObject *mapObject = mapItem->findChild<QQuickItem*>("mapObject");
        if (mapObject) {
            QVariant returnedValue;
            QMetaObject::invokeMethod(mapObject, "getMapType",
                                      Q_RETURN_ARG(QVariant, returnedValue),
                                      Q_ARG(QVariant, indexSelected));
            qDebug() << indexSelected;
            qDebug() << returnedValue;
            qDebug() << mapObject->property("activeMapType");

            mapObject->setProperty("activeMapType", returnedValue);
        }
    });

    connect(ui->pushButton_refreshMapData, &QPushButton::clicked, [mapItem]() {
        QObject *mapObject = mapItem->findChild<QQuickItem*>("mapObject");
        QMetaObject::invokeMethod(mapObject, "refreshMap");
    });

}

func_map::~func_map()
{
    delete ui;
}
