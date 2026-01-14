#pragma once

#include <QtWidgets/QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "ui_TheGame_Client.h"
#include "LoginWidget.h"
#include "RegisterWidget.h"
#include "LobbyWidget.h"
#include "ProfileWidget.h"
#include "GameBoardWidget.h"
#include "GameOverWidget.h"
#include "PreGameNetworkManager.h"

class TheGame_Client : public QMainWindow
{
    Q_OBJECT

public:
    TheGame_Client(QWidget *parent = nullptr);
    ~TheGame_Client();

    QString getCurrentUsername() const { return m_currentUsername; }
    void setCurrentUsername(const QString& username) { m_currentUsername = username; }

private slots:
    void onLoginSuccess(QString username);
    void onGoToRegister();
    void onRegisterSuccess(QString username);
    void onBackToLogin();
    
    void onViewProfile(QString username);
    
    void onGameStarted(QStringList players);
    void onGameEnded(bool victory, int cardsLeft);
    void onBackToLobby();

    void onNetworkError(QString message);

private:
    Ui::TheGame_ClientClass ui;
    
    QStackedWidget* m_stackedWidget;
    
    LoginWidget* m_loginPage;
    RegisterWidget* m_registerPage;
    LobbyWidget* m_lobbyPage;
    ProfileWidget* m_profilePage;
        GameBoardWidget* m_gamePage;
    GameOverWidget* m_gameOverPage;
    
    PreGameNetworkManager* m_preGameNet;
    
    QString m_currentUsername;
    QStringList m_currentPlayers;
    
    void setupUI();
    void createPages();
    void connectSignals();
    void showConnectionErrorOverlay(const QString& message);
};

