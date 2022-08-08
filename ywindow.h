#ifndef YWINDOW_H
#define YWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "webcontent.h"
#include "cfavorite.h"

namespace Ui {
class YWindow;
}
typedef struct favoriteAddress{
    QString Address;
    QString Name;
    QDateTime SaveTime;
    QAction* Action;
}tagFavoriteAddress;

class webpage;

class YWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit YWindow(QWidget *parent = nullptr);
    ~YWindow();

private slots:
    void on_action_exit_triggered();
    void GoClicked(bool bclick);
    void RefleshClicked(bool bclick);
    void RemoveTab(int index);
    void FavoriteClicked(bool bclick);
    void ShowFavorite();
    void showTempFavorite();
    void ShowHistory();
    void AddPage(int index);
    void ChangeCurrentWidget(int index);
    void ForwardClicked();
    void BackwardClicked();
    void ShowFavoriteList();
    void ShowFavoriteDirList();
    void ShowFavoriteFile();
    void ShowFavoriteIndividualFile();
    void ShowFunctionList();
    void ShowAllFavorite();
    void ShowAllHistory();
    void ZoomIn();
    void ZoomOut();
    void InitialZoom();
    void ShowInfo();
private:
    Ui::YWindow *ui;
private:
    void NewPage(const QString& Address);
//    void ReadFavoriteSaved();
//    void WriteFavoriteSaved();
    void ReadHistorySaved();
    void WriteHistorySaved();
    void initialSavedFavorite();
    int CountSavedFiles();
    void ReadDirSaved();
    void WriteDirSaved();
    void DeleteFavoriteSaved();
public:
    void RemoveTab();
    void ChangeTabContent(const QString& title, webpage* changePage);
    WebContent*  newPageContent();
    void AddHistory(QString title);
    void ShowFavoritePattern(QString address);
    void ChangePageBtn();
    void ChangeBarText(const QString& address);
    void AddNewDir(const QString& dir);
    void ChangePageIcon(const QIcon& icon, webpage* changePage);

protected:
    virtual void keyPressEvent(QKeyEvent* ev);
private:
//    QList<tagFavoriteAddress*> m_plFavoriteAddress;
    QList<tagFavoriteAddress*> m_plHistoryAddress;
    QList<tagFavoriteAddress*> m_plTempFavoriteAddress;
    QString saveAddress;
    QString DirSavedAddress;
public:
    QMap<QString, CFavoriteDir*>   m_plFavorite;
    QMap<QString, CFavoriteFile*> m_plFavoriteFiles;
    QString NewDirName;
    QString NewFavoriteName;
};

#endif // YWINDOW_H
