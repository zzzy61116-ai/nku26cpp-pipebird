#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QWidget>

class QLabel;
class QComboBox;
class QPushButton;

class MenuPage : public QWidget
{
    Q_OBJECT

public:
    explicit MenuPage(QWidget *parent = nullptr);

    void setBestScore(int score);

signals:
    void startRequested(int difficultyIndex);
    void exitRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUi();
    void applyStyle();

private:
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QLabel *m_bestScoreLabel;
    QComboBox *m_difficultyCombo;
    QPushButton *m_startButton;
    QPushButton *m_exitButton;
};

#endif
