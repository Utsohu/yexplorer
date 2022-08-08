#include "appinfo.h"
#include "ui_appinfo.h"

AppInfo::AppInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppInfo)
{
    ui->setupUi(this);
    connect(ui->OK,SIGNAL(clicked(bool)),this,SLOT(OK()));
}

AppInfo::~AppInfo()
{
    delete ui;
}

void AppInfo::OK()
{
    accept();
}
