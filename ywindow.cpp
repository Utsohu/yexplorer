#include "ywindow.h"
#include "ui_ywindow.h"
#include <QMessageBox>
#include "webpage.h"
#include <QTabBar>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QToolButton>
#include "addfavoritedialog.h"
#include "appinfo.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

YWindow::YWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YWindow)
{
    ui->setupUi(this);
    connect(ui->btnGO, SIGNAL(clicked(bool)), this, SLOT(GoClicked(bool)));
    connect(ui->btnRefresh, SIGNAL(clicked(bool)), this, SLOT(RefleshClicked(bool)));
    QString initialAddress = "https://www.baidu.com";
    ui->objectAddress->setText(initialAddress);
    ui->tabWidget->removeTab(0);
    QTabBar* bar = ui->tabWidget->tabBar();
    bar->setTabsClosable(true);
    bar->setTabButton(0,QTabBar::RightSide, nullptr);
    connect(bar, SIGNAL(tabCloseRequested(int)),this,SLOT(RemoveTab(int)));
    connect(ui->btnFavorite, SIGNAL(clicked(bool)),this,SLOT(FavoriteClicked(bool)));
    connect(ui->tabWidget,SIGNAL(tabBarClicked(int)),this,SLOT(AddPage(int)));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(ChangeCurrentWidget(int)));
//    ReadFavoriteSaved();
    ReadHistorySaved();
    ReadDirSaved();
    NewPage(initialAddress);
    connect(ui->btnForward,SIGNAL(clicked(bool)),this,SLOT(ForwardClicked()));
    connect(ui->btnBackward,SIGNAL(clicked(bool)),this,SLOT(BackwardClicked()));

    connect(ui->FavoriteList,SIGNAL(clicked(bool)),this,SLOT(ShowFunctionList()));
    initialSavedFavorite();
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
    ui->menuBar->hide();
}

YWindow::~YWindow()
{
//   WriteFavoriteSaved();
   WriteHistorySaved();
   WriteDirSaved();
   delete ui;
/*   foreach (tagFavoriteAddress* i , m_plFavoriteAddress){
        delete i;
   }
   foreach (tagFavoriteAddress* i , m_plHistoryAddress){
       delete i;
   }
*/
}

void YWindow::GoClicked(bool bclick)
{
    Q_UNUSED(bclick);
    QString Address = ui->objectAddress->text();
    if (Address == "") return;
    if (!Address.startsWith("https://")){
        Address.push_front("https://");
    }
    int index = ui->tabWidget->currentIndex();
    QWidget* ReplacedWidget = ui->tabWidget->widget(index);
    if (!ReplacedWidget->inherits("webpage")) return;
    webpage* target = (webpage*) ReplacedWidget;
    target->LoadPage(Address);
}

void YWindow::RefleshClicked(bool bclick)
{
        webpage* CurrentWidget = (webpage*) ui->tabWidget->currentWidget();
        QString Address = CurrentWidget->GetAddress();
        CurrentWidget->LoadPage(Address);
}

void YWindow::on_action_exit_triggered()
{
    int ret = QMessageBox::information(this, "提示", "你是否确认退出呢！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(ret == QMessageBox::Yes){
        qApp->quit();
    }
}

void YWindow::NewPage(const QString& Address)
{
    webpage* page = new webpage(ui->tabWidget);
    int sz = ui->tabWidget->count();
    ui->tabWidget->insertTab(sz-1, page, "new tab");
    ui->tabWidget->setCurrentWidget(page);
    page->OpenWeb(Address, this);
}

WebContent*  YWindow::newPageContent()
{
    webpage* page = new webpage(ui->tabWidget);
    int sz = ui->tabWidget->count();
    ui->tabWidget->insertTab(sz-1, page, "new tab");
    ui->tabWidget->setCurrentWidget(page);
    return page->OpenWeb(this);
}

void YWindow::RemoveTab(int index){
    if (ui->tabWidget->count()==2){
        qApp->quit();
    }
    webpage* Widget = (webpage* )ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    delete Widget;
}

