#ifndef _CAPDIALOG_H_
#define _CAPDIALOG_H_

#include <stdlib.h>
#include <stdio.h>
#include <qobject.h>

//#include "dapplication.h"
#include "getkey.h"

class CapDialog : public ZBaseDialog
{
    Q_OBJECT
public:

    CapDialog(QWidget *parent = 0,const char* name = 0,int modal = NO_MODAL,WFlags f = 0 );
    void initial();
    virtual ~CapDialog();
    //DApplication *DApp;
    int getcapmode()        ;
    int  keycodeitem()    ;
    int keyexititem();
    int  suffixitem()     ;
    bool childcap()       ;
    int  capmunber()      ;
    int  interval()       ;
    QString imgdirname()  ;
    QString imgdirpath()  ;
    int cuemode()       ;

public slots:
    void SelectFolder();
    void setMode(int m);

protected:
    //void keyReleaseEvent (QKeyEvent *);

private:

    UTIL_PushButton *capmode;
    ZScrollView* sv;
    QComboBox *suffix, *cue;
    KeyWidget *keycodes,*keyexit;
    QCheckBox *childpixmap;
    QSpinBox *imgmunber,*intervalsec;
    QLabel *dirname, *keycodeslabel, *imgmunberlabel, *intervallabel,*keyexitlabel, *cuelabel, *suffixlabel;
    QString dg;
    int capmodeID;
};
#endif
