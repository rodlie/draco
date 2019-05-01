#ifndef POWER_RTC_H
#define POWER_RTC_H

#include <QDateTime>

class PowerRtc
{
public:
    static bool setAlarm(const QDateTime &date);
};

#endif // POWER_RTC_H
