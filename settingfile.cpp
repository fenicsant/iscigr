#include "settingfile.h"
#include <QFile>
#include <QRegularExpression>
#include <QDebug>


SettingFile &SettingFile::inst()
{
    static SettingFile * res = SettingFile::newAndLoad();
    return *res;
}

bool SettingFile::refresh()
{
    QFile file("settings.conf");
    if (! file.open(QFile::ReadOnly)) {
       qDebug()<<"Error read settings file:"<<file.errorString();
       return false;
    }
    while (!file.atEnd()) {
        QString line = file.readLine();
        int pos = line.indexOf('=');
        QString name = line.left(pos).trimmed();
        QString value = (pos>0)?line.mid(pos + 1).trimmed():QString();

        operator [](name) = value;
        qDebug()<<" [sett]:"<<name<<"="<<value;
    }
    return 0;
}

SettingFile *SettingFile::newAndLoad()
{
    SettingFile * res = new SettingFile();
    if (res) res->refresh();
    return res;
}

SettingFile::SettingFile()
{

}

