#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include<QStandardItemModel>
#include<QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateFile();

private slots:
    void on_btn_upload_clicked();

    void on_btn_delete_clicked();

    void on_btn_open_clicked();

private:
    Ui::MainWindow *ui;

    QString m_strDataPath;
    QStandardItemModel *m_model;
    QTimer m_timer;
    QString m_cur;
};
#endif // MAINWINDOW_H
