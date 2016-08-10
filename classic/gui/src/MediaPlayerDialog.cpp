/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 4 2015
 */

#include <gui/MediaPlayerDialog.hpp>
#include "ui_MediaPlayerDialog.h"

#include <QFileInfo>

MediaPlayerDialog::MediaCategory MediaPlayerDialog::fromFile(const QString & file) {

    QFileInfo info(file);

    // Grab file extension
    QString suffix = info.suffix();

    // Video
    // https://en.wikipedia.org/wiki/Video_file_format
    if(QList<QString>({"flv",
                     "avi",
                     "mov",
                     "wmv",
                     "mp4",
                     "m4p",
                     "mpg",
                     "mpeg"}).contains(suffix))
        return MediaCategory::video;
    else if(QList<QString>({"mp3",
                           "wav",
                           "wma"}).contains(suffix)) {
        return MediaCategory::audio;
    } else
        throw std::runtime_error("Media category could not be determined from file name suffix.");
}

MediaPlayerDialog::MediaPlayerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MediaPlayerDialog)
    , _state(State::idle)
    , _videoView(this)
    , _duration(0)
{
    ui->setupUi(this);

    // Setup player
    QObject::connect(&_player, &QMediaPlayer::durationChanged, this, &MediaPlayerDialog::setDuration);
    //QObject::connect(&_player, &QMediaPlayer::)
}

MediaPlayerDialog::~MediaPlayerDialog()
{
    delete ui;
}

void MediaPlayerDialog::play(const QUrl & url) {

    // Stop player in case its already playing
    _player.stop();

    // Save url and update media
    _duration = 0;
    _url = url;

    _player.setMedia(url);

    //

    // Figure out what kind of media this is
    //_currentContent =

    if(_currentContent == MediaCategory::video) {

        qDebug() << "Video content requested for playing";

    } else if(_currentContent == MediaCategory::audio) {

        qDebug() << "Audio file scheduled for playing";

    } else
        // We have not covered full enum, could never happen
        Q_ASSERT(false);

    // Start playing
    _player.play();
}

 void MediaPlayerDialog::pause() {

     qDebug() << "Pause requested";

 }

 void MediaPlayerDialog::reset() {

     qDebug() << "Reset requested";

     // hide _videoView?

 }

void MediaPlayerDialog::setDuration(quint64 duration) {
    qDebug() << "Duration being set:" << duration;
}

void MediaPlayerDialog::on_pushButton_clicked() {

}

void MediaPlayerDialog::on_progressHorizontalSlider_sliderReleased() {

}

void MediaPlayerDialog::errorOccured(QMediaPlayer::Error error) {

}

void MediaPlayerDialog::changePosition(qint64 position) {

}

void MediaPlayerDialog::updateVolume(int volume) {

}
