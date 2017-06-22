#include <time.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace UnitHolidays;
using namespace UnitDeyPlan;
using namespace UnitFormPlayList;

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent, Controller *pdata) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings( new SettingsDialog(parent) ),
    settingsWifi( new SettingDialogWifi( parent ) ),
    statusBar(new QLabel(parent)),
    ptrController(pdata)
{
    ui->setupUi(this);
    //create
    createStatusBar();
    createToolBar();
    createToolTip();
    createConnectionGUI();
    createGroupMenu();
    createGroupButton();
    createGroupSlider();
    createPlans(ui->planWidget);
    createHolidays(ui->wHoliday);
    createPlayer(ui->gbPlayTrack);
    createFormPlayList(ui->widgetTrack);
    createStyle();
    // install wigen
    installDiagnosisPage();
    //Read setting in register
    readSettings();
    // database to GUI
    updateDataToGui();
    // install structure datatime
    ptrController->setDateTime(ui->dateTimeEditConfig->dateTime());
    ptrController->setDateTimeZone(ui->cbGMT->currentIndex());
    // visibled
    ui->lbAllTime->setVisible(false);
    ui->chbDayLight->setVisible(false);
    //test
//    QDateTime test(ui->dateTimeEditConfig->dateTime());
//    qDebug()<<test<<test.currentDateTimeUtc().toTime_t()<<test.currentDateTimeUtc().toMSecsSinceEpoch();
//    QDateTime test2;
//    test2.setTime_t(test.currentDateTimeUtc().toTime_t());
//    qDebug()<<test2;
//    QDateTime test3;
//    test3.setTime_t(test.currentDateTimeUtc().toMSecsSinceEpoch()/1000);
//    qDebug()<<test3;
//    qDebug()<<test3.toTimeSpec(Qt::UTC);

    ///////////////
    /*
    time_t rawtime;
    struct tm * ptm;
    //ptm = localtime( &rawtime );
    QDateTime test(QDateTime::currentDateTime());
    ptm->tm_hour = test.time().hour();
    ptm->tm_min = test.time().minute();
    ptm->tm_sec = test.time().second();
    ptm->tm_year = test.date().year()-1900;
    ptm->tm_mon = test.date().month();
    ptm->tm_mday = test.date().day();
    ptm->tm_yday = test.date().dayOfYear();
    ptm->tm_wday = test.date().dayOfWeek();

    rawtime = mktime( ptm );
    test.setTime_t(rawtime);qDebug()<<test;

    qDebug()<<rawtime;
    qDebug()<<ptm->tm_year<<ptm->tm_mon<<ptm->tm_mday<<ptm->tm_wday<<ptm->tm_yday;
    qDebug()<<ptm->tm_hour<<ptm->tm_min<<ptm->tm_sec<<ptm->tm_isdst;
    time_t raw = mktime( ptm );
    struct tm * ptm1;
    ptm1 = gmtime( &raw );
    qDebug()<<raw;
    qDebug()<<ptm1->tm_year<<ptm1->tm_mon<<ptm1->tm_mday<<ptm1->tm_wday<<ptm1->tm_yday;
    qDebug()<<ptm1->tm_hour<<ptm1->tm_min<<ptm1->tm_sec<<ptm1->tm_isdst;

    QDateTime test4;test4.setTime_t(raw);
    qDebug()<<test4;
    */
//    uint64_t time(0x59439ADC);
//    const QString strtime(
//                QDateTime::fromTime_t(time,QTimeZone::systemTimeZone()).toString("dd.MM.yyyy hh:mm"));
//    qDebug()<<strtime;
    //    // test time
//    QTime myTimer;
//    myTimer.start();
//    int nMilliseconds = myTimer.elapsed();
//    qDebug()<<nMilliseconds;
}
/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}
/**
 * @brief MainWin::writeSettings
 */
void MainWindow::writeSettings()
{
    Settings::set(Settings::GEOMETRY) = saveGeometry();
    Settings::set(Settings::IP,Settings::NETWORK) = settingsWifi->getIP();
    Settings::set(Settings::PORT,Settings::NETWORK) = settingsWifi->getPORT();
    Settings::set(Settings::COMNAME,Settings::COMPORT) = settings->getComName();
    Settings::set(Settings::BAUDRATE,Settings::COMPORT) = settings->getBaudRate();
    Settings::set(Settings::DATABITS,Settings::COMPORT) = settings->getDataBits();
    Settings::set(Settings::STOPBITS,Settings::COMPORT) = settings->getStopBits();
    Settings::set(Settings::PARITY,Settings::COMPORT) = settings->getParity();
    Settings::set(Settings::FLOWCONTROL,Settings::COMPORT) = settings->getFlowControl();
}
/**
 * @brief MainWin::readSettings
 */
void MainWindow::readSettings()
{
    // set default
    QFile file(":/config/default.ini");
    file.open(QIODevice::ReadOnly);
    Settings::setDefaults(file.readAll());
    // read
    const QByteArray gemData = Settings::get(Settings::GEOMETRY).toByteArray();
    restoreGeometry(gemData);
    // Ethrenet
    const QString ip = Settings::get(Settings::IP,Settings::NETWORK).toString();
    if (!ip.isEmpty())
        settingsWifi->setIP(ip);
    const QString port = Settings::get(Settings::PORT,Settings::NETWORK).toString();
    if (!ip.isEmpty())
        settingsWifi->setPORT(port);
    settingsWifi->updateParametrs();//
    // COM PORT
    const QString comname = Settings::get(Settings::COMNAME,Settings::COMPORT).toString();
    if (!comname.isEmpty())
        settings->setComName(comname);
    const QString baudRate = Settings::get(Settings::BAUDRATE,Settings::COMPORT).toString();
    if (!baudRate.isEmpty())
        settings->setBaudRate(baudRate);
    const QString databits = Settings::get(Settings::DATABITS,Settings::COMPORT).toString();
    if (!databits.isEmpty())
        settings->setDataBits(databits);
    const QString stopbits = Settings::get(Settings::STOPBITS,Settings::COMPORT).toString();
    if (!stopbits.isEmpty())
        settings->setStopBits(stopbits);
    const QString parity = Settings::get(Settings::PARITY,Settings::COMPORT).toString();
    if (!parity.isEmpty())
        settings->setParity(parity);
    const QString flowcnt = Settings::get(Settings::FLOWCONTROL,Settings::COMPORT).toString();
    if (!flowcnt.isEmpty())
        settings->setFlowControl(flowcnt);
    settings->updateParametrs();
}
/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    // delete form output
    delete settings;
    delete settingsWifi;
    if(mainWindowFormMap[ui->actionWin])
        delete mainWindowFormMap[ui->actionWin];
    event->accept();
}
/**
 * @brief MainWindow::createToolTip
 */
void MainWindow::createToolTip()
{
    ui->actionWrite->setToolTip(tr("Write all to the UZTVOP"));
    ui->actionRead->setToolTip(tr("Read all from the UZTVOP"));
    ui->actionWrite_Setting->setToolTip(tr("Write settings to the UZTVOP"));
    ui->actionRead_Setting->setToolTip(tr("Read settings from the UZTVOP"));
    ui->actionDevice->setToolTip(tr("Get the type of device."));
}
/**
 * @brief MainWindow::createToolBar
 */
void MainWindow::createToolBar()
{
    // add ListCommand
    ListCommand<<ui->actionWrite<<ui->actionRead<<ui->actionWrite_Setting<<ui->actionRead_Setting;
    ui->mainToolBar->addActions(ListCommand);
    ui->actionDevice->setVisible(false); //
    ui->mainToolBar->addSeparator();
    // add ListView
    ListView<<ui->actionSetting<<ui->actionDiagnosis;
    ui->mainToolBar->addActions(ListView);
    ui->mainToolBar->addSeparator();
    // add ListConnect
    ListConnect<<ui->actionConnect_USB<<ui->actionConnect_WIFI;
    ui->actionConnect_USB->setChecked(true);// choose COM PORT
    ui->mainToolBar->addActions(ListConnect);
    ui->mainToolBar->addSeparator();
    defaultChooseConnect();
//    wificon = new QLabel(this);
//       //wificon->setWindowIcon(QPixmap(":/ico/win48x48.ico"));
//       //QString str="<img src=\":/ico/win48x48.ico\"> Открытые ссылки";
//       //wificon->setTextFormat(Qt::RichText);
//       //wificon->setText(str);
//    ui->mainToolBar->addWidget(wificon);
}
/**
 * @brief MainWindow::createStatusBar
 */
void MainWindow::createStatusBar()
{
    /*Add progress in status bar*/
    progress = new QProgressBar(this);
    progress->setVisible(false);
    progress->setTextVisible(true);
    connect(ptrController,SIGNAL(signalProgressRange(int,int)),progress,SLOT(setRange(int,int)),Qt::DirectConnection);
    connect(ptrController,SIGNAL(signalProgressValue(int,bool)),this,SLOT(onSetValueProgress(int,bool)),Qt::DirectConnection);
    ui->statusBar->addPermanentWidget(progress,0);
    /*Set message in status bar*/
    QLabel *p = new QLabel(this);
    p->setText(tr("version 1.1"));
    p->setOpenExternalLinks(true);
    ui->statusBar->addPermanentWidget(p);
    //statusBar->setText(tr("Sound Configurator is open."));
    ui->statusBar->addWidget(statusBar);
    ui->statusBar->showMessage(tr("Sound Configurator is open."),2000);
}
/**
 * @brief MainWindow::createStyle
 */
