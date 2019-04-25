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
#include<unistd.h>


// a backup plan for talking to printer on linux would be to use the echo "G28" >> /dev/ttyACM0
// code used for this.
//https://stackoverflow.com/questions/51209822/qtserialport-not-writing-to-serial-port
//http://www.howtobuildsoftware.com/index.php/how-do/cBtm/c-qt-serial-port-writing-qt-serial-port-not-working

//todo
//verify all buttons working

//parse m114 readData() from listbox to get valid position
//movement buttons need current position to move around better
//keyboard jog mode
//launch commands list for parser
//sdcard parser for already listed files to print sd.
//settings need to be saved/loaded
//need ability to thread the write routines it freezes whole program
//combine pause resume button for sd print


//verify successful print from sd
//verify successful print to printer iron out buffersize to keepthings fast


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    this->setWindowTitle("QPrint3d");

    serial = new QSerialPort(this);
//! [1]
  //  settings = new SettingsDialog;

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
          this, SLOT(on_timedevent()));

    timer->start(10000);



    ui->setupUi(this);
    ui->emstopbtn->setStyleSheet("background-color: red");


  //  this->setWindowOpacity(0.55);
  //  this->setAttribute(Qt::WA_TranslucentBackground, true);
  //  this->setStyleSheet("background-color: yellow;");

    m_settings = QApplication::applicationDirPath().left(1) + ":/settings.ini";
    saveSettings();
    loadSettings();

  //  QOpenGLWidget *widget = new QOpenGLWidget();
    drawer = new GLDrawer(ui->widget);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    ui->widget->setFormat(format);

    msgBox("For Testing Purposes, its not ready unless you know how to code try in a month");

    ui->statusbox->setTitle("Status: .");
    ui->mprintbox->setTitle("PrintBox: connection");

  //   qInfo() << args.value(1);
}

void MainWindow::passargs(QString args){

    arg= args;
    qInfo() << arg;
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
    QString command = ui->lineEdit->text() + "\n"; //"G28;\n";
   // QString command = "M70 P200 Message\n";
    QByteArray x = command.toLocal8Bit();

    if (serial->isOpen() && serial->isWritable())
   {
        serial->write(x);
      // ui->label->setText("sent"+ x);
    }

}


void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}


void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    ui->console->append(data);
}

void MainWindow::serialReceived()
{
    QByteArray output;
    output = serial->readLine();//readAll();
    //ui->label->setText("output");
   // qInfo() << output;
     //qInfo("testing");
     ui->console->append(output);
       QString datas = QString(output);

     //  int x = QString::compare(str1, str2, Qt::CaseInsensitive);  // if strings are equal x should return 0
     //  QString data = ui->data->toPlainText();

       QString search= "X:";
      // datas.contains(match)
           //    if (datas.at(0)=="X" && datas.at(1)==":"){//todo check second char is a :

       if ( datas.contains(search)){//todo check second char is a :
            QStringList strList = datas.split(" ");
            QString X1 = strList.value(0);
            QString Y1 = strList.value(1);
            QString Z1 = strList.value(2);
            QString E1 = strList.value(2);
            strList = X1.split(":");
            X1 = strList.value(1);
            strList = Y1.split(":");
            Y1 = strList.value(1);
            strList = Z1.split(":");
            Z1 = strList.value(1);
            strList = E1.split(":");
            E1 = strList.value(1);
            ui->xcoord->setText(X1);
            ui->ycoord->setText(Y1);
            ui->zcoord->setText(Z1);

            ui->xposlabel->setText("X: "+X1);
            ui->yposlabel->setText("Y: "+Y1);
            ui->zposlabel->setText("Z: "+Z1);
            ui->eposlabel->setText("E: "+E1);

          //  strList.
           // ui->label->setText(X1+Y1+Z1);
            validm114=true;

       }

       search= "T:";
       //T:12.78 / 0 B:10.75 / 0 B@:0 @:0
       if ( datas.contains(search)){//todo check second char is a :
            QStringList strList = datas.split(" ");
            QString T1 = strList.value(0);
            QString B1 = strList.value(3);
          //  QString T2 = strList.value(2);

            strList = T1.split(" ");
            T1 = strList.value(0);
            strList = T1.split(":");
            T1 = strList.value(1);

            strList = B1.split(" ");
            B1 = strList.value(0);
            qInfo() << B1;
            strList = B1.split(":");
            B1 = strList.value(1);
            qInfo() << B1;


        //    ui->xcoord->setText(X1);
        //    ui->ycoord->setText(X1);

            ui->tiptemp->setText(T1);
            ui->bedtemp->setText(B1);
             ui->tiptempslide->setValue(T1.toFloat()) ;
              ui->bedtempslide->setValue(B1.toFloat()) ;
          //  strList.
            //ui->label->setText(X1+Y1+Z1);
        //    validm114=true;

       }


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
             sendCommand("M114;");
        }
    }else{
       // ui->label->setText("closing port");
        closeSerialPort();
        if (!serial->isOpen())
        {
            ui->connectionbtn->setText("Connect");
             ui->connectionbtn->setStyleSheet("background-color: rgb(155,255,0);");
             serial->clear();
             disconnect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));
        }

    }
    //check if printer in ascii mode by lookikng for checksum error ?

}


