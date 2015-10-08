/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 4 2015
 */

#ifndef MEDIAPLAYERDIALOG_HPP
#define MEDIAPLAYERDIALOG_HPP

#include <QDialog>
#include <QMediaPlayer>
#include <QVideoWidget>

namespace Ui {
class MediaPlayerDialog;
}

class MediaPlayerDialog : public QDialog
{
    Q_OBJECT

public:

    enum class State {
        idle, // not set to spesific url yet

        playing,
        paused
    };

    enum class MediaCategory {
        video,
        audio
    };

    static MediaCategory fromFile(const QString & file);

    explicit MediaPlayerDialog(QWidget *parent = 0);
    ~MediaPlayerDialog();

public slots:

    void play(const QUrl & url);

    void pause();

    void reset();

    void setDuration(quint64 duration);

private slots:

    // Buttons

    void on_pushButton_clicked();

    void on_progressHorizontalSlider_sliderReleased();

    // Media player

    void errorOccured(QMediaPlayer::Error error);

    void changePosition(qint64 position);

    void updateVolume(int volume);

private:

    Ui::MediaPlayerDialog *ui;

    // Player state
    State _state;

    // Media player
    QMediaPlayer _player;

    // View for video
    QVideoWidget _videoView;

    // Url player was opened to start
    QUrl _url;

    //
    // Fields below are only
    // valid when _status != Status::idle
    //

    MediaCategory _currentContent;

    // Duration of media
    quint64 _duration;
};

#endif // MEDIAPLAYERDIALOG_HPP
