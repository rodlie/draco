//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================

#include "LSession.h"
#include "LuminaXDG.h"

int main(int argc, char ** argv)
{
    // Setup any pre-QApplication initialization values
    //LTHEME::LoadCustomEnvSettings();
    LXDG::setEnvironmentVars();
    setenv("DESKTOP_SESSION", DESKTOP_APP_NAME, 1);
    setenv("XDG_CURRENT_DESKTOP", DESKTOP_APP_NAME, 1);
    //setenv("QT_QPA_PLATFORMTHEME", "lthemeengine", 1);
    unsetenv("QT_AUTO_SCREEN_SCALE_FACTOR"); // causes pixel-specific scaling issues with the desktop - turn this on after-the-fact for other apps

    // Startup the session
    LSession a(argc, argv);
    if (!a.isPrimaryProcess()) { return 0; }
    a.setupSession();
    return a.exec();
}
