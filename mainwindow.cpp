#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QDirIterator"
#include "QStringList"
#include "QDebug"
#include "QListWidget"
#include "QFile"
#include "QThread"
#include "QtCore"
#include "QMessageBox"
#include "QMenuBar"
#include "QTextEdit"




bool cleared = false;
QString keyword;
int current = 0;
int max_size = 10;

QString *list = new QString[max_size];

QString dir;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->startButton->setEnabled(false);
    ui->progressBar->setValue(0);
    ui->progressBar->setEnabled(false);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_folderButton_clicked()
{
    dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",QFileDialog::DontResolveSymlinks);



    QString fxbText = "<font color = 'green'>.fxb files found in </font>" + dir;
    ui->fxbLabel->setText(fxbText);

    if (dir.size() < 4)
    {
        ui->fxbLabel->setText("<font color = 'red'>Error: Cannot select drive as root folder</font>");
        qDebug() << "Cannot Select Root";
    }
    else
    {
        ui->startButton->setEnabled(true);
        QDirIterator it(dir, QStringList() << "*.fxb", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            if(current != max_size)
            {
                list[current] = it.next();
                current++;

            }
            else
            {
                max_size *= 2;
                QString *temp = new QString[max_size];
                for(int i = 0; i < current; i++)
                {
                    temp[i] = list[i];
                }
                delete [] list;
                list = temp;
                temp->clear();
                qDebug() << max_size;
            }





        }


    }




    for(int i = 0; i < current; i++)
    {
        ui->listWidget->addItem(list[i]);
    }



}



void MainWindow::on_startButton_clicked()
{
    if(cleared != true || keyword == "")
    {
        QMessageBox::information(this, tr("fxbViewer"), tr("Input keyword"));
    }
    else
    {
    qDebug() << current << "\n\n\n\n";
    for(int i = 0; i < current; i++)
    {
        QFile inputFile(list[i]);
        if (inputFile.open(QIODevice::ReadOnly))
        {
           ui->progressBar->setEnabled(true);
           ui->progressBar->setMaximum(current);
           ui->progressBar->setValue(i);


           QTextStream in(&inputFile);
           while (!in.atEnd())
           {
                QString line = in.readLine();
                line = line.toLower();
                if(line.contains(keyword))
                {
                    ui->keywordList->addItem(list[i]);
                }
           }
           inputFile.close();
        }
    }
    ui->progressBar->setValue(0);
    ui->progressBar->setEnabled(false);
    }

}

void MainWindow::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{
    if(cleared == false)
    {
        ui->lineEdit->setText("");
        cleared = true;
    }
    keyword = ui->lineEdit->text();
    keyword = keyword.toLower();
    qDebug() << keyword;


}

void MainWindow::on_stopButton_clicked()
{
    //ummmmm were gonna have to multithread...
}

void MainWindow::on_pushButton_clicked()
{
    ui->keywordList->clear();
}

void MainWindow::on_clearSButton_clicked()
{
    qDebug() << "CLEAR SEARCH";
    ui->fxbLabel->setText(".fxb files in folder");
    ui->listWidget->clear();
    list->clear();
    ui->startButton->setEnabled(false);
    dir.clear();
}
