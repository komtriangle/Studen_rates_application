#include "input.h"
#include "ui_input.h"

Input::Input(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Input)
{
    ui->setupUi(this);
}

Input::~Input()
{
    delete ui;
}
void Input::on_UserName_textEdited(const QString &arg1)
{
    UserName=arg1;
}

void Input::on_UserPassword_textEdited(const QString &arg1)
{
    UserPassword=arg1;
}

void Input::on_inputButton_clicked()
{
    emit login_button_clicked(UserName, UserPassword);
}
void Input::Display(){
   this->show();
    this->setWindowTitle("Авторизация");
}
