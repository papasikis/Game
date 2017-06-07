#include "menu.h"
#include "ui_menu.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
    this->setGeometry(window()->width()/2, window()->height()/2,
                      geometry().width(), geometry().height());
//    on_pushButton_clicked();
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_pushButton_clicked()
{
    new MainWindow();
    delete this;
}

void Menu::on_pushButton_2_clicked()
{
    exit(0);
}
