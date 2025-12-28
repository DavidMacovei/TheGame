#include "TheGame_Client.h"
#include "RegisterWidget.h"
#include "ProfileWidget.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QGuiApplication>
#include <QScreen>

TheGame_Client::TheGame_Client(QWidget *parent)
    : QMainWindow(parent)
    , m_stackedWidget(nullptr)
    , m_loginPage(nullptr)
    , m_registerPage(nullptr)
    , m_lobbyPage(nullptr)
    , m_profilePage(nullptr)
    , m_gamePage(nullptr)
    , m_gameOverPage(nullptr)
    , m_preGameNet(nullptr)
{
    ui.setupUi(this);
    setupUI();
    createPages();
    connectSignals();
    
    setWindowTitle("The Game");
    
    // Dimensiune inițială mai mică (poate fi redimensionată)
    resize(900, 600);
    
    // Centrează fereastra pe ecran
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            QGuiApplication::primaryScreen()->availableGeometry()
        )
    );
}

TheGame_Client::~TheGame_Client()
{
    if (m_preGameNet) {
        m_preGameNet->stopLobbyPolling();
    }
}

void TheGame_Client::setupUI()
{
    // Create stacked widget for navigation
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);
    
    // Create network manager
    m_preGameNet = new PreGameNetworkManager(this);
}

void TheGame_Client::createPages()
{
    // Page 0: Login
    m_loginPage = new LoginWidget(m_preGameNet, this);
    m_stackedWidget->addWidget(m_loginPage);
    
    // Page 1: Register
    m_registerPage = new RegisterWidget(m_preGameNet, this);
    m_stackedWidget->addWidget(m_registerPage);
    
    // Page 2: Lobby
    m_lobbyPage = new LobbyWidget(m_preGameNet, this);
    m_stackedWidget->addWidget(m_lobbyPage);
    
    // Page 3: Profile (optional)
    m_profilePage = new ProfileWidget(m_preGameNet, this);
    m_stackedWidget->addWidget(m_profilePage);
    
    // Page 4: Game Over
    m_gameOverPage = new GameOverWidget(this);
    m_stackedWidget->addWidget(m_gameOverPage);
    connect(m_gameOverPage, &GameOverWidget::backToLobby, this, &TheGame_Client::onBackToLobby);
    
    // Game page will be created dynamically when game starts
    
    // Start at login
    m_stackedWidget->setCurrentWidget(m_loginPage);
}

void TheGame_Client::connectSignals()
{
    // Login page
    connect(m_loginPage, &LoginWidget::loginSuccess, this, &TheGame_Client::onLoginSuccess);
    connect(m_loginPage, &LoginWidget::goToRegister, this, &TheGame_Client::onGoToRegister);
    
    // Register page
    connect(m_registerPage, &RegisterWidget::registerSuccess, this, &TheGame_Client::onRegisterSuccess);
    connect(m_registerPage, &RegisterWidget::backToLogin, this, &TheGame_Client::onBackToLogin);
    
    // Lobby page
    connect(m_lobbyPage, &LobbyWidget::gameStarted, this, &TheGame_Client::onGameStarted);
    connect(m_lobbyPage, &LobbyWidget::backToLogin, this, &TheGame_Client::onBackToLogin);
    
    // Network errors
    connect(m_preGameNet, &PreGameNetworkManager::networkError, this, &TheGame_Client::onNetworkError);
}

// ============== NAVIGATION SLOTS ==============

void TheGame_Client::onLoginSuccess(QString username)
{
    m_currentUsername = username;
    m_lobbyPage->enterLobby(username);
    m_stackedWidget->setCurrentWidget(m_lobbyPage);
}

void TheGame_Client::onGoToRegister()
{
    m_stackedWidget->setCurrentWidget(m_registerPage);
}

void TheGame_Client::onRegisterSuccess(QString username)
{
    m_currentUsername = username;
    m_lobbyPage->enterLobby(username);
    m_stackedWidget->setCurrentWidget(m_lobbyPage);
}

void TheGame_Client::onBackToLogin()
{
    m_preGameNet->stopLobbyPolling();
    m_currentUsername.clear();
    m_currentPlayers.clear();
    m_stackedWidget->setCurrentWidget(m_loginPage);
}

void TheGame_Client::onGameStarted(QStringList players)
{
    m_currentPlayers = players;
    
    // Create game page dynamically
    if (m_gamePage) {
        m_stackedWidget->removeWidget(m_gamePage);
        delete m_gamePage;
    }
    
    m_gamePage = new GameBoardWidget(m_currentUsername, this);
    m_stackedWidget->addWidget(m_gamePage);
    m_stackedWidget->setCurrentWidget(m_gamePage);
    
    // Connect game end signal (you'll need to add this to GameBoardWidget)
    connect(m_gamePage, &GameBoardWidget::gameEnded, this, &TheGame_Client::onGameEnded);
    
    m_gamePage->start();
}

void TheGame_Client::onGameEnded(bool victory, int cardsLeft)
{
    // Show game over screen
    if (victory) {
        m_gameOverPage->showVictory(98);
    } else {
        m_gameOverPage->showDefeat(cardsLeft, "No more valid moves available");
    }
    
    m_stackedWidget->setCurrentWidget(m_gameOverPage);
}

void TheGame_Client::onBackToLobby()
{
    m_lobbyPage->enterLobby(m_currentUsername);
    m_stackedWidget->setCurrentWidget(m_lobbyPage);
}

// ============== ERROR HANDLING ==============

void TheGame_Client::onNetworkError(QString message)
{
    showConnectionErrorOverlay(message);
}

void TheGame_Client::showConnectionErrorOverlay(const QString& message)
{
    QMessageBox::critical(this, "Connection Error", 
        QString("⚠️ Connection Lost\n\n%1\n\nReturning to login...").arg(message));
    onBackToLogin();
}

