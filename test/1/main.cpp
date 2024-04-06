#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

class SerialPortManager : public QObject {
    Q_OBJECT
public:
    SerialPortManager(QWidget *parent = nullptr) : QObject(parent), parentWidget(parent) {
        serial = new QSerialPort(this);
        connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
                SLOT(handleError(QSerialPort::SerialPortError)));
        connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

        QPushButton *connectButton = new QPushButton("Connect");
        connect(connectButton, &QPushButton::clicked, this, &SerialPortManager::openSerialPort);

        QPushButton *disconnectButton = new QPushButton("Disconnect");
        connect(disconnectButton, &QPushButton::clicked, this, &SerialPortManager::closeSerialPort);

        sendButton = new QPushButton("Send");
        connect(sendButton, &QPushButton::clicked, this, &SerialPortManager::sendData);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(connectButton);
        layout->addWidget(disconnectButton);

        inputLineEdit = new QLineEdit;
        layout->addWidget(inputLineEdit);

        layout->addWidget(sendButton);

        textEdit = new QTextEdit;
        layout->addWidget(textEdit);

        connectionStatusLabel = new QLabel("Not connected");
        layout->addWidget(connectionStatusLabel);

        parentWidget->setLayout(layout);
    }

    ~SerialPortManager() {
        delete serial;
    }

public slots:
    void openSerialPort() {
        QSerialPortInfo portToUse;
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            if (!info.isBusy() && (info.description().contains("Arduino") || info.manufacturer().contains("Arduino"))) {
                portToUse = info;
            }
        }

        if (portToUse.isNull() || !portToUse.isValid()) {
            qDebug() << "Port is not valid:" << portToUse.portName();
            return;
        }

        serial->setPortName(portToUse.portName());
        serial->setBaudRate(QSerialPort::Baud115200);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        if (serial->open(QIODevice::ReadWrite)) {
            qDebug() << "Connected to" << portToUse.description() << "on" << portToUse.portName();
            connectionStatusLabel->setText("Connected to " + portToUse.portName());
        } else {
            qCritical() << "Serial Port error:" << serial->errorString();
            qDebug() << "Open error";
        }
    }

    void closeSerialPort() {
        serial->close();
        qDebug() << "Disconnected";
        connectionStatusLabel->setText("Not connected");
    }

    void readData() {
        QByteArray data = serial->readAll();
        textEdit->append("UART:" + data);
    }

    void sendData() {
        QString data = inputLineEdit->text();
        if (!data.isEmpty()) {
            QByteArray byteArray = data.toUtf8();
            writeData(byteArray);
            inputLineEdit->clear();
        }
    }

    void writeData(const QByteArray &data) {
        serial->write(data);
    }

    void handleError(QSerialPort::SerialPortError error) {
        if (error == QSerialPort::ResourceError) {
            qCritical() << "Serial Port error:" << serial->errorString();
            closeSerialPort();
        }
    }

private:
    QWidget *parentWidget;
    QSerialPort *serial;
    QTextEdit *textEdit;
    QLineEdit *inputLineEdit;
    QPushButton *sendButton;
    QLabel *connectionStatusLabel;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    SerialPortManager manager(&window);

    window.resize(300, 300);
    window.show();

    return app.exec();
}

#include "main.moc"
