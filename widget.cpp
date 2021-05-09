#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    buffer=new Buffer;
    pgm=new program;
}

Widget::~Widget()
{
    delete pgm;
    delete buffer;
    delete ui;
}

void Widget::showCode()
{
    ui->textEdit_code->setPlaceholderText("");
}

void Widget::dispatchCmd(string &cmd)
{
    if(cmd=="LOAD"||cmd=="load")
    {
        if(!pgm->onDebug)
            on_pushButton_load_clicked();
        return;
    }

    if(cmd=="RUN"||cmd=="run")
    {
        on_pushButton_run_clicked();
        return;
    }

    if(cmd=="CLEAR"||cmd=="clear")
    {
        if(!pgm->onDebug)
            on_pushButton_clear_clicked();
        return;
    }
    if(cmd=="DEBUG"||cmd=="debug"||cmd=="STEP"||cmd=="step")
    {
        on_pushButton_debug_clicked();
    }
    if(cmd=="HELP"||cmd=="help")
    {
        showhelp();
    }
    if(cmd=="quit"||cmd=="QUIT")
    {
        exit(0);
    }
    buffer->dispatchCmd(cmd);
    buffer->printToUi(ui->textEdit_code);
}

void Widget::on_pushButton_load_clicked()
{

    QString path = QFileDialog::getOpenFileName(this,
                                                   tr("LOAD"),
                                                   ".",
                                                   tr("Text Files(*.txt)"));
    if(!path.isEmpty())
    {
        buffer->clear();
        QFile file(path);
        if (!file.open(QIODevice::Text|QIODevice::ReadOnly))
        {
           QMessageBox::warning(this, tr("LOAD"),
                                 tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream in(&file);
        QString qs;
        string s;
        while(!in.atEnd())
        {
            qs=in.readLine();
            s=qs.toStdString();
            dispatchCmd(s);
        }
        buffer->printToUi(ui->textEdit_code);
        file.close();
    }
    else
    {
        QMessageBox::warning(this, tr("Path"),
                             tr("You did not select any file."));
    }
    ui->textEdit_output->clear();
    ui->textEdit_tree->clear();
    ui->textEdit_var->clear();

}

void Widget::on_lineEdit_returnPressed()
{
    if(Statement::waitForInput)
    {
        Statement::waitForInput=false;
        return;
    }

    string cmd=ui->lineEdit->text().toStdString();
    bool flag;
    try
    {
        flag=pgm->doImmCmd(cmd,this);
        if(flag)
            pgm->printVar(this);
    }
    catch (InvalidInput)
    {
        ui->lineEdit->setText("Invalid Input!");
        return;
    }
    catch(InvalidExpression(EXP))
    {
        QString str="Invalid expression '";
        str+=QString::fromStdString(EXP.exp);
        str+="'!";
        ui->lineEdit->setText(str);
        return;
    }
    catch(InvalidIdentify(ident))
    {
        QString str="Invalid identify '";
        str+=QString::fromStdString(ident.var);
        str+="'!";
        ui->lineEdit->setText(str);
        return;
    }
    catch(UnknownIdentify(ident))
    {
        QString str="Unknown Identify '";
        str+=QString::fromStdString(ident.inf);
        str+="'!";
        ui->lineEdit->setText(str);
        return;
    }
    catch(std::out_of_range)
    {
        ui->lineEdit->clear();
        return;
    }

    if(flag)
    {
        ui->lineEdit->clear();
        return;
    }
    dispatchCmd(cmd);
    ui->lineEdit->clear();
}

void Widget::on_pushButton_clear_clicked()
{
    buffer->clear();
    pgm->clear();
    ui->textEdit_code->clear();
    ui->textEdit_output->clear();
    ui->textEdit_tree->clear();
    ui->textEdit_var->clear();
}


void Widget::on_pushButton_run_clicked()
{
    if(pgm->onDebug)
    {
        while(pgm->onDebug)
           on_pushButton_debug_clicked();
        return;
    }
    ui->textEdit_var->clear();
    ui->textEdit_output->clear();
    ui->textEdit_tree->clear();
    pgm->load(buffer,this);
    pgm->run(this);
    pgm->printVar(this);
}

void Widget::showhelp()
{
    QFile file("./help.txt");
    if (!file.open(QIODevice::Text|QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("HELP"),
                                 tr("Cannot open file:\n%1").arg("help.txt"));
        return;
    }
    QTextStream in(&file);
    QString qs;
    while(!in.atEnd())
    {
        qs=in.readLine();
        ui->textEdit_output->append(qs);
    }
    file.close();

}

void Widget::on_pushButton_debug_clicked()
{
    if(!pgm->onDebug)
    {
        ui->textEdit_output->clear();
        ui->textEdit_tree->clear();
        pgm->load(buffer,this);
        ui->textEdit_tree->clear();
        ui->pushButton_clear->setDisabled(true);
        ui->pushButton_load->setDisabled(true);
        pgm->PC=0;
    }
    pgm->debug(this);
    pgm->printVar(this);
    if(!pgm->onDebug)
    {
        ui->pushButton_clear->setDisabled(false);
        ui->pushButton_load->setDisabled(false);
    }
}