void YWindow::ChangeTabContent(const QString& title, webpage* changePage)
{
    int index = ui->tabWidget->indexOf(changePage);
    ui->tabWidget->setTabText(index, title);
    if (title.size()>8){
        QString name = title.left(8)+"...";
        ui->tabWidget->setTabText(index, name);
    }
}

void YWindow::AddNewDir(const QString& dir)
{
    CFavoriteDir* pDir = new CFavoriteDir(dir);
    QPixmap convertToQP;
    convertToQP.load(":/image/img/png-0033.png");
    QIcon icon(convertToQP);
    pDir->setIcon(icon);
    QAction* BtnAction = new QAction(dir);
    BtnAction->setIcon(icon);
    BtnAction->setIconText(dir);
    BtnAction->setIconVisibleInMenu(true);
    connect(BtnAction,SIGNAL(triggered(bool)),this,SLOT(ShowFavoriteDirList()));
    ui->mainToolBar->addAction(BtnAction);
    pDir->m_pAction = BtnAction;
    m_plFavorite[dir] = pDir;
    QMenu* pMenu = ui->menuFavoriteDir->addMenu(dir);
}

void YWindow::FavoriteClicked(bool)
{
    QStringList list = m_plFavorite.keys();
    webpage* currentPage = (webpage*)ui->tabWidget->currentWidget();
    QString webName = currentPage->GetName();
    AddFavoriteDialog dlg(list, webName, this);
    if (dlg.exec() != QDialog::Accepted){
        DeleteFavoriteSaved();
        return;
    }
    if (dlg.m_sNewDirName == "标签栏"){
        CFavoriteFile* targetFile = new CFavoriteFile(dlg.m_sNewFavoriteName);
        webpage* currentPage = (webpage*)ui->tabWidget->currentWidget();
        targetFile->m_sAddress = currentPage->GetAddress();
        targetFile->m_pAction = new QAction(targetFile->m_sName);
        targetFile->m_Icon = currentPage->getIcon();
        targetFile->m_pAction->setIcon(targetFile->m_Icon);
        m_plFavoriteFiles[targetFile->m_sName] = targetFile;
        connect(targetFile->m_pAction,SIGNAL(triggered(bool)),this,SLOT(ShowFavoriteIndividualFile()));
        ui->mainToolBar->addAction(targetFile->m_pAction);
        ShowFavoritePattern(targetFile->m_sAddress);
        return;
    }

    CFavoriteDir* targetDir = m_plFavorite.value(dlg.m_sNewDirName);
    if(targetDir != 0) {
        CFavoriteFile* targetFile = new CFavoriteFile(dlg.m_sNewFavoriteName);
        webpage* currentPage = (webpage*)ui->tabWidget->currentWidget();
        targetFile->m_sAddress = currentPage->GetAddress();
        targetFile->m_pAction = new QAction(targetFile->m_sName);
        targetFile->m_pAction->setIconVisibleInMenu(true);
        targetFile->m_Icon = currentPage->getIcon();
        targetFile->m_pAction->setIcon(targetFile->m_Icon);
        connect(targetFile->m_pAction,SIGNAL(triggered(bool)),this,SLOT(ShowFavoriteFile()));
        targetDir->addNewFile(targetFile);
        ui->menuFavoriteDir->clear();
        initialSavedFavorite();
        ShowFavoritePattern(targetFile->m_sAddress);
    }
/*
    return;
    tagFavoriteAddress* addAddress = new tagFavoriteAddress;
    int index = ui->tabWidget->currentIndex();
    addAddress->Name = ui->tabWidget->tabText(index);
    addAddress->Address =((webpage*) ui->tabWidget->widget(index))->GetAddress();
    addAddress->SaveTime = QDateTime::currentDateTime();
    addAddress->Action = new QAction(addAddress->Name + addAddress->SaveTime.toString("yyyy/MM/dd hh:mm"));
    for (int i = 0; i<m_plFavoriteAddress.size();i++){
        if (m_plFavoriteAddress.at(i)->Address == addAddress->Address){
            ui->menuFavorite->removeAction(m_plFavoriteAddress.at(i)->Action);
            m_plFavoriteAddress.removeAt(i);
            ShowFavoritePattern(addAddress->Address);
            delete addAddress->Action;
            delete addAddress;
            return;
        }
    }
    for (int i = 0; i<m_plTempFavoriteAddress.size();i++){
        if (m_plTempFavoriteAddress.at(i)->Address == addAddress->Address){
            ui->menuFavorite->removeAction(m_plTempFavoriteAddress.at(i)->Action);
            m_plTempFavoriteAddress.removeAt(i);
            ShowFavoritePattern(addAddress->Address);
            delete addAddress->Action;
            delete addAddress;
            return;
        }
    }
    m_plTempFavoriteAddress.append(addAddress);
    m_plFavoriteAddress.append(addAddress);
    ui->menuFavorite->addAction(addAddress->Action);
    ui->menuTempFavorite->addAction(addAddress->Action);
    connect(addAddress->Action, SIGNAL(triggered(bool)), this, SLOT(ShowFavorite()));
    ShowFavoritePattern(addAddress->Address);
*/
}

