#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"chilewnd.h"
#include<QMdiSubWindow>
#include<QAction>
#include<QFileDialog>
#include<QColorDialog>
#include<QtPrintSupport/QPrinter>
#include<QtPrintSupport/QPrintDialog>
#include<QtPrintSupport/QPrintPreviewDialog>


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

    //对齐方式互斥性,一次只能选一种
    QActionGroup *alignGroup = new QActionGroup(this);
    alignGroup->addAction(ui->leftAction);
    alignGroup->addAction(ui->rightAction);
    alignGroup->addAction(ui->centerAction);
    alignGroup->addAction(ui->justifyAction);

}

void MainWindow::docNew()
{
    ChileWnd *childwnd = new ChileWnd;
    ui->mdiArea->addSubWindow(childwnd);
//    connect(childwnd, &ChileWnd::copyAvailble, ui->cutAction, &QAction::setEnabled);
//    connect(childwnd, &ChileWnd::copyAvailble, ui->copyAction, &QAction::setEnabled);

    //将各种功能设置为可用状态
    connect(childwnd,SIGNAL(copyAvailable(bool)),ui->cutAction,SLOT(setEnabled(bool)));
    connect(childwnd,SIGNAL(copyAvailable(bool)),ui->copyAction,SLOT(setEnabled(bool)));
    connect(childwnd,SIGNAL(copyAvailable(bool)),ui->leftAction,SLOT(setEnabled(bool)));
    connect(childwnd,SIGNAL(copyAvailable(bool)),ui->rightAction,SLOT(setEnabled(bool)));
    connect(childwnd,SIGNAL(copyAvailable(bool)),ui->centerAction,SLOT(setEnabled(bool)));
    connect(childwnd,SIGNAL(copyAvailable(bool)),ui->justifyAction,SLOT(setEnabled(bool)));
    connect(childwnd,SIGNAL(copyAvailable(bool)),ui->blodAction,SLOT(setEnabled(bool)));
    connect(childwnd,SIGNAL(copyAvailable(bool)),ui->inclineAction,SLOT(setEnabled(bool)));
    connect(childwnd,SIGNAL(copyAvailable(bool)),ui->underlineAction,SLOT(setEnabled(bool)));
    connect(childwnd,SIGNAL(copyAvailable(bool)),ui->colorAction,SLOT(setEnabled(bool)));

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

//保存
void MainWindow::docSave()
{
    //如果该活动窗口存在且它的saveDoc返回值为true也就是保存成功
    if(activateChildWnd() && activateChildWnd()->saveDoc())
    {
        statusBar()->showMessage("保存成功",3000);
    }
}

//另存为
void MainWindow::docSaveAs()
{
    //如果该活动窗口存在且它的saveDoc返回值为true也就是保存成功
    if(activateChildWnd() && activateChildWnd()->saveAsDoc())
    {
        statusBar()->showMessage("保存成功",3000);
    }
}

void MainWindow::docPrint()
{
    QPrinter pter(QPrinter::HighResolution);
    QPrintDialog *ddlg = new QPrintDialog(&pter,this);
    if(activateChildWnd())
        ddlg->setOption(QAbstractPrintDialog::PrintSelection,true);
    ddlg->setWindowTitle("打印文档");

    ChileWnd *chilewnd = activateChildWnd();
    if(ddlg->exec() == QDialog::Accepted)
        chilewnd->print(&pter);
    delete ddlg;
}

//每当预览对话框需要绘制预览时，都会触发 paintRequested 信号，从而调用 printPreview 槽函数
void MainWindow::docPrintPreview()
{
    // 创建一个 QPrinter 对象
    QPrinter pter;
    // 创建一个 QPrintPreviewDialog 对象，并将 pter 作为参数传递，同时设置父窗口为 this
    QPrintPreviewDialog preview(&pter,this);
    // 连接预览对话框的 paintRequested 信号到 MainWindow 的 printPreview 槽函数
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),this,SLOT(printPreview(QPrinter*)));
    // 显示打印预览对话框
    preview.exec();
}

//撤销（上一步）
void MainWindow::docUndo()
{
    if(activateChildWnd())
        activateChildWnd()->undo();
}

//重写（下一步）
void MainWindow::docRedo()
{
    if(activateChildWnd())
        activateChildWnd()->redo();
}

//剪切
void MainWindow::docCut()
{
    if(activateChildWnd())
        activateChildWnd()->cut();
}

