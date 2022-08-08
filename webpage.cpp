#include "webpage.h"
#include "ui_webpage.h"
#include <QtWebEngineWidgets/QWebEngineView>
#include "webcontent.h"

webpage::webpage(QWidget *parent) : QWidget(parent),
    ui(new Ui::WebPage)
{
    ui->setupUi(this);
    m_nCurrentIndex = 0;
}

void webpage::OpenWeb(const QString& Web, YWindow* father)
{
    WebContent* view = new WebContent(father, this);
    ui->contentGridLayout->addWidget(view);
    view->load(QUrl(Web));
    if(Web.endsWith("/"))
        m_sAddress = Web.section("/", 0, -2);
    else m_sAddress = Web;
    view->show();
    m_object = father;
    connect(view, SIGNAL(titleChanged(const QString&)),this, SLOT(ChangeTitle(const QString&)));
    connect(view, SIGNAL(loadFinished(bool)),this,SLOT(LoadWebFinished()));
    connect(view, SIGNAL(iconChanged(const QIcon&)),this,SLOT(LoadIcon(const QIcon&)));
    m_View = view;
}

void webpage::LoadWebFinished()
{
    m_object->AddHistory(m_sName);
    m_object->ShowFavoritePattern(m_sAddress);
    if (m_plHistoryAddress.contains(m_sAddress)){
        m_nCurrentIndex = m_plHistoryAddress.indexOf(m_sAddress);
    }
    else {
        m_plHistoryAddress.append(m_sAddress);
        m_nCurrentIndex = m_plHistoryAddress.size()-1;
    }
    m_object->ChangePageBtn();
    m_object->ChangeBarText(m_sAddress);
}

WebContent* webpage::OpenWeb(YWindow* father)
{
    WebContent* view = new WebContent(father, this);
    ui->contentGridLayout->addWidget(view);
    view->show();
    m_object = father;
    connect(view, SIGNAL(titleChanged(const QString&)),this, SLOT(ChangeTitle(const QString&)));
    connect(view, SIGNAL(urlChanged(const QUrl&)),this, SLOT(SetAddress(const QUrl&)));
    connect(view, SIGNAL(loadFinished(bool)),this,SLOT(LoadWebFinished()));
    connect(view, SIGNAL(iconChanged(const QIcon&)),this,SLOT(LoadIcon(const QIcon&)));
    m_View = view;
    return view;
}

void webpage::ChangeTitle(const QString& title)
{
    m_object->ChangeTabContent(title, this);
    m_sName = title;
}

QString webpage::GetAddress()
{
    return m_sAddress;
}

void webpage::SetAddress(const QUrl& url)
{
    m_sAddress = url.url();
}

QList<QString> webpage::ShowHistory()
{
    return m_plHistoryAddress;
}

void webpage::LoadPage(QString address)
{
    if(address.endsWith("/"))
        m_sAddress = address.section("/", 0, -2);
    else m_sAddress = address;
    QUrl target(address);
    m_View->load(target);
}

void webpage::NextPage()
{
    if (m_nCurrentIndex>=m_plHistoryAddress.size()-1) return;
    LoadPage(m_plHistoryAddress.at(++m_nCurrentIndex));
    m_object->ChangePageBtn();
}

void webpage::PreviousPage()
{
    if (m_nCurrentIndex<=0) return;
    LoadPage(m_plHistoryAddress.at(--m_nCurrentIndex));
    m_object->ChangePageBtn();
}

QString webpage::GetName()
{
    return m_sName;
}

void webpage::LoadIcon(const QIcon& icon)
{
    currentIcon = icon;
    m_object->ChangePageIcon(icon,this);
}

QIcon webpage::getIcon()
{
    return currentIcon;
}

void webpage::ZoomIn()
{
    qreal index = m_View->zoomFactor();
    if (index <= 4.5) index += 0.5;
    m_View->setZoomFactor(index);
}

void webpage::ZoomOut()
{
    qreal index = m_View->zoomFactor();
    if (index >= 0.75) index -= 0.5;
    m_View->setZoomFactor(index);
}

void webpage::InitialZoom()
{
    m_View->setZoomFactor(1.0);
}
