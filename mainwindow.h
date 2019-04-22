#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QStringListModel>

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
    QString readData();

    void on_pushButton_2_clicked();
    void serialReceived();

    void on_pushButton_16_clicked();

    void on_xm1_clicked();

    void on_homeallbtn_clicked();

    void on_y10btn_clicked();

    void on_y1btn_clicked();

    void on_emstopbtn_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_opengcodebtn_clicked();

    void on_uploadsdbtn_clicked();

    void on_uploadprintbtn_clicked();

    void on_pushButton_11_clicked();

    void on_pauseSDbtn_2_clicked();
    
    void on_pushButton_20_clicked();
    
    void on_printbtn_clicked();

private:

    Ui::MainWindow *ui;
    QSerialPort *serial;
    QStringListModel *model;
    QVector<QString > lines;
};

#endif // MAINWINDOW_H
