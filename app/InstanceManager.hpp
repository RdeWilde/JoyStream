#ifndef INSTANCE_MANAGER_HPP
#define INSTANCE_MANAGER_HPP

#include <QObject>
#include <QSystemSemaphore>
#include <QLocalServer>
#include <QLocalSocket>

class InstanceManager : public QObject
{
    Q_OBJECT
public:
    explicit InstanceManager(const QString &name, QObject *parent = 0);
    bool isMain() const { return _isMain; }

signals:

    // Indicates application should show its main window
    void activate();

public slots:

private slots:

    void incomingConnection();

private:
    bool _isMain;
    QString _name;
    QSystemSemaphore _semaphore;
    QLocalSocket _client;
    QLocalServer _server;
};

#endif // INSTANCE_MANAGER_HPP
