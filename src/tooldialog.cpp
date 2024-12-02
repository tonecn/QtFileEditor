#include "tooldialog.h"
#include "ui_tooldialog.h"
#include "QMessageBox"

ToolDialog::ToolDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ToolDialog)
{
    ui->setupUi(this);
    ui->horizontalSlider->setValue(20);//为窗口不透明度滑块设定初始值
    this->on_horizontalSlider_valueChanged(20);// 响应不透明度
    this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::Dialog);// 设定固定大小不可调整
}

ToolDialog::~ToolDialog()
{
    delete ui;
}

// 根据传入的标题名称设定Tab选项卡及窗口标题
void ToolDialog::setToolMode(QString title)
{
    this->setWindowTitle(title);
    qDebug()<<"ToolMode:"<<title;
    if(title == "查找")
        ui->tabWidget->setCurrentIndex(0);
    else if(title == "替换")
        ui->tabWidget->setCurrentIndex(1);
}

// 设定当前操作的文本编辑框指针
void ToolDialog::setCurrentTextEdit(QTextEdit *textEdit)
{
    this->textEdit = textEdit;
}


/* 以下均为槽函数 */
// 窗口及工作模式区域
// 取消按钮，关闭窗口
void ToolDialog::on_pushButton_Cancal_clicked()
{
    this->close();
}
// 当前的Tab选项标签页改变，即切换工具模式
void ToolDialog::on_tabWidget_currentChanged(int index)
{
    if(index == 0)
        this->setToolMode("查找");
    else if(index == 1)
        this->setToolMode("替换");
}


// 窗口透明度区
// 窗口透明度滑块条值改变
void ToolDialog::on_horizontalSlider_valueChanged(int value)
{
    qDebug()<<"窗口透明度滑块条值改变："<<value;
    if(ui->radioButton_Always->isChecked())
    {
        // 始终有窗口透明度 开启
        this->setWindowOpacity((100-value)/100.0);
    }else if(ui->radioButton_Close->isChecked())
    {
        // 关闭了窗口透明度
        this->setWindowOpacity(1);
    }
}
// 始终有窗口透明度 单选框被按下
void ToolDialog::on_radioButton_Always_clicked()
{
    this->on_horizontalSlider_valueChanged(ui->horizontalSlider->value());
}
// 关闭窗口透明度 单选框被按下
void ToolDialog::on_radioButton_Close_clicked()
{
    this->on_horizontalSlider_valueChanged(ui->horizontalSlider->value());
}

