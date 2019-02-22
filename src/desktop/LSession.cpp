//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012-2015, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================

#include "LSession.h"
#include "LXcbEventFilter.h"
#include "LuminaX11.h"
#include "LUtils.h"
#include "LDesktopUtils.h"
#include "common.h" // common stuff

#include <QTime>
#include <QScreen>
#include <QtConcurrent>
#include <QMimeData>
#include <unistd.h> //for usleep() usage

XCBEventFilter *evFilter = Q_NULLPTR;
LIconCache *ICONS = Q_NULLPTR;

LSession::LSession(int &argc, char ** argv) :
    LSingleApplication(argc,
                       argv,
                       QString("%1-desktop").arg(DESKTOP_APP))
  , XCB(Q_NULLPTR)
  , watcher(Q_NULLPTR)
  , screenTimer(Q_NULLPTR)
  , xchange(false)
  , appmenu(Q_NULLPTR)
  , settingsmenu(Q_NULLPTR)
  , sysWindow(Q_NULLPTR)
  , currTranslator(Q_NULLPTR)
  , sessionsettings(Q_NULLPTR)
  , DPlugSettings(Q_NULLPTR)
  , cleansession(true)
  , SystemTrayID(0)
  , VisualTrayID(0)
  , TrayDmgEvent(0)
  , TrayDmgError(0)
  , TrayStopping(false)
  , lastActiveWin(0)
  , startupApps(true)
{
    // temp icon theme (TODO)
    QIcon::setThemeName("Adwaita");

    // setup primary
    if (isPrimaryProcess()) {
        qDebug() << "IS PRIMARY";
        connect(this,
                SIGNAL(InputsAvailable(QStringList)),
                this,
                SLOT(NewCommunication(QStringList)));

        setApplicationName(QString("%1 Desktop Environment").arg(DESKTOP_APP_NAME));
        setApplicationVersion(DESKTOP_APP_VERSION);
        setOrganizationName(QString("%1Linux").arg(DESKTOP_APP_NAME));
        setQuitOnLastWindowClosed(false); // since the LDesktop's are not necessarily "window"s

        // disable effects by default
        setEffectEnabled( Qt::UI_AnimateMenu, false);
        setEffectEnabled( Qt::UI_AnimateCombo, false);
        setEffectEnabled( Qt::UI_AnimateTooltip, false);

        //setAttribute(Qt::AA_UseDesktopOpenGL);
        setAttribute(Qt::AA_UseHighDpiPixmaps); // allow pixmaps to be scaled up as well as down

        // setup timer
        screenTimer = new QTimer(this);
        screenTimer->setSingleShot(true);
        screenTimer->setInterval(50);
        connect(screenTimer,
                SIGNAL(timeout()),
                this,
                SLOT(updateDesktops()));

        // check for clean session and startup apps
        for (int i=1; i<argc; i++) {
            if (QString::fromLocal8Bit(argv[i]) == "--noclean") {
                cleansession = false;
            } else if (QString::fromLocal8Bit(argv[i]) == "--nostartup") {
                startupApps = false;
            }
        }

        ICONS = new LIconCache(this);
        XCB = new LXCB(); //need access to XCB data/functions right away

        // Setup the event filter
        evFilter =  new XCBEventFilter(this);
        installNativeEventFilter(evFilter);
        connect(this,
                SIGNAL(screenAdded(QScreen*)),
                this,
                SLOT(screensChanged()));
        connect(this,
                SIGNAL(screenRemoved(QScreen*)),
                this,
                SLOT(screensChanged()));
        connect(this,
                SIGNAL(primaryScreenChanged(QScreen*)),
                this,
                SLOT(screensChanged()));

        // Clipboard
        ignoreClipboard = false;
        qRegisterMetaType<QClipboard::Mode>("QClipboard::Mode");
        connect(QApplication::clipboard(),
              SIGNAL(changed(QClipboard::Mode)),
              this,
              SLOT(handleClipboard(QClipboard::Mode)));

    } // end check for primary process
}

LSession::~LSession()
{
    if (this->isPrimaryProcess()) {
        //WM->stopWM();
        for (int i=0; i<DESKTOPS.length(); i++) {
            DESKTOPS[i]->deleteLater();
        }
        //delete WM;
        settingsmenu->deleteLater();
        appmenu->deleteLater();
        delete currTranslator;
    }
}