void YWindow::ShowFavorite()
{
    QObject* obj = sender();
    QAction* action = qobject_cast<QAction*>(obj);
    if (action == 0){
        return;
    }
/*
    for (int i = 0; i<m_plFavoriteAddress.size();i++){
        tagFavoriteAddress* object = m_plFavoriteAddress.at(i);
        if (object->Action == action){
            NewPage(object->Address);
            break;
        }
    }
*/
}

void YWindow::AddHistory(QString title)
{
    tagFavoriteAddress* addAddress = new tagFavoriteAddress;
    int index = ui->tabWidget->currentIndex();
    addAddress->Name = ui->tabWidget->tabText(index);
    addAddress->Address =((webpage*) ui->tabWidget->widget(index))->GetAddress();
    addAddress->SaveTime = QDateTime::currentDateTime();
    addAddress->Action = new QAction(title + addAddress->SaveTime.toString("yyyy/MM/dd hh:mm"));
    m_plHistoryAddress.append(addAddress);
    ui->menuHistory->addAction(addAddress->Action);
    connect(addAddress->Action, SIGNAL(triggered(bool)), this, SLOT(ShowHistory()));
    if (m_plHistoryAddress.size()>5){
        ui->menuHistory->removeAction(m_plHistoryAddress.at(m_plHistoryAddress.size()-6)->Action);
    }
}

void YWindow::ShowHistory()
{
    QObject* obj = sender();
    QAction* action = qobject_cast<QAction*>(obj);
    if (action == 0){
        return;
    }
    for (int i = 0; i<m_plHistoryAddress.size();i++){
        tagFavoriteAddress* object = m_plHistoryAddress.at(i);
        if (object->Action == action){
            NewPage(object->Address);
            break;
        }
    }
}
/*
void YWindow::ReadFavoriteSaved()
{
    QFile fileAddress("./favoriteAddress.txt");
    if (fileAddress.open(QIODevice::ReadOnly)){
        QDataStream ts(&fileAddress);
        int size;
        ts >> size;
        for (int index = 0;index<size;index++){
            tagFavoriteAddress* object = new tagFavoriteAddress;
            ts >> object->Address >> object->Name >> object->SaveTime;
            object->Action = new QAction(object->Name + object->SaveTime.toString("yyyy/MM/dd hh:mm"));
            m_plFavoriteAddress.append(object);
            ui->menuFavorite->addAction(object->Action);
            connect(object->Action, SIGNAL(triggered(bool)), this, SLOT(ShowFavorite()));
        }
        fileAddress.close();
    }

}

void YWindow::WriteFavoriteSaved()
{
    QFile fileAddress("./favoriteAddress.txt");
    if(fileAddress.open(QIODevice::WriteOnly)){
        QDataStream ts(&fileAddress);
        ts << m_plFavoriteAddress.size();
        foreach (tagFavoriteAddress* object , m_plFavoriteAddress){
            ts << object->Address << object->Name << object->SaveTime;
        }
        fileAddress.close();
    }
}
*/
void YWindow::ReadHistorySaved()
{
    QFile fileAddress("./historyAddress.txt");
    if (fileAddress.open(QIODevice::ReadOnly)){
        QDataStream ts(&fileAddress);
        int size;
        ts >> size;
        for (int index = 0;index<size;index++){
            tagFavoriteAddress* object = new tagFavoriteAddress;
            ts >> object->Address >> object->Name >> object->SaveTime;
            object->Action = new QAction(object->Name + object->SaveTime.toString("yyyy/MM/dd hh:mm"));
            m_plHistoryAddress.append(object);
            if (size-index<=5){
                ui->menuHistory->addAction(object->Action);
                connect(object->Action, SIGNAL(triggered(bool)), this, SLOT(ShowHistory()));
            }
        }
        fileAddress.close();
     }
}

