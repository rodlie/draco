/*
# Copyright (c) 2018, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
*/

#ifndef KEYBOARD_COMMON_H
#define KEYBOARD_COMMON_H

#include <QStringList>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QProcess>
#include <QSettings>
#include <QDebug>

#include "draco.h"

#define SETXKBMAP "setxkbmap"

enum xkbType
{
    xkbModel,
    xkbLayout,
    xkbVariant
};

class KeyboardCommon
{
public:
    static QStringList parseXKB(xkbType type)
    {
        QStringList result;
        QString findType;
        switch(type) {
        case xkbLayout:
            findType = "! layout";
            break;
        case xkbModel:
            findType = "! model";
            break;
        case xkbVariant:
            findType = "! variant";
            break;
        default:;
        }
        if (findType.isEmpty()) { return result; }

        QString xkbRules = QString("%1/../share/X11/xkb/rules/xfree86.lst").arg(QCoreApplication::applicationDirPath());
        QFileInfo xkbRulesFile;
        xkbRulesFile.setFile(xkbRules);
        if (!xkbRulesFile.exists()) {
            QStringList fallback;
            fallback << "/usr/share/X11/xkb/rules/xfree86.lst" << "/usr/local/share/X11/xkb/rules/xfree86.lst";
            fallback << "/usr/pkg/X11/xkb/rules/xfree86.lst" << "/usr/X11R7/share/X11/xkb/rules/xfree86.lst";
            fallback << "/usr/X11R6/share/X11/xkb/rules/xfree86.lst";
            for (int i=0;i<fallback.size();++i) {
                xkbRulesFile.setFile(fallback.at(i));
                if (!xkbRulesFile.exists()) { continue; }
                if (xkbRulesFile.exists()) {
                    qDebug() << "using" << xkbRulesFile.absoluteFilePath();
                    break;
                }
            }
            if (!xkbRulesFile.exists()) {
                qDebug() << "unable to find xfree86.lst!";
                return result;
            }
        }

        QFile xkbFile(xkbRulesFile.absoluteFilePath());
        if (!xkbFile.open(QIODevice::ReadOnly)) { return result; }
        QTextStream lines(&xkbFile);
        bool getContent = false;
        while (!lines.atEnd()) {
            QString line = lines.readLine();
            if (getContent) {
                if (line.startsWith("!")) {
                    getContent = false;
                    continue;
                }
                if (!line.simplified().isEmpty()) { result << line.simplified(); }
            }
            if (line.startsWith(findType)) { getContent = true; }
        }
        xkbFile.close();
        return result;
    }
    static void saveKeyboard(QString type, QString value)
    {
        QSettings settings(Draco::keyboardSettingsFile(), QSettings::IniFormat);
        settings.setValue(type, value);
    }
    static QString defaultKeyboard(QString type)
    {
        QSettings settings(Draco::keyboardSettingsFile(), QSettings::IniFormat);
        return settings.value(type).toString();
    }
    static const QString getKeyboardInfo(const QString &type)
    {
        QString result;
        QProcess proc;
        proc.start(QString("%1 -query").arg(SETXKBMAP));
        proc.waitForFinished();
        QString tmp = proc.readAll();
        QStringList list = tmp.split("\n");
        for (int i=0;i<list.size();++i) {
            QString line = list.at(i);
            if (line.startsWith(QString("%1:").arg(type))) {
                result = line.replace(QString("%1:").arg(type), "").trimmed();
            }
        }
        qDebug() << "using keyboard" << type <<result;
        return result;
    }
    static void loadKeyboard()
    {
        QString cmd;
        cmd.append(SETXKBMAP);

        QString layout = defaultKeyboard("layout");
        QString variant = defaultKeyboard("variant");
        QString model = defaultKeyboard("model");

        if (layout.isEmpty()) { return; }
        if (getKeyboardInfo("layout") == layout &&
            getKeyboardInfo("model") == model) { return; }

        cmd.append(" -layout ");
        cmd.append(layout);

        if (!variant.isEmpty()) {
            cmd.append(" -variant ");
            cmd.append(variant);
        }
        if (!model.isEmpty()) {
            cmd.append(" -model ");
            cmd.append(model);
        }
        QProcess::startDetached(cmd);
    }
};


#endif // KEYBOARD_COMMON_H