void LSession::setupSession()
{
    qDebug() << "setup desktop session";

    // Seed random number generator (if needed)
    qsrand( QTime::currentTime().msec() );

    // Remove existing session file
    if (QFile::exists(Draco::sessionFile())) { QFile::remove(Draco::sessionFile()); }

    // Initialize settings files
    sessionsettings = new QSettings(QString("%1-desktop").arg(DESKTOP_APP),
                                    QString(DE_SESSION_SETTINGS));
    DPlugSettings = new QSettings(QString("%1-desktop").arg(DESKTOP_APP),
                                  QString("%1/%2")
                                  .arg(DE_PLUGIN_SETTINGS)
                                  .arg(DE_DESKTOP_SETTINGS));

    // Initialize the internal variables
    DESKTOPS.clear();

    // Initialize the system tray
    startSystemTray();

    // Initialize the global menus
    appmenu = new AppMenu();

    // Initialize settings menu
    settingsmenu = new SettingsMenu();

    // Initialize system window
    sysWindow = new SystemWindow();

    // Initialize the desktops
    desktopFiles = QDir(LUtils::standardDirectory(LUtils::Desktop))
                   .entryInfoList(QDir::NoDotAndDotDot |
                                  QDir::Files |
                                  QDir::Dirs,
                                  QDir::Name |
                                  QDir::IgnoreCase |
                                  QDir::DirsFirst);
    updateDesktops();

    // Setup the system watcher for changes
    watcher = new QFileSystemWatcher(this);

    // add user media to watcher
    QString userMedia = QString("/run/media/%1")
                        .arg(QDir::homePath().split("/").takeLast());
    if (QFile::exists(userMedia)) { watcherChange(userMedia); }

    // add "standards" directories to watcher
    watcherChange(LUtils::standardDirectory(LUtils::Desktop));

    // watch openbox settings
    // TODO

    // watch settings
    watcherChange(Draco::sessionSettingsFile());
    watcherChange(Draco::desktopSettingsFile());
    watcherChange(Draco::envSettingsFile());

    // connect internal signals/slots
    connect(watcher,
            SIGNAL(directoryChanged(QString)),
            this,
            SLOT(watcherChange(QString)));
    connect(watcher,
            SIGNAL(fileChanged(QString)),
            this,
            SLOT(watcherChange(QString)));
    connect(this,
            SIGNAL(aboutToQuit()),
            this,
            SLOT(SessionEnding()));

    // Initialize startup applications
    launchStartupApps();

    qDebug() << "[DESKTOP INIT FINISHED]";
}

void LSession::CleanupSession()
{
    // Close any running applications and tray utilities (Make sure to keep the UI interactive)
    LSession::processEvents();
    QDateTime time = QDateTime::currentDateTime();
    qDebug() << "Start closing down the session: " << time.toString(Qt::SystemLocaleShortDate);

    // Create a temporary flag to prevent crash dialogs from opening during cleanup
    LUtils::writeFile(Draco::sessionFile(),
                      QStringList() << "yes",
                      true);

    // Stop the background system tray (detaching/closing apps as necessary)
    stopSystemTray(!cleansession);

    // Now perform any other cleanup
    if (cleansession) {
        // Close any open windows
        qDebug() << " - Closing any open windows";
        QList<WId> WL = XCB->WindowList(true);

        for (int i=0; i<WL.length(); i++) {
            qDebug() << " - Closing window:" << XCB->WindowClass(WL[i]) << WL[i];
            XCB->CloseWindow(WL[i]);
            LSession::processEvents();
        }

        // Now wait a moment for things to close down before quitting
        for (int i=0; i<20; i++) { LSession::processEvents(); usleep(25); } //1/2 second pause

        // Kill any remaining windows
        WL = XCB->WindowList(true); //all workspaces
        for (int i=0; i<WL.length(); i++) {
            qDebug() << " - Window did not close, killing application:" << XCB->WindowClass(WL[i]) << WL[i];
            XCB->KillClient(WL[i]);
            LSession::processEvents();
        }
    }

    evFilter->StopEventHandling();

    // Stop the window manager
    //qDebug() << " - Stopping the window manager";
    //WM->stopWM();

    // Now close down the desktop
    qDebug() << " - Closing down the desktop elements";
    for (int i=0; i<DESKTOPS.length(); i++) {
        DESKTOPS[i]->prepareToClose();
        // don't actually close them yet - that will happen when the session exits
        // this will leave the wallpapers up for a few moments (preventing black screens)
    }

    // Now wait a moment for things to close down before quitting
    for (int i=0; i<20; i++) { LSession::processEvents(); usleep(25000); } //1/2 second pause

    // Clean up the temporary flag
    if (QFile::exists(Draco::sessionFile())) { QFile::remove(Draco::sessionFile()); }
}


