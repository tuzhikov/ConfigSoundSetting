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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int MaxPnan = 24;      // set from database
    int MaxHoliday = 186;  // set from database
    uint8_t value_speaker1 = 50;
    uint8_t value_speaker2 = 50;

    QMap<QAction*,QTextEdit*> mainWindowFormMap;
    QList<QAction*> ListCommand;
    QList<QAction*> ListConnect;
    QList<QAction*> ListView;
    QProgressBar *progress;
    QLabel *wificon;
    QMultiMap<int,QList<QSlider*> > soundValue;
    MusicPlayer *player;
    QListWidget* lWgtHoliday;
    QListWidget* lWgtPlayList;
    QList< QListWidget* > lWgtPlansList;
    QStackedWidget *swPlans;
    SettingsDialog *settings;
    SettingDialogWifi *settingsWifi;
    QLabel *statusBar;

    void createToolTip(void);
    void createToolBar(void);
    void createStatusBar(void);
    void createStyle(void);
    void createConnectionGUI(void);
    void createGroupMenu(void);
    void createGroupSlider(void);
    void createSoundMenu(void);
    void createPlans( QWidget * const page );
    void createPlayer( QWidget * const page );
    void createOnePlan( QWidget *page, const int maxItem );
    void createHolidays( QWidget * const page );
    void createOneHoliday( QWidget *page, const int maxItem );
    void createPopurMenuFiles( QWidget * const page );
    void createFormPlayList( QWidget * const page );
    void createItemPlay( QListWidget * const page,
                         QStringList &list,
                         QStringList &tip);
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
    void makeItemHoliday( QListWidget *lstWgt=0,
                          const int index     = 1,
                          const QDate date = QDate::currentDate(),
                          const int num_plan  = 1 );
    void addItemPlan ( QListWidget * );
    void remoteItemPlan( QListWidget * );
    void addItemHoliday ( QListWidget* );
    void remoteItemHoliday( QListWidget* );
    bool remoteItem( QListWidget* );
    bool indexSorting ( QListWidget* );
    void resultSliderValue( QList<QSlider*> listSlider, const int16_t delta );
    void showLabelEnabled( QLabel *const lb );
    void showLabelDisabled( QLabel *const lb );
    void installDiagnosisPage( void );
    bool checkSoundFiles( const QString path);
    QByteArray dataSoundFile(const QString path) const;
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
    void ontest();
signals:
    void signalSendMessage(const QByteArray&,const QColor&);

};

#endif // MAINWINDOW_H
