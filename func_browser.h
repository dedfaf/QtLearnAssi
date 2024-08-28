#ifndef FUNC_BROWSER_H
#define FUNC_BROWSER_H

#include <QWidget>
#include <QWebEngineView>
#include <QVBoxLayout>

class func_browser : public QWidget {
    Q_OBJECT

public:
    explicit func_browser(QWidget *parent = nullptr);

private:
    QWebEngineView *webView;  // 网页视图
};

#endif // FUNC_BROWSER_H
