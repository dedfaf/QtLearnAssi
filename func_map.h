#ifndef FUNC_MAP_H
#define FUNC_MAP_H

#include <QWidget>
#include <QSslSocket>
#include <QStringListModel>
#include <QNetworkReply>

namespace Ui {
class func_map;
}

class func_map : public QWidget
{
    Q_OBJECT

public:
    explicit func_map(QWidget *parent = nullptr);
    ~func_map();

private:
    Ui::func_map *ui;
    QStringList parseGeocodeJson(QByteArray jsonString);
    void on_Search_Reply_Finished(QNetworkReply *reply);
    void on_Navi_Reply_Finished(QNetworkReply *reply);
    QList<QPair<double, double>> locResultData;
    QNetworkAccessManager *networkManager;
    QStringListModel* locResult_model = new QStringListModel();

private slots:
    void on_pushButton_locSearch_clicked();
    void on_pushButton_moveTo_clicked();
    void on_pushButton_addMark_clicked();
    void on_pushButton_navi_clicked();
};

#endif // FUNC_MAP_H
