#ifndef FILEEDIT_H
#define FILEEDIT_H

#include "QString"
#include "QTabWidget"
#include "QTextEdit"
#include "QVector"
#include "QRegularExpression"
#include "QFile.h"
#include "QTextStream"
#include "QMessageBox"
#include "QPushButton"

class fileEditTextNameClass{
    // 用于获取新建文件名，作为tab选项卡的标题，从1开始，到int能表示的最大的数字，以tabPage_1的格式命名
private:
    QVector<int>id;
public:
    fileEditTextNameClass(){
        // 构造函数，无需做任何事
    }
    QString getFileEditTextName()
    {
        // 获取文件名，例 Untitled_1
        // 先从1开始，判断到id.count()之间是否有可用的id
        // int maxId = *std::max_element(id.begin(),id.end()); // 找到最大元素
        for(int i = 1;i <= 10;i++)
        {
            // qDebug()<<"当前遍历到数字"<<i;
            if(!id.contains(i))
            {
                // qDebug()<<"这个数字可用";
                // 当前i可用
                id.append(i);
                QString name = "Untitled_";
                name += QString::number(i,10);
                return name;
            }
        }
        // 1~id.count()都没有可用的id，则直接返回id.count() + 1;
        int i = id.count() + 1;
        id.append(i);
        QString name = "Untitled_";
        name += QString::number(i,10);
        return name;
    }

    // 当标题为 Untitled_数字 的标签被关闭，调用该函数释放id，供下次新建文件时使用
    void removeFileEditTextNameId(int x)
    {
        for(int i = 0;i < this->id.count(); i++)
        {
            if(id[i] == x)
            {
                qDebug()<<"释放文件名id成功"<<x;
                id.remove(i);
                return;
            }
        }
        qDebug()<<"释放文件名id时，未找到传入的id"<<x;
    }
};

// Tab标签页的结构体
typedef struct fileEditPage{
    // QString tabPageName;// tab选项框的名称，用于标识(弃用，因为直接保存了textEdit的指针)
    QString tabPageTitle;// tab选项框的标题，给用户看的，用户可自行更改
    QTextEdit *textEdit;//textEdit指针
    QString filePath;// 当前文件路径
    bool isSave;// 是否保存
    // bool reName;// 是否重命名(保存时是否修改文件名)（又弃用，根据filePath区分是否为新建的空文件）
}fileEditPageStruct;

class FileEditClass : public fileEditTextNameClass{
private:
    QTabWidget *tabWidget;
    // int length;//（弃用，QVector内置方法已经有了count）
    QVector<fileEditPageStruct>pageInfo;
    bool closeStatus;// 为true时，关闭状态，只允许关闭文件
public:
    FileEditClass(){
        // this->length = 0;
        closeStatus = false;
    }
    FileEditClass(QTabWidget *tabWidget){
        // this->length = 0;
        this->tabWidget = tabWidget;
        closeStatus = false;
        // this->addFile();
    }
    void setTabWidget(QTabWidget *tabWidget)
    {
        this->tabWidget = tabWidget;
    }
    // 以上都属于初始化的方法

    // 新建空文件
    QTextEdit* addFile(){
        if(closeStatus)
            return NULL;
        QTextEdit *textEdit = new QTextEdit();// 创建textEdit对象
        fileEditPageStruct fileEditPageStructure;
        fileEditPageStructure.tabPageTitle = this->getFileEditTextName();
        fileEditPageStructure.textEdit = textEdit;
        fileEditPageStructure.filePath = "";
        fileEditPageStructure.isSave = true;
        this->pageInfo.append(fileEditPageStructure);// 向PageInfo中加入该结构体
        int index = this->tabWidget->addTab(textEdit,fileEditPageStructure.tabPageTitle);// 添加Tab标签页，返回值为后续置焦点做准备
        this->tabWidget->setCurrentIndex(index);// 在Tab选项卡中将新建的文件置焦点
        this->tabWidget->setTabIcon(index,QIcon(":/icon/src/file.png"));// 为Tab标签页添加Icon
        qDebug()<<"新建文件：index ="<<index<<",Title ="<<fileEditPageStructure.tabPageTitle;
        return textEdit;
        // 在tab选项框中加入一个layout，在layout中再加入textEdit（该方法已弃用，因为tab选项框中可以直接加入textEdit）
        // QWidget *tabPage = new QWidget();
        // QHBoxLayout *layout = new QHBoxLayout(this->tabWidget);
        // QTextEdit *textEdit = new QTextEdit();
        // fileEditPageNew->tabPage = tabPage;
        // fileEditPageNew->textEdit = textEdit;
        // layout->addWidget(textEdit);
        // layout->setContentsMargins(0,0,0,0);
        // tabPage->setLayout(layout);
        // this->tabWidget->addTab(tabPage,fileEditPageNew->tabPageTitle);
    }

