#include "dialog.h"
#include "desktopfile.h"
#include "mimeutils.h"
#include "common.h"

#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (argc>1) {
        QString fileName = argv[1];
        if (QFile::exists(fileName)) {
            if (fileName.endsWith(QString(".desktop"))) { // open app
                DesktopFile desktop(fileName);
                if (!desktop.getExec().isEmpty()) {
                    QString exe = desktop.getExec();
                    exe.replace("%u","");
                    exe.replace("%U","");
                    exe.replace("%f","");
                    exe.replace("%F","");
                    if (exe.contains("google-chrome") || exe.contains("chromium")) {
                        setenv("DESKTOP_SESSION", "xfce", 1);
                        setenv("XDG_CURRENT_DESKTOP", "xfce", 1);
                        QProcess proc;
                        proc.startDetached(exe);
                        setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
                        setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);
                    } else { QProcess::startDetached(exe); }
                    return 0;
                }
            } else { // run file
                QFileInfo info(fileName);
                if (info.isExecutable()) {
                    QProcess::startDetached(fileName);
                    return 0;
                } else { // open file
                    MimeUtils mimeUtils;
                    QString mime = mimeUtils.getMimeType(fileName);
                    if (!mime.isEmpty()) {
                        QStringList appNames = mimeUtils.getDefault(mime);
                        DesktopFile desktop(Common::findApplication(qApp->applicationFilePath(),
                                                                    appNames.at(0)));
                        if (!desktop.getExec().isEmpty()) {
                            QString exe = desktop.getExec();
                            exe.replace("%u","");
                            exe.replace("%U","");
                            exe.replace("%f","");
                            exe.replace("%F","");
                            if (exe.contains("google-chrome") || exe.contains("chromium")) {
                                setenv("DESKTOP_SESSION", "xfce", 1);
                                setenv("XDG_CURRENT_DESKTOP", "xfce", 1);
                                QProcess proc;
                                proc.startDetached(QString("%1 \"%2\"").arg(exe).arg(fileName));
                                setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
                                setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);
                            } else { QProcess::startDetached(QString("%1 \"%2\"").arg(exe).arg(fileName)); }
                            /*QFileInfo fileInfo(fileName);
                            mimeUtils.openInApp(exe,
                                                fileInfo,
                                                QString());*/
                            return 0;
                        }
                    }
                }
            }
        } else if (fileName.startsWith("http") || fileName.startsWith("ftp")) { // open url in browser
            MimeUtils mimeUtils;
            QString app = mimeUtils.getAppForMimeType("text/html");
            if (!app.isEmpty()) {
                QString desktopPath = Common::findApplication(qApp->applicationDirPath(), app);
                DesktopFile desktop(desktopPath);
                if (!desktop.getExec().isEmpty()) {
                    QString exe = desktop.getExec();
                    exe.replace("%u","");
                    exe.replace("%U","");
                    exe.replace("%f","");
                    exe.replace("%F","");
                    qDebug() << "OPEN BROWSER" << exe << fileName;
                    if (exe.contains("google-chrome") || exe.contains("chromium")) {
                        setenv("DESKTOP_SESSION", "xfce", 1);
                        setenv("XDG_CURRENT_DESKTOP", "xfce", 1);
                        QProcess proc;
                        proc.startDetached(QString("%1 \"%2\"").arg(exe).arg(fileName));
                        setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
                        setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);
                    } else { QProcess::startDetached(QString("%1 \"%2\"").arg(exe).arg(fileName)); }
                    return 0;
                }
            }
        }
        return 1;
    } else {
        Dialog d;
        d.show();
        return a.exec();
    }
}
