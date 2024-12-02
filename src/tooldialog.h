#ifndef TOOLDIALOG_H
#define TOOLDIALOG_H

#include <QDialog>
#include "QTextEdit"

namespace Ui {
class ToolDialog;
}

class ToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ToolDialog(QWidget *parent = nullptr);
    ~ToolDialog();
    void setToolMode(QString);
    void setCurrentTextEdit(QTextEdit *);

private slots:
    void on_pushButton_Cancal_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_horizontalSlider_valueChanged(int value);

    void on_radioButton_Always_clicked();

    void on_radioButton_Close_clicked();

    void on_pushButton_SearchNext_clicked();

    void on_pushButton_SearchCount_clicked();

    // void on_pushButton_SearchAll_clicked();

    void on_pushButton_Replace_clicked();

    void on_pushButton_ReplaceAll_clicked();

private:
    Ui::ToolDialog *ui;
    QTextEdit *textEdit;
};

#endif // TOOLDIALOG_H
