#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FileEdit.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    FileEditClass *FileEdit;
    QTabWidget *tabWidget;

private slots:
    int getCloseRequested(int value);
    void getCurrentChanged(int value);
    void onTextEditTextChanged();
    void createToolDialog(QString);
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
