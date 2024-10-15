#pragma once
#include <QCloseEvent>
#include <QDialog>

#include "Structs/Structs.h"
#include "ui_InitialUserInfo.h"


/// <summary>
/// Form for inputting the user info.
/// </summary>
class InitialUserInfo : public QDialog {
    Q_OBJECT

private:
    Ui::InitialUserInfo m_UI;  // Pointer to the UI class
    bool m_IsValid = false;    // Data are valid if true.

private slots:
    /// <summary>
    /// Handling the submit and checking the validity of the data.
    /// </summary>
    void handleSubmit();

public:
    /// <summary>
    /// Constructor of the dialog.
    /// </summary>
    /// <param name="parent">Dialog's parent</param>
    InitialUserInfo(QWidget* parent = nullptr);

    /// <summary>
    /// Destructor of the dialog.
    /// </summary>
    ~InitialUserInfo();

    /// <summary>
    /// Retriever of the user data.
    /// </summary>
    /// <returns>User data</returns>
    UserData getUserData() const;

    /// <summary>
    /// Checking whether the user data is valid.
    /// </summary>
    /// <returns>True if data is valid, false otherwise</returns>
    bool validData() const;
};


namespace User {
    /// <summary>
    /// Checking whether the user with the given username exists in the database.
    /// </summary>
    /// <param name="username">Username</param>
    /// <returns>Index of the user, -1 if it does not exist.</returns>
    int userIndex(const std::wstring& username);
}