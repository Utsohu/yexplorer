#include "cfavorite.h"

CFavorite::CFavorite(const QString& name, QObject *parent) : QObject(parent)
{
    m_sName = name;
    m_pAction = 0;
}

void CFavorite::setIcon(QIcon icon)
{
    m_Icon = icon;
}

CFavoriteFile::CFavoriteFile(const QString& name, QObject *parent) : CFavorite(name,parent)
{

}

CFavoriteDir::CFavoriteDir(const QString& name, QObject *parent) : CFavorite(name, parent)
{

}

void CFavoriteDir::addNewFile(CFavoriteFile* file)
{
    m_plFavoriteFiles.append(file);
}


