#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QLabel>
#include <QPixmap>
#include <QFile>
#include <QTextStream>

#include <QInputDialog>
#include <QFileDialog>


// a backup plan for talking to printer on linux would be to use the echo "G28" >> /dev/ttyACM0
// code used for this.
//https://stackoverflow.com/questions/51209822/qtserialport-not-writing-to-serial-port
//http://www.howtobuildsoftware.com/index.php/how-do/cBtm/c-qt-serial-port-writing-qt-serial-port-not-working

//todo
//verify all buttons working
//verify successful print
//impliment opengl widget
//parse m114 from listbox to get valid position
//movement buttons need current position to move around better




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{





  //  this->setWindowTitle("QPrint3d");





    serial = new QSerialPort(this);
//! [1]
  //  settings = new SettingsDialog;


   // connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
    //        SLOT(handleError(QSerialPort::SerialPortError)));

//! [2]
   // connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
//! [2]
    //connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)))


    ui->setupUi(this);

    ui->emstopbtn->setStyleSheet("background-color: red");

    timer = new QTimer(this);

    // setup signal and slot
    connect(timer, SIGNAL(timeout()),
          this, SLOT(on_timedevent()));

    // msec
    timer->start(1000);



  //  this->setWindowOpacity(0.55);
  //  this->setAttribute(Qt::WA_TranslucentBackground, true);
  //  this->setStyleSheet("background-color: yellow;");

    m_settings = QApplication::applicationDirPath().left(1) + ":/settings.ini";
    saveSettings();
    loadSettings();

}

void MainWindow::loadSettings()
{
 QSettings settings(m_settings, QSettings::NativeFormat);
 QString sText = settings.value("text", "").toString();
// if (label)
// {
//  ui->label->setText(sText);
// }
}

void MainWindow::saveSettings()
{
 QSettings settings(m_settings, QSettings::NativeFormat);
 QString sText = "testing";//(label) ? label->text() : "";
 settings.setValue("text", sText);
 //if (label)
 //{
 // ui->label->setText(sText);
 //}
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

void MainWindow::msgBox(QString message){

    QMessageBox Msgbox;
    Msgbox.setText(message);
    Msgbox.exec();

 //    QMessageBox::critical(this, tr("Critical Error"), serial->errorString());

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
    else {
        msgBox("not connected");
    }


}

void MainWindow::on_sendbtn_clicked()
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
       ui->label->setText("sent"+ x);
    }

// serial->close();
}


void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
//! [6]

//! [7]
QString MainWindow::readData()
{
    QByteArray data = serial->readAll();

    //QString data = serial->readAll();
    //console->putData(data);
    ui->console->append(data);
    QString datas = data;

  //  int x = QString::compare(str1, str2, Qt::CaseInsensitive);  // if strings are equal x should return 0
//    QString data = ui->data->toPlainText();

    if (datas.at(0)=="X"){
    //QStringList strList = data.split(" ");
         ui->label->setText("Got Position");
    }

   // return data.toStdString();
}

void MainWindow::serialReceived()
{
    QByteArray output;
    output = serial->readAll();
    //ui->label->setText("output");
   // qInfo() << output;
     //qInfo("testing");
     ui->console->append(output);

}



void MainWindow::on_connectionbtn_clicked()
{
    QString btnstatus = ui->connectionbtn->text();
    ui->label->setText(btnstatus);
    //if (btnstatus.toStdString().c_str() == "Connect"){
    if (btnstatus.compare("Connect")==0){
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

        if (serial->isOpen() && serial->isWritable())
       {
            ui->connectionbtn->setText("Disconnect");
            ui->connectionbtn->setStyleSheet("background-color: red");
        }
    }else{
       // ui->label->setText("closing port");
        closeSerialPort();
        if (!serial->isOpen())
        {
            ui->connectionbtn->setText("Connect");
             ui->connectionbtn->setStyleSheet("background-color: rgb(155,255,0);");
        }

    }
    //check if printer in ascii mode by lookikng for checksum error ?
}

void MainWindow::on_pushButton_16_clicked()
{
sendCommand("G28 X0;");
}




//M70 P200 Message


