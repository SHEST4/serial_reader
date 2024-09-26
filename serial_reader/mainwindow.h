#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <QSerialPort>
#include <QSerialPortInfo>
#include "port_settings_form.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void find_and_print_ports();
    void find_ports();
    void set_baudrate(int baudrate);
    void set_COM(QString port);
    void refresh_settings();
    QSerialPort* serial;
private slots:
    void on_actionFind_Ports_triggered();
    void on_actionSettings_triggered();

    void on_actionClear_all_windows_triggered();

    void on_actionClear_output_window_triggered();

    void on_actionClear_input_window_triggered();

    void on_actionOpen_triggered();

    void on_actionClose_triggered();

    void readSerialData();

    void on_actionAbout_triggered();

    void on_Input_textChanged();

    void on_send_button_clicked();

private:
    Ui::MainWindow *ui;
    port_settings_form* port_settings;
    int baudrate;
    QString selected_COM;
    QList<QSerialPortInfo> available_ports;
    QString input_message;
};

#endif // MAINWINDOW_H
