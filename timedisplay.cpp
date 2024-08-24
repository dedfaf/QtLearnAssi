#include "timedisplay.h"
#include "ui_timedisplay.h"
#include <QDateTime>
#include <QTimer>

QString timeOutput()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}


timeDisplay::timeDisplay(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::timeDisplay)
{
    ui->setupUi(this);

    ui->lcdNumber_time->setDigitCount(19);
    ui->lcdNumber_time->setStyleSheet("border: 0px; background: silver;");
    ui->lcdNumber_time->display(timeOutput());

    // Timer to trigger update the lcdNumber
    QTimer *pTimer = new QTimer(this);
    pTimer->setInterval(1000);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(onTimeOutput()));

    pTimer->start();
}

timeDisplay::~timeDisplay()
{
    delete ui;
}

void timeDisplay::onTimeOutput()
{
    ui->lcdNumber_time->display(timeOutput());
}
