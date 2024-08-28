#include "chatwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QUdpSocket>
#include <QHostAddress>

ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    udpSocket(new QUdpSocket(this)),
    port(12345)  // 设置端口号为12345
{
    // 创建 UI 组件
    QVBoxLayout *layout = new QVBoxLayout(this);

    textEditChat = new QTextEdit(this);
    textEditChat->setReadOnly(true);  // 设置为只读
    layout->addWidget(textEditChat);

    lineEditMessage = new QLineEdit(this);
    layout->addWidget(lineEditMessage);

    sendButton = new QPushButton("Send", this);
    layout->addWidget(sendButton);

    // 绑定端口号，以便接收数据
    udpSocket->bind(QHostAddress::Any, port);

    // 连接信号槽，当有数据可读时调用 processPendingDatagrams()
    connect(udpSocket, &QUdpSocket::readyRead, this, &ChatWindow::processPendingDatagrams);
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::on_sendButton_clicked);
}

ChatWindow::~ChatWindow()
{
    delete udpSocket;
    delete sendButton;
    delete lineEditMessage;
    delete textEditChat;
}



void ChatWindow::on_sendButton_clicked()
{
    QString message = lineEditMessage->text();  // 使用 UI 组件
    QByteArray datagram = message.toUtf8();

    // 使用广播地址发送数据包
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, port);
    lineEditMessage->clear();
}


void ChatWindow::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        // 接收数据包
        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        // 将收到的消息显示在聊天窗口中
        QString message = tr("Received from %1: %2").arg(sender.toString()).arg(QString(datagram));
        textEditChat->append(message);  // 使用 UI 组件
    }
}
