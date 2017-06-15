#include "createformplaylist.h"

using namespace UnitFormPlayList;

CreateFormPlayList::CreateFormPlayList(QWidget *parent,
                                       const QString text,
                                       const QString tip,
                                       const QString path ): InterfaceForms(parent)
{
    txt = new QLabel(text,parent);
    txt->setMinimumWidth(55);

    PathTrack = path;
    edit = new QLineEdit(path, parent);
    edit->setReadOnly(true);
    edit->setToolTip(tip);
    createPopurMenu(edit);

    butPlay = new QToolButton(parent);
    butPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    butPlay->setToolTip(tr("Sound file play..."));

    butOpen = new QToolButton(parent);
    butOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    butOpen->setToolTip(tr("Open sound file..."));

    //horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout*hLayout = new QHBoxLayout(parent);
    hLayout->addWidget(txt);
    hLayout->addWidget(butOpen);
    hLayout->addWidget(butPlay);
    hLayout->addWidget(edit);
    //hLayout->addSpacerItem(horizontalSpacer);
    this->setLayout(hLayout);
    this->setMinimumHeight(30);

}
/**
 * @brief createFormPlayList::createPopurMenu
 * @param wd
 */
void CreateFormPlayList::createPopurMenu(QWidget * const wd)
{
    // create popur menu
    QAction *pClear = new QAction(tr("Clear path..."),wd);
    wd->addAction(pClear);
    wd->setContextMenuPolicy(Qt::ActionsContextMenu);
    wd->connect(pClear,SIGNAL( triggered() ),edit,SLOT(clear()) );
}
