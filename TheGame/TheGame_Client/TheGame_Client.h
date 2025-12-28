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

    // Session data (state management)
    QString getCurrentUsername() const { return m_currentUsername; }
    void setCurrentUsername(const QString& username) { m_currentUsername = username; }

private slots:
    // Navigation handlers
    void onLoginSuccess(QString username);
    void onGoToRegister();
    void onRegisterSuccess(QString username);
    void onBackToLogin();
    
    void onGameStarted(QStringList players);
    void onGameEnded(bool victory, int cardsLeft);
    void onBackToLobby();

    // Connection error overlay
    void onNetworkError(QString message);

private:
    Ui::TheGame_ClientClass ui;
    
    // Navigation
    QStackedWidget* m_stackedWidget;
    
    // Pages
    LoginWidget* m_loginPage;
    RegisterWidget* m_registerPage;
    LobbyWidget* m_lobbyPage;
    ProfileWidget* m_profilePage;
    GameBoardWidget* m_gamePage;
    GameOverWidget* m_gameOverPage;
    
    // Network manager
    PreGameNetworkManager* m_preGameNet;
    
    // Session state
    QString m_currentUsername;
    QStringList m_currentPlayers;
    
    // Helper methods
    void setupUI();
    void createPages();
    void connectSignals();
    void showConnectionErrorOverlay(const QString& message);
};

