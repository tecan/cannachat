#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settings.h"
#include "servers.h"
#include "ui_servers.h"
#include "ui_settings.h"

#include "qmessagebox.h"
#include "qprocess.h"
#include "qstring.h"
#include "qcoreapplication.h"
#include "qobject.h"
#include <QDebug>
#include <QEvent>


#include "qstylesheetmanager.h"
#include "loadtheme.h"
//#define IRRLICHT
#ifdef IRRLICHT
#include <QGLWidget>
#include "irrutil.h"
#include "irrlichtwidget.h"
IrrlichtWidget* widget ;
//IrrlichtWidget* widget2 ;
#endif

ServerDlg *serversw;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
       ui->setupUi(this);

      this->setWindowTitle("Cannachat");

  //    Dialog1 *newDlg = new Dialog1();
  //    this->hide();
  //    int result = newDlg->exec();
  //    this->show();
  //    delete newDlg;

       ServerDlg *settings = new ServerDlg();
      // settings->show();

       connect(settings, SIGNAL(findtext()), this , SLOT(on_search_clicked()));



#ifdef IRRLICHT
     // widget = new IrrlichtWidget( ui->tabWidget->findChild<QWidget *>("openGLWidget") );
            widget = new IrrlichtWidget( ui->tabWidget->findChild<QWidget *>("irrRenderWidget0") );
     widget->init();
#endif


      //IrcClient *test
         //     ircwidget = new IrcClient( ui->tabWidget->findChild<QWidget *>("chatwidget"));

     this->resize(1000,500);

            //  vector<IrcClient*> test2;
     //read settings find autojoin servers
     QString servername = "irc.choopa.net";
     QString channel =  "#cannachat";
     bool ssl = 1;
     int port = 9999;

        //      serverarray.push_back(new IrcClient( ui->tabWidget->findChild<QWidget *>("chatwidget"), ui->nickname->text().toLatin1(), channel.toLatin1(), servername.toLatin1(),port,ssl));
int autostart=0;
     if(autostart){
            for (int i=0; i < serverarray.size() ; i++){
             ui->tabWidget->addTab(serverarray[i], servername.toLatin1());
             //     ui->chatwidget->resize(1000,700);
            }
     }
// read settings find autojoin channels for servers

//ircwidget[0] = test;
//    ircwidget2 = new IrcClient( ui->tabWidget->findChild<QWidget *>("chatwidget_2"));

     if (ui->nickname->text().toLatin1() == "") {
         qDebug() << "setnickname";
         ui->nickname->setText("guest" + QString::number( qrand() % 9999) );
        }

     ui->serverlist->setCurrentRow(0);

themeInit();
readsettings();
}

MainWindow::~MainWindow()
{
//   ui->centralWidget->findChild<QWidget *>("irrRenderWidget0"), false);

//    this->irr0->init();

//    this->irr1 = new IrrCoreRenderer(ui->centralWidget->findChild<QWidget *>("irrRenderWidget1"), 1);

//    this->irr1->init();



}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
 //  ircwidget[0].resize(ui->chatwidget->width(),ui->chatwidget->height());
#ifdef IRRLICHT
  // widget->resizeIrrWidget(0, 0, this->size().width(), this->size().height()/2);
  // this->irr1->resizeIrrWidget(this->size().width()/2, 0, this->size().width()/2, this->size().height());
#endif
   // Your code here.
}