void YWindow::WriteHistorySaved()
{
    QFile fileAddress("./historyAddress.txt");
    if(fileAddress.open(QIODevice::WriteOnly)){
        QDataStream ts(&fileAddress);
        ts << m_plHistoryAddress.size();
        foreach (tagFavoriteAddress* object , m_plHistoryAddress){
            ts << object->Address << object->Name << object->SaveTime;
        }
        fileAddress.close();
    }

}

void YWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Return){
        GoClicked(true);
        return;
    }
}

void YWindow::AddPage(int index)
{
    int sz = ui->tabWidget->count();
    if (index == sz-1)
    {
        QString initialAddress = "https://www.baidu.com";
        webpage* page = new webpage(ui->tabWidget);
        ui->tabWidget->insertTab(sz-1, page, "new tab");
        ui->tabWidget->setCurrentWidget(page);
        page->OpenWeb(initialAddress, this);
    }
}

void YWindow::ShowFavoritePattern(QString address)
{
    QString fileAddress = ":/image/img/png-1663.png";
    for (CFavoriteDir* targetDir : m_plFavorite){
        for (CFavoriteFile* targetFile : targetDir->m_plFavoriteFiles){
            if (targetFile->m_sAddress == address){
                fileAddress = ":/image/img/png-1662.png";
            }
        }
    }
    for (CFavoriteFile* targetFile : m_plFavoriteFiles){
        if (targetFile->m_sAddress == address){
            fileAddress = ":/image/img/png-1662.png";
        }
    }
    QPixmap convertToQP;
    convertToQP.load(fileAddress);
    QIcon icon(convertToQP);
    ui->btnFavorite->setIcon(icon);
/*
    QString fileAddress = ":/image/img/png-1663.png";
    for (int index=0;index<m_plFavoriteAddress.size();index++){
        if (m_plFavoriteAddress.at(index)->Address==address){
            fileAddress = ":/image/img/png-1662.png";
        }
    }
    QPixmap convertToQP;
    convertToQP.load(fileAddress);
    QIcon icon(convertToQP);
    ui->btnFavorite->setIcon(icon);
*/
}

void YWindow::ChangeCurrentWidget(int index)
{
    QWidget* target = ui->tabWidget->widget(index);
    webpage* wb = qobject_cast<webpage*>(target);
    if (!wb){
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-2);
        return;
    }
    QString address = wb->GetAddress();
    ShowFavoritePattern(address);
    ChangePageBtn();
}

void YWindow::ForwardClicked()
{
    webpage* page = (webpage*)ui->tabWidget->currentWidget();
    page->NextPage();
}

void YWindow::BackwardClicked()
{
    webpage* page = (webpage*)ui->tabWidget->currentWidget();
    page->PreviousPage();
}

