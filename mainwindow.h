#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QStringListModel>
#include "glwidget.h"
#include "serialthread.h"

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
    void msgBox(QString);

private slots:
    void onSerialPortConnected();
    void onSerialPortResponseRecieved(const QString& response);
  //  void onSerialPortError(QSerialPort::SerialPortError error);
    void onSerialPortTimeout();

private slots:
    void handleError(QSerialPort::SerialPortError error);
   // void openSerialPort();
    //void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void onLayerMinMaxChanged(int min, int max);
    //void serialReceived();

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

    void on_pauseSDbtn_2_clicked();

    void on_printbtn_clicked();

    void on_printbtn_2_clicked();

    void on_connectionbtn_clicked();

    void on_console_textChanged();

    void on_tiptempslide_actionTriggered(int action);

    void on_tiptempslide_sliderReleased();

    void on_bedtempslide_sliderReleased();

    void on_lineEdit_returnPressed();

    void on_sendbtn_clicked();
    void on_timedevent();

    void on_tiptempslide_valueChanged(int value);

    void on_em50btn_clicked();

    void on_em10btn_clicked();

    void on_em1btn_clicked();

    void on_e1btn_clicked();

    void on_e10btn_clicked();

    void on_zm10_clicked();

    void on_zm1btn_clicked();

    void on_z1btn_clicked();

    void on_z10btn_clicked();

    void on_x1btn_clicked();

    void on_x10btn_clicked();

    void on_xm10btn_clicked();

    void on_ym10btn_clicked();

    void on_ym1btn_clicked();

    void on_homexbtn_clicked();

    void on_homeybtn_clicked();

    void on_homezbtn_clicked();

    void on_pausebtn_clicked();

    void on_pauseSDbtn_clicked();

    void on_setTipbutton_clicked();

   void loadSettings();
   void saveSettings();

   void on_uploadprintbtn_3_clicked();

   void on_uploadsdbtn2_clicked();

   void on_actionExit_triggered();

   void on_verticalSlider_valueChanged(int value);

private:

    Ui::MainWindow *ui;
    QSerialPort *serial;
    //SerialThread* m_pSerial;
    QStringListModel *model;
    QVector<QString > lines;
    QTimer *timer;
     QString m_settings;
     GLWidget* drawer;
     int validm114;
};

#endif // MAINWINDOW_H
