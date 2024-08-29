#include "func_browser.h"

func_browser::func_browser(QWidget *parent)
    : QWidget(parent),
      searchBar(new QLineEdit(this)),
      webView(new QWebEngineView(this)),
      searchButton(new QPushButton("Search", this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create a horizontal layout for search bar and button
    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(searchBar);
    searchLayout->addWidget(searchButton);

    // Add the search layout and web view to the main layout
    layout->addLayout(searchLayout);
    layout->addWidget(webView);

    // Set size policy to expand and fill space
    webView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(searchButton, &QPushButton::clicked, this, &func_browser::loadPage);

    // Set a default URL or home page
    webView->setUrl(QUrl("https://www.google.com"));
}

void func_browser::loadPage()
{
    QString url = searchBar->text();
    if (!url.startsWith("http://") && !url.startsWith("https://")) {
        url.prepend("http://");
    }
    webView->setUrl(QUrl(url));
}