void LSession::NewCommunication(QStringList list)
{
    qDebug() << "New Communications:" << list;
    for (int i=0; i<list.length(); i++) {
        if (list[i]=="--check-geoms") {
            screensChanged();
        } else if (list[i]=="--show-start") {
            emit StartButtonActivated();
        } else if (list[i]=="--logout") {
            QTimer::singleShot(1000,
                               this,
                               SLOT(StartLogout()));
        }
    }
}

void LSession::launchStartupApps()
{
    if (!startupApps) { return; }
    QList<XDGDesktop*> xdgapps = LXDG::findAutoStartFiles(true);
    qDebug() << " - launch startup apps";
    for(int i=0; i<xdgapps.length(); i++){
        if (xdgapps[i]->isHidden) {
            qDebug() << "skip since hidden" << xdgapps[i]->name;
            continue;
        }
        if (!xdgapps[i]->isValid(false)) {
            qDebug() << "skip invalid (including ShowOnlyIn!=Draco)" << xdgapps[i]->name;
            continue;
        }
        // Generate command and clean up any stray "Exec" field codes (should not be any here)
        QString cmd = xdgapps[i]->getDesktopExec();
        if (cmd.contains("%")){ cmd = cmd.remove("%U")
                                      .remove("%u")
                                      .remove("%F")
                                      .remove("%f")
                                      .remove("%i")
                                      .remove("%c")
                                      .remove("%k")
                                      .simplified(); }
        // Now run the command
        if (!cmd.isEmpty()) {
            qDebug() << " - Auto-Starting File:" << xdgapps[i]->filePath;
            //QProcess::startDetached(cmd);
        }
    }
    // make sure we clean up all the xdgapps structures
    for (int i=0;  i<xdgapps.length(); i++) { xdgapps[i]->deleteLater(); }
}

void LSession::StartLogout()
{
    CleanupSession();
    QCoreApplication::exit(0);
}

void LSession::StartShutdown()
{
    CleanupSession();
    //LOS::systemShutdown(skipupdates);
    // TODO use powerkit
    QCoreApplication::exit(0);
}

void LSession::StartReboot()
{
    CleanupSession();
    //LOS::systemRestart(skipupdates);
    // TODO use powerkit
    QCoreApplication::exit(0);
}

void LSession::reloadIconTheme()
{
    // Wait a moment for things to settle before sending out the signal to the interfaces
    QApplication::processEvents();
    QApplication::processEvents();
    emit IconThemeChanged();
}

void LSession::watcherChange(QString changed)
{
    if (!sessionSettings()) { return; }

    qDebug() << "Session Watcher Change:" << changed;
    // OPENBOX TODO if(changed.endsWith("fluxbox-init") || changed.endsWith("fluxbox-keys")){ refreshWindowManager(); }

    if (changed.endsWith("sessionsettings.conf") ){
        sessionsettings->sync();
        qDebug() << "Session Settings Changed";
        /*if (sessionsettings->contains("Qt5_theme_engine")) {
            QString engine = sessionsettings->value("Qt5_theme_engine","").toString();
            qDebug() << "Set Qt5 theme engine: " << engine;
            if (engine.isEmpty()) { unsetenv("QT_QPA_PLATFORMTHEME"); }
            else { setenv("QT_QPA_PLATFORMTHEME", engine.toUtf8().data(),1); }
        } else {
            setenv("QT_QPA_PLATFORMTHEME", "lthemeengine",1); //ensure the lumina theme engine is always enabled
        }*/
        emit SessionConfigChanged();
    }
    else if (changed.endsWith("desktopsettings.conf") ) { emit DesktopConfigChanged(); }
    else if (changed == LUtils::standardDirectory(LUtils::Desktop) )
    {
        desktopFiles = QDir(changed).entryInfoList(QDir::NoDotAndDotDot |
                                                   QDir::Files |
                                                   QDir::Dirs, QDir::Name |
                                                   QDir::IgnoreCase |
                                                   QDir::DirsFirst);
        qDebug() << "New Desktop Files:" << desktopFiles.length();
        emit DesktopFilesChanged();
    }
    else if(changed.toLower() == "/media" ||
            changed.toLower().startsWith("/run/media/") ||
            changed == "/tmp/.autofs_change" )
    {
        emit MediaFilesChanged();
    }

    // Now ensure this file was not removed from the watcher
    if (!watcher->files().contains(changed) && !watcher->directories().contains(changed)) {
        if(!QFile::exists(changed)){
            // Create the file really quick to ensure it can be watched
            // TODO
        }
        watcher->addPath(changed);
    }
}

