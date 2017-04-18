#include <time.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace UnitHolidays;
using namespace UnitDeyPlan;

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*create control*/
    createStatusBar();
    createToolBar();
    createToolTip();
    createConnectionGUI();
    createGroupMenu();
    createGroupSlider();
    createPlans();
    createHolidays();
    createPlayer(ui->gbPlayTrack);
    createStyle();
    /*Read setting in register*/
    readSettings();
    // install wigen
    installDiagnosisPage();
    // install event filter
    ui->leZSO->installEventFilter(this);
    ui->leZSP->installEventFilter(this);
    ui->leZSR->installEventFilter(this);
    ui->leZSZ->installEventFilter(this);
    ui->leZSVRR10->installEventFilter(this);
    ui->leZSVRR20->installEventFilter(this);
    ui->leZSVRR30->installEventFilter(this);
    ui->leZSVRR40->installEventFilter(this);
    ui->leZSVRR50->installEventFilter(this);
    ui->leZSVRR60->installEventFilter(this);
    ui->leZSVRR70->installEventFilter(this);
    ui->leZSVRZ10->installEventFilter(this);
    ui->leZSVRZ20->installEventFilter(this);
    ui->leZSVRZ30->installEventFilter(this);
    ui->leZSVRZ40->installEventFilter(this);
    ui->leZSVRZ50->installEventFilter(this);
    ui->leZSVRZ60->installEventFilter(this);
    ui->leZSVRZ70->installEventFilter(this);
    //qApp->installEventFilter(this);
    //////////////////////////////////////////
    // test wiget
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
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
    QSettings settings("Software Ltd. CyberSB", "Setting SoundConffigurator");
    settings.beginGroup("MainForm");
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();
}
/**
 * @brief MainWin::readSettings
 */
void MainWindow::readSettings()
{
    QSettings settings("Software Ltd. CyberSB", "Setting SoundConffigurator");
    settings.beginGroup("MainForm");
    restoreGeometry(settings.value("geometry").toByteArray());
    settings.endGroup();
}
/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    // delete form output
    if(mainWindowFormMap[ui->actionWin])
        delete mainWindowFormMap[ui->actionWin];
    event->accept();
}

void MainWindow::focusInEvent(QFocusEvent *someEvent)
{
    if (someEvent->gotFocus())
        ui->leZSO->selectAll();
}
/**
 * @brief MainWindow::eventFilter
 * @param obj
 * @param evt
 * @return
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
    if( evt->type() == QEvent::MouseButtonDblClick )
    {
        QLineEdit *edit = qobject_cast< QLineEdit * >( obj );
        if (edit)
        {
            const QStringList musicPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
            const QString file =
                QFileDialog::getOpenFileName(this,tr("Open File"),
                                                musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first(),
                                                tr("TRACK (*.wav);;All files (*.*)"));
            if(!file.isEmpty())
            {
                edit->setText(file);
            }
        }
    }
    // one click play trask
    if( evt->type() == QEvent::MouseButtonRelease )
    {
        QLineEdit *edit = qobject_cast< QLineEdit * >( obj );
        if (edit)
        {
            player->playFile(edit->text());
        }
    }
    return QMainWindow::eventFilter( obj, evt );
}
/**
 * @brief MainWindow::createToolTip
 */