void YWindow::ChangePageBtn()
{
    webpage* page = (webpage*)ui->tabWidget->currentWidget();
    QString fileAddressForward = ":/image/img/png-1679.png";
    ui->btnForward->setEnabled(false);
    if (page->m_nCurrentIndex<page->m_plHistoryAddress.size()-1){
        fileAddressForward = ":/image/img/png-1677.png";
        ui->btnForward->setEnabled(true);
    }
    QPixmap convertToQP;
    convertToQP.load(fileAddressForward);
    QIcon iconForward(convertToQP);
    ui->btnForward->setIcon(iconForward);

    QString fileAddressBackward = ":/image/img/png-1639.png";
    ui->btnBackward->setEnabled(false);
    if (page->m_nCurrentIndex>0){
        fileAddressBackward = ":/image/img/png-1638.png";
        ui->btnBackward->setEnabled(true);
    }
    convertToQP.load(fileAddressBackward);
    QIcon iconBackward(convertToQP);
    ui->btnBackward->setIcon(iconBackward);
    ChangeBarText(((webpage*) ui->tabWidget->currentWidget())->GetAddress());
}

void YWindow::ChangeBarText(const QString& address)
{
    ui->objectAddress->setText(address);
}

void YWindow::ShowFavoriteList()
{
    QFile fileSaveAddress(saveAddress);
    if(fileSaveAddress.open(QIODevice::WriteOnly)){
        QDataStream ts(&fileSaveAddress);
        ts << m_plTempFavoriteAddress.size();
        foreach (tagFavoriteAddress* object , m_plTempFavoriteAddress){
            ts << object->Address << object->Name << object->SaveTime;
        }
        fileSaveAddress.close();
    }
    ui->menuTempFavorite->clear();
    QString dlg = QFileDialog::getOpenFileName(NULL,"请选择文件");
    saveAddress = dlg;
    QFile fileAddress(dlg);
    if (fileAddress.open(QIODevice::ReadOnly)){
        QDataStream ts(&fileAddress);
        int size;
        ts >> size;
        for (int index = 0;index<size;index++){
            tagFavoriteAddress* object = new tagFavoriteAddress;
            ts >> object->Address >> object->Name >> object->SaveTime;
            object->Action = new QAction(object->Name + object->SaveTime.toString("yyyy/MM/dd hh:mm"));
            m_plTempFavoriteAddress.append(object);
            ui->menuTempFavorite->addAction(object->Action);
            connect(object->Action, SIGNAL(triggered(bool)), this, SLOT(showTempFavorite()));
        }
        fileAddress.close();
    }
}

void YWindow::showTempFavorite()
{
    QObject* obj = sender();
    QAction* action = qobject_cast<QAction*>(obj);
    if (action == 0){
        return;
    }
    for (int i = 0; i<m_plTempFavoriteAddress.size();i++){
        tagFavoriteAddress* object = m_plTempFavoriteAddress.at(i);
        if (object->Action == action){
            NewPage(object->Address);
            break;
        }
    }
}

void YWindow::initialSavedFavorite()
{
    for (QString DirName : m_plFavorite.keys())
    {
        ui->menuFavoriteDir->addMenu(DirName);
    }
}

void YWindow::ShowFavoriteDirList()
{
    QObject* obj = sender();
    QAction* btn = qobject_cast<QAction*>(obj);
    QString name = btn->text();
    CFavoriteDir* targetDir = m_plFavorite[name];
    QMenu* pMenu = new QMenu();
    for (CFavoriteFile* targetFile : targetDir->m_plFavoriteFiles)
    {
          pMenu->addAction(targetFile->m_pAction);
    }
    pMenu->exec(cursor().pos());
    delete pMenu;
}

void YWindow::ShowFavoriteFile()
{
    QObject* obj = sender();
    QAction* btn = qobject_cast<QAction*>(obj);
    for (CFavoriteDir* targetDir: m_plFavorite){
        for (CFavoriteFile* targetFile : targetDir->m_plFavoriteFiles){
            if (targetFile->m_sName == btn->text()){
                QString Address = targetFile->m_sAddress;
                if (Address == "") return;
                if (!Address.startsWith("https://")){
                    Address.push_front("https://");
                }
                NewPage(Address);
                return;
            }
        }
    }
}