void LSession::screensChanged()
{
    qDebug() << "Screen Number Changed";
    if (screenTimer->isActive()) { screenTimer->stop(); }
    screenTimer->start();
    xchange = true;
}

void LSession::screenResized(int scrn)
{
    qDebug() << "Screen Resized:" << scrn;
    if (screenTimer->isActive()) { screenTimer->stop(); }
    screenTimer->start();
    xchange = true;
}

void LSession::checkWindowGeoms()
{
    //Only do one window per run (this will be called once per new window - with time delays between)
    if (checkWin.isEmpty()) { return; }
    WId win = checkWin.takeFirst();
    if (RunningApps.contains(win) ) { //just to make sure it did not close during the delay
        adjustWindowGeom( win );
    }
}

void LSession::setupFallbackDesktop(QSettings *dset)
{
    if (!dset->contains(QString("desktop-fallback/screen/lastHeight"))) {
        dset->setValue(QString("desktop-fallback/screen/lastHeight"),
                       screenRect.height());
    } else {
        if (dset->value(QString("desktop-fallback/screen/lastHeight"))
                        .toInt() != screenRect.height())
        {
            dset->setValue(QString("desktop-fallback/screen/lastHeight"),
                           screenRect.height());
        }
    }
    if (!dset->contains(QString("desktop-fallback/screen/lastWidth"))) {
        dset->setValue(QString("desktop-fallback/screen/lastWidth"),
                       screenRect.width());
    } else {
        if (dset->value(QString("desktop-fallback/screen/lastWidth"))
                        .toInt() != screenRect.width())
        {
            dset->setValue(QString("desktop-fallback/screen/lastWidth"),
                           screenRect.width());
        }
    }
    if (!dset->contains(QString("desktop-fallback/panels"))) {
        dset->setValue(QString("desktop-fallback/panels"), 1);
    }
    if (!dset->contains(QString("desktop-fallback/generateMediaIcons"))) {
        dset->setValue(QString("desktop-fallback/generateMediaIcons"), true);
    }
    if (!dset->contains(QString("desktop-fallback/generateDesktopIcons"))) {
        dset->setValue(QString("desktop-fallback/generateDesktopIcons"), true);
    }
    if (!dset->contains(QString("desktop-fallback/GridSize"))) {
        dset->setValue(QString("desktop-fallback/GridSize"), 100);
    }
    if (!dset->contains(QString("desktop-fallback/background/format"))) {
        dset->setValue(QString("desktop-fallback/background/format"),
                       QString("full"));
    }
    if (dset->contains(QString("desktop-fallback/background/minutesToChange"))) {
        dset->setValue(QString("desktop-fallback/background/minutesToChange"), 1);
    }
    dset->sync();
}

void LSession::refreshWindowManager()
{
    //LUtils::runCmd("touch \""+QString(getenv("XDG_CONFIG_HOME"))+"/lumina-desktop/fluxbox-init\"" );
}

