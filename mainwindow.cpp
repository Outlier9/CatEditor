#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"chilewnd.h"
#include<QMdiSubWindow>
#include<QAction>
#include<QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindow()
{
    //初始化字号列表
    QFontDatabase fontdb;
    foreach(int fontsize,fontdb.standardSizes())
        ui->sizeComboBox->addItem(QString::number(fontsize));

    QFont defFont;
    QString sFontSize;
    int defFontSize; //当前应用程序默认字号
    int defFontindex; //当前字号在组合框中的索引号

    defFont = QApplication::font(); //获取字体信息
    defFontSize =  defFont.pointSize(); //获取字号信息
    sFontSize = QString::number(defFontSize); //将字号转化为字符类型
    defFontindex = ui->sizeComboBox->findText(sFontSize); //将选中的字号传给findText函数，然后找到其索引

    ui->sizeComboBox->setCurrentIndex(defFontindex); //将获取到的索引设置给下拉框

    //设置多文档区域滚动条
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //刷新菜单
    refreshMenus();
    connect(ui->mdiArea,&QMdiArea::subWindowActivated,this,&MainWindow::refreshMenus);

    //更新菜单
    addSubWndListMenu();
    //点击W（窗体）就更新并添加位于该部分的子窗口列表
    connect(ui->menu_W,&QMenu::aboutToShow,this,&MainWindow::addSubWndListMenu);


    //创建信号映射器
    m_WndMapper = new QSignalMapper(this);
    //点击窗口菜单项的时候，执行map方法，使setActiveSubWindow响应
    connect(m_WndMapper,SIGNAL(mapped(QWidget*)),this,SLOT(setActiveSubWindow(QWidget*)));

}

void MainWindow::docNew()
{
    ChileWnd *childwnd = new ChileWnd;
    ui->mdiArea->addSubWindow(childwnd);
    connect(childwnd, &ChileWnd::copyAvailble, ui->cutAction, &QAction::setEnabled);
    connect(childwnd, &ChileWnd::copyAvailble, ui->copyAction, &QAction::setEnabled);

    childwnd->newDoc();
    childwnd->show();
}

void MainWindow::docOpen()
{
    QString docName = QFileDialog::getOpenFileName(this,
                                     "打开文档",
                                     "",
                                     "文本文件(*.txt);;"
                                     "HTML文件(*.html *.htm);;"
                                     "所有文件(*.*)");

    //如果用户选择了文件并且文件名不为空，则继续执行
    if(!docName.isEmpty())
    {
        //查找是否已经有窗口打开了这个文档。如果找到已经存在的窗口，则将其激活，并返回
        QMdiSubWindow *existWnd = findChileWnd(docName);
        if(existWnd)
        {
            ui->mdiArea->setActiveSubWindow(existWnd);
            return;
        }

        //如果文档尚未被打开，创建一个新的 ChileWnd 子窗口，并将其添加到 QMdiArea 中
        ChileWnd *childWnd = new ChileWnd;
        ui->mdiArea->addSubWindow(childWnd);
        //连接子窗口的信号和主窗口的槽
        connect(childWnd,SIGNAL(copyAvailable(bool)),
                ui->cutAction,SLOT(setEnabled(bool)));
        connect(childWnd,SIGNAL(copyAvailable(bool)),
                ui->copyAction,SLOT(setEnabled(bool)));

        //加载文档内容
        if(childWnd->loadDoc(docName))
        {
            //显示状态栏信息 "文档已打开" 持续 3 秒
            statusBar()->showMessage("文档已打开",3000);
            childWnd->show();
            formatEnable();
        }
        else
        {
            //加载失败，则关闭创建的子窗口
            childWnd->close();
        }
    }
}


void MainWindow::formatEnable()
{
    ui->blodAction->setEnabled(true);
    ui->inclineAction->setEnabled(true);
    ui->underlineAction->setEnabled(true);
    ui->leftAction->setEnabled(true);
    ui->centerAction->setEnabled(true);
    ui->rightAction->setEnabled(true);
    ui->justifyAction->setEnabled(true);
    ui->colorAction->setEnabled(true);
}

ChileWnd *MainWindow::activateChildWnd()
{
    QMdiSubWindow* actWnd = ui->mdiArea->activeSubWindow(); // 返回子窗口并接受
    if(actWnd)
        return qobject_cast<ChileWnd*>(actWnd->widget()); //对象类型转换
    else
        return 0;
}

//匹配子窗口是否存在
QMdiSubWindow *MainWindow::findChileWnd(const QString &docName)
{
    //获取文件的标准路径
    QString strFile = QFileInfo(docName).canonicalFilePath();
    //遍历所有子窗口
    foreach(QMdiSubWindow* subWnd,ui->mdiArea->subWindowList()){
        //转换子窗口的 widget 为 ChileWnd
        ChileWnd* childWnd = qobject_cast<ChileWnd*>(subWnd->widget());
        //比较子窗口的当前文档路径
        if(childWnd->m_CurDocPath == strFile)
            //如果相同，表示找到了对应的子窗口，返回该子窗口指针 subWnd
            return subWnd;
    }
    //未找到对应子窗口的情况
    return 0;
}

