#include "menupage.h"
#include "difficulty.h"

#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QAbstractItemView>
#include <QListView>
#include <QFontDatabase>

MenuPage::MenuPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    applyStyle();
}

void MenuPage::setupUi()
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(28, 28, 28, 28);
    rootLayout->setSpacing(16);
    rootLayout->addStretch(1);

    auto *panel = new QWidget(this);
    panel->setObjectName("menuPanel");
    panel->setMaximumWidth(380);

    auto *shadow = new QGraphicsDropShadowEffect(panel);
    shadow->setBlurRadius(36);
    shadow->setOffset(0, 10);
    shadow->setColor(QColor(0, 0, 0, 95));
    panel->setGraphicsEffect(shadow);

    auto *panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(24, 26, 24, 22);
    panelLayout->setSpacing(14);

    m_titleLabel = new QLabel("管道鸟 Pro", panel);
    m_titleLabel->setObjectName("titleLabel");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_subtitleLabel = new QLabel("", panel);
    m_subtitleLabel->setObjectName("subtitleLabel");
    m_subtitleLabel->setVisible(false);

    m_bestScoreLabel = new QLabel("最高分：0", panel);
    m_bestScoreLabel->setObjectName("bestScoreLabel");
    m_bestScoreLabel->setAlignment(Qt::AlignCenter);

    auto *difficultyLabel = new QLabel("难度选择", panel);
    difficultyLabel->setObjectName("sectionLabel");

    m_difficultyCombo = new QComboBox(panel);
    m_difficultyCombo->setObjectName("difficultyCombo");
    m_difficultyCombo->setFont(QFont("Microsoft YaHei UI", 11));
    auto *difficultyView = new QListView(m_difficultyCombo);
    difficultyView->setFont(QFont("Microsoft YaHei UI", 11));
    m_difficultyCombo->setView(difficultyView);
    m_difficultyCombo->view()->setStyleSheet(
        "QListView {"
        "  background: white;"
        "  border: 1px solid rgba(40,60,90,90);"
        "  outline: 0;"
        "}"
        "QListView::item {"
        "  min-height: 30px;"
        "  padding: 6px 10px;"
        "  color: #24344A;"
        "}"
        "QListView::item:selected {"
        "  background: #2F7BFF;"
        "  color: white;"
        "}"
    );

    for (const auto &d : difficultyList()) {
        m_difficultyCombo->addItem(d.name);
    }

    m_startButton = new QPushButton("开始游戏", panel);
    m_startButton->setObjectName("primaryButton");
    connect(m_startButton, &QPushButton::clicked, this, [this]() {
        emit startRequested(m_difficultyCombo->currentIndex());
    });

    m_exitButton = new QPushButton("退出", panel);
    m_exitButton->setObjectName("secondaryButton");
    connect(m_exitButton, &QPushButton::clicked, this, &MenuPage::exitRequested);

    auto *buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(10);
    buttonRow->addWidget(m_startButton);
    buttonRow->addWidget(m_exitButton);

    panelLayout->addWidget(m_titleLabel);
    panelLayout->addSpacing(10);
    panelLayout->addWidget(m_bestScoreLabel);
    panelLayout->addWidget(difficultyLabel);
    panelLayout->addWidget(m_difficultyCombo);
    panelLayout->addLayout(buttonRow);

    rootLayout->addWidget(panel, 0, Qt::AlignHCenter);
    rootLayout->addStretch(2);
}

void MenuPage::applyStyle()
{
    setObjectName("menuPage");

    const QFont baseFont("Microsoft YaHei UI", 10);
    setFont(baseFont);

    setStyleSheet(R"(
        QWidget#menuPage {
            background: transparent;
        }
        QWidget#menuPanel {
            background: rgba(255, 255, 255, 180);
            border-radius: 24px;
        }
        QLabel#titleLabel {
            color: white;
            font-size: 32px;
            font-weight: 800;
            letter-spacing: 2px;
            padding-bottom: 4px;
        }
        QLabel#bestScoreLabel {
            color: #29435B;
            font-size: 14px;
            font-weight: 700;
        }
        QLabel#sectionLabel {
            color: #29435B;
            font-size: 12px;
            font-weight: 700;
            padding-top: 4px;
        }
        QComboBox {
            padding: 10px 12px;
            border-radius: 14px;
            border: 1px solid rgba(40,60,90,60);
            background: white;
            color: #24344A;
            font-size: 14px;
        }
        QComboBox:hover {
            border: 1px solid rgba(47,123,255,160);
        }
        QComboBox::drop-down {
            border: 0px;
            width: 28px;
        }
        QComboBox QAbstractItemView {
            background: white;
            color: #24344A;
            selection-background-color: #2F7BFF;
            selection-color: white;
            outline: 0;
        }
        QPushButton#primaryButton {
            min-height: 44px;
            border: none;
            border-radius: 16px;
            background: #2F7BFF;
            color: white;
            font-size: 15px;
            font-weight: 700;
        }
        QPushButton#primaryButton:hover {
            background: #1F68EF;
        }
        QPushButton#secondaryButton {
            min-height: 44px;
            border: 1px solid rgba(40,60,90,70);
            border-radius: 16px;
            background: rgba(255,255,255,0.9);
            color: #29435B;
            font-size: 15px;
            font-weight: 700;
        }
        QPushButton#secondaryButton:hover {
            background: white;
        }
    )");
}

void MenuPage::setBestScore(int score)
{
    m_bestScoreLabel->setText(QString("最高分：%1").arg(score));
}

void MenuPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRect r = rect();

    QLinearGradient sky(0, 0, 0, height());
    sky.setColorAt(0.0, QColor(72, 186, 255));
    sky.setColorAt(0.55, QColor(140, 228, 255));
    sky.setColorAt(1.0, QColor(238, 249, 255));
    painter.fillRect(r, sky);

    painter.setPen(Qt::NoPen);

    // 太阳
    painter.setBrush(QColor(255, 236, 140));
    painter.drawEllipse(QPointF(width() - 85, 90), 58, 58);
    painter.setBrush(QColor(255, 247, 200, 80));
    painter.drawEllipse(QPointF(width() - 85, 90), 90, 90);

    // 云
    painter.setBrush(QColor(255, 255, 255, 190));
    for (int i = 0; i < 4; ++i) {
        int x = 60 + i * 120;
        painter.drawEllipse(QPointF(x, 135), 34, 20);
        painter.drawEllipse(QPointF(x + 28, 122), 44, 28);
        painter.drawEllipse(QPointF(x + 60, 136), 30, 18);
    }

    // 远山
    painter.setBrush(QColor(110, 185, 120));
    QPolygon mountain1;
    mountain1 << QPoint(0, height() - 140)
              << QPoint(width() / 4, height() - 320)
              << QPoint(width() / 2, height() - 140);
    painter.drawPolygon(mountain1);

    QPolygon mountain2;
    mountain2 << QPoint(width() / 3, height() - 140)
              << QPoint(width() * 2 / 3, height() - 350)
              << QPoint(width(), height() - 140);
    painter.drawPolygon(mountain2);

    // 中景光斑
    painter.setBrush(QColor(255, 255, 255, 28));
    for (int i = 0; i < 10; ++i) {
        painter.drawEllipse(QPointF(40 + i * 43, 230 + (i % 3) * 18), 8, 8);
    }

    // 地平线雾气
    painter.setBrush(QColor(255, 255, 255, 45));
    painter.drawRect(0, height() - 200, width(), 70);
}
