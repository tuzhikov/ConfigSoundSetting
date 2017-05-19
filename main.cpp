#include "src/mainwindow.h"
#include "src/CONTROLLER/controller.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(Resources);
    QTranslator trn;
    //trn.load(QString(":/TR/TR/TestCountDown_en"));
    a.installTranslator(&trn);
    //setting parametr
    QApplication::setOrganizationName("Software Ltd. CyberSB");
    QApplication::setApplicationName("Setting SoundConffigurator");
    Controller controller;
    //create main windows
    MainWindow w(0,&controller);
    w.setWindowIcon(QPixmap(":/ico/logo.ico"));
    w.setWindowTitle(QObject::tr("UZTVOP"));
    w.move((QApplication::desktop()->width()-w.width())/ 2,
               (QApplication::desktop()->height()-w.height())/ 2
               );
    w.show();

    return a.exec();
}
