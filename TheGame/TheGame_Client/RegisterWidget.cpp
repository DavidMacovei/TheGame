#include "RegisterWidget.h"
#include "ui_RegisterWidget.h"

#include <QMessageBox>

RegisterWidget::RegisterWidget(PreGameNetworkManager* net, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::RegisterWidget)
    , net(net)
{
    ui->setupUi(this);

    connect(ui->registerButton, &QPushButton::clicked,
        this, &RegisterWidget::onRegisterClicked);

    connect(ui->cancelButton, &QPushButton::clicked,
        this, &RegisterWidget::onCancelClicked);

    connect(net, &PreGameNetworkManager::registerSuccess, this, [=](QString username) {
        QMessageBox::information(this, "Registration Complete",
            "Your account has been created successfully!");
        emit registerSuccess(username);
        });

    connect(net, &PreGameNetworkManager::registerFailed, this, [=](QString msg) {
        QMessageBox::warning(this, "Registration Failed", msg);
        });
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}

void RegisterWidget::onRegisterClicked()
{
    QString username = ui->usernameText->text().trimmed();
    QString password = ui->passwordText->text().trimmed();
    QString confirm = ui->confPassText->text().trimmed();

    if (username.isEmpty() || password.isEmpty() || confirm.isEmpty()) {
        QMessageBox::warning(this, "Missing Fields",
            "Please fill in all fields.");
        return;
    }

    if (password != confirm) {
        QMessageBox::warning(this, "Password Mismatch",
            "Passwords do not match.");
        return;
    }

    if (username.length() < 3) {
        QMessageBox::warning(this, "Invalid Username",
            "Username must be at least 3 characters long.");
        return;
    }
    net->registerUser(username, password);
}

void RegisterWidget::onCancelClicked()
{
    emit backToLogin();
}
