#include "MediaPlayerDialog.hpp"
#include "ui_MediaPlayerDialog.h"

MediaPlayerDialog::MediaPlayerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MediaPlayerDialog)
{
    ui->setupUi(this);
}

MediaPlayerDialog::~MediaPlayerDialog()
{
    delete ui;
}