void MainWindow::createStyle()
{
    /*set Style Sweet the windows and button*/
    QFile styleFile(":/QSS/style_files001.qss");
    bool result = styleFile.open(QFile::ReadOnly);
    if(result){
      QByteArray style = styleFile.readAll();
      qApp->setStyleSheet(style);
    }
}
/**
 * @brief MainWindow::createConnectionGUI
 */
void MainWindow::createConnectionGUI()
{
    /*set slot GUI*/
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(onActionabout())); // about
    // toolbar
    connect(ui->mainToolBar,SIGNAL(visibilityChanged(bool)),this,SLOT(onToolBar(bool)));
    connect(ui->actionShow_Toolbar,SIGNAL(triggered(bool)),this,SLOT(onToolBar(bool)));
    // windows out
    connect(ui->actionWin,SIGNAL(triggered(bool)),this,SLOT(onWindowsOut()));
    //connect view panel
    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(signalMapper,SIGNAL(mapped(int)),this,SLOT(onSwitchPanelMode(int)));
    for (int i = 0; i <ListView.size(); i++)
    {
        signalMapper->setMapping(ListView.at(i), i);
        connect(ListView.at(i), SIGNAL(triggered()), signalMapper,SLOT(map()));
    }
    // connect add plan
    connect(ui->pbAddPlan,SIGNAL(clicked(bool)),this,SLOT(onAddPlan()));
    connect(ui->pbRemotePlan,SIGNAL(clicked(bool)),this,SLOT(onRemotePlan()));
    connect(ui->cbSelectPlan,SIGNAL(currentIndexChanged(int)),this,SLOT(onPagePlan(int)));
    // connect holiday
    connect(ui->pbAddHoliday,SIGNAL(clicked(bool)),this,SLOT(onAddHoliday()));
    connect(ui->pbRemoteHoliday,SIGNAL(clicked(bool)),this,SLOT(onRemoteHoliday()));
    // connect plan
    connect(ui->pbAddItemPlan,SIGNAL(clicked(bool)),this,SLOT(onAddItemPlan()));
    connect(ui->pbRemoteItemPlan,SIGNAL(clicked(bool)),this,SLOT(onRemoteItemPlan()));
    // connect slider value, Uncheck the sliders!!!
    //connect(ui->hs_general_spone,SIGNAL(valueChanged(int)),this,SLOT(onSetSliderValue(int)));
    //connect(ui->hs_general_sptwo,SIGNAL(valueChanged(int)),this,SLOT(onSetSliderValue(int)));
    // connect save \ open files
    connect(ui->actionSave_project,SIGNAL(triggered(bool)),this,SLOT(onSaveFiles()));
    connect(ui->actionOpen_project,SIGNAL(triggered(bool)),this,SLOT(onOpenFile()));
    //connect week
    connect(ui->sbTotalPlan,SIGNAL(valueChanged(int)),this,SLOT(onSetMaxPlanWeek(int)));
    // win setting uart
    connect(ui->actionConfig_USB, &QAction::triggered, settings, &MainWindow::show);
    // win setting wifi
    connect(ui->actionConfig_WIFI, &QAction::triggered, settingsWifi, &MainWindow::show);
    // command
    QList<QPushButton*> listButtonCommand;
    listButtonCommand<<ui->pbSetTime<<ui->pbGetTime<<ui->pbPlayTest;
    QSignalMapper *smCommand = new QSignalMapper(this);
    connect(smCommand,SIGNAL(mapped(int)),this,SLOT(onCommandRun(int)));
    for (int i = 0; i <ListCommand.size()+listButtonCommand.size(); i++)
    {
        if (i <ListCommand.size())
        {
            smCommand->setMapping(ListCommand.at(i), i);
            connect(ListCommand.at(i), SIGNAL(triggered()), smCommand,SLOT(map()));
        }else{
            QPushButton *const button(listButtonCommand.at(i - ListCommand.size()));
            smCommand->setMapping(button, i);
            connect(button, SIGNAL(clicked(bool)), smCommand,SLOT(map()));
        }
    }
    // interface
    QSignalMapper *smConnect = new QSignalMapper(this);
    connect(smConnect,SIGNAL(mapped(int)),this,SLOT(onConnect(int)));
    for (int i = 0; i<ListConnect.size(); i++)
    {
        smConnect->setMapping(ListConnect.at(i), i);
        connect(ListConnect.at(i), SIGNAL(triggered()), smConnect,SLOT(map()));
    }
    // command test
    timerTest = new QTimer(this);
    connect(timerTest,&QTimer::timeout,this,&MainWindow::onCommandTest);
    connect(ui->gbDiagnosis,&QGroupBox::toggled,this,&MainWindow::onTimerDiagnosis);
    connect(ptrController,&Controller::signalStop,this,&MainWindow::onTimerDiagnosisDisabled);
    // update new
    connect(ptrController,&Controller::signalStop,this,&MainWindow::onUpdateDataToGui);
    // message
    connect(ptrController,&Controller::signalMessageOk,this,&MainWindow::onWindowOk);
    connect(ptrController,&Controller::signalMessageError,this,&MainWindow::onWindowError);
    // protocol
    connect(ptrController,&Controller::signalSendMessage,this,&MainWindow::onSetMessageOutWin);
    //set parametrs
    connect(settingsWifi,&SettingDialogWifi::signalListParametr,ptrController,&Controller::setParametrWIFI);
    connect(settings,&SettingsDialog::signalListParametr,ptrController,&Controller::setParametrCOMPORT);
    // diagnosis TIME
        // Structure to GUI
    connect(ptrController,&Controller::signalTime,this,&MainWindow::onTestTime);
        // GUI to Sructure
    connect(ui->dateTimeEditConfig,&QDateTimeEdit::dateTimeChanged,ptrController,&Controller::setDateTime);
    connect(ui->cbGMT,SIGNAL(currentIndexChanged(int)),ptrController,SLOT(setDateTimeZone(int)));
    // diagnosis
    connect(ptrController,&Controller::signalTest,this,&MainWindow::onTestDate);
    // test listen
    connect(this,&MainWindow::signaTestListen,ptrController,&Controller::setListenTest);
    // test gui listen
    QList<QToolButton*> listToolButtonListen;
    listToolButtonListen<<ui->tbZSR<<ui->tbZSO<<ui->tbZSP<<ui->tbZSZ<<ui->tbZSVRR<<ui->tbZSVRZ<<ui->tbTVP;
    foreach( QToolButton*bt,listToolButtonListen )
    {
        //connect(bt,SIGNAL(clicked(bool)),this,SLOT(onUpdateListenGuiToDate()));
        connect(bt, &QPushButton::clicked, [this, bt](){
               this->updateGuiToTestListenDate();
           });
    }
    QList<QSpinBox*> listSpinBoxListen;
    listSpinBoxListen<<ui->sbTestSoundSP1<<ui->sbTestSpeaker1<<ui->sbTestPlan1
        <<ui->sbTestSoundSP2<<ui->sbTestSpeaker2<<ui->sbTestPlan2<<ui->sbTestNoise;
    foreach( QSpinBox*sb,listSpinBoxListen )
    {
        connect(sb,SIGNAL(valueChanged(int)),this,SLOT(onUpdateListenGuiToDate()));
    }
}
/**
 * @brief MainWindow::createGroupMenu
 */
void MainWindow::createGroupMenu()
{
    QActionGroup *grouAactionView = new QActionGroup(this);
    grouAactionView->addAction(ui->actionDiagnosis);
    grouAactionView->addAction(ui->actionSetting);

    QActionGroup *grouAactionConnect = new QActionGroup(this);
    grouAactionConnect->addAction(ui->actionConnect_USB);
    grouAactionConnect->addAction(ui->actionConnect_WIFI);

}
/**
 * @brief MainWindow::createGroupButton
 */
void MainWindow::createGroupButton()
{
    grToolButton = new QButtonGroup(this);
    grToolButton->addButton(ui->tbZSR);
    grToolButton->addButton(ui->tbZSO);
    grToolButton->addButton(ui->tbZSP);
    grToolButton->addButton(ui->tbZSZ);
    grToolButton->addButton(ui->tbZSVRR);
    grToolButton->addButton(ui->tbZSVRZ);
    grToolButton->addButton(ui->tbTVP);
}
/**
 * @brief MainWindow::createGroupSlider
 */