// 查找区
// 查找下一个
void ToolDialog::on_pushButton_SearchNext_clicked()
{
    // 判定查找目标是否为空
    if(ui->lineEditTarget->text().isEmpty())
    {
        QMessageBox::information(nullptr,"查找","请输入需要查找的目标");
        return;
    }
    QTextCursor textCursor = this->textEdit->textCursor();
    if(ui->checkBox_SearchReverse->isChecked())
    {
        // 反向查找
        QString toSearchString = this->textEdit->toPlainText().mid(0,textCursor.position());
        int position = toSearchString.lastIndexOf(ui->lineEditTarget->text());
        if(position == -1)
        {
            // 未找到下一个
            textCursor.setPosition(this->textEdit->toPlainText().length());
            this->textEdit->setTextCursor(textCursor);
            QMessageBox::information(nullptr,"查找","未找到下一个，已将光标移动到末尾位置");
        }else{
            // 找到了下一个
            textCursor.setPosition(textCursor.position() - (toSearchString.length() - position) + 1);
            textCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, ui->lineEditTarget->text().length());
            this->textEdit->setTextCursor(textCursor);
        }
    }else{
        // 正向查找
        QString toSearchString = this->textEdit->toPlainText().mid(textCursor.position());
        int position = toSearchString.indexOf(ui->lineEditTarget->text());
        if(position == -1)
        {
            // 未找到下一个
            textCursor.setPosition(0);
            this->textEdit->setTextCursor(textCursor);
            QMessageBox::information(nullptr,"查找","未找到下一个，已将光标移动到起始位置");
        }else{
            // 找到了下一个
            textCursor.setPosition(textCursor.position() + position);
            textCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, ui->lineEditTarget->text().length());
            this->textEdit->setTextCursor(textCursor);
        }
    }
}
// 计数
void ToolDialog::on_pushButton_SearchCount_clicked()
{
    // 判定查找目标是否为空
    if(ui->lineEditTarget->text().isEmpty())
    {
        QMessageBox::information(nullptr,"查找","请输入需要查找的目标");
        return;
    }
    QString toSearchString = this->textEdit->toPlainText();// 全选文本
    int count = 0;
    while(1)
    {
        int position = toSearchString.indexOf(ui->lineEditTarget->text());
        if(position == -1)
        {
            // 未找到下一个
            break;
        }else{
            // 找到了下一个，切片，继续查找
            toSearchString = toSearchString.mid(position + ui->lineEditTarget->text().length());
            count++;
        }
    }
    if(count == 0)
    {
        // 未找到任何一个
        QMessageBox::information(nullptr,"查找","没有找到匹配的目标");
    }else{
        QMessageBox::information(nullptr,"查找","一共有"+QString::number(count)+"个匹配的目标");
    }
}
// 查找所有(弃用，不支持多光标显示)
// void ToolDialog::on_pushButton_SearchAll_clicked()
// {
//     QString toSearchString = this->textEdit->toPlainText();// 全选文本
//     QTextCursor textCursor = this->textEdit->textCursor();// 创建text指针
//     int count = 0;
//     while(1)
//     {
//         int postion = toSearchString.indexOf(ui->lineEditTarget->text());
//         if(postion == -1)
//             break;
//         qDebug()<<"在postion= 的位置找到了一个匹配对象"<<postion;
//         textCursor.setPosition(count + postion);
//         textCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, ui->lineEditTarget->text().length());
//         count += postion;
//         toSearchString = toSearchString.mid(postion + ui->lineEditTarget->text().length());
//     }
//     this->textEdit->setTextCursor(textCursor);
// }

// 替换区
// 替换
void ToolDialog::on_pushButton_Replace_clicked()
{
    // 判定替换目标是否为空
    if(ui->lineEditTarget->text().isEmpty())
    {
        QMessageBox::information(nullptr,"查找","请输入需要查找的目标");
        return;
    }
    QTextCursor textCursor = this->textEdit->textCursor();
    if(textCursor.selectedText().length() != ui->lineEditTarget->text().length())
    {
        // 未选择需要替换的文本
        QMessageBox::information(nullptr,"替换"," 未选择需要替换的文本，请先查找");
        return;
    }
    textCursor.removeSelectedText();
    textCursor.insertText(ui->lineEditReplaceTo->text());
}
// 替换所有
void ToolDialog::on_pushButton_ReplaceAll_clicked()
{
    // 判定替换目标是否为空
    if(ui->lineEditTarget->text().isEmpty())
    {
        QMessageBox::information(nullptr,"查找","请输入需要查找的目标");
        return;
    }
    // 从头开始替换
    QTextCursor textCursor = this->textEdit->textCursor();// 创建text指针
    textCursor.setPosition(0);// 从0开始
    int count = 0;
    while(1)
    {
        QString toSearchString = this->textEdit->toPlainText().mid(textCursor.position());
        int position = toSearchString.indexOf(ui->lineEditTarget->text());
        if(position == -1)
        {
            // 未找到下一个
            break;
        }else{
            // 找到了下一个
            textCursor.setPosition(textCursor.position() + position);// 移动光标
            textCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, ui->lineEditTarget->text().length());// 选中需要替换的文本
            textCursor.removeSelectedText();
            textCursor.insertText(ui->lineEditReplaceTo->text());// 替换
            this->textEdit->setTextCursor(textCursor);
            count++;
        }
    }
    if(count == 0)
        QMessageBox::information(nullptr,"替换"," 未找到需要替换的文本");
    else
        QMessageBox::information(nullptr,"替换","一共替换了" + QString::number(count) + "个文本");
}