void MainWindow::on_pushButton_4_clicked()
{

}

void MainWindow::on_pushButton_3_clicked()  // open file to listview
{


    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open GCode"), "/home", tr("GCode Files (*.gcode)"));

//https://stackoverflow.com/questions/28408542/how-to-populate-qt-listview-with-content-from-a-text-file
    QStringList *allLines = new QStringList(); //Your list for lines from the file.
     allLines->clear();

     QStringListModel *linesModel = new QStringListModel(*allLines, NULL); //Your model to set to the view.

     QFile file(fileName);
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
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open GCode"), "/home", tr("GCode Files (*.gcode)"));

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

    this->setWindowTitle("QPrint3d "+fileName);
 //   ui->filenamelabel->setText(fileName);

}

void MainWindow::on_uploadsdbtn_clicked()
{
        if (0){
      sendCommand("M21;"); // get sdcard ready
      sendCommand("M28 filename.txt;"); //write to file

      // send code here
      sendCommand("M29;"); //stop writing
    }
}

void MainWindow::on_uploadprintbtn_clicked()
{
    //pick filename
    QString text3 = QInputDialog::getText(this,"Pick Filename to save","gcode");
   // QString fileName = QFileDialog::getOpenFileName(this,
   //      tr("Open GCode"), "/home", tr("GCode Files (*.gcode)"));
   // ui->fileName->setText(fileName);

    //check if printer connected

    QString text = ui->textBrowser->toPlainText();
    QTextStream * stream2 = new QTextStream(&text , QIODevice::ReadOnly);

        //if text3 != "" then continue
    if (0){

    sendCommand("M21;"); // get sdcard ready
    sendCommand("M28 "+text3); //write to file

    while (!stream2->atEnd()) {
        sendCommand(stream2->readLine());
    }
    // send code here
    sendCommand("M29;"); //stop writing
    sendCommand("M23 filename.txt;"); //
    sendCommand("M24;"); //start printing
    }
}




void MainWindow::on_printbtn_clicked()
{
    //write gcode buffer if it checks out

    //check for printer connection

    int buffercount=10;
    int count=0;
//if gx == received position
   //read file if gcodeedit is empty

//    int lcount=lines.count();
//    int icount=0;
//   while (lcount >= icount){ // loop whole buffer

       QString text = ui->textBrowser->toPlainText();
       QTextStream * stream2 = new QTextStream(&text , QIODevice::ReadOnly);
     //  QVector<QString > lines;
       bool bstoploop=0;
       while (!stream2->atEnd() && !bstoploop)
       {
         //   << stream2->readLine();
       //update progress bar / tip position

       //wait for receive position
        //mark receivedm114 reset
     //  QString position = readData();

       //parse position for validm114
        int validm114=0;
        int printbuffersize=10;
        //wait for valid m114 before doing more
       while (validm114) { //or specified time
       //    for (int i=1;10,i++;){
          // QString currentline = lines[1].toStdString();
           QString currentline = stream2->readLine();
                 //  lines.pop_front();

        //send 10 lines
         sendCommand(currentline);
         if (count >= printbuffersize){ validm114=0; count++;}; //send m114 every buffer length to see where its at
        }
        sendCommand("M114;");
        validm114=1;  //parse and wait for valid before sending more
        count=0;

       }

}

void MainWindow::on_printbtn_2_clicked()
{
    on_printbtn_clicked();
}

void MainWindow::on_console_textChanged()
{

}

void MainWindow::on_tiptempslide_actionTriggered(int action)
{
int maxtiptemp=230; // if temp higer do software drop
int maxbedtemp=60;

   // ui->label->setText("tempchange");
   // ui->console->append("test");
   // qInfo() << ui->tiptempslide->value();
}

void MainWindow::on_tiptempslide_sliderReleased()
{
    sendCommand("M114;");
}

void MainWindow::on_bedtempslide_sliderReleased()
{
 //   qint8 slidevalue = ui->bedtempslide->value()
 //   QString text = ;
    ui->setbedinput->setText( QString(QString::number(ui->bedtempslide->value())) );
}

void MainWindow::on_lineEdit_returnPressed()
{
    on_sendbtn_clicked();
}