void MainWindow::createGroupSlider()
{
    QList<QSlider*> slOne;
    slOne<<ui->hs_general_spone<<ui->hs_one_zso<<ui->hs_one_zsp<<ui->hs_one_zsr<<ui->hs_one_zsvrr<<ui->hs_one_zsz<<ui->hs_one_zsvrz<<ui->hs_one_tvp;
    soundValue.insert(0,slOne);
    QList<QLabel*> lbOne;
    lbOne<<ui->lb_general_spone<<ui->lb_one_zso<<ui->lb_one_zsp<<ui->lb_one_zsr<<ui->lb_one_zsvrr<<ui->lb_one_zsz<<ui->lb_one_zsvrz<<ui->lb_one_tvp;
    QList<QSlider*> slTwo;
    slTwo<<ui->hs_general_sptwo<<ui->hs_two_zso<<ui->hs_two_zsp<<ui->hs_two_zsr<<ui->hs_two_zsvrr<<ui->hs_two_zsz<<ui->hs_two_zsvrz<<ui->hs_two_tvp;
    soundValue.insert(1,slTwo);
    QList<QLabel*> lbTwo;
    lbTwo<<ui->lb_general_sptwo<<ui->lb_two_zso<<ui->lb_two_zsp<<ui->lb_two_zsr<<ui->lb_two_zsvrr<<ui->lb_two_zsz<<ui->lb_two_zsvrz<<ui->lb_two_tvp;

    if((slOne.size()==lbOne.size())&&(slTwo.size()==lbTwo.size()))
    {
        for(int i=0;i<slOne.size();i++)
        {
            typedef void(QLabel::*IntSlot)(int);
            lbOne[i]->setNum(slOne[i]->value());
            lbTwo[i]->setNum(slTwo[i]->value());
            connect(slOne[i], &QAbstractSlider::valueChanged, lbOne[i], static_cast<IntSlot>(&QLabel::setNum));
            connect(slTwo[i], &QAbstractSlider::valueChanged, lbTwo[i], static_cast<IntSlot>(&QLabel::setNum));
        }
    }
    else
    {
        qDebug()<<"class MainWindow functions createGroupSlider: ERROR size";
    }
}
/**
 * @brief MainWindow::makeItemLabel
 * @param lstWgt
 * @param text
 */
void MainWindow::makeItemLabel(QListWidget *lstWgt, const QString text)
{
    QLabel *form = new QLabel( text, lstWgt );
    form->setAlignment(Qt::AlignCenter); //qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #707070, stop: 1 #FFFFFF)
    form->setStyleSheet("QLabel {background-color: #D3D3D3;"
                        "border: 1px solid gray;border-radius: 5px;margin-top: 5ex;}");
    QListWidgetItem *item = new QListWidgetItem( lstWgt );
    item->setSizeHint( form->sizeHint() );
    lstWgt->setItemWidget( item, form );
}
/**
 * @brief MainWindow::makeItemPlayList
 * @param parent
 * @param text
 * @param path
 */
void MainWindow::makeItemPlayList(QListWidget *lstWgt,
                                  const QString text,
                                  const QString tip,
                                  const QString path)
{
    CreateFormPlayList *form = new CreateFormPlayList(this, text, tip, path);
    QListWidgetItem *item = new QListWidgetItem( lstWgt );
    item->setSizeHint( form->sizeHint() );
    lstWgt->setItemWidget( item, form );
    connect( form->getQToolButton(), SIGNAL(clicked()), SLOT(onItemButtonPlay()) );
    connect( form->getQToolButtonOpen(), SIGNAL(clicked()), SLOT(onOpenSoundFile()) );
}
/**
 * @brief MainWindow::makeItemPlan
 * @param lstWgt
 * @param number
 * @param time
 * @param val1
 * @param val2
 */
void MainWindow::makeItemPlan( QListWidget *lstWgt,
                               const int number,
                               const QTime time,
                               const int val1,
                               const int val2) {
    CreateDayPlan *form = new CreateDayPlan(this,number,time, val1,val2);
    QListWidgetItem *item = new QListWidgetItem( lstWgt );
    item->setSizeHint( form->sizeHint() );
    lstWgt->setItemWidget( item, form );
}
/**
 * @brief MainWindow::makeItemHoliday
 * @param lstWgt
 * @param index
 * @param mounth
 * @param day
 * @param num_plan
 */
void MainWindow::makeItemHoliday(QListWidget *lstWgt,
                                 const int index,
                                 const int max_plan,
                                 const QDate date,
                                 const int num_plan)
{
    CreateHolidays *form = new CreateHolidays( this,index,date,max_plan,num_plan );
    QListWidgetItem *item = new QListWidgetItem( lstWgt );
    item->setSizeHint( form->sizeHint() );
    lstWgt->setItemWidget(item,form);
    connect(ui->sbTotalPlan,SIGNAL(valueChanged(int)),form,SLOT(setMaxPlan(int)));
}
/**
 * @brief MainWindow::addItemPlanFromDatabase
 * @param lWgt
 * @param number_plan
 */
void MainWindow::addItemPlanFromDatabase(QListWidget *const lWgt, const int number_plan, const int item)
{
    const int index_item = item+1;
    TYPEPLANITEMS day = {};
    retDataBase().getOnePlanDay(number_plan, item, &day);
    const QTime time(day.hour, day.min);
    // create new item
    makeItemPlan(lWgt, index_item, time, day.value_speaker1, day.value_speaker2);
    //qDebug("MainWindow::addItemPlanFromDatabase numberPlan = %d index_item = %d", number_plan, index_item );
}
/**
 * @brief MainWindow::addItemPlan
 */
void MainWindow::addItemPlan(QListWidget *lWgt)
{
    if( !lWgt ) return ;
    // read from database
    const int max_plan = retDataBase().getMaxPlans();
    // add new item
    const int indexShow = lWgt->count()+1;
    if(indexShow<=max_plan)
    {
        makeItemPlan( lWgt, indexShow );
        qDebug("add items plan %d",indexShow);
    }
}
/**
 * @brief MainWindow::remoteItemPlan
 */
void MainWindow::remoteItemPlan(QListWidget *lWgt)
{
    remoteItem(lWgt);
}
/**
 * @brief MainWindow::clearItemPlan
 */
void MainWindow::clearItemPlan()
{
    QList< QListWidget* > lWgts(lWgtPlansList);

    foreach (QListWidget* lWd, lWgts)
    {
        const int lengh(lWd->count());
        for (int i=0; i<lengh; i++)
        {
            remoteItemPlan(lWd);
        }
    }
}
/**
 * @brief MainWindow::addItemHoliday
 * @param lWgt
 */
void MainWindow::addItemHoliday(QListWidget *lWgt)
{
    if( !lWgt ) return ;
    // read from database
    const int max_holiday = retDataBase().getMaxHoliday();
    //gui max plan
    const int max_current_plan = ui->sbTotalPlan->value();
    // add new item
    const int indexShow = lWgt->count()+1;
    if(indexShow<=max_holiday)
    {
        makeItemHoliday( lWgt, indexShow, max_current_plan);
        qDebug("add items%d",indexShow);
    }
}
/**
 * @brief MainWindow::remoteItemHoliday
 * @param lWgt
 */
void MainWindow::remoteItemHoliday(QListWidget *lWgt)
{
    remoteItem(lWgt);
}
/**
 * @brief MainWindow::remoteItem
 * @param lWgt
 * @return
 */
bool MainWindow::remoteItem(QListWidget *lWgt)
{
    if( !lWgt ) return false;
    // select items
    QList< QListWidgetItem* > items = lWgt->selectedItems();
    if (items.empty())
    {
        lWgt->setCurrentRow(lWgt->count()-1);
        items<<lWgt->currentItem();
    }
    foreach( QListWidgetItem* item, items )
    {
        int row = lWgt->row( item );
        QWidget *form = lWgt->itemWidget(item);
        lWgt->takeItem( row );
        delete form;
        delete item;
    }
    // sorting index
    indexSorting(lWgt);
    qDebug("remote items %d",lWgt->count());
    return true;
}
/**
 * @brief MainWindow::indexSorting
 * @return
 */
bool MainWindow::indexSorting(QListWidget *lWgt)
{
    if( !lWgt ) return false;
    for(int i=0;i<lWgt->count();i++)
    {
        QListWidgetItem *item = lWgt->item(i);
        InterfaceForms *form = dynamic_cast<InterfaceForms *>( lWgt->itemWidget(item) );
        form->setNumberItem(i+1);
    }
    return true;
}
/**
 * @brief MainWindow::createPlans
 */
