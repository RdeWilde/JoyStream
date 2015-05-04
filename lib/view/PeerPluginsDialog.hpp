#ifndef PEERPLUGINSDIALOG_HPP
#define PEERPLUGINSDIALOG_HPP

#include <QDialog>

namespace Ui {
class PeerPluginsDialog;
}

class PeerPluginsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PeerPluginsDialog(QWidget *parent = 0);
    ~PeerPluginsDialog();

private:
    Ui::PeerPluginsDialog *ui;
};

#endif // PEERPLUGINSDIALOG_HPP