    // 当前标签页切换
    void currentPageChanged(int index)
    {
        qDebug()<<"Tab选项框的标签页被切换：index ="<<index;
        // qDebug()<<this->tabWidget->currentWidget();

    }

    // Tab选项卡中的下标转化为PageInfo中的下标
    int tabIndextoInfoIndex(int index)
    {
        // 获取关闭标签页在PageInfo中的下标
        for(int i = 0;i < this->pageInfo.count();i++)
        {
            if(this->pageInfo[i].textEdit == this->tabWidget->widget(index))
            {
                return i;
            }
        }
        return -1;// 错误
    }

    QTextEdit* getCurrentTextEdit()
    {
        return (QTextEdit *)this->tabWidget->currentWidget();
    }

    // QString对象中提取int
    int QStringtoInt(QString &str)
    {
        QRegularExpression rx("\\d+");
        QRegularExpressionMatch match = rx.match(str, 0);
        if(match.hasMatch())
        {
            return match.captured(0).toInt();
        }else{
            return -1;// 未提取出任何整数
        }
    }

    // 新建的文件名中，将Untitled_后的数字提取，若不成功返回-1
    int QStringtoIntLimitFileName(QString &str)
    {
        if(str.contains("Untitled_"))
        {
            return QStringtoInt(str);
        }else{
            return -1;
        }
    }

    // 根据Tab选项卡当前选定的标签下标获取其文件名（即标题栏名称）
    QString getFileNamebyCurrentIndex(int index)
    {
        return this->pageInfo[tabIndextoInfoIndex(index)].tabPageTitle;
    }

    void setFilePathbyCurrentIndex(int index, QString &str)
    {
        this->pageInfo[tabIndextoInfoIndex(index)].filePath = str;
    }


