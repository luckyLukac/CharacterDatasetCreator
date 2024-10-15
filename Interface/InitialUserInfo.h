#pragma once
#include <QCloseEvent>
#include <QDialog>

#include "Structs/Structs.h"
#include "ui_InitialUserInfo.h"


/// <summary>
/// Form for inputting the user info.
/// </summary>
class InitialUserInfo  : public QDialog {
    Q_OBJECT

private:
    Ui::InitialUserInfo m_UI;  // Pointer to the UI class

private slots:
    void handleSubmit();

public:
    InitialUserInfo(QWidget* parent = nullptr);
    UserData getUserData() const;
};
