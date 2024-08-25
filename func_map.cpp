#include "func_map.h"
#include "ui_func_map.h"
#include <QtLocation/QGeoRoute>

func_map::func_map(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::func_map)
{
    ui->setupUi(this);

    QGeoRoute mainMap;
}

func_map::~func_map()
{
    delete ui;
}
