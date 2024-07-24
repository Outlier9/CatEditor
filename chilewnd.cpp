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

// 文档修改时，窗口的标题栏加‘*’
void ChileWnd::docBeModified()
{
    setWindowModified(document()->isModified());
}

// 更新复制动作的可用状态
void ChileWnd::updateCopyAction()
{
    emit copyAvalible(this->textCursor().hasSelection());
}
