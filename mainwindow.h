#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT




public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void sendCommand(QString);

private slots:
    void on_pushButton_clicked();
    void handleError(QSerialPort::SerialPortError error);
   // void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();

    void on_pushButton_2_clicked();
    void serialReceived();

    void on_pushButton_16_clicked();

    void on_xm1_clicked();

    void on_homeallbtn_clicked();

private:

    Ui::MainWindow *ui;
        QSerialPort *serial;
};

#endif // MAINWINDOW_H
