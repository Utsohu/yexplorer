#include "webcontent.h"
#include "ywindow.h"

WebContent::WebContent(YWindow* MainWindow,QWidget* parent):QWebEngineView(parent)
{
    m_pMainWindow = MainWindow;
}

QWebEngineView *WebContent::createWindow(QWebEnginePage::WebWindowType type)
{
    return m_pMainWindow->newPageContent();
}

