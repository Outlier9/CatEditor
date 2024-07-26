#include "chilewnd.h"
#include <QFileInfo>
#include<QFileDialog>
#include<QTextDocumentWriter>
#include<QMessageBox>
#include<QCloseEvent>

ChileWnd::ChileWnd()
{
    // 子窗口关闭时销毁该类的对象
    setAttribute(Qt::WA_DeleteOnClose);
    //
    m_bSaved = false;

    // 文档内容改变时，更新复制动作的可用状态
    connect(this, &QTextEdit::copyAvailable, this, &ChileWnd::updateCopyAction);
}

// 新建文档
void ChileWnd::newDoc()
{
    static int wndSeqNum = 1; // 新建文档序号
    m_CurDocPath = QString("文档 %1").arg(wndSeqNum++); // 实现新建文档序号自加

    // 设置窗体标题，文档改动后名称后加‘*’
    setWindowTitle(m_CurDocPath + "[*]" + "- MyEditor");
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(docBeModified())); // 连接槽函数
}

// 获取文件名字
QString ChileWnd::getCurDocName()
{
    return QFileInfo(m_CurDocPath).fileName();
}

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
                                 m_CurDocPath,
                                 "HTML文件(*.html *.htm);;"
                                 "所有文档(*.*)");
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
    //创建一个 QTextDocumentWriter 对象 writer，用于写入文档
    QTextDocumentWriter writer(docName);
    bool isSuccess = writer.write(this->document());
    //如果成功写入，就设置文档名字
    if(isSuccess)
    {
        setCurDoc(docName);
    }
    return isSuccess;
}

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


// 文档修改时，窗口的标题栏加‘*’
void ChileWnd::docBeModified()
{
    setWindowModified(document()->isModified());
}

// 更新复制动作的可用状态
void ChileWnd::updateCopyAction()
{
    emit copyAvailble(this->textCursor().hasSelection());
}

