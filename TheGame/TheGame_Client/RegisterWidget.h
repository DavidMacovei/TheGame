#pragma once

#include <QWidget>
#include "PreGameNetworkManager.h"

namespace Ui {
    class RegisterWidget;
}

class RegisterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWidget(PreGameNetworkManager* net, QWidget* parent = nullptr);
    ~RegisterWidget();

signals:
    void goToLogin();

private slots:
    void onRegisterClicked();
    void onCancelClicked();

private:
    Ui::RegisterWidget* ui;
    PreGameNetworkManager* net;
};
