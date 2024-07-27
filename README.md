# CatEditor
Cat文本编辑器--Qt

项目类型：桌面应用程序 -->`QMainwindow`

>  项目环境：
>
> - **Qt Creator 版本**：4.11.0
> - **基于的 Qt 版本**：5.14.0
> - **编译器**：MSVC 2017（微软视觉工作室 2017）
> - **编译位数**：32位
> - **编译日期**：2019年12月10日 12:27:13（注意：这里的日期可能存在笔误，通常日期格式为月/日/年）
> - **源代码修订版本**：017ed74400
> - **版权信息**：2008-2019 The Qt Company Ltd（Qt 公司）
> - **法律声明**：程序提供“按原样”（AS IS），不提供任何形式的保证，包括设计、适销性和特定用途的适用性保证。

提示：

1. 文中的代码段，均为写到该过程的时候的节选，并不是完全的代码，所以写到部分函数的声明或者在初始化函数添加内容的时候不做特别阐述，阅读过程中只关注展示的代码的逻辑即可，具体的完整代码需要看源码，源码中大部分都有注释。

2. 我在最开始犯了个低级错误，把childwnd写成了chilewnd，所以后边就将错就错了，不影响观感，阅读的时候注意一下，在写代码过程中多多少少也还出现过类似拼写的错误，自己在源代码中改了，还是最后以源代码为准，功能模块叙述部分这些就没提了。

3. 这是本人第一次写记录项目开发的相关文档，若有错误请多多包涵

# 一、需求设计

## 1.菜单栏设计

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722071374981-c2fc44e1-53b4-47db-8e3d-21ca859ba295.png)

## 2.工具栏设计

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722071386538-4ad5d34d-728b-4949-bd35-1e768b479875.png)

## 3.功能模块设计

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722071393347-2da3c8d7-4ded-4268-b157-5396264d9561.png)

# 二、项目开发

## 1.菜单栏

### （1）添加菜单栏选项

在创建项目文件的时候，选中创建`.ui`文件，项目类型为`QMainwindown`，在`.ui`界面添加菜单栏选项

添加的时候会发现，无法输入中文，这是`QTCreator`一项不太合理的设计，我们需要事先在文本文件中写好所有需要添加的菜单项，然后逐一复制（复制过后记得按回车才能保存），这样就可以创建中文形式的菜单栏

另外需要一并设置快捷键的，创建的时候需要添加取地址符号，例如：`编辑(&E)`

最后效果如图：

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1721563660642-9b27e1cf-972f-4f4d-a85f-8ca247615d69.png)

### （2）添加图标、快捷方式、状态

图标可以从阿里巴巴矢量图标库下载：[iconfont-阿里巴巴矢量图标库](https://www.iconfont.cn/collections/detail?spm=a313x.search_index.0.da5a778a4.36b33a81rV3Wrs&cid=12507)（下载的图片保存到项目文件的同级文件夹中，我这里命名为`images`）

新建`resource.qrc`文件，将图片添加到项目中，记得保存

接下来添加图标，切换到`Action Editor`（在页面的最下方），选中要进行编辑的action，然后再属性栏中找到`icon`属性，点击右边的下三角，选择资源，找到该action对应的图标然后点击OK

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1721564066349-c246790d-5cfa-4309-9360-af600905763e.png)

然后我们在属性栏中设置添加一个状态栏显示（statusTip）以及快捷键（short）的显示，如图：

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1721564374062-2b39867f-97f3-48a4-a037-3d8349b2d46d.png)

依次设置所有的action，如图：

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1721569186206-27a0a05c-1b30-4224-a041-ec57b4e31184.png)

### （3）添加工具栏

如果没有工具栏，在右侧对象和类面板右击Mainwindow选择添加工具栏，直接点击界面然后右击也可以添加工具栏

然后将`Action Editor`区域内的action往工具栏拖拽即可，每拖拽一类的工具栏后可再次添加工具栏，像字体和对齐方式这样同属格式的，可以右击工具栏选择添加分隔符

最后加上调整标题字体字号的下拉框（ComboBox）

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1721575193876-ad9666f4-ea2a-4286-b989-ad2963b3238c.png)

### （4）初始化字号