void LSession::updateDesktops()
{
    qDebug() << "update desktops";
    QDesktopWidget *DW = this->desktop();

    int sC = DW->screenCount();
    qDebug() << "screen count" << sC;
    qDebug() << "desktops length" << DESKTOPS.length();
    if (sC<1) { return; } //stop here - no screens available temporarily (displayport/4K issue)

    screenRect = QRect(); //clear it
    for (int i=0; i<sC; i++) {
        screenRect = screenRect.united(DW->screenGeometry(i));
        qDebug() << "screen" << i << DW->screenGeometry();
    }

    bool firstrun = (DESKTOPS.length()==0);
    //bool numchange = DESKTOPS.length()!=sC;
    QSettings dset(QString("%1-desktop")
                 .arg(DESKTOP_APP),
                 DE_DESKTOP_SETTINGS);

    // add fallback desktop settings
    setupFallbackDesktop(&dset);

    if (firstrun && sC==1) {
        QString name = QApplication::screens().at(0)->name();
        qDebug() << "checking desktop settings" << name;
        if (!dset.contains("desktop-"+name+"/screen/lastHeight")) {
            qDebug() << "add fallback settings for desktop" << name;
            LDesktopUtils::MigrateDesktopSettings(&dset, "fallback", name);
        }
    }

    // If the screen count is changing on us
    if (sC != DW->screenCount()) {
        qDebug() << "Screen Count changed while running";
        return;
    }

    // First clean up any current desktops
    QList<int> dnums; // keep track of which screens are already managed
    QList<QRect> geoms;
    for (int i=0; i<DESKTOPS.length(); i++) {
        if (DESKTOPS[i]->Screen() < 0 ||
            DESKTOPS[i]->Screen() >= sC ||
            geoms.contains(DW->screenGeometry(DESKTOPS[i]->Screen())))
        {
            qDebug() << " - Close desktop:" << i;
            qDebug() << " - Close desktop on screen:" << DESKTOPS[i]->Screen();
            DESKTOPS[i]->prepareToClose();
            DESKTOPS.takeAt(i)->deleteLater();
            i--;
        } else {
            qDebug() << " - Show desktop:" << i;
            qDebug() << " - Show desktop on screen:" << DESKTOPS[i]->Screen();
            DESKTOPS[i]->UpdateGeometry();
            DESKTOPS[i]->show();
            dnums << DESKTOPS[i]->Screen();
            geoms << DW->screenGeometry(DESKTOPS[i]->Screen());
        }
    }

    // Now add any new desktops
    QStringList allNames;
    QList<QScreen*> scrns = QApplication::screens();
    for (int i=0; i<sC; i++) {
        allNames << scrns.at(i)->name();
        if (!dnums.contains(i) && !geoms.contains(DW->screenGeometry(i))) {
            // Start the desktop on this screen
            qDebug() << " - Start desktop on screen:" << i;
            DESKTOPS << new LDesktop(i);
            geoms << DW->screenGeometry(i);
        }
    }
    dset.setValue("last_used_screens", allNames);

    // REMOVE?
    //Make sure fluxbox also gets prompted to re-load screen config if the number of screens changes in the middle of a session
    if(!firstrun && xchange){
        qDebug() << "Update WM";
        //QProcess::startDetached("killall fluxbox");
        xchange = false;
    }

    // Make sure all the background windows are registered on the system as virtual roots
    QTimer::singleShot(100,this, SLOT(registerDesktopWindows()));
}

void LSession::registerDesktopWindows()
{
    QList<WId> wins;
    for (int i=0; i<DESKTOPS.length(); i++) {
        wins << DESKTOPS[i]->backgroundID();
    }
    XCB->RegisterVirtualRoots(wins);
}

