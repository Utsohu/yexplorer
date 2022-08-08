#ifndef ADDFAVORITEDIALOG_H
#define ADDFAVORITEDIALOG_H

#include <QDialog>
#include "ywindow.h"


namespace Ui {
class AddFavoriteDialog;
}

class AddFavoriteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFavoriteDialog(const QStringList& dirlist, const QString& webName, YWindow *parent = nullptr);
    ~AddFavoriteDialog();
    void AddNewDir();

private slots:
    void AddFavorite();
    void DeleteFavorite();
    void AddDir();
public:
    QString m_sNewDirName;
    QString m_sNewFavoriteName;
private:
    Ui::AddFavoriteDialog *ui;
private:
    YWindow* m_Object;
    QStringList savedDir;
};

#endif // ADDFAVORITEDIALOG_H
