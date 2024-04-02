#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QSerialPort>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QSerialPortInfo>

class SerialThread : public QThread {
    Q_OBJECT
public:
    explicit SerialThread(QObject *parent = nullptr) : QThread(parent), m_Quit(false) {}
    ~SerialThread() {
        mutex.lock();
        cond.wakeOne();
        mutex.unlock();
        m_Quit = true;
        wait();
    }

    void connectToPort(const QString &portName) {
        if (m_Port.isOpen())
            m_Port.close();
        m_PortName = portName;
        m_Port.setPortName(portName);
        m_Port.setBaudRate(QSerialPort::Baud115200);
        if (m_Port.open(QIODevice::ReadWrite)) {
            emit connected();
        } else {
            emit error(m_Port.error());
        }
    }

    void disconnectPort() {
        m_Port.close();
    }

    bool isConnected() const {
        return m_Port.isOpen();
    }

    void transaction(const QString &request) {
        if (!isConnected()) {
          //  emit error(QSerialPort::NotOpen);
            return;
        }
        QMutexLocker locker(&mutex);
        m_Request = request;
        if (!isRunning())
            start();
        else
            cond.wakeOne();
    }

    virtual void run() override {
        while (!m_Quit) {
            mutex.lock();
            cond.wait(&mutex);
            QString currentRequest = m_Request;
            mutex.unlock();
            QByteArray requestData = currentRequest.toLocal8Bit();
            m_Port.write(requestData);
            if (m_Port.waitForBytesWritten(100)) {
                if (m_Port.waitForReadyRead(100)) {
                    QByteArray responseData = m_Port.readAll();
                    while (m_Port.waitForReadyRead(10))
                        responseData += m_Port.readAll();
                    QString response(responseData);
                    emit responseReceived(response);
                } else {
                    emit timeout("Wait read response timeout");
                }
            } else {
                emit timeout("Wait write request timeout");
            }
        }
    }

    QString serialPortErrorString() const {
        return m_Port.errorString();
    }
signals:
    void connected();
    void responseReceived(const QString &response);
    void error(QSerialPort::SerialPortError error);
    void timeout(const QString &s);

private:
    QString m_PortName;
    QString m_Request;
    QSerialPort m_Port;
    QMutex mutex;
    QWaitCondition cond;
    bool m_Quit;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(SerialThread *serialThread, QWidget *parent = nullptr) : QMainWindow(parent), m_SerialThread(serialThread) {
        setupUi();
        connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
        connect(sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
        connect(m_SerialThread, &SerialThread::connected, this, &MainWindow::onSerialConnected);
        connect(m_SerialThread, &SerialThread::responseReceived, this, &MainWindow::onSerialResponseReceived);
        connect(m_SerialThread, &SerialThread::error, this, &MainWindow::onSerialError);
    }

private slots:
    void onConnectButtonClicked() {
        QString portName = portComboBox->currentText();
        m_SerialThread->connectToPort(portName);
    }

    void onSendButtonClicked() {
        QString command = commandLineEdit->text();
        m_SerialThread->transaction(command);
    }

    void onSerialConnected() {
        statusLabel->setText("Connected");
    }

    void onSerialResponseReceived(const QString &response) {
        responseTextEdit->append(response);
    }

    void onSerialError(QSerialPort::SerialPortError error) {
        statusLabel->setText("Error: " + m_SerialThread->serialPortErrorString());
    }

private:
    QComboBox *portComboBox;
    QPushButton *connectButton;
    QPushButton *sendButton;
    QLineEdit *commandLineEdit;
    QTextEdit *responseTextEdit;
    QLabel *statusLabel;
    SerialThread *m_SerialThread;

    void setupUi() {
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        portComboBox = new QComboBox(this);
        layout->addWidget(portComboBox);

        connectButton = new QPushButton("Connect", this);
        layout->addWidget(connectButton);

        commandLineEdit = new QLineEdit(this);
        layout->addWidget(commandLineEdit);

        sendButton = new QPushButton("Send", this);
        layout->addWidget(sendButton);

        responseTextEdit = new QTextEdit(this);
        layout->addWidget(responseTextEdit);

        statusLabel = new QLabel("Status", this);
        layout->addWidget(statusLabel);

        updatePortList();
    }

    void updatePortList() {
        portComboBox->clear();
        for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
            portComboBox->addItem(info.portName());
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    SerialThread serialThread;
    MainWindow w(&serialThread);
    w.show();

    return a.exec();
}

#include "main.moc"
