#include "ProfileWidget.h"
#include "ui_ProfileWidget.h"

#include <QMessageBox>

ProfileWidget::ProfileWidget(PreGameNetworkManager* net, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ProfileWidget)
    , net(net)
{
    ui->setupUi(this);

    connect(ui->backButton, &QPushButton::clicked,
        this, &ProfileWidget::onBackClicked);

    connect(net, &PreGameNetworkManager::profileLoaded,
        this, &ProfileWidget::onProfileLoaded);

    connect(net, &PreGameNetworkManager::profileError,
        this, &ProfileWidget::onProfileError);
}

ProfileWidget::~ProfileWidget()
{
    delete ui;
}

void ProfileWidget::loadProfile(const QString& username)
{
    ui->usernameLabel->setText("User: " + username);
    ui->scoreLabel->setText("Score: ...");
    ui->hoursLabel->setText("Hours played: ...");

    net->fetchProfile(username);
}

void ProfileWidget::onBackClicked()
{
    emit backToMenu();
}

void ProfileWidget::onProfileLoaded(const QString& username,
    int score,
    double hours,
    int gamesWon,
    int gamesPlayed)
{
    ui->usernameLabel->setText("User: " + username);
    
    double rating = score / 100.0;
    QString stars = QString("★").repeated(static_cast<int>(rating));
    ui->scoreLabel->setText(QString("Score: %1 / 5.0 %2")
        .arg(rating, 0, 'f', 2)
        .arg(stars));
    
    ui->hoursLabel->setText("Hours played: " + QString::number(hours, 'f', 2));
    
    ui->winsLabel->setText(QString("Wins: %1/%2")
        .arg(gamesWon)
        .arg(gamesPlayed));
}

void ProfileWidget::onProfileError(const QString& msg)
{
    QMessageBox::warning(this, "Profile Error", msg);
}
