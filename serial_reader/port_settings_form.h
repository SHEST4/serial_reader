#ifndef PORT_SETTINGS_FORM_H
#define PORT_SETTINGS_FORM_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>


namespace Ui {
class port_settings_form;
}

class port_settings_form : public QWidget
{
    Q_OBJECT

public:
    explicit port_settings_form(QWidget *parent = nullptr);
    ~port_settings_form();

    QString selected_port;
    int selected_baudrate;

    void set_Combobox_Items(QList<QSerialPortInfo> items);
private slots:

    void on_lineEdit_baudrate_textChanged(const QString &arg1);

    void on_OK_Button_settings_form_clicked();
signals:
    void settingsUpdated(int baudrate, QString selectedPort);
private:
    Ui::port_settings_form *ui;

};

#endif // PORT_SETTINGS_FORM_H
