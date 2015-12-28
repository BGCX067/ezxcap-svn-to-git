#include "capdialog.h"

CapDialog :: CapDialog(QWidget *parent,const char* name,int modal,WFlags f):
             ZBaseDialog(parent, name, modal, f)
{
    //setFont(QFont::QFont("", 14, 75));

    sv = new ZScrollView(this, "sv");
    sv->setGeometry(ZGlobal::diffRect(ZGlobal::getHomeR(), ZGlobal::getContentR()));
    sv->setFont(QFont::QFont("", 14, 75));

    capmode = new UTIL_PushButton(sv->viewport(),0,0,35);
    QPopupMenu *capmodemenu=new QPopupMenu(sv->viewport());
    capmodemenu->setMinimumWidth(100);
    capmodemenu->insertItem(tr("Key"),0);
    capmodemenu->insertItem(tr("Interval"),1);
    capmodemenu->insertItem(tr("Bar"),2);
    capmode->setPopup(capmodemenu);
    QObject::connect( capmodemenu, SIGNAL(activated (int)), this, SLOT(setMode(int)) );

    keycodeslabel = new QLabel(tr("KeyCodes"), sv->viewport(), "keycodeslabel");
    keycodes = new KeyWidget("",sv->viewport(), "keycodes");

    keyexitlabel = new QLabel(tr("ExitCodes"), sv->viewport(), "keycodeslabel");
    keyexit = new KeyWidget("", sv->viewport(), "keyexit");

    childpixmap = new QCheckBox(tr("Shot area"), sv->viewport(), "childpixmap");

    imgmunberlabel = new QLabel(tr("Image numbers"), sv->viewport(), "imgmunberlabel");
    intervallabel = new QLabel(tr("Interval, sec"), sv->viewport(), "intervallabel");
    imgmunber = new QSpinBox(sv->viewport(), "imgmunber");
    intervalsec = new QSpinBox(sv->viewport(), "interval");
    imgmunber->setMinValue(1);
    intervalsec->setMinValue(4);

    QLabel *dirnamelabel = new QLabel(tr("DirName"), sv->viewport(), "dirnamelabel");
    dirname = new QLabel("",sv->viewport(), "dirname");
    dirname->setAlignment( int(QLabel::WordBreak) );
    ZPushButton *dirbutton = new ZPushButton("FMMS_FolderTree",  sv->viewport());
    dirbutton->resize(24, 24);
    QObject::connect( dirbutton, SIGNAL(clicked()), this, SLOT(SelectFolder()) );

    suffixlabel = new QLabel(tr("Suffix"), sv->viewport(), "suffixlabel");
    suffix = new QComboBox(sv->viewport(), "suffix");
    suffix->insertItem("*.png");
    suffix->insertItem("*.jpg");
    suffix->insertItem("*.bmp");
        
    cuelabel = new QLabel(tr("Hint"), sv->viewport(), "cuelabel");
    cue = new QComboBox(sv->viewport(), "cue");
    cue->insertItem(tr("None"));
    cue->insertItem(tr("Alert"));
    cue->insertItem(tr("Flash"));
    cue->insertItem(tr("Vibrate"));
    
    QHBoxLayout *keycodesbox = new QHBoxLayout(1);
    keycodesbox->addWidget(keycodeslabel);
    keycodesbox->addWidget(keycodes);

    QHBoxLayout *keyexitbox = new QHBoxLayout(1);
    keyexitbox->addWidget(keyexitlabel);
    keyexitbox->addWidget(keyexit);
        
    QHBoxLayout *imgmunberbox = new QHBoxLayout(1);
    imgmunberbox->addWidget(imgmunberlabel);
    imgmunberbox->addWidget(imgmunber);
    
    QHBoxLayout *intervalbox = new QHBoxLayout(1);
    intervalbox->addWidget(intervallabel);
    intervalbox->addWidget(intervalsec);
    
    QHBoxLayout *dirnamebox = new QHBoxLayout(1);
    dirnamebox->addWidget(dirnamelabel);
    dirnamebox->addStretch(1);
    dirnamebox->addWidget(dirname);
    dirnamebox->addWidget(dirbutton);
    
    QHBoxLayout *suffixbox = new QHBoxLayout(1);
    suffixbox->addWidget(suffixlabel);
    suffixbox->addWidget(suffix);

    QHBoxLayout *cuebox = new QHBoxLayout(1);
    cuebox->addWidget(cuelabel);
    cuebox->addWidget(cue);
    
    QVBoxLayout *setbox = new QVBoxLayout(sv->viewport(), 5, 5);
    setbox->addWidget(capmode);
    setbox->addLayout(keycodesbox);
    setbox->addLayout(keyexitbox);
    setbox->addWidget(childpixmap);
    setbox->addLayout(imgmunberbox);
    setbox->addLayout(intervalbox);
    setbox->addStretch(5);
    setbox->addLayout(dirnamebox);
    setbox->addLayout(suffixbox);
    setbox->addLayout(cuebox);

    sv->addChild(capmode);
    sv->addChild(keycodeslabel);
    sv->addChild(keycodes);
    sv->addChild(keyexitlabel);
    sv->addChild(keyexit);
    sv->addChild(childpixmap);
    sv->addChild(imgmunberlabel);
    sv->addChild(imgmunber);
    sv->addChild(intervallabel);
    sv->addChild(intervalsec);
    sv->addChild(suffixlabel);
    sv->addChild(suffix);
    sv->addChild(dirnamelabel);
    sv->addChild(dirname);
    sv->addChild(dirbutton);
    sv->addChild(cue);
    sv->addChild(cuelabel);

    //°´Å¥
    ZPushButton *keep = new ZPushButton("keep", tr("Shot"), this);
    keep->setGeometry(ZGlobal::diffRect(ZGlobal::getHomeR(), ZGlobal::getCst2a_1R()));
    
    ZPushButton *cancel = new ZPushButton("cancel", tr("Exit"), this);
    cancel->setGeometry(ZGlobal::diffRect(ZGlobal::getHomeR(), ZGlobal::getCst2a_2R()));
    
    QObject::connect( capmode, SIGNAL(clicked(int)), this, SLOT(setMode(int)) );
    QObject::connect( keep, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect( cancel, SIGNAL(clicked()), this, SLOT(reject()));

    initial();
}

void CapDialog :: initial()
{
    imgmunber->setMinValue(1);
    intervalsec->setMinValue(1);

    QString name;
    int suf,key,inte,capm,ekey,mode,cuet;
    bool ccap;
    SETUP_SET_Manager mgr("ecap.cfg", false);
    mgr.getSettingInfo("ECAP_SET", "dirname", name, tr("MMC/SD"), false);
    mgr.getSettingInfo("ECAP_SET", "dirpath", dg, "/mmc/mmca1", false);
    mgr.getSettingInfo("ECAP_SET", "suffix", suf, 0);
    mgr.getSettingInfo("ECAP_SET", "capkey", key, 4119);
    mgr.getSettingInfo("ECAP_SET", "exitkey", ekey, 4118);
    mgr.getSettingInfo("ECAP_SET", "cuemode", cuet, 2);
    mgr.getSettingInfo("ECAP_SET", "capmode", mode, 0);
    mgr.getSettingInfo("ECAP_SET", "interval", inte, 2);
    mgr.getSettingInfo("ECAP_SET", "capmunber", capm ,2);
    mgr.getSettingInfo("ECAP_SET", "childcap", ccap, false);

    //capmode->setButton(mode);
    setMode(mode);
    keycodes->setKey(key);
    keycodes->setText(keycodes->C2S(key));
    keyexit->setKey(ekey);
    keyexit->setText(keyexit->C2S(ekey));
    if(ccap)
    {
        childpixmap->setChecked(true);
    }else{
        childpixmap->setChecked(false);
    }
    imgmunber->setValue(capm);
    intervalsec->setValue(inte);
    dirname->setText(name);
    suffix->setCurrentItem(suf);
    cue->setCurrentItem(cuet);
}

CapDialog::~CapDialog()
{
    dbg_printf("exit Dialog\n");

        suffix->~QComboBox();
        cue->~QComboBox();
        childpixmap->~QCheckBox();
        imgmunber->~QSpinBox();
        intervalsec->~QSpinBox();
        dirname->~QLabel();
        keycodeslabel->~QLabel();
        imgmunberlabel->~QLabel();
        intervallabel->~QLabel();
        keyexitlabel->~QLabel();
        cuelabel->~QLabel();
        suffixlabel->~QLabel();
        sv->~ZScrollView();
}

int CapDialog :: getcapmode()
{
    return capmodeID;
}

int  CapDialog :: keycodeitem()
{
    return keycodes->getKey();
}

int  CapDialog :: keyexititem()
{
    return keyexit->getKey();
}

int  CapDialog :: suffixitem()
{
    return suffix->currentItem();
}

bool CapDialog :: childcap()
{
    return childpixmap->isChecked();
}

int  CapDialog :: capmunber()
{
    return imgmunber->value();
}

int  CapDialog :: interval()
{
    return intervalsec->value();
}

QString CapDialog :: imgdirname()
{
    //printf("open %s \n",(dirpath->text()).latin1());
    return dirname->text();
}

QString CapDialog :: imgdirpath()
{
    //printf("open %s \n",(dirpath->text()).latin1());
    return dg;
}

int CapDialog :: cuemode()
{
    return cue->currentItem();
}

void CapDialog :: SelectFolder(){
    ZSelectFolderDlg *sfd = new ZSelectFolderDlg("/mmc/mmca1/");
    QString dn;
    if(sfd->exec() == QDialog::Accepted){
        dg = sfd->getSelected(&dn);
        dirname->setText(dn);
    }
    dbg_printf("open %s \n",dg.latin1());
}

void CapDialog :: setMode(int m)
{
    dbg_printf("mode is %d\n",m);
    capmodeID=m;
    switch(m)
    {
        case 0:
            imgmunberlabel->hide();
            intervallabel->hide();
            imgmunber->hide();
            intervalsec->hide();
            keycodeslabel->show();
            keycodes->show();
            keyexitlabel->show();
            keyexit->show();
            childpixmap->show();
            capmode->setText(tr("Key"));
        break;
        case 1:
            imgmunberlabel->show();
            intervallabel->show();
            imgmunber->show();
            intervalsec->show();
            keycodeslabel->hide();
            keycodes->hide();
            keyexitlabel->hide();
            keyexit->hide();
            childpixmap->hide();
            capmode->setText(tr("Interval"));
        break;
        case 2:
            imgmunberlabel->hide();
            intervallabel->hide();
            imgmunber->hide();
            intervalsec->hide();
            keycodeslabel->hide();
            keycodes->hide();
            keyexitlabel->hide();
            keyexit->hide();
            childpixmap->hide();
            capmode->setText(tr("Bar"));
        break;
    }
}
