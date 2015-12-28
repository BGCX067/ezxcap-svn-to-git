#include "capengine.h"
#include "icon.h"
/*
*自定义键   4173
*挂机键     4145
*音乐键     4147
*亮屏键     4144
*确定键     4100
*导航上     4115
*导航左     4114
*导航下     4117
*导航右     4116
*音量-      4119
*音量+      4118
*播放键     4169
*快进键     4171
*后退键     4172
*相机键     4148
*/

CapEngine::CapEngine(ZMainWidget *mainwidget, QListBox *msgtext){

    ImgTotal=0;
    CapMode=10000;
    ChildPixmap=false;
    TmpState=false;
    KeyCode=4119;
    capmainwidget = mainwidget;
    msg=msgtext;
    tmp = new QLabel(0, "tmp");
    rect = new QLabel(tmp, "rect");
    if(!ChildPixmap)
    {
        PixmapX = 0;
        PixmapY = 0;
        PixmapW = 240;
        PixmapH = 320;
    }

}

CapEngine::~CapEngine()
{
}

void CapEngine :: initial(const QString dirname,
                          const QString dirpath,
                          int suffix,
                          int keycode,
                          int keyexit,
                          int alert,
                          int capmode,
                          int imgmunber,
                          bool childpixmap,
                          int interval)
{
    ImgDirName = dirname;
    ImgDirPath = dirpath;
    Interval = interval;
    CapAlert = alert;
    CapMode = capmode;
    ChildPixmap = childpixmap;
    ImgSuffix = suffix;
    ImgMunber = imgmunber;
    SETUP_SET_Manager mgr("/ezx_user/download/appwrite/setup/ezx_system.cfg", false);
    mgr.getSettingInfo("SYS_SYSTEM_SET", "Brightness", Brightness,25);

    KeyCode = keycode;
    KeyExit = keyexit;
    switch(CapAlert)
    {
        case 0:
            AlertButtonPixmap = QPixmap(alert_none_xpm);
            break;
        case 1:
            AlertButtonPixmap = QPixmap(alert_sound_xpm);
            break;
        case 2:
            AlertButtonPixmap = QPixmap(alert_fblight_xpm);
            break;
        case 3:
            AlertButtonPixmap = QPixmap(alert_vibrate_xpm);
            break;
    }

    //文件后缀和格式定义
    switch(ImgSuffix)
    {
        case 0:
            suffixs = ".png";
            format = "PNG";
            break;
        case 1:
            suffixs = ".jpg";
            format = "JPEG";
            break;
        case 2:
            suffixs = ".bmp";
            format = "BMP";
            break;
    }
}

