#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private slots:
    void on_sendButton_clicked();
    void processPendingDatagrams();

private:
    // UI 组件的指针
    QLineEdit *lineEditMessage;
    QTextEdit *textEditChat;
    QPushButton *sendButton;
    QUdpSocket *udpSocket;
    quint16 port;  // 聊天使用的端口号
};

#endif // CHATWINDOW_H
