#include "addfavoritedialog.h"
#include "ui_addfavoritedialog.h"
#include "newdirdialog.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

AddFavoriteDialog::AddFavoriteDialog(const QStringList& dirlist, const QString& webName, YWindow *parent) :
    QDialog(parent),
    ui(new Ui::AddFavoriteDialog)
{
    ui->setupUi(this);
    connect(ui->btnAdd,SIGNAL(clicked(bool)),this,SLOT(AddFavorite()));
    connect(ui->btnDelete,SIGNAL(clicked(bool)),this,SLOT(DeleteFavorite()));
    connect(ui->DirComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(SelectDir(int)));
    connect(ui->Btn_newDir,SIGNAL(clicked(bool)),this,SLOT(AddDir()));
    savedDir = dirlist;
    ui->DirComboBox->addItems(savedDir);
    ui->DirComboBox->addItem("标签栏");
    ui->DirComboBox->addItem("新建文件夹");
    ui->NameEdit->setText(webName);
    m_Object = parent;
}

AddFavoriteDialog::~AddFavoriteDialog()
{
    delete ui;
}

void AddFavoriteDialog::AddFavorite()
{
    if (ui->DirComboBox->currentText()=="新建文件夹")
    {
        AddNewDir();
        return;
    }
    m_sNewDirName = ui->DirComboBox->currentText();
    m_sNewFavoriteName = ui->NameEdit->text();
    accept();
}

void AddFavoriteDialog::DeleteFavorite()
{
    reject();
}

void AddFavoriteDialog::AddNewDir()
{
    NewDirDialog dlg(this);
    int decision = dlg.exec();
    if (decision == QDialog::Accepted && !savedDir.contains(m_sNewDirName))
    {
        savedDir.append(dlg.m_sNewDir);
        ui->DirComboBox->insertItem(0, dlg.m_sNewDir);
        m_Object->AddNewDir(dlg.m_sNewDir);
    }
}

void AddFavoriteDialog::AddDir()
{
    AddNewDir();
}
