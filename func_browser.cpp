#include "func_browser.h"

func_browser::func_browser(QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);  // 创建垂直布局

    webView = new QWebEngineView(this);  // 创建网页视图
    webView->load(QUrl("https://www.qt.io"));  // 加载默认网页
    layout->addWidget(webView);  // 将网页视图添加到布局中

    setLayout(layout);  // 设置窗口布局
    resize(1024, 768);  // 设置默认窗口大小
}