void MainWindow::on_timedevent(){
//refresh
ui->tiptempslide->setMaximum( 230) ;
 ui->tiptempslide->setRange(0,250) ;
 ui->bedtempslide->setMaximum( 110) ;

 ui->bedtempslide->setRange(0,110) ;
 ui->bedtempslide->setValue(50) ;
  ui->tiptempslide->setValue(144) ;

  QString btnstatus = ui->connectionbtn->text();
  //check printer still connected
  if (!serial->isOpen() && !btnstatus.compare("Disconnect"))
  {
      ui->connectionbtn->setText("Reconnect");
       ui->connectionbtn->setStyleSheet("background-color: rgb(155,255,0);");
  }


}

void MainWindow::on_tiptempslide_valueChanged(int value)
{
    ui->settipinput->setText( QString(QString::number(ui->tiptempslide->value())) );

}

void MainWindow::on_setTipbutton_clicked()
{
      //ui->tiptempslide->setValue(QString::number(ui->settipinput->text())) ;
}


void MainWindow::on_em50btn_clicked()
{
    sendCommand("G0 E-50;");
}

void MainWindow::on_em10btn_clicked()
{
     sendCommand("G0 E-10;");
}

void MainWindow::on_em1btn_clicked()
{
    sendCommand("G0 E-1;");
}

void MainWindow::on_e1btn_clicked()
{
     sendCommand("G0 E1;");
}

void MainWindow::on_e10btn_clicked()
{
     sendCommand("G0 E10;");
}

void MainWindow::on_zm10_clicked()
{
    sendCommand("G0 Z-10;");
}

void MainWindow::on_zm1btn_clicked()
{
    sendCommand("G0 Z-1;");
}

void MainWindow::on_z1btn_clicked()
{
    sendCommand("G0 Z1;");
}

void MainWindow::on_z10btn_clicked()
{
    sendCommand("G0 Z10;");
}

void MainWindow::on_x1btn_clicked()
{
    sendCommand("G0 X1;");
}

void MainWindow::on_x10btn_clicked()
{
        sendCommand("G0 X10;");
}

void MainWindow::on_xm1_clicked()
{
    sendCommand("G28 X-1;");
}

void MainWindow::on_xm10btn_clicked()
{
    sendCommand("G28 X-10;");
}

void MainWindow::on_ym10btn_clicked()
{
    sendCommand("G28 Y-10;");
}

void MainWindow::on_ym1btn_clicked()
{
    sendCommand("G28 Y-1;");
}

void MainWindow::on_y1btn_clicked()
{
    float currentpos = ui->ycoord->text().toFloat();
    float pos=currentpos+1;
   // QString yposstr = QString::number(ypos);
        sendCommand("G0 Y" + QString::number(pos) + ";");
}

void MainWindow::on_y10btn_clicked()
{
    float currentpos = ui->ycoord->text().toFloat();
    //QString b;
   // b.setNum(ui->ycoord->text());
    float pos=currentpos+10;
   // QString yposstr = QString::number(ypos);
        sendCommand("G0 Y" + QString::number(pos) + ";");
}

void MainWindow::on_homeallbtn_clicked()
{
    sendCommand("G28;");
}
void MainWindow::on_homexbtn_clicked()
{
        sendCommand("G28 X0;");
}

void MainWindow::on_homeybtn_clicked()
{
        sendCommand("G28 Y0;");
}

void MainWindow::on_homezbtn_clicked()
{
        sendCommand("G28 Z0;");
}

void MainWindow::on_emstopbtn_clicked()
{
  //   sendCommand("M117;");
}

void MainWindow::on_pausebtn_clicked()
{
    sendCommand("M226;");
}

void MainWindow::on_pauseSDbtn_clicked()
{
    sendCommand("M24;");
}

void MainWindow::on_pauseSDbtn_2_clicked()
{
    sendCommand("M25;");
}


void MainWindow::on_uploadprintbtn_3_clicked()
{
    on_uploadprintbtn_clicked();
}

void MainWindow::on_uploadsdbtn2_clicked()
{
    on_uploadsdbtn_clicked();
}
