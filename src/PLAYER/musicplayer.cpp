/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "musicplayer.h"
#include "volumebutton.h"

#include <QtWidgets>
#include <QtWinExtras>

/**
 * @brief MusicPlayer::MusicPlayer
 * @param parent
 */
MusicPlayer::MusicPlayer(QWidget *parent) : QWidget(parent),
    taskbarButton(0), taskbarProgress(0), thumbnailToolBar(0),
    playToolButton(0), forwardToolButton(0), backwardToolButton(0),
    mediaPlayer(0), playButton(0), volumeButton(0),
    positionSlider(0), positionLabel(0), infoLabel(0)
{
    createWidgets(parent);
    createShortcuts();
    createJumpList();
    createTaskbar();
    createThumbnailToolBar();

    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayer::updatePosition);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &MusicPlayer::updateDuration);
    connect(&mediaPlayer, &QMediaObject::metaDataAvailableChanged, this, &MusicPlayer::updateInfo);

    typedef void(QMediaPlayer::*ErrorSignal)(QMediaPlayer::Error);
    connect(&mediaPlayer, static_cast<ErrorSignal>(&QMediaPlayer::error),
            this, &MusicPlayer::handleError);
    connect(&mediaPlayer, &QMediaPlayer::stateChanged,
            this, &MusicPlayer::updateState);

    stylize();
}
/**
 * @brief MusicPlayer::playFile
 * @param filePath
 */
void MusicPlayer::playFile(const QString &filePath)
{
    if(!filePath.isEmpty())
    {
        playButton->setEnabled(true);
        infoLabel->setText(QFileInfo(filePath).fileName());

        mediaPlayer.setMedia(QUrl::fromLocalFile(filePath));
        mediaPlayer.play();
    }
    else
    {
        qDebug()<<"Class MusicPlayer functions playFile: filePath is Empty";
    }
}
/**
 * @brief MusicPlayer::togglePlayback
 */
void MusicPlayer::togglePlayback()
{
    if (mediaPlayer.mediaStatus() == QMediaPlayer::NoMedia)
        infoLabel->setText(tr("No open file!"));
    else if (mediaPlayer.state() == QMediaPlayer::PlayingState)
        mediaPlayer.pause();
    else
        mediaPlayer.play();
}
/**
 * @brief MusicPlayer::seekForward
 */
void MusicPlayer::seekForward()
{
    positionSlider->triggerAction(QSlider::SliderPageStepAdd);
}
/**
 * @brief MusicPlayer::seekBackward
 */
void MusicPlayer::seekBackward()
{
    positionSlider->triggerAction(QSlider::SliderPageStepSub);
}
/**
 * @brief MusicPlayer::event
 * @param event
 * @return
 */
bool MusicPlayer::event(QEvent *event)
{
    if (event->type() == QWinEvent::CompositionChange || event->type() == QWinEvent::ColorizationChange)
        stylize();
    return QWidget::event(event);
}
/**
 * @brief MusicPlayer::mousePressEvent
 * @param event
 */
void MusicPlayer::mousePressEvent(QMouseEvent *event)
{
    offset = event->globalPos() - pos();
    event->accept();
}
/**
 * @brief MusicPlayer::mouseMoveEvent
 * @param event
 */
void MusicPlayer::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos() - offset);
    event->accept();
}
/**
 * @brief MusicPlayer::mouseReleaseEvent
 * @param event
 */
void MusicPlayer::mouseReleaseEvent(QMouseEvent *event)
{
    offset = QPoint();
    event->accept();
}
/**
 * @brief MusicPlayer::stylize
 */
void MusicPlayer::stylize()
{
    QtWin::resetExtendedFrame(this);
    setAttribute(Qt::WA_TranslucentBackground, false);
    setStyleSheet(QString("MusicPlayer { background: %1; }").arg(QtWin::realColorizationColor().name()));

    volumeButton->stylize();
}
/**
 * @brief MusicPlayer::updateState
 * @param state
 */
