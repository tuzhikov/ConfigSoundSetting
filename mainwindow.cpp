#include <time.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    createHolidaysWidget();
    //createStyle();
    /*Read setting in register*/
    readSettings();
    //////////////////////////////////////////
    // test
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    ui->lbOnGPS->setStyleSheet("background: red;");
    ui->lbGreenSignal->setStyleSheet("background: green;");
    // test
    /*time_t rawtime;
    struct tm * ptm;
    time ( &rawtime );
    ui->lb_test_one->setText(QString::number(rawtime));
    ptm = gmtime ( &rawtime );
    ui->lb_test_two->setText(
                QString::number(ptm->tm_hour)+"\\"+
                QString::number(ptm->tm_mday)+"\\"+
                QString::number(ptm->tm_min)+"\\"+
                QString::number(ptm->tm_mon)+"\\"+
                QString::number(ptm->tm_sec)+"\\"+
                QString::number(ptm->tm_wday)+"\\"+
                QString::number(ptm->tm_yday)+"\\"+
                QString::number(ptm->tm_year)+"\\"+
                QString::number(ptm->tm_isdst)+" end"

                            );
    */
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
    QSettings settings("Software LLC Elintel", "Setting ISD_USB_7Dey");
    settings.beginGroup("MainForm");
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();
}
/**
 * @brief MainWin::readSettings
 */
void MainWindow::readSettings()
{
    QSettings settings("Software LLC Elintel", "Setting ISD_USB_7Dey");
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

    ui->tbPLAY->setToolTip(tr("Play the selected track."));
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
    ui->statusBar->showMessage(tr("WiFi connected!"),2500000);
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

void MainWindow::createGroupSlider()
{
    QList<QSlider*> one;
    one<<ui->hs_one_zso<<ui->hs_one_zsp<<ui->hs_one_zsr<<ui->hs_one_zsvrr<<ui->hs_one_zsz<<ui->hs_one_zsvrz;
    soundValue.insert(0,one);
    QList<QSlider*> two;
    one<<ui->hs_two_zso<<ui->hs_two_zsp<<ui->hs_two_zsr<<ui->hs_two_zsvrr<<ui->hs_two_zsz<<ui->hs_two_zsvrz;
    soundValue.insert(1,two);
}

void MainWindow::createSoundMenu()
{

}

void MainWindow::makeItem( QListWidget* lstWgt ) {
    /*
    QWidget* wgt = new QWidget;
    QLayout* l = new QHBoxLayout;
    l->addWidget( new QLineEdit );
    QPushButton* btn = new QPushButton( "Click me" );
    //connect( btn, SIGNAL( clicked() ), SLOT( onBtnClicked() ) );
    l->addWidget( btn );
    wgt->setLayout( l );
    */
    UnitHolidays::CreateHolidays *form = new UnitHolidays::CreateHolidays(this,"10");

    QListWidgetItem *item = new QListWidgetItem( lstWgt );
    item->setSizeHint( form->sizeHint() );
    lstWgt->setItemWidget( item, form );
}

/**
 * @brief MainWindow::createHolidaysWidget
 */
void MainWindow::createHolidaysWidget()
{
    //QWidget * const region(ui->scrollArea);
    /*
    QVBoxLayout *vLayout = new QVBoxLayout(region);

    vLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        for(int i = 0; i < 20; ++i) {

            QGroupBox *groupB = new QGroupBox(tr("Exclusive Radio Buttons"));

            QVBoxLayout *vbox = new QVBoxLayout;
            QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"));
            QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"));
            QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"));
            vbox->addWidget(radio1);
            vbox->addWidget(radio2);
            vbox->addWidget(radio3);
            vbox->addStretch(1);
            groupB->setLayout(vbox);
            vLayout->addWidget(groupB);
        }*/
    //region->setLayout(vLayout);

    ///////////////////////
    QListWidget* lWgt = new QListWidget;//ui->listWidget;
    QLayout* l = new QVBoxLayout;
    l->addWidget( lWgt );
    ui->gbCurrentPlan->setLayout(l);
    ui->gbCurrentPlan->resize(500,500);
    //setLayout( l );

        // Добавим в список 10 элементов
        for( int i = 0; i < 100; ++i ) {
            makeItem( lWgt );
        }
    makeItem(lWgt);
    //////////////////////

    /*QListWidget *listWidget = ui->listWidget;

    //region->scroll(20,20);

    //scroll->setLayout(vLayout);
    QVBoxLayout *vLayout = new QVBoxLayout(region);
    QHBoxLayout *hLayout = new QHBoxLayout(region);

    QLabel *textM  = new QLabel(tr("Mount"),region);
    QLabel *textD  = new QLabel(tr("Day"),region);
    QLabel *textP  = new QLabel(tr("Plan"),region);
    hLayout->addWidget(textM);
    hLayout->addWidget(textD);
    hLayout->addWidget(textP);
    vLayout->addLayout(hLayout);*/
    /*QSpacerItem *vspacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vLayout->addItem(hspacer);*/
/*
    UnitHolidays::CreateHolidays *form1 = new UnitHolidays::CreateHolidays(region,"1");
    vLayout->addWidget(form1);

    UnitHolidays::CreateHolidays *form2 = new UnitHolidays::CreateHolidays(region,"2");
    vLayout->addWidget(form2);

    UnitHolidays::CreateHolidays *form10 = new UnitHolidays::CreateHolidays(region,"10");
    vLayout->addWidget(form10);

    UnitHolidays::CreateHolidays *form11 = new UnitHolidays::CreateHolidays(region,"10");
    vLayout->addWidget(form11);

    UnitHolidays::CreateHolidays *form12 = new UnitHolidays::CreateHolidays(region,"10");
    vLayout->addWidget(form12);

    UnitHolidays::CreateHolidays *form13 = new UnitHolidays::CreateHolidays(region,"10");
    vLayout->addWidget(form13);

    UnitHolidays::CreateHolidays *form14 = new UnitHolidays::CreateHolidays(region,"10");
    vLayout->addWidget(form14);

    UnitHolidays::CreateHolidays *form15 = new UnitHolidays::CreateHolidays(region,"10");
    vLayout->addWidget(form15);

    UnitHolidays::CreateHolidays *form16 = new UnitHolidays::CreateHolidays(region,"10");
    vLayout->addWidget(form16);

    UnitHolidays::CreateHolidays *form17 = new UnitHolidays::CreateHolidays(region,"10");
    vLayout->addWidget(form17);

    UnitHolidays::CreateHolidays *form18 = new UnitHolidays::CreateHolidays(region,"10");
    vLayout->addWidget(form18);

    UnitHolidays::CreateHolidays *form19 = new UnitHolidays::CreateHolidays(region,"10");
    vLayout->addWidget(form19);

    UnitHolidays::CreateHolidays *form20 = new UnitHolidays::CreateHolidays(region,"10");
    vLayout->addWidget(form20);

    QPushButton *btAdd1 = new QPushButton(tr("ADD"),region);
    btAdd1->setMinimumSize(0,50);
    vLayout->addWidget(btAdd1);

    QSpacerItem *vspacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vLayout->addItem(vspacer);

    QStringList myData;
    QStringListModel stringListModel;
    QListView *listView = ui->listView;

    myData << "one" << "two" << "three";

    stringListModel.setStringList(myData);
    listView->setModel(&stringListModel);

    listView->show();*/

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
        //QDebug << "metod:onSetMessageOutWin - Error object winTest";
    }
}
