#include "func_weather.h"
#include "ui_func_weather.h"

func_weather::func_weather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::func_weather),
    networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    // 发送网络请求获取天气数据
    QUrl url("http://api.seniverse.com/v3/weather/daily.json?key=S2OKf1-kmmUzvOsML&location=beijing&language=zh-Hans&unit=c&start=-1&days=5");
    QNetworkRequest request(url);
    connect(networkManager, &QNetworkAccessManager::finished, this, &func_weather::onWeatherDataReceived);
    networkManager->get(request);
}

func_weather::~func_weather()
{
    delete ui;
}

void func_weather::onWeatherDataReceived(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonDoc.object();

        // 解析 JSON
        QJsonArray resultsArray = jsonObject["results"].toArray();
        if(!resultsArray.isEmpty())
        {
            QJsonObject locationObject = resultsArray[0].toObject()["location"].toObject();
            QJsonArray dailyArray = resultsArray[0].toObject()["daily"].toArray();

            QString locationName = locationObject["path"].toString();
            ui->locationLabel->setText("城市: " + locationName);

            if (!dailyArray.isEmpty()) {
                QJsonObject firstDayWeather = dailyArray[0].toObject();
                QString date = firstDayWeather["date"].toString();
                QString weather = firstDayWeather["text_day"].toString();
                QString weather_night = firstDayWeather["text_night"].toString();
                QString highTemp = firstDayWeather["high"].toString();
                QString lowTemp = firstDayWeather["low"].toString();
                QString wind_direction = firstDayWeather["wind_direction"].toString();
                QString wind_scale = firstDayWeather["wind_scale"].toString();
                QString humidity = firstDayWeather["humidity"].toString();

                QJsonObject secondDayWeather = dailyArray[1].toObject();
                QString date2 = secondDayWeather["date"].toString();
                QString weather2 = secondDayWeather["text_day"].toString();
                QString weather_night2 = secondDayWeather["text_night"].toString();
                QString highTemp2 = secondDayWeather["high"].toString();
                QString lowTemp2 = secondDayWeather["low"].toString();
                QString wind_direction2 = secondDayWeather["wind_direction"].toString();
                QString wind_scale2 = secondDayWeather["wind_scale"].toString();
                QString humidity2 = secondDayWeather["humidity"].toString();

                QJsonObject thirdDayWeather = dailyArray[2].toObject();
                QString date_3 = thirdDayWeather["date"].toString();
                QString weather_3 = thirdDayWeather["text_day"].toString();
                QString weather_night_3 = thirdDayWeather["text_night"].toString();
                QString highTemp_3 = thirdDayWeather["high"].toString();
                QString lowTemp_3 = thirdDayWeather["low"].toString();
                QString wind_direction_3 = thirdDayWeather["wind_direction"].toString();
                QString wind_scale_3 = thirdDayWeather["wind_scale"].toString();
                QString humidity_3 = thirdDayWeather["humidity"].toString();


                ui->dateLabel->setText("日期: " + date);
                ui->weatherLabel->setText("天气: " + weather + "转" + weather_night);
                ui->temperatureLabel->setText("温度: " + lowTemp + "℃" + "~" + highTemp + "℃");
                ui->wind->setText("风力: " + wind_direction + "风" + wind_scale + "级");
                ui->humidity->setText("湿度: " + humidity + "%");

                ui->dateLabel2->setText("日期: " + date2);
                ui->weatherLabel2->setText("天气: " + weather2 + "转" + weather_night2);
                ui->temperatureLabel2->setText("温度: " + lowTemp2 + "℃" + "~" + highTemp2 + "℃");
                ui->wind2->setText("风力: " + wind_direction2 + "风" + wind_scale2 + "级");
                ui->humidity2->setText("湿度: " + humidity2 + "%");

                ui->dateLabel_3->setText("日期: " + date_3);
                ui->weatherLabel_3->setText("天气: " + weather_3 + "转" + weather_night_3);
                ui->temperatureLabel_3->setText("温度: " + lowTemp_3 + "℃" + "~" + highTemp_3 + "℃");
                ui->wind_3->setText("风力: " + wind_direction_3 + "风" + wind_scale_3 + "级");
                ui->humidity_3->setText("湿度: " + humidity_3 + "%");
            }
        }
    }
    else
    {
        // 处理错误
        ui->weatherLabel->setText("Error: " + reply->errorString());
    }
    reply->deleteLater();
}