添加初始化函数`void initMainWindow();`

```cpp
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

}

在构造函数中调用该函数：
    ui->setupUi(this);
    initMainWindow();
```

## 2.功能模块

### （1）新建文件以及多文档界面

MDI（Multiple Document Interface，多文档界面）是一种允许用户同时打开多个文档窗口的界面模式。在这种模式下，每个文档窗口都可以独立操作，并且可以缩放、移动和关闭。

在`.ui`添加MDI控件，然后创建`ChileWnd`类，父类选择`QTextEdit`

在`.ui`界面对newAction转到槽，选`triggered`信号，我们将新建操作进行封装，命名为`docNew`，并在主窗口头文件声明

接着在新建好的子窗口头文件中进行`功能函数和全局变量`的声明，实现子窗口需要的功能

```cpp
#ifndef CHILEWND_H
#define CHILEWND_H

#include <QWidget>
#include<QTextEdit>

class ChileWnd : public QTextEdit
{
    Q_OBJECT
public:
    ChileWnd();
    QString m_CurDocPath; //当前文档路径
    void newDoc(); //新建文档
    QString getCurDocName(); //文档路径中提取文档名

private slots:
    void docBeModified(); //文档修改时，窗口的标题栏加‘*’

private:
    bool m_bSaved; //文档是否保存

};

#endif // CHILEWND_H
```

接着实现各个函数的功能

```cpp
#include "chilewnd.h"
#include<QFileInfo>

ChileWnd::ChileWnd()
{
    //子窗口关闭时销毁该类的对象
    setAttribute(Qt::WA_DeleteOnClose);
    //
    m_bSaved = false;
}

//新建文档
void ChileWnd::newDoc()
{
    static int wndSeqNum = 1; //新建文档序号
    m_CurDocPath = QString("文档 %1").arg(wndSeqNum++); // 实现新建文档序号自加

    //设置窗体标题，文档改动后名称后加‘*’
    setWindowTitle(m_CurDocPath + "[*]" + "- MyWPS");
    connect(document(),SIGNAL(contentsChanged()),this,SLOT(docBeModified()));//连接槽函数
}

//获取文件名字
QString ChileWnd::getCurDocName()
{
    return QFileInfo(m_CurDocPath).fileName();
}

//文档修改时，窗口的标题栏加‘*’
void ChileWnd::docBeModified()
{
    setWindowModified(document()->isModified());
}
void MainWindow::docNew()
{
    ChileWnd *childwnd = new ChileWnd;
    ui->mdiArea->addSubWindow(childwnd);
    // connect(childwnd, &ChileWnd::copyAvailble, ui->cutAction, &QAction::setEnabled);
    // connect(childwnd, &ChileWnd::copyAvailble, ui->copyAction, &QAction::setEnabled);

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
}childwnd->newDoc();
    childwnd->show();
}
```

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1721652795966-0fc2d782-2e4e-4c70-a467-a9eff8530372.png)

### （2）窗口标题图标设置

标题：给窗口设置标题，在`.ui`文件内的属性栏找到`windowTitle`然后直接修改即可

图标：需要`.ico`文件，这里不能直接改后缀，要用软件进行格式转换（尺寸16和32等均可），可使用网页工具[图片转ICO-在线图片格式转换-批量图片格式转换工具](https://www.iyasuo.com/toico)，将文件添加到qrc中即可

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1721654364185-c26d2d13-929a-45e6-8e95-2febf31db1e5.png)

bug报错：（修改完重新构建）

1. [Makefile:84:WPS resource res.o]Error 1 （检查icon文件格式）
2. No rule to make target '../WPS/images/icon.png',needed by'arc_res.cpp'.Stop. （名称不能有中文）

### （3）窗体列表

**①在工具栏的窗体列添加目前创建的文档的列表项功能**

首先要刷新菜单，也就是每次点开软件后都要刷新菜单项的可用状态，根据当前应用程序的状态（如是否有活动窗口、是否有文本被选中等）来启用或禁用菜单项，当用户在MDI应用程序中切换到另一个子窗口时，`QMdiArea` 的 `subWindowActivated` 信号会被触发。这个信号传递了被激活的子窗口的指针。然后在新建文档的时候会自动触发，所以新建的时候也需要进行更新该列表，因此在初始化的时候就添加这两种信号与槽

