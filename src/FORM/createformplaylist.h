#ifndef CREATEFORMPLAYLIST_H
#define CREATEFORMPLAYLIST_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QSpacerItem>
#include <QAction>
#include <QHBoxLayout>
#include <QStyle>

#include "InterfaceForms.h"

namespace UnitFormPlayList{
    class CreateFormPlayList : public InterfaceForms
    {
    Q_OBJECT

    public:
            explicit CreateFormPlayList(QWidget *parent = 0,
                                        const QString text = tr("Number trask"),
                                        const QString tip = "",
                                        const QString path = "");
            void setNumberItem( const int ){}
            QToolButton *getQToolButton(void) const {return but!=NULL?but:NULL;}
            QToolButton *getQToolButtonOpen(void) const {return butOpen!=NULL?butOpen:NULL;}
        private:
            QLabel *txt;
            QLineEdit *edit;
            QToolButton *but;
            QToolButton *butOpen;
            QSpacerItem *horizontalSpacer;
            QString PathTrack;

            void createPopurMenu(QWidget * const wd);
    };
}
#endif // CREATEFORMPLAYLIST_H