void MainWindow::createToolTip()
{
    ui->actionWrite->setToolTip(tr("Write data"));
    ui->actionRead->setToolTip(tr("Read data"));
    ui->actionDevice->setToolTip(tr("Get the type of device."));
    //set tool tip sound track
    ui->leZSO->setToolTip(tr("The file name must begin with <0200_>"));
    ui->leZSP->setToolTip(tr("The file name must begin with <0210_>"));
    ui->leZSR->setToolTip(tr("The file name must begin with <0001_>"));
    ui->leZSZ->setToolTip(tr("The file name must begin with <0002_>"));

    ui->leZSVRR70->setToolTip(tr("The file name must begin with <0090_>"));
    ui->leZSVRR60->setToolTip(tr("The file name must begin with <0060_>"));
    ui->leZSVRR50->setToolTip(tr("The file name must begin with <0050_>"));
    ui->leZSVRR40->setToolTip(tr("The file name must begin with <0040_>"));
    ui->leZSVRR30->setToolTip(tr("The file name must begin with <0030_>"));
    ui->leZSVRR20->setToolTip(tr("The file name must begin with <0020_>"));
    ui->leZSVRR10->setToolTip(tr("The file name must begin with <0010_>"));

    ui->leZSVRZ70->setToolTip(tr("The file name must begin with <0190_>"));
    ui->leZSVRZ60->setToolTip(tr("The file name must begin with <0160_>"));
    ui->leZSVRZ50->setToolTip(tr("The file name must begin with <0150_>"));
    ui->leZSVRZ40->setToolTip(tr("The file name must begin with <0140_>"));
    ui->leZSVRZ30->setToolTip(tr("The file name must begin with <0130_>"));
    ui->leZSVRZ20->setToolTip(tr("The file name must begin with <0120_>"));
    ui->leZSVRZ10->setToolTip(tr("The file name must begin with <0110_>"));
}
/**
 * @brief MainWindow::createToolBar
 */
void MainWindow::createToolBar()
{
    // add ListCommand
    ListCommand<<ui->actionWrite<<ui->actionRead<<ui->actionDevice;
    ui->mainToolBar->addActions(ListCommand);
    ui->mainToolBar->addSeparator();
    // add ListView
    ListView<<ui->actionSetting<<ui->actionDiagnosis;
    ui->mainToolBar->addActions(ListView);
    ui->mainToolBar->addSeparator();
    // add ListConnect
    ListConnect<<ui->actionConnect_USB<<ui->actionConnect_WIFI;
    ui->actionConnect_WIFI->setDisabled(true);
    ui->mainToolBar->addActions(ListConnect);
    wificon = new QLabel(this);
    wificon->setText("TEST");
    ui->mainToolBar->addWidget(wificon);
}
/**
 * @brief MainWindow::createStatusBar
 */
void MainWindow::createStatusBar()
{
    /*Add progress in status bar*/
    progress = new QProgressBar(this);
    progress->setVisible(false);
    progress->setTextVisible(false);
    //connect(usb,SIGNAL(signalProgressRange(int,int)),progress,SLOT(setRange(int,int)));
    //connect(usb,SIGNAL(signalProgressValue(int,bool)),this,SLOT(on_setValueProgress(int,bool)));
    ui->statusBar->addPermanentWidget(progress,0);
    /*Set message in status bar*/
    QLabel *p = new QLabel(this);
    p->setText(tr("version 1.1"));
    p->setOpenExternalLinks(true);
    ui->statusBar->addPermanentWidget(p);
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
    /*connect view panel*/
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
    connect(ui->cbSelectPlan,SIGNAL(currentIndexChanged(int)),
            this,SLOT(onPagePlan(int)));
    // connect holiday
    connect(ui->pbAddHoliday,SIGNAL(clicked(bool)),this,SLOT(onAddHoliday()));
    connect(ui->pbRemoteHoliday,SIGNAL(clicked(bool)),this,SLOT(onRemoteHoliday()));
    // connect plan
    connect(ui->pbAddItemPlan,SIGNAL(clicked(bool)),this,SLOT(onAddItemPlan()));
    connect(ui->pbRemoteItemPlan,SIGNAL(clicked(bool)),this,SLOT(onRemoteItemPlan()));
    // connect slider value
    connect(ui->hs_general_spone,SIGNAL(valueChanged(int)),this,SLOT(onSetSliderValue(int)));
    connect(ui->hs_general_sptwo,SIGNAL(valueChanged(int)),this,SLOT(onSetSliderValue(int)));
    // connect save \ open files
    connect(ui->actionSave_project,SIGNAL(triggered(bool)),this,SLOT(onSaveFiles()));
    connect(ui->actionOpen_project,SIGNAL(triggered(bool)),this,SLOT(onOpenFile()));
    // connect lineedit
    //connect(ui->leZSO,SIGNAL())
    //connect week
    connect(ui->sbTotalPlan,SIGNAL(valueChanged(int)),this,SLOT(onSetMaxPlanWeek(int)));
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
    grouAactionConnect->addAction(ui->actionConfig_WIFI);

}
/**
 * @brief MainWindow::createGroupSlider
 */