```cpp
//刷新菜单
refreshMenus();
connect(ui->mdiArea,&QMdiArea::subWindowActivated,this,&MainWindow::refreshMenus);

//更新菜单
addSubWndListMenu();
//点击W（窗体）就更新并添加位于该部分的子窗口列表
connect(ui->menu_W,&QMenu::aboutToShow,this,&MainWindow::addSubWndListMenu);
```

接着是刷新与更新的具体实现：

```cpp
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
    }
    formatEnable();
}
```

**②点击列表项也能实现文档的焦点切换功能**

这里需要使用信号映射器，并在构造函数中（也就是初始化）实现该机制，

整体的过程为：`**触发连接菜单项的****triggered信号**` **-->** `**触发信号映射器的map方法**` **-->** `**连接信号映射器的mapped信号**` **-->** `**执行****setActiveSubWindow槽函数**`

```cpp
//创建信号映射器
m_WndMapper = new QSignalMapper(this);
//点击窗口菜单项的时候，执行map方法，使setActiveSubWindow响应
connect(m_WndMapper,SIGNAL(mapped(QWidget*)),this,SLOT(setActiveSubWindow(QWidget*)));
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
```

- 当 `menuitem_act`（一个 `QAction` 或 `QMenuItem` 对象）被触发（例如用户点击了菜单项）时，它会发出 `triggered` 信号。这个信号被连接到 `m_WndMapper`（一个 `QSignalMapper` 对象）的 `map` 槽函数。`m_WndMapper` 的 `map` 方法被调用。
- 当 `m_WndMapper` 的 `map` 方法被调用时，它会发出 `mapped` 信号，并传递一个 `QWidget*` 参数（参数需要单独设置，使用`setMapping`），这个信号被连接到 `MainWindow` 类的 `setActiveSubWindow` 槽函数。
- `MainWindow` 类的 `setActiveSubWindow` 方法被调用，并传递参数后，这个方法负责将设置参数中指定的窗口为当前活动的子窗口。

拓展：信号映射器的使用场景

**动态信号连接**：当你需要在运行时动态地将信号连接到槽函数时，信号映射器非常有用。例如，你可以在创建对象时决定将哪个信号连接到哪个槽。

**参数传递**：有些信号不提供足够的参数或不提供你需要的参数类型。使用信号映射器可以将信号与一个参数化的槽函数连接，从而传递额外的信息。

**简化代码**：当多个信号需要触发相同的槽函数，但每个信号需要传递不同的参数时，信号映射器可以简化代码。你不需要为每个信号编写单独的连接代码。

**解耦信号和槽**：信号映射器提供了一种将信号源和槽函数解耦的方法。这使得代码更加模块化，易于维护和扩展。

**处理复杂的用户界面事件**：在复杂的用户界面中，可能有许多按钮或其他控件需要触发不同的操作。使用信号映射器可以根据控件的不同状态或类型来调用不同的槽函数。

**实现自定义的信号分发**：如果你需要根据某些条件或逻辑来决定如何响应信号，信号映射器可以作为一个中间件来实现自定义的信号分发逻辑。

**多信号到单槽的映射**：当多个不同的信号需要调用同一个槽函数，但每个信号需要传递不同的参数时，信号映射器可以统一这些信号到一个槽函数。

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722006963161-053c8b34-cbaf-4bb0-b141-aca60959f7c0.png)

### （4）各种功能添加槽函数

在`.ui`文件的各个`Action`选项右击选择转到槽，选择`triggered`信号，调用对应的方法（均已封装好，可以直接调用）

这里有新建、关闭、关闭所有、平铺、层叠、下一个、上一个

```cpp
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
```

但是在关闭窗口的时候，需要处理窗口关闭前的清理工作或处理用户的关闭请求，以防数据丢失，判断是否有未保存或正在进行中的窗口，所以在头文件中声明事件处理函数

```cpp
void MainWindow::closeEvent(QCloseEvent *event)
{
    ui->mdiArea->closeAllSubWindows();
    //防止在关闭主窗口时意外丢失未保存的数据
    if(ui->mdiArea->currentSubWindow())
        event->ignore();  //如果存在当前活动的子窗口，忽略关闭事件。这意味着窗口不会关闭，用户可以继续操作
    else
        event->accept(); //接受此事件
}
```

