#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDir>
#include<QDebug>
#include<QDateTime>
#include<QFileDialog>
#include<QMessageBox>
#include<QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //获取当前应用程序所在目录
    m_strDataPath=QApplication::applicationDirPath()+"/data";

    QDir d(m_strDataPath);
    if(!d.exists()){
        d.mkdir(m_strDataPath);
    }

    m_model=new QStandardItemModel(this);

//    //文件加载到窗口中
//    updateFile();

    m_timer.start(1000);
    connect(&m_timer,&QTimer::timeout,this,&MainWindow::updateFile);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//实时刷新文件
void MainWindow::updateFile()
{
    QDir d(m_strDataPath);
    QStringList lFiter;
    lFiter<<"*.txt"<<"*.md";
    QFileInfoList lFilesInfo=d.entryInfoList(lFiter,QDir::Files); //列出目录中匹配过滤器的条目

    QString strFilter=ui->lineEdit->text();
    QString strFlag;

    QList<QStandardItem*> lRow;
    QStandardItemModel *temModel=new QStandardItemModel;
    for(int i=0;i<lFilesInfo.size();i++){
        lRow.clear();
        QFileInfo &info=lFilesInfo[i];
        if(!info.completeBaseName().contains(strFilter,Qt::CaseInsensitive))
            continue;
            /*
            函数	返回内容	举例：abc.tar.gz
            fileName()                      带后缀的完整文件名       abc.tar.gz     |       contains(strFilter)
            baseName()                   最后的后缀前的名字       abc.tar          |       文件名是否包含这个关键字（区分大小写）
            completeBaseName()	去掉所有后缀后的名字	  abc               |       不区分写法contains(strFilter, Qt::CaseInsensitive)
            */
        //序号
        QStandardItem *p1=new QStandardItem(info.baseName());
        QStandardItem *p2=new QStandardItem(info.absoluteFilePath());
        strFlag=strFlag+info.absoluteFilePath();
        QStandardItem *p3=new QStandardItem(info.completeSuffix());
        QStandardItem *p4=new QStandardItem(info.birthTime().toString("yyyy-MM-dd  hh:mm:ss"));
        QStandardItem *p5=new QStandardItem(info.lastModified().toString("yyyy-MM-dd  hh:mm:ss"));
        QStandardItem *p6=new QStandardItem(QString::number(info.size()/1024)+"KB");
        lRow<<p1<<p2<<p3<<p4<<p5<<p6;
        temModel->appendRow(lRow);
    }

    if(strFlag==m_cur){
        delete temModel;
        return ;
    }
    delete m_model;
    m_model=nullptr;
    m_model=temModel;

    m_cur=strFlag;
    QStringList lHeaders{"文件名","绝对路径","文件类型","创建日期","最后一次修改","文件大小"};
    m_model->setHorizontalHeaderLabels(lHeaders);
    ui->tableView->setModel(m_model);
}

//上传
void MainWindow::on_btn_upload_clicked()
{
   auto strPath= QFileDialog::getOpenFileName(this,"文件上传",QDir::homePath(),"文本文件 (*.txt *.md);;所有文件 (*.*)");
   if(strPath.isEmpty()){
       return ;
   }
   QFileInfo info(strPath);
   QString fileName=info.fileName();
   auto uploadPath=QDir(m_strDataPath).filePath(fileName);
   //判断我缓存目录中是否有相同文件的文件
   if(QFile::exists(uploadPath)){
       QMessageBox::warning(this,"文件已存在","请重新改名");
       return ;
   }
   bool ret=QFile::copy(strPath,uploadPath);
   QMessageBox::information(this,"提示",ret?"成功":"上传失败");
}

//删除文件夹
void MainWindow::on_btn_delete_clicked()
{
    auto index=ui->tableView->currentIndex();
    if(!index.isValid()){
        return;
    }

    auto itemp=m_model->index(index.row(),1);
   auto str= m_model->data(itemp).toString();
    auto ret=QFile::remove(str);
    QMessageBox::information(nullptr,"信息",ret?"文件删除成功":"文件删除失败");
}

//打开文件夹
void MainWindow::on_btn_open_clicked()
{
    QDesktopServices::openUrl(QUrl(m_strDataPath));
}

