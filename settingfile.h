#ifndef SETTINGFILE_H
#define SETTINGFILE_H

#include <QString>
#include <QMap>

class SettingFile : public QMap<QString, QString>
{
public:
    static SettingFile & inst();
    bool refresh();
private:
    static SettingFile * newAndLoad();
    SettingFile();
};

inline SettingFile & settingFile()
    { return SettingFile::inst(); }

#endif // SETTINGFILE_H