### （5）打开文档

在`.ui`界面对openAction转到槽，选triggered信号，然后和新建文档一样，我们将打开操作进行封装，命名为`docOpen`

加载文件的时候有可能成功也可能失败，所以这里需要一个验证

```cpp
bool loadDoc(const QString& docName); //加载文档验证
void setCurDoc(const QString& docName);//设置当前文档路径及相关状态
//加载指定名称的文档
bool ChileWnd::loadDoc(const QString &docName)
{
    if(!docName.isEmpty())
    {
        QFile file(docName);
        if(!file.exists()) //文件是否存在
            return false;
        if(!file.open(QFile::ReadOnly)) // 打开文档并检查是否只读
            return false;               //如果文件不能以只读方式打开
        QByteArray text = file.readAll(); //读取文件内容
        if(Qt::mightBeRichText(text)) // 是否是富文本类型并设置文本内容
            setHtml(text);
        else
            setPlainText(text); //设置为纯文本

        setCurDoc(docName); //设置当前文档路径
        connect(document(),SIGNAL(contentsChanged()),this,SLOT(docBeModified()));

        return true;
    }
}

//设置当前文档路径及相关状态
void ChileWnd::setCurDoc(const QString &docName)
{
    //返回标准名称路径，可以过滤“.”和“..”
    m_CurDocPath = QFileInfo(docName).canonicalFilePath();
    m_bSaved = true;                            //文档已被保存
    document()->setModified(false);             //文档未被改动
    setWindowModified(false);                   //窗口不显示改动标识
    setWindowTitle(getCurDocName() + "[*]");    //设置子窗口标题
}
public:
    void docOpen();
private:
//匹配子窗口是否存在
QMdiSubWindow* findChileWnd(const QString &docName);
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
```

### （6）保存/另存为文档

在`.ui`界面对saveAction和saveOther转到槽，选triggered信号，然后和新建文档一样，我们将两种保存操作进行封装，命名为`docSave`和`docSaveAs`

```cpp
bool saveDoc(); //保存文档
bool saveAsDoc(); //另存为
bool saveDocOpt(QString docName); //保存文件的操作逻辑
//保存文档
bool ChileWnd::saveDoc()
{
    //查询文档当前是否保存的状态（全局变量）
    if(m_bSaved)
        return saveDocOpt(m_CurDocPath);
    else
        saveAsDoc();
}

//另存为
bool ChileWnd::saveAsDoc()
{
    QString docName = QFileDialog::getSaveFileName(this,
                                 "另存为",
                                 "文档.txt", // 默认保存为 txt 格式
                                 "文本文件(*.txt);;"
                                 "HTML文件(*.html *.htm);;"
                                 "Word文件(*.doc *.docx);;"
                                 "PDF文件(*.pdf);;"
                                 "图片文件(*.png *.jpg *.jpeg *.bmp);;"
                                 "所有文件(*.*)");
    if(docName.isEmpty())
        return false;
    else
        return saveDocOpt(docName);
}

//保存文件的操作逻辑
bool ChileWnd::saveDocOpt(QString docName)
{
    //CaseInsensitive大小写都可以
    if( !(docName.endsWith(".htm",Qt::CaseInsensitive)||
                        docName.endsWith(".html",Qt::CaseInsensitive)))
    {
        //如果没有后缀就添加
        docName += ".html";
    }
    ////创建一个 QTextDocumentWriter 对象 writer，用于写入文档
    QTextDocumentWriter writer(docName);
    bool isSuccess = writer.write(this->document());
    //如果成功写入，就设置文档名字
    if(isSuccess)
    {
        setCurDoc(docName);
    }
    return isSuccess;
}
void docSave();
void docSaveAs();
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

//Action转到槽
void MainWindow::on_saveAction_triggered()
{
    docSave();
}

void MainWindow::on_saveOther_triggered()
{
    docSaveAs();
}
```

在关闭文档的时候，如果文档此时的状态是被修改且未保存，就出现一个提示并可以选择保存，在子类中实现该方法`promptSave`，然后添加关闭事件处理

