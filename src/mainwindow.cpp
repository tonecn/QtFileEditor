#include "mainwindow.h"
#include "QMainWindow"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"
#include "QTabWidget"
#include "QTextEdit"
#include "QDebug"
#include "QClipboard"
#include "tooldialog.h"
#include "QCloseEvent"

// MainWindow构造函数
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),  ui (new Ui::MainWindow) ,FileEdit(new FileEditClass), tabWidget(new QTabWidget())
{
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("FileEditor"));// 设置窗口标题
    this->setWindowIcon(QIcon(":/icon/src/icon.png"));// 图标来源：阿里巴巴矢量图标库
    // 创建菜单栏
    QMenuBar *menuBar = this->menuBar();
    // 创建菜单（用addMenu方法添加入菜单栏）
    QMenu* MenuBar_File = menuBar->addMenu(QStringLiteral("文件(&F)"));
    QMenu* MenuBar_Edit = menuBar->addMenu(QStringLiteral("编辑(&E)"));
    QMenu* MenuBar_Tool = menuBar->addMenu(QStringLiteral("工具(&O)"));
    // QMenu* MenuBar_Setting = menuBar->addMenu(QStringLiteral("设置(&S)"));(暂时弃用)
    QMenu* MenuBar_About = menuBar->addMenu(QStringLiteral("关于(&A)"));


    // 创建菜单Action
    // 文件区
    QAction* Menu_File_NewFile = new QAction(QStringLiteral("新建(&N)"));
    Menu_File_NewFile->setShortcut(QKeySequence("Ctrl+N"));
    QAction* Menu_File_OpenFile = new QAction(QStringLiteral("打开(&O)"));
    Menu_File_OpenFile->setShortcut(QKeySequence("Ctrl+O"));
    QAction* Menu_File_Save = new QAction(QStringLiteral("保存(&S)"));
    Menu_File_Save->setShortcut(QKeySequence("Ctrl+S"));
    QAction* Menu_File_Rename = new QAction(QStringLiteral("重命名(&R)"));
    QAction* Menu_File_Close = new QAction(QStringLiteral("关闭(&C)"));
    Menu_File_Close->setShortcut(QKeySequence("Ctrl+W"));
    QAction* Menu_File_Exit = new QAction(QStringLiteral("退出(&X)"));
    Menu_File_Exit->setShortcut(QKeySequence("Alt+F4"));

    // 编辑区
    QAction* Menu_Edit_Cut = new QAction(QStringLiteral("剪切(&T)"));
    Menu_Edit_Cut->setShortcut(QKeySequence("Ctrl+X"));
    QAction* Menu_Edit_Copy = new QAction(QStringLiteral("复制(&C)"));
    Menu_Edit_Copy->setShortcut(QKeySequence("Ctrl+C"));
    QAction* Menu_Edit_Paste = new QAction(QStringLiteral("粘贴(&P)"));
    Menu_Edit_Paste->setShortcut(QKeySequence("Ctrl+V"));
    QAction* Menu_Edit_Delete = new QAction(QStringLiteral("删除(&D)"));
    Menu_Edit_Delete->setShortcut(QKeySequence("DEL"));
    QAction* Menu_Edit_SelectAll = new QAction(QStringLiteral("全选(&S)"));
    Menu_Edit_SelectAll->setShortcut(QKeySequence("Ctrl+A"));

    // 工具区
    QAction* Menu_Tool_Search = new QAction(QStringLiteral("查找(&F)"));
    Menu_Tool_Search->setShortcut(QKeySequence("Ctrl+F"));
    QAction* Menu_Tool_Replace = new QAction(QStringLiteral("替换(&H)"));
    Menu_Tool_Replace->setShortcut(QKeySequence("Ctrl+H"));

    // 关于区
    QAction *MenuBar_About_About = new QAction(QStringLiteral("关于(&A)"), this);

    // 将Action加入菜单项
    MenuBar_File->addAction(Menu_File_NewFile);
    MenuBar_File->addAction(Menu_File_OpenFile);
    MenuBar_File->addAction(Menu_File_Save);
    MenuBar_File->addAction(Menu_File_Rename);
    MenuBar_File->addAction(Menu_File_Close);
    MenuBar_File->addSeparator();// 分隔符
    MenuBar_File->addAction(Menu_File_Exit);

    MenuBar_Edit->addAction(Menu_Edit_Cut);
    MenuBar_Edit->addAction(Menu_Edit_Copy);
    MenuBar_Edit->addAction(Menu_Edit_Paste);
    MenuBar_Edit->addAction(Menu_Edit_Delete);
    MenuBar_Edit->addAction(Menu_Edit_SelectAll);

    MenuBar_Tool->addAction(Menu_Tool_Search);
    MenuBar_Tool->addAction(Menu_Tool_Replace);

    MenuBar_About->addAction(MenuBar_About_About);

    // MenuBar_Encoding->addAction(Menu_Encoding_UTF8);
    // MenuBar_Encoding->addAction(Menu_Encoding_ANSI);
    // QTabWidget *tabWidget = new QTabWidget();

    tabWidget->setTabsClosable(true);// Tab选项框，标签页可关闭
    tabWidget->setMovable(true);// Tab选项框，标签页可移动
    this->ui->centralwidget->layout()->addWidget(tabWidget);// 将tabWidget加入到ui中
    this->FileEdit->setTabWidget(tabWidget);// 初始化自定义FileEdit对象
    QTextEdit* textEdit = FileEdit->addFile();// 添加一个默认文件
    if(textEdit != NULL)
        connect(textEdit,&QTextEdit::textChanged, this, &MainWindow::onTextEditTextChanged);// 连接编辑框内容改变的信号
    // FileEditClass *FileEdit = new FileEditClass(tabWidget);// 将tabWidget的指针传递给实例化的对象，便于实例化的对象操作tabWidget（已弃用，不在构造函数中实例化对象，直接继承使用）

    //为菜单Action绑定响应函数
    // Menu_File区
    // 新建文件
    connect(Menu_File_NewFile, &QAction::triggered, [=]() {
        QTextEdit* textEdit = FileEdit->addFile();
        if(textEdit != NULL)
            connect(textEdit,&QTextEdit::textChanged, this, &MainWindow::onTextEditTextChanged);
    });
    //  打开文件
    connect(Menu_File_OpenFile, &QAction::triggered, [=]() {
        QString filePath = QFileDialog::getOpenFileName(this,tr("打开文件"),"",tr("text file (*.txt);;ALL File(*.*)"));
        if(!filePath.isEmpty())
        {
            // 有文件路径，则打开文件
            qDebug()<<"打开文件 - 选择了文件路径："<<filePath;
            QTextEdit* textEdit =  this->FileEdit->openFile(filePath);
            if(textEdit != NULL)
                connect(textEdit,&QTextEdit::textChanged, this, &MainWindow::onTextEditTextChanged);
        }
        // FileEdit->addFile();
    });
    // 保存文件
    connect(Menu_File_Save, &QAction::triggered, [=]() {
        // 先向FileEdit发送保存请求，如果没有文件路径则返回0，则选择文件路径，再发送保存请求，否则返回1直接保存成功
        int currentIndex = tabWidget->currentIndex();
        int saveRes = this->FileEdit->saveFile(currentIndex);// 第一次保存
        qDebug()<<"文件正在保存 index ="<<currentIndex<<",返回值 ="<<saveRes;
        switch(saveRes){
        case -1:
            // 保存失败
            QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("文件保存失败，请检查应用运行权限"));
            break;
        case 1:
            // 保存成功
            break;
        case 0:
            // 没有文件路径，先获取路径，再保存
            // 保存前先获取该文件名，便于后续释放
            QString releaseName = this->FileEdit->getFileNamebyCurrentIndex(currentIndex);
            QString savefileName = QFileDialog::getSaveFileName(this, tr("保存为"), this->FileEdit->getFileNamebyCurrentIndex(currentIndex), tr("text file (*.txt);;ALL File(*.*)"));
            if(!savefileName.isEmpty())
            {
                // 选择了保存位置，可以保存
                QString fileName = this->FileEdit->getFileNamebyCurrentIndex(currentIndex);// 提前保存原来的文件名，便于后续释放
                this->FileEdit->setFilePathbyCurrentIndex(currentIndex,savefileName);
                if(this->FileEdit->saveFile(currentIndex))
                {
                    // 保存成功，释放文件名
                    int releaseId = this->FileEdit->QStringtoIntLimitFileName(fileName);// 如果当前关闭的标签页名称为默认的，则释放该名称
                    if(releaseId != -1)
                        this->FileEdit->removeFileEditTextNameId(releaseId);
                    //this->QStringtoIntLimitFileName(this->pageInfo[InfoIndex].tabPageTitle)
                }
            }
            break;
        }
    });
    // 关闭文件
    connect(Menu_File_Close, &QAction::triggered, [=]() {
        this->getCloseRequested(this->tabWidget->currentIndex());
    });

    // Menu_Edit区
    // 剪切
    connect(Menu_Edit_Cut, &QAction::triggered, [=]() {
        QTextEdit *textEdit = this->FileEdit->getCurrentTextEdit();
        QClipboard *clipboard = QApplication::clipboard();
        QTextCursor cursor = textEdit->textCursor();
        QString str = cursor.selectedText();
        cursor.removeSelectedText();
        if(str.length() == 0)
            QMessageBox::information(nullptr,"提示","无法剪切，因为没有选中内容");
        else
            clipboard->setText(str);
    });
    // 复制
    connect(Menu_Edit_Copy, &QAction::triggered, [=]() {
        QTextEdit *textEdit = this->FileEdit->getCurrentTextEdit();
        QClipboard *clipboard = QApplication::clipboard();
        QTextCursor cursor = textEdit->textCursor();
        QString str = cursor.selectedText();
        // cursor.removeSelectedText(); 相比剪切，无需移除选择的文本
        if(str.length() == 0)
            QMessageBox::information(nullptr,"提示","无法复制，因为没有选中内容");
        else
            clipboard->setText(str);
    });
    // 粘贴
    connect(Menu_Edit_Paste, &QAction::triggered, [=]() {
        QTextEdit *textEdit = this->FileEdit->getCurrentTextEdit();
        QClipboard *clipboard = QApplication::clipboard();
        QString clipboardText = clipboard->text();
        textEdit->insertPlainText(clipboardText);
    });
    // 删除
    connect(Menu_Edit_Delete, &QAction::triggered, [=]() {
        QTextEdit *textEdit = this->FileEdit->getCurrentTextEdit();
        QTextCursor cursor = textEdit->textCursor();
        QString str = cursor.selectedText();
        cursor.removeSelectedText();
        if(str.length() == 0)
            QMessageBox::information(nullptr,"提示","无法删除，因为没有选中内容");
    });
    // 全选
    connect(Menu_Edit_SelectAll, &QAction::triggered, [=]() {
        QTextEdit *textEdit = this->FileEdit->getCurrentTextEdit();
        textEdit->selectAll();
    });


    // Menu_Tool区
    // 查找
    connect(Menu_Tool_Search, &QAction::triggered, [=]() {
        this->createToolDialog("查找");
    });
    // 替换
    connect(Menu_Tool_Replace, &QAction::triggered, [=]() {
        this->createToolDialog("替换");
    });


    //  关于（就这一条）
    connect(MenuBar_About_About, &QAction::triggered, [=]() {
        QMessageBox::about(this, QStringLiteral("关于"), QStringLiteral("该应用采用QT6框架创建，采用C++语言编写\n为课程设计作业（文本编辑器）\n学号尾号：616"));
    });

    // 其他事件
    // 关闭Tab标签
    connect(tabWidget, SIGNAL(tabCloseRequested(int)) , SLOT(getCloseRequested(int)));
    // 当前标签页改变
    connect(tabWidget, SIGNAL(currentChanged(int)), SLOT(getCurrentChanged(int)));
}

