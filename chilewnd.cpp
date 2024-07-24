#include "chilewnd.h"
#include <QFileInfo>

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
