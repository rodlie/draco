#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QString>

class Manager : public QObject
{
    Q_OBJECT

public:
    explicit Manager(QObject *parent = NULL);

public slots:
    bool SetWakeAlarm(const QString &alarm);
    bool SetDisplayBacklight(const QString &device, int value);
    bool SetCpuGovernor(const QString &gov);
    bool SetCpuFrequency(const QString &freq);
};

#endif // MANAGER_H
