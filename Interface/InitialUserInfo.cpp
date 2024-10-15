#include <iostream>
#include <QMessageBox>

#include "InitialUserInfo.h"


void InitialUserInfo::handleSubmit() {
    auto xxx = m_UI.leName->text();
    auto nameX = m_UI.leName->text().toUtf8();
    std::string a = "čšž";
    auto nameXX = QString::fromUtf8(a);
    const std::string name = m_UI.leName->text().toLocal8Bit().toStdString();
    bool validGender = m_UI.rbGenderFemale->isChecked() || m_UI.rbGenderMale->isChecked();
    bool validHand = m_UI.rbHandednessLeft->isChecked() || m_UI.rbHandednessRight->isChecked();

    if (name.empty()) {
        QMessageBox::warning(this, "Input Error", "Name cannot be empty. Please enter a valid name.");
        reject();
    }
    else if (!validGender) {
        QMessageBox::warning(this, "Input Error", "Gender cannot be NULL. Please select a valid gender.");
        reject();
    }
    else if (!validHand) {
        QMessageBox::warning(this, "Input Error", "You have at least one hand. Probably. Please select a hand.");
        reject();
    }
    else {
        accept();  // Close the dialog and mark it as accepted
    }
}



InitialUserInfo::InitialUserInfo(QWidget* parent) : QDialog(parent) {
    m_UI.setupUi(this);
    connect(m_UI.okButton, &QPushButton::clicked, this, &InitialUserInfo::handleSubmit);
    connect(m_UI.cancelButton, &QPushButton::clicked, this, [this](){ accept(); });
}

UserData InitialUserInfo::getUserData() const {
    const std::wstring name = m_UI.leName->text().toStdWString();
    const Gender gender = m_UI.rbGenderFemale->isChecked() ? Gender::female : Gender::male;
    const Handedness hand = m_UI.rbHandednessLeft->isChecked() ? Handedness::left : Handedness::right;

    return UserData(name, gender, hand, {});
}
