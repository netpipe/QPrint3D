#include "serialthread.h"
#include <QSerialPort>
#include <QTime>

SerialThread::SerialThread( QObject* parent)
    : QThread(parent)
    , m_Quit(false) {

}

SerialThread::~SerialThread(){
    mutex.lock();
    cond.wakeOne();
    mutex.unlock();
    m_Quit = true;
    wait();
}

void SerialThread::connectToPort(const QString &portName){
    if(m_Port.isOpen()){
        m_Port.close();
    }
    mutex.lock();
    m_PortName = portName;
    mutex.unlock();

    m_Port.setPortName(portName);
    m_Port.setBaudRate(115200);
    m_Port.setDataBits(QSerialPort::Data8);
    m_Port.setParity(QSerialPort::NoParity);
    m_Port.setStopBits(QSerialPort::OneStop);;
    m_Port.setFlowControl(QSerialPort::NoFlowControl);

    if(m_Port.open(QIODevice::ReadWrite)){
        emit conected();
    }else{
        emit error(m_Port.error());//pP tr("Can not open %1, error code %2").arg(m_PortName).arg(m_Port.error()));
    }
}

void SerialThread::disconnectPort(){
    m_Port.close();
}

void SerialThread::transaction(const QString &request){
    if(!isConnected()){
        emit error(m_Port.error());
        return;
    }
    QMutexLocker locker(&mutex);
    this->m_Request = request;
    if(!isRunning()){
        start();
    }else {
        cond.wakeOne();
    }
}

bool SerialThread::isConnected() {
    return m_Port.isOpen();
}

void SerialThread::run(){
    QSerialPort serial;

    while(!m_Quit){
        mutex.lock();
        cond.wait(&mutex);
        QString currentRequest = m_Request;
        mutex.unlock();
        // Write request
        QByteArray requestData = currentRequest.toLocal8Bit();
        serial.write(requestData);
        if(serial.waitForBytesWritten(100)){
            if(serial.waitForReadyRead(100)){
                QByteArray responseData = serial.readAll();
                while(serial.waitForReadyRead(10)){
                    responseData += serial.readAll();
                }
                QString response(responseData);
                emit this->responseRecieved(response);
            }else {
                emit timeout(tr("Wait read response timeout %1").arg(QTime::currentTime().toString()));
            }
        }else {
            emit timeout(tr("Wait write request timeout %1").arg(QTime::currentTime().toString()));
        }

    }
}
