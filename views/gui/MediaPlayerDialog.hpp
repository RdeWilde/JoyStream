#ifndef MEDIAPLAYERDIALOG_HPP
#define MEDIAPLAYERDIALOG_HPP

#include <QDialog>

namespace Ui {
class MediaPlayerDialog;
}

class MediaPlayerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MediaPlayerDialog(QWidget *parent = 0);
    ~MediaPlayerDialog();

private:
    Ui::MediaPlayerDialog *ui;
};

#endif // MEDIAPLAYERDIALOG_HPP
