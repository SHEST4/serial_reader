#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    find_and_print_ports();
    port_settings = nullptr;
    set_baudrate(0);
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
    if(port_settings == nullptr || port_settings->selected_baudrate <= 0 || port_settings->selected_port.isEmpty())
    {
        ui->Output->insertPlainText("ERROR Incorrect baudrate or port name\n");
        return;
    }
    serial = new QSerialPort(this);
    serial->setBaudRate(port_settings->selected_baudrate);
    serial->setPortName(port_settings->selected_port);

    if (serial->open(QIODevice::ReadWrite))
    {
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
        ui->Output->insertPlainText("Port opened successfully!\n");
        QByteArray data = serial->readAll();
        ui->Output->insertPlainText(data);
    }
    else
    {
        ui->Output->setText("Failed to open port: "+serial->errorString() + "\n");
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
    ui->Output->insertPlainText(text);
    QTextCursor cursor = ui->Output->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->Output->setTextCursor(cursor);
}

void MainWindow::on_actionClose_triggered()
{
    if (serial && serial->isOpen())
    {
        disconnect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
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
    serial->write(data);
    if (!serial->waitForBytesWritten(1000))
    {
        ui->Output->insertPlainText("\nSend OK\n");
    }
    else
    {
        ui->Output->insertPlainText("\nError: " + serial->errorString() + "\n");
    }

}

