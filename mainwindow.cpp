#include <QDebug>
#include <QUrl>

#include "accessmanager.h"
#include "downloader.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnOpen, SIGNAL(clicked()), this, SLOT(onOpen()));

//	QString ss = "123456789http://asdasd.asda.jpg123456789";
//	ss = ss.replace(QRegExp("http:.+\.jpg"), "");
//	qDebug() << ss;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpen()
{
//    Downloader *loader = new Downloader(this);
//    loader->httpDownload(QUrl("http://e.hiphotos.baidu.com/image/pic/item/a8773912b31bb05114a597be3b7adab44bede0a7.jpg"), "D:/");

//    g_pAccessManager->getHTML("http://www.jingjiamicro.com/");


	QString strTitle;
    QStringList lstUrl;
    g_pAccessManager->getTitleAndAllPictureUrl("http://www.sanygroup.com", strTitle, lstUrl);
    qDebug() << strTitle;
    qDebug() << lstUrl;
    close();
}