void MainWindow::createPlans( QWidget * const page )
{
    // read from database max plan
    const int max_plans = retDataBase().getMaxPlans();
    // create form plans
    swPlans = new QStackedWidget(page);
    for(int i=0; i<max_plans;i++)
    {
        //read from database
        const int item_plan = retDataBase().countItemPlan(i);
        // create one plan
        QWidget *wdg = new QWidget;
        createOnePlan(wdg, i, item_plan);
        swPlans->addWidget(wdg);
        qDebug("MainWindow::createPlans: item_plan=%d",item_plan);
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(swPlans);
    layout->setContentsMargins(0,0,0,0);
    page->setLayout(layout);
    qDebug("MainWindow::createPlans: swPlans->count()=%d",swPlans->count());
}
/**
 * @brief MainWindow::createPlayer
 * @param page
 */
void MainWindow::createPlayer(QWidget *const page)
{
    player = new MusicPlayer(page);
}
/**
 * @brief MainWindow::createOnePlan
 * @param page
 * @param maxItem
 */
void MainWindow::createOnePlan( QWidget *page, const int number_plan, const int maxItem )
{
    QListWidget* lWgt = new QListWidget;

    QLayout* vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->addWidget( lWgt );

    page->setLayout(vlayout);
    for (int i = 0; i<maxItem; i++)
    {
        addItemPlanFromDatabase( lWgt, number_plan, i);
    }
    // add list
    lWgtPlansList.append(lWgt);
}
/**
 * @brief MainWindow::createHolidays
 */
void MainWindow::createHolidays(QWidget *const page)
{
    // read from database
    const int count_holiday = retDataBase().countItemHoliday();
    // create form holiday
    createOneHoliday(page,count_holiday);
}
/**
 * @brief MainWindow::createOneHoliday
 * @param page
 * @param maxItem
 */
void MainWindow::createOneHoliday(QWidget *page, const int maxItem)
{
    QListWidget* lWgt = new QListWidget;

    QLayout* vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->addWidget( lWgt );

    page->setLayout(vlayout);

    for (int i = 1; i<=maxItem; i++)
    {
        // read from database
        const int max_plan = retDataBase().countPlan();
        TYPEHOLIDAY item = {};
        retDataBase().getOneHoliday(i,&item);
        QDate date(QDate::currentDate().year(),item.month,item.day);
        // make item
        makeItemHoliday( lWgt,i,max_plan,date,item.plan );
    }
    // private pointer
    lWgtHoliday = lWgt;
}
/**
 * @brief MainWindow::createFormPlayList
 * @param page
 */
void MainWindow::createFormPlayList(QWidget * const page)
{
    QListWidget* lWgt = new QListWidget;lWgtPlayList = lWgt;
    QLayout* vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->addWidget( lWgt );

    page->setLayout(vlayout);

    // create general
    makeItemLabel(lWgt, tr("GENERAL TRACK"));
    QStringList generallist;
        generallist<<tr("ZSO:")<<tr("ZSP:");
    QStringList generaltip;
        generaltip<<tr("The file name must begin with <2000_>")<<tr("The file name must begin with <2100_>");
    createItemPlay(lWgt, generallist, generaltip);
    // create allowed
    QStringList allowedlist;
        allowedlist<<tr("ZSR:")<<tr("ZSVRR>60:")<<tr("ZSVRR<60:")
                  <<tr("ZSVRR<50:")<<tr("ZSVRR<40:")<<tr("ZSVRR<30:")<<tr("ZSVRR<20:")<<tr("ZSVRR<10:");
    QStringList allowedtip;
        allowedtip<<tr("The file name must begin with <0001_>")<<tr("The file name must begin with <0090_>")
                    <<tr("The file name must begin with <0060_>")<<tr("The file name must begin with <0050_>")
                      <<tr("The file name must begin with <0040_>")<<tr("The file name must begin with <0030_>")
                        <<tr("The file name must begin with <0020_>")<<tr("The file name must begin with <0010_>");
    makeItemLabel(lWgt, tr("ALLOWED TRACK"));
    createItemPlay(lWgt, allowedlist, allowedtip);
    // create prohibitive
    QStringList prohibitivelist;
        prohibitivelist<<tr("ZSZ:")<<tr("ZSVRZ>60:")<<tr("ZSVRZ<60:")
                          <<tr("ZSVRZ<50:")<<tr("ZSVRZ<40:")<<tr("ZSVRZ<30:")<<tr("ZSVRZ<20:")<<tr("ZSVRZ<10:");
    QStringList prohibitivetip;
        prohibitivetip<<tr("The file name must begin with <0002_>")<<tr("The file name must begin with <0900_>")
                        <<tr("The file name must begin with <0600_>")<<tr("The file name must begin with <0500_>")
                            <<tr("The file name must begin with <0400_>")<<tr("The file name must begin with <0300_>")
                                <<tr("The file name must begin with <0200_>")<<tr("The file name must begin with <0100_>");
    makeItemLabel(lWgt, tr("PROHIBITIVE TRACK"));
    createItemPlay(lWgt, prohibitivelist, prohibitivetip);
    // create TVP track
    QStringList tvplist; tvplist<<tr("TVP:");
    QStringList tvptip; tvptip<<tr("The file name must begin with <3000_>");
    makeItemLabel(lWgt, tr("TVP TRACK"));
    createItemPlay(lWgt, tvplist, tvptip);
}
/**
 * @brief MainWindow::createItemPlay
 * @param page
 * @param list
 * @param tip
 */
void MainWindow::createItemPlay(QListWidget * const page, QStringList &list, QStringList &tip)
{
    foreach(QString txt, list)
        makeItemPlayList( page, txt, ((!tip.isEmpty())?tip.takeFirst():"") );
}
/**
 * @brief MainWindow::setTitle
 * @param str
 */
void MainWindow::setTitle(const QString file_name)
{
    QString wintitle(QObject::tr("UZTVOP "));
    this->setWindowTitle(wintitle+file_name);
}
/**
 * @brief MainWindow::onWindowsOut
 */
void MainWindow::onWindowsOut()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QTextEdit *window = mainWindowFormMap[action];
    if(!window){
          // create debug windows
          window = new QTextEdit;
          window->setWindowIcon(QPixmap(":/ico/win48x48.ico"));
          window->installEventFilter(this);
          mainWindowFormMap.insert(action, window);
          }
    window->setVisible(action->isChecked());
}
/**
 * @brief MainWindow::onActionabout
 */
void MainWindow::onActionabout()
{
    QMessageBox::about(this,tr("About the program a Configure."),
                            tr("<h2> Programming and testing version 1.1</h2>"
                               "<p>Co., Ltd. CyberSB"
                               "<h2>The program configure.</h2>"
                               ));
}
/**
 * @brief MainWindow::onToolBar
 * @param visible
 */
void MainWindow::onToolBar(bool visible)
{
    ui->mainToolBar->setVisible(visible);
    ui->actionShow_Toolbar->setChecked(visible);
}

void MainWindow::onTimeout()
{
//static bool flag = false;

//if(flag){
//    wificon->setText("CONNECT OK");
//    wificon->setEnabled(true);
//    QString str="CONNECT";
//            //"<img src=\":/ico/win48x48.ico\"> Открытые ссылки";
//    //wificon->setTextFormat(Qt::RichText);
//    wificon->setText(str);
//    //wificon->setTextFormat(Qt::TextColorRole);
//    //wificon->setStyleSheet("background: green;");
//}
//else{
//    wificon->setText("CONNECT ERROR");
//    QString str="DISCONNECT";
//            //"<img src=\":/ico/win48x48.ico\"> Открытые ссылки";
//    //wificon->setTextFormat(Qt::RichText);
//    wificon->setText(str);
//    wificon->setEnabled(false);
//    //wificon->setStyleSheet("background: red;");
//}

//flag=!flag;
}
/**
 * @brief MainWindow::onSwitchPanelMode
 * @param numberPanel
 */
void MainWindow::onSwitchPanelMode(int numberPanel)
{
    if(numberPanel<pnEND)
    {
        ui->stackedWidget->setCurrentIndex(numberPanel);
    }
//    // enabled timer test
//    if (numberPanel == pnDIAGNOSIS)
//    {
//        onTimerDiagnosisEnabled(true);
//        ui->actionRead->setEnabled(false);
//        ui->actionWrite->setEnabled(false);
//    }
//    else
//    {
//        onTimerDiagnosisEnabled(false);
//        ui->actionRead->setEnabled(true);
//        ui->actionWrite->setEnabled(true);
//    }
}
/**
 * @brief MainWindow::onAddPlan
 */
void MainWindow::onAddPlan()
{
    // read from database max parametr
    const int max_plans = retDataBase().getMaxPlans();
    // add
    const int index = ui->sbTotalPlan->value()+1;
    if (index<max_plans)
    {
        ui->sbTotalPlan->setValue(index);
        // set item combobox
        ui->cbSelectPlan->clear();
        for(int i=1;i<=index;i++)
        {
            ui->cbSelectPlan->addItem(tr("Number Plan ")+QString::number(i));
        }
    }
}
/**
 * @brief MainWindow::onRemotePlan
 */
void MainWindow::onRemotePlan()
{
    const int index = ui->sbTotalPlan->value()-1;
    if (index)
    {
        // set new value
        ui->sbTotalPlan->setValue(index);
        // remote item combobox
        ui->cbSelectPlan->clear();
        for(int i=1;i<=index;i++)
        {
            ui->cbSelectPlan->addItem(tr("Number Plan ")+QString::number(i));
        }
    }
}
/**
 * @brief MainWindow::onAddItemPlan
 */