void MainWindow::readsettings(){
    bool settingsexists=false;
QFile Fout("settings.txt");    if(Fout.exists())    {       settingsexists=true;    }    Fout.close();
if (settingsexists){
    QString searchString(":");
    QFile MyFile("settings.txt");
    MyFile.open(QIODevice::ReadWrite);
    QTextStream in (&MyFile);
    QString line;
  //  int ii=0;
    QStringList list;
     //   QList<QString> nums;
    QStringList nums;
    QString Nick;


    do {
        line = in.readLine();
        searchString=":";
        if (line.contains(searchString)) { //, Qt::CaseSensitive
            // do something
            QRegExp rx("[:]");// match a comma or a space
            list = line.split(rx);
            nums.append(list.at(1).toLatin1());

        }
    } while (!line.isNull());

    ui->nickname->setText(nums.at(0).toLatin1());
    ui->password->setText(nums.at(1).toLatin1());
    ui->identname->setText(nums.at(2).toLatin1());
    ui->client->setText(nums.at(3).toLatin1());
    ui->quitmsg->setText(nums.at(4).toLatin1());
    ui->idlemessage->setText(nums.at(5).toLatin1());
    ui->awaymessage->setText(nums.at(6).toLatin1());

    ui->colors->setChecked(nums.at(7).toInt());
    ui->themes->setChecked(nums.at(8).toInt());
    ui->smtp->setChecked(nums.at(9).toInt());
    ui->logging->setChecked(nums.at(10).toInt());
    ui->identify->setChecked(nums.at(11).toInt());
    nums.clear();

    bool serversexists=false;
QFile Fout("servers.txt");    if(Fout.exists())    {       serversexists=true;    }    Fout.close();
if (serversexists){
qDebug() << "test";
    QFile MyFile2("servers.txt");
    MyFile2.open(QIODevice::ReadWrite);
    QTextStream in2 (&MyFile2);

    do {
        line = in2.readLine();
        searchString=":";
        if (line.contains(searchString)) { //, Qt::CaseSensitive
            // do something
           // QRegExp rx("[:]");// match a comma or a space
          //  list = line.split(rx);
           nums.append(line);
        }
    } while (!line.isNull());
    MyFile2.close();

//    ui->serverlist->blockSignals(true);
//    ui->serverlist->clear();
//    ui->serverlist->blockSignals(false);
    for (int i=0; i < ui->serverlist->count(); i++){
        ui->serverlist->takeItem(0);
    }


                  //      int sized = ui->channelList->count();
    ui->serverlist->item(ui->serverlist->currentRow())->setText(nums.at(0).toLatin1());
    for (int i=1; i < nums.count() ; i++){
        ui->serverlist->addItem(nums.at(i).toLatin1());
    }
}


}
    //ircwidget->resize(ui->chatwidget->width(),ui->chatwidget->height());
       writesettings();
}

void MainWindow::writesettings(){

    QFile file("settings.txt");

          if(file.open(QIODevice::WriteOnly | QIODevice::Text))
          {
                  file.seek(0);
              QTextStream stream(&file);
                stream << "name:" << ui->nickname->text().toLatin1() << endl;
                stream << "password:" << ui->password->text().toLatin1()<< endl;
                stream << "ident:" << ui->identname->text().toLatin1()<< endl;
                stream << "client:" << ui->client->text().toLatin1()<< endl;
                stream << "quit:" << ui->quitmsg->text().toLatin1()<< endl;
                stream << "idle:" << ui->idlemessage->text().toLatin1()<< endl;
                stream << "away:" << ui->awaymessage->text().toLatin1()<< endl;

                stream << "colors:" << ui->colors->isChecked()<< endl;
                stream << "themes:" << ui->themes->isChecked()<< endl;
                stream << "smtp:" << ui->smtp->isChecked()<< endl;
                stream << "logging:" << ui->logging->isChecked()<< endl;
                stream << "identify:" << ui->identify->isChecked()<< endl;

              file.close();
          }

          QStringList splitlist = ui->serverlist->currentItem()->text().split(":");
          QString servername = splitlist[0].toUtf8();


          QFile file2(servername.toLatin1()+".txt");
            //   QFile file2("servers.txt");
            //    if(file.open(QIODevice::WriteOnly | QIODevice::Text))

                if(file2.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                    file2.seek(0);
                    QTextStream stream(&file2);
                    int sized = ui->channelList->count();
                    for (int i=0; i < sized; i++){
                        ui->channelList->setCurrentRow(i);
                         stream << servername.toLatin1()+":" << ui->channelList->currentItem()->text().toLatin1() << endl;
                    }
                    file2.close();
                }

qDebug() << "writing settings";




          QFile file3("servers.txt");
                if(file3.open(QIODevice::WriteOnly | QIODevice::Text))
                {

                    QTextStream stream(&file3);
                                        file3.seek(0);
                    int sized = ui->serverlist->count();
                    qDebug() << sized;
                    for (int i=0; i < sized; i++){
                        ui->serverlist->setCurrentRow(i);

                         stream << ui->serverlist->currentItem()->text().toLatin1() << endl;
                    }
                    file3.close();
                }

}

void MainWindow::on_actionExit_triggered()
{
QApplication::quit();
}

void MainWindow::on_actionNew_triggered()
{
    serversw = new ServerDlg();
    serversw->show();
}

void MainWindow::on_actionClose_triggered()
{

}

void MainWindow::on_actionRestore_triggered()
{

}


void MainWindow::on_pushButton_clicked()
{
//ircwidget->Connections();
}