void CapEngine::cap()
{
    if(CapMode==2)

    {
        toolbar->resize(0,0);
        usleep(500);
    }
    char name[20];
    QString filename;
    
    //以时间为文件名
    struct tm *newtime;
    time_t lt1;
    time( &lt1 );
    newtime=localtime(&lt1);
    strftime( name, 20, "%Y%m%d%H%M%S", newtime);

    if(CapAlert==1)
    {
        dbg_printf("alert is 1\n");
        system("/usr/SYSqtapp/phone/alertprocess -playvol 10 -playfile /usr/SYSqtapp/setup/soundfile/Hard_Key_Send.wav");
    }

    if(CapAlert==2)
    {
        dbg_printf("alert is 2\n");
        backlightctrl(true,0);
    }
    if(CapAlert==3)
    {
        dbg_printf("alert is 3\n");
        system("/usr/SYSqtapp/phone/vibrateprocess -playperiod 1");
    }

    if(!ChildPixmap){
        QFile fb("/dev/fb0");
        fb.open(IO_ReadOnly);
        QDataStream fbs(&fb);
        fbs.readRawBytes((char*)s, 240*320*3);
        fbs.~QDataStream();
        fb.close();
    }

    filename=name;
    filename+=suffixs;
    dbg_printf(filename+"\n");
    
    ZDirectPixmap *img = new ZDirectPixmap( PixmapW, PixmapH, 32);
    int i;
    unsigned char *point;
    i = PixmapY*240*3;
    for(int iy = 0; iy < PixmapH; iy ++){
      i = i+PixmapX*3;
      for ( int ix = 0; ix < PixmapW*3; ix ++ ){
    
          point = (unsigned char *)img->scanLine(iy) + ix;
    
          *point = s[i];
          i++;
      }
      i = i+(240-(PixmapX+PixmapW))*3;
    }
    img->save(ImgDirPath+"/"+filename, format);
    img->~ZDirectPixmap();
        if(CapAlert==2)backlightctrl(true,100);
    ImgTotal++;

    //添加文件列表项目
    double iconscale;
    if(PixmapW<PixmapH)
    {
        iconscale = 60./PixmapH;
    }
    else
    {
        iconscale = 60./PixmapW;
    }
    dbg_printf("scale is %d %d %d %f\n",CapMode,PixmapW, PixmapH,iconscale);
    msg->insertItem(QPixmap(ImgDirPath+"/"+filename).xForm(QWMatrix().scale(iconscale, iconscale)),filename);
    if(CapAlert==2)backlightctrl(true,Brightness*2);
    if(CapMode==2)toolbar->resize(124,30);
}

//工具条模式
void CapEngine::toolbarcap()
{
    toolbar = new CapToolBar();
    toolbar->setGeometry(60,180,124,30);
    toolbar->setPixmap(QPixmap(move_xpm));

    UTIL_PushButton *capbutton = new UTIL_PushButton(toolbar,0,24,24);
    capbutton->setNormalOverlay(QPixmap(cap_xpm));
    capbutton->setGeometry(0,3,24,24);
    QObject::connect( capbutton, SIGNAL(clicked()), this, SLOT(cap()) );

    alertbutton = new UTIL_PushButton(toolbar,0,24,24);
    alertbutton->setNormalOverlay(AlertButtonPixmap);
    alertbutton->setGeometry(24,3,24,24);
    QPopupMenu *alertmenu=new QPopupMenu(alertbutton);
    alertmenu->insertItem(QPixmap(alert_none_xpm),0);
    alertmenu->insertItem(QPixmap(alert_sound_xpm),1);
    alertmenu->insertItem(QPixmap(alert_fblight_xpm),2);
    alertmenu->insertItem(QPixmap(alert_vibrate_xpm),3);
    alertbutton->setPopup(alertmenu);
    QObject::connect( alertmenu, SIGNAL(activated (int)), this, SLOT(alerttag(int)) );

    suffixbutton = new UTIL_PushButton(toolbar,0,24,24);
    suffixbutton->setNormalOverlay(NULL);
    suffixbutton->setGeometry(24*2,3,48,24);
    suffixbutton->setText(format.left(1));
    QPopupMenu *suffixmenu=new QPopupMenu(alertbutton);
    suffixmenu->insertItem("png",0);
    suffixmenu->insertItem("jpg",1);
    suffixmenu->insertItem("bmp",2);
    suffixbutton->setPopup(suffixmenu);
    QObject::connect( suffixmenu, SIGNAL(activated (int)), this, SLOT(setsuffix(int)) );

    UTIL_PushButton *exitbutton = new UTIL_PushButton(toolbar,0,24,24);
    exitbutton->setNormalOverlay(QPixmap(exit_xpm));
    exitbutton->setGeometry(24*3,3,24,24);
    QObject::connect( exitbutton, SIGNAL(clicked()), this, SLOT(beforeterminate()) );

    toolbar->show();
}

void CapEngine::alerttag(int n)
{
    switch(n)
    {
        case 0:
            AlertButtonPixmap = QPixmap(alert_none_xpm);
            break;
        case 1:
            AlertButtonPixmap = QPixmap(alert_sound_xpm);
            break;
        case 2:
            AlertButtonPixmap = QPixmap(alert_fblight_xpm);
            break;
        case 3:
            AlertButtonPixmap = QPixmap(alert_vibrate_xpm);
            break;
    }
    alertbutton->setNormalOverlay(AlertButtonPixmap);
    CapAlert = n;
}

