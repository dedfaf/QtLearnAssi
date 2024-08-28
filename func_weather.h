#ifndef FUNC_WEATHER_H
#define FUNC_WEATHER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLineEdit>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>
#include <QDebug>

using namespace QtCharts;

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
    void sendWeatherRequest(const QString &location);
    void sendLifeSuggestionRequest(const QString &location);
    QNetworkAccessManager *networkManager;
    QLineEdit *locationInput;
    void processWeatherData(const QJsonObject &jsonObject, int source);
    QChart *chart;
    QLineSeries *seriesHigh;
    QLineSeries *seriesLow;

private slots:
    void onUserInputChanged(const QString &text);
    void onWeatherDataReceived(QNetworkReply *reply);
    void onSecondWeatherDataReceived(QNetworkReply *reply);
};

#endif // FUNC_WEATHER_H
