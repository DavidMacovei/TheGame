#pragma once

#include <QWidget>
#include <QString>
#include "PreGameNetworkManager.h"

namespace Ui {
    class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(PreGameNetworkManager* net, QWidget* parent = nullptr);
    ~LoginWidget();

signals:
    void loginSuccess(QString username);  
    void goToRegister();                   

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    Ui::LoginWidget* ui;
    PreGameNetworkManager* net;
};