void MainWindow::onAddItemPlan()
{
    const int index = ui->cbSelectPlan->currentIndex();
    if ( ( !lWgtPlansList.isEmpty()) && (index<lWgtPlansList.count()))
    {
        addItemPlan(lWgtPlansList.at(index));
        qDebug("onAddItemPlan add %d",ui->cbSelectPlan->currentIndex());
    }else{
        qDebug("onAddItemPlan ERROR LENGH %d",index);
    }

}
/**
 * @brief MainWindow::onRemoteItemPlan
 */
void MainWindow::onRemoteItemPlan()
{
    const int index = ui->cbSelectPlan->currentIndex();
    if ( ( !lWgtPlansList.isEmpty()) && (index<lWgtPlansList.count()))
    {
        remoteItemPlan(lWgtPlansList.at(index));
        qDebug("onRemoteItemPlan remote %d",ui->cbSelectPlan->currentIndex());
    }else{
        qDebug("onRemoteItemPlan ERROR LENGH %d",index);
    }
}
/**
 * @brief MainWindow::onPagePlan
 */
void MainWindow::onPagePlan(int index)
{
    swPlans->setCurrentIndex(index);
    ui->gbCurrentPlan->setTitle(tr("Current Plan ")+QString::number(index+1));
}
/**
 * @brief MainWindow::onAddHoliday
 */
void MainWindow::onAddHoliday()
{
    addItemHoliday(lWgtHoliday);
}
/**
 * @brief MainWindow::onRemoteHoliday
 */
void MainWindow::onRemoteHoliday()
{
    remoteItemHoliday(lWgtHoliday);
}
/**
 * @brief MainWindow::resultSliderValue
 * @param listSlider
 * @param delta
 */
void MainWindow::resultSliderValue(QList<QSlider*> listSlider, const int16_t delta)
{
    for(int i = 1; i<listSlider.count();i++)
    {
        const uint16_t result =  listSlider.at(i)->value();
        listSlider.at(i)->setValue(result+delta);
    }
}
/**
 * @brief MainWindow::showLabelEnabled
 * @param lb
 */
void MainWindow::showLabelEnabled(QLabel * const lb)
{
    lb->setStyleSheet("background: green;");
}
/**
 * @brief MainWindow::showLabelDisabled
 * @param lb
 */
void MainWindow::showLabelDisabled(QLabel * const lb)
{
    lb->setStyleSheet("background: red;");
}
/**
 * @brief MainWindow::showLabel
 * @param on
 * @param lb
 */
void MainWindow::showLabel(const bool on, QLabel * const lb)
{
    if (on)
        showLabelEnabled(lb);
    else
        showLabelDisabled(lb);
}
/**
 * @brief MainWindow::installDiagnosisPage
 */
void MainWindow::installDiagnosisPage()
{
    // disabled
    showLabelDisabled(ui->lbOnGPS);
    showLabelDisabled(ui->lbRedSignal);
    showLabelDisabled(ui->lbGreenSignal);
    showLabelDisabled(ui->lbButtonOn);
    showLabelDisabled(ui->lbMotionSens);
    showLabelDisabled(ui->lbWait);
    //showLabel(true,ui->lbMotionSens);
}
/**
 * @brief MainWindow::checkSuffixSoundFiles
 * @param path
 */
bool MainWindow::checkSuffixSoundFiles(const QString path) const
{
    QFileInfo info(path);
    const QString typeWav("WAV");
    //const QString typeMp3("MP3");
    const QString suffix(info.suffix().toUpper());

    if ( (suffix != typeWav) )// && (suffix != typeMp3) )
    {
        return false;
    }
    return true;
}
/**
 * @brief MainWindow::dataSoundFile
 * @param path
 * @return
 */
QByteArray MainWindow::dataSoundFile(const QString path) const
{
    QFile file( path );
    if( file.open( QIODevice::ReadOnly ) )
    {
        QByteArray data = file.readAll();
        file.close();
        return data;
    }
    return QByteArray();
}
/**
 * @brief MainWindow::retPrefixNameFile
 * @param sample_name
 * @return
 */
QString MainWindow::retPrefixNameFile(const QString sample_name) const
{
    QRegExp re( " <([^>]+)>" );
    int lastPos = 0;
    while( ( lastPos = re.indexIn( sample_name, lastPos ) ) != -1 )
    {
        lastPos += re.matchedLength();
        return re.cap( 1 );
    }
    // error
    qDebug() << "ERROR MainWindow::retPrefixNameFile";
    return QString();
}
/**
 * @brief MainWindow::clearGuiToTracks
 */
void MainWindow::clearGuiToTracks()
{
    QListWidget const *const lWgt(lWgtPlayList);
    for(int i=0;i<lWgt->count();i++)
    {
        QListWidgetItem *const item = lWgt->item(i);
        CreateFormPlayList *const formItem = dynamic_cast<CreateFormPlayList *>( lWgt->itemWidget(item) );
        if ( formItem )
        {
            formItem->getQLineEdit()->setText(QString());
        }
    }
}
/**
 * @brief MainWindow::checkNameSoundFile
 * @param sample_name
 * @param file_name
 * @return
 */
QString MainWindow::checkNameSoundFile(const QString sample_name, const QString file_name) const
{
    QString return_name(file_name);
    QString pattern(retPrefixNameFile(sample_name));
    // add prefix
    if (!file_name.contains(pattern))
    {
        return_name.prepend(pattern);
    }
    return return_name;
}
/**
 * @brief MainWindow::updateGuiToTracks
 */
void MainWindow::updateGuiToTracks()
{
    // clear database
    retDataProject().clearPlayList();
    // updata database
    QListWidget const *const lWgt(lWgtPlayList);
    for(int i=0;i<lWgt->count();i++)
    {
        QListWidgetItem *const item = lWgt->item(i);
        CreateFormPlayList *const formItem = dynamic_cast<CreateFormPlayList *>( lWgt->itemWidget(item) );
        if ( formItem )
        {
            const QString path(formItem->getPath());
            const QString file_name(QFileInfo(path).fileName());
            const QString toolTip(formItem->getQLineEdit()->toolTip());
            QString file_name_db(checkNameSoundFile(toolTip, file_name));
            if ( checkSuffixSoundFiles(file_name_db) )
            {
                const QByteArray data_file(dataSoundFile(path));
                retDataProject().retPlayList()[file_name_db] = data_file;
            }
        }
    }
}
/**
 * @brief MainWindow::updateTracksPathToGui
 * @param path_file
 */
void MainWindow::updateTracksPathToGui(const QString path_file) const
{
    //
    QListWidget const *const lWgt(lWgtPlayList);
    for(int i=0;i<lWgt->count();i++)
    {
        QListWidgetItem *const item = lWgt->item(i);
        CreateFormPlayList *const formItem = dynamic_cast<CreateFormPlayList *>( lWgt->itemWidget(item) );
        if ( formItem )
        {
            const QString pattern(retPrefixNameFile(formItem->getQLineEdit()->toolTip()));
            if ( path_file.contains(pattern) )
            {
                formItem->getQLineEdit()->setText(path_file);
                return;
            }
        }
    }
}
/**
 * @brief MainWindow::updateTracksToGui
 */
void MainWindow::updateTracksToGui(const QString path_file)
{
    const QString name_folder(QFileInfo(path_file).fileName().remove(QChar('.'), Qt::CaseInsensitive).toUpper());
    QDir dir(QFileInfo(path_file).path());
    // create folder
    if( !dir.exists(name_folder) )
    {
        dir.mkdir(name_folder);
    }
    //enter to folder
    dir.cd(name_folder);
    // clear giu
    clearGuiToTracks();
    // extract sound files to folder
    QMap <QString, QByteArray>::iterator it = retDataProject().retPlayList().begin();
    for (;it!= retDataProject().retPlayList().end(); ++it)
    {
        const QString name_file(it.key());
        if ( checkSuffixSoundFiles(name_file) )
        {
            const QString path(dir.filePath(name_file));
            QFile soundFile(path);
            soundFile.open (QIODevice::WriteOnly);
            QDataStream inFile(&soundFile);
            inFile.setVersion(QDataStream::Qt_5_7);
            const QByteArray data_file(it.value());
            inFile.writeRawData(data_file.data(),data_file.count());
            //inFile<<data_file; // This operation was added at the beginning of the file 4 bytes?
            soundFile.flush();
            soundFile.close();
            // update GIU
            updateTracksPathToGui(path);
        }
    }
}
/**
 * @brief MainWindow::updateGuiToVolume
 */
void MainWindow::updateGuiToVolume()
{
    for (int i=0;i<soundValue.size();i++)
    {
        QMap<int,QList<QSlider*> >::const_iterator sp = soundValue.lowerBound(i);
        QList<QSlider*> speaker = sp.value();
        uint8_t general = speaker.at(0)->value();
        const uint8_t max_separate = speaker.count()-1;
        uint8_t separate[max_separate] = {0};
        for ( int j=1; j<=max_separate; j++ )
        {
            separate[j-1] = speaker.at(j)->value();
        }
        retDataBase().setSoundValue( i, general, separate, max_separate );
    }
}
/**
 * @brief MainWindow::updateVolumeToGui
 */