void MainWindow::createGroupSlider()
{
    QList<QSlider*> slOne;
    slOne<<ui->hs_general_spone<<ui->hs_one_zso<<ui->hs_one_zsp<<ui->hs_one_zsr<<ui->hs_one_zsvrr<<ui->hs_one_zsz<<ui->hs_one_zsvrz;
    soundValue.insert(0,slOne);
    QList<QLabel*> lbOne;
    lbOne<<ui->lb_general_spone<<ui->lb_one_zso<<ui->lb_one_zsp<<ui->lb_one_zsr<<ui->lb_one_zsvrr<<ui->lb_one_zsz<<ui->lb_one_zsvrz;
    QList<QSlider*> slTwo;
    slTwo<<ui->hs_general_sptwo<<ui->hs_two_zso<<ui->hs_two_zsp<<ui->hs_two_zsr<<ui->hs_two_zsvrr<<ui->hs_two_zsz<<ui->hs_two_zsvrz;
    soundValue.insert(1,slTwo);
    QList<QLabel*> lbTwo;
    lbTwo<<ui->lb_general_sptwo<<ui->lb_two_zso<<ui->lb_two_zsp<<ui->lb_two_zsr<<ui->lb_two_zsvrr<<ui->lb_two_zsz<<ui->lb_two_zsvrz;

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

void MainWindow::createSoundMenu()
{

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
    // connect( btn, SIGNAL( clicked() ), SLOT( onBtnClicked() ) );
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
                                 const QDate date,
                                 const int num_plan)
{
    CreateHolidays *form = new CreateHolidays(this,index,date,num_plan);
    QListWidgetItem *item = new QListWidgetItem( lstWgt );
    item->setSizeHint( form->sizeHint() );
    lstWgt->setItemWidget(item,form);
    connect(ui->sbTotalPlan,SIGNAL(valueChanged(int)),form,SLOT(setMaxPlan(int)));
}
/**
 * @brief MainWindow::addItemPlan
 */
void MainWindow::addItemPlan(QListWidget *lWgt)
{
    if( !lWgt ) return ;
    // read from database
    // add new item
    const int indexShow = lWgt->count()+1;
    if(indexShow<=MaxPnan)
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
    // write to database
}
/**
 * @brief MainWindow::addItemHoliday
 * @param lWgt
 */
void MainWindow::addItemHoliday(QListWidget *lWgt)
{
    if( !lWgt ) return ;
    // read from database
    // add new item
    const int indexShow = lWgt->count()+1;
    if(indexShow<=MaxHoliday)
    {
        makeItemHoliday( lWgt, indexShow );
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
    // write to database

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
void MainWindow::createPlans()
{
    // read database
    MaxPnan = 24;
    const int MaxItem = 4;
    // create form plans
    QWidget *const page = ui->planWidget;
    swPlans = new QStackedWidget(page);
    for(int i=0; i<MaxPnan;i++)
    {
        QWidget *wdg = new QWidget;
        createOnePlan(wdg,MaxItem);
        swPlans->addWidget(wdg);
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(swPlans);
    layout->setContentsMargins(0,0,0,0);
    page->setLayout(layout);
    qDebug("Count stack %d",swPlans->count());
}

void MainWindow::createPlayer(QWidget *page)
{
    QLayout* vlayout = new QVBoxLayout;
    player = new MusicPlayer(page);
    player->resize(page->size());
    vlayout->addWidget(player);
    page->setLayout(vlayout);
}
/**
 * @brief MainWindow::createOnePlan
 * @param page
 * @param maxItem
 */
void MainWindow::createOnePlan( QWidget *page, const int maxItem )
{
    QListWidget* lWgt = new QListWidget;
    QLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget( lWgt );
    page->setLayout(vlayout);
    for (int i = 1; i<=maxItem; i++)
    {
        // read item database
        makeItemPlan( lWgt, i );
    }
    // add list
    lWgtPlansList.append(lWgt);
}
/**
 * @brief MainWindow::createHolidays
 */
void MainWindow::createHolidays()
{
    // read items database
    MaxHoliday = 20;
    QWidget *const page = ui->wHoliday;
    // create form holiday
    createOneHoliday(page,MaxHoliday);
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
    vlayout->addWidget( lWgt );
    page->setLayout(vlayout);
    const int maxPlan = ui->sbTotalPlan->value();
    for (int i = 1; i<=maxItem; i++)
    {
        // read item database
        makeItemHoliday( lWgt,i,QDate::currentDate(),maxPlan );
    }
    // private pointer
    lWgtHoliday = lWgt;
}
/**
 * @brief MainWindow::onWindowsOut
 */
void MainWindow::onWindowsOut()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QTextEdit *window = mainWindowFormMap[action];
    if(!window){
          window = new QTextEdit();
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
static bool flag = false;

if(flag){
    wificon->setText("CONNECT OK");
    //wificon->setTextFormat(Qt::TextColorRole);
    //wificon->setStyleSheet("background: green;");
}
else{
    wificon->setText("CONNECT ERROR");
    //wificon->setStyleSheet("background: red;");
}

flag=!flag;
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
}
/**
 * @brief MainWindow::onAddPlan
 */
void MainWindow::onAddPlan()
{
    const int index = ui->sbTotalPlan->value()+1;
    if (index<MaxPnan)
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
    addItemPlan(lWgtPlansList[ui->cbSelectPlan->currentIndex()]);
    qDebug("index add %d",ui->cbSelectPlan->currentIndex());
}
/**
 * @brief MainWindow::onRemoteItemPlan
 */
void MainWindow::onRemoteItemPlan()
{
    remoteItemPlan(lWgtPlansList[ui->cbSelectPlan->currentIndex()]);
    qDebug("index remote %d",ui->cbSelectPlan->currentIndex());
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
}
/**
 * @brief MainWindow::onSetSliderValue
 * @param value
 */
void MainWindow::onSetSliderValue(const int value)
{
    if( QSlider* curSlid = qobject_cast< QSlider* >( sender() ) )
    {
        QMultiMap<int,QList<QSlider*> >::const_iterator sp1 = soundValue.lowerBound(0);
        QMultiMap<int,QList<QSlider*> >::const_iterator sp2 = soundValue.lowerBound(1);

        QList<QSlider*> speakerOneMass = sp1.value();
        QList<QSlider*> speakerTwoMass = sp2.value();

        if(speakerOneMass.at(0)==curSlid)
        {
            const int16_t delta = value-value_speaker1;
            value_speaker1 = curSlid->value();
            resultSliderValue(speakerOneMass,delta);
            qDebug("Slider one %d",delta);
            return;
        }
        if(speakerTwoMass.at(0)==curSlid)
        {
            const int16_t delta = value-value_speaker2;
            value_speaker2 = curSlid->value();
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
    QTextEdit * const winTest = mainWindowFormMap[ui->actionWin];
    if (winTest)
    {
        winTest->setTextColor(color);
        QByteArray str(st.toHex().toUpper().trimmed());
        winTest->append(str);
    }else{
        qDebug() << "metod:onSetMessageOutWin - Error object winTest";
    }
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
 * @brief MainWindow::onOpenFile
 */
void MainWindow::onOpenFile(void)
{
    static QString file = "/";
    file = QFileDialog::getOpenFileName(this,tr("Open File"),file,tr("XML (*.xml);;"));
    if(!file.isEmpty())
    {
        //loadFile(file);
    }
}
/**
 * @brief MainWindow::onSaveFiles
 */
void MainWindow::onSaveFiles(void)
{
    static QString file = "/";
    file = QFileDialog::getSaveFileName(this,tr("Save File"),file,tr("XML (*.xml);;"));
    if(!file.isEmpty())
    {
        //saveFile(file);
    }
}
/**
 * @brief MainWindow::onOpenSoundFile
 */
void MainWindow::onOpenSoundFile()
{
    static QString file = "/";
    file = QFileDialog::getOpenFileName(this,tr("Open File"),file,tr("TRACK (*.wav);;"));
    if(!file.isEmpty())
    {
        //loadFile(file);
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
        //saveFile(file);
    }
}