    // 打开文件
    QTextEdit* openFile(QString &filePath)
    {
        // 打开文件
        QFile file(filePath);
        QString content;
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            content = in.readAll();
            file.close();
        }else{
            qDebug()<<"打开文件失败";
            return NULL;
        }
        QTextEdit *textEdit = new QTextEdit();
        textEdit->setText(content);
        fileEditPageStruct fileEditPageStructure;
        fileEditPageStructure.tabPageTitle = filePath.split("/").last();//替换为文件名
        fileEditPageStructure.textEdit = textEdit;
        fileEditPageStructure.filePath = filePath;
        fileEditPageStructure.isSave = true;
        // fileEditPageStructure.reName = true;// 打开的文件，不需要进入命名规则桶
        this->pageInfo.append(fileEditPageStructure);
        int index = this->tabWidget->addTab(textEdit,fileEditPageStructure.tabPageTitle);
        this->tabWidget->setCurrentIndex(index);// 在Tab选项卡中将打开的文件置焦点
        this->tabWidget->setTabIcon(index,QIcon(":/icon/src/file.png"));
        qDebug()<<"成功打开文件：index ="<<index<<",Title ="<<fileEditPageStructure.tabPageTitle;
        return textEdit;
    }

    // 保存文件，返回值：1保存成功，0保存失败，-1空路径
    int saveFile(int index)
    {
        int InfoIndex = tabIndextoInfoIndex(index);// 获取在PageInfo中的下标
        if(this->pageInfo[InfoIndex].filePath.isEmpty())
        {
            // 文件路径为空，则需要用户给定保存路径及文件名，先返回，等待后续调用
            return 0;
        }else{
            // 文件路径不为空，则可以保存
            QString savefilePath = this->pageInfo[InfoIndex].filePath;
            QFile file(savefilePath);
            qDebug()<<"正在保存到"<<savefilePath;
            if(file.open(QIODevice::WriteOnly | QIODevice::Text))//打开已存在的文件或创建未存在的文件
            {
                // 更新UI标题
                this->pageInfo[InfoIndex].tabPageTitle = this->pageInfo[InfoIndex].filePath.split("/").last();// 先将filePath中的文件名替换为标题
                this->tabWidget->setTabText(index,this->pageInfo[InfoIndex].tabPageTitle);// 更新到ui
                // 将文本框内的内容写入,以字节数组的形式读出并存储到文件中
                QString msg = this->pageInfo[InfoIndex].textEdit->toPlainText();
                file.write(msg.toUtf8());
                file.close();
                // 保存成功的标志位
                this->setSaveFile(index);
                this->tabWidget->setTabIcon(index,QIcon(":/icon/src/file.png"));
                return 1;
            }else{
                // 文件打开失败
                return -1;
            }
        }
        return -1;
    }

    // 置标志位，为不保存并退出的情况设计该函数，同时也在保存过程中使用该函数
    void setSaveFile(int index)
    {
        int InfoIndex = tabIndextoInfoIndex(index);// 获取在PageInfo中的下标
        this->pageInfo[InfoIndex].isSave = true;
    }

    // 标签页被按下关闭按钮，返回值：1关闭成功，-1未保存，询问是否保存
    int closeTabPage(int index)
    {
        qDebug()<<"Tab选项框的标签页关闭按钮被点击：PageIndex ="<<index;
        QTextEdit *closeTabPagePointer = (QTextEdit *)this->tabWidget->widget(index);// 关闭标签页的指针（QTextEdit）
        int InfoIndex = tabIndextoInfoIndex(index);// 在PageInfo中的下标

        // 判断该编辑框中是否有内容，没有内容则可以直接关闭
        QString text = closeTabPagePointer->toPlainText();
        if(text.isEmpty())
        {
            // 编辑框内容为空，关闭
            int releaseId = this->QStringtoIntLimitFileName(this->pageInfo[InfoIndex].tabPageTitle);// 如果当前关闭的标签页名称为默认的，则释放该名称
            if(releaseId != -1)
                this->removeFileEditTextNameId(releaseId);
            this->tabWidget->removeTab(index);// ui中移除该标签页
            this->pageInfo.remove(InfoIndex);// pageInfo中移除该结构体
            // removeFileEditTextNameId
            return 1;
        }
        // 判断该编辑框是否已保存，若未保存则提示用户
        if(this->pageInfo[InfoIndex].isSave == true)
        {
            // 已保存或未修改，直接关闭
            int releaseId = this->QStringtoIntLimitFileName(this->pageInfo[InfoIndex].tabPageTitle);// 如果当前关闭的标签页名称为默认的，则释放该名称
            if(releaseId != -1)
                this->removeFileEditTextNameId(releaseId);
            this->tabWidget->removeTab(index);// ui中移除该标签页
            this->pageInfo.remove(InfoIndex);// pageInfo中移除该结构体
            // removeFileEditTextNameId
            return 1;
        }else{
            // 未保存
            return -1;
        }
    }

    // 文本框中内容被改变，会触发该函数
    void textEditChanged()
    {
        int InfoIndex = this->tabIndextoInfoIndex(this->tabWidget->currentIndex());
        this->pageInfo[InfoIndex].isSave = false;
        this->tabWidget->setTabIcon(this->tabWidget->currentIndex(),QIcon("file_unsave"));
        this->tabWidget->setTabIcon(this->tabWidget->currentIndex(),QIcon(":/icon/src/file_unsave.png"));
    }

    // 获取当前文件是否保存
    bool getFileisSave()
    {
        int InfoIndex = this->tabIndextoInfoIndex(this->tabWidget->currentIndex());
        return this->pageInfo[InfoIndex].isSave;
    }

    // 获取当前文本框的行数
    int getLineCountbyCurrentIndex()
    {
        // 先，获取下标
        int currentIndex = this->tabWidget->currentIndex();
        // 再，info中找到TextEdit对象
        QTextEdit *textEdit = this->pageInfo[this->tabIndextoInfoIndex(currentIndex)].textEdit;
        // 后，统计，返回
        return textEdit->document()->lineCount();
    }

    // 获取当前标签页
    int getFileCount()
    {
        return this->pageInfo.count();
    }

    void setCloseStatus(bool status)
    {
        this->closeStatus = status;
    }

    void log()
    {
        // qDebug()<<this->head.next;
    }

    // QT6中不能使用malloc手动分配内存空间，会导致程序崩溃（弃用）
    // fileEditPageStruct* getFileEditPageStructure()
    // {
    // 分配内存空间给fileEditPageStruct，并初始化
    // fileEditPageStruct* fileEditPageNew = (fileEditPageStruct*)malloc(sizeof(fileEditPageStruct));
    // if(fileEditPageNew == NULL)
    // qDebug()<<"malloc fail";
    // qDebug()<<fileEditPageNew->next;
    // fileEditPageNew->tabPageTitle = this->getFileEditTextName();// 获取该tab选项卡标签标题
    // fileEditPageNew->textEdit = NULL;
    // fileEditPageNew->filePath = "";
    // fileEditPageNew->isSave = true;
    // fileEditPageNew->next = NULL;
    //     return fileEditPageNew;
    // }
};

#endif // FILEEDIT_H