void MainWindow::updateVolumeToGui()
{
    for (int i=0;i<soundValue.size();i++)
    {
        QMap<int,QList<QSlider*> >::const_iterator sp = soundValue.lowerBound(i);
        QList<QSlider*> speaker = sp.value();
        uint8_t general = 0;
        const uint8_t max_separate = speaker.count()-1;
        uint8_t separate[max_separate]= {0};
        if (retDataBase().getSoundValue(i, &general, separate, max_separate) )
        {
            speaker.at(0)->setValue(general);
            value_speaker[i] = general;
            for (int j=1; j<=max_separate; j++)
            {
                speaker.at(j)->setValue(separate[j-1]);
            }
        }else{
             qDebug("updateDataToGui size: %d, %d", general, max_separate);
        }
    }
}
/**
 * @brief MainWindow::updateGuiToVibration
 */
void MainWindow::updateGuiToVibration()
{
    const uint32_t time_lenght = ui->sb_vibr_lenght->value();
    const uint32_t time_pause = ui->sb_vibr_pause->value();
    const uint32_t intensity = ui->sb_vibr_intensity->value();
    const uint8_t on = ui->cb_enable_button->isChecked();
    // set database
    retDataBase().setVibration(time_lenght, time_pause, intensity, on);
}
/**
 * @brief MainWindow::updateVibrationToGui
 */
void MainWindow::updateVibrationToGui()
{
    TYPEVIBRATION vibration={};
    retDataBase().getVibration(&vibration);
    // set GUI
    ui->sb_vibr_lenght->setValue( vibration.time_lenght );
    ui->sb_vibr_pause->setValue( vibration.time_pause );
    ui->sb_vibr_intensity->setValue( vibration.intensity );
    ui->cb_enable_button->setChecked( vibration.on );
}
/**
 * @brief MainWindow::updateGuiToPlan
 * @param numberPlan
 */
void MainWindow::updateOneGuiToPlan(QListWidget *const lWgt, const int numberPlan)
{
    for ( int i=0; i<lWgt->count(); i++ )
    {
        QListWidgetItem *const item = lWgt->item(i);
        CreateDayPlan *const formItem = dynamic_cast<CreateDayPlan *>( lWgt->itemWidget(item) );
        int volume1 = 0;
        int volume2 = 0;
        formItem->getVolume( &volume1, &volume2 );
            const TYPEPLANITEMS plan = {
                .hour = (uint8_t)formItem->getTime().hour(),
                .min = (uint8_t)formItem->getTime().minute(),
                .value_speaker1 = (uint8_t)volume1,
                .value_speaker2 = (uint8_t)volume2
            };
            if ( retDataBase().setOnePlanDay( numberPlan, i, plan) == false )
            {
                qDebug() << "Class MainWindow: functions updateGuiToPlan: ERROR";
            }
    }
}
/**
 * @brief MainWindow::updateGuiToPlan
 */
void MainWindow::updateGuiToPlan()
{
    QList< QListWidget* > lWgts(lWgtPlansList);
    retDataBase().clearPlans();

    for (int i=0;i<lWgts.count(); i++)
    {
        QListWidget *const lWgt(lWgts.at(i));
        updateOneGuiToPlan(lWgt, i);
    }
}
void MainWindow::updateOnePlanToGui(QListWidget * const lWgt,
                                 const int numberPlan)
{
    const int count_item = retDataBase().countItemPlan(numberPlan);
    for ( int i=0; i<count_item; i++)
    {
        addItemPlanFromDatabase(lWgt, numberPlan, i);
    }
    qDebug("MainWindow::updateOnePlanToGui count_item=%d",count_item);
}
/**
 * @brief MainWindow::updatePlanToGui
 */
void MainWindow::updatePlanToGui()
{
    // delete item
    clearItemPlan();
    // add new items
    QList< QListWidget* > lWgts(lWgtPlansList);
    const int count_plan = retDataBase().countPlan();
    for ( int i=0; i<count_plan; i++)
    {
        QListWidget *const iwd = lWgts.at(i);
        updateOnePlanToGui(iwd, i);
    }
    // set tool date
    ui->sbTotalPlan->setValue(count_plan);
    // set item combobox (If the plans are zero)
    ui->cbSelectPlan->clear();
    for(int i=0;i<count_plan;i++)
    {
        ui->cbSelectPlan->addItem(tr("Number Plan ")+QString::number(i+1));
    }
    // If the plans are zero,
    if (!count_plan) // bug configurations
    {
        ui->cbSelectPlan->addItem(tr("Number Plan 1"));
    }
    qDebug("MainWindow::updatePlanToGui count_plan=%d",count_plan);
}
/**
 * @brief MainWindow::updateGuiToHoliday
 */
void MainWindow::updateGuiToHoliday()
{
    QListWidget const *const lWgt(lWgtHoliday);

    retDataBase().clearHoliday();

    for(int i=0;i<lWgt->count();i++)
    {
        QListWidgetItem *const item = lWgt->item(i);
        CreateHolidays *const formItem = dynamic_cast<CreateHolidays *>( lWgt->itemWidget(item) );
        const TYPEHOLIDAY holiday = {
            .month = (uint8_t)formItem->getDateEdit().month(),
            .day = (uint8_t)formItem->getDateEdit().day(),
            .plan = (uint8_t)formItem->getNumberPlan(),
            .reserved = (uint8_t)0
        };
        if ( retDataBase().setOneHoliday(i,holiday) == false )
        {
            qDebug() << "Class MainWindow: functions updateGuiToHoliday: ERROR";
        }
    }
}
/**
 * @brief MainWindow::updateHolidayToGui
 */
void MainWindow::updateHolidayToGui()
{

    QListWidget *const lWgt(lWgtHoliday);

    if (!lWgt) return;
    // remote old page
    for ( int i=0; i<lWgt->count();i++ )
    {
        remoteItemHoliday(lWgt);
    }
    const uint32_t max_current_plan = retDataBase().countPlan();
    const uint32_t max_current_holiday = retDataBase().countItemHoliday();
    // create new page
    for ( uint32_t i=0; i<max_current_holiday; i++)
    {
        TYPEHOLIDAY holiday = {};
        retDataBase().getOneHoliday(i, &holiday);
        const QDate date(QDate::currentDate().year(), holiday.month, holiday.day);
        makeItemHoliday( lWgt, (i+1), max_current_plan, date, holiday.plan);
        qDebug("updateHolidayToGui %d",(i+1));
    }
}
/**
 * @brief MainWindow::updateGuiToWeek
 */
void MainWindow::updateGuiToWeek()
{
    QList <QSpinBox*> list; list<<ui->sbMonday<<ui->sbTuesday<<ui->sbWednesday
                               <<ui->sbThursday<<ui->sbFriday<<ui->sbSaturday<<ui->sbSunday;
    for (int i = 0; i<list.count(); i++)
    {
        const int plan = list.at(i)->value();
        retDataBase().setOneWeek(i, plan);
    }
}
/**
 * @brief MainWindow::updateWeekToGui
 */
void MainWindow::updateWeekToGui()
{
    QList <QSpinBox*> list; list<<ui->sbMonday<<ui->sbTuesday<<ui->sbWednesday
                               <<ui->sbThursday<<ui->sbFriday<<ui->sbSaturday<<ui->sbSunday;
    for (int i = 0; i<list.count(); i++)
    {
        uint8_t plan = 0;
        retDataBase().getOneWeek(i, &plan);
        list.at(i)->setValue(plan);
    }
}
/**
 * @brief MainWindow::updateGuiToOther
 */
void MainWindow::updateGuiToOther()
{
    // sound acceleration
    const TYPEACCELERATION acceleration = {
        .time = (uint32_t)ui->sb_time_acceleration->value(),
        .on = (uint8_t)ui->gb_acceleration->isChecked()
    };
    retDataBase().setAcceleration( acceleration );
    // motion
    const TYPEMOTION motion ={
        .time = (uint32_t)ui->sb_motion_time->value(),
        .asTVP = (uint8_t)ui->check_motion_as_tvp->isChecked(),
        .on = (uint8_t)ui->gb_motion_sensor->isChecked(),
        .reserved = (uint8_t)0
    };
    retDataBase().setMotion( motion );
    // noise
    const TYPENOISE noise={
        .time = (uint32_t)ui->sb_noise_time->value(),
        .min_level = (uint8_t)ui->sb_noise_min_level->value(),
        .max_level = (uint8_t)ui->sb_noise_max_level->value(),
        .min_ct = (uint8_t)ui->sb_noise_min_coef->value(),
        .max_ct = (uint8_t)ui->sb_noise_max_coef->value(),
        .on = (uint8_t)ui->gb_noise_sensor->isChecked(),
        .reserved = (uint8_t)0
    };
    retDataBase().setNoise( noise );
    // button
    const TYPEBUTTON button = {
        .time = (uint32_t)ui->sb_time_button->value(),
        .on = (uint8_t)true
    };
    retDataBase().setButton( button );
    // setting
    QList <QCheckBox*> list_check;
    list_check<<ui->cbGREEN_BLINK_TABU<<ui->cbZSZ_BEGIN_BAN_TIME<<ui->cbZSZ_BUTT_CLICK<<ui->cbZSVRZ_PERIODICALLY
            <<ui->cbZSVRZ_BUTT_CLICK<<ui->cbZSR_BEGIN_BAN_TIME<<ui->cbZSR_BUTT_CLICK<<ui->cbZSVRR_PERIODICALLY
                <<ui->cbZSVRR_BUTT_CLICK<<ui->cbAPPLY_BUTTON_AS_TVP;
    uint64_t flags = 0;
    for (int i=0;i<list_check.count();i++)
    {
        QCheckBox *const ch = list_check.at(i);
        if (ch->isChecked())
        {
            flags|=1<<i;
        }
    }
    const TYPESETTING setting ={
        .flag = (uint64_t)flags,
        .period = (uint32_t)ui->sb_zso_period->value(),
        .delay = (uint32_t)ui->sb_zso_delay->value()
    };
    retDataBase().setSetting( setting );
}
/**
 * @brief MainWindow::updateOtherToGui
 */