void CapEngine::setsuffix(int n)
{
    switch(n)
    {
        case 0:
            suffixs = ".png";
            format = "PNG";
            suffixbutton->setText("P");
            break;
        case 1:
            suffixs = ".jpg";
            format = "JPEG";
            suffixbutton->setText("J");
            break;
        case 2:
            suffixs = ".bmp";
            format = "BMP";
            suffixbutton->setText("B");
            break;
    }
    ImgSuffix = n;
}

//间隔捕捉
void CapEngine::intervalcap()
{
    for(int mun=0;mun<ImgMunber;mun++){
        if(Interval!=0){
            sleep(Interval-3);
        }
        cap();
    }
    beforeterminate();
}

//按下
void CapEngine :: pointerPressed( int x, int y )
{
    if(ChildPixmap){
        PixmapOriginX=x;
        PixmapOriginY=y;
        dbg_printf("pressed %d %d !\n", x, y);
    }

}

//拖曳
void CapEngine :: pointerDrag( int x, int y )
{
    if(ChildPixmap){
        if(x>PixmapOriginX)
        {
            PixmapX = PixmapOriginX;
            PixmapW=x-PixmapOriginX;
        }else{
            PixmapX = x;
            PixmapW=PixmapOriginX-x;
        }

        if(y>PixmapOriginY)
        {
            PixmapY = PixmapOriginY;
            PixmapH=y-PixmapOriginY;
        }else{
            PixmapY = y;
            PixmapH=PixmapOriginY-y;
        }
        dbg_printf("rect %d %d %d %d !\n", PixmapX, PixmapY, PixmapW, PixmapH);
        rect->setGeometry(PixmapX, PixmapY, PixmapW, PixmapH);
        rect->setText(QString::number(PixmapW)+"*"+QString::number(PixmapH));

    }

}

//释放
void CapEngine :: pointerReleased( int x, int y )
{
    if(ChildPixmap){

        dbg_printf("released %d %d !\n", x, y);
        if(tmp->close()){
            TmpState = true;
        }
        cap();

    }

}

//快捷键捕捉
void CapEngine :: keyPressed(int keycode)
{
    int maskXbit = 3;
    bool maskX = true;
    bool maskY = true;

    if(CapMode==0){
        if(KeyCode == keycode) {

            if(ChildPixmap){

                QFile fb("/dev/fb0");
                fb.open(IO_ReadOnly);
                
                QDataStream fbs(&fb);
                fbs.readRawBytes((char*)s, 240*320*3);
                fbs.~QDataStream();
                fb.close();

                ZDirectPixmap *img = new ZDirectPixmap( 240, 320, 32);
                int i;
                unsigned char *point;
                i = 0;
                for(int iy = 0; iy < 320; iy ++){
                  for ( int ix = 0; ix < 240*3; ix ++ ){
                
                      point = (unsigned char *)img->scanLine(iy) + ix;
                
                      *point = s[i];
                      i++;
                  }
                }

                ZDirectPixmap *img1 = new ZDirectPixmap( 240, 320, 32);
                int i1;
                unsigned char *point1;
                i1 = 0;
                for(int iy = 0; iy < 320; iy ++){
                  if(maskY)
                  {
                      maskX = false;
                      maskY = false;
                      maskXbit = 0;
                  }else{
                      maskX = true;
                      maskY = true;
                      maskXbit = 3;
                  }
                  for ( int ix = 0; ix < 240*3; ix ++ ){

                      point1 = (unsigned char *)img1->scanLine(iy) + ix;
                      if(maskX)
                      {
                          *point1 = s[i1];
                          maskXbit--;

                          if(maskXbit == 0)
                          {
                              maskX = false;
                          }
                      }else{
                          *point1 = (Q_UINT8)0xF0;
                          maskXbit++;

                          if(maskXbit == 3)
                          {
                              maskX = true;
                          }
                      }
                      i1++;
                  }
                }


                tmp->setGeometry(0, 0, 240, 320);
                tmp->setBackgroundPixmap(*img1);

                rect->setGeometry(0, 0, 0, 0);
                rect->setBackgroundPixmap(*img);
                rect->setBackgroundOrigin(QWidget::ParentOrigin);
                rect->setAlignment(Qt::AlignCenter);

                tmp->show();
                img->~ZDirectPixmap();
                img1->~ZDirectPixmap();

            }else{

                cap();

            }
        }
        if(KeyExit == keycode) {

            beforeterminate();

        }
    }
}

