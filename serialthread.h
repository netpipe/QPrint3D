#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QSerialPort>

class SerialThread : public QThread {
    Q_OBJECT
public:
    explicit SerialThread( QObject* pParent = NULL);
    ~SerialThread();

    void connectToPort(const QString& portName);
    void disconnectPort();
    bool isConnected();
    void transaction(const QString& request);
    virtual void run();
    QString serialPortErrorString(){
        return m_Port.errorString();
    }

signals:
    void conected();
    void responseRecieved(const QString& response);
    void error(const QSerialPort::SerialPortError error);
    void timeout(const QString& s);

private:
    QString m_PortName;
    int m_WaitTimeout;
    QString m_Request;

    QSerialPort m_Port;
    QMutex mutex;
    QWaitCondition cond;

    bool m_Quit;
};

#endif // SERIALTHREAD_H