void MainWindow::updateOtherToGui()
{
    // sound acceleration
    TYPEACCELERATION acceleration = {};
    retDataBase().getAcceleration(&acceleration);
    ui->gb_acceleration->setChecked(acceleration.on);
    ui->sb_time_acceleration->setValue(acceleration.time);
    // motion
    TYPEMOTION motion ={};
    retDataBase().getMotion(&motion);
    ui->gb_motion_sensor->setChecked(motion.on);
    ui->check_motion_as_tvp->setChecked(motion.asTVP);
    ui->sb_motion_time->setValue(motion.time);
    // noise
    TYPENOISE noise={};
    retDataBase().getNoise(&noise);
    ui->gb_noise_sensor->setChecked(noise.on);
    ui->sb_noise_time->setValue(noise.time);
    ui->sb_noise_min_level->setValue(noise.min_level);
    ui->sb_noise_max_level->setValue(noise.max_level);
    ui->sb_noise_min_coef->setValue(noise.min_ct);
    ui->sb_noise_max_coef->setValue(noise.max_ct);
    // button
    TYPEBUTTON button = {};
    retDataBase().getButton(&button);
    ui->sb_time_button->setValue(button.time);
    // setting
    TYPESETTING setting ={};
    retDataBase().getSetting( &setting );
    ui->sb_zso_period->setValue(setting.period);
    ui->sb_zso_delay->setValue(setting.delay);
    // flag
    QList <QCheckBox*> list_check;
    list_check<<ui->cbGREEN_BLINK_TABU<<ui->cbZSZ_BEGIN_BAN_TIME<<ui->cbZSZ_BUTT_CLICK<<ui->cbZSVRZ_PERIODICALLY
            <<ui->cbZSVRZ_BUTT_CLICK<<ui->cbZSR_BEGIN_BAN_TIME<<ui->cbZSR_BUTT_CLICK<<ui->cbZSVRR_PERIODICALLY
                <<ui->cbZSVRR_BUTT_CLICK<<ui->cbAPPLY_BUTTON_AS_TVP;
    for (int i=0;i<list_check.count();i++)
    {
        QCheckBox *const ch(list_check.at(i));
        bool result = (setting.flag&(1<<i));
        ch->setChecked(result);
    }
}
/**
 * @brief MainWindow::updateGuiToData
 */
void MainWindow::updateGuiToData()
{
    // tracks
    updateGuiToTracks();
    // volume
    updateGuiToVolume();
    // vibration
    updateGuiToVibration();
    // plan
    updateGuiToPlan();
    //week
    updateGuiToWeek();
    // holiday
    updateGuiToHoliday();
    // setting
    updateGuiToOther();
}
/**
 * @brief MainWindow::updateDataToGui
 */
void MainWindow::updateDataToGui()
{
    // volume speaker
    updateVolumeToGui();
    // vibration
    updateVibrationToGui();
    // plan
    updatePlanToGui();
    //week
    updateWeekToGui();
    // holiday
    updateHolidayToGui();
    // setting
    updateOtherToGui();
}
/**
 * @brief MainWindow::defaultChooseConnect
 */
void MainWindow::defaultChooseConnect()
{
    for (int i=0; i<ListConnect.count(); i++)
    {
        if (ListConnect.at(i)->isChecked() )
        {
            onConnect(i);
        }
    }
}
/**
 * @brief MainWindow::updateTestListenDate
 */
void MainWindow::updateGuiToTestListenDate()
{
    uint8_t number_music = 0;
    QList<QToolButton*> listToolButtonListen;
    listToolButtonListen<<ui->tbZSR<<ui->tbZSO<<ui->tbZSP<<ui->tbZSZ<<ui->tbZSVRR<<ui->tbZSVRZ<<ui->tbTVP;
    foreach( QToolButton*bt,listToolButtonListen )
    {
        if (bt->isChecked())
        {
            break;
        }
        number_music++;
    }
    TYPE_TEST_TRACK test = {};
    test.value[SPEAKER_ONE].sound_sp = static_cast <uint8_t>(ui->sbTestSoundSP1->value());
    test.value[SPEAKER_ONE].value_sk = static_cast <uint8_t>(ui->sbTestSpeaker1->value());
    test.value[SPEAKER_ONE].ct_plan = static_cast <uint8_t>(ui->sbTestPlan1->value());

    test.value[SPEAKER_TWO].sound_sp = static_cast <uint8_t>(ui->sbTestSoundSP2->value());
    test.value[SPEAKER_TWO].value_sk = static_cast <uint8_t>(ui->sbTestSpeaker2->value());
    test.value[SPEAKER_TWO].ct_plan = static_cast <uint8_t>(ui->sbTestPlan2->value());

    test.noise = static_cast <uint16_t>(ui->sbTestNoise->value());
    test.number_music = number_music;

    emit signaTestListen(test);
    qDebug()<<"Listen number:"<<number_music<<"SP1: "<<test.value[SPEAKER_ONE].sound_sp;
}
/**
 * @brief MainWindow::onSetSliderValue
 * @param value
 */
void MainWindow::onSetSliderValue(const int value)
{
    if( QSlider* curSlid = qobject_cast< QSlider* >( sender() ) )
    {
        QMap<int,QList<QSlider*> >::const_iterator sp1 = soundValue.lowerBound(0);
        QMap<int,QList<QSlider*> >::const_iterator sp2 = soundValue.lowerBound(1);

        QList<QSlider*> speakerOneMass = sp1.value();
        QList<QSlider*> speakerTwoMass = sp2.value();

        if(speakerOneMass.at(0)==curSlid)
        {
            const int16_t delta = value-value_speaker[0];
            value_speaker[0] = curSlid->value();
            resultSliderValue(speakerOneMass,delta);
            qDebug("Slider one %d",delta);
            return;
        }
        if(speakerTwoMass.at(0)==curSlid)
        {
            const int16_t delta = value-value_speaker[1];
            value_speaker[1] = curSlid->value();
            resultSliderValue(speakerTwoMass,delta);
            qDebug("Slider one %d",delta);
            return;
        }
    }
}
/**
 * @brief MainWindow::onSetMessageOutWin
 * @param st
 * @param color
 */
void MainWindow::onSetMessageOutWin(const QByteArray &st, const QColor &color)
{
//    QByteArray str(st.toHex().toUpper().trimmed());
//    str = "Read:" + st.toHex().toUpper().trimmed();
//    str = "Send:" + st.toHex().toUpper().trimmed();
    if (st.isEmpty())
        return;
    QByteArray str(st.toHex().toUpper().trimmed());
    QTextEdit * const winTest = mainWindowFormMap[ui->actionWin];
    if (winTest)
    {
        winTest->setTextColor(color);
        winTest->append(str);
    }
    qDebug()<<str;
}
/**
 * @brief MainWindow::onSetMaxPlan
 */
void MainWindow::onSetMaxPlanWeek(const int maxPlan)
{
    ui->sbMonday->setMaximum(maxPlan);
    ui->sbTuesday->setMaximum(maxPlan);
    ui->sbWednesday->setMaximum(maxPlan);
    ui->sbThursday->setMaximum(maxPlan);
    ui->sbFriday->setMaximum(maxPlan);
    ui->sbSaturday->setMaximum(maxPlan);
    ui->sbSunday->setMaximum(maxPlan);
}
/**
 * @brief MainWindow::onSetValueProgress
 */
void MainWindow::onSetValueProgress(const int value, const bool visible)
{
    progress->setVisible(visible);
    progress->setValue(value);
}
/**
 * @brief MainWindow::onOpenFile
 */