void MusicPlayer::updateState(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState) {
        playButton->setToolTip(tr("Pause"));
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    } else {
        playButton->setToolTip(tr("Play"));
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}
/**
 * @brief MusicPlayer::updatePosition
 * @param position
 */
void MusicPlayer::updatePosition(qint64 position)
{
    positionSlider->setValue(position);

    QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));
    positionLabel->setText(duration.toString(tr("mm:ss")));
}
/**
 * @brief MusicPlayer::updateDuration
 * @param duration
 */
void MusicPlayer::updateDuration(qint64 duration)
{
    positionSlider->setRange(0, duration);
    positionSlider->setEnabled(duration > 0);
    positionSlider->setPageStep(duration / 10);
}
/**
 * @brief MusicPlayer::setPosition
 * @param position
 */
void MusicPlayer::setPosition(int position)
{
    // avoid seeking when the slider value change is triggered from updatePosition()
    if (qAbs(mediaPlayer.position() - position) > 99)
        mediaPlayer.setPosition(position);
}
/**
 * @brief MusicPlayer::updateInfo
 */
void MusicPlayer::updateInfo()
{
    QStringList info;
    QString author = mediaPlayer.metaData("Author").toString();
    if (!author.isEmpty())
        info += author;
    QString title = mediaPlayer.metaData("Title").toString();
    if (!title.isEmpty())
        info += title;
    if (!info.isEmpty())
        infoLabel->setText(info.join(tr(" - ")));
}

void MusicPlayer::handleError()
{
    playButton->setEnabled(false);
    infoLabel->setText(tr("Error: %1").arg(mediaPlayer.errorString()));
}
/**
 * @brief MusicPlayer::updateTaskbar
 */
void MusicPlayer::updateTaskbar()
{
    switch (mediaPlayer.state()) {
    case QMediaPlayer::PlayingState:
        taskbarButton->setOverlayIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        taskbarProgress->show();
        taskbarProgress->resume();
        break;
    case QMediaPlayer::PausedState:
        taskbarButton->setOverlayIcon(style()->standardIcon(QStyle::SP_MediaPause));
        taskbarProgress->show();
        taskbarProgress->pause();
        break;
    case QMediaPlayer::StoppedState:
        taskbarButton->setOverlayIcon(style()->standardIcon(QStyle::SP_MediaStop));
        taskbarProgress->hide();
        break;
    }
}
/**
 * @brief MusicPlayer::updateThumbnailToolBar
 */
