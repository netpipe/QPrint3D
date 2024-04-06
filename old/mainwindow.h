#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QStringListModel>
#include <QTimer>
#include <QSettings>

#include "gsim/GLDrawer.h"

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
   void passargs(QString);

private slots:
    void handleError(QSerialPort::SerialPortError error);
   // void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();

    void serialReceived();

    void on_xm1_clicked();

    void on_homeallbtn_clicked();

    void on_y10btn_clicked();

    void on_y1btn_clicked();

    void on_emstopbtn_clicked();

    void on_pushButton_3_clicked();

    void on_opengcodebtn_clicked();

    void on_uploadsdbtn_clicked();

    void on_uploadprintbtn_clicked();

    void on_pauseSDbtn_2_clicked();
    
    void on_printbtn_clicked();

    void on_printbtn_2_clicked();

    void on_connectionbtn_clicked();

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


   void on_setBedbtn_clicked();

   void on_comboBox_2_activated(const QString &arg1);

   void on_bedpowerbtn_clicked();

   void on_tippowerbtn_clicked();

private:

    Ui::MainWindow *ui;
    QSerialPort *serial;
    QStringListModel *model;
    QVector<QString > lines;
    QTimer *timer;
     QString m_settings;
     GLDrawer* drawer;
     int validm114;
     QString arg;
     QByteArray buffer;
};

#endif // MAINWINDOW_H
