#include "func_map.h"
#include "ui_func_map.h"
#include <QDir>
#include <QtDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQuickItem>
#include <QItemSelectionModel>

func_map::func_map(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::func_map),
    networkManager(new QNetworkAccessManager(this)),
    locResult_model(new QStringListModel(this))
{
    ui->setupUi(this);

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

    ui->listView_locResult->setModel(locResult_model);

    // Connect the move to button
    connect(ui->pushButton_moveTo, &QPushButton::clicked, this, &func_map::on_pushButton_moveTo_clicked);
}

func_map::~func_map()
{
    delete ui;
}

QStringList func_map::parseGeocodeJson(QByteArray jsonString)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonString);
    QJsonObject jsonObj = jsonResponse.object();

    QJsonArray featuresArray = jsonObj["features"].toArray();
    QStringList list;
    locResultData.clear(); // Clear previous data

    qDebug() << "Parsing JSON response...";
    qDebug() << featuresArray;

    for (const QJsonValue &value : featuresArray) {
        QJsonObject featureObj = value.toObject();
        QJsonObject propertiesObj = featureObj["properties"].toObject();

        QString address = propertiesObj["full_address"].toString();
        QString name = propertiesObj["name_preferred"].toString();
//        QString latitude = propertiesObj["lat"].toString();
//        QString longitude = propertiesObj["lon"].toString();
        QJsonObject coordinates = propertiesObj["coordinates"].toObject();
        double latitude = coordinates["latitude"].toDouble();
        double longitude = coordinates["longitude"].toDouble();

        qDebug() << featureObj;

        list << name;
        locResultData.append(QPair<double, double>(latitude, longitude)); // Store latitude and longitude
    }

    qDebug() << list;
    qDebug() << locResultData;
    return list;
}

void func_map::on_pushButton_locSearch_clicked()
{
    qDebug() << "Search button clicked";

    QString query = ui->lineEdit_locInput->text();
    QString accessToken = "sk.eyJ1Ijoid2lreW1vdXIiLCJhIjoiY20wYnphcmEwMGQ2aTJqcHYwdm9zZWkxbCJ9.Z-CC2Z24SDiILAogSPwssA";
    QUrl url("https://api.mapbox.com/search/geocode/v6/forward?q=" + query + "&country=CN&access_token=" + accessToken);

    qDebug() << "Request URL:" << url;

    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        qDebug() << "Reply finished";
        on_Reply_Finished(reply);
    });
}

void func_map::on_pushButton_moveTo_clicked()
{
    QQuickItem *mapItem = ui->quickWidget_mainMap->rootObject();
    QObject *mapObject = mapItem->findChild<QQuickItem*>("mapObject");

    QItemSelectionModel *selectionModel = ui->listView_locResult->selectionModel();
    QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

    int currentRow;

    foreach (const QModelIndex &index, selectedIndexes) {
        currentRow = index.row();
        qDebug() << "Selected row:" << currentRow ;
    }

    if (!selectedIndexes.isEmpty()) {
        qDebug() << "Latitude:" << locResultData[currentRow].first;
        qDebug() << "Longitude:" << locResultData[currentRow].second;
//        QVariantList coordinate;
//        coordinate << locResultData[currentRow].first << locResultData[currentRow].second;

        QMetaObject::invokeMethod(mapObject, "moveTo", Q_ARG(QVariant, locResultData[currentRow].first), Q_ARG(QVariant, locResultData[currentRow].second));
    } else {
        qDebug() << "No item is selected.";
    }
}

void func_map::on_Reply_Finished(QNetworkReply *reply)
{
    QByteArray replyJson = reply->readAll();
    qInfo() << "Reply received:" << replyJson;

    locResult_model->setStringList(parseGeocodeJson(replyJson));

    reply->deleteLater();
}