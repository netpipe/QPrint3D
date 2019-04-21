#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QLabel>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <QInputDialog>


// a backup plan for talking to printer on linux would be to use the echo "G28" >> /dev/ttyACM0
// code used for this.
//https://stackoverflow.com/questions/51209822/qtserialport-not-writing-to-serial-port
//http://www.howtobuildsoftware.com/index.php/how-do/cBtm/c-qt-serial-port-writing-qt-serial-port-not-working




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{











    serial = new QSerialPort(this);
//! [1]
  //  settings = new SettingsDialog;


   // connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
    //        SLOT(handleError(QSerialPort::SerialPortError)));

//! [2]
   // connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
//! [2]
    //connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));





    ui->setupUi(this);
}

void MainWindow::closeSerialPort()
{
    serial->close();
//    console->setEnabled(false);
  //  ui->actionConnect->setEnabled(true);
 //   ui->actionDisconnect->setEnabled(false);
  //  ui->actionConfigure->setEnabled(true);
   // ui->statusBar->showMessage(tr("Disconnected"));
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendCommand(QString commandstr)
{
    QString command = commandstr + "\n";
   // QString command = "G28/n";
   // QString command = "M70 P200 Message\n";
    QByteArray x = command.toLocal8Bit();

    if (serial->isOpen() && serial->isWritable())
   {
   // serial->write("G28;\n");
        serial->write(x);
       ui->label->setText("sent");
    }

}

void MainWindow::on_pushButton_clicked()
{
   // QString command = "G28\n";
   // ui->lineEdit->text();
    QString command = ui->lineEdit->text() + "\n"; //"G28;\n";
   // QString command = "G28/n";
   // QString command = "M70 P200 Message\n";
    QByteArray x = command.toLocal8Bit();

    if (serial->isOpen() && serial->isWritable())
   {
   // serial->write("G28;\n");
        serial->write(x);
       ui->label->setText("sent");
    }

// serial->close();
}


void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
//! [6]

//! [7]
void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    //console->putData(data);
}

void MainWindow::serialReceived()
{
    QByteArray output;
    output = serial->readAll();
    ui->label->setText("output");
    qInfo() << output;
     //qInfo("testing");

}





void MainWindow::on_pushButton_2_clicked()
{

    serial->setPortName(ui->portBox->currentText());
      connect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));
  // serial->setPortName(QString('/dev/ttyACM0'));
    serial->setBaudRate(115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);;
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite)) {
       ui->label->setText("Connected to Printer!!");
    } else {
        ui->label->setText("Error: Failed to connect");
    }

}

void MainWindow::on_pushButton_16_clicked()
{
sendCommand("G28 X0;");
}

void MainWindow::on_xm1_clicked()
{
    sendCommand("G28 X0;");
}

void MainWindow::on_homeallbtn_clicked()
{
    sendCommand("G28;");
}

void MainWindow::on_y10btn_clicked()
{
     sendCommand("G0 Y10;");
}

//M70 P200 Message
void MainWindow::on_y1btn_clicked()
{
        sendCommand("G0 Y1;");
}

void MainWindow::on_emstopbtn_clicked()
{
     sendCommand("M117;");
}



void MainWindow::on_pushButton_4_clicked()
{

}

void MainWindow::on_pushButton_3_clicked()
{



//https://stackoverflow.com/questions/28408542/how-to-populate-qt-listview-with-content-from-a-text-file
    QStringList *allLines = new QStringList(); //Your list for lines from the file.
     allLines->clear();

     QStringListModel *linesModel = new QStringListModel(*allLines, NULL); //Your model to set to the view.

     QFile file("/home/netpipe/text.gcode");
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     while (!file.atEnd()) {
         QByteArray line = file.readLine(); //Lines are read as QByteArray.
         const char *line_c = line.data(); //convert to const char*
         QString line_str = QString(QLatin1String(line_c)); //And finally convert to QString
         allLines->append(line_str); //Add to the list
     }

     linesModel->setStringList(*allLines); //Set your model's list your stringlist
     ui->listView_2->setModel(linesModel); //set model of your listView linesModel. You need to use your listView's name, which might be ui->listView.



    ui->listView_2->
            setEditTriggers(QAbstractItemView::AnyKeyPressed |
                            QAbstractItemView::DoubleClicked);

}

void MainWindow::on_opengcodebtn_clicked()
{
    QFile file("/home/netpipe/text.gcode");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString content = stream.readAll();
    file.close();
    ui->textBrowser->append(content);


    QString text = ui->textBrowser->toPlainText();
    QTextStream * stream2 = new QTextStream(&text , QIODevice::ReadOnly);
  //  QVector<QString > lines;
    while (!stream2->atEnd())
    {
        lines << stream2->readLine();
    }

QString text3 = QInputDialog::getText(this,"Title","text");
}

void MainWindow::on_uploadsdbtn_clicked()
{
      sendCommand("M21;"); // get sdcard ready
      sendCommand("M28 filename.txt;"); //write to file

      // send code here
      sendCommand("M29;"); //stop writing

}

void MainWindow::on_uploadprintbtn_clicked()
{
    //pick filename

    sendCommand("M21;"); // get sdcard ready
    sendCommand("M28 filename.txt;"); //write to file

    // send code here
    sendCommand("M29;"); //stop writing
    sendCommand("M23 filename.txt;"); //
    sendCommand("M24;"); //start printing
}
