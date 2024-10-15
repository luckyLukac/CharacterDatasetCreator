#pragma once
#include <QDialog>

#include "ui_UserLoadingWindow.h"


class UserLoadingWindow : public QDialog {
    Q_OBJECT

private:
    Ui::UserLoadingWindowClass m_UI;  // UI of the dialog.
    int m_User = -1;                 // The provided user's index.

private slots:
    /// <summary>
    /// Handling the submit and checking if the user exists.
    /// </summary>
    void handleSubmit();

public:
    /// <summary>
    /// Window constructor.
    /// </summary>
    /// <param name="parent">Parent</param>
    UserLoadingWindow(QWidget* parent = nullptr);

    /// <summary>
    /// Retriever of the user index.
    /// </summary>
    /// <returns>User index, -1 if not existing</returns>
    int userIndex() const;
};
