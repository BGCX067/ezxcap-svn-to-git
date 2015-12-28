#include "emainwidget.h"

EMainWidget::EMainWidget(const QString& title, bool editorMode, QWidget* parent,const char* name,WFlags f):
             ZMainWidget(title,editorMode,parent,name,f)
{
    setGeometry(ZGlobal::getHomeR());
}

void EMainWidget::about(){

    dbg_printf("about \n");
    ZMainWidget *about = new ZMainWidget(tr("About..."),false,0,"about");
    UTIL_CST *aboutCst = new UTIL_CST(about, tr("OK"),EZX_CST_BTN_MENU,"CST_Back");
    QTextView *aboutContent = new QTextView(about, "aboutContent");
               aboutContent->setFont(QFont::QFont("", 12, 75));
               aboutContent->setHScrollBarMode(QScrollView::AlwaysOff);
               aboutContent->setVScrollBarMode(QScrollView::AlwaysOff);
               aboutContent->setPaper(QBrush(QColor( 148, 166, 204),QPixmap("Logos.jpg")));
               aboutContent->setText("<p align=\"center\"><big>eCap for E6/A1200</big></p>"
                                          "<hr/>"
                                          "<p>This software is free software, you can free dissemination and use. Prohibited any form of commercial uses, including bundled in paid products.<br/><br/>"
                                          "Thank Yan0, Foxe6 such as the contribution. To thank my wife and my family to my encouragement and support!<br/>"
                                          "<a href='mailto:embont@gmail.com'>embont@gmail.com</a><br/><a href='http://www.dnxh.cn'>http://www.dnxh.cn</a></p>");
    about->setContentWidget(aboutContent);
    about->setCSTWidget(aboutCst);
    QObject::connect( aboutCst->getMidBtn(), SIGNAL(clicked()), about, SLOT(close()));
    QObject::connect( aboutCst->getRightBtn(), SIGNAL(clicked()), about, SLOT(close()));
    about->show();

}
