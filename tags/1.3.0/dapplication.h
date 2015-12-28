#ifndef _DAPPLICATION_H_
#define _DAPPLICATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <qtextstream.h>
#include <qfile.h>
#include <qdir.h>
#include <qfont.h>
#include <qtextview.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qvbuttongroup.h>
#include <qhbuttongroup.h>
#include <qradiobutton.h>
#include <qimage.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qtextcodec.h>
#include <qcolor.h>
#include <qtimer.h>
#include <qpopupmenu.h>
#include <qstringlist.h>
#include <qlineedit.h>
#include <qmenubar.h> 

#include <time.h>
#include <ZApplication.h>
#include <ZMainWidget.h>
#include <ZMessageBox.h>
#include <ZPushButton.h>
#include <ZBaseDialog.h>
#include <UTIL_DlgCst.h>
#include <UTIL_Language.h>
#include <ZLanguage.h>
#include <ezxutilpushbutton.h>
#include <ZMultiLineEdit.h>
#include <ZScrollView.h>
#include <ZGlobal.h>
#include <ezxutilcst.h>
#include <ezxutildialog.h>
#include <qlistbox.h>
#include <ezxutillistbox.h>
#include <ezxres.h>
#include <ZSelectFolderDlg.h>
#include <SetupSetManager.h>

//#define APPDIR "/mmc/mmca1/.system/QTDownLoad/ecap_dnxh/"
#define DEBUG_PRINT 0
#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )
#define dbg_printf(fmt,args...) \
    ((void) ({ \
     if (DEBUG_PRINT) \
        printf(fmt,## args); \
     0; \
    }))

class PointerListener : public QObject {
    //Q_OBJECT
    public:

        PointerListener() {}
        virtual void QCopReceived(int message) = 0;
        virtual void PropertyReceived( ) = 0;
        virtual void pointerPressed( int x, int y) = 0;
        virtual void pointerReleased( int x, int y) = 0;
        virtual void pointerDrag( int x, int y) = 0;
        virtual void keyPressed(int keycode) = 0;
        virtual void keyPresslong(int keycode) = 0;
};

class DApplication : public ZApplication {
    Q_OBJECT
    private:

        PointerListener *pointerListener;
        bool press;
        bool keypress;
        int pressedtime;
        QString DirPath;

    public:
        DApplication(int argc, char** argv):
        ZApplication(argc, argv)
        {
            pointerListener = 0;
            keypress = true;
            pressedtime = 0;
        }
        virtual ~DApplication() {dbg_printf("exit application\n");}
        void insertStr(QWSEvent*) {}

        virtual bool qwsEventFilter(QWSEvent* event);
        void timerEvent(QTimerEvent *);
     
        void setPointerListener( PointerListener *listener )
        {
        pointerListener = listener;
        }

        void setDirPath(const QString &str);

    public slots:
        //void OpenDir();
        void OpenFile(QListBoxItem *item,const QPoint &);
 };

#endif
