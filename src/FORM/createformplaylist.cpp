#include "createformplaylist.h"

using namespace UnitFormPlayList;

CreateFormPlayList::CreateFormPlayList(QWidget *parent,
                                       const QString text,
                                       const QString tip,
                                       const QString path ): InterfaceForms(parent)
{
    txt = new QLabel(text,parent);
    txt->setMinimumWidth(70);

    PathTrack = path;
    edit = new QLineEdit(path, parent);
    edit->setReadOnly(true);
    edit->setToolTip(tip);

    but = new QToolButton(parent);
    but->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    but->setToolTip(tr("Sound file play."));

    butOpen = new QToolButton(parent);
    butOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    butOpen->setToolTip(tr("Open sound file..."));

    horizontalSpacer = new QSpacerItem(217, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout*hLayout = new QHBoxLayout(parent);
    hLayout->addWidget(txt);
    hLayout->addWidget(edit);
    hLayout->addWidget(butOpen);
    hLayout->addWidget(but);
    hLayout->addSpacerItem(horizontalSpacer);
    this->setLayout(hLayout);
    this->setMinimumHeight(30);

}
/**
 * @brief createFormPlayList::createPopurMenu
 * @param wd
 */
//void CreateFormPlayList::createPopurMenu(QWidget * const wd)
//{
//    // create popur menu
//    QAction *pOpen = new QAction(tr("Open sound file..."),wd);
//    wd->addAction(pOpen);
//    wd->setContextMenuPolicy(Qt::ActionsContextMenu);
//    wd->connect(pOpen,SIGNAL( triggered() ),this,SLOT( onOpenSoundFile() ) );
//}
