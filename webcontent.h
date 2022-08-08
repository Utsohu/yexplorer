#ifndef WEBCONTENT_H
#define WEBCONTENT_H
#include <QWebEngineView>

class YWindow;
class WebContent : public QWebEngineView
{
public:
    WebContent(YWindow* MainWindow, QWidget* parent = Q_NULLPTR);
protected:
    virtual QWebEngineView* createWindow(QWebEnginePage::WebWindowType type);
private:
    YWindow* m_pMainWindow;
};

#endif // WEBCONTENT_H
