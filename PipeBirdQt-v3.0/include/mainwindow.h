#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QStackedWidget;
class MenuPage;
class GameWidget;

class MainWindow : public QWidget
{

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void buildUi();

private:
    QStackedWidget *m_stack;
    MenuPage *m_menuPage;
    GameWidget *m_gamePage;
};

#endif