// REMOVE?
void LSession::adjustWindowGeom(WId win, bool maximize){
  //return; //temporary disable
  qDebug() << "AdjustWindowGeometry():" << win << maximize << XCB->WindowClass(win);
  if(XCB->WindowIsFullscreen(win) >=0 ){ return; } //don't touch it
  //Quick hack for making sure that new windows are not located underneath any panels
  // Get the window location
  QRect geom = XCB->WindowGeometry(win, false);
  //Get the frame size
  QList<int> frame = XCB->WindowFrameGeometry(win); //[top,bottom,left,right] sizes of the frame
  //Calculate the full geometry (window + frame)
  QRect fgeom = QRect(geom.x()-frame[2], geom.y()-frame[0], geom.width()+frame[2]+frame[3], geom.height()+frame[0]+frame[1]);

    qDebug() << "Check Window Geometry:" << XCB->WindowClass(win) << !geom.isNull() << geom << fgeom;

  if(geom.isNull()){ return; } //Could not get geometry for some reason
  //Get the available geometry for the screen the window is on
  QRect desk;
  for(int i=0; i<DESKTOPS.length(); i++){
    if( this->desktop()->screenGeometry(DESKTOPS[i]->Screen()).contains(geom.center()) ){
      //Window is on this screen
      qDebug() << " - On Screen:" << DESKTOPS[i]->Screen();
      desk = DESKTOPS[i]->availableScreenGeom();
      qDebug() << " - Screen Geom:" << desk;
      break;
    }
  }
  if(desk.isNull()){ return; } //Unable to determine screen
  //Adjust the window location if necessary
  if(maximize){
    qDebug() << " - Maximizing New Window:" << desk.width() << desk.height();
    geom = desk; //Use the full screen
    XCB->MoveResizeWindow(win, geom);
    XCB->MaximizeWindow(win, true); //directly set the appropriate "maximized" flags (bypassing WM)

  }else if(!desk.contains(fgeom) ){
    //Adjust origin point for left/top margins
    if(fgeom.y() < desk.y()){ geom.moveTop(desk.y()+frame[0]); fgeom.moveTop(desk.y()); } //move down to the edge (top panel)
    if(fgeom.x() < desk.x()){ geom.moveLeft(desk.x()+frame[2]); fgeom.moveLeft(desk.x()); } //move right to the edge (left panel)
    //Adjust size for bottom margins (within reason, since window titles are on top normally)
   // if(geom.right() > desk.right() && (geom.width() > 100)){ geom.setRight(desk.right()); }
    if(fgeom.bottom() > desk.bottom() && geom.height() > 10){
      qDebug() << "Adjust Y:" << fgeom << geom << desk;
      int diff = fgeom.bottom()-desk.bottom(); //amount of overlap
      qDebug() << "Y-Diff:" << diff;
      if(diff < 0){ diff = -diff; } //need a positive value
      if( (fgeom.height()+ diff)< desk.height()){
        //just move the window - there is room for it above
	geom.setBottom(desk.bottom()-frame[1]);
	fgeom.setBottom(desk.bottom());
      }else if(geom.height() > diff){ //window bigger than the difference
	//Need to resize the window - keeping the origin point the same
	geom.setHeight( geom.height()-diff-1 ); //shrink it by the difference (need an extra pixel somewhere)
	fgeom.setHeight( fgeom.height()-diff );
      }
    }
    //Now move/resize the window

      qDebug() << " - New Geom:" << geom << fgeom;

    XCB->WM_Request_MoveResize_Window(win, geom);
  }

}

void LSession::SessionEnding()
{
    stopSystemTray(); // just in case it was not stopped properly earlier
}

void LSession::handleClipboard(QClipboard::Mode mode)
{
    if ( !ignoreClipboard && mode == QClipboard::Clipboard ){ //only support Clipboard
        const QMimeData *mime = QApplication::clipboard()->mimeData(mode);
        if (mime==Q_NULLPTR) { return; }
        if (mime->hasText() && !QApplication::clipboard()->ownsClipboard()) {
            // preserve the entire mimeData set, not just the text
            // Note that even when we want to "save" the test, we should keep the whole thing
            //  this preserves formatting codes and more that apps might need
            QMimeData *copy = new QMimeData();
            QStringList fmts = mime->formats();
            for (int i=0; i<fmts.length(); i++) { copy->setData(fmts[i], mime->data(fmts[i])); }
            ignoreClipboard = true;
            QApplication::clipboard()->setMimeData(copy, mode);
            ignoreClipboard = false;
            //QMetaObject::invokeMethod(this, "storeClipboard", Qt::QueuedConnection, Q_ARG(QString, mime->text()), Q_ARG(QClipboard::Mode, mode));
        }
    }
}

void LSession::storeClipboard(QString text, QClipboard::Mode mode) {
    ignoreClipboard = true;
    QApplication::clipboard()->setText(text, mode);
    ignoreClipboard = false;
}

//===============
//  SYSTEM ACCESS
//===============
void LSession::LaunchApplication(QString cmd)
{
    qDebug() << "launch application" << cmd;
    QProcess::startDetached(cmd);
}

QFileInfoList LSession::DesktopFiles()
{
    return desktopFiles;
}

QRect LSession::screenGeom(int num)
{
    if (num < 0 || num >= this->desktop()->screenCount() ) { return QRect(); }
    QRect geom = this->desktop()->screenGeometry(num);
    return geom;
}

AppMenu* LSession::applicationMenu()
{
    return appmenu;
}

SettingsMenu* LSession::settingsMenu()
{
    return settingsmenu;
}

QSettings* LSession::sessionSettings()
{
    return sessionsettings;
}

QSettings* LSession::DesktopPluginSettings()
{
    return DPlugSettings;
}

