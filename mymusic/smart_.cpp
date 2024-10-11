#include "smart_.h"
#include "ui_smart_.h"

smart_::smart_(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::smart_)
{
    ui->setupUi(this);
}

smart_::~smart_()
{
    delete ui;
}
