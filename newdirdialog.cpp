#include "newdirdialog.h"
#include "ui_newdirdialog.h"
#include "addfavoritedialog.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

NewDirDialog::NewDirDialog(AddFavoriteDialog *parent) :
    QDialog(parent),
    ui(new Ui::NewDirDialog)
{
    ui->setupUi(this);
    ui->NewDirNameEdit->setText("文件夹");
    m_Object = parent;
    connect(ui->AdmitDirName,SIGNAL(clicked(bool)),this,SLOT(OkClicked()));
}

NewDirDialog::~NewDirDialog()
{
    delete ui;
}

void NewDirDialog::OkClicked()
{
    m_sNewDir = ui->NewDirNameEdit->text();
    accept();
}

QString NewDirDialog::SendDirName()
{
    return ui->NewDirNameEdit->text();
}
