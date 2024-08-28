#include "func_weather.h"
#include "ui_func_weather.h"

func_weather::func_weather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::func_weather),
    networkManager(new QNetworkAccessManager(this)),
    chart(new QChart),
    seriesHigh(new QLineSeries),
    seriesLow(new QLineSeries)
{

    locationInput = new QLineEdit(this);
    connect(locationInput, &QLineEdit::textChanged, this, &func_weather::onUserInputChanged);
    sendWeatherRequest("十堰");
    sendLifeSuggestionRequest("十堰");

    ui->setupUi(this);
    // 发送网络请求获取天气数据
    QUrl url("http://api.seniverse.com/v3/weather/daily.json?key=S2OKf1-kmmUzvOsML&location=beijing&language=zh-Hans&unit=c&start=-1&days=5");
    QNetworkRequest request(url);
    connect(networkManager, &QNetworkAccessManager::finished, this, &func_weather::onWeatherDataReceived);
    networkManager->get(request);

    QUrl url2("http://api.seniverse.com/v3/life/suggestion.json?key=S2OKf1-kmmUzvOsML&location=beijing&language=zh-Hans&days=1");
    QNetworkRequest request2(url2);
    connect(networkManager, &QNetworkAccessManager::finished, this, &func_weather::onSecondWeatherDataReceived);
    networkManager->get(request2);
}

func_weather::~func_weather()
{
    delete ui;
}

void func_weather::onWeatherDataReceived(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {

        if (reply->url().toString().contains("weather/daily.json")) {
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
                QString code = firstDayWeather["code_day"].toString();

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
                QString iconPath;

                if (code == "0") {
                    iconPath = ":/icon/0.png";
                } else if (code == "1") {
                    iconPath = ":/icon/1.png";
                } else if (code == "2") {
                    iconPath = ":/icon/2.png";
                }else if (code == "3") {
                    iconPath = ":/icon/3.png";
                }else if (code == "4") {
                    iconPath = ":/icon/4.png";
                }else if (code == "5") {
                    iconPath = ":/icon/5.png";
                }else if (code == "6") {
                    iconPath = ":/icon/6.png";
                }else if (code == "7") {
                    iconPath = ":/icon/7.png";
                }else if (code == "8") {
                    iconPath = ":/icon/8.png";
                }else if (code == "9") {
                    iconPath = ":/icon/9.png";
                }else if (code == "10") {
                    iconPath = ":/icon/10.png";
                }else if (code == "11") {
                    iconPath = ":/icon/11.png";
                }else if (code == "12") {
                    iconPath = ":/icon/12.png";
                }else if (code == "13") {
                    iconPath = ":/icon/13.png";
                }else if (code == "14") {
                    iconPath = ":/icon/14.png";
                }else if (code == "15") {
                    iconPath = ":/icon/15.png";
                }else if (code == "16") {
                    iconPath = ":/icon/16.png";
                }else if (code == "17") {
                    iconPath = ":/icon/17.png";
                }else if (code == "18") {
                    iconPath = ":/icon/18.png";
                }else if (code == "19") {
                    iconPath = ":/icon/19.png";
                }else if (code == "20") {
                    iconPath = ":/icon/20.png";
                }else if (code == "21") {
                    iconPath = ":/icon/21.png";
                }else if (code == "22") {
                    iconPath = ":/icon/22.png";
                }else if (code == "23") {
                    iconPath = ":/icon/23.png";
                }else if (code == "24") {
                    iconPath = ":/icon/24.png";
                }else if (code == "25") {
                    iconPath = ":/icon/25.png";
                }else if (code == "26") {
                    iconPath = ":/icon/26.png";
                }else if (code == "27") {
                    iconPath = ":/icon/27.png";
                }else if (code == "28") {
                    iconPath = ":/icon/28.png";
                }else if (code == "29") {
                    iconPath = ":/icon/29.png";
                }else if (code == "30") {
                    iconPath = ":/icon/30.png";
                }else if (code == "31") {
                    iconPath = ":/icon/31.png";
                }else if (code == "32") {
                    iconPath = ":/icon/32.png";
                }else if (code == "33") {
                    iconPath = ":/icon/33.png";
                }else if (code == "34") {
                    iconPath = ":/icon/34.png";
                }else if (code == "35") {
                    iconPath = ":/icon/35.png";
                }else if (code == "36") {
                    iconPath = ":/icon/36.png";
                }else if (code == "37") {
                    iconPath = ":/icon/37.png";
                }else if (code == "38") {
                    iconPath = ":/icon/38.png";
                }else {
                    iconPath = ":/icon/99.png";
                }

                QPixmap weatherIcon(iconPath);
                ui->icon->setPixmap(weatherIcon);

                ui->chartView->setChart(chart);

                chart->addSeries(seriesHigh);
                chart->addSeries(seriesLow);

                chart->createDefaultAxes();
                seriesHigh->clear();
                seriesLow->clear();
                // 设置X轴和Y轴
                QValueAxis *axisY = new QValueAxis;
                chart->setAxisY(axisY, seriesHigh);
                chart->setAxisY(axisY, seriesLow);

                QCategoryAxis *axisX = new QCategoryAxis;
                chart->setAxisX(axisX, seriesHigh);
                chart->setAxisX(axisX, seriesLow);

                for (int i = 0; i < dailyArray.size(); ++i) {
                    QJsonObject dayWeather = dailyArray[i].toObject();
                    QString date = dayWeather["date"].toString();
                    qreal highTemp = dayWeather["high"].toString().toDouble();
                    qreal lowTemp = dayWeather["low"].toString().toDouble();

                    // 添加数据点到折线图
                    seriesHigh->append(i, highTemp);
                    seriesLow->append(i, lowTemp);

                    ui->chartView->chart()->axisX()->setRange(0, dailyArray.size() - 1);
                    ui->chartView->chart()->axisY()->setRange(0, 35); // 假设温度范围在0-35之间

                    // 设置X轴标签
                    QCategoryAxis *axisX = static_cast<QCategoryAxis *>(chart->axisX());
                    axisX->append(date, i);
            }

            }
        }
    }
    else
    {
        // 处理错误
        qDebug() << "Error: " << reply->errorString();
    }
    reply->deleteLater();
}
}


