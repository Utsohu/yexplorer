#ifndef NEWDIRDIALOG_H
#define NEWDIRDIALOG_H

#include <QDialog>
#include "addfavoritedialog.h"
#include "ywindow.h"

namespace Ui {
class NewDirDialog;
}

class NewDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDirDialog(AddFavoriteDialog *parent = nullptr);
    ~NewDirDialog();
    QString SendDirName();
private slots:
    void OkClicked();
private:
    Ui::NewDirDialog *ui;
    AddFavoriteDialog* m_Object;
public:
    QString m_sNewDir;
};

#endif // NEWDIRDIALOG_H
