#include <fstream>
#include <QMessageBox>

#include "InitialUserInfo.h"
#include "UserLoadingWindow.h"


void UserLoadingWindow::handleSubmit() {
    const bool addSamples = m_UI.rbAddSamples->isChecked();
    const bool editSamples = m_UI.rbEditSamples->isChecked();
    const std::wstring username = m_UI.leUsername->text().toLower().toStdWString();
    m_AddSamples = false;
    m_EditSamples = false;

    accept();

    if (addSamples) {
        m_AddSamples = true;
    }
    else if (editSamples) {
        m_EditSamples = true;
    }

    const int index = User::userIndex(username);
    if (index == -1) {
        QMessageBox::warning(this, "Input Error", "The provided username does not exist in the database.");
    }
    else {
        std::stringstream ss;
        ss << "./Results/" << index << ".json";
        std::ifstream file(ss.str());
        std::string json_string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        UserData data = json::parse(json_string);

        if (m_EditSamples || data.characters.size() < 200) {
            m_User = index;
        }
        else {
            QMessageBox::warning(this, "Been there, done that.", "You have completed the challenge and have put in all the characters.");
        }
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

std::pair<bool, bool> UserLoadingWindow::addAndEditSamples() const {
    return { m_AddSamples, m_EditSamples };
}
