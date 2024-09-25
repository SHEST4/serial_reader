#ifndef PORT_SETTINGS_H
#define PORT_SETTINGS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class port_settings_form; }
QT_END_NAMESPACE

class port_settings_form : public QMainWindow {
    Q_OBJECT

public:
    explicit port_settings_form(QWidget *parent = nullptr);
    ~port_settings_form();

private:
    Ui::port_settings_form *ui;
};



#endif // PORT_SETTINGS_H
