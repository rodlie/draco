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
                    QProcess::startDetached(desktop.getExec());
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
                            QFileInfo fileInfo(fileName);
                            mimeUtils.openInApp(desktop.getExec(),
                                                fileInfo,
                                                QString());
                            return 0;
                        }
                    }
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