// 显示工具对话框（查找和替换）
void MainWindow::createToolDialog(QString title)
{
    ToolDialog toolDialog(this);// 创建对话框对象
    toolDialog.setToolMode(title);// 设定工具对话框的模式
    toolDialog.setCurrentTextEdit(this->FileEdit->getCurrentTextEdit());// 设定当前文本编辑框
    toolDialog.exec();// 显示对话框
}

// Tab标签页被关闭
int MainWindow::getCloseRequested(int value)
{
    // qDebug()<< "选项卡关闭按钮: " << value;
    if(this->FileEdit->closeTabPage(value) == -1)
    {
        // 文件未保存，询问是否保存
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,"提示","文件未保存，是否保存？",QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(reply == QMessageBox::Save){
            // 保存文件
            // this->FileEdit->saveFile(value);
            {
                // 先向FileEdit发送保存请求，如果没有文件路径则返回0，则选择文件路径，再发送保存请求，否则返回1直接保存成功
                // int currentIndex = tabWidget->currentIndex();BUG，已修复
                int currentIndex = value;
                int saveRes = this->FileEdit->saveFile(currentIndex);// 第一次保存
                qDebug()<<"文件正在保存 index ="<<currentIndex<<",返回值 ="<<saveRes;
                switch(saveRes)
                {
                case -1:
                    // 保存失败
                    QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("文件保存失败，请检查应用运行权限"));
                    break;
                case 1:
                    // 保存成功
                    break;
                case 0:
                    // 没有文件路径，先获取路径，再保存
                    // 保存前先获取该文件名，便于后续释放
                    QString releaseName = this->FileEdit->getFileNamebyCurrentIndex(currentIndex);
                    QString savefileName = QFileDialog::getSaveFileName(this, tr("保存为"), this->FileEdit->getFileNamebyCurrentIndex(currentIndex), tr("text file (*.txt);;ALL File(*.*)"));
                    if(!savefileName.isEmpty())
                    {
                        // 选择了保存位置，可以保存
                        QString fileName = this->FileEdit->getFileNamebyCurrentIndex(currentIndex);// 提前保存原来的文件名，便于后续释放
                        this->FileEdit->setFilePathbyCurrentIndex(currentIndex,savefileName);
                        if(this->FileEdit->saveFile(currentIndex))
                        {
                            // 保存成功，释放文件名
                            int releaseId = this->FileEdit->QStringtoIntLimitFileName(fileName);// 如果当前关闭的标签页名称为默认的，则释放该名称
                            if(releaseId != -1)
                                this->FileEdit->removeFileEditTextNameId(releaseId);
                            //this->QStringtoIntLimitFileName(this->pageInfo[InfoIndex].tabPageTitle)
                        }
                    }
                    break;
                }
            }
            this->FileEdit->closeTabPage(value);// 关闭标签页
        }else if(reply == QMessageBox::Discard){
            // 置保存位，继续退出
            this->FileEdit->setSaveFile(value);
            this->FileEdit->closeTabPage(value);
        }else if(reply == QMessageBox::Cancel){
            // 取消保存，则不操作
            return 1;// 取消关闭，停止退出程序
        }
    }
    // 判断是否关闭的最后一个标签页（应至少保留一个标签页）
    if(this->FileEdit->getFileCount() == 0)
    {
        QTextEdit* textEdit = FileEdit->addFile();
        if(textEdit != NULL)
            connect(textEdit,&QTextEdit::textChanged, this, &MainWindow::onTextEditTextChanged);// 将编辑框文本修改事件与槽函数连接，否则会导致没有文件保存提示
    }
    return 0;// 正常关闭
}

