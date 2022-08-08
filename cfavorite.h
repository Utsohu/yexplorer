#ifndef CFAVORITE_H
#define CFAVORITE_H

#include <QObject>
#include <QAction>
#include <QPushButton>

class CFavorite : public QObject
{
public:
    explicit CFavorite(const QString& name, QObject *parent = nullptr);
public:
    QString m_sName;
    QAction* m_pAction;
    QIcon m_Icon;
    void setIcon(QIcon icon);
    bool isDir;
    QString m_sDirName;
};

class CFavoriteFile : public CFavorite
{
public:
    explicit CFavoriteFile(const QString& name, QObject *parent = nullptr);
public:
    QString m_sAddress;

};

class CFavoriteDir : public CFavorite
{
public:
    explicit CFavoriteDir(const QString& name, QObject *parent = nullptr);
public:
    QList<CFavoriteFile*> m_plFavoriteFiles;
    void addNewFile(CFavoriteFile* file);
};

#endif // CFAVORITE_H
