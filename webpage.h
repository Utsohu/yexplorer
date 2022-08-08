#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWidget>
#include "ywindow.h"
#include "webcontent.h"
namespace Ui {
class WebPage;
}

class webpage : public QWidget
{
    Q_OBJECT
public:
    explicit webpage(QWidget *parent = nullptr);

signals:

private slots:
    void ChangeTitle(const QString& title);
    void SetAddress(const QUrl& url);
    void LoadWebFinished();
    void LoadIcon(const QIcon& icon);
private:
    Ui::WebPage *ui;
    YWindow* m_object;
    QString m_sAddress;
    QString m_sName;
    WebContent* m_View;
    QIcon currentIcon;

public:
    void OpenWeb(const QString& Web, YWindow* father);
    QString GetName();
    WebContent* OpenWeb(YWindow* father);
    QString GetAddress();
    QList<QString> ShowHistory();
    int m_nCurrentIndex;
    void LoadPage(QString address);
    void PreviousPage();
    void NextPage();
    QIcon getIcon();
    void ZoomIn();
    void ZoomOut();
    void InitialZoom();
public:
    QList<QString> m_plHistoryAddress;


};

#endif // WEBPAGE_H
