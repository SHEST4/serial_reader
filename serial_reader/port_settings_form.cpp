#include "port_settings_form.h"
#include "ui_port_settings_form.h"

#include "mainwindow.h"

port_settings_form::port_settings_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::port_settings_form)
{
    ui->setupUi(this);

}

port_settings_form::~port_settings_form()
{
    delete ui;
}


void port_settings_form::set_Combobox_Items(QList<QSerialPortInfo> items)
{
    for(const QSerialPortInfo &item : items)
    {
        ui->comboBox_COM->addItem(item.portName());
    }
}

void port_settings_form::on_lineEdit_baudrate_textChanged(const QString &arg1)
{
    port_settings_form::selected_baudrate = arg1.toInt();
}

void port_settings_form::on_OK_Button_settings_form_clicked()
{
    selected_port = ui->comboBox_COM->currentText();
    emit settingsUpdated(selected_baudrate, selected_port);
    port_settings_form::close();
}