//M70 P200 Message

void MainWindow::on_pushButton_3_clicked()  // open file to listview not needed
{


    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open GCode"), "./", tr("GCode Files (*.gcode)"));

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

void MainWindow::on_opengcodebtn_clicked() // lineedit
{
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open GCode"), "./", tr("GCode Files (*.gcode)"));

    QFile file(fileName);
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

   drawer->loadGCode(fileName.toStdString());

}

void MainWindow::on_uploadsdbtn_clicked()
{
        QString text3 = QInputDialog::getText(this,"Pick Filename to save","gcode");

        QString text = ui->textBrowser->toPlainText();
        QTextStream * stream2 = new QTextStream(&text , QIODevice::ReadOnly);

        if (1){

        sendCommand("M21;"); // get sdcard ready
        sendCommand("M28 "+text3); //write to file

        while (!stream2->atEnd()) {
            sendCommand(stream2->readLine());
            sleep(10);
        }
        // send code here
        sendCommand("M29;"); //stop writing
        sendCommand("M23 filename.txt;"); //

        }
        ui->movementbox->setTitle("Movement: Upload");
}

void MainWindow::on_uploadprintbtn_clicked()
{
    on_uploadsdbtn_clicked();
    sendCommand("M24;"); //start printing
    sendCommand("M114;");
     ui->movementbox->setTitle("Movement: Upload");
}




void MainWindow::on_printbtn_clicked()
{
    //btnstatus.compare("Disconnect")
    //check for printer connection

    int buffercount=10;
    int count=0;
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
        //on_timedevent();
       //wait for receive position

       validm114=1; // everything should be homed maybe do a 0,0,0 check unless resuming
        int printbuffersize=10;

       while (validm114) { //or specified time
          //for (int i=1;10,i++;){
          // QString currentline = lines[1].toStdString();
           QString currentline = stream2->readLine();
                 //  lines.pop_front();
         sendCommand(currentline);
         if (count >= printbuffersize){ validm114=0; count++;}; //send m114 every buffer length to see where its at
        }
        sendCommand("M114;");
     //   validm114=1;  //parse and wait for valid before sending more
        count=0;

       }
}

void MainWindow::on_printbtn_2_clicked()
{
    on_printbtn_clicked();
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
 //ui->tiptempslide->setMaximum( 230) ;
 ui->tiptempslide->setRange(0,230) ;

 ui->bedtempslide->setMaximum( 110) ;

 ui->bedtempslide->setRange(0,110) ;

  QString btnstatus = ui->connectionbtn->text();
  //check printer still connected
  if (!serial->isOpen() && !btnstatus.compare("Disconnect"))
  {
      ui->connectionbtn->setText("Reconnect");
       ui->connectionbtn->setStyleSheet("background-color: rgb(155,255,0);");
        disconnect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));
  }
  else if (btnstatus.compare("Disconnect")==0 ){
      sendCommand("M114;");
      sendCommand("M105;");
  }


}