```cpp
protected:
    void closeEvent(QCloseEvent *event); //关闭事件发生时，进行未保存该文档提示
private:
    bool promptSave(); //提示未保存但是要关闭的时候进行保存
void ChileWnd::closeEvent(QCloseEvent *event)
{
    if(promptSave())
        event->accept();
    else
        event->ignore();

}

//提示未保存但是要关闭的时候进行保存
bool ChileWnd::promptSave()
{
    if(!document()->isModified())
        return true;
    //提示消息框
    QMessageBox::StandardButton result;
    result = QMessageBox::warning(this,
                                  QString("系统提示"),
                                  QString("文档%1已修改，是否保存？")
                                  .arg(getCurDocName()),
                                  QMessageBox::Yes | QMessageBox::Discard | QMessageBox::Cancel);
    if(result == QMessageBox::Yes)
        return saveDoc();
    else if(result == QMessageBox::Cancel)
        return false;
    return true;

    if(!document()->isModified())
        return true;

}
```

### （7）文档内容操作

文档内容的剪切、复制、粘贴，撤销（上一步）、重写（下一步）等操作，同上，在`.ui`界面对这些Action转到槽，选triggered信号，关于这几种功能，在Qt中有已实现的函数，直接调用即可

```cpp
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
```

### （8）字体格式

文字的加粗、倾斜、下划线操作，同上，在`.ui`界面对这些Action转到槽，选triggered信号，关于这几种功能，是需要对选中的文字进行设置

```cpp
void setFormatOnSelectedWord(const QTextCharFormat &fmt);//对选中的字体格式进行设置
void ChileWnd::setFormatOnSelectedWord(const QTextCharFormat &fmt)
{
    //获取文档光标
    QTextCursor tcursor = textCursor();
    if(!tcursor.hasSelection())
        tcursor.select(QTextCursor::WordUnderCursor);//选中模式
    tcursor.mergeCharFormat(fmt);
    //合并格式
    mergeCurrentCharFormat(fmt);
}
//转到槽
void on_blodAction_triggered();

void on_inclineAction_triggered();

void on_underlineAction_triggered();
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
```

在该功能实现中，对字体的设置是对选中的字体进行设置，所以代码在`isChecked`这一步，需要确保该Action是可勾选的，也就是`Checkable`

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722006879306-b20e37b4-d245-4cea-aba1-21cd7fa224ea.png)

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722007376880-32a1fbb1-f767-4818-8a27-1b767c1cc9d8.png)

### （9）字号字体

文字设置字体和字号操作，在`.ui`界面对这些控件转到槽，选`activated(QString)`信号，关于这几种功能，是需要对选中的文字进行设置

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722007834323-dcb806fc-0545-417b-9e4f-29b5dcf9cd20.png)



```cpp
void textFamily(const QString &fmly); //设置字体
void textSize(const QString &ps); //设置字号
void MainWindow::on_fontComboBox_activated(const QString &arg1)
{
    textFamily(arg1);
}

void MainWindow::on_sizeComboBox_activated(const QString &arg1)
{
    textSize(arg1);
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
```

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722008152476-bd989295-2ee6-44e1-9fa3-5f5496231636.png)

### （10）段落对齐

对齐方式有左端对齐、居中对齐、右端对齐、两端对齐，同时只能存在一种，所以这四种互斥，需要在初始化中设置互斥性

```cpp
//对齐方式互斥性,一次只能选一种
    QActionGroup *alignGroup = new QActionGroup(this);
    alignGroup->addAction(ui->leftAction);
    alignGroup->addAction(ui->rightAction);
    alignGroup->addAction(ui->centerAction);
    alignGroup->addAction(ui->justifyAction);
```

然后完成段落对齐的逻辑