void CapEngine :: keyPresslong(int keycode)
{
    if(CapMode==0){
        if(KeyCode == keycode)
        {

            RES_ICON_Reader ir;
            QPixmap pm("Dialog_Complete.gif");
            pm=ir.getIcon("Dialog_Complete.gif",true);
            if(ChildPixmap)
            {
                //全屏捕捉开启
                ZMessageBox::timerMessage(0, pm,tr("Full-screen capture is open"),1);
                ChildPixmap = false;
                PixmapX = 0;
                PixmapY = 0;
                PixmapW = 240;
                PixmapH = 320;
                dbg_printf("ok\n");
            }else{
                //局部捕捉开启
                ZMessageBox::timerMessage(0, pm,tr("Area capture is open"),1);
                ChildPixmap = true;
            }
    
        }
    }
}
//屏幕背光控制
void CapEngine :: backlightctrl(bool onoff,int brightness)
{
    int fbd = open("/dev/fb0", O_RDWR);
    if(fbd<0){
    dbg_printf("can not open fb0\n");
    return;
    }
    if(onoff){
        ioctl(fbd,FBIOBLANK,0);
        ioctl(fbd,FBIOSETBRIGHTNESS,brightness);
        ioctl(fbd,FBIOSETBKLIGHT,onoff);
    }else{
        ioctl(fbd,FBIOSETBRIGHTNESS,0);
        ioctl(fbd,FBIOSETBKLIGHT,onoff);
        ioctl(fbd,FBIOBLANK,0x3);
    }
    close(fbd);

    dbg_printf("Set backlight %s\n",onoff == true ?  "ON" : "OFF");
    keylightctrl(onoff);
}
//键盘灯控制
void CapEngine :: keylightctrl(bool onoff)
{
    int keyd = open("/dev/keylight", O_RDWR);
    if(keyd<0){
    dbg_printf("can not open keylight device\n");
    return;
    }
    if(onoff){
        ioctl(keyd,0xf0,0);
    }else{
        ioctl(keyd,0xf2,0);
    }
    close(keyd);

    dbg_printf("Set keylight %s\n",onoff == true ?  "ON" : "OFF");
}

void CapEngine :: QCopReceived(int message)
{
    dbg_printf("QCopReceived %d\n", message);
}

void CapEngine :: PropertyReceived( )
{
}

void CapEngine :: beforeterminate( )
{
    if(CapMode==2)delete toolbar;
    dbg_printf("before terminate\n");
    ChildPixmap=false;
    capmainwidget->show();
}
//////////////////////////////////////////////////////
//CapToolBar
//////////////////////////////////////////////////////
CapToolBar::CapToolBar():
            QLabel(0,"mytoolbar",WStyle_Customize|WStyle_StaysOnTop)
{
    setBackgroundMode(PaletteBackground);
    setPalette(QPalette( QColorGroup::Background , QColor( 148, 166, 204) ) );
}

void CapToolBar::mouseMoveEvent(  QMouseEvent *e )
{
    this->move(e->globalX()-110,e->globalY()-12);
    update(0,0,240,50);
}

