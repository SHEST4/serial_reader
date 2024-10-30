#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    find_and_print_ports();
    port_settings = nullptr;
    serial = new QSerialPort(this);
    set_baudrate(baudrate);
    set_COM(available_ports[0].portName());

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionFind_Ports_triggered()
{
    ui->Output->clear();
    find_and_print_ports();
}


void MainWindow::find_and_print_ports()
{
    available_ports = QSerialPortInfo::availablePorts();
    QString str("Available ports:\n");
    ui->Output->insertPlainText(str);
    for (const QSerialPortInfo &available_port_info : available_ports)
    {
        ui->Output->insertPlainText(available_port_info.portName() + " " + available_port_info.description() + "\n");
    }
}

void MainWindow::find_ports()
{
    available_ports = QSerialPortInfo::availablePorts();
}


void MainWindow::on_actionSettings_triggered()
{
    if(!port_settings)
    {
        port_settings = new port_settings_form();
        connect(port_settings, &port_settings_form::settingsUpdated, this, &MainWindow::refresh_settings);
        find_ports();
        port_settings->set_Combobox_Items(MainWindow::available_ports);
    }
    port_settings->show();
}

void MainWindow::set_baudrate(int baudrate)
{
    QString baudrate_str = QString::number(baudrate);
    ui->baudrate->setText(baudrate_str);
}

void MainWindow::set_COM(QString port)
{
    selected_COM.clear();
    selected_COM = port;
    ui->selected_port->setText(port);
}

void MainWindow::refresh_settings()
{
    set_baudrate(port_settings->selected_baudrate);
    set_COM(port_settings->selected_port);
}

void MainWindow::on_actionClear_all_windows_triggered()
{
    ui->Output->clear();
    ui->Input->clear();
}


void MainWindow::on_actionClear_output_window_triggered()
{
    ui->Output->clear();
}


void MainWindow::on_actionClear_input_window_triggered()
{
    ui->Input->clear();
}


void MainWindow::on_actionOpen_triggered()
{
    if(baudrate <= 0 || selected_COM.isEmpty())
    {
        ui->Output->insertPlainText("\nERROR Incorrect baudrate or port name\n");
        return;
    }

    serial->setBaudRate(baudrate);
    serial->setPortName(selected_COM);

    if (serial->open(QIODevice::ReadWrite))
    {
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
        ui->Output->insertPlainText("\nPort opened successfully!\n");
        QByteArray data = serial->readAll();
        ui->Output->insertPlainText(data);
    }
    else
    {
        ui->Output->setText("Failed to open port: "+serial->errorString());
    }
}

void MainWindow::readSerialData()
{
    QByteArray data = serial->readAll();
    QString text = QString::fromUtf8(data);
    int index = text.indexOf('\0');
    if (index != -1)
    {
        text = text.left(index);
    }
    ui->Output->insertPlainText("<<<"+this->selected_COM+"<<<"+text+'\n');
    add_log(text.toUtf8(), false);
    QTextCursor cursor = ui->Output->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->Output->setTextCursor(cursor);
}

void MainWindow::on_actionClose_triggered()
{
    if (serial && serial->isOpen())
    {
        disconnect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
        serial->close();
        ui->Output->insertPlainText("\nStopped reading from port.\n");
    }
}


void MainWindow::on_actionAbout_triggered()
{
    QLabel* label = new QLabel();
    label->setFixedSize(200, 100);
    label->setText("Ver 0.01");
    label->show();
}


void MainWindow::on_Input_textChanged()
{
    input_message.clear();
    input_message = ui->Input->toPlainText();
}


void MainWindow::on_send_button_clicked()
{
    if(input_message.isEmpty())
    {
        ui->Output->insertPlainText("\nInput field is empty!\n");
        return;
    }

    if (!serial || !serial->isOpen())
    {
        ui->Output->insertPlainText("\nCOM port is closed\n");
        return;
    }
    QByteArray data = input_message.toUtf8();
    data.push_back(0x0D);
    data.push_back(0x0A);
    serial->write(data);

    ui->Output->insertPlainText("\n>>>"+this->selected_COM+">>>"+data);
    add_log(data, true);

}

void MainWindow::add_log(QByteArray data, bool t_r)
{
    QDateTime current_datetime = QDateTime::currentDateTime();
    QString hexString;
    ui->Log->insertPlainText("-------------------------------\n");
    if(t_r)
    {
        ui->Log->insertPlainText(current_datetime.toString() + " SEND:\n" + data);
        for (int i = 0; i < data.size(); ++i)
        {
            hexString.append(QString::number(static_cast<unsigned char>(data[i]), 16).rightJustified(2, '0'));
            hexString.append(" ");
        }
        ui->Log->insertPlainText(hexString.toUpper()+"\r\n");
    }
    else
    {
        ui->Log->insertPlainText(current_datetime.toString() + " RECEIVE: " + data + " ");
        for (int i = 0; i < data.size(); ++i)
        {
            hexString.append(QString::number(static_cast<unsigned char>(data[i]), 16).rightJustified(2, '0'));
            hexString.append(" ");
        }
        ui->Log->insertPlainText(hexString.toUpper()+"\r\n");
    }

}

