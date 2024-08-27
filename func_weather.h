#ifndef FUNC_WEATHER_H
#define FUNC_WEATHER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class func_weather;
}

class func_weather : public QWidget
{
    Q_OBJECT

public:
    explicit func_weather(QWidget *parent = nullptr);
    ~func_weather();

private:
    Ui::func_weather *ui;
    QNetworkAccessManager *networkManager;

private slots:
    void onWeatherDataReceived(QNetworkReply *reply);
};

#endif // FUNC_WEATHER_H
