#include <QMessageBox>

#include "InitialUserInfo.h"
#include "UserLoadingWindow.h"


void UserLoadingWindow::handleSubmit() {
    const std::wstring username = m_UI.leUsername->text().toLower().toStdWString();

    accept();

    const int index = User::userIndex(username);
    if (index == -1) {
        QMessageBox::warning(this, "Input Error", "The provided username does not exist in the database.");
    }
    else {
        m_User = index;
    }
}


UserLoadingWindow::UserLoadingWindow(QWidget* parent) : QDialog(parent) {
    m_UI.setupUi(this);

    connect(m_UI.btnLoadUser, &QPushButton::clicked, this, &UserLoadingWindow::handleSubmit);
    connect(m_UI.btnCancel, &QPushButton::clicked, this, [this]() { this->close();  reject(); });
}

int UserLoadingWindow::userIndex() const {
    return m_User;
}