void func_weather::onSecondWeatherDataReceived(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        if (reply->url().toString().contains("life/suggestion.json")) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonDoc.object();

            // 解析 JSON
            QJsonArray resultsArray = jsonObject["results"].toArray();
            if(!resultsArray.isEmpty())
            {
                QJsonArray suggestionArray = resultsArray[0].toObject()["suggestion"].toArray();

                if (!suggestionArray.isEmpty()) {
                    for (const QJsonValue &value : suggestionArray)
                    {
                        QJsonObject suggestionObject = value.toObject();

                        QString acDetails = suggestionObject["ac"].toObject()["details"].toString();
                        QString airPollutionDetails = suggestionObject["air_pollution"].toObject()["details"].toString();
                        QString airingDetails = suggestionObject["airing"].toObject()["details"].toString();
                        QString umbrella = suggestionObject["umbrella"].toObject()["details"].toString();
                        QString guomin = suggestionObject["allergy"].toObject()["details"].toString();
                        QString ziwai = suggestionObject["sunscreen"].toObject()["details"].toString();
                        QString xiche = suggestionObject["car_washing"].toObject()["details"].toString();
                        QString chuanyi = suggestionObject["dressing"].toObject()["details"].toString();
                        QString lumian = suggestionObject["traffic"].toObject()["details"].toString();
                        QString daban = suggestionObject["makeup"].toObject()["details"].toString();


                        // 更新 UI
                        ui->kongtiao->setText(acDetails);
                        ui->shiwai->setText(airPollutionDetails);
                        ui->daisan->setText(umbrella);
                        ui->guomin->setText(guomin);
                        ui->ziwaixian->setText(ziwai);
                        ui->xiche->setText(xiche);
                        ui->chuanyi->setText(chuanyi);
                        ui->lumian->setText(lumian);
                        ui->daban->setText(daban);
                    }
                }
            }
        }
    }
    else
    {
        // 处理错误

    }
    reply->deleteLater();
}

void func_weather::onUserInputChanged(const QString &text) {
    // 根据用户输入更新 URL 并重新发送请求
    sendWeatherRequest(text);
    sendLifeSuggestionRequest(text);
}

void func_weather::sendWeatherRequest(const QString &location) {
    QString encodedLocation = QString::fromUtf8(QUrl::toPercentEncoding(location));
    QUrl url(QString("http://api.seniverse.com/v3/weather/daily.json?key=S2OKf1-kmmUzvOsML&location=%1&language=zh-Hans&unit=c&start=-1&days=5")
                 .arg(encodedLocation));
    QNetworkRequest request(url);
    connect(networkManager, &QNetworkAccessManager::finished, this, &func_weather::onWeatherDataReceived);
    networkManager->get(request);
}

void func_weather::sendLifeSuggestionRequest(const QString &location) {
    QString encodedLocation = QString::fromUtf8(QUrl::toPercentEncoding(location));
    QUrl url(QString("http://api.seniverse.com/v3/life/suggestion.json?key=S2OKf1-kmmUzvOsML&location=%1&language=zh-Hans&days=1")
                 .arg(encodedLocation));
    QNetworkRequest request(url);
    connect(networkManager, &QNetworkAccessManager::finished, this, &func_weather::onSecondWeatherDataReceived);
    networkManager->get(request);
}
