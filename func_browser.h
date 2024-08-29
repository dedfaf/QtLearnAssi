#ifndef FUNC_BROWSER_H
#define FUNC_BROWSER_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWebEngineView>

class func_browser : public QWidget
{
    Q_OBJECT

public:
    explicit func_browser(QWidget *parent = nullptr);

private slots:
    void loadPage();

private:
    QLineEdit *searchBar;
    QWebEngineView *webView;
    QPushButton *searchButton;
};

#endif // FUNC_BROWSER_H
