#pragma once

#include <QWidget>
#include "PreGameNetworkManager.h"

namespace Ui {
    class ProfileWidget;
}

class ProfileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileWidget(PreGameNetworkManager* net, QWidget* parent = nullptr);
    ~ProfileWidget();

    void loadProfile(const QString& username);

signals:
    void backToMenu();

private slots:
    void onBackClicked();
    void onProfileLoaded(const QString& username, int score, double hours);
    void onProfileError(const QString& msg);

private:
    Ui::ProfileWidget* ui;
    PreGameNetworkManager* net;
};
