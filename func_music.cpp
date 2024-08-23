#include "func_music.h"
#include "ui_func_music.h"

func_Music::func_Music(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::func_Music)
{
    ui->setupUi(this);
}

func_Music::~func_Music()
{
    delete ui;
}
