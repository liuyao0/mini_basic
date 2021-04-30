#ifndef WIDGET_H
#define WIDGET_H

#include <sstream>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include "program.h"
#include "statement.h"
#include "Buffer.h"
using std::stringstream;
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
class Editor;
class program;
class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    stringstream a;
    Ui::Widget *ui;
    void dispatchCmd(string &cmd);
    void showhelp();
private slots:
    void on_pushButton_load_clicked();
    void on_lineEdit_returnPressed();
    void on_pushButton_clear_clicked();
    void on_pushButton_run_clicked();

private:
    program *pgm;
    Buffer *buffer;
    stringstream changeCmdtoss();
    void showCode();
};
#endif // WIDGET_H
