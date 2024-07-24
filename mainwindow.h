#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"chilewnd.h"

#include <QMainWindow>
#include<QCloseEvent>
#include<QSignalMapper>
#include<QMdiSubWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initMainWindow();
    void docNew();
    void docOpen();

//槽函数
private slots:
    void on_newAction_triggered();
    void refreshMenus();
    void addSubWndListMenu();

    void on_closeAction_triggered();

    void on_closeAllAction_triggered();

    void on_titleAction_triggered();

    void on_cascadeAction_triggered();

    void on_nextAction_triggered();

    void on_previousAction_triggered();

    //设置活动子窗口
    void setActiveSubWindow(QWidget* wnd);

    //关闭事件
    void closeEvent(QCloseEvent *event);


    void on_openAction_triggered();

private:
    void formatEnable();
    ChileWnd* activateChildWnd();
    //匹配子窗口是否存在
    QMdiSubWindow* findChileWnd(const QString &docName);

private:
    Ui::MainWindow *ui;
    QSignalMapper* m_WndMapper; //信号映射器

};
#endif // MAINWINDOW_H
