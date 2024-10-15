#pragma once
#include <QMainWindow>

#include "ui_EntryWindow.h"


class EntryWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::EntryWindowClass m_UI;  // UI of the window.

public:
    /// <summary>
    /// Constructor of the window.
    /// </summary>
    /// <param name="parent"></param>
    EntryWindow(QWidget* parent = nullptr);

    /// <summary>
    /// Creating a new user profile.
    /// </summary>
    void newUser();
};
