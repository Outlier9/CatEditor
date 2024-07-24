#ifndef CHILEWND_H
#define CHILEWND_H

#include <QWidget>
#include <QTextEdit>

class ChileWnd : public QTextEdit
{
    Q_OBJECT
public:
    ChileWnd();
    QString m_CurDocPath; // 当前文档路径
    void newDoc(); // 新建文档
    QString getCurDocName(); // 从路径中提取文档名

signals:
    void copyAvalible(bool available);

private slots:
    void docBeModified(); // 文档修改时，窗口的标题栏加‘*’
    void updateCopyAction(); // 更新复制动作的可用状态

private:
    bool m_bSaved; // 文档是否保存
};

#endif // CHILEWND_H
