#include "LoginWidget.h"
#include "ui_LoginWidget.h"

#include <QMessageBox>

LoginWidget::LoginWidget(PreGameNetworkManager* net, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
    , net(net)
{
    ui->setupUi(this);

    connect(ui->loginButton, &QPushButton::clicked,
        this, &LoginWidget::onLoginClicked);

    connect(ui->registerButton, &QPushButton::clicked,
        this, &LoginWidget::onRegisterClicked);

    connect(net, &PreGameNetworkManager::loginSuccess, this, [&](QString username) {
        emit loginSuccess(username);
        });

    connect(net, &PreGameNetworkManager::loginFailed, this, [&](QString msg) {
        QMessageBox::warning(this, "Login failed", msg);
        });
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::onLoginClicked()
{
    QString username = ui->usernameText->text().trimmed();
    QString password = ui->registerText->text().trimmed();  

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Missing Fields", "Please enter both username and password.");
        return;
    }

   
    net->login(username, password);
}

void LoginWidget::onRegisterClicked()
{
    emit goToRegister();
}