void MusicPlayer::updateThumbnailToolBar()
{
    playToolButton->setEnabled(mediaPlayer.duration() > 0);
    backwardToolButton->setEnabled(mediaPlayer.position() > 0);
    forwardToolButton->setEnabled(mediaPlayer.position() < mediaPlayer.duration());

    if (mediaPlayer.state() == QMediaPlayer::PlayingState) {
        playToolButton->setToolTip(tr("Pause"));
        playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    } else {
        playToolButton->setToolTip(tr("Play"));
        playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}
/**
 * @brief MusicPlayer::createWidgets
 * @param parent
 */
void MusicPlayer::createWidgets(QWidget *parent)
{
    playButton = new QToolButton(parent);
    playButton->setEnabled(false);
    playButton->setToolTip(tr("Play"));
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playButton, &QAbstractButton::clicked, this, &MusicPlayer::togglePlayback);

    volumeButton = new VolumeButton(parent);
    volumeButton->setToolTip(tr("Adjust volume"));
    volumeButton->setVolume(mediaPlayer.volume());
    connect(volumeButton, &VolumeButton::volumeChanged, &mediaPlayer, &QMediaPlayer::setVolume);

    positionSlider = new QSlider(Qt::Horizontal, parent);
    positionSlider->setMinimumHeight(25);
    positionSlider->setEnabled(false);
    positionSlider->setToolTip(tr("Seek"));
    connect(positionSlider, &QAbstractSlider::valueChanged, this, &MusicPlayer::setPosition);

    infoLabel = new QLabel( parent );

    positionLabel = new QLabel(tr("00:00"), parent);
    positionLabel->setMinimumWidth(positionLabel->sizeHint().width());

    QBoxLayout *controlLayout = new QHBoxLayout(parent);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(positionSlider);
    controlLayout->addWidget(positionLabel);
    controlLayout->addWidget(volumeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(parent);
    mainLayout->setContentsMargins(0,10,0,0);
    mainLayout->addWidget(infoLabel);
    mainLayout->addLayout(controlLayout);
    //setLayout(mainLayout);
}
/**
 * @brief MusicPlayer::createShortcuts
 */
void MusicPlayer::createShortcuts()
{
    QShortcut *quitShortcut = new QShortcut(QKeySequence::Quit, this);
    connect(quitShortcut, &QShortcut::activated, QCoreApplication::quit);

    QShortcut *toggleShortcut = new QShortcut(Qt::Key_Space, this);
    connect(toggleShortcut, &QShortcut::activated, this, &MusicPlayer::togglePlayback);

    QShortcut *forwardShortcut = new QShortcut(Qt::Key_Right, this);
    connect(forwardShortcut, &QShortcut::activated, this, &MusicPlayer::seekForward);

    QShortcut *backwardShortcut = new QShortcut(Qt::Key_Left, this);
    connect(backwardShortcut, &QShortcut::activated, this, &MusicPlayer::seekBackward);

    QShortcut *increaseShortcut = new QShortcut(Qt::Key_Up, this);
    connect(increaseShortcut, &QShortcut::activated, volumeButton, &VolumeButton::increaseVolume);

    QShortcut *decreaseShortcut = new QShortcut(Qt::Key_Down, this);
    connect(decreaseShortcut, &QShortcut::activated, volumeButton, &VolumeButton::descreaseVolume);
}
/**
 * @brief MusicPlayer::createJumpList
 */
void MusicPlayer::createJumpList()
{
    QWinJumpList jumplist;
    jumplist.recent()->setVisible(true);
}
/**
 * @brief MusicPlayer::createTaskbar
 */
void MusicPlayer::createTaskbar()
{
    taskbarButton = new QWinTaskbarButton(this);
    taskbarButton->setWindow(windowHandle());

    taskbarProgress = taskbarButton->progress();
    connect(positionSlider, &QAbstractSlider::valueChanged, taskbarProgress, &QWinTaskbarProgress::setValue);
    connect(positionSlider, &QAbstractSlider::rangeChanged, taskbarProgress, &QWinTaskbarProgress::setRange);

    connect(&mediaPlayer, &QMediaPlayer::stateChanged, this, &MusicPlayer::updateTaskbar);
}
/**
 * @brief MusicPlayer::createThumbnailToolBar
 */
void MusicPlayer::createThumbnailToolBar()
{
    thumbnailToolBar = new QWinThumbnailToolBar(this);
    thumbnailToolBar->setWindow(windowHandle());

    playToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    playToolButton->setEnabled(false);
    playToolButton->setToolTip(tr("Play"));
    playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playToolButton, &QWinThumbnailToolButton::clicked, this, &MusicPlayer::togglePlayback);

    forwardToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    forwardToolButton->setEnabled(false);
    forwardToolButton->setToolTip(tr("Fast forward"));
    forwardToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    connect(forwardToolButton, &QWinThumbnailToolButton::clicked, this, &MusicPlayer::seekForward);

    backwardToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    backwardToolButton->setEnabled(false);
    backwardToolButton->setToolTip(tr("Rewind"));
    backwardToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    connect(backwardToolButton, &QWinThumbnailToolButton::clicked, this, &MusicPlayer::seekBackward);

    thumbnailToolBar->addButton(backwardToolButton);
    thumbnailToolBar->addButton(playToolButton);
    thumbnailToolBar->addButton(forwardToolButton);

    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayer::updateThumbnailToolBar);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &MusicPlayer::updateThumbnailToolBar);
    connect(&mediaPlayer, &QMediaPlayer::stateChanged, this, &MusicPlayer::updateThumbnailToolBar);
}
