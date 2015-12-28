#ifndef _EMAINWIDGET_H_
#define _EMAINWIDGET_H_

#include "dapplication.h"

class EMainWidget : public ZMainWidget
{
    Q_OBJECT
    public:
        EMainWidget(const QString& title, bool editorMode, QWidget* parent,const char* name = 0,WFlags f = 0 );

    public slots:
        void about();
    private:
        //QListBox *lbox;

};

#endif
