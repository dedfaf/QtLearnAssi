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
    void on_Reply_Finished(QNetworkReply *reply);
    QNetworkAccessManager *networkManager;
    QStringListModel* locResult_model = new QStringListModel();

private slots:
    void on_pushButton_locSearch_clicked();
};

#endif // FUNC_MAP_H