```cpp
void setAlignOfDocumentText(int aligntype); //设置段落对齐方式
//设置段落对齐方式
void ChileWnd::setAlignOfDocumentText(int aligntype)
{
    //给传入的参数设置编号，1-->左端对齐，2-->右端对齐，3-->居中对齐，4-->两端对齐
    if(aligntype == 1)
    {
        setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    }
    else if(aligntype == 2)
    {
        setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    }
    else if(aligntype == 3)
    {
        setAlignment(Qt::AlignCenter);
    }
    else if(aligntype == 4)
    {
        setAlignment(Qt::AlignJustify);
    }
}
    void on_leftAction_triggered();
    
    void on_rightAction_triggered();
    
    void on_centerAction_triggered();
    
    void on_justifyAction_triggered();
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
```

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722008972852-13da2ed2-1427-4a24-a1fc-e6253942a676.png)

### （11）字体颜色

文字设置颜色操作，在`.ui`界面对`colorAction`转到槽，选`triggered`信号，然后将功能封装为`textcolor`

```cpp
void textColor(); //设置颜色
void MainWindow::on_colorAction_triggered()
{
    textColor();
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
```

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722054033319-1e65e31e-ce55-4c61-a8a3-4f2fbb6f68e7.png)

### （12）项目符号

给文字设置项目符号操作，在`.ui`界面对这些控件转到槽，选`activated(int)`信号

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722059943158-6c628383-bde3-4def-860b-50ecfba0a116.png)

```cpp
void paraStyle(int nStyle); //设置项目符号
void MainWindow::on_comboBox_activated(int index)
{
   paraStyle(index);
}

void MainWindow::paraStyle(int nStyle)
{
    if(activateChildWnd())
        activateChildWnd()->setParaSyle(nStyle);
}
void setParaSyle(int pstyle); //设置项目符号
void ChileWnd::setParaSyle(int pstyle)
{
    // 获取当前文本光标
    QTextCursor tcursor = textCursor();
    // 声明一个 QTextListFormat::Style 变量，用于存储列表样式
    QTextListFormat::Style sname;
    // 如果 pstyle 不为 0，设置列表样式
    if(pstyle != 0)
    {
        // 根据 pstyle 的值设置不同的列表样式
        switch (pstyle) {
        case 1:
            sname = QTextListFormat::ListDisc; //黑心实心圆
            break;
        case 2:
            sname = QTextListFormat::ListCircle; //空心圆
            break;
        case 3:
            sname = QTextListFormat::ListSquare; //方形
            break;
        case 4:
            sname = QTextListFormat::ListDecimal; //十进制整数
            break;
        case 5:
            sname = QTextListFormat::ListLowerAlpha; //小写字母
            break;
        case 6:
            sname = QTextListFormat::ListUpperAlpha; //大写字母
            break;
        case 7:
            sname = QTextListFormat::ListLowerRoman; //小写罗马字母
            break;
        case 8:
            sname = QTextListFormat::ListUpperRoman; //大写罗马字母
            break;
        default:
            sname = QTextListFormat::ListDisc;
        }

        // 开始编辑块
        tcursor.beginEditBlock();
        // 获取当前段落格式
        QTextBlockFormat tBlockFmt = tcursor.blockFormat();
        // 声明一个 QTextListFormat 变量，用于存储列表格式
        QTextListFormat tListFmt;
        // 如果当前光标所在位置已经有列表
        if(tcursor.currentList())
        {
            // 获取当前列表的格式
            // 使用format方法需要添加头文件QtWidgets
            tListFmt = tcursor.currentList()->format();
        }
        else
        {
            // 如果没有列表，设置新的列表格式
            tListFmt.setIndent(tBlockFmt.indent()+1);
            tBlockFmt.setIndent(0);
            tcursor.setBlockFormat(tBlockFmt);
        }

        // 设置列表样式
        tListFmt.setStyle(sname);
        // 创建列表
        tcursor.createList(tListFmt);
        // 结束编辑块
        tcursor.endEditBlock();
    }
    else
    {
        // 如果 pstyle 为 0，清除列表格式
        QTextBlockFormat tbfmt;
        tbfmt.setObjectIndex(-1);
        tcursor.mergeBlockFormat(tbfmt);
    }
}
```

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722060241875-487b4c64-9b10-4336-875b-9ab47f827414.png)

### （13）文档打印/预览

该功能需要在项目文件里添加模块`printsupport`

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722063761094-e2abbeb1-01bc-4649-8fbc-b1debab8193b.png)

然后将文档打印功能封装，实现该函数时需要添加头文件`<QtPrintSupport/QPrinter>`和`<QtPrintSupport/QPrintDialog>`