void MainWindow::onOpenFile(void)
{
    static QString file = QDir::homePath();
    file = QFileDialog::getOpenFileName(this,tr("Open File"),file,tr("SPG (*.spg);;"));
    if(!file.isEmpty())
    {
        QFile readFile(file);
        readFile.open(QFile::ReadOnly);
        QDataStream outFile(&readFile);
        outFile.setVersion(QDataStream::Qt_5_7);
        retDataProject().readFileToProject(outFile);
        //ptrController->retDataProject().readFileToProject(outFile);
        readFile.close();
        //update GUI
        updateDataToGui();
        updateTracksToGui(file);
        // set file to title
        //setTitle(QFileInfo(file).fileName());
        setTitle(file);
    }
}
/**
 * @brief MainWindow::onSaveFiles
 */
void MainWindow::onSaveFiles(void)
{
    //update
    updateGuiToData();
    //
    static QString file = QDir::homePath();
    file = QFileDialog::getSaveFileName(this,tr("Save File"),file,tr("SPG (*.spg);;"));
    if(!file.isEmpty())
    {
        QFile appFile(file);
        //appFile.open(QFile::Append); // открываем файл для дозаписи;
        appFile.open(QIODevice::WriteOnly);
        QDataStream inFile(&appFile); // передаем потоку указатель на QIODevice;
        inFile.setVersion(QDataStream::Qt_5_7); //  явно указываем версию Qt, для сериализации;
        retDataProject().writeProjectToFile(inFile);
        appFile.flush(); // записываем весь буффер в файл;
        appFile.close();
        // set file to title
        setTitle(QFileInfo(file).fileName());
    }
}
/**
 * @brief MainWindow::onOpenSoundFile
 */
void MainWindow::onOpenSoundFile()
{

    //const QStringList musicPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
    //const QString file =
    static QString file = QDir::homePath();
    file = QFileDialog::getOpenFileName(this,tr("Open File"),
                                        //musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first(),
                                        file,
                                        tr("TRACK (*.wav);;All files (*.*)"));
    if(!file.isEmpty())
    {
        if( QToolButton* btn = qobject_cast< QToolButton* >( sender() ) )
        {
            if( QLineEdit* edit = btn->parent()->findChild< QLineEdit* >() )
            {
                if ( checkSuffixSoundFiles(file) )
                {
                    edit->setText(file);
                }else{
                    QMessageBox::critical(this, QObject::tr("ERROR"), QObject::tr("Open the file of the format \"*.wav\"") );
                }
                return;
            }
        }
    }else{
        qDebug()<<"Class MainWindow functions onOpenSoundFile: file is Empty";
    }
}
/**
 * @brief MainWindow::onSaveSoundFile
 */
void MainWindow::onSaveSoundFile()
{
    static QString file = "/";
    file = QFileDialog::getSaveFileName(this,tr("Save File"),file,tr("TRACK (*.wav);;"));
    if(!file.isEmpty())
    {

    }
}
/**
 * @brief MainWindow::onItemButtonPlay
 */
void MainWindow::onItemButtonPlay()
{
    if( QToolButton* btn = qobject_cast< QToolButton* >( sender() ) )
    {
        if( QLineEdit* edit = btn->parent()->findChild< QLineEdit* >() )
        {
            const QString path = edit->text();
            if ( !path.isEmpty() )
            {
                player->playFile(path);
            }else{
                qDebug()<<"Class MainWindow functions onItemButtonPlay: file is Empty";
            }
            return;
        }
    }
}
/**
 * @brief MainWindow::onCommandRun
 * @param indexCommand
 */
void MainWindow::onCommandRun(const int indexCommand)
{
    ui->mainToolBar->setEnabled(false);// lock button
    ui->gbDiagnosis->setChecked(false);// clear check diagnosis
    //update
    updateGuiToData();
    // run command
    ptrController->commandRun(indexCommand);
    qDebug("number command %d",indexCommand);
}
/**
 * @brief MainWindow::onConnect
 */
void MainWindow::onConnect(const int number)
{
    ptrController->checkInterface(number);
    qDebug("number connect %d",number);
}
/**
 * @brief MainWindow::onCommandTest
 */
void MainWindow::onCommandTest()
{
    ui->mainToolBar->setEnabled(false);// lock button
    ptrController->commandRun(Controller::cmTest);
    qDebug("command test %d",Controller::cmTest);
}
/**
 * @brief MainWindow::onWindowError
 * @param msg
 */
void MainWindow::onWindowError(const QString &msg)
{
    QMessageBox::critical(this,tr("Device Protocol Error!"),msg);
}
/**
 * @brief MainWindow::onTestDate
 */
void MainWindow::onTestDate(const TYPE_TEST &cmd)
{
    // flag
    QList <QLabel*> list_label;
    list_label<<ui->lbRedSignal<<ui->lbGreenSignal<<ui->lbWait
             <<ui->lbOnGPS<<ui->lbButtonOn<<ui->lbMotionSens;
    for (int i=0;i<list_label.count();i++)
    {
        QLabel *const lb(list_label.at(i));
        const bool result = (cmd.flagTest&(1<<i));
        showLabel(result,lb);
    }
    //time
    const QString strtime(
                QDateTime::fromTime_t(cmd.time,QTimeZone::systemTimeZone()).toString("dd.MM.yyyy hh:mm"));
    ui->lbTestTime->setText(strtime);
    //plan
    ui->lbPlanNumber->setText(QString::number(cmd.plan_number));
    ui->lbPlanLine->setText(QString::number(cmd.plan_item));
    ui->lbVolPlan->setText(QString::number(cmd.plan_value));
    ui->lbSpVolume1->setText(QString::number(cmd.value_speaker1));
    ui->lbSpVolume2->setText(QString::number(cmd.value_speaker2));
    ui->lbNoiseLevel->setText(QString::number(cmd.noise_level));
    ui->lbSensNoise->setText(QString::number(cmd.noise_sensor_volume));
}
/**
 * @brief MainWindow::onTestTime
 */
void MainWindow::onTestTime(const TYPETIME &time)
{
    QDateTime date_time;
    date_time.setTimeZone(QTimeZone::utc());
    date_time.setTime_t(time.time);
    ui->dateTimeEditConfig->setDateTime(date_time);
    ui->chbDayLight->setChecked(date_time.isDaylightTime());

    const int indexUTC0 = 12;
    const int index = indexUTC0+time.gmt;
    QComboBox *cbm(ui->cbGMT);
    if ((index>=0)||(index<cbm->count()))
    {
        cbm->setCurrentIndex(index);
    }
    else
    {
        cbm->setCurrentIndex(indexUTC0);
    }
    struct tm * ptm;
    ptm = gmtime( (time_t*)&time.time );
    qDebug()<<"command Set Time: " <<time.time<<"GTM: "<<time.gmt<<"TIME hour: "<<ptm->tm_hour;
}
/**
 * @brief MainWindow::onUpdateDataToGui
 */
void MainWindow::onUpdateDataToGui()
{
    updateDataToGui(); // update date
    ui->mainToolBar->setEnabled(true); // unlock button
}
/**
 * @brief MainWindow::onTimerDiagnosisEnabled
 * @param enabled
 */
void MainWindow::onTimerDiagnosis(bool enabled)
{
    if ( enabled )
    {
        onTimerDiagnosisEnabled();
    }
    else
    {
        onTimerDiagnosisDisabled();
    }
}
/**
 * @brief MainWindow::onTimerDiagnosisDisabled
 */
void MainWindow::onTimerDiagnosisDisabled()
{
    timerTest->stop();
    ui->gbDiagnosis->setChecked(false);
}
/**
 * @brief MainWindow::onTimerDiagnosisEnabled
 */
void MainWindow::onTimerDiagnosisEnabled()
{
    timerTest->start(1000);
    ui->gbDiagnosis->setChecked(true);
}
/**
 * @brief MainWindow::onUpdateListenGuiToDate
 */
void MainWindow::onUpdateListenGuiToDate()
{
    updateGuiToTestListenDate();
}
/**
 * @brief MainWindow::onWindowOk
 * @param msg
 */
void MainWindow::onWindowOk(const QString &msg)
{
    QMessageBox::information(this,tr("Device response!"),msg);
}
/**
 * @brief MainWindow::eventFilter
 * @param obj
 * @param evt
 * @return
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{

    if ( evt->type() == QEvent::Close)
    {
        if (QTextEdit *win = static_cast<QTextEdit *>(obj))
        {
            win->append("Close windows!");
            ui->actionWin->setChecked(false);
        }

    }
    if( evt->type() == QEvent::MouseButtonDblClick )
    {

    }
    if( evt->type() == QEvent::MouseButtonRelease )
    {

    }
    return QMainWindow::eventFilter( obj, evt );
}
/**
 * @brief MainWindow::showStatusMessage
 * @param message
 */
void MainWindow::showStatusMessage(const QString &message)
{
    statusBar->setText(message);
}
/**
 * @brief MainWindow::createPopurMenuFiles
 * @param page
 */
void MainWindow::createPopurMenuFiles(QWidget * const page)
{
    QAction* pOpen = new QAction(tr("Open sound file..."),page);
    page->addAction(pOpen);
    page->setContextMenuPolicy(Qt::ActionsContextMenu);
    page->connect(pOpen,SIGNAL(triggered()),this,SLOT(onOpenSoundFile()));
}