WId LSession::activeWindow()
{
    // Check the last active window pointer first
    WId active = XCB->ActiveWindow();
    qDebug() << "Check Active Window:" << active << lastActiveWin;
    if (RunningApps.contains(active)) { lastActiveWin = active; }
    else if (RunningApps.contains(lastActiveWin) && XCB->WindowState(lastActiveWin) >= LXCB::VISIBLE){} //no change needed
    else if (RunningApps.contains(lastActiveWin) && RunningApps.length()>1){
        int start = RunningApps.indexOf(lastActiveWin);
        if (start<1) { lastActiveWin = RunningApps.length()-1; } // wrap around to the last item
        else { lastActiveWin = RunningApps[start-1]; }
    } else {
        // Need to change the last active window - find the first one which is visible
        lastActiveWin = 0; // fallback value - nothing active
        for (int i=0; i<RunningApps.length(); i++) {
            if (XCB->WindowState(RunningApps[i]) >= LXCB::VISIBLE) {
                lastActiveWin = RunningApps[i];
                break;
            }
        }
        qDebug() << " -- New Last Active Window:" << lastActiveWin;
    }
    return lastActiveWin;
}

// Temporarily change the session locale (nothing saved between sessions)
void LSession::switchLocale(QString localeCode)
{
    Q_UNUSED(localeCode)
  /*currTranslator = LUtils::LoadTranslation(this, "lumina-desktop", localeCode, currTranslator);
  if(currTranslator!=0 || localeCode=="en_US"){
    LUtils::setLocaleEnv(localeCode); //will set everything to this locale (no custom settings)
  }
  emit LocaleChanged();*/
}

void LSession::systemWindow()
{
    if (sysWindow==Q_NULLPTR) { sysWindow = new SystemWindow(); }
    else { sysWindow->updateWindow(); }
    sysWindow->show();
    //LSession::processEvents();
}

// =======================
//  XCB EVENT FILTER FUNCTIONS
// =======================
void LSession::RootSizeChange()
{
    if (DESKTOPS.isEmpty() || screenRect.isNull()) { return; } // Initial setup not run yet
    QDesktopWidget *DW = this->desktop();
    int sC = DW->screenCount();
    QRect tmp;
    for (int i=0; i<sC; i++) { tmp = tmp.united(DW->screenGeometry(i)); }
    if (tmp == screenRect) { return; } // false event - session size did not change
    qDebug() << "Got Root Size Change";
    xchange = true;
    screenTimer->start();
}

void LSession::WindowPropertyEvent()
{
    qDebug() << "Window Property Event";
    QList<WId> newapps = XCB->WindowList();
    if (RunningApps.length() < newapps.length()) {
        // New Window found
        qDebug() << "New window found";
        // Perform sanity checks on any new window geometries
        for (int i=0; i<newapps.length() && !TrayStopping; i++) {
            if (!RunningApps.contains(newapps[i])) {
                checkWin << newapps[i];
                XCB->SelectInput(newapps[i]); // make sure we get property/focus events for this window
                qDebug() << "New Window - check geom in a moment:" << XCB->WindowClass(newapps[i]);
                QTimer::singleShot(50, this, SLOT(checkWindowGeoms()) );
            }
        }
    }

    // Now save the list and send out the event
    RunningApps = newapps;
    emit WindowListEvent();
}

void LSession::WindowPropertyEvent(WId win)
{
    // Emit the single-app signal if the window in question is one used by the task manager
    if (RunningApps.contains(win)) {
        qDebug() << "Single-window property event";
        WindowPropertyEvent(); // Run through the entire routine for window checks
    } else if (RunningTrayApps.contains(win)) {
        emit TrayIconChanged(win);
    }
}

void LSession::SysTrayDockRequest(WId win)
{
    if (TrayStopping) { return; }
    attachTrayWindow(win); // Check to see if the window is already registered
}

void LSession::WindowClosedEvent(WId win)
{
    if (TrayStopping) { return; }
    removeTrayWindow(win); // Check to see if the window is a tray app
}

void LSession::WindowConfigureEvent(WId win)
{
    if (TrayStopping){ return; }
    if (RunningTrayApps.contains(win)) {
        qDebug() << "SysTray: Configure Event";
        emit TrayIconChanged(win); // trigger a repaint event
    } else if (RunningApps.contains(win)) {
        WindowPropertyEvent();
    }
}

void LSession::WindowDamageEvent(WId win)
{
    if (TrayStopping) { return; }
    if (RunningTrayApps.contains(win)) {
        qDebug() << "SysTray: Damage Event";
        emit TrayIconChanged(win); // trigger a repaint event
    }
}