// Tab选项框，标签页被移动
void MainWindow::getCurrentChanged(int value)
{
    // qDebug()<< "点击选项卡: " << value;
    this->FileEdit->currentPageChanged(value);
}

// 编辑框内容改变
void MainWindow::onTextEditTextChanged()
{
    this->FileEdit->textEditChanged();
    QTextEdit *textEdit = FileEdit->getCurrentTextEdit();
    int lineCount = this->FileEdit->getLineCountbyCurrentIndex();
    QTextCursor textCursor = textEdit->textCursor();
    int CurrentlineNum = textCursor.blockNumber();
    QString msg = "行数：" + QString::number(lineCount) + "       ";
    msg+="当前 " + QString::number(CurrentlineNum+1) + " 行       ";
    msg+="共 " + QString::number(textEdit->toPlainText().length()) + " 个字符";
    this->ui->statusbar->showMessage(msg);
}

// 窗口关闭事件，关闭所有文件
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"窗口关闭中...";
    this->FileEdit->setCloseStatus(true);
    while(1)
    {
        if(this->FileEdit->getFileCount() == 0)
            break;
        if(this->getCloseRequested(0))
        {
            // 停止退出
            this->FileEdit->setCloseStatus(false);
            qDebug()<<"取消关闭";
            event->ignore();
            return;
        }
    }
    qDebug()<<"窗口成功关闭";
    event->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->FileEdit;
    delete this->tabWidget;
}
