#include "ecap_dnxh.h"
char APPDIR[55];

int main(int argc, char** argv)
{
    printf("Ecap by www.dnxh.cn %s %s, %s \n",VERSION,__TIME__,__DATE__);
    strncpy(APPDIR,argv[0],strlen(argv[0])-9);
    dbg_printf("%s\n",argv[0]);
    QDir::setCurrent(APPDIR);

    DApplication app (argc, argv);

    // translation file for application strings
    QTranslator myapp( 0 );
    myapp.load( QString( "ecap." )+ZLanguage::getSystemLanguageCode(), "." );
    app.installTranslator( &myapp );
    //printf("%s\n",ZLanguage::getSystemLanguageCode().latin1());

    //主窗口
    EMainWidget *w =new EMainWidget(QObject::tr("%1 - screen capture").arg("eCap"), false, 0, "w");
    w->setGeometry(ZGlobal::getHomeR());
    //w->setFont(QFont::QFont("", 15, 75));

    //内容
    QListBox *lbox = new QListBox(w,NULL,0);
    //lbox->setSelectionMode(QListBox::Multi);
    //ZApplication::setMouseMode(lbox,ZApplication::PRESS_HOLD);
    QObject::connect( lbox, SIGNAL(clicked( QListBoxItem *,const QPoint &)), &app, SLOT(OpenFile( QListBoxItem *,const QPoint &)) );

    //CST
    UTIL_CST *capcst = new UTIL_CST(w, " ",EZX_CST_BTN_MENU,"CST_Exit");
    ZPushButton *menu = capcst->getLeftBtn();
    QPopupMenu *file=new QPopupMenu(menu);
    file->insertItem(QObject::tr("About..."),w,SLOT(about()));
    menu->setPopup( file );

    ZPushButton *opendir = capcst->getMidBtn();
    opendir->setEnabled(false);

    ZPushButton *quitbutton = capcst->getRightBtn();
    QObject::connect( quitbutton, SIGNAL(clicked()), &app, SLOT(quit()) );

    w->setContentWidget(lbox);
    w->setCSTWidget(capcst);

    //事件引擎
    CapEngine *engine = new CapEngine( w, lbox);
    app.setPointerListener( (PointerListener*) engine );

//启动画面
    QLabel *logo = new QLabel(0, "logo");
    logo->setBackgroundPixmap(QPixmap("Logo.jpg"));
    //logo->setFont(QFont::QFont("", 12, 75));
    logo->setPalette( QPalette( QColor( 148, 166, 204) ) );
    QLabel *vertext = new QLabel(logo,"ver");
    vertext->setText(QString("the screen capture tool\n\n%1 %2 %3\n(c)2008 www.dnxh.cn").arg(VERSION).arg(__TIME__).arg(__DATE__));
    vertext->setFont(QFont::QFont("", 12, 75));
    vertext->setGeometry(0, 235, 240, 80);
    vertext->setBackgroundPixmap(QPixmap("Logo.jpg"));
    vertext->setBackgroundOrigin(QWidget::ParentOrigin);
    vertext->setAlignment(Qt::AlignCenter);
    logo->showFullScreen();
    QTimer::singleShot( 3*1000, logo, SLOT(close()));

//参数对话框
    CapDialog *dlg = new CapDialog(0, "dlg", true);
    dlg->setGeometry(ZGlobal::getHomeR());

    if(dlg->exec() == QDialog::Accepted ){
        dbg_printf("save set keyitem %d capmunber %d int %d \n",dlg->keycodeitem(), dlg->capmunber(), dlg->interval());
        app.setDirPath(dlg->imgdirpath());

        SETUP_SET_Manager mgr("ecap.cfg", false);
        mgr.setSettingInfo("ECAP_SET", "dirname", dlg->imgdirname(), false);
        mgr.setSettingInfo("ECAP_SET", "dirpath", dlg->imgdirpath(), false);
        mgr.setSettingInfo("ECAP_SET", "suffix", dlg->suffixitem());
        mgr.setSettingInfo("ECAP_SET", "capkey", dlg->keycodeitem());
        mgr.setSettingInfo("ECAP_SET", "exitkey", dlg->keyexititem());
        mgr.setSettingInfo("ECAP_SET", "cuemode", dlg->cuemode());
        mgr.setSettingInfo("ECAP_SET", "capmode", dlg->getcapmode());
        mgr.setSettingInfo("ECAP_SET", "interval", dlg->interval());
        mgr.setSettingInfo("ECAP_SET", "capmunber", dlg->capmunber());
        mgr.setSettingInfo("ECAP_SET", "childcap", dlg->childcap());

        if(dlg->getcapmode()==0){
            engine->initial(dlg->imgdirname(),
                            dlg->imgdirpath(),
                            dlg->suffixitem(),
                            dlg->keycodeitem(),
                            dlg->keyexititem(),
                            dlg->cuemode(),
                            dlg->getcapmode(),
                            1,
                            dlg->childcap());
        }

        if(dlg->getcapmode()==1){
            engine->initial(dlg->imgdirname(),
                            dlg->imgdirpath(),
                            dlg->suffixitem(),
                            dlg->keycodeitem(),
                            dlg->keyexititem(),
                            dlg->cuemode(),
                            dlg->getcapmode(),
                            dlg->capmunber(),
                            false,
                            dlg->interval());
            engine->intervalcap();
        }
        if(dlg->getcapmode()==2){
            engine->initial(dlg->imgdirname(),
                            dlg->imgdirpath(),
                            dlg->suffixitem(),
                            dlg->keycodeitem(),
                            dlg->keyexititem(),
                            dlg->cuemode(),
                            dlg->getcapmode(),
                            dlg->capmunber(),
                            false,
                            dlg->interval());
            engine->toolbarcap();
        }
    }else{
        exit(0);
    }
    dlg->~CapDialog();
    return app.exec();
}