//刷新菜单
void MainWindow::refreshMenus()
{
    bool hasChild;
    hasChild = (activateChildWnd() != 0);

    ui->saveAction->setEnabled(hasChild);
    ui->saveOther->setEnabled(hasChild);
    ui->printAction->setEnabled(hasChild);
    ui->printPreviewAction->setEnabled(hasChild);
    ui->pasteAction->setEnabled(hasChild);
    ui->closeAction->setEnabled(hasChild);
    ui->closeAllAction->setEnabled(hasChild);
    ui->titleAction->setEnabled(hasChild);
    ui->cascadeAction->setEnabled(hasChild);
    ui->nextAction->setEnabled(hasChild);
    ui->previousAction->setEnabled(hasChild);

    //有没有选中文本
    bool hasSelect = (activateChildWnd() && activateChildWnd()->textCursor().hasSelection()); //获取光标信息，以及是否有内容选中

    ui->cutAction->setEnabled(hasSelect);
    ui->copyAction->setEnabled(hasSelect);
    ui->blodAction->setEnabled(hasSelect);
    ui->inclineAction->setEnabled(hasSelect);
    ui->underlineAction->setEnabled(hasSelect);
    ui->leftAction->setEnabled(hasSelect);
    ui->centerAction->setEnabled(hasSelect);
    ui->rightAction->setEnabled(hasSelect);
    ui->justifyAction->setEnabled(hasSelect);
    ui->colorAction->setEnabled(hasSelect);

}

void MainWindow::addSubWndListMenu()
{
    //每次点击都要进行清空并更新，否则会重复将整个列表再次添加到该列
    ui->menu_W->clear();
    ui->menu_W->addAction(ui->closeAction);
    ui->menu_W->addAction(ui->closeAllAction);
    ui->menu_W->addSeparator();
    ui->menu_W->addAction(ui->titleAction);
    ui->menu_W->addAction(ui->cascadeAction);
    ui->menu_W->addSeparator();
    ui->menu_W->addAction(ui->nextAction);
    ui->menu_W->addAction(ui->previousAction);

    //获取所有的子窗口
    QList<QMdiSubWindow*> wnds = ui->mdiArea->subWindowList();
    //判断有没有子窗口
    if(!wnds.isEmpty())
        ui->menu_W->addSeparator(); //加分割线

    //将所有子窗口添加到列表
    for (int i = 0;i < wnds.size();++i) {
        ChileWnd* childWnd = qobject_cast<ChileWnd*>(wnds.at(i)->widget());
        //获取文档名称
        QString menuitem_text;
        menuitem_text = QString("%1 %2")
                .arg(i+1) //arg方法将字符串中的占位符替换为指定的值
                .arg(childWnd->getCurDocName());

        QAction *menuitem_act = ui->menu_W->addAction(menuitem_text);
        //添加勾选
        menuitem_act->setCheckable(true);
        menuitem_act->setChecked(childWnd == activateChildWnd());

        //使用信号映射器，触发map方法，与初始化时候设置好的槽函数关联
        connect(menuitem_act,SIGNAL(triggered(bool)),m_WndMapper,SLOT(map()));
        //设置参数
        m_WndMapper->setMapping(menuitem_act,wnds.at(i));
    }
    formatEnable();
}


void MainWindow::on_newAction_triggered()
{
    docNew();
}

void MainWindow::on_closeAction_triggered()
{
    ui->mdiArea->closeActiveSubWindow();
}

void MainWindow::on_closeAllAction_triggered()
{
    ui->mdiArea->closeAllSubWindows();
}

void MainWindow::on_titleAction_triggered()
{
    ui->mdiArea->tileSubWindows();
}

void MainWindow::on_cascadeAction_triggered()
{
    ui->mdiArea->cascadeSubWindows();
}

void MainWindow::on_nextAction_triggered()
{
    ui->mdiArea->activateNextSubWindow();
}

void MainWindow::on_previousAction_triggered()
{
    ui->mdiArea->activatePreviousSubWindow();
}

//设置活动子窗口
void MainWindow::setActiveSubWindow(QWidget *wnd)
{
    //窗口无效直接返回
    if(!wnd)
        return;
    //有效执行
    ui->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(wnd));

}



void MainWindow::closeEvent(QCloseEvent *event)
{
    ui->mdiArea->closeAllSubWindows();
    //防止在关闭主窗口时意外丢失未保存的数据
    if(ui->mdiArea->currentSubWindow())
        event->ignore();  //如果存在当前活动的子窗口，忽略关闭事件。这意味着窗口不会关闭，用户可以继续操作
    else
        event->accept(); //接受此事件
}

void MainWindow::on_openAction_triggered()
{
    docOpen();
}

void MainWindow::on_saveAction_triggered()
{

}

void MainWindow::on_saveOther_triggered()
{

}