void MainWindow::on_tiptempslide_valueChanged(int value)
{
    ui->settipinput->setText( QString(ui->tiptempslide->value()) );
     ui->movementbox->setTitle("Movement: stip");

}



void MainWindow::on_em50btn_clicked()
{
    float currentpos = ui->ecoord->text().toFloat();
    float pos=currentpos-50;
    sendCommand("G0 E" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent E-50");
}

void MainWindow::on_em10btn_clicked()
{
    float currentpos = ui->ecoord->text().toFloat();
    float pos=currentpos-10;
    sendCommand("G0 E" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent E-10");
}

void MainWindow::on_em1btn_clicked()
{
    float currentpos = ui->ecoord->text().toFloat();
    float pos=currentpos-1;
    sendCommand("G0 E" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent E-1");
}

void MainWindow::on_e1btn_clicked()
{
    float currentpos = ui->ecoord->text().toFloat();
    float pos=currentpos+10;
    sendCommand("G0 Z" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent E+1");
}

void MainWindow::on_e10btn_clicked()
{
    float currentpos = ui->ecoord->text().toFloat();
    float pos=currentpos+10;
    sendCommand("G0 E" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent E+1");
}

void MainWindow::on_zm10_clicked()
{
    float currentpos = ui->zcoord->text().toFloat();
    float pos=currentpos-10;
    sendCommand("G0 Z" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent Z-10");
}

void MainWindow::on_zm1btn_clicked()
{
    float currentpos = ui->zcoord->text().toFloat();
    float pos=currentpos-1;
    sendCommand("G0 Z" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent Z-1");
}

void MainWindow::on_z1btn_clicked()
{
    float currentpos = ui->zcoord->text().toFloat();
    float pos=currentpos+10;
    sendCommand("G0 Z" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent Z+1");

}

void MainWindow::on_z10btn_clicked()
{
    float currentpos = ui->zcoord->text().toFloat();
    float pos=currentpos+10;
    sendCommand("G0 Z" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent Z+10");
}

void MainWindow::on_x1btn_clicked()
{
    float currentpos = ui->xcoord->text().toFloat();
    float pos=currentpos+1;
    sendCommand("G0 X" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent X+1");
}

void MainWindow::on_x10btn_clicked()
{
    float currentpos = ui->xcoord->text().toFloat();
    float pos=currentpos+10;
    sendCommand("G0 X" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent X+10");
}

void MainWindow::on_xm1_clicked()
{
    float currentpos = ui->xcoord->text().toFloat();
    float pos=currentpos-1;
    sendCommand("G0 X" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent X-1");
}

void MainWindow::on_xm10btn_clicked()
{
    float currentpos = ui->xcoord->text().toFloat();
    float pos=currentpos-10;
    sendCommand("G0 X" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent X-10");
}

void MainWindow::on_ym10btn_clicked()
{

    float currentpos = ui->ycoord->text().toFloat();
    float pos=currentpos-10;
    sendCommand("G0 Y" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent y-10");
}

void MainWindow::on_ym1btn_clicked()
{
        float currentpos = ui->ycoord->text().toFloat();
        float pos=currentpos-1;
        sendCommand("G0 Y" + QString::number(pos) + ";");
        sendCommand("M114;");
         ui->movementbox->setTitle("Movement: sent y-1");
}

void MainWindow::on_y1btn_clicked()
{
    float currentpos = ui->ycoord->text().toFloat();
    float pos=currentpos+1;
    sendCommand("G0 Y" + QString::number(pos) + ";");
    sendCommand("M114;");
    ui->movementbox->setTitle("Movement: sent Y+1");
}

void MainWindow::on_y10btn_clicked()
{
    float currentpos = ui->ycoord->text().toFloat();
    //QString b;
   // b.setNum(ui->ycoord->text());
    float pos=currentpos+10;
        sendCommand("G0 Y" + QString::number(pos) + ";");
            sendCommand("M114;");
            ui->movementbox->setTitle("Movement: sent Y+10");
}

void MainWindow::on_homeallbtn_clicked()
{
    sendCommand("G28;");
    sendCommand("M114;");
     ui->movementbox->setTitle("Movement: HomeAll");
}
void MainWindow::on_homexbtn_clicked()
{
        sendCommand("G28 X0;");
        sendCommand("M114;");
         ui->movementbox->setTitle("Movement: HomeX");
}

void MainWindow::on_homeybtn_clicked()
{
        sendCommand("G28 Y0;");
            sendCommand("M114;");
             ui->movementbox->setTitle("Movement: HomeY");
}

void MainWindow::on_homezbtn_clicked()
{
        sendCommand("G28 Z0;");
            sendCommand("M114;");
             ui->movementbox->setTitle("Movement: HomeZ");
}

void MainWindow::on_emstopbtn_clicked()
{
  //   sendCommand("M117;");
        sendCommand("M114;");
         ui->movementbox->setTitle("Movement: Stop");
}

void MainWindow::on_pausebtn_clicked()
{
    sendCommand("M226;");
        sendCommand("M114;");
         ui->movementbox->setTitle("Movement: Pause");
}

void MainWindow::on_pauseSDbtn_clicked()
{
    sendCommand("M24;");
        sendCommand("M114;");
          ui->movementbox->setTitle("Movement: PauseSD");
}

void MainWindow::on_pauseSDbtn_2_clicked()
{
    sendCommand("M25;");
      ui->movementbox->setTitle("Movement: PauseSD");
}


void MainWindow::on_uploadprintbtn_3_clicked()
{
    on_uploadprintbtn_clicked();

}

void MainWindow::on_uploadsdbtn2_clicked()
{
    on_uploadsdbtn_clicked();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_setBedbtn_clicked()
{

    sendCommand("M140 S"+QString(ui->setbedinput->text()));
      ui->movementbox->setTitle("Movement: SetBedTemp");

}

void MainWindow::on_setTipbutton_clicked()
{
      //ui->tiptempslide->setValue(QString::number(ui->settipinput->text())) ;
    sendCommand("M104 S"+QString(ui->settipinput->text()));
     ui->movementbox->setTitle("Movement: stip");
}


void MainWindow::on_comboBox_2_activated(const QString &arg1)
{
  //  QString value = "ON";
  //  combo->setCurrentIndex(index);
  //  int index = combo->findData(value);
  //  if ( index != -1 ) { // -1 for not found
  //     combo->setCurrentIndex(index);
  //  }

    ui->comboBox_2->setCurrentIndex(1);
            ui->comboBox_2->setStyleSheet("background-color: red");
}

void MainWindow::on_bedpowerbtn_clicked()
{
     if (serial->isOpen()){
    if (ui->bedpowerbtn->text()=="OFF"){

    ui->bedpowerbtn->setText("ON");
    ui->bedpowerbtn->setStyleSheet("background-color: rgb(155,255,0);");
    on_setBedbtn_clicked();
    }
    else{
        ui->bedpowerbtn->setText("OFF");
        ui->setbedinput->setText("0");
        ui->bedpowerbtn->setStyleSheet("background-color: red");
         on_setBedbtn_clicked();
    }
     }
}

void MainWindow::on_tippowerbtn_clicked()
{
    if (serial->isOpen()){
   if (ui->tippowerbtn->text()=="OFF"){

   ui->tippowerbtn->setText("ON");
   ui->tippowerbtn->setStyleSheet("background-color: rgb(155,255,0);");
   on_setTipbutton_clicked();
   }
   else{
       ui->tippowerbtn->setText("OFF");
       ui->tippowerbtn->setText("0");
       ui->tippowerbtn->setStyleSheet("background-color: red");
        on_setTipbutton_clicked();
   }
    }
}