文档打印操作，在`.ui`界面对`printAction`转到槽，选`triggered`信号，然后将功能封装为`docPrint`

```cpp
void docPrint();//文档打印
void MainWindow::on_printAction_triggered()
{
    docPrint();
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
```

**文档打印**

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722064385203-5c3f626a-1aee-4b84-9b09-752ef347a766.png)

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722064424383-5a077abc-bbb9-47b3-8b9e-3bbeb449a4e8.png)

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722064466115-d4e0459f-5cd2-4159-ba16-69c1d33373fb.png)

**打印预览**

文档打印操作，在`.ui`界面对`printPreviewAction`转到槽，选`triggered`信号，然后将功能封装为`docPrintPreview`

将打印预览功能封装，实现该函数时需要添加头文件`<QtPrintSupport/QPrintPreviewDialog>`

```cpp
void docPrintPreview();//打印预览
void printPreview(QPrinter* printer); //printPreview 槽函数
void MainWindow::on_printPreviewAction_triggered()
{
    docPrintPreview();
}

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

    //每当预览对话框需要绘制预览时，都会触发 paintRequested 信号，从而调用 printPreview 槽函数
}


void MainWindow::printPreview(QPrinter *printer)
{
    activateChildWnd()->print(printer);
}
```

![img](https://cdn.nlark.com/yuque/0/2024/png/29309193/1722065185449-d577e182-e7ce-4870-b567-b7927ac03eb4.png)

# 三、Bug/疑问集合

## 1.关于子窗口创建的疑问

疑惑点：在写该项目的时候，我对控件的了解只是知道最基本的常用控件，诸如使用到的`MDI Area`并不熟悉，因此在实现新建文档的功能的时候我会产生一个疑问： 我并没有发现new的语句，这个窗口是怎么创建的？再三检查相关代码，整个过程中我只创建了Chilewnd实例`ChileWnd *childwnd = new ChileWnd;`并添加到了`MDI`中`ui->mdiArea->addSubWindow(childwnd);`

但是这个类属于自定义类，且类的实现过程中，更没有创建的语句。按常例来说，假如我要创建一个按钮控件，我要写`new QPushButton();`然后将其添加到`ui`中，所以对此处我向GPT进行求问：

解答：当将一个 `ChileWnd` 实例添加到 `QMdiArea` 中时，它会自动成为一个子窗口。`ChileWnd` 本质上是继承自 `QTextEdit` 的一个自定义类，因此它具备 `QTextEdit` 的所有功能，并且可以被添加到 `QMdiArea` 中管理。在 Qt 的 `QMdiArea` 中，**任何继承自** `**QWidget**` **的类都可以作为子窗口添加，不需要特别考虑类型，只要是** `**QWidget**` **或其子类都可以。你可以将其理解为任何可以显示的控件都可以添加到** `**QMdiArea**` **中，而** `**QMdiArea**` **会将其作为子窗口处理。**

## 2.关于给窗体设置图标的时候遇到的bug

① [Makefile:84:WPS resource res.o]Error 1 

检查icon文件格式，不能直接png等图片改后缀，要用专门的工具进行格式转换

[图片转ICO-在线图片格式转换-批量图片格式转换工具](https://www.iyasuo.com/toico)

② No rule to make target '../WPS/images/icon.png',needed by'arc_res.cpp'.Stop. 

名称不能有中文，可以试试修改路径中的中文，不过我的路径中均为英文，遇到这个错误是因为将images中的png直接转后缀为ico文件后，又上传了qrc中，两个命名重复且已上传到qrc中的png图片已不存在，我将这还原以后就不再报错了，所以这里的图片操作要规范，不能图省事

# 四、bug修复日志

7.27-----修复-----新建文档后输入内容选中无法设置段落对齐方式以及字体格式的bug

7.27-----完善-----将保存格式从一种（html）添加到office能保存的所有格式，并设置默认格式为txt

7.27-----修复-----新建文档的槽函数报错，以前使用新语法解决该问题，以为是语法的错误，后发现槽函数应设置setEnable，写成了setAvaliable，导致槽函数无法使用，已改正，以前的解决方式也可以，在原文中注释