void YWindow::WriteDirSaved()
{
    DirSavedAddress = "./DirFavorite.txt";
    QFile fileSaveAddress(DirSavedAddress);
    if(fileSaveAddress.open(QIODevice::WriteOnly)){
        QDataStream ts(&fileSaveAddress);
        ts << CountSavedFiles();
        for (QString key : m_plFavorite.keys())
        {
            CFavoriteDir* targetDir = m_plFavorite[key];
            for (CFavoriteFile* targetFile : targetDir->m_plFavoriteFiles)
            {
                ts << targetFile->m_sName << key << targetFile->m_sAddress << targetFile->m_Icon << true;
            }
        }
        for (QString key : m_plFavoriteFiles.keys()){
            CFavoriteFile* targetFile = m_plFavoriteFiles[key];
            ts << targetFile->m_sName << "标签栏" << targetFile->m_sAddress << targetFile->m_Icon << false;
        }
        fileSaveAddress.close();
    }
}

void YWindow::ReadDirSaved()
{
    DirSavedAddress = "./DirFavorite.txt";
    QFile fileAddress(DirSavedAddress);
    if (fileAddress.open(QIODevice::ReadOnly)){
        QDataStream ts(&fileAddress);
        int size;
        ts >> size;
        for (int index = 0;index<size;index++){
            CFavoriteFile* targetFile = new CFavoriteFile("");
            ts >> targetFile->m_sName >> targetFile->m_sDirName >> targetFile->m_sAddress >> targetFile->m_Icon >> targetFile->isDir;
            targetFile->m_pAction = new QAction(targetFile->m_sName);
            targetFile->m_pAction->setIcon(targetFile->m_Icon);
            if (targetFile->isDir){
                if (m_plFavorite.contains(targetFile->m_sDirName)){
                    m_plFavorite[targetFile->m_sDirName]->addNewFile(targetFile);
                    connect(targetFile->m_pAction,SIGNAL(triggered(bool)),this,SLOT(ShowFavoriteFile()));
                }
                else{
                    AddNewDir(targetFile->m_sDirName);
                    m_plFavorite[targetFile->m_sDirName]->addNewFile(targetFile);
                    connect(targetFile->m_pAction,SIGNAL(triggered(bool)),this,SLOT(ShowFavoriteFile()));
                }
            }
            else{
                m_plFavoriteFiles[targetFile->m_sName] = targetFile;
                connect(targetFile->m_pAction,SIGNAL(triggered(bool)),this,SLOT(ShowFavoriteIndividualFile()));
                ui->mainToolBar->addAction(targetFile->m_pAction);
            }
        }
        fileAddress.close();
    }
}

int YWindow::CountSavedFiles()
{
    int count = 0;
    for (QString key : m_plFavorite.keys()){
        count += m_plFavorite[key]->m_plFavoriteFiles.size();
    }
    count += m_plFavoriteFiles.size();
    return count;
}

void YWindow::ShowFavoriteIndividualFile()
{
    QObject* obj = sender();
    QAction* btn = qobject_cast<QAction*>(obj);
    for (CFavoriteFile* targetFile : m_plFavoriteFiles){
        if (targetFile->m_sName == btn->text()){
            QString Address = targetFile->m_sAddress;
            if (Address == "") return;
            if (!Address.startsWith("https://")){
                Address.push_front("https://");
            }
            NewPage(Address);
            return;
        }
    }
}

void YWindow::ChangePageIcon(const QIcon& icon, webpage* changePage)
{
    int index = ui->tabWidget->indexOf(changePage);
    ui->tabWidget->setTabIcon(index,icon);
}

