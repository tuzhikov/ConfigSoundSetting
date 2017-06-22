#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <qprogressbar.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qmessagebox.h>
#include <QtWidgets/QSlider>
#include <QRadioButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>
#include <QStackedWidget>
#include <stdint.h>
#include <QFileDialog>

#include "src/FORM/createholidays.h"
#include "src/FORM/createdayplan.h"
#include "src/FORM/createformplaylist.h"
#include "src/FORM/settingsdialog.h"
#include "src/FORM/settingdialogwifi.h"
#include "src/PLAYER/musicplayer.h"
#include "src/SETTING/settings.h"
#include "src/CONTROLLER/controller.h"

namespace Ui {
class MainWindow;
}

//class TextEdit : public QTextEdit
//{
//    Q_OBJECT

//public:

//    TextEdit( QWidget* parent = 0 ) : QTextEdit( parent )
//    {
//          resize_timer_.setInterval( 50 );
//          resize_timer_.setSingleShot( true );
//          //connect( resize_timer_, SIGNAL( timeout() ), this, SLOT( delayed_resize() ) );
//    }
//    virtual ~TextEdit() {}

//protected slots:
//    void delayed_resize()
//    {
//        QTextEdit::resizeEvent( &QResizeEvent( size(), old_size_ ) );
//    }

//protected:
//     void resizeEvent( QResizeEvent* e )
//     {
//          if( !resize_timer_.isActive() )
//            old_size_ = e->oldSize();
//          resize_timer_.start();
//     }

//private:
//    QTimer   resize_timer_;
//    QSize    old_size_;
//};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum PANEL{pnSETTING,pnDIAGNOSIS,pnEND};
    explicit MainWindow(QWidget *parent = 0, Controller *pdata=0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    uint8_t value_speaker[2] = {50,50};
    QMap<QAction*,QTextEdit*> mainWindowFormMap;
    QList<QAction*> ListCommand;
    QList<QAction*> ListConnect;
    QList<QAction*> ListView;

    QProgressBar *progress;
    QLabel *wificon;
    QMap<int,QList<QSlider*> > soundValue;
    MusicPlayer *player;
    QListWidget* lWgtHoliday;
    QListWidget* lWgtPlayList;
    QList< QListWidget* > lWgtPlansList;
    QStackedWidget *swPlans;
    SettingsDialog *settings;
    SettingDialogWifi *settingsWifi;
    QLabel *statusBar;
    Controller *ptrController;
    QTimer *timerTest;
    QButtonGroup *grToolButton;
    void createToolTip(void);
    void createToolBar(void);
    void createStatusBar(void);
    void createStyle(void);
    void createConnectionGUI(void);
    void createGroupMenu(void);
    void createGroupButton(void);
    void createGroupSlider(void);
    void createSoundMenu(void);
    void createPlans( QWidget * const page );
    void createPlayer( QWidget * const page );
    void createOnePlan(QWidget *page, const int, const int );
    void createHolidays( QWidget * const page );
    void createOneHoliday( QWidget *page, const int maxItem );
    void createPopurMenuFiles( QWidget * const page );
    void createFormPlayList( QWidget * const page );
    void createItemPlay( QListWidget * const page,
                         QStringList &list,
                         QStringList &tip);
    void setTitle(const QString file_name);
    void writeSettings();
    void readSettings();
    void showStatusMessage(const QString &message);
    void makeItemLabel(QListWidget *lstWgt = 0,
                       const QString text = "");
    void makeItemPlayList(QListWidget *lstWgt = 0,
                          const QString text = tr("Trask"),
                          const QString tip = "",
                          const QString path = "");

    void makeItemPlan( QListWidget *lstWgt=0,
                       const int number = 1,
                       const QTime time = QTime::currentTime(),
                       const int val1 = 30,
                       const int val2 = 70 );
    void makeItemHoliday(QListWidget *lstWgt=0,
                          const int index    = 1,
                          const int max_plan = 1,
                          const QDate date   = QDate::currentDate(),
                          const int num_plan = 1);
    void addItemPlan ( QListWidget * );
    void addItemPlanFromDatabase(QListWidget *const, const int , const int );
    void remoteItemPlan( QListWidget * );
    void clearItemPlan ();
    void addItemHoliday ( QListWidget* );
    void remoteItemHoliday( QListWidget* );
    bool remoteItem( QListWidget* );
    bool indexSorting ( QListWidget* );
    void resultSliderValue( QList<QSlider*> listSlider, const int16_t delta );
    void showLabelEnabled( QLabel *const lb );
    void showLabelDisabled( QLabel *const lb );
    void showLabel( const bool on, QLabel *const lb );
    void installDiagnosisPage( void );
    bool checkSuffixSoundFiles(const QString path)const;
    QByteArray dataSoundFile(const QString path) const;
    QString checkNameSoundFile(const QString sample_name,
                            const QString file_name) const;
    QString retPrefixNameFile(const QString sample_name)const;
    void clearGuiToTracks();
    void updateGuiToTracks();
    void updateTracksToGui( const QString );
    void updateTracksPathToGui( const QString ) const;
    void updateGuiToVolume();
    void updateVolumeToGui();
    void updateGuiToVibration();
    void updateVibrationToGui();
    void updateGuiToPlan();
    void updateOneGuiToPlan(QListWidget * const lWgt,
                         const int numberPlan );
    void updatePlanToGui();
    void updateOnePlanToGui(QListWidget * const lWgt,
                         const int numberPlan);
    void updateGuiToHoliday();
    void updateHolidayToGui();
    void updateGuiToWeek();
    void updateWeekToGui();
    void updateGuiToOther();
    void updateOtherToGui();
    void updateGuiToData();
    void updateDataToGui();
    void defaultChooseConnect(void);
    void updateGuiToTestListenDate();

    AccessData &retDataBase()const
    {
        AccessData &data(ptrController->retDataProject().retDataProject());
        return data;
    }
    AccessDataProject &retDataProject()const
    {
        AccessDataProject &data(ptrController->retDataProject());
        return data;
    }
protected:
    void closeEvent(QCloseEvent *event);
    bool eventFilter( QObject *obj, QEvent *evt );
private slots:
    void onWindowsOut();
    void onActionabout();
    void onOpenFile(void);
    void onSaveFiles(void);
    void onOpenSoundFile(void);
    void onSaveSoundFile(void);
    void onItemButtonPlay(void);
    void onToolBar(bool visible);
    void onTimeout();
    void onSwitchPanelMode(int);
    void onAddPlan(void);
    void onRemotePlan(void);
    void onAddItemPlan(void);
    void onRemoteItemPlan(void);
    void onPagePlan( int );
    void onAddHoliday( void );
    void onRemoteHoliday( void );
    void onSetSliderValue( const int );
    void onSetMessageOutWin(const QByteArray&,const QColor&);
    void onSetMaxPlanWeek(const int);
    void onSetValueProgress(const int, const bool);
    void onCommandRun( const int );
    void onConnect( const int );
    void onCommandTest();
    void onWindowOk(const QString &msg);
    void onWindowError(const QString &msg);
    void onTestDate (const TYPE_TEST &);
    void onTestTime (const TYPETIME &);
    void onUpdateDataToGui();
    void onTimerDiagnosis(bool);
    void onTimerDiagnosisDisabled();
    void onTimerDiagnosisEnabled();
    void onUpdateListenGuiToDate();

signals:
    void signalSendMessage(const QByteArray&,const QColor&);
    void signaTestListen(const TYPE_TEST_TRACK &);

};

#endif // MAINWINDOW_H
