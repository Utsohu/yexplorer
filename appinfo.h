#ifndef APPINFO_H
#define APPINFO_H

#include <QDialog>

namespace Ui {
class AppInfo;
}

class AppInfo : public QDialog
{
    Q_OBJECT

public:
    explicit AppInfo(QWidget *parent = nullptr);
    ~AppInfo();
private slots:
    void OK();
private:
    Ui::AppInfo *ui;
};

#endif // APPINFO_H
