#include "mainwindow.h"
#include "menupage.h"
#include "gamewidget.h"

#include <QStackedWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    buildUi();
    setWindowTitle("管道鸟 Pro");
    setFixedSize(480, 720);
}

void MainWindow::buildUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_stack = new QStackedWidget(this);
    m_menuPage = new MenuPage(this);
    m_gamePage = new GameWidget(this);

    m_stack->addWidget(m_menuPage);
    m_stack->addWidget(m_gamePage);
    layout->addWidget(m_stack);

    m_stack->setCurrentWidget(m_menuPage);

    connect(m_menuPage, &MenuPage::startRequested, this, [this](int difficultyIndex) {
        m_gamePage->startGame(difficultyIndex);
        m_stack->setCurrentWidget(m_gamePage);
        m_gamePage->setFocus();
    });

    connect(m_menuPage, &MenuPage::exitRequested, this, &QWidget::close);

    connect(m_gamePage, &GameWidget::backToMenuRequested, this, [this]() {
        m_menuPage->setBestScore(m_gamePage->bestScore());
        m_stack->setCurrentWidget(m_menuPage);
        m_menuPage->setFocus();
    });

    connect(m_gamePage, &GameWidget::bestScoreChanged, this, [this](int score) {
        m_menuPage->setBestScore(score);
    });

    m_menuPage->setBestScore(m_gamePage->bestScore());
}
