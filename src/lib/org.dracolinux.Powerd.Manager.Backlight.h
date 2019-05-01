#ifndef POWER_BACKLIGHT_H
#define POWER_BACKLIGHT_H

#include <QString>

class PowerBacklight
{
public:
    static const QString getDevice();
    static bool canAdjustBrightness(const QString &device);
    static bool canAdjustBrightness();
    static int getMaxBrightness(const QString &device);
    static int getMaxBrightness();
    static int getCurrentBrightness(const QString &device);
    static int getCurrentBrightness();
    static bool setCurrentBrightness(const QString &device, int value);
    static bool setCurrentBrightness(int value);
};

#endif // POWER_BACKLIGHT_H
