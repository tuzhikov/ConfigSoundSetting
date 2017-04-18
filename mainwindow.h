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

#include "createholidays.h"
#include "createdayplan.h"
#include "musicplayer.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum PANEL{pnSETTING,pnDIAGNOSIS,pnEND};
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int MaxPnan = 24; // set from database
    int MaxHoliday = 186; // set from database

    QMap<QAction*,QTextEdit*> mainWindowFormMap;
    QList<QAction*> ListCommand;
    QList<QAction*> ListConnect;
    QList<QAction*> ListView;
    QProgressBar *progress;
    QLabel *wificon;
    QMultiMap<int,QList<QSlider*> > soundValue;
    MusicPlayer *player;
    QListWidget* lWgtHoliday;
    QList< QListWidget* > lWgtPlansList;
    QStackedWidget *swPlans;
    uint8_t value_speaker1 = 50;
    uint8_t value_speaker2 = 50;

    void createToolTip(void);
    void createToolBar(void);
    void createStatusBar(void);
    void createStyle(void);
    void createConnectionGUI(void);
    void createGroupMenu(void);
    void createGroupSlider(void);
    void createSoundMenu(void);
    void createPlans(void);
    void createPlayer( QWidget *page );
    void createOnePlan( QWidget *page, const int maxItem );
    void createHolidays( void );
    void createOneHoliday( QWidget *page, const int maxItem );
    void writeSettings();
    void readSettings();
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
protected:
    void closeEvent(QCloseEvent *event);
    void focusInEvent(QFocusEvent *someEvent);
    bool eventFilter( QObject *obj, QEvent *evt );
private  slots:
    void onWindowsOut();
    void onActionabout();
    void onOpenFile(void);
    void onSaveFiles(void);
    void onOpenSoundFile(void);
    void onSaveSoundFile(void);
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
signals:
    void signalSendMessage(const QByteArray&,const QColor&);

};

#endif // MAINWINDOW_H
