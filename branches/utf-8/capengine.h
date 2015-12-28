#ifndef _CAPENGINE_H_
#define _CAPENGINE_H_

#include "dapplication.h"
#include <iostream>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <unistd.h>

// added for backlight control
#define FBIOSETBKLIGHT          0x4619
#define FBIOGETBKLIGHT          0x461A
#define FBIOSETBRIGHTNESS       0x461B 
#define FBIOGETBRIGHTNESS       0x461C 

class CapToolBar;
class CapEngine : public PointerListener
{
    Q_OBJECT
    private:

        ZMainWidget *capmainwidget;
        QListBox *msg;
        QLabel *tmp;
        QLabel *rect;
        QString ImgDirName,ImgDirPath,suffixs,format;
        CapToolBar *toolbar;
        QPixmap AlertButtonPixmap,SuffixButtonPixmap;
        UTIL_PushButton *alertbutton,*suffixbutton;
        int Interval;
        int CapAlert;
        int CapMode;
        bool ChildPixmap;
        bool TmpState;
        int ImgSuffix;
        int ImgMunber;
        int ImgTotal;
        int PixmapX;
        int PixmapY;
        int PixmapW;
        int PixmapH;
        int PixmapOriginX;
        int PixmapOriginY;
        int KeyCode;
        int KeyExit;
        int Brightness;
        char s[240*320*3+1];

    public:

        CapEngine(ZMainWidget *mainwidget, QListBox *msgtext);
        virtual ~CapEngine();

        void initial(const QString dirname,
                     const QString dirpath,
                     int suffix,
                     int keycode,
                     int keyexit,
                     int alert,
                     int capmode,
                     int imgmunber,
                     bool childpixmap=false,
                     int interval=0);
        void intervalcap();
        void toolbarcap();
        void pointerPressed( int x, int y );
        void pointerReleased( int x, int y );
        void pointerDrag( int x, int y );
        void keyPressed(int keycode);
        void keyPresslong(int keycode);
        void QCopReceived(int message = 0);
        void PropertyReceived();
        void backlightctrl(bool onoff,int brightness = 20);
        void keylightctrl(bool onoff);

    public slots:
        void beforeterminate();
        void cap();
        void alerttag(int);
        void setsuffix(int);
};

class CapToolBar : public QLabel{
	Q_OBJECT

    public:
        //int floattoolbarx,floattoolbary;

    protected:
        void mouseMoveEvent  (  QMouseEvent *e );

    public:
        CapToolBar();

};

#endif