void YWindow::DeleteFavoriteSaved()
{
    webpage* currentPage = (webpage*)ui->tabWidget->currentWidget();
    QString pageAddress = currentPage->GetAddress();
    for (QString key : m_plFavorite.keys()){
        for (int index = 0;index<m_plFavorite[key]->m_plFavoriteFiles.count();index++){
            CFavoriteFile* targetFile = m_plFavorite[key]->m_plFavoriteFiles.at(index);
            if (targetFile->m_sAddress == pageAddress){
                m_plFavorite[key]->m_plFavoriteFiles.removeAt(index);
                index--;
            }
        }
    }
    for (CFavoriteFile* targetFile : m_plFavoriteFiles){
        if (targetFile->m_sAddress == pageAddress){
            m_plFavoriteFiles.remove(targetFile->m_sName);
            ui->mainToolBar->removeAction(targetFile->m_pAction);
            return;
        }
    }
    for (int index = 0;index<m_plFavorite.count();index++){
        QString targetKey = m_plFavorite.keys().at(index);
        CFavoriteDir* targetDir = m_plFavorite[targetKey];
        if (targetDir->m_plFavoriteFiles.count() == 0){
            m_plFavorite.remove(targetDir->m_sName);
            ui->mainToolBar->removeAction(targetDir->m_pAction);
            index--;
        }
    }
}

void YWindow::ShowFunctionList()
{
    QMenu* pMenu = new QMenu();

    QAction* FavoriteList = new QAction("收藏");
    connect(FavoriteList,SIGNAL(triggered(bool)),this,SLOT(ShowAllFavorite()));
    pMenu->addAction(FavoriteList);

    QAction* HistoryList = new QAction("历史");
    connect(HistoryList,SIGNAL(triggered(bool)),this,SLOT(ShowAllHistory()));
    pMenu->addAction(HistoryList);

    pMenu->addSeparator();

    QAction* ZoomIn = new QAction("放大");
    connect(ZoomIn,SIGNAL(triggered(bool)),this,SLOT(ZoomIn()));
    pMenu->addAction(ZoomIn);

    QAction* ZoomOut = new QAction("缩小");
    connect(ZoomOut,SIGNAL(triggered(bool)),this,SLOT(ZoomOut()));
    pMenu->addAction(ZoomOut);

    QAction* InitialZoom = new QAction("还原");
    connect(InitialZoom,SIGNAL(triggered(bool)),this,SLOT(InitialZoom()));
    pMenu->addAction(InitialZoom);

    pMenu->addSeparator();

    QAction* ShowInfo = new QAction("版本信息");
    connect(ShowInfo,SIGNAL(triggered(bool)),this,SLOT(ShowInfo()));
    pMenu->addAction(ShowInfo);
    pMenu->exec(cursor().pos());
}

void YWindow::ShowAllFavorite()
{
    QMenu* pMenu = new QMenu();
    for (CFavoriteDir* targetDir : m_plFavorite){
        for (CFavoriteFile* targetFile : targetDir->m_plFavoriteFiles){
            pMenu->addAction(targetFile->m_pAction);
        }
    }
    pMenu->exec(cursor().pos());
}

void YWindow::ShowAllHistory()
{
    QMenu* pMenu = new QMenu();
    for (int index = m_plHistoryAddress.count()-1;index>=0;index--){
        if (m_plHistoryAddress.count()-index>10) break;
        tagFavoriteAddress* target = m_plHistoryAddress.at(index);
        target->Action->setText(target->Name + "  " + target->SaveTime.toString());
        pMenu->addAction(target->Action);
    }
    pMenu->exec(cursor().pos());
}

void YWindow::ZoomIn()
{
    webpage* page = (webpage*)ui->tabWidget->currentWidget();
    page->ZoomIn();
}

void YWindow::ZoomOut()
{
    webpage* page = (webpage*)ui->tabWidget->currentWidget();
    page->ZoomOut();
}

void YWindow::InitialZoom()
{
    webpage* page = (webpage*)ui->tabWidget->currentWidget();
    page->InitialZoom();
}

void YWindow::ShowInfo()
{
    AppInfo* info = new AppInfo();
    info->exec();
}