//复制
void MainWindow::docCopy()
{
    if(activateChildWnd())
        activateChildWnd()->copy();
}

//粘贴
void MainWindow::docPaste()
{
    if(activateChildWnd())
        activateChildWnd()->paste();
}

//加粗
void MainWindow::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(ui->blodAction->isChecked() ? QFont::Bold : QFont::Normal);
    if(activateChildWnd())
        activateChildWnd()->setFormatOnSelectedWord(fmt);
}

//倾斜
void MainWindow::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(ui->inclineAction->isChecked());
    if(activateChildWnd())
        activateChildWnd()->setFormatOnSelectedWord(fmt);
}

//下划线
void MainWindow::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(ui->underlineAction->isChecked());
    if(activateChildWnd())
        activateChildWnd()->setFormatOnSelectedWord(fmt);
}

//设置字体
void MainWindow::textFamily(const QString &fmly)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(fmly);
    if(activateChildWnd())
        activateChildWnd()->setFormatOnSelectedWord(fmt);
}

//设置字号
void MainWindow::textSize(const QString &ps)
{
    qreal pointSize = ps.toFloat();
    if(ps.toFloat() > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        if(activateChildWnd())
            activateChildWnd()->setFormatOnSelectedWord(fmt);
    }
}

void MainWindow::textColor()
{
    if(activateChildWnd())
    {
        // 弹出颜色选择对话框，并以当前激活子窗口的文本颜色作为初始颜色
        QColor color = QColorDialog::getColor(activateChildWnd()->textColor(),this);
        // 检查用户是否选择了有效的颜色
        if(!color.isValid())
            return;
        QTextCharFormat fmt;
        // 设置文本字符格式的前景色（文本颜色）
        fmt.setForeground(color);
        activateChildWnd()->setFormatOnSelectedWord(fmt);

        // 创建一个16x16像素的像素图，并用所选颜色填充
        QPixmap pix(16,16);
        pix.fill(color);
        // 将填充了颜色的像素图设置为某个动作（按钮）的图标
        ui->colorAction->setIcon(pix);
    }
}

void MainWindow::paraStyle(int nStyle)
{
    if(activateChildWnd())
        activateChildWnd()->setParaSyle(nStyle);
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
    ui->tileAction->setEnabled(hasChild);
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
    ui->menu_W->addAction(ui->tileAction);
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

void MainWindow::on_tileAction_triggered()
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
    docSave();
}

void MainWindow::on_saveOther_triggered()
{
    docSaveAs();
}

void MainWindow::on_undoAction_triggered()
{
    docUndo();
}

void MainWindow::on_redoAction_triggered()
{
    docRedo();
}

void MainWindow::on_cutAction_triggered()
{
    docCut();
}

void MainWindow::on_copyAction_triggered()
{
    docCopy();
}

void MainWindow::on_pasteAction_triggered()
{
    docPaste();
}

void MainWindow::on_blodAction_triggered()
{
    textBold();
}

void MainWindow::on_inclineAction_triggered()
{
    textItalic();
}

void MainWindow::on_underlineAction_triggered()
{
    textUnderline();
}

void MainWindow::on_fontComboBox_activated(const QString &arg1)
{
    textFamily(arg1);
}

void MainWindow::on_sizeComboBox_activated(const QString &arg1)
{
    textSize(arg1);
}

void MainWindow::on_leftAction_triggered()
{
    if(activateChildWnd())
        activateChildWnd()->setAlignOfDocumentText(1);
}

void MainWindow::on_rightAction_triggered()
{
    if(activateChildWnd())
        activateChildWnd()->setAlignOfDocumentText(2);
}

void MainWindow::on_centerAction_triggered()
{
    if(activateChildWnd())
        activateChildWnd()->setAlignOfDocumentText(3);
}

void MainWindow::on_justifyAction_triggered()
{
    if(activateChildWnd())
        activateChildWnd()->setAlignOfDocumentText(4);
}

void MainWindow::on_colorAction_triggered()
{
    textColor();
}

//项目符号
void MainWindow::on_comboBox_activated(int index)
{
   paraStyle(index);
}

void MainWindow::on_printAction_triggered()
{
    docPrint();
}

void MainWindow::on_printPreviewAction_triggered()
{
    docPrintPreview();
}

void MainWindow::printPreview(QPrinter *printer)
{
    activateChildWnd()->print(printer);
}