void MainWindow::on_connect_clicked()
{

    QStringList splitlist = ui->serverlist->currentItem()->text().split(":");
    QString servername = splitlist[0].toUtf8();
    qDebug() << splitlist[0].toUtf8();
    QString channel =  "#cannachat";
    int port = splitlist[1].toInt();
    bool ssl = splitlist[2].toInt();

//        QString servername = "irc.choopa.net";
//        QString channel =  "#cannachat";
//        bool ssl = 1;
//        int port = 9999;

    serverarray.push_back(new IrcClient( ui->tabWidget->findChild<QWidget *>("chatwidget"), ui->nickname->text().toUtf8(), channel.toUtf8(), servername.toUtf8(),port,ssl));
    qDebug() << serverarray.size();
    ui->tabWidget->addTab(serverarray[serverarray.size()-1], servername.toLatin1());
    //serverarray[serverarray.size()-1]->registerSelf(&serverarray[serverarray.size()-1]);
    //serverarray[serverarray.size()-1]->appendText("testing 12345");

}

void MainWindow::on_actionSave_triggered()
{
    writesettings();
}

void MainWindow::on_addserver_clicked()
{
    ui->serverlist->addItem(ui->server->text().toLatin1());
    writesettings();
}

void MainWindow::on_removeserver_clicked()
{
    //check for duplicate server before removing file
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you sure ?", "remove ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      qDebug() << "Yes was clicked";
      if (reply){
          //    ui->serverlist->addItem(ui->server->text().toLatin1())
            qDeleteAll(ui->serverlist->selectedItems());
            writesettings();
            //delete file
            //ui->serverlist->currentItem()->text()
      }
    }


}


void MainWindow::on_addchannel_clicked()
{
    ui->channelList->addItem(ui->channeledit->text().toLatin1());
    writesettings();
}

void MainWindow::on_removechannel_clicked()
{
            //    QListWidgetItem items = ui->listWidget->selectedItems();
            //ui->listWidget->removeItemWidget(item);
//    QList<QListWidgetItem*> items = ui->channelList->selectedItems();
//    foreach(QListWidgetItem * item, items)
//    {
//        delete ui->channelList->takeItem(ui->channelList->row(item));
//    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you sure ?", "remove ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      qDebug() << "Yes was clicked";
      if (reply){
        qDeleteAll(ui->channelList->selectedItems());

        writesettings();
        }
    }
}

void MainWindow::on_serveredit_clicked()
{
        ui->serverlist->item(ui->serverlist->currentRow())->setText(ui->server->text().toLatin1());
    //ui->channelList->addItem(ui->editchannel->text().toLatin1());
    writesettings();
}

void MainWindow::on_editchannel_clicked()
{
    ui->channelList->item(ui->channelList->currentRow())->setText(ui->channeledit->text().toLatin1());
    //ui->channelList->addItem(ui->editchannel->text().toLatin1());
    writesettings();
}

void MainWindow::on_joinchannel_clicked()
{
    //get selected tab
serverarray[0]->JoinChannel(ui->channelList->currentItem()->text().toLatin1());

}

void MainWindow::on_channelList_clicked(const QModelIndex &index)
{
    ui->channeledit->setText(ui->channelList->currentItem()->text().toLatin1());
}

void MainWindow::on_serverlist_clicked(const QModelIndex &index)
{


  ui->server->setText(ui->serverlist->currentItem()->text().toLatin1());
}

void MainWindow::on_serverlist_currentRowChanged(int currentRow)
{
    //load channels for server maybe use sqlite db
    QStringList splitlist = ui->serverlist->currentItem()->text().split(":");
    QString servername = splitlist[0].toUtf8();
    QFile Fout(servername.toLatin1()+".txt");    if(Fout.exists())    {     ui->channelList->clear();   }    Fout.close();

   // qDebug() << "channels";
    QString searchString(":");
    QFile MyFile(servername.toLatin1()+".txt");
    MyFile.open(QIODevice::ReadWrite);
    QTextStream in (&MyFile);

    QStringList list;
    QString line;
    QStringList nums;

    do {
        line = in.readLine();
        if (line.contains(searchString.toLatin1())) { //, Qt::CaseSensitive
            QRegExp rx("[:]");// match a comma or a space
            list = line.split(rx);
            nums.append(list.at(1).toLatin1());
        }
    } while (!line.isNull());
    MyFile.close();
    foreach (QString list2,nums){
        ui->channelList->addItem(list2.toLatin1());
    }
}