void LSession::WindowSelectionClearEvent(WId win)
{
    if (win==SystemTrayID && !TrayStopping) {
        qDebug() << "Stopping system tray";
        stopSystemTray(true); // make sure to detach all windows
    }
}


//======================
//   SYSTEM TRAY FUNCTIONS
//======================
bool LSession::registerVisualTray(WId visualTray)
{
    // Only one visual tray can be registered at a time
    //  (limitation of how tray apps are embedded)
    if (TrayStopping) { return false; }
    else if (VisualTrayID==0) { VisualTrayID = visualTray; return true; }
    else if (VisualTrayID==visualTray) { return true; }
    else { return false; }
}

void LSession::unregisterVisualTray(WId visualTray)
{
    if (VisualTrayID==visualTray) {
        qDebug() << "Unregistered Visual Tray";
        VisualTrayID=0;
        if (!TrayStopping) { emit VisualTrayAvailable(); }
    }
}

QList<WId> LSession::currentTrayApps(WId visualTray)
{
    if (visualTray==VisualTrayID) {
        // Check the validity of all the current tray apps (make sure nothing closed erratically)
        for (int i=0; i<RunningTrayApps.length(); i++) {
            if (XCB->WindowClass(RunningTrayApps[i]).isEmpty()) { RunningTrayApps.removeAt(i); i--; }
        }
        return RunningTrayApps;
    }
    else if (registerVisualTray(visualTray)) {
        return RunningTrayApps;
    } else {
        return QList<WId>();
    }
}

void LSession::startSystemTray()
{
    if (SystemTrayID!=0) { return; }
    RunningTrayApps.clear(); //nothing running yet
    SystemTrayID = XCB->startSystemTray(0);
    TrayStopping = false;
    if (SystemTrayID!=0) {
        XCB->SelectInput(SystemTrayID); // make sure TrayID events get forwarded here
        TrayDmgEvent = XCB->GenerateDamageID(SystemTrayID);
        evFilter->setTrayDamageFlag(TrayDmgEvent);
        qDebug() << "System Tray Started Successfully";
        qDebug() << " - System Tray Flags:" << TrayDmgEvent << TrayDmgError;
    }
}

void LSession::stopSystemTray(bool detachall)
{
    if (TrayStopping) { return; } // already run
    qDebug() << "Stopping system tray...";
    TrayStopping = true; // make sure the internal list does not modified during this

    // Close all the running Tray Apps
    QList<WId> tmpApps = RunningTrayApps;
    RunningTrayApps.clear(); // clear this ahead of time so tray's do not attempt to re-access the apps
    if (!detachall){
        for (int i=0; i<tmpApps.length(); i++) {
            qDebug() << " - Stopping tray app:" << XCB->WindowClass(tmpApps[i]);
            // Tray apps are special and closing the window does not close the app
            XCB->KillClient(tmpApps[i]);
            LSession::processEvents();
        }
    }

    // Now close down the tray backend
    XCB->closeSystemTray(SystemTrayID);
    SystemTrayID = 0;
    TrayDmgEvent = 0;
    TrayDmgError = 0;
    evFilter->setTrayDamageFlag(0); // turn off tray event handling
    emit TrayListChanged();
    LSession::processEvents();
}

void LSession::attachTrayWindow(WId win)
{
    if (TrayStopping) { return; }
    if (RunningTrayApps.contains(win)) { return; } // already managed
    qDebug() << "Session Tray: Window Added";
    RunningTrayApps << win;
    qDebug() << "Tray List Changed";
    emit TrayListChanged();
}

void LSession::removeTrayWindow(WId win)
{
    if (SystemTrayID==0) { return; }
    for (int i=0; i<RunningTrayApps.length(); i++) {
        if (win==RunningTrayApps[i]) {
            qDebug() << "Session Tray: Window Removed";
            RunningTrayApps.removeAt(i);
            emit TrayListChanged();
            break;
        }
    }
}

//=========================
//  START MENU FUNCTIONS
//=========================
bool LSession::registerStartButton(QString ID)
{
    if (StartButtonID.isEmpty()) { StartButtonID = ID; }
    return (StartButtonID==ID);
}

void LSession::unregisterStartButton(QString ID)
{
    if (StartButtonID == ID) {
        StartButtonID.clear();
        emit StartButtonAvailable();
    }
}
