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
#include "createholidays.h"

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
    QMap<QAction*,QTextEdit*> mainWindowFormMap;
    QList<QAction*> ListCommand;
    QList<QAction*> ListConnect;
    QList<QAction*> ListView;
    QProgressBar *progress;
    QLabel *wificon;
    QMap<int,QList<QSlider*> > soundValue;
    void createToolTip(void);
    void createToolBar(void);
    void createStatusBar(void);
    void createStyle(void);
    void createConnectionGUI(void);
    void createGroupMenu(void);
    void createGroupSlider(void);
    void createSoundMenu(void);
    void createHolidaysWidget(void);
    void writeSettings();
    void readSettings();
    void makeItem( QListWidget* lstWgt );
protected:
    void closeEvent(QCloseEvent *event);
private  slots:
    void onWindowsOut();
    void onActionabout();
    void onToolBar(bool visible);
    void onTimeout();
    void onSwitchPanelMode(int);
    void onSetMessageOutWin(const QByteArray&,const QColor&);
signals:
    void signalSendMessage(const QByteArray&,const QColor&);

};

#endif // MAINWINDOW_H
