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
            int getNumberItem() const {return 0;}
            QToolButton *getQToolButton(void) const {return butPlay!=NULL?butPlay:NULL;}
            QToolButton *getQToolButtonOpen(void) const {return butOpen!=NULL?butOpen:NULL;}
            QLineEdit *getQLineEdit(void) const {return edit!=NULL?edit:NULL;}
            QString getPath()const {return edit->text();}
        private:
            QLabel *txt;
            QLineEdit *edit;
            QToolButton *butPlay;
            QToolButton *butOpen;
            QSpacerItem *horizontalSpacer;
            QString PathTrack;

            void createPopurMenu(QWidget * const wd);
    };
}
#endif // CREATEFORMPLAYLIST_H
